/*
** Copyright (c) 2018-2020 Valve Corporation
** Copyright (c) 2018-2020 LunarG, Inc.
** Copyright (c) 2019 Advanced Micro Devices, Inc. All rights reserved.
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

#include "encode/capture_settings.h"

#include "util/file_path.h"
#include "util/platform.h"
#include "util/settings_loader.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <limits>
#include <sstream>

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(encode)

// Available settings (upper and lower-case)
// clang-format off
#define CAPTURE_COMPRESSION_TYPE_LOWER      "capture_compression_type"
#define CAPTURE_COMPRESSION_TYPE_UPPER      "CAPTURE_COMPRESSION_TYPE"
#define CAPTURE_FILE_NAME_LOWER             "capture_file"
#define CAPTURE_FILE_NAME_UPPER             "CAPTURE_FILE"
#define CAPTURE_FILE_USE_TIMESTAMP_LOWER    "capture_file_timestamp"
#define CAPTURE_FILE_USE_TIMESTAMP_UPPER    "CAPTURE_FILE_TIMESTAMP"
#define CAPTURE_FILE_FLUSH_LOWER            "capture_file_flush"
#define CAPTURE_FILE_FLUSH_UPPER            "CAPTURE_FILE_FLUSH"
#define LOG_ALLOW_INDENTS_LOWER             "log_allow_indents"
#define LOG_ALLOW_INDENTS_UPPER             "LOG_ALLOW_INDENTS"
#define LOG_BREAK_ON_ERROR_LOWER            "log_break_on_error"
#define LOG_BREAK_ON_ERROR_UPPER            "LOG_BREAK_ON_ERROR"
#define LOG_ERRORS_TO_STDERR_LOWER          "log_errors_to_stderr"
#define LOG_ERRORS_TO_STDERR_UPPER          "LOG_ERRORS_TO_STDERR"
#define LOG_DETAILED_LOWER                  "log_detailed"
#define LOG_DETAILED_UPPER                  "LOG_DETAILED"
#define LOG_FILE_NAME_LOWER                 "log_file"
#define LOG_FILE_NAME_UPPER                 "LOG_FILE"
#define LOG_FILE_CREATE_NEW_LOWER           "log_file_create_new"
#define LOG_FILE_CREATE_NEW_UPPER           "LOG_FILE_CREATE_NEW"
#define LOG_FILE_FLUSH_AFTER_WRITE_LOWER    "log_file_flush_after_write"
#define LOG_FILE_FLUSH_AFTER_WRITE_UPPER    "LOG_FILE_FLUSH_AFTER_WRITE"
#define LOG_FILE_KEEP_OPEN_LOWER            "log_file_keep_open"
#define LOG_FILE_KEEP_OPEN_UPPER            "LOG_FILE_KEEP_OPEN"
#define LOG_LEVEL_LOWER                     "log_level"
#define LOG_LEVEL_UPPER                     "LOG_LEVEL"
#define LOG_OUTPUT_TO_CONSOLE_LOWER         "log_output_to_console"
#define LOG_OUTPUT_TO_CONSOLE_UPPER         "LOG_OUTPUT_TO_CONSOLE"
#define LOG_OUTPUT_TO_OS_DEBUG_STRING_LOWER "log_output_to_os_debug_string"
#define LOG_OUTPUT_TO_OS_DEBUG_STRING_UPPER "LOG_OUTPUT_TO_OS_DEBUG_STRING"
#define MEMORY_TRACKING_MODE_LOWER          "memory_tracking_mode"
#define MEMORY_TRACKING_MODE_UPPER          "MEMORY_TRACKING_MODE"
#define CAPTURE_FRAMES_LOWER                "capture_frames"
#define CAPTURE_FRAMES_UPPER                "CAPTURE_FRAMES"
#define CAPTURE_TRIGGER_LOWER               "capture_trigger"
#define CAPTURE_TRIGGER_UPPER               "CAPTURE_TRIGGER"
#define PAGE_GUARD_COPY_ON_MAP_LOWER        "page_guard_copy_on_map"
#define PAGE_GUARD_COPY_ON_MAP_UPPER        "PAGE_GUARD_COPY_ON_MAP"
#define PAGE_GUARD_SEPARATE_READ_LOWER      "page_guard_separate_read"
#define PAGE_GUARD_SEPARATE_READ_UPPER      "PAGE_GUARD_SEPARATE_READ"
#define PAGE_GUARD_PERSISTENT_MEMORY_LOWER  "page_guard_persistent_memory"
#define PAGE_GUARD_PERSISTENT_MEMORY_UPPER  "PAGE_GUARD_PERSISTENT_MEMORY"
#define PAGE_GUARD_ALIGN_BUFFER_SIZES_LOWER "page_guard_align_buffer_sizes"
#define PAGE_GUARD_ALIGN_BUFFER_SIZES_UPPER "PAGE_GUARD_ALIGN_BUFFER_SIZES"
#define PAGE_GUARD_TRACK_AHB_MEMORY_LOWER   "page_guard_track_ahb_memory"
#define PAGE_GUARD_TRACK_AHB_MEMORY_UPPER   "PAGE_GUARD_TRACK_AHB_MEMORY"
#define PAGE_GUARD_EXTERNAL_MEMORY_LOWER    "page_guard_external_memory"
#define PAGE_GUARD_EXTERNAL_MEMORY_UPPER    "PAGE_GUARD_EXTERNAL_MEMORY"
// clang-format on

#if defined(__ANDROID__)
// Android Properties
#define GFXRECON_ENV_VAR_PREFIX "debug.gfxrecon."

const char CaptureSettings::kDefaultCaptureFileName[] = "/sdcard/gfxrecon_capture" GFXRECON_FILE_EXTENSION;

const char kCaptureCompressionTypeEnvVar[]    = GFXRECON_ENV_VAR_PREFIX CAPTURE_COMPRESSION_TYPE_LOWER;
const char kCaptureFileFlushEnvVar[]          = GFXRECON_ENV_VAR_PREFIX CAPTURE_FILE_FLUSH_LOWER;
const char kCaptureFileNameEnvVar[]           = GFXRECON_ENV_VAR_PREFIX CAPTURE_FILE_NAME_LOWER;
const char kCaptureFileUseTimestampEnvVar[]   = GFXRECON_ENV_VAR_PREFIX CAPTURE_FILE_USE_TIMESTAMP_LOWER;
const char kLogAllowIndentsEnvVar[]           = GFXRECON_ENV_VAR_PREFIX LOG_ALLOW_INDENTS_LOWER;
const char kLogBreakOnErrorEnvVar[]           = GFXRECON_ENV_VAR_PREFIX LOG_BREAK_ON_ERROR_LOWER;
const char kLogDetailedEnvVar[]               = GFXRECON_ENV_VAR_PREFIX LOG_DETAILED_LOWER;
const char kLogErrorsToStderrEnvVar[]         = GFXRECON_ENV_VAR_PREFIX LOG_ERRORS_TO_STDERR_LOWER;
const char kLogFileNameEnvVar[]               = GFXRECON_ENV_VAR_PREFIX LOG_FILE_NAME_LOWER;
const char kLogFileCreateNewEnvVar[]          = GFXRECON_ENV_VAR_PREFIX LOG_FILE_CREATE_NEW_LOWER;
const char kLogFileFlushAfterWriteEnvVar[]    = GFXRECON_ENV_VAR_PREFIX LOG_FILE_FLUSH_AFTER_WRITE_LOWER;
const char kLogFileKeepFileOpenEnvVar[]       = GFXRECON_ENV_VAR_PREFIX LOG_FILE_KEEP_OPEN_LOWER;
const char kLogLevelEnvVar[]                  = GFXRECON_ENV_VAR_PREFIX LOG_LEVEL_LOWER;
const char kLogOutputToConsoleEnvVar[]        = GFXRECON_ENV_VAR_PREFIX LOG_OUTPUT_TO_CONSOLE_LOWER;
const char kLogOutputToOsDebugStringEnvVar[]  = GFXRECON_ENV_VAR_PREFIX LOG_OUTPUT_TO_OS_DEBUG_STRING_LOWER;
const char kMemoryTrackingModeEnvVar[]        = GFXRECON_ENV_VAR_PREFIX MEMORY_TRACKING_MODE_LOWER;
const char kCaptureFramesEnvVar[]             = GFXRECON_ENV_VAR_PREFIX CAPTURE_FRAMES_LOWER;
const char kCaptureTriggerEnvVar[]            = GFXRECON_ENV_VAR_PREFIX CAPTURE_TRIGGER_LOWER;
const char kPageGuardCopyOnMapEnvVar[]        = GFXRECON_ENV_VAR_PREFIX PAGE_GUARD_COPY_ON_MAP_LOWER;
const char kPageGuardSeparateReadEnvVar[]     = GFXRECON_ENV_VAR_PREFIX PAGE_GUARD_SEPARATE_READ_LOWER;
const char kPageGuardPersistentMemoryEnvVar[] = GFXRECON_ENV_VAR_PREFIX PAGE_GUARD_PERSISTENT_MEMORY_LOWER;
const char kPageGuardAlignBufferSizesEnvVar[] = GFXRECON_ENV_VAR_PREFIX PAGE_GUARD_ALIGN_BUFFER_SIZES_LOWER;
const char kPageGuardTrackAhbMemoryEnvVar[]   = GFXRECON_ENV_VAR_PREFIX PAGE_GUARD_TRACK_AHB_MEMORY_LOWER;
const char kPageGuardExternalMemoryEnvVar[]   = GFXRECON_ENV_VAR_PREFIX PAGE_GUARD_EXTERNAL_MEMORY_LOWER;

#else
// Desktop environment settings
#define GFXRECON_ENV_VAR_PREFIX "GFXRECON_"

const char CaptureSettings::kDefaultCaptureFileName[] = "gfxrecon_capture" GFXRECON_FILE_EXTENSION;

const char kCaptureCompressionTypeEnvVar[]    = GFXRECON_ENV_VAR_PREFIX CAPTURE_COMPRESSION_TYPE_UPPER;
const char kCaptureFileFlushEnvVar[]          = GFXRECON_ENV_VAR_PREFIX CAPTURE_FILE_FLUSH_UPPER;
const char kCaptureFileNameEnvVar[]           = GFXRECON_ENV_VAR_PREFIX CAPTURE_FILE_NAME_UPPER;
const char kCaptureFileUseTimestampEnvVar[]   = GFXRECON_ENV_VAR_PREFIX CAPTURE_FILE_USE_TIMESTAMP_UPPER;
const char kLogAllowIndentsEnvVar[]           = GFXRECON_ENV_VAR_PREFIX LOG_ALLOW_INDENTS_UPPER;
const char kLogBreakOnErrorEnvVar[]           = GFXRECON_ENV_VAR_PREFIX LOG_BREAK_ON_ERROR_UPPER;
const char kLogDetailedEnvVar[]               = GFXRECON_ENV_VAR_PREFIX LOG_DETAILED_UPPER;
const char kLogErrorsToStderrEnvVar[]         = GFXRECON_ENV_VAR_PREFIX LOG_ERRORS_TO_STDERR_UPPER;
const char kLogFileNameEnvVar[]               = GFXRECON_ENV_VAR_PREFIX LOG_FILE_NAME_UPPER;
const char kLogFileCreateNewEnvVar[]          = GFXRECON_ENV_VAR_PREFIX LOG_FILE_CREATE_NEW_UPPER;
const char kLogFileFlushAfterWriteEnvVar[]    = GFXRECON_ENV_VAR_PREFIX LOG_FILE_FLUSH_AFTER_WRITE_UPPER;
const char kLogFileKeepFileOpenEnvVar[]       = GFXRECON_ENV_VAR_PREFIX LOG_FILE_KEEP_OPEN_UPPER;
const char kLogLevelEnvVar[]                  = GFXRECON_ENV_VAR_PREFIX LOG_LEVEL_UPPER;
const char kLogOutputToConsoleEnvVar[]        = GFXRECON_ENV_VAR_PREFIX LOG_OUTPUT_TO_CONSOLE_UPPER;
const char kLogOutputToOsDebugStringEnvVar[]  = GFXRECON_ENV_VAR_PREFIX LOG_OUTPUT_TO_OS_DEBUG_STRING_UPPER;
const char kMemoryTrackingModeEnvVar[]        = GFXRECON_ENV_VAR_PREFIX MEMORY_TRACKING_MODE_UPPER;
const char kCaptureFramesEnvVar[]             = GFXRECON_ENV_VAR_PREFIX CAPTURE_FRAMES_UPPER;
const char kPageGuardCopyOnMapEnvVar[]        = GFXRECON_ENV_VAR_PREFIX PAGE_GUARD_COPY_ON_MAP_UPPER;
const char kPageGuardSeparateReadEnvVar[]     = GFXRECON_ENV_VAR_PREFIX PAGE_GUARD_SEPARATE_READ_UPPER;
const char kPageGuardPersistentMemoryEnvVar[] = GFXRECON_ENV_VAR_PREFIX PAGE_GUARD_PERSISTENT_MEMORY_UPPER;
const char kPageGuardAlignBufferSizesEnvVar[] = GFXRECON_ENV_VAR_PREFIX PAGE_GUARD_ALIGN_BUFFER_SIZES_UPPER;
const char kPageGuardTrackAhbMemoryEnvVar[]   = GFXRECON_ENV_VAR_PREFIX PAGE_GUARD_TRACK_AHB_MEMORY_UPPER;
const char kPageGuardExternalMemoryEnvVar[]   = GFXRECON_ENV_VAR_PREFIX PAGE_GUARD_EXTERNAL_MEMORY_UPPER;
const char kCaptureTriggerEnvVar[]            = GFXRECON_ENV_VAR_PREFIX CAPTURE_TRIGGER_UPPER;
#endif

// Capture options for settings file.
// clang-format off
const char kSettingsFilter[] = "lunarg_gfxreconstruct.";

const std::string kOptionKeyCaptureCompressionType    = std::string(kSettingsFilter) + std::string(CAPTURE_COMPRESSION_TYPE_LOWER);
const std::string kOptionKeyCaptureFile               = std::string(kSettingsFilter) + std::string(CAPTURE_FILE_NAME_LOWER);
const std::string kOptionKeyCaptureFileForceFlush     = std::string(kSettingsFilter) + std::string(CAPTURE_FILE_FLUSH_LOWER);
const std::string kOptionKeyCaptureFileUseTimestamp   = std::string(kSettingsFilter) + std::string(CAPTURE_FILE_USE_TIMESTAMP_LOWER);
const std::string kOptionKeyLogAllowIndents           = std::string(kSettingsFilter) + std::string(LOG_ALLOW_INDENTS_LOWER);
const std::string kOptionKeyLogBreakOnError           = std::string(kSettingsFilter) + std::string(LOG_BREAK_ON_ERROR_LOWER);
const std::string kOptionKeyLogDetailed               = std::string(kSettingsFilter) + std::string(LOG_DETAILED_LOWER);
const std::string kOptionKeyLogErrorsToStderr         = std::string(kSettingsFilter) + std::string(LOG_ERRORS_TO_STDERR_LOWER);
const std::string kOptionKeyLogFile                   = std::string(kSettingsFilter) + std::string(LOG_FILE_NAME_LOWER);
const std::string kOptionKeyLogFileCreateNew          = std::string(kSettingsFilter) + std::string(LOG_FILE_CREATE_NEW_LOWER);
const std::string kOptionKeyLogFileFlushAfterWrite    = std::string(kSettingsFilter) + std::string(LOG_FILE_FLUSH_AFTER_WRITE_LOWER);
const std::string kOptionKeyLogFileKeepOpen           = std::string(kSettingsFilter) + std::string(LOG_FILE_KEEP_OPEN_LOWER);
const std::string kOptionKeyLogLevel                  = std::string(kSettingsFilter) + std::string(LOG_LEVEL_LOWER);
const std::string kOptionKeyLogOutputToConsole        = std::string(kSettingsFilter) + std::string(LOG_OUTPUT_TO_CONSOLE_LOWER);
const std::string kOptionKeyLogOutputToOsDebugString  = std::string(kSettingsFilter) + std::string(LOG_OUTPUT_TO_OS_DEBUG_STRING_LOWER);
const std::string kOptionKeyMemoryTrackingMode        = std::string(kSettingsFilter) + std::string(MEMORY_TRACKING_MODE_LOWER);
const std::string kOptionKeyCaptureFrames             = std::string(kSettingsFilter) + std::string(CAPTURE_FRAMES_LOWER);
const std::string kOptionKeyCaptureTrigger            = std::string(kSettingsFilter) + std::string(CAPTURE_TRIGGER_LOWER);
const std::string kOptionKeyPageGuardCopyOnMap        = std::string(kSettingsFilter) + std::string(PAGE_GUARD_COPY_ON_MAP_LOWER);
const std::string kOptionKeyPageGuardSeparateRead     = std::string(kSettingsFilter) + std::string(PAGE_GUARD_SEPARATE_READ_LOWER);
const std::string kOptionKeyPageGuardPersistentMemory = std::string(kSettingsFilter) + std::string(PAGE_GUARD_PERSISTENT_MEMORY_LOWER);
const std::string kOptionKeyPageGuardAlignBufferSizes = std::string(kSettingsFilter) + std::string(PAGE_GUARD_ALIGN_BUFFER_SIZES_LOWER);
const std::string kOptionKeyPageGuardTrackAhbMemory   = std::string(kSettingsFilter) + std::string(PAGE_GUARD_TRACK_AHB_MEMORY_LOWER);
const std::string kOptionKeyPageGuardExternalMemory   = std::string(kSettingsFilter) + std::string(PAGE_GUARD_EXTERNAL_MEMORY_LOWER);

#if defined(ENABLE_LZ4_COMPRESSION)
const format::CompressionType kDefaultCompressionType = format::CompressionType::kLz4;
#else
const format::CompressionType kDefaultCompressionType = format::CompressionType::kNone;
#endif
// clang-format on

CaptureSettings::CaptureSettings() {}

CaptureSettings::~CaptureSettings() {}

void CaptureSettings::Load()
{
    options_map_.clear();

    LoadOptionsFile();
    LoadOptionsEnvVar();
    ProcessOptions();

    // Valid options are removed as they are read from the OptionsMap.  Therefore, if anything
    // is remaining in it after we're done, it's an invalid setting.
    if (!options_map_.empty())
    {
        for (auto iter = options_map_.begin(); iter != options_map_.end(); ++iter)
        {
            cached_log_messages_.push_back({ util::Log::Severity::kWarningSeverity,
                                             "Settings Loader: Ignoring unrecognized option \"" + iter->first +
                                                 "\" with value \"" + iter->second + "\"" });
        }
    }
}

void CaptureSettings::LogCachedMessages()
{
    for (const CachedLogMessage& msg : cached_log_messages_)
    {
        switch (msg.severity)
        {
            case util::Log::Severity::kDebugSeverity:
                GFXRECON_LOG_DEBUG(msg.message.c_str());
                break;
            case util::Log::Severity::kInfoSeverity:
                GFXRECON_LOG_INFO(msg.message.c_str());
                break;
            case util::Log::Severity::kWarningSeverity:
                GFXRECON_LOG_WARNING(msg.message.c_str());
                break;
            case util::Log::Severity::kErrorSeverity:
                GFXRECON_LOG_ERROR(msg.message.c_str());
                break;
            case util::Log::Severity::kFatalSeverity:
                GFXRECON_LOG_FATAL(msg.message.c_str());
                break;
            default:
                assert(false);
                break;
        }
    }
    cached_log_messages_.clear();
}

void CaptureSettings::LoadSingleOptionEnvVar(const std::string& environment_variable,
                                             const std::string& option_key)
{
    std::string value = util::platform::GetEnv(environment_variable.c_str());
    if (!value.empty())
    {
        std::string entry = util::settings::RemoveQuotes(value);
        cached_log_messages_.push_back(
            { util::Log::Severity::kInfoSeverity,
              "Settings Loader: Found option \"" + environment_variable + "\" with value \"" + entry + "\"" });
        options_map_[option_key] = entry;
    }
}

void CaptureSettings::LoadOptionsEnvVar()
{
    assert(options != nullptr);

    // Capture file environment variables
    LoadSingleOptionEnvVar(kCaptureFileNameEnvVar, kOptionKeyCaptureFile);
    LoadSingleOptionEnvVar(kCaptureFileUseTimestampEnvVar, kOptionKeyCaptureFileUseTimestamp);
    LoadSingleOptionEnvVar(kCaptureCompressionTypeEnvVar, kOptionKeyCaptureCompressionType);
    LoadSingleOptionEnvVar(kCaptureFileFlushEnvVar, kOptionKeyCaptureFileForceFlush);

    // Logging environment variables
    LoadSingleOptionEnvVar(kLogAllowIndentsEnvVar, kOptionKeyLogAllowIndents);
    LoadSingleOptionEnvVar(kLogBreakOnErrorEnvVar, kOptionKeyLogBreakOnError);
    LoadSingleOptionEnvVar(kLogDetailedEnvVar, kOptionKeyLogDetailed);
    LoadSingleOptionEnvVar(kLogErrorsToStderrEnvVar, kOptionKeyLogErrorsToStderr);
    LoadSingleOptionEnvVar(kLogFileNameEnvVar, kOptionKeyLogFile);
    LoadSingleOptionEnvVar(kLogFileCreateNewEnvVar, kOptionKeyLogFileCreateNew);
    LoadSingleOptionEnvVar(kLogFileFlushAfterWriteEnvVar, kOptionKeyLogFileFlushAfterWrite);
    LoadSingleOptionEnvVar(kLogFileKeepFileOpenEnvVar, kOptionKeyLogFileKeepOpen);
    LoadSingleOptionEnvVar(kLogLevelEnvVar, kOptionKeyLogLevel);
    LoadSingleOptionEnvVar(kLogOutputToConsoleEnvVar, kOptionKeyLogOutputToConsole);
    LoadSingleOptionEnvVar(kLogOutputToOsDebugStringEnvVar, kOptionKeyLogOutputToOsDebugString);

    // Memory environment variables
    LoadSingleOptionEnvVar(kMemoryTrackingModeEnvVar, kOptionKeyMemoryTrackingMode);

    // Trimming environment variables
    LoadSingleOptionEnvVar(kCaptureFramesEnvVar, kOptionKeyCaptureFrames);
    LoadSingleOptionEnvVar(kCaptureTriggerEnvVar, kOptionKeyCaptureTrigger);

    // Page guard environment variables
    LoadSingleOptionEnvVar(kPageGuardCopyOnMapEnvVar, kOptionKeyPageGuardCopyOnMap);
    LoadSingleOptionEnvVar(kPageGuardSeparateReadEnvVar, kOptionKeyPageGuardSeparateRead);
    LoadSingleOptionEnvVar(kPageGuardPersistentMemoryEnvVar, kOptionKeyPageGuardPersistentMemory);
    LoadSingleOptionEnvVar(kPageGuardAlignBufferSizesEnvVar, kOptionKeyPageGuardAlignBufferSizes);
    LoadSingleOptionEnvVar(kPageGuardTrackAhbMemoryEnvVar, kOptionKeyPageGuardTrackAhbMemory);
    LoadSingleOptionEnvVar(kPageGuardExternalMemoryEnvVar, kOptionKeyPageGuardExternalMemory);
}

void CaptureSettings::LoadOptionsFile()
{
    assert(options != nullptr);

    std::string settings_filename = util::settings::FindLayerSettingsFile();

    if (!settings_filename.empty())
    {
        cached_log_messages_.push_back(
            { util::Log::Severity::kInfoSeverity, "Found layer settings file: " + settings_filename });

        int32_t result = util::settings::LoadLayerSettingsFile(settings_filename, kSettingsFilter, &options_map_);

        if (result == 0)
        {
            cached_log_messages_.push_back(
                { util::Log::Severity::kInfoSeverity, "Successfully loaded settings from file" });
        }
        else
        {
            cached_log_messages_.push_back(
                { util::Log::Severity::kInfoSeverity,
                  "Failed to load settings from file (errno = " + std::to_string(result) + ")" });
        }
    }
}

void CaptureSettings::ProcessOptions()
{
    // Capture file options
    trace_settings_.capture_file_options.compression_type =
        ParseCompressionTypeString(FindOption(kOptionKeyCaptureCompressionType), kDefaultCompressionType);
    trace_settings_.capture_file =
        FindOption(kOptionKeyCaptureFile, trace_settings_.capture_file);
    trace_settings_.time_stamp_file = ParseBoolString(FindOption(kOptionKeyCaptureFileUseTimestamp),
                                                                trace_settings_.time_stamp_file);
    trace_settings_.force_flush =
        ParseBoolString(FindOption(kOptionKeyCaptureFileForceFlush), trace_settings_.force_flush);

    // Memory tracking options
    trace_settings_.memory_tracking_mode = ParseMemoryTrackingModeString(
        FindOption(kOptionKeyMemoryTrackingMode), trace_settings_.memory_tracking_mode);

    // Trimming options:
    // trim ranges and trim hotkey are exclusive
    // with trim key will be parsed only
    // if trim ranges is empty, else it will be ignored
    ParseTrimRangeString(FindOption(kOptionKeyCaptureFrames), &trace_settings_.trim_ranges);
    std::string trim_key_option = FindOption(kOptionKeyCaptureTrigger);
    if (!trim_key_option.empty())
    {
        if (trace_settings_.trim_ranges.empty())
        {
            trace_settings_.trim_key = ParseTrimKeyString(trim_key_option);
        }
        else
        {
            cached_log_messages_.push_back(
                { util::Log::Severity::kWarningSeverity,
                  "Settings Loader: Ignore trim key setting as trim ranges has been specified." });
        }
    }

    // Page guard environment variables
    trace_settings_.page_guard_copy_on_map = ParseBoolString(
        FindOption(kOptionKeyPageGuardCopyOnMap), trace_settings_.page_guard_copy_on_map);
    trace_settings_.page_guard_separate_read = ParseBoolString(
        FindOption(kOptionKeyPageGuardSeparateRead), trace_settings_.page_guard_separate_read);
    trace_settings_.page_guard_persistent_memory =
        ParseBoolString(FindOption(kOptionKeyPageGuardPersistentMemory),
                        trace_settings_.page_guard_persistent_memory);
    trace_settings_.page_guard_align_buffer_sizes =
        ParseBoolString(FindOption(kOptionKeyPageGuardAlignBufferSizes),
                        trace_settings_.page_guard_align_buffer_sizes);
    trace_settings_.page_guard_track_ahb_memory = ParseBoolString(
        FindOption(kOptionKeyPageGuardTrackAhbMemory), trace_settings_.page_guard_track_ahb_memory);
    trace_settings_.page_guard_external_memory = ParseBoolString(
        FindOption(kOptionKeyPageGuardExternalMemory), trace_settings_.page_guard_external_memory);

    // Log options
    log_settings_.use_indent =
        ParseBoolString(FindOption(kOptionKeyLogAllowIndents), log_settings_.use_indent);
    log_settings_.break_on_error =
        ParseBoolString(FindOption(kOptionKeyLogBreakOnError), log_settings_.break_on_error);
    log_settings_.output_detailed_log_info =
        ParseBoolString(FindOption(kOptionKeyLogDetailed), log_settings_.output_detailed_log_info);
    log_settings_.file_name = FindOption(kOptionKeyLogFile, log_settings_.file_name);
    log_settings_.create_new =
        ParseBoolString(FindOption(kOptionKeyLogFileCreateNew), log_settings_.create_new);
    log_settings_.flush_after_write = ParseBoolString(FindOption(kOptionKeyLogFileFlushAfterWrite),
                                                                log_settings_.flush_after_write);
    log_settings_.leave_file_open =
        ParseBoolString(FindOption(kOptionKeyLogFileKeepOpen), log_settings_.leave_file_open);
    log_settings_.output_errors_to_stderr = ParseBoolString(FindOption(kOptionKeyLogErrorsToStderr),
                                                                      log_settings_.output_errors_to_stderr);
    log_settings_.write_to_console =
        ParseBoolString(FindOption(kOptionKeyLogOutputToConsole), log_settings_.write_to_console);
    log_settings_.output_to_os_debug_string = ParseBoolString(
        FindOption(kOptionKeyLogOutputToOsDebugString), log_settings_.output_to_os_debug_string);
    log_settings_.min_severity =
        ParseLogLevelString(FindOption(kOptionKeyLogLevel), log_settings_.min_severity);
}

std::string CaptureSettings::FindOption(const std::string& key, const std::string& default_value)
{
    assert(options != nullptr);

    std::string result = default_value;

    auto entry = options_map_.find(key);
    if (entry != options_map_.end())
    {
        result = entry->second;
        cached_log_messages_.push_back(
            { util::Log::Severity::kDebugSeverity,
              "Settings Loader: Found option \"" + key + "\" with value \"" + result + "\"" });

        // Erase the option as it comes in so that we should have no valid options remaining in the
        // map when we're done.
        options_map_.erase(key);
    }

    return result;
}

bool CaptureSettings::ParseBoolString(const std::string& value_string, bool default_value)
{
    bool result = default_value;

    // Checking for "false" or zero, or "true" or a non-zero number.
    if ((util::platform::StringCompareNoCase("true", value_string.c_str()) == 0) || (atoi(value_string.c_str()) != 0))
    {
        result = true;
    }
    else if ((util::platform::StringCompareNoCase("false", value_string.c_str()) == 0) || (value_string == "0"))
    {
        result = false;
    }
    else
    {
        if (!value_string.empty())
        {
            cached_log_messages_.push_back(
                { util::Log::Severity::kWarningSeverity,
                  "Settings Loader: Ignoring unrecognized Boolean option value \"" + value_string + "\"" });
        }
    }

    return result;
}

CaptureSettings::MemoryTrackingMode
CaptureSettings::ParseMemoryTrackingModeString(const std::string&                  value_string,
                                               CaptureSettings::MemoryTrackingMode default_value)
{
    CaptureSettings::MemoryTrackingMode result = default_value;

    if (util::platform::StringCompareNoCase("page_guard", value_string.c_str()) == 0)
    {
        result = MemoryTrackingMode::kPageGuard;
    }
    else if (util::platform::StringCompareNoCase("assisted", value_string.c_str()) == 0)
    {
        result = MemoryTrackingMode::kAssisted;
    }
    else if (util::platform::StringCompareNoCase("unassisted", value_string.c_str()) == 0)
    {
        result = MemoryTrackingMode::kUnassisted;
    }
    else
    {
        if (!value_string.empty())
        {
            cached_log_messages_.push_back(
                { util::Log::Severity::kWarningSeverity,
                  "Settings Loader: Ignoring unrecognized memory tracking mode option value \"" + value_string +
                      "\"" });
        }
    }

    return result;
}

format::CompressionType CaptureSettings::ParseCompressionTypeString(const std::string&      value_string,
                                                                    format::CompressionType default_value)
{
    format::CompressionType result = default_value;

    if (util::platform::StringCompareNoCase("none", value_string.c_str()) == 0)
    {
        result = format::CompressionType::kNone;
    }
    else if (util::platform::StringCompareNoCase("lz4", value_string.c_str()) == 0)
    {
        result = format::CompressionType::kLz4;
    }
    else if (util::platform::StringCompareNoCase("zlib", value_string.c_str()) == 0)
    {
        result = format::CompressionType::kZlib;
    }
    else if (util::platform::StringCompareNoCase("zstd", value_string.c_str()) == 0)
    {
        result = format::CompressionType::kZstd;
    }
    else
    {
        if (!value_string.empty())
        {
            cached_log_messages_.push_back(
                { util::Log::Severity::kWarningSeverity,
                  "Settings Loader: Ignoring unrecognized compression type option value \"" + value_string + "\"" });
        }
    }

    return result;
}

util::Log::Severity CaptureSettings::ParseLogLevelString(const std::string&  value_string,
                                                         util::Log::Severity default_value)
{
    util::Log::Severity result;

    if (!util::Log::StringToSeverity(value_string, result))
    {
        result = default_value;
        if (!value_string.empty())
        {
            cached_log_messages_.push_back(
                { util::Log::Severity::kWarningSeverity,
                  "Settings Loader: Ignoring unrecognized log level option value \"" + value_string + "\"" });
        }
    }

    return result;
}

void CaptureSettings::ParseTrimRangeString(const std::string&                       value_string,
                                           std::vector<CaptureSettings::TrimRange>* ranges)
{
    assert(ranges != nullptr);

    if (!value_string.empty())
    {
        std::istringstream value_string_input;
        value_string_input.str(value_string);

        for (std::string range; std::getline(value_string_input, range, ',');)
        {
            if (range.empty() || (std::count(range.begin(), range.end(), '-') > 1))
            {
                cached_log_messages_.push_back(
                    { util::Log::Severity::kWarningSeverity,
                      "Settings Loader: Ignoring invalid capture frame range \"" + range + "\"" });
                continue;
            }

            // Remove whitespace.
            range.erase(std::remove_if(range.begin(), range.end(), ::isspace), range.end());

            // Split string on '-' delimiter.
            bool                     invalid = false;
            std::vector<std::string> values;
            std::istringstream       range_input;
            range_input.str(range);

            for (std::string value; std::getline(range_input, value, '-');)
            {
                if (value.empty())
                {
                    break;
                }

                // Check that the value string only contains numbers.
                size_t count = std::count_if(value.begin(), value.end(), ::isdigit);
                if (count == value.length())
                {
                    values.push_back(value);
                }
                else
                {
                    cached_log_messages_.push_back({ util::Log::Severity::kWarningSeverity,
                                                     "Settings Loader: Ignoring invalid capture frame range \"" +
                                                         range + "\", which contains non-numeric values" });
                    invalid = true;
                    break;
                }
            }

            if (!invalid)
            {
                CaptureSettings::TrimRange trim_range;

                if (values.size() == 1)
                {
                    if (std::count(range.begin(), range.end(), '-') == 0)
                    {
                        trim_range.first = std::stoi(values[0]);
                        trim_range.total = 1;
                    }
                    else
                    {
                        cached_log_messages_.push_back(
                            { util::Log::Severity::kWarningSeverity,
                              "Settings Loader: Ignoring invalid capture frame range \"" + range + "\"" });
                        continue;
                    }
                }
                else if (values.size() == 2)
                {
                    trim_range.first = std::stoi(values[0]);

                    uint32_t last = std::stoi(values[1]);
                    if (last >= trim_range.first)
                    {
                        trim_range.total = (last - trim_range.first) + 1;
                    }
                    else
                    {
                        cached_log_messages_.push_back({
                            util::Log::Severity::kWarningSeverity,
                            "Settings Loader: Ignoring invalid capture frame range \"" + range +
                                "\", where first frame is greater than last frame",
                        });
                        continue;
                    }
                }
                else
                {
                    cached_log_messages_.push_back(
                        { util::Log::Severity::kWarningSeverity,
                          "Settings Loader: Ignoring invalid capture frame range \"" + range + "\"" });
                    continue;
                }

                // Check for invalid start frame of 0.
                if (trim_range.first == 0)
                {
                    cached_log_messages_.push_back({ util::Log::Severity::kWarningSeverity,
                                                     "Settings Loader: Ignoring invalid capture frame range \"" +
                                                         range + "\", with first frame equal to zero" });
                    continue;
                }

                uint32_t next_allowed = 0;

                // Check that start frame is outside the bounds of the previous range.
                if (!ranges->empty())
                {
                    // This produces the number of the frame after the last frame in the range.
                    next_allowed = ranges->back().first + ranges->back().total;
                }

                if (trim_range.first >= next_allowed)
                {
                    ranges->emplace_back(trim_range);
                }
                else
                {
                    cached_log_messages_.push_back(
                        { util::Log::Severity::kWarningSeverity,
                          "Settings Loader: Ignoring invalid capture frame range \"" + range +
                              "\", where start frame precedes the end of the previous range \"" +
                              std::to_string(ranges->back().first) + "-" + std::to_string(next_allowed - 1) + "\"" });
                }
            }
        }
    }
}

std::string CaptureSettings::ParseTrimKeyString(const std::string& value_string)
{
    std::string trim_key;
    if (!value_string.empty())
    {
        trim_key = value_string;
        // Remove whitespace.
        trim_key.erase(std::remove_if(trim_key.begin(), trim_key.end(), ::isspace), trim_key.end());
    }
    else
    {
        cached_log_messages_.push_back({ util::Log::Severity::kWarningSeverity,
                                         "Settings Loader: Ignoring invalid trim trigger key \"" + trim_key + "\"" });
    }
    return trim_key;
}

GFXRECON_END_NAMESPACE(encode)
GFXRECON_END_NAMESPACE(gfxrecon)
