/*
** Copyright (c) 2022 LunarG, Inc.
** Copyright (c) 2022-2023 Advanced Micro Devices, Inc. All rights reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the "Software"),
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, sublicense,
** and/or sell copies of the Software, and to permit persons to whom the
** Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
*/

#include "dx12_optimize_util.h"
#include "block_skipping_file_processor.h"
#include "dx12_resource_value_tracking_consumer.h"
#include "util/logging.h"

#include "dx12_file_optimizer.h"
#include "decode/dx12_object_info.h"
#include "generated/generated_dx12_replay_consumer.h"
#include "decode/dx12_experimental_resource_value_tracker.h"
#include "decode/dx12_resource_value_tracker.h"
#include "decode/file_processor.h"

#ifdef GFXRECON_AGS_SUPPORT
#include "decode/custom_ags_consumer_base.h"
#include "decode/custom_ags_decoder.h"
#include "decode/custom_ags_replay_consumer.h"
#endif // GFXRECON_AGS_SUPPORT

#include <algorithm>
#include <cinttypes>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

GFXRECON_BEGIN_NAMESPACE(gfxrecon)

struct Dx12OptimizationInfo
{
    // PSO removal
    std::unordered_set<uint64_t>         unreferenced_blocks;
    decode::UnreferencedPsoCreationCalls calls_info{};

    // Resource removal
    std::unordered_set<format::HandleId> unreferenced_resources;

    // DXR optimization
    decode::Dx12FillCommandResourceValueMap  fill_command_resource_values;
    decode::Dx12UnassociatedResourceValueMap unassociated_resource_values;

    // DXR/EI perturbation-verification inputs collected from the second experimental pass.
    std::vector<decode::Dx12ScanHitCandidate>                             scan_hits;
    std::map<format::HandleId, decode::Dx12CaptureAllocation>             needle_allocations;
    std::unordered_map<uint64_t, format::HandleId>                        unresolved_gpu_vas;
    decode::Dx12ResourceValueTrackingConsumer::NonRvExecuteIndirectRanges non_rv_ei_ranges;

    bool found_opt_fill_mem{ false };
    bool inject_noop_resource_value_optimization{ false };
};

void CreateResourceValueTrackingConsumer(
    decode::FileProcessor*                                      file_processor,
    std::unique_ptr<decode::Dx12ResourceValueTrackingConsumer>& dx12_replay_consumer,
    std::shared_ptr<application::Application>&                  application,
    const decode::Dx12OptimizationOptions&                      options,
    bool                                                        replay_resource_value_calls)
{
    std::string app_string = "GFXReconstruct Optimizer - analyzing file";
    if (options.optimize_resource_values_experimental)
    {
        app_string += ", image may appear corrupt";
    }

    // Dx12ReplayConsumer requires a windowed application.
    application = std::make_shared<gfxrecon::application::Application>(app_string, file_processor);
    application->InitializeDx12WsiContext();

    // Use default replay options, except gpu index.
    decode::DxReplayOptions dx_replay_options;
    dx_replay_options.override_gpu_index = options.override_gpu_index;

    // Create the replay consumer.
    dx12_replay_consumer = std::make_unique<decode::Dx12ResourceValueTrackingConsumer>(
        application, dx_replay_options, options.optimize_resource_values_experimental);
    if (options.optimize_resource_values_experimental && !replay_resource_value_calls)
    {
        dx12_replay_consumer->EnableReplayOfResourceValueCalls(false);
    }
}

bool FileProcessorSucceeded(const decode::FileProcessor& processor)
{
    if ((processor.GetErrorState() == gfxrecon::decode::BlockIOError::kErrorNone) == false)
    {
        GFXRECON_WRITE_CONSOLE("Encountered error while reading the capture.");
    }

    if ((processor.EntireFileWasProcessed()) == false)
    {
        GFXRECON_WRITE_CONSOLE("Did not reach the end of the capture.");
    }

    return (processor.GetErrorState() == gfxrecon::decode::BlockIOError::kErrorNone) &&
           processor.EntireFileWasProcessed();
}

// Sets info.found_opt_fill_mem and info.inject_noop_resource_value_optimization and returns info.found_opt_fill_mem
bool BypassResourceValueOptimization(const gfxrecon::decode::Dx12Consumer&  dx12_consumer,
                                     const decode::Dx12OptimizationOptions& options,
                                     Dx12OptimizationInfo&                  info)
{
    bool contains_resource_value_workload = dx12_consumer.ContainsDxrWorkload() || dx12_consumer.ContainsEiWorkload();
    info.found_opt_fill_mem               = dx12_consumer.ContainsOptFillMem();

    if (info.found_opt_fill_mem)
    {
        if (options.optimize_resource_values)
        {
            GFXRECON_WRITE_CONSOLE("Bypassing DXR/ExecuteIndirect optimization. Capture file has already been "
                                   "optimized for DXR/EI replay.");
        }
    }
    else if (!contains_resource_value_workload)
    {
        info.inject_noop_resource_value_optimization = true;
    }

    return info.found_opt_fill_mem;
}

bool GetUnreferencedObjectOptimizationInfo(const std::string&               input_filename,
                                           decode::Dx12OptimizationOptions& options,
                                           Dx12OptimizationInfo&            info)
{
    bool ref_scan_result = false;

    decode::FileProcessor file_processor;
    if (file_processor.Initialize(input_filename))
    {
        gfxrecon::decode::Dx12Decoder                    decoder;
        gfxrecon::decode::Dx12ObjectScanningConsumer     pso_consumer;
        gfxrecon::decode::Dx12ReferencedResourceConsumer resource_consumer;

        GFXRECON_WRITE_CONSOLE("Scanning D3D12 capture %s for unreferenced objects.", input_filename.c_str());
        if (options.remove_redundant_psos)
            decoder.AddConsumer(&pso_consumer);
        if (options.remove_redundant_resources)
            decoder.AddConsumer(&resource_consumer);
        file_processor.AddDecoder(&decoder);
        file_processor.ProcessAllFrames();
        if (FileProcessorSucceeded(file_processor))
        {
            pso_consumer.GetUnreferencedObjectCreationBlocks(&info.unreferenced_blocks, &info.calls_info);
            resource_consumer.GetReferencedResourceIds(nullptr, &info.unreferenced_resources);
            GFXRECON_WRITE_CONSOLE("Finished scanning capture file for unreferenced objects.");

            ref_scan_result = true;

            if (BypassResourceValueOptimization(pso_consumer, options, info) == true)
            {
                options.optimize_resource_values = false;
            }
        }
        else if (file_processor.GetErrorState() != gfxrecon::decode::BlockIOError::kErrorNone)
        {
            GFXRECON_WRITE_CONSOLE("A failure has occurred during scanning capture file for unreferenced objects.");
        }
        else if (!file_processor.EntireFileWasProcessed())
        {
            GFXRECON_WRITE_CONSOLE("Failed to process the entire capture file for unreferenced objects.");
        }
        else
        {
            GFXRECON_WRITE_CONSOLE("Unreferenced object removal optimization detected invalid capture. Please ensure "
                                   "that traces input to the optimizer already replay on their own.");
        }
    }

    return ref_scan_result;
}

void WriteDxrAuditCounts(const char* type_name, const decode::Dx12ResourceValueAuditCounts& counts)
{
    if (counts.total == 0)
    {
        return;
    }
    GFXRECON_WRITE_CONSOLE("  %s: %" PRIu64 " observed = %" PRIu64 " walk-attributed + %" PRIu64
                           " candidate-covered + %" PRIu64 " dead capture VA + %" PRIu64 " unresolved",
                           type_name,
                           counts.total,
                           counts.satisfied_by_walk,
                           counts.covered_by_candidate,
                           counts.dead_capture_va,
                           counts.unresolved);
    GFXRECON_WRITE_CONSOLE("  %s: post-execution readback checked %" PRIu64 " value(s), %" PRIu64
                           " changed during the submission that consumed them%s",
                           type_name,
                           counts.post_exec_checked,
                           counts.post_exec_changed,
                           (counts.post_exec_changed > 0) ? "; their recorded identity may be stale" : "");
}

// Report only; the emitted annotations are unchanged.
void WriteDxrAuditReport(const decode::Dx12ResourceValueAuditSummary& summary)
{
    uint64_t total_observed = summary.gpu_va.total + summary.descriptor_handle.total + summary.shader_id.total;

    GFXRECON_WRITE_CONSOLE("DXR/EI value audit (report only, annotations unchanged):");
    if (total_observed == 0)
    {
        GFXRECON_WRITE_CONSOLE("  No resource values were observed at use sites.");
        return;
    }

    WriteDxrAuditCounts("GPU VAs", summary.gpu_va);
    WriteDxrAuditCounts("descriptor handles", summary.descriptor_handle);
    WriteDxrAuditCounts("shader IDs", summary.shader_id);

    if (summary.gpu_va.dead_capture_va > 0)
    {
        GFXRECON_WRITE_CONSOLE("  Dead capture VAs reference %" PRIu64
                               " distinct freed address(es); replay leaves these unchanged and no annotation "
                               "can cover them.",
                               summary.distinct_dead_capture_vas);
    }

    uint64_t total_unresolved =
        summary.gpu_va.unresolved + summary.descriptor_handle.unresolved + summary.shader_id.unresolved;
    if (total_unresolved == 0)
    {
        GFXRECON_WRITE_CONSOLE("  All observed values are covered by walk attribution or content-scan candidates. "
                               "Candidate coverage is a content match only and is not yet verified.");
    }
    else
    {
        GFXRECON_WRITE_CONSOLE("  UNRESOLVED: %" PRIu64 " value observation(s) (%" PRIu64 " distinct GPU VAs, %" PRIu64
                               " distinct shader IDs) have no walk attribution and no content-scan coverage. "
                               "The optimized file will not map these values.",
                               total_unresolved,
                               summary.distinct_unresolved_gpu_vas,
                               summary.distinct_unresolved_shader_ids);

        // Top unresolved counts by the resource read from (use site) and by the resource pointed into (target).
        const size_t kMaxListedResources = 10;
        auto         write_resource_counts = [](const char*                                 label,
                                        const std::map<format::HandleId, uint64_t>& counts_by_resource) {
            std::vector<std::pair<uint64_t, format::HandleId>> resource_counts;
            for (const auto& resource_pair : counts_by_resource)
            {
                resource_counts.emplace_back(resource_pair.second, resource_pair.first);
            }
            std::sort(resource_counts.rbegin(), resource_counts.rend());
            for (size_t i = 0; (i < resource_counts.size()) && (i < kMaxListedResources); ++i)
            {
                GFXRECON_WRITE_CONSOLE("    %s resource id=%" PRIu64 ": %" PRIu64 " unresolved observation(s)",
                                       label,
                                       resource_counts[i].second,
                                       resource_counts[i].first);
            }
            if (resource_counts.size() > kMaxListedResources)
            {
                GFXRECON_WRITE_CONSOLE("    (%zu additional %s resource(s) not listed)",
                                       resource_counts.size() - kMaxListedResources,
                                       label);
            }
        };
        write_resource_counts("use-site", summary.unresolved_by_resource);
        write_resource_counts("target", summary.unresolved_by_target_resource);

        for (const auto& sample : summary.unresolved_gpu_va_samples)
        {
            GFXRECON_WRITE_CONSOLE("    sample VA 0x%" PRIx64 " -> target resource id=%" PRIu64
                                   ", first seen at block %" PRIu64,
                                   sample.value,
                                   sample.target_resource_id,
                                   sample.first_seen_block);
        }
    }
}

bool GetDxrOptimizationInfo(const std::string&               input_filename,
                            Dx12OptimizationInfo&            info,
                            bool                             first_pass,
                            decode::Dx12OptimizationOptions& options)
{
    // If it was already detected that a noop RV block should be injected, exit early.
    if (info.inject_noop_resource_value_optimization)
    {
        return true;
    }

    bool dxr_scan_result = false;

    std::shared_ptr<application::Application> application;
    decode::BlockSkippingFileProcessor        dxr_pass_file_processor;
    if (dxr_pass_file_processor.Initialize(input_filename))
    {
        decode::Dx12Decoder                                        dxr_pass_decoder;
        std::unique_ptr<decode::Dx12ResourceValueTrackingConsumer> resource_value_tracking_consumer = nullptr;

        // The first (track) pass runs with resource value mapping active, so DXR/EI calls execute like a
        // plain --dxr pass; skipping them leaves garbage BVHs that unskippable inline raytracing walks.
        // The later passes do not patch the GPU and keep those calls disabled.
        CreateResourceValueTrackingConsumer(
            &dxr_pass_file_processor, resource_value_tracking_consumer, application, options, first_pass);

        // If this is a second pass, set unassociated resource values on Dx12ResourceValueTracker.
        if (first_pass)
        {
            GFXRECON_WRITE_CONSOLE("Scanning D3D12 capture %s for DXR/EI optimization information.",
                                   input_filename.c_str());
        }
        else
        {
            GFXRECON_WRITE_CONSOLE(
                "Scanning D3D12 file %s another time for additional DXR/EI optimization information.",
                input_filename.c_str());
            resource_value_tracking_consumer->SetUnassociatedResourceValues(
                std::move(info.fill_command_resource_values), std::move(info.unassociated_resource_values));
        }

        // Free up any existing info.
        info.fill_command_resource_values = decode::Dx12FillCommandResourceValueMap();
        info.unassociated_resource_values = decode::Dx12UnassociatedResourceValueMap();

        dxr_pass_decoder.AddConsumer(resource_value_tracking_consumer.get());

        dxr_pass_file_processor.AddDecoder(&dxr_pass_decoder);
        dxr_pass_file_processor.SetBlocksToSkip(info.unreferenced_blocks);

#ifdef GFXRECON_AGS_SUPPORT
        gfxrecon::decode::AgsReplayConsumer ags_replay_consumer;
        gfxrecon::decode::AgsDecoder        ags_decoder;
        ags_replay_consumer.AddDx12Consumer(resource_value_tracking_consumer.get());
        ags_decoder.AddConsumer(reinterpret_cast<gfxrecon::decode::AgsConsumerBase*>(&ags_replay_consumer));

        dxr_pass_file_processor.AddDecoder(&ags_decoder);
#endif // GFXRECON_AGS_SUPPORT

        GFXRECON_ASSERT(application != nullptr);

        application->Run();

        GFXRECON_ASSERT(dxr_pass_file_processor.IsSkippingFinished());

        if (FileProcessorSucceeded(dxr_pass_file_processor))
        {
            resource_value_tracking_consumer->GetTrackedResourceValues(info.fill_command_resource_values);
            resource_value_tracking_consumer->GetUnassociatedResourceValues(info.unassociated_resource_values);

            if (!first_pass)
            {
                decode::Dx12ResourceValueAuditSummary audit_summary;
                resource_value_tracking_consumer->GetResourceValueAuditSummary(audit_summary);
                WriteDxrAuditReport(audit_summary);

                // Export the verification-pass inputs; scan hits are not yet merged into the output annotations.
                resource_value_tracking_consumer->GetResourceValueScanHits(info.scan_hits);
                resource_value_tracking_consumer->GetNeedleAllocations(info.needle_allocations);
                resource_value_tracking_consumer->GetUnresolvedGpuVaValues(info.unresolved_gpu_vas);
                info.non_rv_ei_ranges = resource_value_tracking_consumer->GetNonRvExecuteIndirectRanges();
            }

            if (BypassResourceValueOptimization(*resource_value_tracking_consumer, options, info))
            {
                // No further DXR/EI optimization needed if the file was already optimized.
                options.optimize_resource_values = false;
            }
            else if (info.fill_command_resource_values.empty() && info.unassociated_resource_values.empty() &&
                     info.scan_hits.empty())
            {
                // If the file is not optimized for DXR/EI but does not contain any resource values that need to be
                // mapped during replay, mark it as optimized.
                info.inject_noop_resource_value_optimization = true;
            }

            GFXRECON_WRITE_CONSOLE("Finished scanning capture file for DXR/EI optimization.");

            dxr_scan_result = true;
        }
        else if (dxr_pass_file_processor.GetErrorState() != gfxrecon::decode::BlockIOError::kErrorNone)
        {
            GFXRECON_WRITE_CONSOLE("A failure has occurred during capture processing for DXR/EI optimization");
        }
        else if (!dxr_pass_file_processor.EntireFileWasProcessed())
        {
            GFXRECON_WRITE_CONSOLE("Failed to process the entire capture file for DXR/EI optimization.");
        }
        else
        {
            GFXRECON_WRITE_CONSOLE(
                "DXR/EI optimization detected invalid capture. Please ensure that traces input to the optimizer "
                "already replay on their own.");
        }
    }

    return dxr_scan_result;
}

// Run the perturbation (verification) replay: tagged candidate bytes are patched into fill/init payloads,
// nothing is written to the GPU by the mapper, and every use-site GPU VA observation is decoded against the
// plan. Returns false if the replay did not complete (e.g. a tagged false positive broke it).
bool RunDxrPerturbationPass(const std::string&                                                input_filename,
                            const Dx12OptimizationInfo&                                       info,
                            const decode::Dx12OptimizationOptions&                            options,
                            decode::Dx12ResourceValueTrackingConsumer::PerturbationPatchMap&& patches,
                            decode::Dx12PerturbationPlan&&                                    plan,
                            decode::Dx12PerturbationResults&                                  results)
{
    bool pass_result = false;

    std::shared_ptr<application::Application> application;
    decode::BlockSkippingFileProcessor        file_processor;
    if (file_processor.Initialize(input_filename))
    {
        decode::Dx12Decoder                                        decoder;
        std::unique_ptr<decode::Dx12ResourceValueTrackingConsumer> consumer = nullptr;

        CreateResourceValueTrackingConsumer(&file_processor, consumer, application, options, false);

        GFXRECON_WRITE_CONSOLE("Verifying DXR/EI optimization candidates with a perturbation replay.");
        consumer->SetResourceValuePerturbation(std::move(patches), std::move(plan));

        decoder.AddConsumer(consumer.get());
        file_processor.AddDecoder(&decoder);
        file_processor.SetBlocksToSkip(info.unreferenced_blocks);

#ifdef GFXRECON_AGS_SUPPORT
        gfxrecon::decode::AgsReplayConsumer ags_replay_consumer;
        gfxrecon::decode::AgsDecoder        ags_decoder;
        ags_replay_consumer.AddDx12Consumer(consumer.get());
        ags_decoder.AddConsumer(reinterpret_cast<gfxrecon::decode::AgsConsumerBase*>(&ags_replay_consumer));

        file_processor.AddDecoder(&ags_decoder);
#endif // GFXRECON_AGS_SUPPORT

        GFXRECON_ASSERT(application != nullptr);

        application->Run();

        if (FileProcessorSucceeded(file_processor))
        {
            consumer->GetPerturbationResults(results);
            pass_result = true;
        }
        else
        {
            GFXRECON_WRITE_CONSOLE("The perturbation verification replay did not complete.");
        }
    }

    return pass_result;
}

// Append the flagged scan hits to the output annotations, then sort each block's values by offset and drop
// exact duplicates and overlaps (the walk and the scan can annotate the same location).
void MergeDxrScanHitAnnotations(decode::Dx12FillCommandResourceValueMap&         annotations,
                                const std::vector<decode::Dx12ScanHitCandidate>& hits,
                                const std::vector<bool>&                         emit_flags)
{
    for (size_t i = 0; i < hits.size(); ++i)
    {
        if (emit_flags[i])
        {
            annotations[hits[i].block_index].push_back({ hits[i].offset, hits[i].type });
        }
    }

    for (auto& block_pair : annotations)
    {
        auto& values = block_pair.second;
        std::sort(values.begin(),
                  values.end(),
                  [](const decode::Dx12FillCommandResourceValue& l, const decode::Dx12FillCommandResourceValue& r) {
                      if (l.offset != r.offset)
                      {
                          return l.offset < r.offset;
                      }
                      return static_cast<uint32_t>(l.type) < static_cast<uint32_t>(r.type);
                  });

        std::vector<decode::Dx12FillCommandResourceValue> merged;
        merged.reserve(values.size());
        uint64_t next_free_offset = 0;
        for (const auto& value : values)
        {
            if (!merged.empty() && (value.offset < next_free_offset))
            {
                continue;
            }
            merged.push_back(value);
            next_free_offset = value.offset + decode::GetResourceValueSize(value.type);
        }
        values = std::move(merged);
    }
}

// Verify the second pass's content-scan candidates with a perturbation replay and merge the verdicts into
// the output annotations: GPU VA hits are emitted only when their value was confirmed at a use site
// (directly, or named as the base of a GPU-derived value); other hit types are emitted as found.
void VerifyDxrOptimizationCandidates(const std::string&                     input_filename,
                                     const decode::Dx12OptimizationOptions& options,
                                     Dx12OptimizationInfo&                  info)
{
    const auto& hits = info.scan_hits;
    if (hits.empty())
    {
        return;
    }

    std::vector<bool> emit(hits.size(), false);
    std::vector<bool> excluded(hits.size(), false);
    uint64_t          va_location_count       = 0;
    uint64_t          excluded_location_count = 0;

    for (size_t i = 0; i < hits.size(); ++i)
    {
        if (hits[i].type != decode::ResourceValueType::kGpuVirtualAddress)
        {
            emit[i] = true;
            continue;
        }
        ++va_location_count;

        auto ranges_iter = info.non_rv_ei_ranges.find(hits[i].location_resource_id);
        if (ranges_iter != info.non_rv_ei_ranges.end())
        {
            for (const auto& range : ranges_iter->second)
            {
                if ((hits[i].offset < range.second) && ((hits[i].offset + sizeof(uint64_t)) > range.first))
                {
                    excluded[i] = true;
                    ++excluded_location_count;
                    break;
                }
            }
        }
    }

    // Distinct candidate values grouped by the resource they point into, from non-excluded locations.
    std::map<format::HandleId, std::set<uint64_t>> candidate_sets;
    for (size_t i = 0; i < hits.size(); ++i)
    {
        if ((hits[i].type == decode::ResourceValueType::kGpuVirtualAddress) && !excluded[i])
        {
            candidate_sets[hits[i].target_resource_id].insert(hits[i].value);
        }
    }
    std::map<format::HandleId, std::vector<uint64_t>> tested;
    for (const auto& target_pair : candidate_sets)
    {
        tested[target_pair.first].assign(target_pair.second.begin(), target_pair.second.end());
    }

    std::map<format::HandleId, std::vector<uint64_t>> unresolved_by_target;
    for (const auto& unresolved_pair : info.unresolved_gpu_vas)
    {
        unresolved_by_target[unresolved_pair.second].push_back(unresolved_pair.first);
    }

    decode::Dx12PerturbationResults results;
    std::unordered_set<uint64_t>    tested_values;
    std::unordered_set<uint64_t>    dropped_by_halving;
    uint64_t                        untested_value_count = 0;
    uint64_t                        ambiguous_count      = 0;
    bool                            verified             = false;
    const int                       kMaxAttempts         = 3;
    for (int attempt = 0; (attempt < kMaxAttempts) && !verified; ++attempt)
    {
        decode::Dx12PerturbationPlan plan;
        decode::Dx12ExperimentalResourceValueTracker::BuildPerturbationPlan(
            tested, unresolved_by_target, info.needle_allocations, plan);

        tested_values.clear();
        for (const auto& value_pair : plan.value_to_tagged)
        {
            tested_values.insert(value_pair.first);
        }
        untested_value_count = plan.untested_values.size();
        ambiguous_count      = plan.ambiguous_expectations;

        decode::Dx12ResourceValueTrackingConsumer::PerturbationPatchMap patches;
        for (size_t i = 0; i < hits.size(); ++i)
        {
            if ((hits[i].type != decode::ResourceValueType::kGpuVirtualAddress) || excluded[i])
            {
                continue;
            }
            auto tagged_iter = plan.value_to_tagged.find(hits[i].value);
            if (tagged_iter != plan.value_to_tagged.end())
            {
                patches[hits[i].block_index].push_back({ hits[i].offset, tagged_iter->second });
            }
        }

        decode::Dx12PerturbationResults attempt_results;
        if (RunDxrPerturbationPass(
                input_filename, info, options, std::move(patches), std::move(plan), attempt_results))
        {
            results  = std::move(attempt_results);
            verified = true;
        }
        else if ((attempt + 1) < kMaxAttempts)
        {
            // A tagged false positive broke the replay; halve every target's tested set to contain it. Keep
            // the removed values so the report can account for them: they end the run untested, not refuted.
            GFXRECON_WRITE_CONSOLE("Retrying perturbation verification with half the tested candidates.");
            for (auto& target_pair : tested)
            {
                size_t keep = (target_pair.second.size() + 1) / 2;
                for (size_t i = keep; i < target_pair.second.size(); ++i)
                {
                    dropped_by_halving.insert(target_pair.second[i]);
                }
                target_pair.second.resize(keep);
            }
        }
    }

    if (!verified)
    {
        GFXRECON_WRITE_CONSOLE("WARNING: perturbation verification did not complete; emitting all content-scan "
                               "candidates unverified.");
        for (size_t i = 0; i < hits.size(); ++i)
        {
            if (hits[i].type == decode::ResourceValueType::kGpuVirtualAddress)
            {
                emit[i] = true;
            }
        }
        MergeDxrScanHitAnnotations(info.fill_command_resource_values, hits, emit);
        return;
    }

    uint64_t emitted_va_locations = 0;
    for (size_t i = 0; i < hits.size(); ++i)
    {
        if ((hits[i].type == decode::ResourceValueType::kGpuVirtualAddress) && !excluded[i] &&
            (results.confirmed_values.count(hits[i].value) > 0))
        {
            emit[i] = true;
            ++emitted_va_locations;
        }
    }

    uint64_t refuted_value_count = 0;
    for (auto value : tested_values)
    {
        if (results.confirmed_values.count(value) == 0)
        {
            ++refuted_value_count;
        }
    }

    GFXRECON_WRITE_CONSOLE("DXR/EI perturbation verification:");
    GFXRECON_WRITE_CONSOLE("  tagged %zu candidate value(s); %" PRIu64 " untested (no delta room), %" PRIu64
                           " of %" PRIu64 " VA location(s) excluded (consumed by executed ExecuteIndirect), %" PRIu64
                           " ambiguous expectation(s)",
                           tested_values.size(),
                           untested_value_count,
                           excluded_location_count,
                           va_location_count,
                           ambiguous_count);
    GFXRECON_WRITE_CONSOLE("  decodes at use sites: %" PRIu64 " copied, %" PRIu64 " derived",
                           results.copied_decodes,
                           results.derived_decodes);
    GFXRECON_WRITE_CONSOLE("  confirmed %zu candidate value(s) -> %" PRIu64
                           " VA location(s) emitted; refuted %" PRIu64 " value(s) (tagged, never decoded; dropped)",
                           results.confirmed_values.size(),
                           emitted_va_locations,
                           refuted_value_count);
    if (!dropped_by_halving.empty())
    {
        GFXRECON_WRITE_CONSOLE("  WARNING: %zu candidate value(s) dropped UNTESTED by retry halving; their "
                               "locations are not emitted. Rerunning optimization may recover them.",
                               dropped_by_halving.size());
    }
    GFXRECON_WRITE_CONSOLE("  GPU-derived residue: %zu of %zu distinct unresolved value(s) proven derived from "
                           "tagged bases",
                           results.derived_confirmed_values.size(),
                           info.unresolved_gpu_vas.size());
    if (!results.unverified_unresolved_values.empty())
    {
        GFXRECON_WRITE_CONSOLE("  STILL UNVERIFIED: %zu unresolved value(s) observed unchanged (%" PRIu64
                               " observation(s)); their derivation chains do not root in any tagged location.",
                               results.unverified_unresolved_values.size(),
                               results.unverified_unresolved_observations);
    }
    if (!results.observed_untagged_values.empty())
    {
        GFXRECON_WRITE_CONSOLE("  %zu tagged value(s) also observed untagged at use sites (%" PRIu64
                               " observation(s)); some flows bypass the perturbed locations.",
                               results.observed_untagged_values.size(),
                               results.untagged_candidate_observations);
    }

    MergeDxrScanHitAnnotations(info.fill_command_resource_values, hits, emit);
}

bool GetDx12OptimizationInfo(const std::string&               input_filename,
                             decode::Dx12OptimizationOptions& options,
                             Dx12OptimizationInfo&            info)
{
    bool ref_scan_result = true;
    bool dxr_scan_result = true;

    if (options.remove_redundant_psos || options.remove_redundant_resources)
    {
        ref_scan_result = GetUnreferencedObjectOptimizationInfo(input_filename, options, info);
    }

    if (gfxrecon::graphics::dx12::VerifyAgilitySDKRuntime() == false)
    {
        GFXRECON_LOG_ERROR("Did not find Agility SDK runtimes. Verify \\D3D12\\D3D12Core.dll exists in the same "
                           "directory as gfxrecon-optimize.exe.");
    }

    if (options.optimize_resource_values)
    {
        dxr_scan_result = GetDxrOptimizationInfo(input_filename, info, true, options);

        // If unassocaited resource values were found the resource value tracker must be run again.
        if (options.optimize_resource_values_experimental && (info.unassociated_resource_values.size() > 0))
        {
            GFXRECON_WRITE_CONSOLE(
                "The first pass of experimental DXR/EI optimization was unable to find all required optimization data. "
                "A second pass will attempt to find this data using a brute-force search.");
            dxr_scan_result = dxr_scan_result && GetDxrOptimizationInfo(input_filename, info, false, options);

            if (dxr_scan_result && options.optimize_resource_values)
            {
                VerifyDxrOptimizationCandidates(input_filename, options, info);
            }
        }
    }

    return ref_scan_result || dxr_scan_result;
}

bool ApplyDx12OptimizationInfo(const std::string&                     input_filename,
                               const std::string&                     output_filename,
                               const decode::Dx12OptimizationOptions& options,
                               const Dx12OptimizationInfo&            info)
{
    bool result                  = false;
    bool found_optimization_data = false;

    // Log info about PSO removal.
    if (options.remove_redundant_psos)
    {
        if (!info.unreferenced_blocks.empty())
        {
            found_optimization_data = true;

            const auto& calls_info = info.calls_info;

            GFXRECON_WRITE_CONSOLE("Removing %" PRIu64 " unused PSO related calls.", info.unreferenced_blocks.size());
            if (calls_info.graphics_pso_creation_calls > 0)
            {
                GFXRECON_WRITE_CONSOLE("Removing %" PRIu64 " CreateGraphicsPipelineState() calls.",
                                       calls_info.graphics_pso_creation_calls);
            }
            if (calls_info.compute_pso_creation_calls > 0)
            {
                GFXRECON_WRITE_CONSOLE("Removing %" PRIu64 " CreateComputePipelineState() calls.",
                                       calls_info.compute_pso_creation_calls);
            }
            if (calls_info.storepipeline_calls > 0)
            {
                GFXRECON_WRITE_CONSOLE("Removing %" PRIu64 " StorePipeline() calls.", calls_info.storepipeline_calls);
            }
        }
        else
        {
            GFXRECON_WRITE_CONSOLE("No redundant PSOs detected. Skipping PSO removal optimization.");
        }
    }

    if (options.remove_redundant_resources)
    {
        if (!info.unreferenced_resources.empty())
        {
            found_optimization_data = true;
            GFXRECON_WRITE_CONSOLE("Removing initialization data for %" PRIu64 " unused resources.",
                                   info.unreferenced_resources.size());
        }
        else
        {
            GFXRECON_WRITE_CONSOLE("No redundant resources detected. Skipping resource removal optimization.");
        }
    }

    // Log info about resource value optimization
    if (options.optimize_resource_values)
    {
        if (info.inject_noop_resource_value_optimization)
        {
            found_optimization_data = true;
            GFXRECON_WRITE_CONSOLE("No DXR/EI optimization data found. Marking file as optimized for DXR/EI replay.",
                                   info.fill_command_resource_values.size());
        }
        else if (!info.fill_command_resource_values.empty())
        {
            found_optimization_data = true;
            GFXRECON_WRITE_CONSOLE("Optimizing %zu FillMemoryCommand blocks for DXR/EI replay.",
                                   info.fill_command_resource_values.size());
        }
        else
        {
            GFXRECON_WRITE_CONSOLE("Found no DXR or EI optimization info. Skipping DXR/EI optimization.");
        }
    }

    // Verify that some optimization info was found.
    if (!found_optimization_data)
    {
        GFXRECON_WRITE_CONSOLE("No optimizable data was found. A new capture file will not be created.");
        result = true;
    }
    else
    {
        // Write optimized capture file.
        GFXRECON_WRITE_CONSOLE("Writing optimized file.");

        gfxrecon::Dx12FileOptimizer file_optimizer(info.unreferenced_resources, info.unreferenced_blocks);
        if (file_optimizer.Initialize(input_filename, output_filename, "optimize"))
        {
            file_optimizer.SetFillCommandResourceValues(&info.fill_command_resource_values,
                                                        info.inject_noop_resource_value_optimization);

            file_optimizer.Process();

            if (file_optimizer.GetErrorState() != gfxrecon::decode::BlockIOError::kErrorNone)
            {
                GFXRECON_WRITE_CONSOLE("A failure has occurred during capture processing (error=%d). If it was "
                                       "created, the output file may be invalid.",
                                       file_optimizer.GetErrorState());
            }
            else
            {
                // we expect "resultant_objects == info.unreferenced_blocks.size()".
                // this will prove the two scan passes match.
                uint64_t resultant_objects = file_optimizer.GetNumRemovedBlocks();

                // Check that all fill commands were optimized.
                uint64_t optimized_fill_commands = file_optimizer.GetNumOptimizedFillCommands();
                uint64_t expected_fill_commands  = info.fill_command_resource_values.size();
                if (info.inject_noop_resource_value_optimization)
                {
                    expected_fill_commands = 1;
                }

                if (resultant_objects < info.unreferenced_blocks.size())
                {
                    GFXRECON_WRITE_CONSOLE("Failed to remove all blocks for PSO removal optimization (%" PRIu64
                                           " unoptimized blocks). The output file may be invalid.",
                                           resultant_objects);
                }

                if (optimized_fill_commands != expected_fill_commands)
                {
                    GFXRECON_WRITE_CONSOLE("Failed to optimize all blocks for DXR optimization (%" PRIu64
                                           " unoptimized blocks). The output file may be invalid.",
                                           (expected_fill_commands - optimized_fill_commands));
                }

                result = true;
            }

            GFXRECON_WRITE_CONSOLE("Optimization complete.");
            GFXRECON_WRITE_CONSOLE("\tOriginal file size:  %" PRIu64 " bytes", file_optimizer.GetNumBytesRead());
            GFXRECON_WRITE_CONSOLE("\tOptimized file size: %" PRIu64 " bytes", file_optimizer.GetNumBytesWritten());
        }
    }

    return result;
}

bool Dx12OptimizeFile(std::string input_filename, std::string output_filename, decode::Dx12OptimizationOptions& options)
{
    // Return early if no DX12 optimizations were enabled.
    if (!(options.remove_redundant_psos || options.remove_redundant_resources || options.optimize_resource_values))
    {
        return true;
    }

    // Run a scanning pass to collect the necessary optimization info.
    Dx12OptimizationInfo info;
    bool                 scan_result = GetDx12OptimizationInfo(input_filename, options, info);
    if (scan_result == false)
    {
        GFXRECON_WRITE_CONSOLE("File processing has encountered a fatal error and cannot continue.");
        return false;
    }

    // Use the result of the scanning pass to write an optimized file.
    bool filter_result = ApplyDx12OptimizationInfo(input_filename, output_filename, options, info);
    if (filter_result == false)
    {
        GFXRECON_WRITE_CONSOLE("Failure creating the optimized file.");
        return false;
    }

    return true;
}

GFXRECON_END_NAMESPACE(gfxrecon)
