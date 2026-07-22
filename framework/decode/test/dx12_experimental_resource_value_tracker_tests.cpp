/*
** Copyright (c) 2026 LunarG, Inc.
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

#include <catch2/catch.hpp>

#include "decode/dx12_experimental_resource_value_tracker.h"
#include "graphics/dx12_shader_id_map.h"

#include <cstring>
#include <vector>

using namespace gfxrecon;
using namespace gfxrecon::decode;

namespace
{

const format::HandleId kVaResourceId   = 42;
const uint64_t         kCaptureVaStart = 0x100000;
const uint64_t         kVaWidth        = 0x10000;
const uint64_t         kReplayVaStart  = 0x900000;

DxObjectInfo* NullObjectLookup(format::HandleId)
{
    return nullptr;
}

// Shader ID bytes chosen so no aligned 8-byte window inside them looks like an in-range GPU VA.
std::vector<uint8_t> MakeShaderIdBytes(uint8_t tail)
{
    std::vector<uint8_t> id(D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES, 0xAB);
    id[30] = 0xCD;
    id[31] = tail;
    return id;
}

void WriteU64(std::vector<uint8_t>& data, size_t offset, uint64_t value)
{
    memcpy(data.data() + offset, &value, sizeof(value));
}

Dx12UnassociatedResourceValue MakeUnassociatedShaderId(const std::vector<uint8_t>& id_bytes)
{
    Dx12UnassociatedResourceValue value{};
    value.type      = ResourceValueType::kShaderIdentifier;
    value.shader_id = graphics::PackDx12ShaderIdentifier(id_bytes.data());
    return value;
}

Dx12UnassociatedResourceValue MakeUnassociatedGpuVa(format::HandleId resource_id)
{
    Dx12UnassociatedResourceValue value{};
    value.type        = ResourceValueType::kGpuVirtualAddress;
    value.resource_id = resource_id;
    return value;
}

bool HasValue(const std::vector<Dx12FillCommandResourceValue>& values, uint64_t offset, ResourceValueType type)
{
    size_t count = 0;
    for (const auto& value : values)
    {
        if ((value.offset == offset) && (value.type == type))
        {
            ++count;
        }
    }
    return count == 1;
}

} // namespace

TEST_CASE("FindResourceValues finds IDs and unaligned VA values", "[dx12][experimental-tracker]")
{
    uint64_t                             block_index = 1;
    Dx12ExperimentalResourceValueTracker tracker(NullObjectLookup, [&block_index]() { return block_index; });

    // Track mode (default): AddResourceGpuVa establishes the VA range gate.
    tracker.AddResourceGpuVa(kVaResourceId, kReplayVaStart, kVaWidth, kCaptureVaStart);

    graphics::Dx12GpuVaMap va_map;
    va_map.Add(kVaResourceId, kCaptureVaStart, kVaWidth, kReplayVaStart);

    auto id_bytes = MakeShaderIdBytes(0x01);

    std::set<graphics::Dx12ShaderIdentifier> shader_ids;
    shader_ids.insert(graphics::PackDx12ShaderIdentifier(id_bytes.data()));

    std::vector<uint8_t> data(4096, 0);
    memcpy(data.data() + 100, id_bytes.data(), id_bytes.size());
    // A VA value that is only 4-byte aligned: rejected by the old 16-byte value-alignment gate.
    WriteU64(data, 200, kCaptureVaStart + 0x404);
    // A 16-byte aligned VA value.
    WriteU64(data, 300, kCaptureVaStart + 0x800);
    // In-range-looking value that is not in the VA map: below the range gate minimum.
    WriteU64(data, 400, kCaptureVaStart - 0x1000);

    std::vector<std::pair<uint64_t, format::ResourceValueType>> found;
    tracker.FindResourceValues(data.data(), data.size(), 0, &shader_ids, &va_map, nullptr, &found);

    REQUIRE(found.size() == 3);
    CHECK(found[0] == std::make_pair(uint64_t(100), ResourceValueType::kShaderIdentifier));
    CHECK(found[1] == std::make_pair(uint64_t(200), ResourceValueType::kGpuVirtualAddress));
    CHECK(found[2] == std::make_pair(uint64_t(300), ResourceValueType::kGpuVirtualAddress));
}

TEST_CASE("FindResourceValues record_limit bounds starts but not reads", "[dx12][experimental-tracker]")
{
    uint64_t                             block_index = 1;
    Dx12ExperimentalResourceValueTracker tracker(NullObjectLookup, [&block_index]() { return block_index; });
    tracker.AddResourceGpuVa(kVaResourceId, kReplayVaStart, kVaWidth, kCaptureVaStart);

    graphics::Dx12GpuVaMap va_map;
    va_map.Add(kVaResourceId, kCaptureVaStart, kVaWidth, kReplayVaStart);

    auto id_bytes = MakeShaderIdBytes(0x02);

    std::set<graphics::Dx12ShaderIdentifier> shader_ids;
    shader_ids.insert(graphics::PackDx12ShaderIdentifier(id_bytes.data()));

    std::vector<uint8_t> data(256, 0);
    // Starts before the limit, extends past it: must be found.
    memcpy(data.data() + 112, id_bytes.data(), id_bytes.size());
    // Starts after the limit: must not be reported by this scan.
    WriteU64(data, 160, kCaptureVaStart + 0x10);

    std::vector<std::pair<uint64_t, format::ResourceValueType>> found;
    tracker.FindResourceValues(data.data(), data.size(), 128, &shader_ids, &va_map, nullptr, &found);

    REQUIRE(found.size() == 1);
    CHECK(found[0] == std::make_pair(uint64_t(112), ResourceValueType::kShaderIdentifier));
}

TEST_CASE("FindResourceValuesThreaded finds values straddling chunk boundaries", "[dx12][experimental-tracker]")
{
    uint64_t                             block_index = 1;
    Dx12ExperimentalResourceValueTracker tracker(NullObjectLookup, [&block_index]() { return block_index; });

    auto id_bytes = MakeShaderIdBytes(0x03);

    // Enter resolve mode with an ID needle and a VA needle (resource 42).
    Dx12FillCommandResourceValueMap    tracked;
    Dx12UnassociatedResourceValueMap   unassociated;
    Dx12UnassociatedResourceValueGroup group;
    group.block_index = 1;
    group.values.push_back(MakeUnassociatedShaderId(id_bytes));
    group.values.push_back(MakeUnassociatedGpuVa(kVaResourceId));
    unassociated[7].push_back(group);
    tracker.SetUnassociatedResourceValues(std::move(tracked), std::move(unassociated));

    // Resolve mode: registers the active VA map entry and the range gate.
    tracker.AddResourceGpuVa(kVaResourceId, kReplayVaStart, kVaWidth, kCaptureVaStart);

    // 8192 bytes -> 8 threads x 1024-byte chunks with boundaries at multiples of 1024.
    std::vector<uint8_t> data(8192, 0);
    // Shader ID straddling the 1024 boundary (starts 16 bytes before it).
    memcpy(data.data() + 1008, id_bytes.data(), id_bytes.size());
    // Shader ID starting exactly at a boundary: owned by the second chunk, must be found exactly once.
    memcpy(data.data() + 3072, id_bytes.data(), id_bytes.size());
    // GPU VA straddling the 2048 boundary (starts 4 bytes before it).
    WriteU64(data, 2044, kCaptureVaStart + 0x20);

    Dx12ResourceValueTracker::TrackedFillCommandInfo fill_command;
    fill_command.fill_command_block_index = 99;
    fill_command.original_offset          = 1000;
    fill_command.offset                   = 1000;
    fill_command.size                     = data.size();

    tracker.FindResourceValuesThreaded(fill_command, data.data(), data.size());

    Dx12FillCommandResourceValueMap results;
    tracker.GetTrackedResourceValues(results);

    REQUIRE(results.find(99) != results.end());
    const auto& values = results[99];
    CHECK(values.size() == 3);
    CHECK(HasValue(values, 1000 + 1008, ResourceValueType::kShaderIdentifier));
    CHECK(HasValue(values, 1000 + 3072, ResourceValueType::kShaderIdentifier));
    CHECK(HasValue(values, 1000 + 2044, ResourceValueType::kGpuVirtualAddress));
}

TEST_CASE("GetTrackedResourceValues applies exclusions after a range empties a block", "[dx12][experimental-tracker]")
{
    uint64_t                             block_index = 5;
    Dx12ExperimentalResourceValueTracker tracker(NullObjectLookup, [&block_index]() { return block_index; });

    // Seed fill provenance for resource 7 at block 5 (track mode: no object lookups happen).
    std::vector<uint8_t> fill_data(4096, 0);
    tracker.PostProcessFillMemoryCommand(7, 0, fill_data.size(), fill_data.data());

    // Inject tracked values for block 5 and switch to resolve mode.
    Dx12FillCommandResourceValueMap tracked;
    tracked[5] = { { 100, ResourceValueType::kGpuVirtualAddress },
                   { 104, ResourceValueType::kGpuVirtualAddress },
                   { 200, ResourceValueType::kGpuVirtualAddress } };
    Dx12UnassociatedResourceValueMap unassociated;
    tracker.SetUnassociatedResourceValues(std::move(tracked), std::move(unassociated));

    // Non-DXR ranges for the same block. Ranges are iterated in descending order of start offset, so the
    // range that empties the block ([96, 208)) is processed before [0, 8): the erase of the emptied block
    // must not invalidate the iteration over the remaining range.
    tracker.AddNonDxrFillCommandBlocks(7, 96, 208);
    tracker.AddNonDxrFillCommandBlocks(7, 0, 8);

    Dx12FillCommandResourceValueMap results;
    tracker.GetTrackedResourceValues(results);

    CHECK(results.find(5) == results.end());
}
