/*
** Copyright (c) 2018-2022 Valve Corporation
** Copyright (c) 2018-2024 LunarG, Inc.
** Copyright (c) 2019-2023 Advanced Micro Devices, Inc. All rights reserved.
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

#include PROJECT_VERSION_HEADER_FILE

#include "encode/api_capture_manager.h"

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(encode)

uint64_t                      ApiCaptureManager::handle_id_offset_ = 0;
std::vector<format::HandleId> ApiCaptureManager::handle_id_stack_;

void ApiCaptureManager::PushHandleId(const format::HandleId* id)
{
    if (id != nullptr)
    {
        handle_id_stack_.push_back(*id);
    }
}

void ApiCaptureManager::ClearHandleIds()
{
    handle_id_stack_.clear();
}

void ApiCaptureManager::SetHandleIdOffset(format::HandleId offset)
{
    handle_id_offset_ = offset;
}

format::HandleId ApiCaptureManager::GetUniqueId()
{
    uint64_t result = 0;
    if (handle_id_stack_.empty())
    {
        result = CommonCaptureManager::GetUniqueId() + handle_id_offset_;
    }
    else
    {
        if (handle_id_stack_.back() == format::kNullHandleId)
        {
            // Allow replay to push kNullHandleId to force a replay-unique ID.
            result = CommonCaptureManager::GetUniqueId() + handle_id_offset_;
        }
        else
        {
            result = handle_id_stack_.back();
        }
        handle_id_stack_.pop_back();
    }
    return result;
}

CaptureSettings::TraceSettings ApiCaptureManager::GetDefaultTraceSettings()
{
    // Return default trace settings.
    return CaptureSettings::TraceSettings();
}

GFXRECON_END_NAMESPACE(encode)
GFXRECON_END_NAMESPACE(gfxrecon)
