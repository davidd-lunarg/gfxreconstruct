/*
** Copyright (c) 2021 LunarG, Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to
** deal in the Software without restriction, including without limitation the
** rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
** sell copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
** IN THE SOFTWARE.
*/

/*
** This file is generated from dx12_convert_to_texts_header_generator.py.
**
*/

#ifndef  GFXRECON_GENERATED_DX12_CONVERT_TO_TEXTS_H
#define  GFXRECON_GENERATED_DX12_CONVERT_TO_TEXTS_H

#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#include <dxgi1_5.h>
#include <dxgi1_6.h>
#include <dxgicommon.h>
#include <dxgiformat.h>
#include <dxgitype.h>
#include <d3d12.h>
#include <d3dcommon.h>
#include <d3d12sdklayers.h>
#include <Unknwnbase.h>
#include <guiddef.h>
#include <windef.h>
#include <minwinbase.h>
#include "util/defines.h"

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(decode)

static const std::string ConverttoText(const DXGI_RESIDENCY value)
{
    switch(value)
    {
        case(DXGI_RESIDENCY_FULLY_RESIDENT):
            return "DXGI_RESIDENCY_FULLY_RESIDENT";
        case(DXGI_RESIDENCY_RESIDENT_IN_SHARED_MEMORY):
            return "DXGI_RESIDENCY_RESIDENT_IN_SHARED_MEMORY";
        case(DXGI_RESIDENCY_EVICTED_TO_DISK):
            return "DXGI_RESIDENCY_EVICTED_TO_DISK";
        default:
            {
                std::string code = "Invalid DXGI_RESIDENCY(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_SWAP_EFFECT value)
{
    switch(value)
    {
        case(DXGI_SWAP_EFFECT_DISCARD):
            return "DXGI_SWAP_EFFECT_DISCARD";
        case(DXGI_SWAP_EFFECT_SEQUENTIAL):
            return "DXGI_SWAP_EFFECT_SEQUENTIAL";
        case(DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL):
            return "DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL";
        case(DXGI_SWAP_EFFECT_FLIP_DISCARD):
            return "DXGI_SWAP_EFFECT_FLIP_DISCARD";
        default:
            {
                std::string code = "Invalid DXGI_SWAP_EFFECT(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_SWAP_CHAIN_FLAG value)
{
    switch(value)
    {
        case(DXGI_SWAP_CHAIN_FLAG_NONPREROTATED):
            return "DXGI_SWAP_CHAIN_FLAG_NONPREROTATED";
        case(DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH):
            return "DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH";
        case(DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE):
            return "DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE";
        case(DXGI_SWAP_CHAIN_FLAG_RESTRICTED_CONTENT):
            return "DXGI_SWAP_CHAIN_FLAG_RESTRICTED_CONTENT";
        case(DXGI_SWAP_CHAIN_FLAG_RESTRICT_SHARED_RESOURCE_DRIVER):
            return "DXGI_SWAP_CHAIN_FLAG_RESTRICT_SHARED_RESOURCE_DRIVER";
        case(DXGI_SWAP_CHAIN_FLAG_DISPLAY_ONLY):
            return "DXGI_SWAP_CHAIN_FLAG_DISPLAY_ONLY";
        case(DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT):
            return "DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT";
        case(DXGI_SWAP_CHAIN_FLAG_FOREGROUND_LAYER):
            return "DXGI_SWAP_CHAIN_FLAG_FOREGROUND_LAYER";
        case(DXGI_SWAP_CHAIN_FLAG_FULLSCREEN_VIDEO):
            return "DXGI_SWAP_CHAIN_FLAG_FULLSCREEN_VIDEO";
        case(DXGI_SWAP_CHAIN_FLAG_YUV_VIDEO):
            return "DXGI_SWAP_CHAIN_FLAG_YUV_VIDEO";
        case(DXGI_SWAP_CHAIN_FLAG_HW_PROTECTED):
            return "DXGI_SWAP_CHAIN_FLAG_HW_PROTECTED";
        case(DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING):
            return "DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING";
        case(DXGI_SWAP_CHAIN_FLAG_RESTRICTED_TO_ALL_HOLOGRAPHIC_DISPLAYS):
            return "DXGI_SWAP_CHAIN_FLAG_RESTRICTED_TO_ALL_HOLOGRAPHIC_DISPLAYS";
        default:
            {
                std::string code = "Invalid DXGI_SWAP_CHAIN_FLAG(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_ADAPTER_FLAG value)
{
    switch(value)
    {
        case(DXGI_ADAPTER_FLAG_NONE):
            return "DXGI_ADAPTER_FLAG_NONE";
        case(DXGI_ADAPTER_FLAG_REMOTE):
            return "DXGI_ADAPTER_FLAG_REMOTE";
        case(DXGI_ADAPTER_FLAG_SOFTWARE):
            return "DXGI_ADAPTER_FLAG_SOFTWARE";
        case(DXGI_ADAPTER_FLAG_FORCE_DWORD):
            return "DXGI_ADAPTER_FLAG_FORCE_DWORD";
        default:
            {
                std::string code = "Invalid DXGI_ADAPTER_FLAG(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_OUTDUPL_POINTER_SHAPE_TYPE value)
{
    switch(value)
    {
        case(DXGI_OUTDUPL_POINTER_SHAPE_TYPE_MONOCHROME):
            return "DXGI_OUTDUPL_POINTER_SHAPE_TYPE_MONOCHROME";
        case(DXGI_OUTDUPL_POINTER_SHAPE_TYPE_COLOR):
            return "DXGI_OUTDUPL_POINTER_SHAPE_TYPE_COLOR";
        case(DXGI_OUTDUPL_POINTER_SHAPE_TYPE_MASKED_COLOR):
            return "DXGI_OUTDUPL_POINTER_SHAPE_TYPE_MASKED_COLOR";
        default:
            {
                std::string code = "Invalid DXGI_OUTDUPL_POINTER_SHAPE_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_ALPHA_MODE value)
{
    switch(value)
    {
        case(DXGI_ALPHA_MODE_UNSPECIFIED):
            return "DXGI_ALPHA_MODE_UNSPECIFIED";
        case(DXGI_ALPHA_MODE_PREMULTIPLIED):
            return "DXGI_ALPHA_MODE_PREMULTIPLIED";
        case(DXGI_ALPHA_MODE_STRAIGHT):
            return "DXGI_ALPHA_MODE_STRAIGHT";
        case(DXGI_ALPHA_MODE_IGNORE):
            return "DXGI_ALPHA_MODE_IGNORE";
        case(DXGI_ALPHA_MODE_FORCE_DWORD):
            return "DXGI_ALPHA_MODE_FORCE_DWORD";
        default:
            {
                std::string code = "Invalid DXGI_ALPHA_MODE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_OFFER_RESOURCE_PRIORITY value)
{
    switch(value)
    {
        case(DXGI_OFFER_RESOURCE_PRIORITY_LOW):
            return "DXGI_OFFER_RESOURCE_PRIORITY_LOW";
        case(DXGI_OFFER_RESOURCE_PRIORITY_NORMAL):
            return "DXGI_OFFER_RESOURCE_PRIORITY_NORMAL";
        case(DXGI_OFFER_RESOURCE_PRIORITY_HIGH):
            return "DXGI_OFFER_RESOURCE_PRIORITY_HIGH";
        default:
            {
                std::string code = "Invalid DXGI_OFFER_RESOURCE_PRIORITY(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_SCALING value)
{
    switch(value)
    {
        case(DXGI_SCALING_STRETCH):
            return "DXGI_SCALING_STRETCH";
        case(DXGI_SCALING_NONE):
            return "DXGI_SCALING_NONE";
        case(DXGI_SCALING_ASPECT_RATIO_STRETCH):
            return "DXGI_SCALING_ASPECT_RATIO_STRETCH";
        default:
            {
                std::string code = "Invalid DXGI_SCALING(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_GRAPHICS_PREEMPTION_GRANULARITY value)
{
    switch(value)
    {
        case(DXGI_GRAPHICS_PREEMPTION_DMA_BUFFER_BOUNDARY):
            return "DXGI_GRAPHICS_PREEMPTION_DMA_BUFFER_BOUNDARY";
        case(DXGI_GRAPHICS_PREEMPTION_PRIMITIVE_BOUNDARY):
            return "DXGI_GRAPHICS_PREEMPTION_PRIMITIVE_BOUNDARY";
        case(DXGI_GRAPHICS_PREEMPTION_TRIANGLE_BOUNDARY):
            return "DXGI_GRAPHICS_PREEMPTION_TRIANGLE_BOUNDARY";
        case(DXGI_GRAPHICS_PREEMPTION_PIXEL_BOUNDARY):
            return "DXGI_GRAPHICS_PREEMPTION_PIXEL_BOUNDARY";
        case(DXGI_GRAPHICS_PREEMPTION_INSTRUCTION_BOUNDARY):
            return "DXGI_GRAPHICS_PREEMPTION_INSTRUCTION_BOUNDARY";
        default:
            {
                std::string code = "Invalid DXGI_GRAPHICS_PREEMPTION_GRANULARITY(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_COMPUTE_PREEMPTION_GRANULARITY value)
{
    switch(value)
    {
        case(DXGI_COMPUTE_PREEMPTION_DMA_BUFFER_BOUNDARY):
            return "DXGI_COMPUTE_PREEMPTION_DMA_BUFFER_BOUNDARY";
        case(DXGI_COMPUTE_PREEMPTION_DISPATCH_BOUNDARY):
            return "DXGI_COMPUTE_PREEMPTION_DISPATCH_BOUNDARY";
        case(DXGI_COMPUTE_PREEMPTION_THREAD_GROUP_BOUNDARY):
            return "DXGI_COMPUTE_PREEMPTION_THREAD_GROUP_BOUNDARY";
        case(DXGI_COMPUTE_PREEMPTION_THREAD_BOUNDARY):
            return "DXGI_COMPUTE_PREEMPTION_THREAD_BOUNDARY";
        case(DXGI_COMPUTE_PREEMPTION_INSTRUCTION_BOUNDARY):
            return "DXGI_COMPUTE_PREEMPTION_INSTRUCTION_BOUNDARY";
        default:
            {
                std::string code = "Invalid DXGI_COMPUTE_PREEMPTION_GRANULARITY(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAGS value)
{
    switch(value)
    {
        case(DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAG_NOMINAL_RANGE):
            return "DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAG_NOMINAL_RANGE";
        case(DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAG_BT709):
            return "DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAG_BT709";
        case(DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAG_xvYCC):
            return "DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAG_xvYCC";
        default:
            {
                std::string code = "Invalid DXGI_MULTIPLANE_OVERLAY_YCbCr_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_FRAME_PRESENTATION_MODE value)
{
    switch(value)
    {
        case(DXGI_FRAME_PRESENTATION_MODE_COMPOSED):
            return "DXGI_FRAME_PRESENTATION_MODE_COMPOSED";
        case(DXGI_FRAME_PRESENTATION_MODE_OVERLAY):
            return "DXGI_FRAME_PRESENTATION_MODE_OVERLAY";
        case(DXGI_FRAME_PRESENTATION_MODE_NONE):
            return "DXGI_FRAME_PRESENTATION_MODE_NONE";
        case(DXGI_FRAME_PRESENTATION_MODE_COMPOSITION_FAILURE):
            return "DXGI_FRAME_PRESENTATION_MODE_COMPOSITION_FAILURE";
        default:
            {
                std::string code = "Invalid DXGI_FRAME_PRESENTATION_MODE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_OVERLAY_SUPPORT_FLAG value)
{
    switch(value)
    {
        case(DXGI_OVERLAY_SUPPORT_FLAG_DIRECT):
            return "DXGI_OVERLAY_SUPPORT_FLAG_DIRECT";
        case(DXGI_OVERLAY_SUPPORT_FLAG_SCALING):
            return "DXGI_OVERLAY_SUPPORT_FLAG_SCALING";
        default:
            {
                std::string code = "Invalid DXGI_OVERLAY_SUPPORT_FLAG(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG value)
{
    switch(value)
    {
        case(DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT):
            return "DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT";
        case(DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_OVERLAY_PRESENT):
            return "DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_OVERLAY_PRESENT";
        default:
            {
                std::string code = "Invalid DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_OVERLAY_COLOR_SPACE_SUPPORT_FLAG value)
{
    switch(value)
    {
        case(DXGI_OVERLAY_COLOR_SPACE_SUPPORT_FLAG_PRESENT):
            return "DXGI_OVERLAY_COLOR_SPACE_SUPPORT_FLAG_PRESENT";
        default:
            {
                std::string code = "Invalid DXGI_OVERLAY_COLOR_SPACE_SUPPORT_FLAG(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_MEMORY_SEGMENT_GROUP value)
{
    switch(value)
    {
        case(DXGI_MEMORY_SEGMENT_GROUP_LOCAL):
            return "DXGI_MEMORY_SEGMENT_GROUP_LOCAL";
        case(DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL):
            return "DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL";
        default:
            {
                std::string code = "Invalid DXGI_MEMORY_SEGMENT_GROUP(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_OUTDUPL_FLAG value)
{
    switch(value)
    {
        case(DXGI_OUTDUPL_COMPOSITED_UI_CAPTURE_ONLY):
            return "DXGI_OUTDUPL_COMPOSITED_UI_CAPTURE_ONLY";
        default:
            {
                std::string code = "Invalid DXGI_OUTDUPL_FLAG(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_HDR_METADATA_TYPE value)
{
    switch(value)
    {
        case(DXGI_HDR_METADATA_TYPE_NONE):
            return "DXGI_HDR_METADATA_TYPE_NONE";
        case(DXGI_HDR_METADATA_TYPE_HDR10):
            return "DXGI_HDR_METADATA_TYPE_HDR10";
        case(DXGI_HDR_METADATA_TYPE_HDR10PLUS):
            return "DXGI_HDR_METADATA_TYPE_HDR10PLUS";
        default:
            {
                std::string code = "Invalid DXGI_HDR_METADATA_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_OFFER_RESOURCE_FLAGS value)
{
    switch(value)
    {
        case(DXGI_OFFER_RESOURCE_FLAG_ALLOW_DECOMMIT):
            return "DXGI_OFFER_RESOURCE_FLAG_ALLOW_DECOMMIT";
        default:
            {
                std::string code = "Invalid DXGI_OFFER_RESOURCE_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_RECLAIM_RESOURCE_RESULTS value)
{
    switch(value)
    {
        case(DXGI_RECLAIM_RESOURCE_RESULT_OK):
            return "DXGI_RECLAIM_RESOURCE_RESULT_OK";
        case(DXGI_RECLAIM_RESOURCE_RESULT_DISCARDED):
            return "DXGI_RECLAIM_RESOURCE_RESULT_DISCARDED";
        case(DXGI_RECLAIM_RESOURCE_RESULT_NOT_COMMITTED):
            return "DXGI_RECLAIM_RESOURCE_RESULT_NOT_COMMITTED";
        default:
            {
                std::string code = "Invalid DXGI_RECLAIM_RESOURCE_RESULTS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_FEATURE value)
{
    switch(value)
    {
        case(DXGI_FEATURE_PRESENT_ALLOW_TEARING):
            return "DXGI_FEATURE_PRESENT_ALLOW_TEARING";
        default:
            {
                std::string code = "Invalid DXGI_FEATURE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_ADAPTER_FLAG3 value)
{
    switch(value)
    {
        case(DXGI_ADAPTER_FLAG3_NONE):
            return "DXGI_ADAPTER_FLAG3_NONE";
        case(DXGI_ADAPTER_FLAG3_REMOTE):
            return "DXGI_ADAPTER_FLAG3_REMOTE";
        case(DXGI_ADAPTER_FLAG3_SOFTWARE):
            return "DXGI_ADAPTER_FLAG3_SOFTWARE";
        case(DXGI_ADAPTER_FLAG3_ACG_COMPATIBLE):
            return "DXGI_ADAPTER_FLAG3_ACG_COMPATIBLE";
        case(DXGI_ADAPTER_FLAG3_SUPPORT_MONITORED_FENCES):
            return "DXGI_ADAPTER_FLAG3_SUPPORT_MONITORED_FENCES";
        case(DXGI_ADAPTER_FLAG3_SUPPORT_NON_MONITORED_FENCES):
            return "DXGI_ADAPTER_FLAG3_SUPPORT_NON_MONITORED_FENCES";
        case(DXGI_ADAPTER_FLAG3_KEYED_MUTEX_CONFORMANCE):
            return "DXGI_ADAPTER_FLAG3_KEYED_MUTEX_CONFORMANCE";
        case(DXGI_ADAPTER_FLAG3_FORCE_DWORD):
            return "DXGI_ADAPTER_FLAG3_FORCE_DWORD";
        default:
            {
                std::string code = "Invalid DXGI_ADAPTER_FLAG3(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_HARDWARE_COMPOSITION_SUPPORT_FLAGS value)
{
    switch(value)
    {
        case(DXGI_HARDWARE_COMPOSITION_SUPPORT_FLAG_FULLSCREEN):
            return "DXGI_HARDWARE_COMPOSITION_SUPPORT_FLAG_FULLSCREEN";
        case(DXGI_HARDWARE_COMPOSITION_SUPPORT_FLAG_WINDOWED):
            return "DXGI_HARDWARE_COMPOSITION_SUPPORT_FLAG_WINDOWED";
        case(DXGI_HARDWARE_COMPOSITION_SUPPORT_FLAG_CURSOR_STRETCHED):
            return "DXGI_HARDWARE_COMPOSITION_SUPPORT_FLAG_CURSOR_STRETCHED";
        default:
            {
                std::string code = "Invalid DXGI_HARDWARE_COMPOSITION_SUPPORT_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_GPU_PREFERENCE value)
{
    switch(value)
    {
        case(DXGI_GPU_PREFERENCE_UNSPECIFIED):
            return "DXGI_GPU_PREFERENCE_UNSPECIFIED";
        case(DXGI_GPU_PREFERENCE_MINIMUM_POWER):
            return "DXGI_GPU_PREFERENCE_MINIMUM_POWER";
        case(DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE):
            return "DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE";
        default:
            {
                std::string code = "Invalid DXGI_GPU_PREFERENCE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_COLOR_SPACE_TYPE value)
{
    switch(value)
    {
        case(DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709):
            return "DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709";
        case(DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709):
            return "DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709";
        case(DXGI_COLOR_SPACE_RGB_STUDIO_G22_NONE_P709):
            return "DXGI_COLOR_SPACE_RGB_STUDIO_G22_NONE_P709";
        case(DXGI_COLOR_SPACE_RGB_STUDIO_G22_NONE_P2020):
            return "DXGI_COLOR_SPACE_RGB_STUDIO_G22_NONE_P2020";
        case(DXGI_COLOR_SPACE_RESERVED):
            return "DXGI_COLOR_SPACE_RESERVED";
        case(DXGI_COLOR_SPACE_YCBCR_FULL_G22_NONE_P709_X601):
            return "DXGI_COLOR_SPACE_YCBCR_FULL_G22_NONE_P709_X601";
        case(DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P601):
            return "DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P601";
        case(DXGI_COLOR_SPACE_YCBCR_FULL_G22_LEFT_P601):
            return "DXGI_COLOR_SPACE_YCBCR_FULL_G22_LEFT_P601";
        case(DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P709):
            return "DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P709";
        case(DXGI_COLOR_SPACE_YCBCR_FULL_G22_LEFT_P709):
            return "DXGI_COLOR_SPACE_YCBCR_FULL_G22_LEFT_P709";
        case(DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P2020):
            return "DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P2020";
        case(DXGI_COLOR_SPACE_YCBCR_FULL_G22_LEFT_P2020):
            return "DXGI_COLOR_SPACE_YCBCR_FULL_G22_LEFT_P2020";
        case(DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020):
            return "DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020";
        case(DXGI_COLOR_SPACE_YCBCR_STUDIO_G2084_LEFT_P2020):
            return "DXGI_COLOR_SPACE_YCBCR_STUDIO_G2084_LEFT_P2020";
        case(DXGI_COLOR_SPACE_RGB_STUDIO_G2084_NONE_P2020):
            return "DXGI_COLOR_SPACE_RGB_STUDIO_G2084_NONE_P2020";
        case(DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_TOPLEFT_P2020):
            return "DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_TOPLEFT_P2020";
        case(DXGI_COLOR_SPACE_YCBCR_STUDIO_G2084_TOPLEFT_P2020):
            return "DXGI_COLOR_SPACE_YCBCR_STUDIO_G2084_TOPLEFT_P2020";
        case(DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P2020):
            return "DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P2020";
        case(DXGI_COLOR_SPACE_YCBCR_STUDIO_GHLG_TOPLEFT_P2020):
            return "DXGI_COLOR_SPACE_YCBCR_STUDIO_GHLG_TOPLEFT_P2020";
        case(DXGI_COLOR_SPACE_YCBCR_FULL_GHLG_TOPLEFT_P2020):
            return "DXGI_COLOR_SPACE_YCBCR_FULL_GHLG_TOPLEFT_P2020";
        case(DXGI_COLOR_SPACE_RGB_STUDIO_G24_NONE_P709):
            return "DXGI_COLOR_SPACE_RGB_STUDIO_G24_NONE_P709";
        case(DXGI_COLOR_SPACE_RGB_STUDIO_G24_NONE_P2020):
            return "DXGI_COLOR_SPACE_RGB_STUDIO_G24_NONE_P2020";
        case(DXGI_COLOR_SPACE_YCBCR_STUDIO_G24_LEFT_P709):
            return "DXGI_COLOR_SPACE_YCBCR_STUDIO_G24_LEFT_P709";
        case(DXGI_COLOR_SPACE_YCBCR_STUDIO_G24_LEFT_P2020):
            return "DXGI_COLOR_SPACE_YCBCR_STUDIO_G24_LEFT_P2020";
        case(DXGI_COLOR_SPACE_YCBCR_STUDIO_G24_TOPLEFT_P2020):
            return "DXGI_COLOR_SPACE_YCBCR_STUDIO_G24_TOPLEFT_P2020";
        case(DXGI_COLOR_SPACE_CUSTOM):
            return "DXGI_COLOR_SPACE_CUSTOM";
        default:
            {
                std::string code = "Invalid DXGI_COLOR_SPACE_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_FORMAT value)
{
    switch(value)
    {
        case(DXGI_FORMAT_UNKNOWN):
            return "DXGI_FORMAT_UNKNOWN";
        case(DXGI_FORMAT_R32G32B32A32_TYPELESS):
            return "DXGI_FORMAT_R32G32B32A32_TYPELESS";
        case(DXGI_FORMAT_R32G32B32A32_FLOAT):
            return "DXGI_FORMAT_R32G32B32A32_FLOAT";
        case(DXGI_FORMAT_R32G32B32A32_UINT):
            return "DXGI_FORMAT_R32G32B32A32_UINT";
        case(DXGI_FORMAT_R32G32B32A32_SINT):
            return "DXGI_FORMAT_R32G32B32A32_SINT";
        case(DXGI_FORMAT_R32G32B32_TYPELESS):
            return "DXGI_FORMAT_R32G32B32_TYPELESS";
        case(DXGI_FORMAT_R32G32B32_FLOAT):
            return "DXGI_FORMAT_R32G32B32_FLOAT";
        case(DXGI_FORMAT_R32G32B32_UINT):
            return "DXGI_FORMAT_R32G32B32_UINT";
        case(DXGI_FORMAT_R32G32B32_SINT):
            return "DXGI_FORMAT_R32G32B32_SINT";
        case(DXGI_FORMAT_R16G16B16A16_TYPELESS):
            return "DXGI_FORMAT_R16G16B16A16_TYPELESS";
        case(DXGI_FORMAT_R16G16B16A16_FLOAT):
            return "DXGI_FORMAT_R16G16B16A16_FLOAT";
        case(DXGI_FORMAT_R16G16B16A16_UNORM):
            return "DXGI_FORMAT_R16G16B16A16_UNORM";
        case(DXGI_FORMAT_R16G16B16A16_UINT):
            return "DXGI_FORMAT_R16G16B16A16_UINT";
        case(DXGI_FORMAT_R16G16B16A16_SNORM):
            return "DXGI_FORMAT_R16G16B16A16_SNORM";
        case(DXGI_FORMAT_R16G16B16A16_SINT):
            return "DXGI_FORMAT_R16G16B16A16_SINT";
        case(DXGI_FORMAT_R32G32_TYPELESS):
            return "DXGI_FORMAT_R32G32_TYPELESS";
        case(DXGI_FORMAT_R32G32_FLOAT):
            return "DXGI_FORMAT_R32G32_FLOAT";
        case(DXGI_FORMAT_R32G32_UINT):
            return "DXGI_FORMAT_R32G32_UINT";
        case(DXGI_FORMAT_R32G32_SINT):
            return "DXGI_FORMAT_R32G32_SINT";
        case(DXGI_FORMAT_R32G8X24_TYPELESS):
            return "DXGI_FORMAT_R32G8X24_TYPELESS";
        case(DXGI_FORMAT_D32_FLOAT_S8X24_UINT):
            return "DXGI_FORMAT_D32_FLOAT_S8X24_UINT";
        case(DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS):
            return "DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS";
        case(DXGI_FORMAT_X32_TYPELESS_G8X24_UINT):
            return "DXGI_FORMAT_X32_TYPELESS_G8X24_UINT";
        case(DXGI_FORMAT_R10G10B10A2_TYPELESS):
            return "DXGI_FORMAT_R10G10B10A2_TYPELESS";
        case(DXGI_FORMAT_R10G10B10A2_UNORM):
            return "DXGI_FORMAT_R10G10B10A2_UNORM";
        case(DXGI_FORMAT_R10G10B10A2_UINT):
            return "DXGI_FORMAT_R10G10B10A2_UINT";
        case(DXGI_FORMAT_R11G11B10_FLOAT):
            return "DXGI_FORMAT_R11G11B10_FLOAT";
        case(DXGI_FORMAT_R8G8B8A8_TYPELESS):
            return "DXGI_FORMAT_R8G8B8A8_TYPELESS";
        case(DXGI_FORMAT_R8G8B8A8_UNORM):
            return "DXGI_FORMAT_R8G8B8A8_UNORM";
        case(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB):
            return "DXGI_FORMAT_R8G8B8A8_UNORM_SRGB";
        case(DXGI_FORMAT_R8G8B8A8_UINT):
            return "DXGI_FORMAT_R8G8B8A8_UINT";
        case(DXGI_FORMAT_R8G8B8A8_SNORM):
            return "DXGI_FORMAT_R8G8B8A8_SNORM";
        case(DXGI_FORMAT_R8G8B8A8_SINT):
            return "DXGI_FORMAT_R8G8B8A8_SINT";
        case(DXGI_FORMAT_R16G16_TYPELESS):
            return "DXGI_FORMAT_R16G16_TYPELESS";
        case(DXGI_FORMAT_R16G16_FLOAT):
            return "DXGI_FORMAT_R16G16_FLOAT";
        case(DXGI_FORMAT_R16G16_UNORM):
            return "DXGI_FORMAT_R16G16_UNORM";
        case(DXGI_FORMAT_R16G16_UINT):
            return "DXGI_FORMAT_R16G16_UINT";
        case(DXGI_FORMAT_R16G16_SNORM):
            return "DXGI_FORMAT_R16G16_SNORM";
        case(DXGI_FORMAT_R16G16_SINT):
            return "DXGI_FORMAT_R16G16_SINT";
        case(DXGI_FORMAT_R32_TYPELESS):
            return "DXGI_FORMAT_R32_TYPELESS";
        case(DXGI_FORMAT_D32_FLOAT):
            return "DXGI_FORMAT_D32_FLOAT";
        case(DXGI_FORMAT_R32_FLOAT):
            return "DXGI_FORMAT_R32_FLOAT";
        case(DXGI_FORMAT_R32_UINT):
            return "DXGI_FORMAT_R32_UINT";
        case(DXGI_FORMAT_R32_SINT):
            return "DXGI_FORMAT_R32_SINT";
        case(DXGI_FORMAT_R24G8_TYPELESS):
            return "DXGI_FORMAT_R24G8_TYPELESS";
        case(DXGI_FORMAT_D24_UNORM_S8_UINT):
            return "DXGI_FORMAT_D24_UNORM_S8_UINT";
        case(DXGI_FORMAT_R24_UNORM_X8_TYPELESS):
            return "DXGI_FORMAT_R24_UNORM_X8_TYPELESS";
        case(DXGI_FORMAT_X24_TYPELESS_G8_UINT):
            return "DXGI_FORMAT_X24_TYPELESS_G8_UINT";
        case(DXGI_FORMAT_R8G8_TYPELESS):
            return "DXGI_FORMAT_R8G8_TYPELESS";
        case(DXGI_FORMAT_R8G8_UNORM):
            return "DXGI_FORMAT_R8G8_UNORM";
        case(DXGI_FORMAT_R8G8_UINT):
            return "DXGI_FORMAT_R8G8_UINT";
        case(DXGI_FORMAT_R8G8_SNORM):
            return "DXGI_FORMAT_R8G8_SNORM";
        case(DXGI_FORMAT_R8G8_SINT):
            return "DXGI_FORMAT_R8G8_SINT";
        case(DXGI_FORMAT_R16_TYPELESS):
            return "DXGI_FORMAT_R16_TYPELESS";
        case(DXGI_FORMAT_R16_FLOAT):
            return "DXGI_FORMAT_R16_FLOAT";
        case(DXGI_FORMAT_D16_UNORM):
            return "DXGI_FORMAT_D16_UNORM";
        case(DXGI_FORMAT_R16_UNORM):
            return "DXGI_FORMAT_R16_UNORM";
        case(DXGI_FORMAT_R16_UINT):
            return "DXGI_FORMAT_R16_UINT";
        case(DXGI_FORMAT_R16_SNORM):
            return "DXGI_FORMAT_R16_SNORM";
        case(DXGI_FORMAT_R16_SINT):
            return "DXGI_FORMAT_R16_SINT";
        case(DXGI_FORMAT_R8_TYPELESS):
            return "DXGI_FORMAT_R8_TYPELESS";
        case(DXGI_FORMAT_R8_UNORM):
            return "DXGI_FORMAT_R8_UNORM";
        case(DXGI_FORMAT_R8_UINT):
            return "DXGI_FORMAT_R8_UINT";
        case(DXGI_FORMAT_R8_SNORM):
            return "DXGI_FORMAT_R8_SNORM";
        case(DXGI_FORMAT_R8_SINT):
            return "DXGI_FORMAT_R8_SINT";
        case(DXGI_FORMAT_A8_UNORM):
            return "DXGI_FORMAT_A8_UNORM";
        case(DXGI_FORMAT_R1_UNORM):
            return "DXGI_FORMAT_R1_UNORM";
        case(DXGI_FORMAT_R9G9B9E5_SHAREDEXP):
            return "DXGI_FORMAT_R9G9B9E5_SHAREDEXP";
        case(DXGI_FORMAT_R8G8_B8G8_UNORM):
            return "DXGI_FORMAT_R8G8_B8G8_UNORM";
        case(DXGI_FORMAT_G8R8_G8B8_UNORM):
            return "DXGI_FORMAT_G8R8_G8B8_UNORM";
        case(DXGI_FORMAT_BC1_TYPELESS):
            return "DXGI_FORMAT_BC1_TYPELESS";
        case(DXGI_FORMAT_BC1_UNORM):
            return "DXGI_FORMAT_BC1_UNORM";
        case(DXGI_FORMAT_BC1_UNORM_SRGB):
            return "DXGI_FORMAT_BC1_UNORM_SRGB";
        case(DXGI_FORMAT_BC2_TYPELESS):
            return "DXGI_FORMAT_BC2_TYPELESS";
        case(DXGI_FORMAT_BC2_UNORM):
            return "DXGI_FORMAT_BC2_UNORM";
        case(DXGI_FORMAT_BC2_UNORM_SRGB):
            return "DXGI_FORMAT_BC2_UNORM_SRGB";
        case(DXGI_FORMAT_BC3_TYPELESS):
            return "DXGI_FORMAT_BC3_TYPELESS";
        case(DXGI_FORMAT_BC3_UNORM):
            return "DXGI_FORMAT_BC3_UNORM";
        case(DXGI_FORMAT_BC3_UNORM_SRGB):
            return "DXGI_FORMAT_BC3_UNORM_SRGB";
        case(DXGI_FORMAT_BC4_TYPELESS):
            return "DXGI_FORMAT_BC4_TYPELESS";
        case(DXGI_FORMAT_BC4_UNORM):
            return "DXGI_FORMAT_BC4_UNORM";
        case(DXGI_FORMAT_BC4_SNORM):
            return "DXGI_FORMAT_BC4_SNORM";
        case(DXGI_FORMAT_BC5_TYPELESS):
            return "DXGI_FORMAT_BC5_TYPELESS";
        case(DXGI_FORMAT_BC5_UNORM):
            return "DXGI_FORMAT_BC5_UNORM";
        case(DXGI_FORMAT_BC5_SNORM):
            return "DXGI_FORMAT_BC5_SNORM";
        case(DXGI_FORMAT_B5G6R5_UNORM):
            return "DXGI_FORMAT_B5G6R5_UNORM";
        case(DXGI_FORMAT_B5G5R5A1_UNORM):
            return "DXGI_FORMAT_B5G5R5A1_UNORM";
        case(DXGI_FORMAT_B8G8R8A8_UNORM):
            return "DXGI_FORMAT_B8G8R8A8_UNORM";
        case(DXGI_FORMAT_B8G8R8X8_UNORM):
            return "DXGI_FORMAT_B8G8R8X8_UNORM";
        case(DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM):
            return "DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM";
        case(DXGI_FORMAT_B8G8R8A8_TYPELESS):
            return "DXGI_FORMAT_B8G8R8A8_TYPELESS";
        case(DXGI_FORMAT_B8G8R8A8_UNORM_SRGB):
            return "DXGI_FORMAT_B8G8R8A8_UNORM_SRGB";
        case(DXGI_FORMAT_B8G8R8X8_TYPELESS):
            return "DXGI_FORMAT_B8G8R8X8_TYPELESS";
        case(DXGI_FORMAT_B8G8R8X8_UNORM_SRGB):
            return "DXGI_FORMAT_B8G8R8X8_UNORM_SRGB";
        case(DXGI_FORMAT_BC6H_TYPELESS):
            return "DXGI_FORMAT_BC6H_TYPELESS";
        case(DXGI_FORMAT_BC6H_UF16):
            return "DXGI_FORMAT_BC6H_UF16";
        case(DXGI_FORMAT_BC6H_SF16):
            return "DXGI_FORMAT_BC6H_SF16";
        case(DXGI_FORMAT_BC7_TYPELESS):
            return "DXGI_FORMAT_BC7_TYPELESS";
        case(DXGI_FORMAT_BC7_UNORM):
            return "DXGI_FORMAT_BC7_UNORM";
        case(DXGI_FORMAT_BC7_UNORM_SRGB):
            return "DXGI_FORMAT_BC7_UNORM_SRGB";
        case(DXGI_FORMAT_AYUV):
            return "DXGI_FORMAT_AYUV";
        case(DXGI_FORMAT_Y410):
            return "DXGI_FORMAT_Y410";
        case(DXGI_FORMAT_Y416):
            return "DXGI_FORMAT_Y416";
        case(DXGI_FORMAT_NV12):
            return "DXGI_FORMAT_NV12";
        case(DXGI_FORMAT_P010):
            return "DXGI_FORMAT_P010";
        case(DXGI_FORMAT_P016):
            return "DXGI_FORMAT_P016";
        case(DXGI_FORMAT_420_OPAQUE):
            return "DXGI_FORMAT_420_OPAQUE";
        case(DXGI_FORMAT_YUY2):
            return "DXGI_FORMAT_YUY2";
        case(DXGI_FORMAT_Y210):
            return "DXGI_FORMAT_Y210";
        case(DXGI_FORMAT_Y216):
            return "DXGI_FORMAT_Y216";
        case(DXGI_FORMAT_NV11):
            return "DXGI_FORMAT_NV11";
        case(DXGI_FORMAT_AI44):
            return "DXGI_FORMAT_AI44";
        case(DXGI_FORMAT_IA44):
            return "DXGI_FORMAT_IA44";
        case(DXGI_FORMAT_P8):
            return "DXGI_FORMAT_P8";
        case(DXGI_FORMAT_A8P8):
            return "DXGI_FORMAT_A8P8";
        case(DXGI_FORMAT_B4G4R4A4_UNORM):
            return "DXGI_FORMAT_B4G4R4A4_UNORM";
        case(DXGI_FORMAT_P208):
            return "DXGI_FORMAT_P208";
        case(DXGI_FORMAT_V208):
            return "DXGI_FORMAT_V208";
        case(DXGI_FORMAT_V408):
            return "DXGI_FORMAT_V408";
        case(DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE):
            return "DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE";
        case(DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE):
            return "DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE";
        case(DXGI_FORMAT_FORCE_UINT):
            return "DXGI_FORMAT_FORCE_UINT";
        default:
            {
                std::string code = "Invalid DXGI_FORMAT(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_MODE_SCANLINE_ORDER value)
{
    switch(value)
    {
        case(DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED):
            return "DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED";
        case(DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE):
            return "DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE";
        case(DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST):
            return "DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST";
        case(DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST):
            return "DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST";
        default:
            {
                std::string code = "Invalid DXGI_MODE_SCANLINE_ORDER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_MODE_SCALING value)
{
    switch(value)
    {
        case(DXGI_MODE_SCALING_UNSPECIFIED):
            return "DXGI_MODE_SCALING_UNSPECIFIED";
        case(DXGI_MODE_SCALING_CENTERED):
            return "DXGI_MODE_SCALING_CENTERED";
        case(DXGI_MODE_SCALING_STRETCHED):
            return "DXGI_MODE_SCALING_STRETCHED";
        default:
            {
                std::string code = "Invalid DXGI_MODE_SCALING(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const DXGI_MODE_ROTATION value)
{
    switch(value)
    {
        case(DXGI_MODE_ROTATION_UNSPECIFIED):
            return "DXGI_MODE_ROTATION_UNSPECIFIED";
        case(DXGI_MODE_ROTATION_IDENTITY):
            return "DXGI_MODE_ROTATION_IDENTITY";
        case(DXGI_MODE_ROTATION_ROTATE90):
            return "DXGI_MODE_ROTATION_ROTATE90";
        case(DXGI_MODE_ROTATION_ROTATE180):
            return "DXGI_MODE_ROTATION_ROTATE180";
        case(DXGI_MODE_ROTATION_ROTATE270):
            return "DXGI_MODE_ROTATION_ROTATE270";
        default:
            {
                std::string code = "Invalid DXGI_MODE_ROTATION(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_COMMAND_LIST_TYPE value)
{
    switch(value)
    {
        case(D3D12_COMMAND_LIST_TYPE_DIRECT):
            return "D3D12_COMMAND_LIST_TYPE_DIRECT";
        case(D3D12_COMMAND_LIST_TYPE_BUNDLE):
            return "D3D12_COMMAND_LIST_TYPE_BUNDLE";
        case(D3D12_COMMAND_LIST_TYPE_COMPUTE):
            return "D3D12_COMMAND_LIST_TYPE_COMPUTE";
        case(D3D12_COMMAND_LIST_TYPE_COPY):
            return "D3D12_COMMAND_LIST_TYPE_COPY";
        case(D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE):
            return "D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE";
        case(D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS):
            return "D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS";
        case(D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE):
            return "D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE";
        default:
            {
                std::string code = "Invalid D3D12_COMMAND_LIST_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_COMMAND_QUEUE_FLAGS value)
{
    switch(value)
    {
        case(D3D12_COMMAND_QUEUE_FLAG_NONE):
            return "D3D12_COMMAND_QUEUE_FLAG_NONE";
        case(D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT):
            return "D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT";
        default:
            {
                std::string code = "Invalid D3D12_COMMAND_QUEUE_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_COMMAND_QUEUE_PRIORITY value)
{
    switch(value)
    {
        case(D3D12_COMMAND_QUEUE_PRIORITY_NORMAL):
            return "D3D12_COMMAND_QUEUE_PRIORITY_NORMAL";
        case(D3D12_COMMAND_QUEUE_PRIORITY_HIGH):
            return "D3D12_COMMAND_QUEUE_PRIORITY_HIGH";
        case(D3D12_COMMAND_QUEUE_PRIORITY_GLOBAL_REALTIME):
            return "D3D12_COMMAND_QUEUE_PRIORITY_GLOBAL_REALTIME";
        default:
            {
                std::string code = "Invalid D3D12_COMMAND_QUEUE_PRIORITY(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_PRIMITIVE_TOPOLOGY_TYPE value)
{
    switch(value)
    {
        case(D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED):
            return "D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED";
        case(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT):
            return "D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT";
        case(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE):
            return "D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE";
        case(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE):
            return "D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE";
        case(D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH):
            return "D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH";
        default:
            {
                std::string code = "Invalid D3D12_PRIMITIVE_TOPOLOGY_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_INPUT_CLASSIFICATION value)
{
    switch(value)
    {
        case(D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA):
            return "D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA";
        case(D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA):
            return "D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA";
        default:
            {
                std::string code = "Invalid D3D12_INPUT_CLASSIFICATION(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_FILL_MODE value)
{
    switch(value)
    {
        case(D3D12_FILL_MODE_WIREFRAME):
            return "D3D12_FILL_MODE_WIREFRAME";
        case(D3D12_FILL_MODE_SOLID):
            return "D3D12_FILL_MODE_SOLID";
        default:
            {
                std::string code = "Invalid D3D12_FILL_MODE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_CULL_MODE value)
{
    switch(value)
    {
        case(D3D12_CULL_MODE_NONE):
            return "D3D12_CULL_MODE_NONE";
        case(D3D12_CULL_MODE_FRONT):
            return "D3D12_CULL_MODE_FRONT";
        case(D3D12_CULL_MODE_BACK):
            return "D3D12_CULL_MODE_BACK";
        default:
            {
                std::string code = "Invalid D3D12_CULL_MODE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_COMPARISON_FUNC value)
{
    switch(value)
    {
        case(D3D12_COMPARISON_FUNC_NEVER):
            return "D3D12_COMPARISON_FUNC_NEVER";
        case(D3D12_COMPARISON_FUNC_LESS):
            return "D3D12_COMPARISON_FUNC_LESS";
        case(D3D12_COMPARISON_FUNC_EQUAL):
            return "D3D12_COMPARISON_FUNC_EQUAL";
        case(D3D12_COMPARISON_FUNC_LESS_EQUAL):
            return "D3D12_COMPARISON_FUNC_LESS_EQUAL";
        case(D3D12_COMPARISON_FUNC_GREATER):
            return "D3D12_COMPARISON_FUNC_GREATER";
        case(D3D12_COMPARISON_FUNC_NOT_EQUAL):
            return "D3D12_COMPARISON_FUNC_NOT_EQUAL";
        case(D3D12_COMPARISON_FUNC_GREATER_EQUAL):
            return "D3D12_COMPARISON_FUNC_GREATER_EQUAL";
        case(D3D12_COMPARISON_FUNC_ALWAYS):
            return "D3D12_COMPARISON_FUNC_ALWAYS";
        default:
            {
                std::string code = "Invalid D3D12_COMPARISON_FUNC(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DEPTH_WRITE_MASK value)
{
    switch(value)
    {
        case(D3D12_DEPTH_WRITE_MASK_ZERO):
            return "D3D12_DEPTH_WRITE_MASK_ZERO";
        case(D3D12_DEPTH_WRITE_MASK_ALL):
            return "D3D12_DEPTH_WRITE_MASK_ALL";
        default:
            {
                std::string code = "Invalid D3D12_DEPTH_WRITE_MASK(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_STENCIL_OP value)
{
    switch(value)
    {
        case(D3D12_STENCIL_OP_KEEP):
            return "D3D12_STENCIL_OP_KEEP";
        case(D3D12_STENCIL_OP_ZERO):
            return "D3D12_STENCIL_OP_ZERO";
        case(D3D12_STENCIL_OP_REPLACE):
            return "D3D12_STENCIL_OP_REPLACE";
        case(D3D12_STENCIL_OP_INCR_SAT):
            return "D3D12_STENCIL_OP_INCR_SAT";
        case(D3D12_STENCIL_OP_DECR_SAT):
            return "D3D12_STENCIL_OP_DECR_SAT";
        case(D3D12_STENCIL_OP_INVERT):
            return "D3D12_STENCIL_OP_INVERT";
        case(D3D12_STENCIL_OP_INCR):
            return "D3D12_STENCIL_OP_INCR";
        case(D3D12_STENCIL_OP_DECR):
            return "D3D12_STENCIL_OP_DECR";
        default:
            {
                std::string code = "Invalid D3D12_STENCIL_OP(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_BLEND value)
{
    switch(value)
    {
        case(D3D12_BLEND_ZERO):
            return "D3D12_BLEND_ZERO";
        case(D3D12_BLEND_ONE):
            return "D3D12_BLEND_ONE";
        case(D3D12_BLEND_SRC_COLOR):
            return "D3D12_BLEND_SRC_COLOR";
        case(D3D12_BLEND_INV_SRC_COLOR):
            return "D3D12_BLEND_INV_SRC_COLOR";
        case(D3D12_BLEND_SRC_ALPHA):
            return "D3D12_BLEND_SRC_ALPHA";
        case(D3D12_BLEND_INV_SRC_ALPHA):
            return "D3D12_BLEND_INV_SRC_ALPHA";
        case(D3D12_BLEND_DEST_ALPHA):
            return "D3D12_BLEND_DEST_ALPHA";
        case(D3D12_BLEND_INV_DEST_ALPHA):
            return "D3D12_BLEND_INV_DEST_ALPHA";
        case(D3D12_BLEND_DEST_COLOR):
            return "D3D12_BLEND_DEST_COLOR";
        case(D3D12_BLEND_INV_DEST_COLOR):
            return "D3D12_BLEND_INV_DEST_COLOR";
        case(D3D12_BLEND_SRC_ALPHA_SAT):
            return "D3D12_BLEND_SRC_ALPHA_SAT";
        case(D3D12_BLEND_BLEND_FACTOR):
            return "D3D12_BLEND_BLEND_FACTOR";
        case(D3D12_BLEND_INV_BLEND_FACTOR):
            return "D3D12_BLEND_INV_BLEND_FACTOR";
        case(D3D12_BLEND_SRC1_COLOR):
            return "D3D12_BLEND_SRC1_COLOR";
        case(D3D12_BLEND_INV_SRC1_COLOR):
            return "D3D12_BLEND_INV_SRC1_COLOR";
        case(D3D12_BLEND_SRC1_ALPHA):
            return "D3D12_BLEND_SRC1_ALPHA";
        case(D3D12_BLEND_INV_SRC1_ALPHA):
            return "D3D12_BLEND_INV_SRC1_ALPHA";
        default:
            {
                std::string code = "Invalid D3D12_BLEND(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_BLEND_OP value)
{
    switch(value)
    {
        case(D3D12_BLEND_OP_ADD):
            return "D3D12_BLEND_OP_ADD";
        case(D3D12_BLEND_OP_SUBTRACT):
            return "D3D12_BLEND_OP_SUBTRACT";
        case(D3D12_BLEND_OP_REV_SUBTRACT):
            return "D3D12_BLEND_OP_REV_SUBTRACT";
        case(D3D12_BLEND_OP_MIN):
            return "D3D12_BLEND_OP_MIN";
        case(D3D12_BLEND_OP_MAX):
            return "D3D12_BLEND_OP_MAX";
        default:
            {
                std::string code = "Invalid D3D12_BLEND_OP(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_COLOR_WRITE_ENABLE value)
{
    switch(value)
    {
        case(D3D12_COLOR_WRITE_ENABLE_RED):
            return "D3D12_COLOR_WRITE_ENABLE_RED";
        case(D3D12_COLOR_WRITE_ENABLE_GREEN):
            return "D3D12_COLOR_WRITE_ENABLE_GREEN";
        case(D3D12_COLOR_WRITE_ENABLE_BLUE):
            return "D3D12_COLOR_WRITE_ENABLE_BLUE";
        case(D3D12_COLOR_WRITE_ENABLE_ALPHA):
            return "D3D12_COLOR_WRITE_ENABLE_ALPHA";
        default:
            {
                std::string code = "Invalid D3D12_COLOR_WRITE_ENABLE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_LOGIC_OP value)
{
    switch(value)
    {
        case(D3D12_LOGIC_OP_CLEAR):
            return "D3D12_LOGIC_OP_CLEAR";
        case(D3D12_LOGIC_OP_SET):
            return "D3D12_LOGIC_OP_SET";
        case(D3D12_LOGIC_OP_COPY):
            return "D3D12_LOGIC_OP_COPY";
        case(D3D12_LOGIC_OP_COPY_INVERTED):
            return "D3D12_LOGIC_OP_COPY_INVERTED";
        case(D3D12_LOGIC_OP_NOOP):
            return "D3D12_LOGIC_OP_NOOP";
        case(D3D12_LOGIC_OP_INVERT):
            return "D3D12_LOGIC_OP_INVERT";
        case(D3D12_LOGIC_OP_AND):
            return "D3D12_LOGIC_OP_AND";
        case(D3D12_LOGIC_OP_NAND):
            return "D3D12_LOGIC_OP_NAND";
        case(D3D12_LOGIC_OP_OR):
            return "D3D12_LOGIC_OP_OR";
        case(D3D12_LOGIC_OP_NOR):
            return "D3D12_LOGIC_OP_NOR";
        case(D3D12_LOGIC_OP_XOR):
            return "D3D12_LOGIC_OP_XOR";
        case(D3D12_LOGIC_OP_EQUIV):
            return "D3D12_LOGIC_OP_EQUIV";
        case(D3D12_LOGIC_OP_AND_REVERSE):
            return "D3D12_LOGIC_OP_AND_REVERSE";
        case(D3D12_LOGIC_OP_AND_INVERTED):
            return "D3D12_LOGIC_OP_AND_INVERTED";
        case(D3D12_LOGIC_OP_OR_REVERSE):
            return "D3D12_LOGIC_OP_OR_REVERSE";
        case(D3D12_LOGIC_OP_OR_INVERTED):
            return "D3D12_LOGIC_OP_OR_INVERTED";
        default:
            {
                std::string code = "Invalid D3D12_LOGIC_OP(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_CONSERVATIVE_RASTERIZATION_MODE value)
{
    switch(value)
    {
        case(D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF):
            return "D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF";
        case(D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON):
            return "D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON";
        default:
            {
                std::string code = "Invalid D3D12_CONSERVATIVE_RASTERIZATION_MODE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_INDEX_BUFFER_STRIP_CUT_VALUE value)
{
    switch(value)
    {
        case(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED):
            return "D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED";
        case(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF):
            return "D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF";
        case(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF):
            return "D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF";
        default:
            {
                std::string code = "Invalid D3D12_INDEX_BUFFER_STRIP_CUT_VALUE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_PIPELINE_STATE_FLAGS value)
{
    switch(value)
    {
        case(D3D12_PIPELINE_STATE_FLAG_NONE):
            return "D3D12_PIPELINE_STATE_FLAG_NONE";
        case(D3D12_PIPELINE_STATE_FLAG_TOOL_DEBUG):
            return "D3D12_PIPELINE_STATE_FLAG_TOOL_DEBUG";
        default:
            {
                std::string code = "Invalid D3D12_PIPELINE_STATE_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_PIPELINE_STATE_SUBOBJECT_TYPE value)
{
    switch(value)
    {
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_ROOT_SIGNATURE):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_ROOT_SIGNATURE";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VS):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VS";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PS):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PS";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DS):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DS";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_HS):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_HS";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_GS):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_GS";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_CS):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_CS";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_STREAM_OUTPUT):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_STREAM_OUTPUT";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_BLEND):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_BLEND";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_MASK):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_MASK";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RASTERIZER):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RASTERIZER";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_INPUT_LAYOUT):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_INPUT_LAYOUT";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_IB_STRIP_CUT_VALUE):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_IB_STRIP_CUT_VALUE";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PRIMITIVE_TOPOLOGY):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_PRIMITIVE_TOPOLOGY";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RENDER_TARGET_FORMATS):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_RENDER_TARGET_FORMATS";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL_FORMAT):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL_FORMAT";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_DESC):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_SAMPLE_DESC";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_NODE_MASK):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_NODE_MASK";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_CACHED_PSO):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_CACHED_PSO";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_FLAGS):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_FLAGS";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL1):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_DEPTH_STENCIL1";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VIEW_INSTANCING):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_VIEW_INSTANCING";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_AS):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_AS";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_MS):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_MS";
        case(D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_MAX_VALID):
            return "D3D12_PIPELINE_STATE_SUBOBJECT_TYPE_MAX_VALID";
        default:
            {
                std::string code = "Invalid D3D12_PIPELINE_STATE_SUBOBJECT_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_FEATURE value)
{
    switch(value)
    {
        case(D3D12_FEATURE_D3D12_OPTIONS):
            return "D3D12_FEATURE_D3D12_OPTIONS";
        case(D3D12_FEATURE_ARCHITECTURE):
            return "D3D12_FEATURE_ARCHITECTURE";
        case(D3D12_FEATURE_FEATURE_LEVELS):
            return "D3D12_FEATURE_FEATURE_LEVELS";
        case(D3D12_FEATURE_FORMAT_SUPPORT):
            return "D3D12_FEATURE_FORMAT_SUPPORT";
        case(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS):
            return "D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS";
        case(D3D12_FEATURE_FORMAT_INFO):
            return "D3D12_FEATURE_FORMAT_INFO";
        case(D3D12_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT):
            return "D3D12_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT";
        case(D3D12_FEATURE_SHADER_MODEL):
            return "D3D12_FEATURE_SHADER_MODEL";
        case(D3D12_FEATURE_D3D12_OPTIONS1):
            return "D3D12_FEATURE_D3D12_OPTIONS1";
        case(D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_SUPPORT):
            return "D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_SUPPORT";
        case(D3D12_FEATURE_ROOT_SIGNATURE):
            return "D3D12_FEATURE_ROOT_SIGNATURE";
        case(D3D12_FEATURE_ARCHITECTURE1):
            return "D3D12_FEATURE_ARCHITECTURE1";
        case(D3D12_FEATURE_D3D12_OPTIONS2):
            return "D3D12_FEATURE_D3D12_OPTIONS2";
        case(D3D12_FEATURE_SHADER_CACHE):
            return "D3D12_FEATURE_SHADER_CACHE";
        case(D3D12_FEATURE_COMMAND_QUEUE_PRIORITY):
            return "D3D12_FEATURE_COMMAND_QUEUE_PRIORITY";
        case(D3D12_FEATURE_D3D12_OPTIONS3):
            return "D3D12_FEATURE_D3D12_OPTIONS3";
        case(D3D12_FEATURE_EXISTING_HEAPS):
            return "D3D12_FEATURE_EXISTING_HEAPS";
        case(D3D12_FEATURE_D3D12_OPTIONS4):
            return "D3D12_FEATURE_D3D12_OPTIONS4";
        case(D3D12_FEATURE_SERIALIZATION):
            return "D3D12_FEATURE_SERIALIZATION";
        case(D3D12_FEATURE_CROSS_NODE):
            return "D3D12_FEATURE_CROSS_NODE";
        case(D3D12_FEATURE_D3D12_OPTIONS5):
            return "D3D12_FEATURE_D3D12_OPTIONS5";
        case(D3D12_FEATURE_D3D12_OPTIONS6):
            return "D3D12_FEATURE_D3D12_OPTIONS6";
        case(D3D12_FEATURE_QUERY_META_COMMAND):
            return "D3D12_FEATURE_QUERY_META_COMMAND";
        case(D3D12_FEATURE_D3D12_OPTIONS7):
            return "D3D12_FEATURE_D3D12_OPTIONS7";
        case(D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_TYPE_COUNT):
            return "D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_TYPE_COUNT";
        case(D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_TYPES):
            return "D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_TYPES";
        default:
            {
                std::string code = "Invalid D3D12_FEATURE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_SHADER_MIN_PRECISION_SUPPORT value)
{
    switch(value)
    {
        case(D3D12_SHADER_MIN_PRECISION_SUPPORT_NONE):
            return "D3D12_SHADER_MIN_PRECISION_SUPPORT_NONE";
        case(D3D12_SHADER_MIN_PRECISION_SUPPORT_10_BIT):
            return "D3D12_SHADER_MIN_PRECISION_SUPPORT_10_BIT";
        case(D3D12_SHADER_MIN_PRECISION_SUPPORT_16_BIT):
            return "D3D12_SHADER_MIN_PRECISION_SUPPORT_16_BIT";
        default:
            {
                std::string code = "Invalid D3D12_SHADER_MIN_PRECISION_SUPPORT(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_TILED_RESOURCES_TIER value)
{
    switch(value)
    {
        case(D3D12_TILED_RESOURCES_TIER_NOT_SUPPORTED):
            return "D3D12_TILED_RESOURCES_TIER_NOT_SUPPORTED";
        case(D3D12_TILED_RESOURCES_TIER_1):
            return "D3D12_TILED_RESOURCES_TIER_1";
        case(D3D12_TILED_RESOURCES_TIER_2):
            return "D3D12_TILED_RESOURCES_TIER_2";
        case(D3D12_TILED_RESOURCES_TIER_3):
            return "D3D12_TILED_RESOURCES_TIER_3";
        case(D3D12_TILED_RESOURCES_TIER_4):
            return "D3D12_TILED_RESOURCES_TIER_4";
        default:
            {
                std::string code = "Invalid D3D12_TILED_RESOURCES_TIER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RESOURCE_BINDING_TIER value)
{
    switch(value)
    {
        case(D3D12_RESOURCE_BINDING_TIER_1):
            return "D3D12_RESOURCE_BINDING_TIER_1";
        case(D3D12_RESOURCE_BINDING_TIER_2):
            return "D3D12_RESOURCE_BINDING_TIER_2";
        case(D3D12_RESOURCE_BINDING_TIER_3):
            return "D3D12_RESOURCE_BINDING_TIER_3";
        default:
            {
                std::string code = "Invalid D3D12_RESOURCE_BINDING_TIER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_CONSERVATIVE_RASTERIZATION_TIER value)
{
    switch(value)
    {
        case(D3D12_CONSERVATIVE_RASTERIZATION_TIER_NOT_SUPPORTED):
            return "D3D12_CONSERVATIVE_RASTERIZATION_TIER_NOT_SUPPORTED";
        case(D3D12_CONSERVATIVE_RASTERIZATION_TIER_1):
            return "D3D12_CONSERVATIVE_RASTERIZATION_TIER_1";
        case(D3D12_CONSERVATIVE_RASTERIZATION_TIER_2):
            return "D3D12_CONSERVATIVE_RASTERIZATION_TIER_2";
        case(D3D12_CONSERVATIVE_RASTERIZATION_TIER_3):
            return "D3D12_CONSERVATIVE_RASTERIZATION_TIER_3";
        default:
            {
                std::string code = "Invalid D3D12_CONSERVATIVE_RASTERIZATION_TIER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_FORMAT_SUPPORT1 value)
{
    switch(value)
    {
        case(D3D12_FORMAT_SUPPORT1_NONE):
            return "D3D12_FORMAT_SUPPORT1_NONE";
        case(D3D12_FORMAT_SUPPORT1_BUFFER):
            return "D3D12_FORMAT_SUPPORT1_BUFFER";
        case(D3D12_FORMAT_SUPPORT1_IA_VERTEX_BUFFER):
            return "D3D12_FORMAT_SUPPORT1_IA_VERTEX_BUFFER";
        case(D3D12_FORMAT_SUPPORT1_IA_INDEX_BUFFER):
            return "D3D12_FORMAT_SUPPORT1_IA_INDEX_BUFFER";
        case(D3D12_FORMAT_SUPPORT1_SO_BUFFER):
            return "D3D12_FORMAT_SUPPORT1_SO_BUFFER";
        case(D3D12_FORMAT_SUPPORT1_TEXTURE1D):
            return "D3D12_FORMAT_SUPPORT1_TEXTURE1D";
        case(D3D12_FORMAT_SUPPORT1_TEXTURE2D):
            return "D3D12_FORMAT_SUPPORT1_TEXTURE2D";
        case(D3D12_FORMAT_SUPPORT1_TEXTURE3D):
            return "D3D12_FORMAT_SUPPORT1_TEXTURE3D";
        case(D3D12_FORMAT_SUPPORT1_TEXTURECUBE):
            return "D3D12_FORMAT_SUPPORT1_TEXTURECUBE";
        case(D3D12_FORMAT_SUPPORT1_SHADER_LOAD):
            return "D3D12_FORMAT_SUPPORT1_SHADER_LOAD";
        case(D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE):
            return "D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE";
        case(D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_COMPARISON):
            return "D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_COMPARISON";
        case(D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_MONO_TEXT):
            return "D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_MONO_TEXT";
        case(D3D12_FORMAT_SUPPORT1_MIP):
            return "D3D12_FORMAT_SUPPORT1_MIP";
        case(D3D12_FORMAT_SUPPORT1_RENDER_TARGET):
            return "D3D12_FORMAT_SUPPORT1_RENDER_TARGET";
        case(D3D12_FORMAT_SUPPORT1_BLENDABLE):
            return "D3D12_FORMAT_SUPPORT1_BLENDABLE";
        case(D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL):
            return "D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL";
        case(D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RESOLVE):
            return "D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RESOLVE";
        case(D3D12_FORMAT_SUPPORT1_DISPLAY):
            return "D3D12_FORMAT_SUPPORT1_DISPLAY";
        case(D3D12_FORMAT_SUPPORT1_CAST_WITHIN_BIT_LAYOUT):
            return "D3D12_FORMAT_SUPPORT1_CAST_WITHIN_BIT_LAYOUT";
        case(D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RENDERTARGET):
            return "D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RENDERTARGET";
        case(D3D12_FORMAT_SUPPORT1_MULTISAMPLE_LOAD):
            return "D3D12_FORMAT_SUPPORT1_MULTISAMPLE_LOAD";
        case(D3D12_FORMAT_SUPPORT1_SHADER_GATHER):
            return "D3D12_FORMAT_SUPPORT1_SHADER_GATHER";
        case(D3D12_FORMAT_SUPPORT1_BACK_BUFFER_CAST):
            return "D3D12_FORMAT_SUPPORT1_BACK_BUFFER_CAST";
        case(D3D12_FORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW):
            return "D3D12_FORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW";
        case(D3D12_FORMAT_SUPPORT1_SHADER_GATHER_COMPARISON):
            return "D3D12_FORMAT_SUPPORT1_SHADER_GATHER_COMPARISON";
        case(D3D12_FORMAT_SUPPORT1_DECODER_OUTPUT):
            return "D3D12_FORMAT_SUPPORT1_DECODER_OUTPUT";
        case(D3D12_FORMAT_SUPPORT1_VIDEO_PROCESSOR_OUTPUT):
            return "D3D12_FORMAT_SUPPORT1_VIDEO_PROCESSOR_OUTPUT";
        case(D3D12_FORMAT_SUPPORT1_VIDEO_PROCESSOR_INPUT):
            return "D3D12_FORMAT_SUPPORT1_VIDEO_PROCESSOR_INPUT";
        case(D3D12_FORMAT_SUPPORT1_VIDEO_ENCODER):
            return "D3D12_FORMAT_SUPPORT1_VIDEO_ENCODER";
        default:
            {
                std::string code = "Invalid D3D12_FORMAT_SUPPORT1(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_FORMAT_SUPPORT2 value)
{
    switch(value)
    {
        case(D3D12_FORMAT_SUPPORT2_NONE):
            return "D3D12_FORMAT_SUPPORT2_NONE";
        case(D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_ADD):
            return "D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_ADD";
        case(D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_BITWISE_OPS):
            return "D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_BITWISE_OPS";
        case(D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_COMPARE_STORE_OR_COMPARE_EXCHANGE):
            return "D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_COMPARE_STORE_OR_COMPARE_EXCHANGE";
        case(D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_EXCHANGE):
            return "D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_EXCHANGE";
        case(D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_SIGNED_MIN_OR_MAX):
            return "D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_SIGNED_MIN_OR_MAX";
        case(D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_UNSIGNED_MIN_OR_MAX):
            return "D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_UNSIGNED_MIN_OR_MAX";
        case(D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD):
            return "D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD";
        case(D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE):
            return "D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE";
        case(D3D12_FORMAT_SUPPORT2_OUTPUT_MERGER_LOGIC_OP):
            return "D3D12_FORMAT_SUPPORT2_OUTPUT_MERGER_LOGIC_OP";
        case(D3D12_FORMAT_SUPPORT2_TILED):
            return "D3D12_FORMAT_SUPPORT2_TILED";
        case(D3D12_FORMAT_SUPPORT2_MULTIPLANE_OVERLAY):
            return "D3D12_FORMAT_SUPPORT2_MULTIPLANE_OVERLAY";
        case(D3D12_FORMAT_SUPPORT2_SAMPLER_FEEDBACK):
            return "D3D12_FORMAT_SUPPORT2_SAMPLER_FEEDBACK";
        default:
            {
                std::string code = "Invalid D3D12_FORMAT_SUPPORT2(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS value)
{
    switch(value)
    {
        case(D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE):
            return "D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE";
        case(D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_TILED_RESOURCE):
            return "D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_TILED_RESOURCE";
        default:
            {
                std::string code = "Invalid D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_CROSS_NODE_SHARING_TIER value)
{
    switch(value)
    {
        case(D3D12_CROSS_NODE_SHARING_TIER_NOT_SUPPORTED):
            return "D3D12_CROSS_NODE_SHARING_TIER_NOT_SUPPORTED";
        case(D3D12_CROSS_NODE_SHARING_TIER_1_EMULATED):
            return "D3D12_CROSS_NODE_SHARING_TIER_1_EMULATED";
        case(D3D12_CROSS_NODE_SHARING_TIER_1):
            return "D3D12_CROSS_NODE_SHARING_TIER_1";
        case(D3D12_CROSS_NODE_SHARING_TIER_2):
            return "D3D12_CROSS_NODE_SHARING_TIER_2";
        case(D3D12_CROSS_NODE_SHARING_TIER_3):
            return "D3D12_CROSS_NODE_SHARING_TIER_3";
        default:
            {
                std::string code = "Invalid D3D12_CROSS_NODE_SHARING_TIER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RESOURCE_HEAP_TIER value)
{
    switch(value)
    {
        case(D3D12_RESOURCE_HEAP_TIER_1):
            return "D3D12_RESOURCE_HEAP_TIER_1";
        case(D3D12_RESOURCE_HEAP_TIER_2):
            return "D3D12_RESOURCE_HEAP_TIER_2";
        default:
            {
                std::string code = "Invalid D3D12_RESOURCE_HEAP_TIER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER value)
{
    switch(value)
    {
        case(D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER_NOT_SUPPORTED):
            return "D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER_NOT_SUPPORTED";
        case(D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER_1):
            return "D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER_1";
        case(D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER_2):
            return "D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER_2";
        default:
            {
                std::string code = "Invalid D3D12_PROGRAMMABLE_SAMPLE_POSITIONS_TIER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_VIEW_INSTANCING_TIER value)
{
    switch(value)
    {
        case(D3D12_VIEW_INSTANCING_TIER_NOT_SUPPORTED):
            return "D3D12_VIEW_INSTANCING_TIER_NOT_SUPPORTED";
        case(D3D12_VIEW_INSTANCING_TIER_1):
            return "D3D12_VIEW_INSTANCING_TIER_1";
        case(D3D12_VIEW_INSTANCING_TIER_2):
            return "D3D12_VIEW_INSTANCING_TIER_2";
        case(D3D12_VIEW_INSTANCING_TIER_3):
            return "D3D12_VIEW_INSTANCING_TIER_3";
        default:
            {
                std::string code = "Invalid D3D12_VIEW_INSTANCING_TIER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_ROOT_SIGNATURE_VERSION value)
{
    switch(value)
    {
        case(D3D_ROOT_SIGNATURE_VERSION_1):
            return "D3D_ROOT_SIGNATURE_VERSION_1";
        case(D3D_ROOT_SIGNATURE_VERSION_1_1):
            return "D3D_ROOT_SIGNATURE_VERSION_1_1";
        default:
            {
                std::string code = "Invalid D3D_ROOT_SIGNATURE_VERSION(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_SHADER_MODEL value)
{
    switch(value)
    {
        case(D3D_SHADER_MODEL_5_1):
            return "D3D_SHADER_MODEL_5_1";
        case(D3D_SHADER_MODEL_6_0):
            return "D3D_SHADER_MODEL_6_0";
        case(D3D_SHADER_MODEL_6_1):
            return "D3D_SHADER_MODEL_6_1";
        case(D3D_SHADER_MODEL_6_2):
            return "D3D_SHADER_MODEL_6_2";
        case(D3D_SHADER_MODEL_6_3):
            return "D3D_SHADER_MODEL_6_3";
        case(D3D_SHADER_MODEL_6_4):
            return "D3D_SHADER_MODEL_6_4";
        case(D3D_SHADER_MODEL_6_5):
            return "D3D_SHADER_MODEL_6_5";
        case(D3D_SHADER_MODEL_6_6):
            return "D3D_SHADER_MODEL_6_6";
        default:
            {
                std::string code = "Invalid D3D_SHADER_MODEL(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_SHADER_CACHE_SUPPORT_FLAGS value)
{
    switch(value)
    {
        case(D3D12_SHADER_CACHE_SUPPORT_NONE):
            return "D3D12_SHADER_CACHE_SUPPORT_NONE";
        case(D3D12_SHADER_CACHE_SUPPORT_SINGLE_PSO):
            return "D3D12_SHADER_CACHE_SUPPORT_SINGLE_PSO";
        case(D3D12_SHADER_CACHE_SUPPORT_LIBRARY):
            return "D3D12_SHADER_CACHE_SUPPORT_LIBRARY";
        case(D3D12_SHADER_CACHE_SUPPORT_AUTOMATIC_INPROC_CACHE):
            return "D3D12_SHADER_CACHE_SUPPORT_AUTOMATIC_INPROC_CACHE";
        case(D3D12_SHADER_CACHE_SUPPORT_AUTOMATIC_DISK_CACHE):
            return "D3D12_SHADER_CACHE_SUPPORT_AUTOMATIC_DISK_CACHE";
        default:
            {
                std::string code = "Invalid D3D12_SHADER_CACHE_SUPPORT_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_COMMAND_LIST_SUPPORT_FLAGS value)
{
    switch(value)
    {
        case(D3D12_COMMAND_LIST_SUPPORT_FLAG_NONE):
            return "D3D12_COMMAND_LIST_SUPPORT_FLAG_NONE";
        default:
            {
                std::string code = "Invalid D3D12_COMMAND_LIST_SUPPORT_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER value)
{
    switch(value)
    {
        case(D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER_0):
            return "D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER_0";
        case(D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER_1):
            return "D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER_1";
        case(D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER_2):
            return "D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER_2";
        default:
            {
                std::string code = "Invalid D3D12_SHARED_RESOURCE_COMPATIBILITY_TIER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_HEAP_SERIALIZATION_TIER value)
{
    switch(value)
    {
        case(D3D12_HEAP_SERIALIZATION_TIER_0):
            return "D3D12_HEAP_SERIALIZATION_TIER_0";
        case(D3D12_HEAP_SERIALIZATION_TIER_10):
            return "D3D12_HEAP_SERIALIZATION_TIER_10";
        default:
            {
                std::string code = "Invalid D3D12_HEAP_SERIALIZATION_TIER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RENDER_PASS_TIER value)
{
    switch(value)
    {
        case(D3D12_RENDER_PASS_TIER_0):
            return "D3D12_RENDER_PASS_TIER_0";
        case(D3D12_RENDER_PASS_TIER_1):
            return "D3D12_RENDER_PASS_TIER_1";
        case(D3D12_RENDER_PASS_TIER_2):
            return "D3D12_RENDER_PASS_TIER_2";
        default:
            {
                std::string code = "Invalid D3D12_RENDER_PASS_TIER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RAYTRACING_TIER value)
{
    switch(value)
    {
        case(D3D12_RAYTRACING_TIER_NOT_SUPPORTED):
            return "D3D12_RAYTRACING_TIER_NOT_SUPPORTED";
        case(D3D12_RAYTRACING_TIER_1_0):
            return "D3D12_RAYTRACING_TIER_1_0";
        case(D3D12_RAYTRACING_TIER_1_1):
            return "D3D12_RAYTRACING_TIER_1_1";
        default:
            {
                std::string code = "Invalid D3D12_RAYTRACING_TIER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_VARIABLE_SHADING_RATE_TIER value)
{
    switch(value)
    {
        case(D3D12_VARIABLE_SHADING_RATE_TIER_NOT_SUPPORTED):
            return "D3D12_VARIABLE_SHADING_RATE_TIER_NOT_SUPPORTED";
        case(D3D12_VARIABLE_SHADING_RATE_TIER_1):
            return "D3D12_VARIABLE_SHADING_RATE_TIER_1";
        case(D3D12_VARIABLE_SHADING_RATE_TIER_2):
            return "D3D12_VARIABLE_SHADING_RATE_TIER_2";
        default:
            {
                std::string code = "Invalid D3D12_VARIABLE_SHADING_RATE_TIER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_MESH_SHADER_TIER value)
{
    switch(value)
    {
        case(D3D12_MESH_SHADER_TIER_NOT_SUPPORTED):
            return "D3D12_MESH_SHADER_TIER_NOT_SUPPORTED";
        case(D3D12_MESH_SHADER_TIER_1):
            return "D3D12_MESH_SHADER_TIER_1";
        default:
            {
                std::string code = "Invalid D3D12_MESH_SHADER_TIER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_SAMPLER_FEEDBACK_TIER value)
{
    switch(value)
    {
        case(D3D12_SAMPLER_FEEDBACK_TIER_NOT_SUPPORTED):
            return "D3D12_SAMPLER_FEEDBACK_TIER_NOT_SUPPORTED";
        case(D3D12_SAMPLER_FEEDBACK_TIER_0_9):
            return "D3D12_SAMPLER_FEEDBACK_TIER_0_9";
        case(D3D12_SAMPLER_FEEDBACK_TIER_1_0):
            return "D3D12_SAMPLER_FEEDBACK_TIER_1_0";
        default:
            {
                std::string code = "Invalid D3D12_SAMPLER_FEEDBACK_TIER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_HEAP_TYPE value)
{
    switch(value)
    {
        case(D3D12_HEAP_TYPE_DEFAULT):
            return "D3D12_HEAP_TYPE_DEFAULT";
        case(D3D12_HEAP_TYPE_UPLOAD):
            return "D3D12_HEAP_TYPE_UPLOAD";
        case(D3D12_HEAP_TYPE_READBACK):
            return "D3D12_HEAP_TYPE_READBACK";
        case(D3D12_HEAP_TYPE_CUSTOM):
            return "D3D12_HEAP_TYPE_CUSTOM";
        default:
            {
                std::string code = "Invalid D3D12_HEAP_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_CPU_PAGE_PROPERTY value)
{
    switch(value)
    {
        case(D3D12_CPU_PAGE_PROPERTY_UNKNOWN):
            return "D3D12_CPU_PAGE_PROPERTY_UNKNOWN";
        case(D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE):
            return "D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE";
        case(D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE):
            return "D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE";
        case(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK):
            return "D3D12_CPU_PAGE_PROPERTY_WRITE_BACK";
        default:
            {
                std::string code = "Invalid D3D12_CPU_PAGE_PROPERTY(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_MEMORY_POOL value)
{
    switch(value)
    {
        case(D3D12_MEMORY_POOL_UNKNOWN):
            return "D3D12_MEMORY_POOL_UNKNOWN";
        case(D3D12_MEMORY_POOL_L0):
            return "D3D12_MEMORY_POOL_L0";
        case(D3D12_MEMORY_POOL_L1):
            return "D3D12_MEMORY_POOL_L1";
        default:
            {
                std::string code = "Invalid D3D12_MEMORY_POOL(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_HEAP_FLAGS value)
{
    switch(value)
    {
        case(D3D12_HEAP_FLAG_NONE):
            return "D3D12_HEAP_FLAG_NONE";
        case(D3D12_HEAP_FLAG_SHARED):
            return "D3D12_HEAP_FLAG_SHARED";
        case(D3D12_HEAP_FLAG_DENY_BUFFERS):
            return "D3D12_HEAP_FLAG_DENY_BUFFERS";
        case(D3D12_HEAP_FLAG_ALLOW_DISPLAY):
            return "D3D12_HEAP_FLAG_ALLOW_DISPLAY";
        case(D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER):
            return "D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER";
        case(D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES):
            return "D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES";
        case(D3D12_HEAP_FLAG_DENY_NON_RT_DS_TEXTURES):
            return "D3D12_HEAP_FLAG_DENY_NON_RT_DS_TEXTURES";
        case(D3D12_HEAP_FLAG_HARDWARE_PROTECTED):
            return "D3D12_HEAP_FLAG_HARDWARE_PROTECTED";
        case(D3D12_HEAP_FLAG_ALLOW_WRITE_WATCH):
            return "D3D12_HEAP_FLAG_ALLOW_WRITE_WATCH";
        case(D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS):
            return "D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS";
        case(D3D12_HEAP_FLAG_CREATE_NOT_RESIDENT):
            return "D3D12_HEAP_FLAG_CREATE_NOT_RESIDENT";
        case(D3D12_HEAP_FLAG_CREATE_NOT_ZEROED):
            return "D3D12_HEAP_FLAG_CREATE_NOT_ZEROED";
        case(D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS):
            return "D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS";
        case(D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES):
            return "D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES";
        case(D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES):
            return "D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES";
        default:
            {
                std::string code = "Invalid D3D12_HEAP_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RESOURCE_DIMENSION value)
{
    switch(value)
    {
        case(D3D12_RESOURCE_DIMENSION_UNKNOWN):
            return "D3D12_RESOURCE_DIMENSION_UNKNOWN";
        case(D3D12_RESOURCE_DIMENSION_BUFFER):
            return "D3D12_RESOURCE_DIMENSION_BUFFER";
        case(D3D12_RESOURCE_DIMENSION_TEXTURE1D):
            return "D3D12_RESOURCE_DIMENSION_TEXTURE1D";
        case(D3D12_RESOURCE_DIMENSION_TEXTURE2D):
            return "D3D12_RESOURCE_DIMENSION_TEXTURE2D";
        case(D3D12_RESOURCE_DIMENSION_TEXTURE3D):
            return "D3D12_RESOURCE_DIMENSION_TEXTURE3D";
        default:
            {
                std::string code = "Invalid D3D12_RESOURCE_DIMENSION(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_TEXTURE_LAYOUT value)
{
    switch(value)
    {
        case(D3D12_TEXTURE_LAYOUT_UNKNOWN):
            return "D3D12_TEXTURE_LAYOUT_UNKNOWN";
        case(D3D12_TEXTURE_LAYOUT_ROW_MAJOR):
            return "D3D12_TEXTURE_LAYOUT_ROW_MAJOR";
        case(D3D12_TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE):
            return "D3D12_TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE";
        case(D3D12_TEXTURE_LAYOUT_64KB_STANDARD_SWIZZLE):
            return "D3D12_TEXTURE_LAYOUT_64KB_STANDARD_SWIZZLE";
        default:
            {
                std::string code = "Invalid D3D12_TEXTURE_LAYOUT(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RESOURCE_FLAGS value)
{
    switch(value)
    {
        case(D3D12_RESOURCE_FLAG_NONE):
            return "D3D12_RESOURCE_FLAG_NONE";
        case(D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET):
            return "D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET";
        case(D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL):
            return "D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL";
        case(D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS):
            return "D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS";
        case(D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE):
            return "D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE";
        case(D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER):
            return "D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER";
        case(D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS):
            return "D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS";
        case(D3D12_RESOURCE_FLAG_VIDEO_DECODE_REFERENCE_ONLY):
            return "D3D12_RESOURCE_FLAG_VIDEO_DECODE_REFERENCE_ONLY";
        default:
            {
                std::string code = "Invalid D3D12_RESOURCE_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_TILE_RANGE_FLAGS value)
{
    switch(value)
    {
        case(D3D12_TILE_RANGE_FLAG_NONE):
            return "D3D12_TILE_RANGE_FLAG_NONE";
        case(D3D12_TILE_RANGE_FLAG_NULL):
            return "D3D12_TILE_RANGE_FLAG_NULL";
        case(D3D12_TILE_RANGE_FLAG_SKIP):
            return "D3D12_TILE_RANGE_FLAG_SKIP";
        case(D3D12_TILE_RANGE_FLAG_REUSE_SINGLE_TILE):
            return "D3D12_TILE_RANGE_FLAG_REUSE_SINGLE_TILE";
        default:
            {
                std::string code = "Invalid D3D12_TILE_RANGE_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_TILE_MAPPING_FLAGS value)
{
    switch(value)
    {
        case(D3D12_TILE_MAPPING_FLAG_NONE):
            return "D3D12_TILE_MAPPING_FLAG_NONE";
        case(D3D12_TILE_MAPPING_FLAG_NO_HAZARD):
            return "D3D12_TILE_MAPPING_FLAG_NO_HAZARD";
        default:
            {
                std::string code = "Invalid D3D12_TILE_MAPPING_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_TILE_COPY_FLAGS value)
{
    switch(value)
    {
        case(D3D12_TILE_COPY_FLAG_NONE):
            return "D3D12_TILE_COPY_FLAG_NONE";
        case(D3D12_TILE_COPY_FLAG_NO_HAZARD):
            return "D3D12_TILE_COPY_FLAG_NO_HAZARD";
        case(D3D12_TILE_COPY_FLAG_LINEAR_BUFFER_TO_SWIZZLED_TILED_RESOURCE):
            return "D3D12_TILE_COPY_FLAG_LINEAR_BUFFER_TO_SWIZZLED_TILED_RESOURCE";
        case(D3D12_TILE_COPY_FLAG_SWIZZLED_TILED_RESOURCE_TO_LINEAR_BUFFER):
            return "D3D12_TILE_COPY_FLAG_SWIZZLED_TILED_RESOURCE_TO_LINEAR_BUFFER";
        default:
            {
                std::string code = "Invalid D3D12_TILE_COPY_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RESOURCE_STATES value)
{
    switch(value)
    {
        case(D3D12_RESOURCE_STATE_COMMON):
            return "D3D12_RESOURCE_STATE_COMMON";
        case(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER):
            return "D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER";
        case(D3D12_RESOURCE_STATE_INDEX_BUFFER):
            return "D3D12_RESOURCE_STATE_INDEX_BUFFER";
        case(D3D12_RESOURCE_STATE_RENDER_TARGET):
            return "D3D12_RESOURCE_STATE_RENDER_TARGET";
        case(D3D12_RESOURCE_STATE_UNORDERED_ACCESS):
            return "D3D12_RESOURCE_STATE_UNORDERED_ACCESS";
        case(D3D12_RESOURCE_STATE_DEPTH_WRITE):
            return "D3D12_RESOURCE_STATE_DEPTH_WRITE";
        case(D3D12_RESOURCE_STATE_DEPTH_READ):
            return "D3D12_RESOURCE_STATE_DEPTH_READ";
        case(D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE):
            return "D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE";
        case(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE):
            return "D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE";
        case(D3D12_RESOURCE_STATE_STREAM_OUT):
            return "D3D12_RESOURCE_STATE_STREAM_OUT";
        case(D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT):
            return "D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT";
        case(D3D12_RESOURCE_STATE_COPY_DEST):
            return "D3D12_RESOURCE_STATE_COPY_DEST";
        case(D3D12_RESOURCE_STATE_COPY_SOURCE):
            return "D3D12_RESOURCE_STATE_COPY_SOURCE";
        case(D3D12_RESOURCE_STATE_RESOLVE_DEST):
            return "D3D12_RESOURCE_STATE_RESOLVE_DEST";
        case(D3D12_RESOURCE_STATE_RESOLVE_SOURCE):
            return "D3D12_RESOURCE_STATE_RESOLVE_SOURCE";
        case(D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE):
            return "D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE";
        case(D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE):
            return "D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE";
        case(D3D12_RESOURCE_STATE_VIDEO_DECODE_READ):
            return "D3D12_RESOURCE_STATE_VIDEO_DECODE_READ";
        case(D3D12_RESOURCE_STATE_VIDEO_DECODE_WRITE):
            return "D3D12_RESOURCE_STATE_VIDEO_DECODE_WRITE";
        case(D3D12_RESOURCE_STATE_VIDEO_PROCESS_READ):
            return "D3D12_RESOURCE_STATE_VIDEO_PROCESS_READ";
        case(D3D12_RESOURCE_STATE_VIDEO_PROCESS_WRITE):
            return "D3D12_RESOURCE_STATE_VIDEO_PROCESS_WRITE";
        case(D3D12_RESOURCE_STATE_VIDEO_ENCODE_READ):
            return "D3D12_RESOURCE_STATE_VIDEO_ENCODE_READ";
        case(D3D12_RESOURCE_STATE_VIDEO_ENCODE_WRITE):
            return "D3D12_RESOURCE_STATE_VIDEO_ENCODE_WRITE";
        default:
            {
                std::string code = "Invalid D3D12_RESOURCE_STATES(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RESOURCE_BARRIER_TYPE value)
{
    switch(value)
    {
        case(D3D12_RESOURCE_BARRIER_TYPE_TRANSITION):
            return "D3D12_RESOURCE_BARRIER_TYPE_TRANSITION";
        case(D3D12_RESOURCE_BARRIER_TYPE_ALIASING):
            return "D3D12_RESOURCE_BARRIER_TYPE_ALIASING";
        case(D3D12_RESOURCE_BARRIER_TYPE_UAV):
            return "D3D12_RESOURCE_BARRIER_TYPE_UAV";
        default:
            {
                std::string code = "Invalid D3D12_RESOURCE_BARRIER_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RESOURCE_BARRIER_FLAGS value)
{
    switch(value)
    {
        case(D3D12_RESOURCE_BARRIER_FLAG_NONE):
            return "D3D12_RESOURCE_BARRIER_FLAG_NONE";
        case(D3D12_RESOURCE_BARRIER_FLAG_BEGIN_ONLY):
            return "D3D12_RESOURCE_BARRIER_FLAG_BEGIN_ONLY";
        case(D3D12_RESOURCE_BARRIER_FLAG_END_ONLY):
            return "D3D12_RESOURCE_BARRIER_FLAG_END_ONLY";
        default:
            {
                std::string code = "Invalid D3D12_RESOURCE_BARRIER_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_TEXTURE_COPY_TYPE value)
{
    switch(value)
    {
        case(D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX):
            return "D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX";
        case(D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT):
            return "D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT";
        default:
            {
                std::string code = "Invalid D3D12_TEXTURE_COPY_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RESOLVE_MODE value)
{
    switch(value)
    {
        case(D3D12_RESOLVE_MODE_DECOMPRESS):
            return "D3D12_RESOLVE_MODE_DECOMPRESS";
        case(D3D12_RESOLVE_MODE_MIN):
            return "D3D12_RESOLVE_MODE_MIN";
        case(D3D12_RESOLVE_MODE_MAX):
            return "D3D12_RESOLVE_MODE_MAX";
        case(D3D12_RESOLVE_MODE_AVERAGE):
            return "D3D12_RESOLVE_MODE_AVERAGE";
        case(D3D12_RESOLVE_MODE_ENCODE_SAMPLER_FEEDBACK):
            return "D3D12_RESOLVE_MODE_ENCODE_SAMPLER_FEEDBACK";
        case(D3D12_RESOLVE_MODE_DECODE_SAMPLER_FEEDBACK):
            return "D3D12_RESOLVE_MODE_DECODE_SAMPLER_FEEDBACK";
        default:
            {
                std::string code = "Invalid D3D12_RESOLVE_MODE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_VIEW_INSTANCING_FLAGS value)
{
    switch(value)
    {
        case(D3D12_VIEW_INSTANCING_FLAG_NONE):
            return "D3D12_VIEW_INSTANCING_FLAG_NONE";
        case(D3D12_VIEW_INSTANCING_FLAG_ENABLE_VIEW_INSTANCE_MASKING):
            return "D3D12_VIEW_INSTANCING_FLAG_ENABLE_VIEW_INSTANCE_MASKING";
        default:
            {
                std::string code = "Invalid D3D12_VIEW_INSTANCING_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_SHADER_COMPONENT_MAPPING value)
{
    switch(value)
    {
        case(D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_0):
            return "D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_0";
        case(D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_1):
            return "D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_1";
        case(D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_2):
            return "D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_2";
        case(D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_3):
            return "D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_3";
        case(D3D12_SHADER_COMPONENT_MAPPING_FORCE_VALUE_0):
            return "D3D12_SHADER_COMPONENT_MAPPING_FORCE_VALUE_0";
        case(D3D12_SHADER_COMPONENT_MAPPING_FORCE_VALUE_1):
            return "D3D12_SHADER_COMPONENT_MAPPING_FORCE_VALUE_1";
        default:
            {
                std::string code = "Invalid D3D12_SHADER_COMPONENT_MAPPING(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_BUFFER_SRV_FLAGS value)
{
    switch(value)
    {
        case(D3D12_BUFFER_SRV_FLAG_NONE):
            return "D3D12_BUFFER_SRV_FLAG_NONE";
        case(D3D12_BUFFER_SRV_FLAG_RAW):
            return "D3D12_BUFFER_SRV_FLAG_RAW";
        default:
            {
                std::string code = "Invalid D3D12_BUFFER_SRV_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_SRV_DIMENSION value)
{
    switch(value)
    {
        case(D3D12_SRV_DIMENSION_UNKNOWN):
            return "D3D12_SRV_DIMENSION_UNKNOWN";
        case(D3D12_SRV_DIMENSION_BUFFER):
            return "D3D12_SRV_DIMENSION_BUFFER";
        case(D3D12_SRV_DIMENSION_TEXTURE1D):
            return "D3D12_SRV_DIMENSION_TEXTURE1D";
        case(D3D12_SRV_DIMENSION_TEXTURE1DARRAY):
            return "D3D12_SRV_DIMENSION_TEXTURE1DARRAY";
        case(D3D12_SRV_DIMENSION_TEXTURE2D):
            return "D3D12_SRV_DIMENSION_TEXTURE2D";
        case(D3D12_SRV_DIMENSION_TEXTURE2DARRAY):
            return "D3D12_SRV_DIMENSION_TEXTURE2DARRAY";
        case(D3D12_SRV_DIMENSION_TEXTURE2DMS):
            return "D3D12_SRV_DIMENSION_TEXTURE2DMS";
        case(D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY):
            return "D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY";
        case(D3D12_SRV_DIMENSION_TEXTURE3D):
            return "D3D12_SRV_DIMENSION_TEXTURE3D";
        case(D3D12_SRV_DIMENSION_TEXTURECUBE):
            return "D3D12_SRV_DIMENSION_TEXTURECUBE";
        case(D3D12_SRV_DIMENSION_TEXTURECUBEARRAY):
            return "D3D12_SRV_DIMENSION_TEXTURECUBEARRAY";
        case(D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE):
            return "D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE";
        default:
            {
                std::string code = "Invalid D3D12_SRV_DIMENSION(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_FILTER value)
{
    switch(value)
    {
        case(D3D12_FILTER_MIN_MAG_MIP_POINT):
            return "D3D12_FILTER_MIN_MAG_MIP_POINT";
        case(D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR):
            return "D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR";
        case(D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT):
            return "D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT";
        case(D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR):
            return "D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR";
        case(D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT):
            return "D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT";
        case(D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR):
            return "D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR";
        case(D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT):
            return "D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT";
        case(D3D12_FILTER_MIN_MAG_MIP_LINEAR):
            return "D3D12_FILTER_MIN_MAG_MIP_LINEAR";
        case(D3D12_FILTER_ANISOTROPIC):
            return "D3D12_FILTER_ANISOTROPIC";
        case(D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT):
            return "D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT";
        case(D3D12_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR):
            return "D3D12_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR";
        case(D3D12_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT):
            return "D3D12_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT";
        case(D3D12_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR):
            return "D3D12_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR";
        case(D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT):
            return "D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT";
        case(D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR):
            return "D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR";
        case(D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT):
            return "D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT";
        case(D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR):
            return "D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR";
        case(D3D12_FILTER_COMPARISON_ANISOTROPIC):
            return "D3D12_FILTER_COMPARISON_ANISOTROPIC";
        case(D3D12_FILTER_MINIMUM_MIN_MAG_MIP_POINT):
            return "D3D12_FILTER_MINIMUM_MIN_MAG_MIP_POINT";
        case(D3D12_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR):
            return "D3D12_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR";
        case(D3D12_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT):
            return "D3D12_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT";
        case(D3D12_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR):
            return "D3D12_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR";
        case(D3D12_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT):
            return "D3D12_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT";
        case(D3D12_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR):
            return "D3D12_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR";
        case(D3D12_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT):
            return "D3D12_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT";
        case(D3D12_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR):
            return "D3D12_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR";
        case(D3D12_FILTER_MINIMUM_ANISOTROPIC):
            return "D3D12_FILTER_MINIMUM_ANISOTROPIC";
        case(D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_POINT):
            return "D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_POINT";
        case(D3D12_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR):
            return "D3D12_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR";
        case(D3D12_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT):
            return "D3D12_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT";
        case(D3D12_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR):
            return "D3D12_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR";
        case(D3D12_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT):
            return "D3D12_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT";
        case(D3D12_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR):
            return "D3D12_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR";
        case(D3D12_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT):
            return "D3D12_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT";
        case(D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR):
            return "D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR";
        case(D3D12_FILTER_MAXIMUM_ANISOTROPIC):
            return "D3D12_FILTER_MAXIMUM_ANISOTROPIC";
        default:
            {
                std::string code = "Invalid D3D12_FILTER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_FILTER_TYPE value)
{
    switch(value)
    {
        case(D3D12_FILTER_TYPE_POINT):
            return "D3D12_FILTER_TYPE_POINT";
        case(D3D12_FILTER_TYPE_LINEAR):
            return "D3D12_FILTER_TYPE_LINEAR";
        default:
            {
                std::string code = "Invalid D3D12_FILTER_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_FILTER_REDUCTION_TYPE value)
{
    switch(value)
    {
        case(D3D12_FILTER_REDUCTION_TYPE_STANDARD):
            return "D3D12_FILTER_REDUCTION_TYPE_STANDARD";
        case(D3D12_FILTER_REDUCTION_TYPE_COMPARISON):
            return "D3D12_FILTER_REDUCTION_TYPE_COMPARISON";
        case(D3D12_FILTER_REDUCTION_TYPE_MINIMUM):
            return "D3D12_FILTER_REDUCTION_TYPE_MINIMUM";
        case(D3D12_FILTER_REDUCTION_TYPE_MAXIMUM):
            return "D3D12_FILTER_REDUCTION_TYPE_MAXIMUM";
        default:
            {
                std::string code = "Invalid D3D12_FILTER_REDUCTION_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_TEXTURE_ADDRESS_MODE value)
{
    switch(value)
    {
        case(D3D12_TEXTURE_ADDRESS_MODE_WRAP):
            return "D3D12_TEXTURE_ADDRESS_MODE_WRAP";
        case(D3D12_TEXTURE_ADDRESS_MODE_MIRROR):
            return "D3D12_TEXTURE_ADDRESS_MODE_MIRROR";
        case(D3D12_TEXTURE_ADDRESS_MODE_CLAMP):
            return "D3D12_TEXTURE_ADDRESS_MODE_CLAMP";
        case(D3D12_TEXTURE_ADDRESS_MODE_BORDER):
            return "D3D12_TEXTURE_ADDRESS_MODE_BORDER";
        case(D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE):
            return "D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE";
        default:
            {
                std::string code = "Invalid D3D12_TEXTURE_ADDRESS_MODE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_BUFFER_UAV_FLAGS value)
{
    switch(value)
    {
        case(D3D12_BUFFER_UAV_FLAG_NONE):
            return "D3D12_BUFFER_UAV_FLAG_NONE";
        case(D3D12_BUFFER_UAV_FLAG_RAW):
            return "D3D12_BUFFER_UAV_FLAG_RAW";
        default:
            {
                std::string code = "Invalid D3D12_BUFFER_UAV_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_UAV_DIMENSION value)
{
    switch(value)
    {
        case(D3D12_UAV_DIMENSION_UNKNOWN):
            return "D3D12_UAV_DIMENSION_UNKNOWN";
        case(D3D12_UAV_DIMENSION_BUFFER):
            return "D3D12_UAV_DIMENSION_BUFFER";
        case(D3D12_UAV_DIMENSION_TEXTURE1D):
            return "D3D12_UAV_DIMENSION_TEXTURE1D";
        case(D3D12_UAV_DIMENSION_TEXTURE1DARRAY):
            return "D3D12_UAV_DIMENSION_TEXTURE1DARRAY";
        case(D3D12_UAV_DIMENSION_TEXTURE2D):
            return "D3D12_UAV_DIMENSION_TEXTURE2D";
        case(D3D12_UAV_DIMENSION_TEXTURE2DARRAY):
            return "D3D12_UAV_DIMENSION_TEXTURE2DARRAY";
        case(D3D12_UAV_DIMENSION_TEXTURE3D):
            return "D3D12_UAV_DIMENSION_TEXTURE3D";
        default:
            {
                std::string code = "Invalid D3D12_UAV_DIMENSION(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RTV_DIMENSION value)
{
    switch(value)
    {
        case(D3D12_RTV_DIMENSION_UNKNOWN):
            return "D3D12_RTV_DIMENSION_UNKNOWN";
        case(D3D12_RTV_DIMENSION_BUFFER):
            return "D3D12_RTV_DIMENSION_BUFFER";
        case(D3D12_RTV_DIMENSION_TEXTURE1D):
            return "D3D12_RTV_DIMENSION_TEXTURE1D";
        case(D3D12_RTV_DIMENSION_TEXTURE1DARRAY):
            return "D3D12_RTV_DIMENSION_TEXTURE1DARRAY";
        case(D3D12_RTV_DIMENSION_TEXTURE2D):
            return "D3D12_RTV_DIMENSION_TEXTURE2D";
        case(D3D12_RTV_DIMENSION_TEXTURE2DARRAY):
            return "D3D12_RTV_DIMENSION_TEXTURE2DARRAY";
        case(D3D12_RTV_DIMENSION_TEXTURE2DMS):
            return "D3D12_RTV_DIMENSION_TEXTURE2DMS";
        case(D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY):
            return "D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY";
        case(D3D12_RTV_DIMENSION_TEXTURE3D):
            return "D3D12_RTV_DIMENSION_TEXTURE3D";
        default:
            {
                std::string code = "Invalid D3D12_RTV_DIMENSION(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DSV_FLAGS value)
{
    switch(value)
    {
        case(D3D12_DSV_FLAG_NONE):
            return "D3D12_DSV_FLAG_NONE";
        case(D3D12_DSV_FLAG_READ_ONLY_DEPTH):
            return "D3D12_DSV_FLAG_READ_ONLY_DEPTH";
        case(D3D12_DSV_FLAG_READ_ONLY_STENCIL):
            return "D3D12_DSV_FLAG_READ_ONLY_STENCIL";
        default:
            {
                std::string code = "Invalid D3D12_DSV_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DSV_DIMENSION value)
{
    switch(value)
    {
        case(D3D12_DSV_DIMENSION_UNKNOWN):
            return "D3D12_DSV_DIMENSION_UNKNOWN";
        case(D3D12_DSV_DIMENSION_TEXTURE1D):
            return "D3D12_DSV_DIMENSION_TEXTURE1D";
        case(D3D12_DSV_DIMENSION_TEXTURE1DARRAY):
            return "D3D12_DSV_DIMENSION_TEXTURE1DARRAY";
        case(D3D12_DSV_DIMENSION_TEXTURE2D):
            return "D3D12_DSV_DIMENSION_TEXTURE2D";
        case(D3D12_DSV_DIMENSION_TEXTURE2DARRAY):
            return "D3D12_DSV_DIMENSION_TEXTURE2DARRAY";
        case(D3D12_DSV_DIMENSION_TEXTURE2DMS):
            return "D3D12_DSV_DIMENSION_TEXTURE2DMS";
        case(D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY):
            return "D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY";
        default:
            {
                std::string code = "Invalid D3D12_DSV_DIMENSION(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_CLEAR_FLAGS value)
{
    switch(value)
    {
        case(D3D12_CLEAR_FLAG_DEPTH):
            return "D3D12_CLEAR_FLAG_DEPTH";
        case(D3D12_CLEAR_FLAG_STENCIL):
            return "D3D12_CLEAR_FLAG_STENCIL";
        default:
            {
                std::string code = "Invalid D3D12_CLEAR_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_FENCE_FLAGS value)
{
    switch(value)
    {
        case(D3D12_FENCE_FLAG_NONE):
            return "D3D12_FENCE_FLAG_NONE";
        case(D3D12_FENCE_FLAG_SHARED):
            return "D3D12_FENCE_FLAG_SHARED";
        case(D3D12_FENCE_FLAG_SHARED_CROSS_ADAPTER):
            return "D3D12_FENCE_FLAG_SHARED_CROSS_ADAPTER";
        case(D3D12_FENCE_FLAG_NON_MONITORED):
            return "D3D12_FENCE_FLAG_NON_MONITORED";
        default:
            {
                std::string code = "Invalid D3D12_FENCE_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DESCRIPTOR_HEAP_TYPE value)
{
    switch(value)
    {
        case(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV):
            return "D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV";
        case(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER):
            return "D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER";
        case(D3D12_DESCRIPTOR_HEAP_TYPE_RTV):
            return "D3D12_DESCRIPTOR_HEAP_TYPE_RTV";
        case(D3D12_DESCRIPTOR_HEAP_TYPE_DSV):
            return "D3D12_DESCRIPTOR_HEAP_TYPE_DSV";
        case(D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES):
            return "D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES";
        default:
            {
                std::string code = "Invalid D3D12_DESCRIPTOR_HEAP_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DESCRIPTOR_HEAP_FLAGS value)
{
    switch(value)
    {
        case(D3D12_DESCRIPTOR_HEAP_FLAG_NONE):
            return "D3D12_DESCRIPTOR_HEAP_FLAG_NONE";
        case(D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE):
            return "D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE";
        default:
            {
                std::string code = "Invalid D3D12_DESCRIPTOR_HEAP_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DESCRIPTOR_RANGE_TYPE value)
{
    switch(value)
    {
        case(D3D12_DESCRIPTOR_RANGE_TYPE_SRV):
            return "D3D12_DESCRIPTOR_RANGE_TYPE_SRV";
        case(D3D12_DESCRIPTOR_RANGE_TYPE_UAV):
            return "D3D12_DESCRIPTOR_RANGE_TYPE_UAV";
        case(D3D12_DESCRIPTOR_RANGE_TYPE_CBV):
            return "D3D12_DESCRIPTOR_RANGE_TYPE_CBV";
        case(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER):
            return "D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER";
        default:
            {
                std::string code = "Invalid D3D12_DESCRIPTOR_RANGE_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_SHADER_VISIBILITY value)
{
    switch(value)
    {
        case(D3D12_SHADER_VISIBILITY_ALL):
            return "D3D12_SHADER_VISIBILITY_ALL";
        case(D3D12_SHADER_VISIBILITY_VERTEX):
            return "D3D12_SHADER_VISIBILITY_VERTEX";
        case(D3D12_SHADER_VISIBILITY_HULL):
            return "D3D12_SHADER_VISIBILITY_HULL";
        case(D3D12_SHADER_VISIBILITY_DOMAIN):
            return "D3D12_SHADER_VISIBILITY_DOMAIN";
        case(D3D12_SHADER_VISIBILITY_GEOMETRY):
            return "D3D12_SHADER_VISIBILITY_GEOMETRY";
        case(D3D12_SHADER_VISIBILITY_PIXEL):
            return "D3D12_SHADER_VISIBILITY_PIXEL";
        case(D3D12_SHADER_VISIBILITY_AMPLIFICATION):
            return "D3D12_SHADER_VISIBILITY_AMPLIFICATION";
        case(D3D12_SHADER_VISIBILITY_MESH):
            return "D3D12_SHADER_VISIBILITY_MESH";
        default:
            {
                std::string code = "Invalid D3D12_SHADER_VISIBILITY(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_ROOT_PARAMETER_TYPE value)
{
    switch(value)
    {
        case(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE):
            return "D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE";
        case(D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS):
            return "D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS";
        case(D3D12_ROOT_PARAMETER_TYPE_CBV):
            return "D3D12_ROOT_PARAMETER_TYPE_CBV";
        case(D3D12_ROOT_PARAMETER_TYPE_SRV):
            return "D3D12_ROOT_PARAMETER_TYPE_SRV";
        case(D3D12_ROOT_PARAMETER_TYPE_UAV):
            return "D3D12_ROOT_PARAMETER_TYPE_UAV";
        default:
            {
                std::string code = "Invalid D3D12_ROOT_PARAMETER_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_ROOT_SIGNATURE_FLAGS value)
{
    switch(value)
    {
        case(D3D12_ROOT_SIGNATURE_FLAG_NONE):
            return "D3D12_ROOT_SIGNATURE_FLAG_NONE";
        case(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT):
            return "D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT";
        case(D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS):
            return "D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS";
        case(D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS):
            return "D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS";
        case(D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS):
            return "D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS";
        case(D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS):
            return "D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS";
        case(D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS):
            return "D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS";
        case(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT):
            return "D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT";
        case(D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE):
            return "D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE";
        case(D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS):
            return "D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS";
        case(D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS):
            return "D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS";
        default:
            {
                std::string code = "Invalid D3D12_ROOT_SIGNATURE_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_STATIC_BORDER_COLOR value)
{
    switch(value)
    {
        case(D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK):
            return "D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK";
        case(D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK):
            return "D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK";
        case(D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE):
            return "D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE";
        default:
            {
                std::string code = "Invalid D3D12_STATIC_BORDER_COLOR(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DESCRIPTOR_RANGE_FLAGS value)
{
    switch(value)
    {
        case(D3D12_DESCRIPTOR_RANGE_FLAG_NONE):
            return "D3D12_DESCRIPTOR_RANGE_FLAG_NONE";
        case(D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE):
            return "D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE";
        case(D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE):
            return "D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE";
        case(D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE):
            return "D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE";
        case(D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC):
            return "D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC";
        case(D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_STATIC_KEEPING_BUFFER_BOUNDS_CHECKS):
            return "D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_STATIC_KEEPING_BUFFER_BOUNDS_CHECKS";
        default:
            {
                std::string code = "Invalid D3D12_DESCRIPTOR_RANGE_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_ROOT_DESCRIPTOR_FLAGS value)
{
    switch(value)
    {
        case(D3D12_ROOT_DESCRIPTOR_FLAG_NONE):
            return "D3D12_ROOT_DESCRIPTOR_FLAG_NONE";
        case(D3D12_ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE):
            return "D3D12_ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE";
        case(D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE):
            return "D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE";
        case(D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC):
            return "D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC";
        default:
            {
                std::string code = "Invalid D3D12_ROOT_DESCRIPTOR_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_QUERY_HEAP_TYPE value)
{
    switch(value)
    {
        case(D3D12_QUERY_HEAP_TYPE_OCCLUSION):
            return "D3D12_QUERY_HEAP_TYPE_OCCLUSION";
        case(D3D12_QUERY_HEAP_TYPE_TIMESTAMP):
            return "D3D12_QUERY_HEAP_TYPE_TIMESTAMP";
        case(D3D12_QUERY_HEAP_TYPE_PIPELINE_STATISTICS):
            return "D3D12_QUERY_HEAP_TYPE_PIPELINE_STATISTICS";
        case(D3D12_QUERY_HEAP_TYPE_SO_STATISTICS):
            return "D3D12_QUERY_HEAP_TYPE_SO_STATISTICS";
        case(D3D12_QUERY_HEAP_TYPE_VIDEO_DECODE_STATISTICS):
            return "D3D12_QUERY_HEAP_TYPE_VIDEO_DECODE_STATISTICS";
        case(D3D12_QUERY_HEAP_TYPE_COPY_QUEUE_TIMESTAMP):
            return "D3D12_QUERY_HEAP_TYPE_COPY_QUEUE_TIMESTAMP";
        default:
            {
                std::string code = "Invalid D3D12_QUERY_HEAP_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_QUERY_TYPE value)
{
    switch(value)
    {
        case(D3D12_QUERY_TYPE_OCCLUSION):
            return "D3D12_QUERY_TYPE_OCCLUSION";
        case(D3D12_QUERY_TYPE_BINARY_OCCLUSION):
            return "D3D12_QUERY_TYPE_BINARY_OCCLUSION";
        case(D3D12_QUERY_TYPE_TIMESTAMP):
            return "D3D12_QUERY_TYPE_TIMESTAMP";
        case(D3D12_QUERY_TYPE_PIPELINE_STATISTICS):
            return "D3D12_QUERY_TYPE_PIPELINE_STATISTICS";
        case(D3D12_QUERY_TYPE_SO_STATISTICS_STREAM0):
            return "D3D12_QUERY_TYPE_SO_STATISTICS_STREAM0";
        case(D3D12_QUERY_TYPE_SO_STATISTICS_STREAM1):
            return "D3D12_QUERY_TYPE_SO_STATISTICS_STREAM1";
        case(D3D12_QUERY_TYPE_SO_STATISTICS_STREAM2):
            return "D3D12_QUERY_TYPE_SO_STATISTICS_STREAM2";
        case(D3D12_QUERY_TYPE_SO_STATISTICS_STREAM3):
            return "D3D12_QUERY_TYPE_SO_STATISTICS_STREAM3";
        case(D3D12_QUERY_TYPE_VIDEO_DECODE_STATISTICS):
            return "D3D12_QUERY_TYPE_VIDEO_DECODE_STATISTICS";
        default:
            {
                std::string code = "Invalid D3D12_QUERY_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_PREDICATION_OP value)
{
    switch(value)
    {
        case(D3D12_PREDICATION_OP_EQUAL_ZERO):
            return "D3D12_PREDICATION_OP_EQUAL_ZERO";
        case(D3D12_PREDICATION_OP_NOT_EQUAL_ZERO):
            return "D3D12_PREDICATION_OP_NOT_EQUAL_ZERO";
        default:
            {
                std::string code = "Invalid D3D12_PREDICATION_OP(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_INDIRECT_ARGUMENT_TYPE value)
{
    switch(value)
    {
        case(D3D12_INDIRECT_ARGUMENT_TYPE_DRAW):
            return "D3D12_INDIRECT_ARGUMENT_TYPE_DRAW";
        case(D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED):
            return "D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED";
        case(D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH):
            return "D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH";
        case(D3D12_INDIRECT_ARGUMENT_TYPE_VERTEX_BUFFER_VIEW):
            return "D3D12_INDIRECT_ARGUMENT_TYPE_VERTEX_BUFFER_VIEW";
        case(D3D12_INDIRECT_ARGUMENT_TYPE_INDEX_BUFFER_VIEW):
            return "D3D12_INDIRECT_ARGUMENT_TYPE_INDEX_BUFFER_VIEW";
        case(D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT):
            return "D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT";
        case(D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW):
            return "D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW";
        case(D3D12_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW):
            return "D3D12_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW";
        case(D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW):
            return "D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW";
        case(D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH_RAYS):
            return "D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH_RAYS";
        case(D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH_MESH):
            return "D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH_MESH";
        default:
            {
                std::string code = "Invalid D3D12_INDIRECT_ARGUMENT_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_WRITEBUFFERIMMEDIATE_MODE value)
{
    switch(value)
    {
        case(D3D12_WRITEBUFFERIMMEDIATE_MODE_DEFAULT):
            return "D3D12_WRITEBUFFERIMMEDIATE_MODE_DEFAULT";
        case(D3D12_WRITEBUFFERIMMEDIATE_MODE_MARKER_IN):
            return "D3D12_WRITEBUFFERIMMEDIATE_MODE_MARKER_IN";
        case(D3D12_WRITEBUFFERIMMEDIATE_MODE_MARKER_OUT):
            return "D3D12_WRITEBUFFERIMMEDIATE_MODE_MARKER_OUT";
        default:
            {
                std::string code = "Invalid D3D12_WRITEBUFFERIMMEDIATE_MODE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_MULTIPLE_FENCE_WAIT_FLAGS value)
{
    switch(value)
    {
        case(D3D12_MULTIPLE_FENCE_WAIT_FLAG_NONE):
            return "D3D12_MULTIPLE_FENCE_WAIT_FLAG_NONE";
        case(D3D12_MULTIPLE_FENCE_WAIT_FLAG_ANY):
            return "D3D12_MULTIPLE_FENCE_WAIT_FLAG_ANY";
        default:
            {
                std::string code = "Invalid D3D12_MULTIPLE_FENCE_WAIT_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RESIDENCY_PRIORITY value)
{
    switch(value)
    {
        case(D3D12_RESIDENCY_PRIORITY_MINIMUM):
            return "D3D12_RESIDENCY_PRIORITY_MINIMUM";
        case(D3D12_RESIDENCY_PRIORITY_LOW):
            return "D3D12_RESIDENCY_PRIORITY_LOW";
        case(D3D12_RESIDENCY_PRIORITY_NORMAL):
            return "D3D12_RESIDENCY_PRIORITY_NORMAL";
        case(D3D12_RESIDENCY_PRIORITY_HIGH):
            return "D3D12_RESIDENCY_PRIORITY_HIGH";
        case(D3D12_RESIDENCY_PRIORITY_MAXIMUM):
            return "D3D12_RESIDENCY_PRIORITY_MAXIMUM";
        default:
            {
                std::string code = "Invalid D3D12_RESIDENCY_PRIORITY(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RESIDENCY_FLAGS value)
{
    switch(value)
    {
        case(D3D12_RESIDENCY_FLAG_NONE):
            return "D3D12_RESIDENCY_FLAG_NONE";
        case(D3D12_RESIDENCY_FLAG_DENY_OVERBUDGET):
            return "D3D12_RESIDENCY_FLAG_DENY_OVERBUDGET";
        default:
            {
                std::string code = "Invalid D3D12_RESIDENCY_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_COMMAND_LIST_FLAGS value)
{
    switch(value)
    {
        case(D3D12_COMMAND_LIST_FLAG_NONE):
            return "D3D12_COMMAND_LIST_FLAG_NONE";
        default:
            {
                std::string code = "Invalid D3D12_COMMAND_LIST_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_COMMAND_POOL_FLAGS value)
{
    switch(value)
    {
        case(D3D12_COMMAND_POOL_FLAG_NONE):
            return "D3D12_COMMAND_POOL_FLAG_NONE";
        default:
            {
                std::string code = "Invalid D3D12_COMMAND_POOL_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_COMMAND_RECORDER_FLAGS value)
{
    switch(value)
    {
        case(D3D12_COMMAND_RECORDER_FLAG_NONE):
            return "D3D12_COMMAND_RECORDER_FLAG_NONE";
        default:
            {
                std::string code = "Invalid D3D12_COMMAND_RECORDER_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_PROTECTED_SESSION_STATUS value)
{
    switch(value)
    {
        case(D3D12_PROTECTED_SESSION_STATUS_OK):
            return "D3D12_PROTECTED_SESSION_STATUS_OK";
        case(D3D12_PROTECTED_SESSION_STATUS_INVALID):
            return "D3D12_PROTECTED_SESSION_STATUS_INVALID";
        default:
            {
                std::string code = "Invalid D3D12_PROTECTED_SESSION_STATUS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_PROTECTED_RESOURCE_SESSION_SUPPORT_FLAGS value)
{
    switch(value)
    {
        case(D3D12_PROTECTED_RESOURCE_SESSION_SUPPORT_FLAG_NONE):
            return "D3D12_PROTECTED_RESOURCE_SESSION_SUPPORT_FLAG_NONE";
        case(D3D12_PROTECTED_RESOURCE_SESSION_SUPPORT_FLAG_SUPPORTED):
            return "D3D12_PROTECTED_RESOURCE_SESSION_SUPPORT_FLAG_SUPPORTED";
        default:
            {
                std::string code = "Invalid D3D12_PROTECTED_RESOURCE_SESSION_SUPPORT_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_PROTECTED_RESOURCE_SESSION_FLAGS value)
{
    switch(value)
    {
        case(D3D12_PROTECTED_RESOURCE_SESSION_FLAG_NONE):
            return "D3D12_PROTECTED_RESOURCE_SESSION_FLAG_NONE";
        default:
            {
                std::string code = "Invalid D3D12_PROTECTED_RESOURCE_SESSION_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_LIFETIME_STATE value)
{
    switch(value)
    {
        case(D3D12_LIFETIME_STATE_IN_USE):
            return "D3D12_LIFETIME_STATE_IN_USE";
        case(D3D12_LIFETIME_STATE_NOT_IN_USE):
            return "D3D12_LIFETIME_STATE_NOT_IN_USE";
        default:
            {
                std::string code = "Invalid D3D12_LIFETIME_STATE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_META_COMMAND_PARAMETER_TYPE value)
{
    switch(value)
    {
        case(D3D12_META_COMMAND_PARAMETER_TYPE_FLOAT):
            return "D3D12_META_COMMAND_PARAMETER_TYPE_FLOAT";
        case(D3D12_META_COMMAND_PARAMETER_TYPE_UINT64):
            return "D3D12_META_COMMAND_PARAMETER_TYPE_UINT64";
        case(D3D12_META_COMMAND_PARAMETER_TYPE_GPU_VIRTUAL_ADDRESS):
            return "D3D12_META_COMMAND_PARAMETER_TYPE_GPU_VIRTUAL_ADDRESS";
        case(D3D12_META_COMMAND_PARAMETER_TYPE_CPU_DESCRIPTOR_HANDLE_HEAP_TYPE_CBV_SRV_UAV):
            return "D3D12_META_COMMAND_PARAMETER_TYPE_CPU_DESCRIPTOR_HANDLE_HEAP_TYPE_CBV_SRV_UAV";
        case(D3D12_META_COMMAND_PARAMETER_TYPE_GPU_DESCRIPTOR_HANDLE_HEAP_TYPE_CBV_SRV_UAV):
            return "D3D12_META_COMMAND_PARAMETER_TYPE_GPU_DESCRIPTOR_HANDLE_HEAP_TYPE_CBV_SRV_UAV";
        default:
            {
                std::string code = "Invalid D3D12_META_COMMAND_PARAMETER_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_META_COMMAND_PARAMETER_FLAGS value)
{
    switch(value)
    {
        case(D3D12_META_COMMAND_PARAMETER_FLAG_INPUT):
            return "D3D12_META_COMMAND_PARAMETER_FLAG_INPUT";
        case(D3D12_META_COMMAND_PARAMETER_FLAG_OUTPUT):
            return "D3D12_META_COMMAND_PARAMETER_FLAG_OUTPUT";
        default:
            {
                std::string code = "Invalid D3D12_META_COMMAND_PARAMETER_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_META_COMMAND_PARAMETER_STAGE value)
{
    switch(value)
    {
        case(D3D12_META_COMMAND_PARAMETER_STAGE_CREATION):
            return "D3D12_META_COMMAND_PARAMETER_STAGE_CREATION";
        case(D3D12_META_COMMAND_PARAMETER_STAGE_INITIALIZATION):
            return "D3D12_META_COMMAND_PARAMETER_STAGE_INITIALIZATION";
        case(D3D12_META_COMMAND_PARAMETER_STAGE_EXECUTION):
            return "D3D12_META_COMMAND_PARAMETER_STAGE_EXECUTION";
        default:
            {
                std::string code = "Invalid D3D12_META_COMMAND_PARAMETER_STAGE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_GRAPHICS_STATES value)
{
    switch(value)
    {
        case(D3D12_GRAPHICS_STATE_NONE):
            return "D3D12_GRAPHICS_STATE_NONE";
        default:
            {
                std::string code = "Invalid D3D12_GRAPHICS_STATES(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_STATE_SUBOBJECT_TYPE value)
{
    switch(value)
    {
        case(D3D12_STATE_SUBOBJECT_TYPE_STATE_OBJECT_CONFIG):
            return "D3D12_STATE_SUBOBJECT_TYPE_STATE_OBJECT_CONFIG";
        case(D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE):
            return "D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE";
        case(D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE):
            return "D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE";
        case(D3D12_STATE_SUBOBJECT_TYPE_NODE_MASK):
            return "D3D12_STATE_SUBOBJECT_TYPE_NODE_MASK";
        case(D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY):
            return "D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY";
        case(D3D12_STATE_SUBOBJECT_TYPE_EXISTING_COLLECTION):
            return "D3D12_STATE_SUBOBJECT_TYPE_EXISTING_COLLECTION";
        case(D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION):
            return "D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION";
        case(D3D12_STATE_SUBOBJECT_TYPE_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION):
            return "D3D12_STATE_SUBOBJECT_TYPE_DXIL_SUBOBJECT_TO_EXPORTS_ASSOCIATION";
        case(D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG):
            return "D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG";
        case(D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG):
            return "D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG";
        case(D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP):
            return "D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP";
        case(D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG1):
            return "D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG1";
        case(D3D12_STATE_SUBOBJECT_TYPE_MAX_VALID):
            return "D3D12_STATE_SUBOBJECT_TYPE_MAX_VALID";
        default:
            {
                std::string code = "Invalid D3D12_STATE_SUBOBJECT_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_STATE_OBJECT_FLAGS value)
{
    switch(value)
    {
        case(D3D12_STATE_OBJECT_FLAG_NONE):
            return "D3D12_STATE_OBJECT_FLAG_NONE";
        case(D3D12_STATE_OBJECT_FLAG_ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITIONS):
            return "D3D12_STATE_OBJECT_FLAG_ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITIONS";
        case(D3D12_STATE_OBJECT_FLAG_ALLOW_EXTERNAL_DEPENDENCIES_ON_LOCAL_DEFINITIONS):
            return "D3D12_STATE_OBJECT_FLAG_ALLOW_EXTERNAL_DEPENDENCIES_ON_LOCAL_DEFINITIONS";
        case(D3D12_STATE_OBJECT_FLAG_ALLOW_STATE_OBJECT_ADDITIONS):
            return "D3D12_STATE_OBJECT_FLAG_ALLOW_STATE_OBJECT_ADDITIONS";
        default:
            {
                std::string code = "Invalid D3D12_STATE_OBJECT_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_EXPORT_FLAGS value)
{
    switch(value)
    {
        case(D3D12_EXPORT_FLAG_NONE):
            return "D3D12_EXPORT_FLAG_NONE";
        default:
            {
                std::string code = "Invalid D3D12_EXPORT_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_HIT_GROUP_TYPE value)
{
    switch(value)
    {
        case(D3D12_HIT_GROUP_TYPE_TRIANGLES):
            return "D3D12_HIT_GROUP_TYPE_TRIANGLES";
        case(D3D12_HIT_GROUP_TYPE_PROCEDURAL_PRIMITIVE):
            return "D3D12_HIT_GROUP_TYPE_PROCEDURAL_PRIMITIVE";
        default:
            {
                std::string code = "Invalid D3D12_HIT_GROUP_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RAYTRACING_PIPELINE_FLAGS value)
{
    switch(value)
    {
        case(D3D12_RAYTRACING_PIPELINE_FLAG_NONE):
            return "D3D12_RAYTRACING_PIPELINE_FLAG_NONE";
        case(D3D12_RAYTRACING_PIPELINE_FLAG_SKIP_TRIANGLES):
            return "D3D12_RAYTRACING_PIPELINE_FLAG_SKIP_TRIANGLES";
        case(D3D12_RAYTRACING_PIPELINE_FLAG_SKIP_PROCEDURAL_PRIMITIVES):
            return "D3D12_RAYTRACING_PIPELINE_FLAG_SKIP_PROCEDURAL_PRIMITIVES";
        default:
            {
                std::string code = "Invalid D3D12_RAYTRACING_PIPELINE_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_STATE_OBJECT_TYPE value)
{
    switch(value)
    {
        case(D3D12_STATE_OBJECT_TYPE_COLLECTION):
            return "D3D12_STATE_OBJECT_TYPE_COLLECTION";
        case(D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE):
            return "D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE";
        default:
            {
                std::string code = "Invalid D3D12_STATE_OBJECT_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RAYTRACING_GEOMETRY_FLAGS value)
{
    switch(value)
    {
        case(D3D12_RAYTRACING_GEOMETRY_FLAG_NONE):
            return "D3D12_RAYTRACING_GEOMETRY_FLAG_NONE";
        case(D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE):
            return "D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE";
        case(D3D12_RAYTRACING_GEOMETRY_FLAG_NO_DUPLICATE_ANYHIT_INVOCATION):
            return "D3D12_RAYTRACING_GEOMETRY_FLAG_NO_DUPLICATE_ANYHIT_INVOCATION";
        default:
            {
                std::string code = "Invalid D3D12_RAYTRACING_GEOMETRY_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RAYTRACING_GEOMETRY_TYPE value)
{
    switch(value)
    {
        case(D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES):
            return "D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES";
        case(D3D12_RAYTRACING_GEOMETRY_TYPE_PROCEDURAL_PRIMITIVE_AABBS):
            return "D3D12_RAYTRACING_GEOMETRY_TYPE_PROCEDURAL_PRIMITIVE_AABBS";
        default:
            {
                std::string code = "Invalid D3D12_RAYTRACING_GEOMETRY_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RAYTRACING_INSTANCE_FLAGS value)
{
    switch(value)
    {
        case(D3D12_RAYTRACING_INSTANCE_FLAG_NONE):
            return "D3D12_RAYTRACING_INSTANCE_FLAG_NONE";
        case(D3D12_RAYTRACING_INSTANCE_FLAG_TRIANGLE_CULL_DISABLE):
            return "D3D12_RAYTRACING_INSTANCE_FLAG_TRIANGLE_CULL_DISABLE";
        case(D3D12_RAYTRACING_INSTANCE_FLAG_TRIANGLE_FRONT_COUNTERCLOCKWISE):
            return "D3D12_RAYTRACING_INSTANCE_FLAG_TRIANGLE_FRONT_COUNTERCLOCKWISE";
        case(D3D12_RAYTRACING_INSTANCE_FLAG_FORCE_OPAQUE):
            return "D3D12_RAYTRACING_INSTANCE_FLAG_FORCE_OPAQUE";
        case(D3D12_RAYTRACING_INSTANCE_FLAG_FORCE_NON_OPAQUE):
            return "D3D12_RAYTRACING_INSTANCE_FLAG_FORCE_NON_OPAQUE";
        default:
            {
                std::string code = "Invalid D3D12_RAYTRACING_INSTANCE_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS value)
{
    switch(value)
    {
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE";
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE";
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_COMPACTION):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_COMPACTION";
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE";
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_BUILD):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_BUILD";
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_MINIMIZE_MEMORY):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_MINIMIZE_MEMORY";
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE";
        default:
            {
                std::string code = "Invalid D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE value)
{
    switch(value)
    {
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE_CLONE):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE_CLONE";
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE_COMPACT):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE_COMPACT";
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE_VISUALIZATION_DECODE_FOR_TOOLS):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE_VISUALIZATION_DECODE_FOR_TOOLS";
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE_SERIALIZE):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE_SERIALIZE";
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE_DESERIALIZE):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE_DESERIALIZE";
        default:
            {
                std::string code = "Invalid D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE value)
{
    switch(value)
    {
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL";
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL";
        default:
            {
                std::string code = "Invalid D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_ELEMENTS_LAYOUT value)
{
    switch(value)
    {
        case(D3D12_ELEMENTS_LAYOUT_ARRAY):
            return "D3D12_ELEMENTS_LAYOUT_ARRAY";
        case(D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS):
            return "D3D12_ELEMENTS_LAYOUT_ARRAY_OF_POINTERS";
        default:
            {
                std::string code = "Invalid D3D12_ELEMENTS_LAYOUT(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_TYPE value)
{
    switch(value)
    {
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_COMPACTED_SIZE):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_COMPACTED_SIZE";
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_TOOLS_VISUALIZATION):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_TOOLS_VISUALIZATION";
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_SERIALIZATION):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_SERIALIZATION";
        case(D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_CURRENT_SIZE):
            return "D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_CURRENT_SIZE";
        default:
            {
                std::string code = "Invalid D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_SERIALIZED_DATA_TYPE value)
{
    switch(value)
    {
        case(D3D12_SERIALIZED_DATA_RAYTRACING_ACCELERATION_STRUCTURE):
            return "D3D12_SERIALIZED_DATA_RAYTRACING_ACCELERATION_STRUCTURE";
        default:
            {
                std::string code = "Invalid D3D12_SERIALIZED_DATA_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DRIVER_MATCHING_IDENTIFIER_STATUS value)
{
    switch(value)
    {
        case(D3D12_DRIVER_MATCHING_IDENTIFIER_COMPATIBLE_WITH_DEVICE):
            return "D3D12_DRIVER_MATCHING_IDENTIFIER_COMPATIBLE_WITH_DEVICE";
        case(D3D12_DRIVER_MATCHING_IDENTIFIER_UNSUPPORTED_TYPE):
            return "D3D12_DRIVER_MATCHING_IDENTIFIER_UNSUPPORTED_TYPE";
        case(D3D12_DRIVER_MATCHING_IDENTIFIER_UNRECOGNIZED):
            return "D3D12_DRIVER_MATCHING_IDENTIFIER_UNRECOGNIZED";
        case(D3D12_DRIVER_MATCHING_IDENTIFIER_INCOMPATIBLE_VERSION):
            return "D3D12_DRIVER_MATCHING_IDENTIFIER_INCOMPATIBLE_VERSION";
        case(D3D12_DRIVER_MATCHING_IDENTIFIER_INCOMPATIBLE_TYPE):
            return "D3D12_DRIVER_MATCHING_IDENTIFIER_INCOMPATIBLE_TYPE";
        default:
            {
                std::string code = "Invalid D3D12_DRIVER_MATCHING_IDENTIFIER_STATUS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RAY_FLAGS value)
{
    switch(value)
    {
        case(D3D12_RAY_FLAG_NONE):
            return "D3D12_RAY_FLAG_NONE";
        case(D3D12_RAY_FLAG_FORCE_OPAQUE):
            return "D3D12_RAY_FLAG_FORCE_OPAQUE";
        case(D3D12_RAY_FLAG_FORCE_NON_OPAQUE):
            return "D3D12_RAY_FLAG_FORCE_NON_OPAQUE";
        case(D3D12_RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH):
            return "D3D12_RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH";
        case(D3D12_RAY_FLAG_SKIP_CLOSEST_HIT_SHADER):
            return "D3D12_RAY_FLAG_SKIP_CLOSEST_HIT_SHADER";
        case(D3D12_RAY_FLAG_CULL_BACK_FACING_TRIANGLES):
            return "D3D12_RAY_FLAG_CULL_BACK_FACING_TRIANGLES";
        case(D3D12_RAY_FLAG_CULL_FRONT_FACING_TRIANGLES):
            return "D3D12_RAY_FLAG_CULL_FRONT_FACING_TRIANGLES";
        case(D3D12_RAY_FLAG_CULL_OPAQUE):
            return "D3D12_RAY_FLAG_CULL_OPAQUE";
        case(D3D12_RAY_FLAG_CULL_NON_OPAQUE):
            return "D3D12_RAY_FLAG_CULL_NON_OPAQUE";
        case(D3D12_RAY_FLAG_SKIP_TRIANGLES):
            return "D3D12_RAY_FLAG_SKIP_TRIANGLES";
        case(D3D12_RAY_FLAG_SKIP_PROCEDURAL_PRIMITIVES):
            return "D3D12_RAY_FLAG_SKIP_PROCEDURAL_PRIMITIVES";
        default:
            {
                std::string code = "Invalid D3D12_RAY_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_HIT_KIND value)
{
    switch(value)
    {
        case(D3D12_HIT_KIND_TRIANGLE_FRONT_FACE):
            return "D3D12_HIT_KIND_TRIANGLE_FRONT_FACE";
        case(D3D12_HIT_KIND_TRIANGLE_BACK_FACE):
            return "D3D12_HIT_KIND_TRIANGLE_BACK_FACE";
        default:
            {
                std::string code = "Invalid D3D12_HIT_KIND(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_AUTO_BREADCRUMB_OP value)
{
    switch(value)
    {
        case(D3D12_AUTO_BREADCRUMB_OP_SETMARKER):
            return "D3D12_AUTO_BREADCRUMB_OP_SETMARKER";
        case(D3D12_AUTO_BREADCRUMB_OP_BEGINEVENT):
            return "D3D12_AUTO_BREADCRUMB_OP_BEGINEVENT";
        case(D3D12_AUTO_BREADCRUMB_OP_ENDEVENT):
            return "D3D12_AUTO_BREADCRUMB_OP_ENDEVENT";
        case(D3D12_AUTO_BREADCRUMB_OP_DRAWINSTANCED):
            return "D3D12_AUTO_BREADCRUMB_OP_DRAWINSTANCED";
        case(D3D12_AUTO_BREADCRUMB_OP_DRAWINDEXEDINSTANCED):
            return "D3D12_AUTO_BREADCRUMB_OP_DRAWINDEXEDINSTANCED";
        case(D3D12_AUTO_BREADCRUMB_OP_EXECUTEINDIRECT):
            return "D3D12_AUTO_BREADCRUMB_OP_EXECUTEINDIRECT";
        case(D3D12_AUTO_BREADCRUMB_OP_DISPATCH):
            return "D3D12_AUTO_BREADCRUMB_OP_DISPATCH";
        case(D3D12_AUTO_BREADCRUMB_OP_COPYBUFFERREGION):
            return "D3D12_AUTO_BREADCRUMB_OP_COPYBUFFERREGION";
        case(D3D12_AUTO_BREADCRUMB_OP_COPYTEXTUREREGION):
            return "D3D12_AUTO_BREADCRUMB_OP_COPYTEXTUREREGION";
        case(D3D12_AUTO_BREADCRUMB_OP_COPYRESOURCE):
            return "D3D12_AUTO_BREADCRUMB_OP_COPYRESOURCE";
        case(D3D12_AUTO_BREADCRUMB_OP_COPYTILES):
            return "D3D12_AUTO_BREADCRUMB_OP_COPYTILES";
        case(D3D12_AUTO_BREADCRUMB_OP_RESOLVESUBRESOURCE):
            return "D3D12_AUTO_BREADCRUMB_OP_RESOLVESUBRESOURCE";
        case(D3D12_AUTO_BREADCRUMB_OP_CLEARRENDERTARGETVIEW):
            return "D3D12_AUTO_BREADCRUMB_OP_CLEARRENDERTARGETVIEW";
        case(D3D12_AUTO_BREADCRUMB_OP_CLEARUNORDEREDACCESSVIEW):
            return "D3D12_AUTO_BREADCRUMB_OP_CLEARUNORDEREDACCESSVIEW";
        case(D3D12_AUTO_BREADCRUMB_OP_CLEARDEPTHSTENCILVIEW):
            return "D3D12_AUTO_BREADCRUMB_OP_CLEARDEPTHSTENCILVIEW";
        case(D3D12_AUTO_BREADCRUMB_OP_RESOURCEBARRIER):
            return "D3D12_AUTO_BREADCRUMB_OP_RESOURCEBARRIER";
        case(D3D12_AUTO_BREADCRUMB_OP_EXECUTEBUNDLE):
            return "D3D12_AUTO_BREADCRUMB_OP_EXECUTEBUNDLE";
        case(D3D12_AUTO_BREADCRUMB_OP_PRESENT):
            return "D3D12_AUTO_BREADCRUMB_OP_PRESENT";
        case(D3D12_AUTO_BREADCRUMB_OP_RESOLVEQUERYDATA):
            return "D3D12_AUTO_BREADCRUMB_OP_RESOLVEQUERYDATA";
        case(D3D12_AUTO_BREADCRUMB_OP_BEGINSUBMISSION):
            return "D3D12_AUTO_BREADCRUMB_OP_BEGINSUBMISSION";
        case(D3D12_AUTO_BREADCRUMB_OP_ENDSUBMISSION):
            return "D3D12_AUTO_BREADCRUMB_OP_ENDSUBMISSION";
        case(D3D12_AUTO_BREADCRUMB_OP_DECODEFRAME):
            return "D3D12_AUTO_BREADCRUMB_OP_DECODEFRAME";
        case(D3D12_AUTO_BREADCRUMB_OP_PROCESSFRAMES):
            return "D3D12_AUTO_BREADCRUMB_OP_PROCESSFRAMES";
        case(D3D12_AUTO_BREADCRUMB_OP_ATOMICCOPYBUFFERUINT):
            return "D3D12_AUTO_BREADCRUMB_OP_ATOMICCOPYBUFFERUINT";
        case(D3D12_AUTO_BREADCRUMB_OP_ATOMICCOPYBUFFERUINT64):
            return "D3D12_AUTO_BREADCRUMB_OP_ATOMICCOPYBUFFERUINT64";
        case(D3D12_AUTO_BREADCRUMB_OP_RESOLVESUBRESOURCEREGION):
            return "D3D12_AUTO_BREADCRUMB_OP_RESOLVESUBRESOURCEREGION";
        case(D3D12_AUTO_BREADCRUMB_OP_WRITEBUFFERIMMEDIATE):
            return "D3D12_AUTO_BREADCRUMB_OP_WRITEBUFFERIMMEDIATE";
        case(D3D12_AUTO_BREADCRUMB_OP_DECODEFRAME1):
            return "D3D12_AUTO_BREADCRUMB_OP_DECODEFRAME1";
        case(D3D12_AUTO_BREADCRUMB_OP_SETPROTECTEDRESOURCESESSION):
            return "D3D12_AUTO_BREADCRUMB_OP_SETPROTECTEDRESOURCESESSION";
        case(D3D12_AUTO_BREADCRUMB_OP_DECODEFRAME2):
            return "D3D12_AUTO_BREADCRUMB_OP_DECODEFRAME2";
        case(D3D12_AUTO_BREADCRUMB_OP_PROCESSFRAMES1):
            return "D3D12_AUTO_BREADCRUMB_OP_PROCESSFRAMES1";
        case(D3D12_AUTO_BREADCRUMB_OP_BUILDRAYTRACINGACCELERATIONSTRUCTURE):
            return "D3D12_AUTO_BREADCRUMB_OP_BUILDRAYTRACINGACCELERATIONSTRUCTURE";
        case(D3D12_AUTO_BREADCRUMB_OP_EMITRAYTRACINGACCELERATIONSTRUCTUREPOSTBUILDINFO):
            return "D3D12_AUTO_BREADCRUMB_OP_EMITRAYTRACINGACCELERATIONSTRUCTUREPOSTBUILDINFO";
        case(D3D12_AUTO_BREADCRUMB_OP_COPYRAYTRACINGACCELERATIONSTRUCTURE):
            return "D3D12_AUTO_BREADCRUMB_OP_COPYRAYTRACINGACCELERATIONSTRUCTURE";
        case(D3D12_AUTO_BREADCRUMB_OP_DISPATCHRAYS):
            return "D3D12_AUTO_BREADCRUMB_OP_DISPATCHRAYS";
        case(D3D12_AUTO_BREADCRUMB_OP_INITIALIZEMETACOMMAND):
            return "D3D12_AUTO_BREADCRUMB_OP_INITIALIZEMETACOMMAND";
        case(D3D12_AUTO_BREADCRUMB_OP_EXECUTEMETACOMMAND):
            return "D3D12_AUTO_BREADCRUMB_OP_EXECUTEMETACOMMAND";
        case(D3D12_AUTO_BREADCRUMB_OP_ESTIMATEMOTION):
            return "D3D12_AUTO_BREADCRUMB_OP_ESTIMATEMOTION";
        case(D3D12_AUTO_BREADCRUMB_OP_RESOLVEMOTIONVECTORHEAP):
            return "D3D12_AUTO_BREADCRUMB_OP_RESOLVEMOTIONVECTORHEAP";
        case(D3D12_AUTO_BREADCRUMB_OP_SETPIPELINESTATE1):
            return "D3D12_AUTO_BREADCRUMB_OP_SETPIPELINESTATE1";
        case(D3D12_AUTO_BREADCRUMB_OP_INITIALIZEEXTENSIONCOMMAND):
            return "D3D12_AUTO_BREADCRUMB_OP_INITIALIZEEXTENSIONCOMMAND";
        case(D3D12_AUTO_BREADCRUMB_OP_EXECUTEEXTENSIONCOMMAND):
            return "D3D12_AUTO_BREADCRUMB_OP_EXECUTEEXTENSIONCOMMAND";
        case(D3D12_AUTO_BREADCRUMB_OP_DISPATCHMESH):
            return "D3D12_AUTO_BREADCRUMB_OP_DISPATCHMESH";
        default:
            {
                std::string code = "Invalid D3D12_AUTO_BREADCRUMB_OP(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DRED_VERSION value)
{
    switch(value)
    {
        case(D3D12_DRED_VERSION_1_0):
            return "D3D12_DRED_VERSION_1_0";
        case(D3D12_DRED_VERSION_1_1):
            return "D3D12_DRED_VERSION_1_1";
        case(D3D12_DRED_VERSION_1_2):
            return "D3D12_DRED_VERSION_1_2";
        default:
            {
                std::string code = "Invalid D3D12_DRED_VERSION(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DRED_FLAGS value)
{
    switch(value)
    {
        case(D3D12_DRED_FLAG_NONE):
            return "D3D12_DRED_FLAG_NONE";
        case(D3D12_DRED_FLAG_FORCE_ENABLE):
            return "D3D12_DRED_FLAG_FORCE_ENABLE";
        case(D3D12_DRED_FLAG_DISABLE_AUTOBREADCRUMBS):
            return "D3D12_DRED_FLAG_DISABLE_AUTOBREADCRUMBS";
        default:
            {
                std::string code = "Invalid D3D12_DRED_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DRED_ENABLEMENT value)
{
    switch(value)
    {
        case(D3D12_DRED_ENABLEMENT_SYSTEM_CONTROLLED):
            return "D3D12_DRED_ENABLEMENT_SYSTEM_CONTROLLED";
        case(D3D12_DRED_ENABLEMENT_FORCED_OFF):
            return "D3D12_DRED_ENABLEMENT_FORCED_OFF";
        case(D3D12_DRED_ENABLEMENT_FORCED_ON):
            return "D3D12_DRED_ENABLEMENT_FORCED_ON";
        default:
            {
                std::string code = "Invalid D3D12_DRED_ENABLEMENT(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DRED_ALLOCATION_TYPE value)
{
    switch(value)
    {
        case(D3D12_DRED_ALLOCATION_TYPE_COMMAND_QUEUE):
            return "D3D12_DRED_ALLOCATION_TYPE_COMMAND_QUEUE";
        case(D3D12_DRED_ALLOCATION_TYPE_COMMAND_ALLOCATOR):
            return "D3D12_DRED_ALLOCATION_TYPE_COMMAND_ALLOCATOR";
        case(D3D12_DRED_ALLOCATION_TYPE_PIPELINE_STATE):
            return "D3D12_DRED_ALLOCATION_TYPE_PIPELINE_STATE";
        case(D3D12_DRED_ALLOCATION_TYPE_COMMAND_LIST):
            return "D3D12_DRED_ALLOCATION_TYPE_COMMAND_LIST";
        case(D3D12_DRED_ALLOCATION_TYPE_FENCE):
            return "D3D12_DRED_ALLOCATION_TYPE_FENCE";
        case(D3D12_DRED_ALLOCATION_TYPE_DESCRIPTOR_HEAP):
            return "D3D12_DRED_ALLOCATION_TYPE_DESCRIPTOR_HEAP";
        case(D3D12_DRED_ALLOCATION_TYPE_HEAP):
            return "D3D12_DRED_ALLOCATION_TYPE_HEAP";
        case(D3D12_DRED_ALLOCATION_TYPE_QUERY_HEAP):
            return "D3D12_DRED_ALLOCATION_TYPE_QUERY_HEAP";
        case(D3D12_DRED_ALLOCATION_TYPE_COMMAND_SIGNATURE):
            return "D3D12_DRED_ALLOCATION_TYPE_COMMAND_SIGNATURE";
        case(D3D12_DRED_ALLOCATION_TYPE_PIPELINE_LIBRARY):
            return "D3D12_DRED_ALLOCATION_TYPE_PIPELINE_LIBRARY";
        case(D3D12_DRED_ALLOCATION_TYPE_VIDEO_DECODER):
            return "D3D12_DRED_ALLOCATION_TYPE_VIDEO_DECODER";
        case(D3D12_DRED_ALLOCATION_TYPE_VIDEO_PROCESSOR):
            return "D3D12_DRED_ALLOCATION_TYPE_VIDEO_PROCESSOR";
        case(D3D12_DRED_ALLOCATION_TYPE_RESOURCE):
            return "D3D12_DRED_ALLOCATION_TYPE_RESOURCE";
        case(D3D12_DRED_ALLOCATION_TYPE_PASS):
            return "D3D12_DRED_ALLOCATION_TYPE_PASS";
        case(D3D12_DRED_ALLOCATION_TYPE_CRYPTOSESSION):
            return "D3D12_DRED_ALLOCATION_TYPE_CRYPTOSESSION";
        case(D3D12_DRED_ALLOCATION_TYPE_CRYPTOSESSIONPOLICY):
            return "D3D12_DRED_ALLOCATION_TYPE_CRYPTOSESSIONPOLICY";
        case(D3D12_DRED_ALLOCATION_TYPE_PROTECTEDRESOURCESESSION):
            return "D3D12_DRED_ALLOCATION_TYPE_PROTECTEDRESOURCESESSION";
        case(D3D12_DRED_ALLOCATION_TYPE_VIDEO_DECODER_HEAP):
            return "D3D12_DRED_ALLOCATION_TYPE_VIDEO_DECODER_HEAP";
        case(D3D12_DRED_ALLOCATION_TYPE_COMMAND_POOL):
            return "D3D12_DRED_ALLOCATION_TYPE_COMMAND_POOL";
        case(D3D12_DRED_ALLOCATION_TYPE_COMMAND_RECORDER):
            return "D3D12_DRED_ALLOCATION_TYPE_COMMAND_RECORDER";
        case(D3D12_DRED_ALLOCATION_TYPE_STATE_OBJECT):
            return "D3D12_DRED_ALLOCATION_TYPE_STATE_OBJECT";
        case(D3D12_DRED_ALLOCATION_TYPE_METACOMMAND):
            return "D3D12_DRED_ALLOCATION_TYPE_METACOMMAND";
        case(D3D12_DRED_ALLOCATION_TYPE_SCHEDULINGGROUP):
            return "D3D12_DRED_ALLOCATION_TYPE_SCHEDULINGGROUP";
        case(D3D12_DRED_ALLOCATION_TYPE_VIDEO_MOTION_ESTIMATOR):
            return "D3D12_DRED_ALLOCATION_TYPE_VIDEO_MOTION_ESTIMATOR";
        case(D3D12_DRED_ALLOCATION_TYPE_VIDEO_MOTION_VECTOR_HEAP):
            return "D3D12_DRED_ALLOCATION_TYPE_VIDEO_MOTION_VECTOR_HEAP";
        case(D3D12_DRED_ALLOCATION_TYPE_VIDEO_EXTENSION_COMMAND):
            return "D3D12_DRED_ALLOCATION_TYPE_VIDEO_EXTENSION_COMMAND";
        case(D3D12_DRED_ALLOCATION_TYPE_INVALID):
            return "D3D12_DRED_ALLOCATION_TYPE_INVALID";
        default:
            {
                std::string code = "Invalid D3D12_DRED_ALLOCATION_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_BACKGROUND_PROCESSING_MODE value)
{
    switch(value)
    {
        case(D3D12_BACKGROUND_PROCESSING_MODE_ALLOWED):
            return "D3D12_BACKGROUND_PROCESSING_MODE_ALLOWED";
        case(D3D12_BACKGROUND_PROCESSING_MODE_ALLOW_INTRUSIVE_MEASUREMENTS):
            return "D3D12_BACKGROUND_PROCESSING_MODE_ALLOW_INTRUSIVE_MEASUREMENTS";
        case(D3D12_BACKGROUND_PROCESSING_MODE_DISABLE_BACKGROUND_WORK):
            return "D3D12_BACKGROUND_PROCESSING_MODE_DISABLE_BACKGROUND_WORK";
        case(D3D12_BACKGROUND_PROCESSING_MODE_DISABLE_PROFILING_BY_SYSTEM):
            return "D3D12_BACKGROUND_PROCESSING_MODE_DISABLE_PROFILING_BY_SYSTEM";
        default:
            {
                std::string code = "Invalid D3D12_BACKGROUND_PROCESSING_MODE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_MEASUREMENTS_ACTION value)
{
    switch(value)
    {
        case(D3D12_MEASUREMENTS_ACTION_KEEP_ALL):
            return "D3D12_MEASUREMENTS_ACTION_KEEP_ALL";
        case(D3D12_MEASUREMENTS_ACTION_COMMIT_RESULTS):
            return "D3D12_MEASUREMENTS_ACTION_COMMIT_RESULTS";
        case(D3D12_MEASUREMENTS_ACTION_COMMIT_RESULTS_HIGH_PRIORITY):
            return "D3D12_MEASUREMENTS_ACTION_COMMIT_RESULTS_HIGH_PRIORITY";
        case(D3D12_MEASUREMENTS_ACTION_DISCARD_PREVIOUS):
            return "D3D12_MEASUREMENTS_ACTION_DISCARD_PREVIOUS";
        default:
            {
                std::string code = "Invalid D3D12_MEASUREMENTS_ACTION(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE value)
{
    switch(value)
    {
        case(D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD):
            return "D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD";
        case(D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE):
            return "D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE";
        case(D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR):
            return "D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR";
        case(D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_NO_ACCESS):
            return "D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_NO_ACCESS";
        default:
            {
                std::string code = "Invalid D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RENDER_PASS_ENDING_ACCESS_TYPE value)
{
    switch(value)
    {
        case(D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD):
            return "D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD";
        case(D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE):
            return "D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE";
        case(D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_RESOLVE):
            return "D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_RESOLVE";
        case(D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_NO_ACCESS):
            return "D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_NO_ACCESS";
        default:
            {
                std::string code = "Invalid D3D12_RENDER_PASS_ENDING_ACCESS_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RENDER_PASS_FLAGS value)
{
    switch(value)
    {
        case(D3D12_RENDER_PASS_FLAG_NONE):
            return "D3D12_RENDER_PASS_FLAG_NONE";
        case(D3D12_RENDER_PASS_FLAG_ALLOW_UAV_WRITES):
            return "D3D12_RENDER_PASS_FLAG_ALLOW_UAV_WRITES";
        case(D3D12_RENDER_PASS_FLAG_SUSPENDING_PASS):
            return "D3D12_RENDER_PASS_FLAG_SUSPENDING_PASS";
        case(D3D12_RENDER_PASS_FLAG_RESUMING_PASS):
            return "D3D12_RENDER_PASS_FLAG_RESUMING_PASS";
        default:
            {
                std::string code = "Invalid D3D12_RENDER_PASS_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_AXIS_SHADING_RATE value)
{
    switch(value)
    {
        case(D3D12_AXIS_SHADING_RATE_1X):
            return "D3D12_AXIS_SHADING_RATE_1X";
        case(D3D12_AXIS_SHADING_RATE_2X):
            return "D3D12_AXIS_SHADING_RATE_2X";
        case(D3D12_AXIS_SHADING_RATE_4X):
            return "D3D12_AXIS_SHADING_RATE_4X";
        default:
            {
                std::string code = "Invalid D3D12_AXIS_SHADING_RATE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_SHADING_RATE value)
{
    switch(value)
    {
        case(D3D12_SHADING_RATE_1X1):
            return "D3D12_SHADING_RATE_1X1";
        case(D3D12_SHADING_RATE_1X2):
            return "D3D12_SHADING_RATE_1X2";
        case(D3D12_SHADING_RATE_2X1):
            return "D3D12_SHADING_RATE_2X1";
        case(D3D12_SHADING_RATE_2X2):
            return "D3D12_SHADING_RATE_2X2";
        case(D3D12_SHADING_RATE_2X4):
            return "D3D12_SHADING_RATE_2X4";
        case(D3D12_SHADING_RATE_4X2):
            return "D3D12_SHADING_RATE_4X2";
        case(D3D12_SHADING_RATE_4X4):
            return "D3D12_SHADING_RATE_4X4";
        default:
            {
                std::string code = "Invalid D3D12_SHADING_RATE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_SHADING_RATE_COMBINER value)
{
    switch(value)
    {
        case(D3D12_SHADING_RATE_COMBINER_PASSTHROUGH):
            return "D3D12_SHADING_RATE_COMBINER_PASSTHROUGH";
        case(D3D12_SHADING_RATE_COMBINER_OVERRIDE):
            return "D3D12_SHADING_RATE_COMBINER_OVERRIDE";
        case(D3D12_SHADING_RATE_COMBINER_MIN):
            return "D3D12_SHADING_RATE_COMBINER_MIN";
        case(D3D12_SHADING_RATE_COMBINER_MAX):
            return "D3D12_SHADING_RATE_COMBINER_MAX";
        case(D3D12_SHADING_RATE_COMBINER_SUM):
            return "D3D12_SHADING_RATE_COMBINER_SUM";
        default:
            {
                std::string code = "Invalid D3D12_SHADING_RATE_COMBINER(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_DRIVER_TYPE value)
{
    switch(value)
    {
        case(D3D_DRIVER_TYPE_UNKNOWN):
            return "D3D_DRIVER_TYPE_UNKNOWN";
        case(D3D_DRIVER_TYPE_HARDWARE):
            return "D3D_DRIVER_TYPE_HARDWARE";
        case(D3D_DRIVER_TYPE_REFERENCE):
            return "D3D_DRIVER_TYPE_REFERENCE";
        case(D3D_DRIVER_TYPE_NULL):
            return "D3D_DRIVER_TYPE_NULL";
        case(D3D_DRIVER_TYPE_SOFTWARE):
            return "D3D_DRIVER_TYPE_SOFTWARE";
        case(D3D_DRIVER_TYPE_WARP):
            return "D3D_DRIVER_TYPE_WARP";
        default:
            {
                std::string code = "Invalid D3D_DRIVER_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_FEATURE_LEVEL value)
{
    switch(value)
    {
        case(D3D_FEATURE_LEVEL_1_0_CORE):
            return "D3D_FEATURE_LEVEL_1_0_CORE";
        case(D3D_FEATURE_LEVEL_9_1):
            return "D3D_FEATURE_LEVEL_9_1";
        case(D3D_FEATURE_LEVEL_9_2):
            return "D3D_FEATURE_LEVEL_9_2";
        case(D3D_FEATURE_LEVEL_9_3):
            return "D3D_FEATURE_LEVEL_9_3";
        case(D3D_FEATURE_LEVEL_10_0):
            return "D3D_FEATURE_LEVEL_10_0";
        case(D3D_FEATURE_LEVEL_10_1):
            return "D3D_FEATURE_LEVEL_10_1";
        case(D3D_FEATURE_LEVEL_11_0):
            return "D3D_FEATURE_LEVEL_11_0";
        case(D3D_FEATURE_LEVEL_11_1):
            return "D3D_FEATURE_LEVEL_11_1";
        case(D3D_FEATURE_LEVEL_12_0):
            return "D3D_FEATURE_LEVEL_12_0";
        case(D3D_FEATURE_LEVEL_12_1):
            return "D3D_FEATURE_LEVEL_12_1";
        default:
            {
                std::string code = "Invalid D3D_FEATURE_LEVEL(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_PRIMITIVE_TOPOLOGY value)
{
    switch(value)
    {
        case(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED):
            return "D3D_PRIMITIVE_TOPOLOGY_UNDEFINED";
        case(D3D_PRIMITIVE_TOPOLOGY_POINTLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_POINTLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_LINELIST):
            return "D3D_PRIMITIVE_TOPOLOGY_LINELIST";
        case(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP):
            return "D3D_PRIMITIVE_TOPOLOGY_LINESTRIP";
        case(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST):
            return "D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST";
        case(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP):
            return "D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP";
        case(D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ):
            return "D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ";
        case(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ):
            return "D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ";
        case(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ):
            return "D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ";
        case(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ):
            return "D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ";
        case(D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST";
        case(D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST):
            return "D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST";
        default:
            {
                std::string code = "Invalid D3D_PRIMITIVE_TOPOLOGY(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_PRIMITIVE value)
{
    switch(value)
    {
        case(D3D_PRIMITIVE_UNDEFINED):
            return "D3D_PRIMITIVE_UNDEFINED";
        case(D3D_PRIMITIVE_POINT):
            return "D3D_PRIMITIVE_POINT";
        case(D3D_PRIMITIVE_LINE):
            return "D3D_PRIMITIVE_LINE";
        case(D3D_PRIMITIVE_TRIANGLE):
            return "D3D_PRIMITIVE_TRIANGLE";
        case(D3D_PRIMITIVE_LINE_ADJ):
            return "D3D_PRIMITIVE_LINE_ADJ";
        case(D3D_PRIMITIVE_TRIANGLE_ADJ):
            return "D3D_PRIMITIVE_TRIANGLE_ADJ";
        case(D3D_PRIMITIVE_1_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_1_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_2_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_2_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_3_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_3_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_4_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_4_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_5_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_5_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_6_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_6_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_7_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_7_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_8_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_8_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_9_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_9_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_10_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_10_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_11_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_11_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_12_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_12_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_13_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_13_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_14_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_14_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_15_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_15_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_16_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_16_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_17_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_17_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_18_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_18_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_19_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_19_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_20_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_20_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_21_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_21_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_22_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_22_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_23_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_23_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_24_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_24_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_25_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_25_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_26_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_26_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_27_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_27_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_28_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_28_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_29_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_29_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_30_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_30_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_31_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_31_CONTROL_POINT_PATCH";
        case(D3D_PRIMITIVE_32_CONTROL_POINT_PATCH):
            return "D3D_PRIMITIVE_32_CONTROL_POINT_PATCH";
        default:
            {
                std::string code = "Invalid D3D_PRIMITIVE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_SRV_DIMENSION value)
{
    switch(value)
    {
        case(D3D_SRV_DIMENSION_UNKNOWN):
            return "D3D_SRV_DIMENSION_UNKNOWN";
        case(D3D_SRV_DIMENSION_BUFFER):
            return "D3D_SRV_DIMENSION_BUFFER";
        case(D3D_SRV_DIMENSION_TEXTURE1D):
            return "D3D_SRV_DIMENSION_TEXTURE1D";
        case(D3D_SRV_DIMENSION_TEXTURE1DARRAY):
            return "D3D_SRV_DIMENSION_TEXTURE1DARRAY";
        case(D3D_SRV_DIMENSION_TEXTURE2D):
            return "D3D_SRV_DIMENSION_TEXTURE2D";
        case(D3D_SRV_DIMENSION_TEXTURE2DARRAY):
            return "D3D_SRV_DIMENSION_TEXTURE2DARRAY";
        case(D3D_SRV_DIMENSION_TEXTURE2DMS):
            return "D3D_SRV_DIMENSION_TEXTURE2DMS";
        case(D3D_SRV_DIMENSION_TEXTURE2DMSARRAY):
            return "D3D_SRV_DIMENSION_TEXTURE2DMSARRAY";
        case(D3D_SRV_DIMENSION_TEXTURE3D):
            return "D3D_SRV_DIMENSION_TEXTURE3D";
        case(D3D_SRV_DIMENSION_TEXTURECUBE):
            return "D3D_SRV_DIMENSION_TEXTURECUBE";
        case(D3D_SRV_DIMENSION_TEXTURECUBEARRAY):
            return "D3D_SRV_DIMENSION_TEXTURECUBEARRAY";
        case(D3D_SRV_DIMENSION_BUFFEREX):
            return "D3D_SRV_DIMENSION_BUFFEREX";
        default:
            {
                std::string code = "Invalid D3D_SRV_DIMENSION(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_INCLUDE_TYPE value)
{
    switch(value)
    {
        case(D3D_INCLUDE_LOCAL):
            return "D3D_INCLUDE_LOCAL";
        case(D3D_INCLUDE_SYSTEM):
            return "D3D_INCLUDE_SYSTEM";
        case(D3D_INCLUDE_FORCE_DWORD):
            return "D3D_INCLUDE_FORCE_DWORD";
        default:
            {
                std::string code = "Invalid D3D_INCLUDE_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_SHADER_VARIABLE_CLASS value)
{
    switch(value)
    {
        case(D3D_SVC_SCALAR):
            return "D3D_SVC_SCALAR";
        case(D3D_SVC_VECTOR):
            return "D3D_SVC_VECTOR";
        case(D3D_SVC_MATRIX_ROWS):
            return "D3D_SVC_MATRIX_ROWS";
        case(D3D_SVC_MATRIX_COLUMNS):
            return "D3D_SVC_MATRIX_COLUMNS";
        case(D3D_SVC_OBJECT):
            return "D3D_SVC_OBJECT";
        case(D3D_SVC_STRUCT):
            return "D3D_SVC_STRUCT";
        case(D3D_SVC_INTERFACE_CLASS):
            return "D3D_SVC_INTERFACE_CLASS";
        case(D3D_SVC_INTERFACE_POINTER):
            return "D3D_SVC_INTERFACE_POINTER";
        case(D3D_SVC_FORCE_DWORD):
            return "D3D_SVC_FORCE_DWORD";
        default:
            {
                std::string code = "Invalid D3D_SHADER_VARIABLE_CLASS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_SHADER_VARIABLE_FLAGS value)
{
    switch(value)
    {
        case(D3D_SVF_USERPACKED):
            return "D3D_SVF_USERPACKED";
        case(D3D_SVF_USED):
            return "D3D_SVF_USED";
        case(D3D_SVF_INTERFACE_POINTER):
            return "D3D_SVF_INTERFACE_POINTER";
        case(D3D_SVF_INTERFACE_PARAMETER):
            return "D3D_SVF_INTERFACE_PARAMETER";
        case(D3D_SVF_FORCE_DWORD):
            return "D3D_SVF_FORCE_DWORD";
        default:
            {
                std::string code = "Invalid D3D_SHADER_VARIABLE_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_SHADER_VARIABLE_TYPE value)
{
    switch(value)
    {
        case(D3D_SVT_VOID):
            return "D3D_SVT_VOID";
        case(D3D_SVT_BOOL):
            return "D3D_SVT_BOOL";
        case(D3D_SVT_INT):
            return "D3D_SVT_INT";
        case(D3D_SVT_FLOAT):
            return "D3D_SVT_FLOAT";
        case(D3D_SVT_STRING):
            return "D3D_SVT_STRING";
        case(D3D_SVT_TEXTURE):
            return "D3D_SVT_TEXTURE";
        case(D3D_SVT_TEXTURE1D):
            return "D3D_SVT_TEXTURE1D";
        case(D3D_SVT_TEXTURE2D):
            return "D3D_SVT_TEXTURE2D";
        case(D3D_SVT_TEXTURE3D):
            return "D3D_SVT_TEXTURE3D";
        case(D3D_SVT_TEXTURECUBE):
            return "D3D_SVT_TEXTURECUBE";
        case(D3D_SVT_SAMPLER):
            return "D3D_SVT_SAMPLER";
        case(D3D_SVT_SAMPLER1D):
            return "D3D_SVT_SAMPLER1D";
        case(D3D_SVT_SAMPLER2D):
            return "D3D_SVT_SAMPLER2D";
        case(D3D_SVT_SAMPLER3D):
            return "D3D_SVT_SAMPLER3D";
        case(D3D_SVT_SAMPLERCUBE):
            return "D3D_SVT_SAMPLERCUBE";
        case(D3D_SVT_PIXELSHADER):
            return "D3D_SVT_PIXELSHADER";
        case(D3D_SVT_VERTEXSHADER):
            return "D3D_SVT_VERTEXSHADER";
        case(D3D_SVT_PIXELFRAGMENT):
            return "D3D_SVT_PIXELFRAGMENT";
        case(D3D_SVT_VERTEXFRAGMENT):
            return "D3D_SVT_VERTEXFRAGMENT";
        case(D3D_SVT_UINT):
            return "D3D_SVT_UINT";
        case(D3D_SVT_UINT8):
            return "D3D_SVT_UINT8";
        case(D3D_SVT_GEOMETRYSHADER):
            return "D3D_SVT_GEOMETRYSHADER";
        case(D3D_SVT_RASTERIZER):
            return "D3D_SVT_RASTERIZER";
        case(D3D_SVT_DEPTHSTENCIL):
            return "D3D_SVT_DEPTHSTENCIL";
        case(D3D_SVT_BLEND):
            return "D3D_SVT_BLEND";
        case(D3D_SVT_BUFFER):
            return "D3D_SVT_BUFFER";
        case(D3D_SVT_CBUFFER):
            return "D3D_SVT_CBUFFER";
        case(D3D_SVT_TBUFFER):
            return "D3D_SVT_TBUFFER";
        case(D3D_SVT_TEXTURE1DARRAY):
            return "D3D_SVT_TEXTURE1DARRAY";
        case(D3D_SVT_TEXTURE2DARRAY):
            return "D3D_SVT_TEXTURE2DARRAY";
        case(D3D_SVT_RENDERTARGETVIEW):
            return "D3D_SVT_RENDERTARGETVIEW";
        case(D3D_SVT_DEPTHSTENCILVIEW):
            return "D3D_SVT_DEPTHSTENCILVIEW";
        case(D3D_SVT_TEXTURE2DMS):
            return "D3D_SVT_TEXTURE2DMS";
        case(D3D_SVT_TEXTURE2DMSARRAY):
            return "D3D_SVT_TEXTURE2DMSARRAY";
        case(D3D_SVT_TEXTURECUBEARRAY):
            return "D3D_SVT_TEXTURECUBEARRAY";
        case(D3D_SVT_HULLSHADER):
            return "D3D_SVT_HULLSHADER";
        case(D3D_SVT_DOMAINSHADER):
            return "D3D_SVT_DOMAINSHADER";
        case(D3D_SVT_INTERFACE_POINTER):
            return "D3D_SVT_INTERFACE_POINTER";
        case(D3D_SVT_COMPUTESHADER):
            return "D3D_SVT_COMPUTESHADER";
        case(D3D_SVT_DOUBLE):
            return "D3D_SVT_DOUBLE";
        case(D3D_SVT_RWTEXTURE1D):
            return "D3D_SVT_RWTEXTURE1D";
        case(D3D_SVT_RWTEXTURE1DARRAY):
            return "D3D_SVT_RWTEXTURE1DARRAY";
        case(D3D_SVT_RWTEXTURE2D):
            return "D3D_SVT_RWTEXTURE2D";
        case(D3D_SVT_RWTEXTURE2DARRAY):
            return "D3D_SVT_RWTEXTURE2DARRAY";
        case(D3D_SVT_RWTEXTURE3D):
            return "D3D_SVT_RWTEXTURE3D";
        case(D3D_SVT_RWBUFFER):
            return "D3D_SVT_RWBUFFER";
        case(D3D_SVT_BYTEADDRESS_BUFFER):
            return "D3D_SVT_BYTEADDRESS_BUFFER";
        case(D3D_SVT_RWBYTEADDRESS_BUFFER):
            return "D3D_SVT_RWBYTEADDRESS_BUFFER";
        case(D3D_SVT_STRUCTURED_BUFFER):
            return "D3D_SVT_STRUCTURED_BUFFER";
        case(D3D_SVT_RWSTRUCTURED_BUFFER):
            return "D3D_SVT_RWSTRUCTURED_BUFFER";
        case(D3D_SVT_APPEND_STRUCTURED_BUFFER):
            return "D3D_SVT_APPEND_STRUCTURED_BUFFER";
        case(D3D_SVT_CONSUME_STRUCTURED_BUFFER):
            return "D3D_SVT_CONSUME_STRUCTURED_BUFFER";
        case(D3D_SVT_MIN8FLOAT):
            return "D3D_SVT_MIN8FLOAT";
        case(D3D_SVT_MIN10FLOAT):
            return "D3D_SVT_MIN10FLOAT";
        case(D3D_SVT_MIN16FLOAT):
            return "D3D_SVT_MIN16FLOAT";
        case(D3D_SVT_MIN12INT):
            return "D3D_SVT_MIN12INT";
        case(D3D_SVT_MIN16INT):
            return "D3D_SVT_MIN16INT";
        case(D3D_SVT_MIN16UINT):
            return "D3D_SVT_MIN16UINT";
        case(D3D_SVT_FORCE_DWORD):
            return "D3D_SVT_FORCE_DWORD";
        default:
            {
                std::string code = "Invalid D3D_SHADER_VARIABLE_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_SHADER_INPUT_FLAGS value)
{
    switch(value)
    {
        case(D3D_SIF_USERPACKED):
            return "D3D_SIF_USERPACKED";
        case(D3D_SIF_COMPARISON_SAMPLER):
            return "D3D_SIF_COMPARISON_SAMPLER";
        case(D3D_SIF_TEXTURE_COMPONENT_0):
            return "D3D_SIF_TEXTURE_COMPONENT_0";
        case(D3D_SIF_TEXTURE_COMPONENT_1):
            return "D3D_SIF_TEXTURE_COMPONENT_1";
        case(D3D_SIF_TEXTURE_COMPONENTS):
            return "D3D_SIF_TEXTURE_COMPONENTS";
        case(D3D_SIF_UNUSED):
            return "D3D_SIF_UNUSED";
        case(D3D_SIF_FORCE_DWORD):
            return "D3D_SIF_FORCE_DWORD";
        default:
            {
                std::string code = "Invalid D3D_SHADER_INPUT_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_SHADER_INPUT_TYPE value)
{
    switch(value)
    {
        case(D3D_SIT_CBUFFER):
            return "D3D_SIT_CBUFFER";
        case(D3D_SIT_TBUFFER):
            return "D3D_SIT_TBUFFER";
        case(D3D_SIT_TEXTURE):
            return "D3D_SIT_TEXTURE";
        case(D3D_SIT_SAMPLER):
            return "D3D_SIT_SAMPLER";
        case(D3D_SIT_UAV_RWTYPED):
            return "D3D_SIT_UAV_RWTYPED";
        case(D3D_SIT_STRUCTURED):
            return "D3D_SIT_STRUCTURED";
        case(D3D_SIT_UAV_RWSTRUCTURED):
            return "D3D_SIT_UAV_RWSTRUCTURED";
        case(D3D_SIT_BYTEADDRESS):
            return "D3D_SIT_BYTEADDRESS";
        case(D3D_SIT_UAV_RWBYTEADDRESS):
            return "D3D_SIT_UAV_RWBYTEADDRESS";
        case(D3D_SIT_UAV_APPEND_STRUCTURED):
            return "D3D_SIT_UAV_APPEND_STRUCTURED";
        case(D3D_SIT_UAV_CONSUME_STRUCTURED):
            return "D3D_SIT_UAV_CONSUME_STRUCTURED";
        case(D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER):
            return "D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER";
        case(D3D_SIT_RTACCELERATIONSTRUCTURE):
            return "D3D_SIT_RTACCELERATIONSTRUCTURE";
        case(D3D_SIT_UAV_FEEDBACKTEXTURE):
            return "D3D_SIT_UAV_FEEDBACKTEXTURE";
        default:
            {
                std::string code = "Invalid D3D_SHADER_INPUT_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_SHADER_CBUFFER_FLAGS value)
{
    switch(value)
    {
        case(D3D_CBF_USERPACKED):
            return "D3D_CBF_USERPACKED";
        case(D3D_CBF_FORCE_DWORD):
            return "D3D_CBF_FORCE_DWORD";
        default:
            {
                std::string code = "Invalid D3D_SHADER_CBUFFER_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_CBUFFER_TYPE value)
{
    switch(value)
    {
        case(D3D_CT_CBUFFER):
            return "D3D_CT_CBUFFER";
        case(D3D_CT_TBUFFER):
            return "D3D_CT_TBUFFER";
        case(D3D_CT_INTERFACE_POINTERS):
            return "D3D_CT_INTERFACE_POINTERS";
        case(D3D_CT_RESOURCE_BIND_INFO):
            return "D3D_CT_RESOURCE_BIND_INFO";
        default:
            {
                std::string code = "Invalid D3D_CBUFFER_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_NAME value)
{
    switch(value)
    {
        case(D3D_NAME_UNDEFINED):
            return "D3D_NAME_UNDEFINED";
        case(D3D_NAME_POSITION):
            return "D3D_NAME_POSITION";
        case(D3D_NAME_CLIP_DISTANCE):
            return "D3D_NAME_CLIP_DISTANCE";
        case(D3D_NAME_CULL_DISTANCE):
            return "D3D_NAME_CULL_DISTANCE";
        case(D3D_NAME_RENDER_TARGET_ARRAY_INDEX):
            return "D3D_NAME_RENDER_TARGET_ARRAY_INDEX";
        case(D3D_NAME_VIEWPORT_ARRAY_INDEX):
            return "D3D_NAME_VIEWPORT_ARRAY_INDEX";
        case(D3D_NAME_VERTEX_ID):
            return "D3D_NAME_VERTEX_ID";
        case(D3D_NAME_PRIMITIVE_ID):
            return "D3D_NAME_PRIMITIVE_ID";
        case(D3D_NAME_INSTANCE_ID):
            return "D3D_NAME_INSTANCE_ID";
        case(D3D_NAME_IS_FRONT_FACE):
            return "D3D_NAME_IS_FRONT_FACE";
        case(D3D_NAME_SAMPLE_INDEX):
            return "D3D_NAME_SAMPLE_INDEX";
        case(D3D_NAME_FINAL_QUAD_EDGE_TESSFACTOR):
            return "D3D_NAME_FINAL_QUAD_EDGE_TESSFACTOR";
        case(D3D_NAME_FINAL_QUAD_INSIDE_TESSFACTOR):
            return "D3D_NAME_FINAL_QUAD_INSIDE_TESSFACTOR";
        case(D3D_NAME_FINAL_TRI_EDGE_TESSFACTOR):
            return "D3D_NAME_FINAL_TRI_EDGE_TESSFACTOR";
        case(D3D_NAME_FINAL_TRI_INSIDE_TESSFACTOR):
            return "D3D_NAME_FINAL_TRI_INSIDE_TESSFACTOR";
        case(D3D_NAME_FINAL_LINE_DETAIL_TESSFACTOR):
            return "D3D_NAME_FINAL_LINE_DETAIL_TESSFACTOR";
        case(D3D_NAME_FINAL_LINE_DENSITY_TESSFACTOR):
            return "D3D_NAME_FINAL_LINE_DENSITY_TESSFACTOR";
        case(D3D_NAME_BARYCENTRICS):
            return "D3D_NAME_BARYCENTRICS";
        case(D3D_NAME_SHADINGRATE):
            return "D3D_NAME_SHADINGRATE";
        case(D3D_NAME_CULLPRIMITIVE):
            return "D3D_NAME_CULLPRIMITIVE";
        case(D3D_NAME_TARGET):
            return "D3D_NAME_TARGET";
        case(D3D_NAME_DEPTH):
            return "D3D_NAME_DEPTH";
        case(D3D_NAME_COVERAGE):
            return "D3D_NAME_COVERAGE";
        case(D3D_NAME_DEPTH_GREATER_EQUAL):
            return "D3D_NAME_DEPTH_GREATER_EQUAL";
        case(D3D_NAME_DEPTH_LESS_EQUAL):
            return "D3D_NAME_DEPTH_LESS_EQUAL";
        case(D3D_NAME_STENCIL_REF):
            return "D3D_NAME_STENCIL_REF";
        case(D3D_NAME_INNER_COVERAGE):
            return "D3D_NAME_INNER_COVERAGE";
        default:
            {
                std::string code = "Invalid D3D_NAME(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_RESOURCE_RETURN_TYPE value)
{
    switch(value)
    {
        case(D3D_RETURN_TYPE_UNORM):
            return "D3D_RETURN_TYPE_UNORM";
        case(D3D_RETURN_TYPE_SNORM):
            return "D3D_RETURN_TYPE_SNORM";
        case(D3D_RETURN_TYPE_SINT):
            return "D3D_RETURN_TYPE_SINT";
        case(D3D_RETURN_TYPE_UINT):
            return "D3D_RETURN_TYPE_UINT";
        case(D3D_RETURN_TYPE_FLOAT):
            return "D3D_RETURN_TYPE_FLOAT";
        case(D3D_RETURN_TYPE_MIXED):
            return "D3D_RETURN_TYPE_MIXED";
        case(D3D_RETURN_TYPE_DOUBLE):
            return "D3D_RETURN_TYPE_DOUBLE";
        case(D3D_RETURN_TYPE_CONTINUED):
            return "D3D_RETURN_TYPE_CONTINUED";
        default:
            {
                std::string code = "Invalid D3D_RESOURCE_RETURN_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_REGISTER_COMPONENT_TYPE value)
{
    switch(value)
    {
        case(D3D_REGISTER_COMPONENT_UNKNOWN):
            return "D3D_REGISTER_COMPONENT_UNKNOWN";
        case(D3D_REGISTER_COMPONENT_UINT32):
            return "D3D_REGISTER_COMPONENT_UINT32";
        case(D3D_REGISTER_COMPONENT_SINT32):
            return "D3D_REGISTER_COMPONENT_SINT32";
        case(D3D_REGISTER_COMPONENT_FLOAT32):
            return "D3D_REGISTER_COMPONENT_FLOAT32";
        default:
            {
                std::string code = "Invalid D3D_REGISTER_COMPONENT_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_TESSELLATOR_DOMAIN value)
{
    switch(value)
    {
        case(D3D_TESSELLATOR_DOMAIN_UNDEFINED):
            return "D3D_TESSELLATOR_DOMAIN_UNDEFINED";
        case(D3D_TESSELLATOR_DOMAIN_ISOLINE):
            return "D3D_TESSELLATOR_DOMAIN_ISOLINE";
        case(D3D_TESSELLATOR_DOMAIN_TRI):
            return "D3D_TESSELLATOR_DOMAIN_TRI";
        case(D3D_TESSELLATOR_DOMAIN_QUAD):
            return "D3D_TESSELLATOR_DOMAIN_QUAD";
        default:
            {
                std::string code = "Invalid D3D_TESSELLATOR_DOMAIN(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_TESSELLATOR_PARTITIONING value)
{
    switch(value)
    {
        case(D3D_TESSELLATOR_PARTITIONING_UNDEFINED):
            return "D3D_TESSELLATOR_PARTITIONING_UNDEFINED";
        case(D3D_TESSELLATOR_PARTITIONING_INTEGER):
            return "D3D_TESSELLATOR_PARTITIONING_INTEGER";
        case(D3D_TESSELLATOR_PARTITIONING_POW2):
            return "D3D_TESSELLATOR_PARTITIONING_POW2";
        case(D3D_TESSELLATOR_PARTITIONING_FRACTIONAL_ODD):
            return "D3D_TESSELLATOR_PARTITIONING_FRACTIONAL_ODD";
        case(D3D_TESSELLATOR_PARTITIONING_FRACTIONAL_EVEN):
            return "D3D_TESSELLATOR_PARTITIONING_FRACTIONAL_EVEN";
        default:
            {
                std::string code = "Invalid D3D_TESSELLATOR_PARTITIONING(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_TESSELLATOR_OUTPUT_PRIMITIVE value)
{
    switch(value)
    {
        case(D3D_TESSELLATOR_OUTPUT_UNDEFINED):
            return "D3D_TESSELLATOR_OUTPUT_UNDEFINED";
        case(D3D_TESSELLATOR_OUTPUT_POINT):
            return "D3D_TESSELLATOR_OUTPUT_POINT";
        case(D3D_TESSELLATOR_OUTPUT_LINE):
            return "D3D_TESSELLATOR_OUTPUT_LINE";
        case(D3D_TESSELLATOR_OUTPUT_TRIANGLE_CW):
            return "D3D_TESSELLATOR_OUTPUT_TRIANGLE_CW";
        case(D3D_TESSELLATOR_OUTPUT_TRIANGLE_CCW):
            return "D3D_TESSELLATOR_OUTPUT_TRIANGLE_CCW";
        default:
            {
                std::string code = "Invalid D3D_TESSELLATOR_OUTPUT_PRIMITIVE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_MIN_PRECISION value)
{
    switch(value)
    {
        case(D3D_MIN_PRECISION_DEFAULT):
            return "D3D_MIN_PRECISION_DEFAULT";
        case(D3D_MIN_PRECISION_FLOAT_16):
            return "D3D_MIN_PRECISION_FLOAT_16";
        case(D3D_MIN_PRECISION_FLOAT_2_8):
            return "D3D_MIN_PRECISION_FLOAT_2_8";
        case(D3D_MIN_PRECISION_RESERVED):
            return "D3D_MIN_PRECISION_RESERVED";
        case(D3D_MIN_PRECISION_SINT_16):
            return "D3D_MIN_PRECISION_SINT_16";
        case(D3D_MIN_PRECISION_UINT_16):
            return "D3D_MIN_PRECISION_UINT_16";
        case(D3D_MIN_PRECISION_ANY_16):
            return "D3D_MIN_PRECISION_ANY_16";
        case(D3D_MIN_PRECISION_ANY_10):
            return "D3D_MIN_PRECISION_ANY_10";
        default:
            {
                std::string code = "Invalid D3D_MIN_PRECISION(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_INTERPOLATION_MODE value)
{
    switch(value)
    {
        case(D3D_INTERPOLATION_UNDEFINED):
            return "D3D_INTERPOLATION_UNDEFINED";
        case(D3D_INTERPOLATION_CONSTANT):
            return "D3D_INTERPOLATION_CONSTANT";
        case(D3D_INTERPOLATION_LINEAR):
            return "D3D_INTERPOLATION_LINEAR";
        case(D3D_INTERPOLATION_LINEAR_CENTROID):
            return "D3D_INTERPOLATION_LINEAR_CENTROID";
        case(D3D_INTERPOLATION_LINEAR_NOPERSPECTIVE):
            return "D3D_INTERPOLATION_LINEAR_NOPERSPECTIVE";
        case(D3D_INTERPOLATION_LINEAR_NOPERSPECTIVE_CENTROID):
            return "D3D_INTERPOLATION_LINEAR_NOPERSPECTIVE_CENTROID";
        case(D3D_INTERPOLATION_LINEAR_SAMPLE):
            return "D3D_INTERPOLATION_LINEAR_SAMPLE";
        case(D3D_INTERPOLATION_LINEAR_NOPERSPECTIVE_SAMPLE):
            return "D3D_INTERPOLATION_LINEAR_NOPERSPECTIVE_SAMPLE";
        default:
            {
                std::string code = "Invalid D3D_INTERPOLATION_MODE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D_PARAMETER_FLAGS value)
{
    switch(value)
    {
        case(D3D_PF_NONE):
            return "D3D_PF_NONE";
        case(D3D_PF_IN):
            return "D3D_PF_IN";
        case(D3D_PF_OUT):
            return "D3D_PF_OUT";
        case(D3D_PF_FORCE_DWORD):
            return "D3D_PF_FORCE_DWORD";
        default:
            {
                std::string code = "Invalid D3D_PARAMETER_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_GPU_BASED_VALIDATION_FLAGS value)
{
    switch(value)
    {
        case(D3D12_GPU_BASED_VALIDATION_FLAGS_NONE):
            return "D3D12_GPU_BASED_VALIDATION_FLAGS_NONE";
        case(D3D12_GPU_BASED_VALIDATION_FLAGS_DISABLE_STATE_TRACKING):
            return "D3D12_GPU_BASED_VALIDATION_FLAGS_DISABLE_STATE_TRACKING";
        default:
            {
                std::string code = "Invalid D3D12_GPU_BASED_VALIDATION_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_RLDO_FLAGS value)
{
    switch(value)
    {
        case(D3D12_RLDO_NONE):
            return "D3D12_RLDO_NONE";
        case(D3D12_RLDO_SUMMARY):
            return "D3D12_RLDO_SUMMARY";
        case(D3D12_RLDO_DETAIL):
            return "D3D12_RLDO_DETAIL";
        case(D3D12_RLDO_IGNORE_INTERNAL):
            return "D3D12_RLDO_IGNORE_INTERNAL";
        default:
            {
                std::string code = "Invalid D3D12_RLDO_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DEBUG_DEVICE_PARAMETER_TYPE value)
{
    switch(value)
    {
        case(D3D12_DEBUG_DEVICE_PARAMETER_FEATURE_FLAGS):
            return "D3D12_DEBUG_DEVICE_PARAMETER_FEATURE_FLAGS";
        case(D3D12_DEBUG_DEVICE_PARAMETER_GPU_BASED_VALIDATION_SETTINGS):
            return "D3D12_DEBUG_DEVICE_PARAMETER_GPU_BASED_VALIDATION_SETTINGS";
        case(D3D12_DEBUG_DEVICE_PARAMETER_GPU_SLOWDOWN_PERFORMANCE_FACTOR):
            return "D3D12_DEBUG_DEVICE_PARAMETER_GPU_SLOWDOWN_PERFORMANCE_FACTOR";
        default:
            {
                std::string code = "Invalid D3D12_DEBUG_DEVICE_PARAMETER_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DEBUG_FEATURE value)
{
    switch(value)
    {
        case(D3D12_DEBUG_FEATURE_NONE):
            return "D3D12_DEBUG_FEATURE_NONE";
        case(D3D12_DEBUG_FEATURE_ALLOW_BEHAVIOR_CHANGING_DEBUG_AIDS):
            return "D3D12_DEBUG_FEATURE_ALLOW_BEHAVIOR_CHANGING_DEBUG_AIDS";
        case(D3D12_DEBUG_FEATURE_CONSERVATIVE_RESOURCE_STATE_TRACKING):
            return "D3D12_DEBUG_FEATURE_CONSERVATIVE_RESOURCE_STATE_TRACKING";
        case(D3D12_DEBUG_FEATURE_DISABLE_VIRTUALIZED_BUNDLES_VALIDATION):
            return "D3D12_DEBUG_FEATURE_DISABLE_VIRTUALIZED_BUNDLES_VALIDATION";
        case(D3D12_DEBUG_FEATURE_EMULATE_WINDOWS7):
            return "D3D12_DEBUG_FEATURE_EMULATE_WINDOWS7";
        default:
            {
                std::string code = "Invalid D3D12_DEBUG_FEATURE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_GPU_BASED_VALIDATION_SHADER_PATCH_MODE value)
{
    switch(value)
    {
        case(D3D12_GPU_BASED_VALIDATION_SHADER_PATCH_MODE_NONE):
            return "D3D12_GPU_BASED_VALIDATION_SHADER_PATCH_MODE_NONE";
        case(D3D12_GPU_BASED_VALIDATION_SHADER_PATCH_MODE_STATE_TRACKING_ONLY):
            return "D3D12_GPU_BASED_VALIDATION_SHADER_PATCH_MODE_STATE_TRACKING_ONLY";
        case(D3D12_GPU_BASED_VALIDATION_SHADER_PATCH_MODE_UNGUARDED_VALIDATION):
            return "D3D12_GPU_BASED_VALIDATION_SHADER_PATCH_MODE_UNGUARDED_VALIDATION";
        case(D3D12_GPU_BASED_VALIDATION_SHADER_PATCH_MODE_GUARDED_VALIDATION):
            return "D3D12_GPU_BASED_VALIDATION_SHADER_PATCH_MODE_GUARDED_VALIDATION";
        case(NUM_D3D12_GPU_BASED_VALIDATION_SHADER_PATCH_MODES):
            return "NUM_D3D12_GPU_BASED_VALIDATION_SHADER_PATCH_MODES";
        default:
            {
                std::string code = "Invalid D3D12_GPU_BASED_VALIDATION_SHADER_PATCH_MODE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_GPU_BASED_VALIDATION_PIPELINE_STATE_CREATE_FLAGS value)
{
    switch(value)
    {
        case(D3D12_GPU_BASED_VALIDATION_PIPELINE_STATE_CREATE_FLAG_NONE):
            return "D3D12_GPU_BASED_VALIDATION_PIPELINE_STATE_CREATE_FLAG_NONE";
        case(D3D12_GPU_BASED_VALIDATION_PIPELINE_STATE_CREATE_FLAG_FRONT_LOAD_CREATE_TRACKING_ONLY_SHADERS):
            return "D3D12_GPU_BASED_VALIDATION_PIPELINE_STATE_CREATE_FLAG_FRONT_LOAD_CREATE_TRACKING_ONLY_SHADERS";
        case(D3D12_GPU_BASED_VALIDATION_PIPELINE_STATE_CREATE_FLAG_FRONT_LOAD_CREATE_UNGUARDED_VALIDATION_SHADERS):
            return "D3D12_GPU_BASED_VALIDATION_PIPELINE_STATE_CREATE_FLAG_FRONT_LOAD_CREATE_UNGUARDED_VALIDATION_SHADERS";
        case(D3D12_GPU_BASED_VALIDATION_PIPELINE_STATE_CREATE_FLAG_FRONT_LOAD_CREATE_GUARDED_VALIDATION_SHADERS):
            return "D3D12_GPU_BASED_VALIDATION_PIPELINE_STATE_CREATE_FLAG_FRONT_LOAD_CREATE_GUARDED_VALIDATION_SHADERS";
        case(D3D12_GPU_BASED_VALIDATION_PIPELINE_STATE_CREATE_FLAGS_VALID_MASK):
            return "D3D12_GPU_BASED_VALIDATION_PIPELINE_STATE_CREATE_FLAGS_VALID_MASK";
        default:
            {
                std::string code = "Invalid D3D12_GPU_BASED_VALIDATION_PIPELINE_STATE_CREATE_FLAGS(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_DEBUG_COMMAND_LIST_PARAMETER_TYPE value)
{
    switch(value)
    {
        case(D3D12_DEBUG_COMMAND_LIST_PARAMETER_GPU_BASED_VALIDATION_SETTINGS):
            return "D3D12_DEBUG_COMMAND_LIST_PARAMETER_GPU_BASED_VALIDATION_SETTINGS";
        default:
            {
                std::string code = "Invalid D3D12_DEBUG_COMMAND_LIST_PARAMETER_TYPE(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_MESSAGE_CATEGORY value)
{
    switch(value)
    {
        case(D3D12_MESSAGE_CATEGORY_APPLICATION_DEFINED):
            return "D3D12_MESSAGE_CATEGORY_APPLICATION_DEFINED";
        case(D3D12_MESSAGE_CATEGORY_MISCELLANEOUS):
            return "D3D12_MESSAGE_CATEGORY_MISCELLANEOUS";
        case(D3D12_MESSAGE_CATEGORY_INITIALIZATION):
            return "D3D12_MESSAGE_CATEGORY_INITIALIZATION";
        case(D3D12_MESSAGE_CATEGORY_CLEANUP):
            return "D3D12_MESSAGE_CATEGORY_CLEANUP";
        case(D3D12_MESSAGE_CATEGORY_COMPILATION):
            return "D3D12_MESSAGE_CATEGORY_COMPILATION";
        case(D3D12_MESSAGE_CATEGORY_STATE_CREATION):
            return "D3D12_MESSAGE_CATEGORY_STATE_CREATION";
        case(D3D12_MESSAGE_CATEGORY_STATE_SETTING):
            return "D3D12_MESSAGE_CATEGORY_STATE_SETTING";
        case(D3D12_MESSAGE_CATEGORY_STATE_GETTING):
            return "D3D12_MESSAGE_CATEGORY_STATE_GETTING";
        case(D3D12_MESSAGE_CATEGORY_RESOURCE_MANIPULATION):
            return "D3D12_MESSAGE_CATEGORY_RESOURCE_MANIPULATION";
        case(D3D12_MESSAGE_CATEGORY_EXECUTION):
            return "D3D12_MESSAGE_CATEGORY_EXECUTION";
        case(D3D12_MESSAGE_CATEGORY_SHADER):
            return "D3D12_MESSAGE_CATEGORY_SHADER";
        default:
            {
                std::string code = "Invalid D3D12_MESSAGE_CATEGORY(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_MESSAGE_SEVERITY value)
{
    switch(value)
    {
        case(D3D12_MESSAGE_SEVERITY_CORRUPTION):
            return "D3D12_MESSAGE_SEVERITY_CORRUPTION";
        case(D3D12_MESSAGE_SEVERITY_ERROR):
            return "D3D12_MESSAGE_SEVERITY_ERROR";
        case(D3D12_MESSAGE_SEVERITY_WARNING):
            return "D3D12_MESSAGE_SEVERITY_WARNING";
        case(D3D12_MESSAGE_SEVERITY_INFO):
            return "D3D12_MESSAGE_SEVERITY_INFO";
        case(D3D12_MESSAGE_SEVERITY_MESSAGE):
            return "D3D12_MESSAGE_SEVERITY_MESSAGE";
        default:
            {
                std::string code = "Invalid D3D12_MESSAGE_SEVERITY(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static const std::string ConverttoText(const D3D12_MESSAGE_ID value)
{
    switch(value)
    {
        case(D3D12_MESSAGE_ID_UNKNOWN):
            return "D3D12_MESSAGE_ID_UNKNOWN";
        case(D3D12_MESSAGE_ID_STRING_FROM_APPLICATION):
            return "D3D12_MESSAGE_ID_STRING_FROM_APPLICATION";
        case(D3D12_MESSAGE_ID_CORRUPTED_THIS):
            return "D3D12_MESSAGE_ID_CORRUPTED_THIS";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER1):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER1";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER2):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER2";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER3):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER3";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER4):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER4";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER5):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER5";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER6):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER6";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER7):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER7";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER8):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER8";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER9):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER9";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER10):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER10";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER11):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER11";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER12):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER12";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER13):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER13";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER14):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER14";
        case(D3D12_MESSAGE_ID_CORRUPTED_PARAMETER15):
            return "D3D12_MESSAGE_ID_CORRUPTED_PARAMETER15";
        case(D3D12_MESSAGE_ID_CORRUPTED_MULTITHREADING):
            return "D3D12_MESSAGE_ID_CORRUPTED_MULTITHREADING";
        case(D3D12_MESSAGE_ID_MESSAGE_REPORTING_OUTOFMEMORY):
            return "D3D12_MESSAGE_ID_MESSAGE_REPORTING_OUTOFMEMORY";
        case(D3D12_MESSAGE_ID_GETPRIVATEDATA_MOREDATA):
            return "D3D12_MESSAGE_ID_GETPRIVATEDATA_MOREDATA";
        case(D3D12_MESSAGE_ID_SETPRIVATEDATA_INVALIDFREEDATA):
            return "D3D12_MESSAGE_ID_SETPRIVATEDATA_INVALIDFREEDATA";
        case(D3D12_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS):
            return "D3D12_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS";
        case(D3D12_MESSAGE_ID_SETPRIVATEDATA_OUTOFMEMORY):
            return "D3D12_MESSAGE_ID_SETPRIVATEDATA_OUTOFMEMORY";
        case(D3D12_MESSAGE_ID_CREATESHADERRESOURCEVIEW_UNRECOGNIZEDFORMAT):
            return "D3D12_MESSAGE_ID_CREATESHADERRESOURCEVIEW_UNRECOGNIZEDFORMAT";
        case(D3D12_MESSAGE_ID_CREATESHADERRESOURCEVIEW_INVALIDDESC):
            return "D3D12_MESSAGE_ID_CREATESHADERRESOURCEVIEW_INVALIDDESC";
        case(D3D12_MESSAGE_ID_CREATESHADERRESOURCEVIEW_INVALIDFORMAT):
            return "D3D12_MESSAGE_ID_CREATESHADERRESOURCEVIEW_INVALIDFORMAT";
        case(D3D12_MESSAGE_ID_CREATESHADERRESOURCEVIEW_INVALIDVIDEOPLANESLICE):
            return "D3D12_MESSAGE_ID_CREATESHADERRESOURCEVIEW_INVALIDVIDEOPLANESLICE";
        case(D3D12_MESSAGE_ID_CREATESHADERRESOURCEVIEW_INVALIDPLANESLICE):
            return "D3D12_MESSAGE_ID_CREATESHADERRESOURCEVIEW_INVALIDPLANESLICE";
        case(D3D12_MESSAGE_ID_CREATESHADERRESOURCEVIEW_INVALIDDIMENSIONS):
            return "D3D12_MESSAGE_ID_CREATESHADERRESOURCEVIEW_INVALIDDIMENSIONS";
        case(D3D12_MESSAGE_ID_CREATESHADERRESOURCEVIEW_INVALIDRESOURCE):
            return "D3D12_MESSAGE_ID_CREATESHADERRESOURCEVIEW_INVALIDRESOURCE";
        case(D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_UNRECOGNIZEDFORMAT):
            return "D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_UNRECOGNIZEDFORMAT";
        case(D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_UNSUPPORTEDFORMAT):
            return "D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_UNSUPPORTEDFORMAT";
        case(D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_INVALIDDESC):
            return "D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_INVALIDDESC";
        case(D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_INVALIDFORMAT):
            return "D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_INVALIDFORMAT";
        case(D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_INVALIDVIDEOPLANESLICE):
            return "D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_INVALIDVIDEOPLANESLICE";
        case(D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_INVALIDPLANESLICE):
            return "D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_INVALIDPLANESLICE";
        case(D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_INVALIDDIMENSIONS):
            return "D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_INVALIDDIMENSIONS";
        case(D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_INVALIDRESOURCE):
            return "D3D12_MESSAGE_ID_CREATERENDERTARGETVIEW_INVALIDRESOURCE";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILVIEW_UNRECOGNIZEDFORMAT):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILVIEW_UNRECOGNIZEDFORMAT";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILVIEW_INVALIDDESC):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILVIEW_INVALIDDESC";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILVIEW_INVALIDFORMAT):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILVIEW_INVALIDFORMAT";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILVIEW_INVALIDDIMENSIONS):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILVIEW_INVALIDDIMENSIONS";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILVIEW_INVALIDRESOURCE):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILVIEW_INVALIDRESOURCE";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_OUTOFMEMORY):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_OUTOFMEMORY";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_TOOMANYELEMENTS):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_TOOMANYELEMENTS";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_INVALIDFORMAT):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_INVALIDFORMAT";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_INCOMPATIBLEFORMAT):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_INCOMPATIBLEFORMAT";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_INVALIDSLOT):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_INVALIDSLOT";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_INVALIDINPUTSLOTCLASS):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_INVALIDINPUTSLOTCLASS";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_STEPRATESLOTCLASSMISMATCH):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_STEPRATESLOTCLASSMISMATCH";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_INVALIDSLOTCLASSCHANGE):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_INVALIDSLOTCLASSCHANGE";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_INVALIDSTEPRATECHANGE):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_INVALIDSTEPRATECHANGE";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_INVALIDALIGNMENT):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_INVALIDALIGNMENT";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_DUPLICATESEMANTIC):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_DUPLICATESEMANTIC";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_UNPARSEABLEINPUTSIGNATURE):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_UNPARSEABLEINPUTSIGNATURE";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_NULLSEMANTIC):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_NULLSEMANTIC";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_MISSINGELEMENT):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_MISSINGELEMENT";
        case(D3D12_MESSAGE_ID_CREATEVERTEXSHADER_OUTOFMEMORY):
            return "D3D12_MESSAGE_ID_CREATEVERTEXSHADER_OUTOFMEMORY";
        case(D3D12_MESSAGE_ID_CREATEVERTEXSHADER_INVALIDSHADERBYTECODE):
            return "D3D12_MESSAGE_ID_CREATEVERTEXSHADER_INVALIDSHADERBYTECODE";
        case(D3D12_MESSAGE_ID_CREATEVERTEXSHADER_INVALIDSHADERTYPE):
            return "D3D12_MESSAGE_ID_CREATEVERTEXSHADER_INVALIDSHADERTYPE";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADER_OUTOFMEMORY):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADER_OUTOFMEMORY";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADER_INVALIDSHADERBYTECODE):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADER_INVALIDSHADERBYTECODE";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADER_INVALIDSHADERTYPE):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADER_INVALIDSHADERTYPE";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_OUTOFMEMORY):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_OUTOFMEMORY";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDSHADERBYTECODE):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDSHADERBYTECODE";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDSHADERTYPE):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDSHADERTYPE";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDNUMENTRIES):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDNUMENTRIES";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_OUTPUTSTREAMSTRIDEUNUSED):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_OUTPUTSTREAMSTRIDEUNUSED";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_OUTPUTSLOT0EXPECTED):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_OUTPUTSLOT0EXPECTED";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDOUTPUTSLOT):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDOUTPUTSLOT";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_ONLYONEELEMENTPERSLOT):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_ONLYONEELEMENTPERSLOT";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDCOMPONENTCOUNT):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDCOMPONENTCOUNT";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDSTARTCOMPONENTANDCOMPONENTCOUNT):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDSTARTCOMPONENTANDCOMPONENTCOUNT";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDGAPDEFINITION):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDGAPDEFINITION";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_REPEATEDOUTPUT):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_REPEATEDOUTPUT";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDOUTPUTSTREAMSTRIDE):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDOUTPUTSTREAMSTRIDE";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_MISSINGSEMANTIC):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_MISSINGSEMANTIC";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_MASKMISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_MASKMISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_CANTHAVEONLYGAPS):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_CANTHAVEONLYGAPS";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_DECLTOOCOMPLEX):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_DECLTOOCOMPLEX";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_MISSINGOUTPUTSIGNATURE):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_MISSINGOUTPUTSIGNATURE";
        case(D3D12_MESSAGE_ID_CREATEPIXELSHADER_OUTOFMEMORY):
            return "D3D12_MESSAGE_ID_CREATEPIXELSHADER_OUTOFMEMORY";
        case(D3D12_MESSAGE_ID_CREATEPIXELSHADER_INVALIDSHADERBYTECODE):
            return "D3D12_MESSAGE_ID_CREATEPIXELSHADER_INVALIDSHADERBYTECODE";
        case(D3D12_MESSAGE_ID_CREATEPIXELSHADER_INVALIDSHADERTYPE):
            return "D3D12_MESSAGE_ID_CREATEPIXELSHADER_INVALIDSHADERTYPE";
        case(D3D12_MESSAGE_ID_CREATERASTERIZERSTATE_INVALIDFILLMODE):
            return "D3D12_MESSAGE_ID_CREATERASTERIZERSTATE_INVALIDFILLMODE";
        case(D3D12_MESSAGE_ID_CREATERASTERIZERSTATE_INVALIDCULLMODE):
            return "D3D12_MESSAGE_ID_CREATERASTERIZERSTATE_INVALIDCULLMODE";
        case(D3D12_MESSAGE_ID_CREATERASTERIZERSTATE_INVALIDDEPTHBIASCLAMP):
            return "D3D12_MESSAGE_ID_CREATERASTERIZERSTATE_INVALIDDEPTHBIASCLAMP";
        case(D3D12_MESSAGE_ID_CREATERASTERIZERSTATE_INVALIDSLOPESCALEDDEPTHBIAS):
            return "D3D12_MESSAGE_ID_CREATERASTERIZERSTATE_INVALIDSLOPESCALEDDEPTHBIAS";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDDEPTHWRITEMASK):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDDEPTHWRITEMASK";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDDEPTHFUNC):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDDEPTHFUNC";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDFRONTFACESTENCILFAILOP):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDFRONTFACESTENCILFAILOP";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDFRONTFACESTENCILZFAILOP):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDFRONTFACESTENCILZFAILOP";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDFRONTFACESTENCILPASSOP):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDFRONTFACESTENCILPASSOP";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDFRONTFACESTENCILFUNC):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDFRONTFACESTENCILFUNC";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDBACKFACESTENCILFAILOP):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDBACKFACESTENCILFAILOP";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDBACKFACESTENCILZFAILOP):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDBACKFACESTENCILZFAILOP";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDBACKFACESTENCILPASSOP):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDBACKFACESTENCILPASSOP";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDBACKFACESTENCILFUNC):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_INVALIDBACKFACESTENCILFUNC";
        case(D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDSRCBLEND):
            return "D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDSRCBLEND";
        case(D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDDESTBLEND):
            return "D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDDESTBLEND";
        case(D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDBLENDOP):
            return "D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDBLENDOP";
        case(D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDSRCBLENDALPHA):
            return "D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDSRCBLENDALPHA";
        case(D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDDESTBLENDALPHA):
            return "D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDDESTBLENDALPHA";
        case(D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDBLENDOPALPHA):
            return "D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDBLENDOPALPHA";
        case(D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDRENDERTARGETWRITEMASK):
            return "D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDRENDERTARGETWRITEMASK";
        case(D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_INVALID):
            return "D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_INVALID";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_ROOT_SIGNATURE_NOT_SET):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_ROOT_SIGNATURE_NOT_SET";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_ROOT_SIGNATURE_MISMATCH):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_ROOT_SIGNATURE_MISMATCH";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_VERTEX_BUFFER_NOT_SET):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_VERTEX_BUFFER_NOT_SET";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_VERTEX_BUFFER_STRIDE_TOO_SMALL):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_VERTEX_BUFFER_STRIDE_TOO_SMALL";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_VERTEX_BUFFER_TOO_SMALL):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_VERTEX_BUFFER_TOO_SMALL";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_INDEX_BUFFER_NOT_SET):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_INDEX_BUFFER_NOT_SET";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_INDEX_BUFFER_FORMAT_INVALID):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_INDEX_BUFFER_FORMAT_INVALID";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_INDEX_BUFFER_TOO_SMALL):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_INDEX_BUFFER_TOO_SMALL";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_INVALID_PRIMITIVETOPOLOGY):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_INVALID_PRIMITIVETOPOLOGY";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_VERTEX_STRIDE_UNALIGNED):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_VERTEX_STRIDE_UNALIGNED";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_INDEX_OFFSET_UNALIGNED):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_INDEX_OFFSET_UNALIGNED";
        case(D3D12_MESSAGE_ID_DEVICE_REMOVAL_PROCESS_AT_FAULT):
            return "D3D12_MESSAGE_ID_DEVICE_REMOVAL_PROCESS_AT_FAULT";
        case(D3D12_MESSAGE_ID_DEVICE_REMOVAL_PROCESS_POSSIBLY_AT_FAULT):
            return "D3D12_MESSAGE_ID_DEVICE_REMOVAL_PROCESS_POSSIBLY_AT_FAULT";
        case(D3D12_MESSAGE_ID_DEVICE_REMOVAL_PROCESS_NOT_AT_FAULT):
            return "D3D12_MESSAGE_ID_DEVICE_REMOVAL_PROCESS_NOT_AT_FAULT";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_TRAILING_DIGIT_IN_SEMANTIC):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_TRAILING_DIGIT_IN_SEMANTIC";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_TRAILING_DIGIT_IN_SEMANTIC):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_TRAILING_DIGIT_IN_SEMANTIC";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_TYPE_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_TYPE_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_EMPTY_LAYOUT):
            return "D3D12_MESSAGE_ID_CREATEINPUTLAYOUT_EMPTY_LAYOUT";
        case(D3D12_MESSAGE_ID_LIVE_OBJECT_SUMMARY):
            return "D3D12_MESSAGE_ID_LIVE_OBJECT_SUMMARY";
        case(D3D12_MESSAGE_ID_LIVE_DEVICE):
            return "D3D12_MESSAGE_ID_LIVE_DEVICE";
        case(D3D12_MESSAGE_ID_LIVE_SWAPCHAIN):
            return "D3D12_MESSAGE_ID_LIVE_SWAPCHAIN";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILVIEW_INVALIDFLAGS):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILVIEW_INVALIDFLAGS";
        case(D3D12_MESSAGE_ID_CREATEVERTEXSHADER_INVALIDCLASSLINKAGE):
            return "D3D12_MESSAGE_ID_CREATEVERTEXSHADER_INVALIDCLASSLINKAGE";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADER_INVALIDCLASSLINKAGE):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADER_INVALIDCLASSLINKAGE";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDSTREAMTORASTERIZER):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDSTREAMTORASTERIZER";
        case(D3D12_MESSAGE_ID_CREATEPIXELSHADER_INVALIDCLASSLINKAGE):
            return "D3D12_MESSAGE_ID_CREATEPIXELSHADER_INVALIDCLASSLINKAGE";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDSTREAM):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDSTREAM";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_UNEXPECTEDENTRIES):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_UNEXPECTEDENTRIES";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_UNEXPECTEDSTRIDES):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_UNEXPECTEDSTRIDES";
        case(D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDNUMSTRIDES):
            return "D3D12_MESSAGE_ID_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_INVALIDNUMSTRIDES";
        case(D3D12_MESSAGE_ID_CREATEHULLSHADER_OUTOFMEMORY):
            return "D3D12_MESSAGE_ID_CREATEHULLSHADER_OUTOFMEMORY";
        case(D3D12_MESSAGE_ID_CREATEHULLSHADER_INVALIDSHADERBYTECODE):
            return "D3D12_MESSAGE_ID_CREATEHULLSHADER_INVALIDSHADERBYTECODE";
        case(D3D12_MESSAGE_ID_CREATEHULLSHADER_INVALIDSHADERTYPE):
            return "D3D12_MESSAGE_ID_CREATEHULLSHADER_INVALIDSHADERTYPE";
        case(D3D12_MESSAGE_ID_CREATEHULLSHADER_INVALIDCLASSLINKAGE):
            return "D3D12_MESSAGE_ID_CREATEHULLSHADER_INVALIDCLASSLINKAGE";
        case(D3D12_MESSAGE_ID_CREATEDOMAINSHADER_OUTOFMEMORY):
            return "D3D12_MESSAGE_ID_CREATEDOMAINSHADER_OUTOFMEMORY";
        case(D3D12_MESSAGE_ID_CREATEDOMAINSHADER_INVALIDSHADERBYTECODE):
            return "D3D12_MESSAGE_ID_CREATEDOMAINSHADER_INVALIDSHADERBYTECODE";
        case(D3D12_MESSAGE_ID_CREATEDOMAINSHADER_INVALIDSHADERTYPE):
            return "D3D12_MESSAGE_ID_CREATEDOMAINSHADER_INVALIDSHADERTYPE";
        case(D3D12_MESSAGE_ID_CREATEDOMAINSHADER_INVALIDCLASSLINKAGE):
            return "D3D12_MESSAGE_ID_CREATEDOMAINSHADER_INVALIDCLASSLINKAGE";
        case(D3D12_MESSAGE_ID_RESOURCE_UNMAP_NOTMAPPED):
            return "D3D12_MESSAGE_ID_RESOURCE_UNMAP_NOTMAPPED";
        case(D3D12_MESSAGE_ID_DEVICE_CHECKFEATURESUPPORT_MISMATCHED_DATA_SIZE):
            return "D3D12_MESSAGE_ID_DEVICE_CHECKFEATURESUPPORT_MISMATCHED_DATA_SIZE";
        case(D3D12_MESSAGE_ID_CREATECOMPUTESHADER_OUTOFMEMORY):
            return "D3D12_MESSAGE_ID_CREATECOMPUTESHADER_OUTOFMEMORY";
        case(D3D12_MESSAGE_ID_CREATECOMPUTESHADER_INVALIDSHADERBYTECODE):
            return "D3D12_MESSAGE_ID_CREATECOMPUTESHADER_INVALIDSHADERBYTECODE";
        case(D3D12_MESSAGE_ID_CREATECOMPUTESHADER_INVALIDCLASSLINKAGE):
            return "D3D12_MESSAGE_ID_CREATECOMPUTESHADER_INVALIDCLASSLINKAGE";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEVERTEXSHADER_DOUBLEFLOATOPSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEVERTEXSHADER_DOUBLEFLOATOPSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEHULLSHADER_DOUBLEFLOATOPSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEHULLSHADER_DOUBLEFLOATOPSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEDOMAINSHADER_DOUBLEFLOATOPSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEDOMAINSHADER_DOUBLEFLOATOPSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEGEOMETRYSHADER_DOUBLEFLOATOPSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEGEOMETRYSHADER_DOUBLEFLOATOPSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_DOUBLEFLOATOPSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_DOUBLEFLOATOPSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEPIXELSHADER_DOUBLEFLOATOPSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEPIXELSHADER_DOUBLEFLOATOPSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATECOMPUTESHADER_DOUBLEFLOATOPSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATECOMPUTESHADER_DOUBLEFLOATOPSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_INVALIDRESOURCE):
            return "D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_INVALIDRESOURCE";
        case(D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_INVALIDDESC):
            return "D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_INVALIDDESC";
        case(D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_INVALIDFORMAT):
            return "D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_INVALIDFORMAT";
        case(D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_INVALIDVIDEOPLANESLICE):
            return "D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_INVALIDVIDEOPLANESLICE";
        case(D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_INVALIDPLANESLICE):
            return "D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_INVALIDPLANESLICE";
        case(D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_INVALIDDIMENSIONS):
            return "D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_INVALIDDIMENSIONS";
        case(D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_UNRECOGNIZEDFORMAT):
            return "D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_UNRECOGNIZEDFORMAT";
        case(D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_INVALIDFLAGS):
            return "D3D12_MESSAGE_ID_CREATEUNORDEREDACCESSVIEW_INVALIDFLAGS";
        case(D3D12_MESSAGE_ID_CREATERASTERIZERSTATE_INVALIDFORCEDSAMPLECOUNT):
            return "D3D12_MESSAGE_ID_CREATERASTERIZERSTATE_INVALIDFORCEDSAMPLECOUNT";
        case(D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDLOGICOPS):
            return "D3D12_MESSAGE_ID_CREATEBLENDSTATE_INVALIDLOGICOPS";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEVERTEXSHADER_DOUBLEEXTENSIONSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEVERTEXSHADER_DOUBLEEXTENSIONSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEHULLSHADER_DOUBLEEXTENSIONSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEHULLSHADER_DOUBLEEXTENSIONSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEDOMAINSHADER_DOUBLEEXTENSIONSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEDOMAINSHADER_DOUBLEEXTENSIONSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEGEOMETRYSHADER_DOUBLEEXTENSIONSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEGEOMETRYSHADER_DOUBLEEXTENSIONSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_DOUBLEEXTENSIONSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_DOUBLEEXTENSIONSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEPIXELSHADER_DOUBLEEXTENSIONSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEPIXELSHADER_DOUBLEEXTENSIONSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATECOMPUTESHADER_DOUBLEEXTENSIONSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATECOMPUTESHADER_DOUBLEEXTENSIONSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEVERTEXSHADER_UAVSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEVERTEXSHADER_UAVSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEHULLSHADER_UAVSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEHULLSHADER_UAVSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEDOMAINSHADER_UAVSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEDOMAINSHADER_UAVSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEGEOMETRYSHADER_UAVSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEGEOMETRYSHADER_UAVSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_UAVSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEGEOMETRYSHADERWITHSTREAMOUTPUT_UAVSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATEPIXELSHADER_UAVSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATEPIXELSHADER_UAVSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CREATECOMPUTESHADER_UAVSNOTSUPPORTED):
            return "D3D12_MESSAGE_ID_DEVICE_CREATECOMPUTESHADER_UAVSNOTSUPPORTED";
        case(D3D12_MESSAGE_ID_DEVICE_CLEARVIEW_INVALIDSOURCERECT):
            return "D3D12_MESSAGE_ID_DEVICE_CLEARVIEW_INVALIDSOURCERECT";
        case(D3D12_MESSAGE_ID_DEVICE_CLEARVIEW_EMPTYRECT):
            return "D3D12_MESSAGE_ID_DEVICE_CLEARVIEW_EMPTYRECT";
        case(D3D12_MESSAGE_ID_UPDATETILEMAPPINGS_INVALID_PARAMETER):
            return "D3D12_MESSAGE_ID_UPDATETILEMAPPINGS_INVALID_PARAMETER";
        case(D3D12_MESSAGE_ID_COPYTILEMAPPINGS_INVALID_PARAMETER):
            return "D3D12_MESSAGE_ID_COPYTILEMAPPINGS_INVALID_PARAMETER";
        case(D3D12_MESSAGE_ID_CREATEDEVICE_INVALIDARGS):
            return "D3D12_MESSAGE_ID_CREATEDEVICE_INVALIDARGS";
        case(D3D12_MESSAGE_ID_CREATEDEVICE_WARNING):
            return "D3D12_MESSAGE_ID_CREATEDEVICE_WARNING";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_TYPE):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_TYPE";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_NULL_POINTER):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_NULL_POINTER";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_SUBRESOURCE):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_SUBRESOURCE";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_RESERVED_BITS):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_RESERVED_BITS";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISSING_BIND_FLAGS):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISSING_BIND_FLAGS";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_MISC_FLAGS):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_MISC_FLAGS";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_MATCHING_STATES):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_MATCHING_STATES";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_COMBINATION):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_COMBINATION";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_BEFORE_AFTER_MISMATCH):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_BEFORE_AFTER_MISMATCH";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_RESOURCE):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_RESOURCE";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_SAMPLE_COUNT):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_SAMPLE_COUNT";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_FLAGS):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_FLAGS";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_COMBINED_FLAGS):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_COMBINED_FLAGS";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_FLAGS_FOR_FORMAT):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_FLAGS_FOR_FORMAT";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_SPLIT_BARRIER):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_SPLIT_BARRIER";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_UNMATCHED_END):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_UNMATCHED_END";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_UNMATCHED_BEGIN):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_UNMATCHED_BEGIN";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_FLAG):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_FLAG";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_COMMAND_LIST_TYPE):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_COMMAND_LIST_TYPE";
        case(D3D12_MESSAGE_ID_INVALID_SUBRESOURCE_STATE):
            return "D3D12_MESSAGE_ID_INVALID_SUBRESOURCE_STATE";
        case(D3D12_MESSAGE_ID_COMMAND_ALLOCATOR_CONTENTION):
            return "D3D12_MESSAGE_ID_COMMAND_ALLOCATOR_CONTENTION";
        case(D3D12_MESSAGE_ID_COMMAND_ALLOCATOR_RESET):
            return "D3D12_MESSAGE_ID_COMMAND_ALLOCATOR_RESET";
        case(D3D12_MESSAGE_ID_COMMAND_ALLOCATOR_RESET_BUNDLE):
            return "D3D12_MESSAGE_ID_COMMAND_ALLOCATOR_RESET_BUNDLE";
        case(D3D12_MESSAGE_ID_COMMAND_ALLOCATOR_CANNOT_RESET):
            return "D3D12_MESSAGE_ID_COMMAND_ALLOCATOR_CANNOT_RESET";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_OPEN):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_OPEN";
        case(D3D12_MESSAGE_ID_INVALID_BUNDLE_API):
            return "D3D12_MESSAGE_ID_INVALID_BUNDLE_API";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_CLOSED):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_CLOSED";
        case(D3D12_MESSAGE_ID_WRONG_COMMAND_ALLOCATOR_TYPE):
            return "D3D12_MESSAGE_ID_WRONG_COMMAND_ALLOCATOR_TYPE";
        case(D3D12_MESSAGE_ID_COMMAND_ALLOCATOR_SYNC):
            return "D3D12_MESSAGE_ID_COMMAND_ALLOCATOR_SYNC";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_SYNC):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_SYNC";
        case(D3D12_MESSAGE_ID_SET_DESCRIPTOR_HEAP_INVALID):
            return "D3D12_MESSAGE_ID_SET_DESCRIPTOR_HEAP_INVALID";
        case(D3D12_MESSAGE_ID_CREATE_COMMANDQUEUE):
            return "D3D12_MESSAGE_ID_CREATE_COMMANDQUEUE";
        case(D3D12_MESSAGE_ID_CREATE_COMMANDALLOCATOR):
            return "D3D12_MESSAGE_ID_CREATE_COMMANDALLOCATOR";
        case(D3D12_MESSAGE_ID_CREATE_PIPELINESTATE):
            return "D3D12_MESSAGE_ID_CREATE_PIPELINESTATE";
        case(D3D12_MESSAGE_ID_CREATE_COMMANDLIST12):
            return "D3D12_MESSAGE_ID_CREATE_COMMANDLIST12";
        case(D3D12_MESSAGE_ID_CREATE_RESOURCE):
            return "D3D12_MESSAGE_ID_CREATE_RESOURCE";
        case(D3D12_MESSAGE_ID_CREATE_DESCRIPTORHEAP):
            return "D3D12_MESSAGE_ID_CREATE_DESCRIPTORHEAP";
        case(D3D12_MESSAGE_ID_CREATE_ROOTSIGNATURE):
            return "D3D12_MESSAGE_ID_CREATE_ROOTSIGNATURE";
        case(D3D12_MESSAGE_ID_CREATE_LIBRARY):
            return "D3D12_MESSAGE_ID_CREATE_LIBRARY";
        case(D3D12_MESSAGE_ID_CREATE_HEAP):
            return "D3D12_MESSAGE_ID_CREATE_HEAP";
        case(D3D12_MESSAGE_ID_CREATE_MONITOREDFENCE):
            return "D3D12_MESSAGE_ID_CREATE_MONITOREDFENCE";
        case(D3D12_MESSAGE_ID_CREATE_QUERYHEAP):
            return "D3D12_MESSAGE_ID_CREATE_QUERYHEAP";
        case(D3D12_MESSAGE_ID_CREATE_COMMANDSIGNATURE):
            return "D3D12_MESSAGE_ID_CREATE_COMMANDSIGNATURE";
        case(D3D12_MESSAGE_ID_LIVE_COMMANDQUEUE):
            return "D3D12_MESSAGE_ID_LIVE_COMMANDQUEUE";
        case(D3D12_MESSAGE_ID_LIVE_COMMANDALLOCATOR):
            return "D3D12_MESSAGE_ID_LIVE_COMMANDALLOCATOR";
        case(D3D12_MESSAGE_ID_LIVE_PIPELINESTATE):
            return "D3D12_MESSAGE_ID_LIVE_PIPELINESTATE";
        case(D3D12_MESSAGE_ID_LIVE_COMMANDLIST12):
            return "D3D12_MESSAGE_ID_LIVE_COMMANDLIST12";
        case(D3D12_MESSAGE_ID_LIVE_RESOURCE):
            return "D3D12_MESSAGE_ID_LIVE_RESOURCE";
        case(D3D12_MESSAGE_ID_LIVE_DESCRIPTORHEAP):
            return "D3D12_MESSAGE_ID_LIVE_DESCRIPTORHEAP";
        case(D3D12_MESSAGE_ID_LIVE_ROOTSIGNATURE):
            return "D3D12_MESSAGE_ID_LIVE_ROOTSIGNATURE";
        case(D3D12_MESSAGE_ID_LIVE_LIBRARY):
            return "D3D12_MESSAGE_ID_LIVE_LIBRARY";
        case(D3D12_MESSAGE_ID_LIVE_HEAP):
            return "D3D12_MESSAGE_ID_LIVE_HEAP";
        case(D3D12_MESSAGE_ID_LIVE_MONITOREDFENCE):
            return "D3D12_MESSAGE_ID_LIVE_MONITOREDFENCE";
        case(D3D12_MESSAGE_ID_LIVE_QUERYHEAP):
            return "D3D12_MESSAGE_ID_LIVE_QUERYHEAP";
        case(D3D12_MESSAGE_ID_LIVE_COMMANDSIGNATURE):
            return "D3D12_MESSAGE_ID_LIVE_COMMANDSIGNATURE";
        case(D3D12_MESSAGE_ID_DESTROY_COMMANDQUEUE):
            return "D3D12_MESSAGE_ID_DESTROY_COMMANDQUEUE";
        case(D3D12_MESSAGE_ID_DESTROY_COMMANDALLOCATOR):
            return "D3D12_MESSAGE_ID_DESTROY_COMMANDALLOCATOR";
        case(D3D12_MESSAGE_ID_DESTROY_PIPELINESTATE):
            return "D3D12_MESSAGE_ID_DESTROY_PIPELINESTATE";
        case(D3D12_MESSAGE_ID_DESTROY_COMMANDLIST12):
            return "D3D12_MESSAGE_ID_DESTROY_COMMANDLIST12";
        case(D3D12_MESSAGE_ID_DESTROY_RESOURCE):
            return "D3D12_MESSAGE_ID_DESTROY_RESOURCE";
        case(D3D12_MESSAGE_ID_DESTROY_DESCRIPTORHEAP):
            return "D3D12_MESSAGE_ID_DESTROY_DESCRIPTORHEAP";
        case(D3D12_MESSAGE_ID_DESTROY_ROOTSIGNATURE):
            return "D3D12_MESSAGE_ID_DESTROY_ROOTSIGNATURE";
        case(D3D12_MESSAGE_ID_DESTROY_LIBRARY):
            return "D3D12_MESSAGE_ID_DESTROY_LIBRARY";
        case(D3D12_MESSAGE_ID_DESTROY_HEAP):
            return "D3D12_MESSAGE_ID_DESTROY_HEAP";
        case(D3D12_MESSAGE_ID_DESTROY_MONITOREDFENCE):
            return "D3D12_MESSAGE_ID_DESTROY_MONITOREDFENCE";
        case(D3D12_MESSAGE_ID_DESTROY_QUERYHEAP):
            return "D3D12_MESSAGE_ID_DESTROY_QUERYHEAP";
        case(D3D12_MESSAGE_ID_DESTROY_COMMANDSIGNATURE):
            return "D3D12_MESSAGE_ID_DESTROY_COMMANDSIGNATURE";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDDIMENSIONS):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDDIMENSIONS";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDMISCFLAGS):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDMISCFLAGS";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDARG_RETURN):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDARG_RETURN";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_OUTOFMEMORY_RETURN):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_OUTOFMEMORY_RETURN";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDDESC):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDDESC";
        case(D3D12_MESSAGE_ID_POSSIBLY_INVALID_SUBRESOURCE_STATE):
            return "D3D12_MESSAGE_ID_POSSIBLY_INVALID_SUBRESOURCE_STATE";
        case(D3D12_MESSAGE_ID_INVALID_USE_OF_NON_RESIDENT_RESOURCE):
            return "D3D12_MESSAGE_ID_INVALID_USE_OF_NON_RESIDENT_RESOURCE";
        case(D3D12_MESSAGE_ID_POSSIBLE_INVALID_USE_OF_NON_RESIDENT_RESOURCE):
            return "D3D12_MESSAGE_ID_POSSIBLE_INVALID_USE_OF_NON_RESIDENT_RESOURCE";
        case(D3D12_MESSAGE_ID_BUNDLE_PIPELINE_STATE_MISMATCH):
            return "D3D12_MESSAGE_ID_BUNDLE_PIPELINE_STATE_MISMATCH";
        case(D3D12_MESSAGE_ID_PRIMITIVE_TOPOLOGY_MISMATCH_PIPELINE_STATE):
            return "D3D12_MESSAGE_ID_PRIMITIVE_TOPOLOGY_MISMATCH_PIPELINE_STATE";
        case(D3D12_MESSAGE_ID_RENDER_TARGET_FORMAT_MISMATCH_PIPELINE_STATE):
            return "D3D12_MESSAGE_ID_RENDER_TARGET_FORMAT_MISMATCH_PIPELINE_STATE";
        case(D3D12_MESSAGE_ID_RENDER_TARGET_SAMPLE_DESC_MISMATCH_PIPELINE_STATE):
            return "D3D12_MESSAGE_ID_RENDER_TARGET_SAMPLE_DESC_MISMATCH_PIPELINE_STATE";
        case(D3D12_MESSAGE_ID_DEPTH_STENCIL_FORMAT_MISMATCH_PIPELINE_STATE):
            return "D3D12_MESSAGE_ID_DEPTH_STENCIL_FORMAT_MISMATCH_PIPELINE_STATE";
        case(D3D12_MESSAGE_ID_DEPTH_STENCIL_SAMPLE_DESC_MISMATCH_PIPELINE_STATE):
            return "D3D12_MESSAGE_ID_DEPTH_STENCIL_SAMPLE_DESC_MISMATCH_PIPELINE_STATE";
        case(D3D12_MESSAGE_ID_CREATESHADER_INVALIDBYTECODE):
            return "D3D12_MESSAGE_ID_CREATESHADER_INVALIDBYTECODE";
        case(D3D12_MESSAGE_ID_CREATEHEAP_NULLDESC):
            return "D3D12_MESSAGE_ID_CREATEHEAP_NULLDESC";
        case(D3D12_MESSAGE_ID_CREATEHEAP_INVALIDSIZE):
            return "D3D12_MESSAGE_ID_CREATEHEAP_INVALIDSIZE";
        case(D3D12_MESSAGE_ID_CREATEHEAP_UNRECOGNIZEDHEAPTYPE):
            return "D3D12_MESSAGE_ID_CREATEHEAP_UNRECOGNIZEDHEAPTYPE";
        case(D3D12_MESSAGE_ID_CREATEHEAP_UNRECOGNIZEDCPUPAGEPROPERTIES):
            return "D3D12_MESSAGE_ID_CREATEHEAP_UNRECOGNIZEDCPUPAGEPROPERTIES";
        case(D3D12_MESSAGE_ID_CREATEHEAP_UNRECOGNIZEDMEMORYPOOL):
            return "D3D12_MESSAGE_ID_CREATEHEAP_UNRECOGNIZEDMEMORYPOOL";
        case(D3D12_MESSAGE_ID_CREATEHEAP_INVALIDPROPERTIES):
            return "D3D12_MESSAGE_ID_CREATEHEAP_INVALIDPROPERTIES";
        case(D3D12_MESSAGE_ID_CREATEHEAP_INVALIDALIGNMENT):
            return "D3D12_MESSAGE_ID_CREATEHEAP_INVALIDALIGNMENT";
        case(D3D12_MESSAGE_ID_CREATEHEAP_UNRECOGNIZEDMISCFLAGS):
            return "D3D12_MESSAGE_ID_CREATEHEAP_UNRECOGNIZEDMISCFLAGS";
        case(D3D12_MESSAGE_ID_CREATEHEAP_INVALIDMISCFLAGS):
            return "D3D12_MESSAGE_ID_CREATEHEAP_INVALIDMISCFLAGS";
        case(D3D12_MESSAGE_ID_CREATEHEAP_INVALIDARG_RETURN):
            return "D3D12_MESSAGE_ID_CREATEHEAP_INVALIDARG_RETURN";
        case(D3D12_MESSAGE_ID_CREATEHEAP_OUTOFMEMORY_RETURN):
            return "D3D12_MESSAGE_ID_CREATEHEAP_OUTOFMEMORY_RETURN";
        case(D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_NULLHEAPPROPERTIES):
            return "D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_NULLHEAPPROPERTIES";
        case(D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_UNRECOGNIZEDHEAPTYPE):
            return "D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_UNRECOGNIZEDHEAPTYPE";
        case(D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_UNRECOGNIZEDCPUPAGEPROPERTIES):
            return "D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_UNRECOGNIZEDCPUPAGEPROPERTIES";
        case(D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_UNRECOGNIZEDMEMORYPOOL):
            return "D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_UNRECOGNIZEDMEMORYPOOL";
        case(D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_INVALIDHEAPPROPERTIES):
            return "D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_INVALIDHEAPPROPERTIES";
        case(D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_UNRECOGNIZEDHEAPMISCFLAGS):
            return "D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_UNRECOGNIZEDHEAPMISCFLAGS";
        case(D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_INVALIDHEAPMISCFLAGS):
            return "D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_INVALIDHEAPMISCFLAGS";
        case(D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_INVALIDARG_RETURN):
            return "D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_INVALIDARG_RETURN";
        case(D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_OUTOFMEMORY_RETURN):
            return "D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_OUTOFMEMORY_RETURN";
        case(D3D12_MESSAGE_ID_GETCUSTOMHEAPPROPERTIES_UNRECOGNIZEDHEAPTYPE):
            return "D3D12_MESSAGE_ID_GETCUSTOMHEAPPROPERTIES_UNRECOGNIZEDHEAPTYPE";
        case(D3D12_MESSAGE_ID_GETCUSTOMHEAPPROPERTIES_INVALIDHEAPTYPE):
            return "D3D12_MESSAGE_ID_GETCUSTOMHEAPPROPERTIES_INVALIDHEAPTYPE";
        case(D3D12_MESSAGE_ID_CREATE_DESCRIPTOR_HEAP_INVALID_DESC):
            return "D3D12_MESSAGE_ID_CREATE_DESCRIPTOR_HEAP_INVALID_DESC";
        case(D3D12_MESSAGE_ID_INVALID_DESCRIPTOR_HANDLE):
            return "D3D12_MESSAGE_ID_INVALID_DESCRIPTOR_HANDLE";
        case(D3D12_MESSAGE_ID_CREATERASTERIZERSTATE_INVALID_CONSERVATIVERASTERMODE):
            return "D3D12_MESSAGE_ID_CREATERASTERIZERSTATE_INVALID_CONSERVATIVERASTERMODE";
        case(D3D12_MESSAGE_ID_CREATE_CONSTANT_BUFFER_VIEW_INVALID_RESOURCE):
            return "D3D12_MESSAGE_ID_CREATE_CONSTANT_BUFFER_VIEW_INVALID_RESOURCE";
        case(D3D12_MESSAGE_ID_CREATE_CONSTANT_BUFFER_VIEW_INVALID_DESC):
            return "D3D12_MESSAGE_ID_CREATE_CONSTANT_BUFFER_VIEW_INVALID_DESC";
        case(D3D12_MESSAGE_ID_CREATE_UNORDEREDACCESS_VIEW_INVALID_COUNTER_USAGE):
            return "D3D12_MESSAGE_ID_CREATE_UNORDEREDACCESS_VIEW_INVALID_COUNTER_USAGE";
        case(D3D12_MESSAGE_ID_COPY_DESCRIPTORS_INVALID_RANGES):
            return "D3D12_MESSAGE_ID_COPY_DESCRIPTORS_INVALID_RANGES";
        case(D3D12_MESSAGE_ID_COPY_DESCRIPTORS_WRITE_ONLY_DESCRIPTOR):
            return "D3D12_MESSAGE_ID_COPY_DESCRIPTORS_WRITE_ONLY_DESCRIPTOR";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_RTV_FORMAT_NOT_UNKNOWN):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_RTV_FORMAT_NOT_UNKNOWN";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INVALID_RENDER_TARGET_COUNT):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INVALID_RENDER_TARGET_COUNT";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_VERTEX_SHADER_NOT_SET):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_VERTEX_SHADER_NOT_SET";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INPUTLAYOUT_NOT_SET):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INPUTLAYOUT_NOT_SET";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_HS_DS_SIGNATURE_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_HS_DS_SIGNATURE_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_REGISTERINDEX):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_REGISTERINDEX";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_COMPONENTTYPE):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_COMPONENTTYPE";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_REGISTERMASK):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_REGISTERMASK";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_SYSTEMVALUE):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_SYSTEMVALUE";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_NEVERWRITTEN_ALWAYSREADS):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_NEVERWRITTEN_ALWAYSREADS";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_MINPRECISION):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_MINPRECISION";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_SEMANTICNAME_NOT_FOUND):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_LINKAGE_SEMANTICNAME_NOT_FOUND";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_HS_XOR_DS_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_HS_XOR_DS_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_HULL_SHADER_INPUT_TOPOLOGY_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_HULL_SHADER_INPUT_TOPOLOGY_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_HS_DS_CONTROL_POINT_COUNT_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_HS_DS_CONTROL_POINT_COUNT_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_HS_DS_TESSELLATOR_DOMAIN_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_HS_DS_TESSELLATOR_DOMAIN_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INVALID_USE_OF_CENTER_MULTISAMPLE_PATTERN):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INVALID_USE_OF_CENTER_MULTISAMPLE_PATTERN";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INVALID_USE_OF_FORCED_SAMPLE_COUNT):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INVALID_USE_OF_FORCED_SAMPLE_COUNT";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INVALID_PRIMITIVETOPOLOGY):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INVALID_PRIMITIVETOPOLOGY";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INVALID_SYSTEMVALUE):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INVALID_SYSTEMVALUE";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_OM_DUAL_SOURCE_BLENDING_CAN_ONLY_HAVE_RENDER_TARGET_0):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_OM_DUAL_SOURCE_BLENDING_CAN_ONLY_HAVE_RENDER_TARGET_0";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_OM_RENDER_TARGET_DOES_NOT_SUPPORT_BLENDING):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_OM_RENDER_TARGET_DOES_NOT_SUPPORT_BLENDING";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_PS_OUTPUT_TYPE_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_PS_OUTPUT_TYPE_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_OM_RENDER_TARGET_DOES_NOT_SUPPORT_LOGIC_OPS):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_OM_RENDER_TARGET_DOES_NOT_SUPPORT_LOGIC_OPS";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_RENDERTARGETVIEW_NOT_SET):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_RENDERTARGETVIEW_NOT_SET";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_DEPTHSTENCILVIEW_NOT_SET):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_DEPTHSTENCILVIEW_NOT_SET";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_GS_INPUT_PRIMITIVE_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_GS_INPUT_PRIMITIVE_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_POSITION_NOT_PRESENT):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_POSITION_NOT_PRESENT";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_MISSING_ROOT_SIGNATURE_FLAGS):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_MISSING_ROOT_SIGNATURE_FLAGS";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INVALID_INDEX_BUFFER_PROPERTIES):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INVALID_INDEX_BUFFER_PROPERTIES";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INVALID_SAMPLE_DESC):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INVALID_SAMPLE_DESC";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_HS_ROOT_SIGNATURE_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_HS_ROOT_SIGNATURE_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_DS_ROOT_SIGNATURE_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_DS_ROOT_SIGNATURE_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_VS_ROOT_SIGNATURE_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_VS_ROOT_SIGNATURE_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_GS_ROOT_SIGNATURE_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_GS_ROOT_SIGNATURE_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_PS_ROOT_SIGNATURE_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_PS_ROOT_SIGNATURE_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_MISSING_ROOT_SIGNATURE):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_MISSING_ROOT_SIGNATURE";
        case(D3D12_MESSAGE_ID_EXECUTE_BUNDLE_OPEN_BUNDLE):
            return "D3D12_MESSAGE_ID_EXECUTE_BUNDLE_OPEN_BUNDLE";
        case(D3D12_MESSAGE_ID_EXECUTE_BUNDLE_DESCRIPTOR_HEAP_MISMATCH):
            return "D3D12_MESSAGE_ID_EXECUTE_BUNDLE_DESCRIPTOR_HEAP_MISMATCH";
        case(D3D12_MESSAGE_ID_EXECUTE_BUNDLE_TYPE):
            return "D3D12_MESSAGE_ID_EXECUTE_BUNDLE_TYPE";
        case(D3D12_MESSAGE_ID_DRAW_EMPTY_SCISSOR_RECTANGLE):
            return "D3D12_MESSAGE_ID_DRAW_EMPTY_SCISSOR_RECTANGLE";
        case(D3D12_MESSAGE_ID_CREATE_ROOT_SIGNATURE_BLOB_NOT_FOUND):
            return "D3D12_MESSAGE_ID_CREATE_ROOT_SIGNATURE_BLOB_NOT_FOUND";
        case(D3D12_MESSAGE_ID_CREATE_ROOT_SIGNATURE_DESERIALIZE_FAILED):
            return "D3D12_MESSAGE_ID_CREATE_ROOT_SIGNATURE_DESERIALIZE_FAILED";
        case(D3D12_MESSAGE_ID_CREATE_ROOT_SIGNATURE_INVALID_CONFIGURATION):
            return "D3D12_MESSAGE_ID_CREATE_ROOT_SIGNATURE_INVALID_CONFIGURATION";
        case(D3D12_MESSAGE_ID_CREATE_ROOT_SIGNATURE_NOT_SUPPORTED_ON_DEVICE):
            return "D3D12_MESSAGE_ID_CREATE_ROOT_SIGNATURE_NOT_SUPPORTED_ON_DEVICE";
        case(D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_NULLRESOURCEPROPERTIES):
            return "D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_NULLRESOURCEPROPERTIES";
        case(D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_NULLHEAP):
            return "D3D12_MESSAGE_ID_CREATERESOURCEANDHEAP_NULLHEAP";
        case(D3D12_MESSAGE_ID_GETRESOURCEALLOCATIONINFO_INVALIDRDESCS):
            return "D3D12_MESSAGE_ID_GETRESOURCEALLOCATIONINFO_INVALIDRDESCS";
        case(D3D12_MESSAGE_ID_MAKERESIDENT_NULLOBJECTARRAY):
            return "D3D12_MESSAGE_ID_MAKERESIDENT_NULLOBJECTARRAY";
        case(D3D12_MESSAGE_ID_EVICT_NULLOBJECTARRAY):
            return "D3D12_MESSAGE_ID_EVICT_NULLOBJECTARRAY";
        case(D3D12_MESSAGE_ID_SET_DESCRIPTOR_TABLE_INVALID):
            return "D3D12_MESSAGE_ID_SET_DESCRIPTOR_TABLE_INVALID";
        case(D3D12_MESSAGE_ID_SET_ROOT_CONSTANT_INVALID):
            return "D3D12_MESSAGE_ID_SET_ROOT_CONSTANT_INVALID";
        case(D3D12_MESSAGE_ID_SET_ROOT_CONSTANT_BUFFER_VIEW_INVALID):
            return "D3D12_MESSAGE_ID_SET_ROOT_CONSTANT_BUFFER_VIEW_INVALID";
        case(D3D12_MESSAGE_ID_SET_ROOT_SHADER_RESOURCE_VIEW_INVALID):
            return "D3D12_MESSAGE_ID_SET_ROOT_SHADER_RESOURCE_VIEW_INVALID";
        case(D3D12_MESSAGE_ID_SET_ROOT_UNORDERED_ACCESS_VIEW_INVALID):
            return "D3D12_MESSAGE_ID_SET_ROOT_UNORDERED_ACCESS_VIEW_INVALID";
        case(D3D12_MESSAGE_ID_SET_VERTEX_BUFFERS_INVALID_DESC):
            return "D3D12_MESSAGE_ID_SET_VERTEX_BUFFERS_INVALID_DESC";
        case(D3D12_MESSAGE_ID_SET_INDEX_BUFFER_INVALID_DESC):
            return "D3D12_MESSAGE_ID_SET_INDEX_BUFFER_INVALID_DESC";
        case(D3D12_MESSAGE_ID_SET_STREAM_OUTPUT_BUFFERS_INVALID_DESC):
            return "D3D12_MESSAGE_ID_SET_STREAM_OUTPUT_BUFFERS_INVALID_DESC";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_UNRECOGNIZEDDIMENSIONALITY):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_UNRECOGNIZEDDIMENSIONALITY";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_UNRECOGNIZEDLAYOUT):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_UNRECOGNIZEDLAYOUT";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDDIMENSIONALITY):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDDIMENSIONALITY";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDALIGNMENT):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDALIGNMENT";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDMIPLEVELS):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDMIPLEVELS";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDSAMPLEDESC):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDSAMPLEDESC";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDLAYOUT):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDLAYOUT";
        case(D3D12_MESSAGE_ID_SET_INDEX_BUFFER_INVALID):
            return "D3D12_MESSAGE_ID_SET_INDEX_BUFFER_INVALID";
        case(D3D12_MESSAGE_ID_SET_VERTEX_BUFFERS_INVALID):
            return "D3D12_MESSAGE_ID_SET_VERTEX_BUFFERS_INVALID";
        case(D3D12_MESSAGE_ID_SET_STREAM_OUTPUT_BUFFERS_INVALID):
            return "D3D12_MESSAGE_ID_SET_STREAM_OUTPUT_BUFFERS_INVALID";
        case(D3D12_MESSAGE_ID_SET_RENDER_TARGETS_INVALID):
            return "D3D12_MESSAGE_ID_SET_RENDER_TARGETS_INVALID";
        case(D3D12_MESSAGE_ID_CREATEQUERY_HEAP_INVALID_PARAMETERS):
            return "D3D12_MESSAGE_ID_CREATEQUERY_HEAP_INVALID_PARAMETERS";
        case(D3D12_MESSAGE_ID_BEGIN_END_QUERY_INVALID_PARAMETERS):
            return "D3D12_MESSAGE_ID_BEGIN_END_QUERY_INVALID_PARAMETERS";
        case(D3D12_MESSAGE_ID_CLOSE_COMMAND_LIST_OPEN_QUERY):
            return "D3D12_MESSAGE_ID_CLOSE_COMMAND_LIST_OPEN_QUERY";
        case(D3D12_MESSAGE_ID_RESOLVE_QUERY_DATA_INVALID_PARAMETERS):
            return "D3D12_MESSAGE_ID_RESOLVE_QUERY_DATA_INVALID_PARAMETERS";
        case(D3D12_MESSAGE_ID_SET_PREDICATION_INVALID_PARAMETERS):
            return "D3D12_MESSAGE_ID_SET_PREDICATION_INVALID_PARAMETERS";
        case(D3D12_MESSAGE_ID_TIMESTAMPS_NOT_SUPPORTED):
            return "D3D12_MESSAGE_ID_TIMESTAMPS_NOT_SUPPORTED";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_UNRECOGNIZEDFORMAT):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_UNRECOGNIZEDFORMAT";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDFORMAT):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDFORMAT";
        case(D3D12_MESSAGE_ID_GETCOPYABLEFOOTPRINTS_INVALIDSUBRESOURCERANGE):
            return "D3D12_MESSAGE_ID_GETCOPYABLEFOOTPRINTS_INVALIDSUBRESOURCERANGE";
        case(D3D12_MESSAGE_ID_GETCOPYABLEFOOTPRINTS_INVALIDBASEOFFSET):
            return "D3D12_MESSAGE_ID_GETCOPYABLEFOOTPRINTS_INVALIDBASEOFFSET";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_HEAP):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_INVALID_HEAP";
        case(D3D12_MESSAGE_ID_CREATE_SAMPLER_INVALID):
            return "D3D12_MESSAGE_ID_CREATE_SAMPLER_INVALID";
        case(D3D12_MESSAGE_ID_CREATECOMMANDSIGNATURE_INVALID):
            return "D3D12_MESSAGE_ID_CREATECOMMANDSIGNATURE_INVALID";
        case(D3D12_MESSAGE_ID_EXECUTE_INDIRECT_INVALID_PARAMETERS):
            return "D3D12_MESSAGE_ID_EXECUTE_INDIRECT_INVALID_PARAMETERS";
        case(D3D12_MESSAGE_ID_GETGPUVIRTUALADDRESS_INVALID_RESOURCE_DIMENSION):
            return "D3D12_MESSAGE_ID_GETGPUVIRTUALADDRESS_INVALID_RESOURCE_DIMENSION";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDCLEARVALUE):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDCLEARVALUE";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_UNRECOGNIZEDCLEARVALUEFORMAT):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_UNRECOGNIZEDCLEARVALUEFORMAT";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDCLEARVALUEFORMAT):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_INVALIDCLEARVALUEFORMAT";
        case(D3D12_MESSAGE_ID_CREATERESOURCE_CLEARVALUEDENORMFLUSH):
            return "D3D12_MESSAGE_ID_CREATERESOURCE_CLEARVALUEDENORMFLUSH";
        case(D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE):
            return "D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE";
        case(D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE):
            return "D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE";
        case(D3D12_MESSAGE_ID_MAP_INVALIDHEAP):
            return "D3D12_MESSAGE_ID_MAP_INVALIDHEAP";
        case(D3D12_MESSAGE_ID_UNMAP_INVALIDHEAP):
            return "D3D12_MESSAGE_ID_UNMAP_INVALIDHEAP";
        case(D3D12_MESSAGE_ID_MAP_INVALIDRESOURCE):
            return "D3D12_MESSAGE_ID_MAP_INVALIDRESOURCE";
        case(D3D12_MESSAGE_ID_UNMAP_INVALIDRESOURCE):
            return "D3D12_MESSAGE_ID_UNMAP_INVALIDRESOURCE";
        case(D3D12_MESSAGE_ID_MAP_INVALIDSUBRESOURCE):
            return "D3D12_MESSAGE_ID_MAP_INVALIDSUBRESOURCE";
        case(D3D12_MESSAGE_ID_UNMAP_INVALIDSUBRESOURCE):
            return "D3D12_MESSAGE_ID_UNMAP_INVALIDSUBRESOURCE";
        case(D3D12_MESSAGE_ID_MAP_INVALIDRANGE):
            return "D3D12_MESSAGE_ID_MAP_INVALIDRANGE";
        case(D3D12_MESSAGE_ID_UNMAP_INVALIDRANGE):
            return "D3D12_MESSAGE_ID_UNMAP_INVALIDRANGE";
        case(D3D12_MESSAGE_ID_MAP_INVALIDDATAPOINTER):
            return "D3D12_MESSAGE_ID_MAP_INVALIDDATAPOINTER";
        case(D3D12_MESSAGE_ID_MAP_INVALIDARG_RETURN):
            return "D3D12_MESSAGE_ID_MAP_INVALIDARG_RETURN";
        case(D3D12_MESSAGE_ID_MAP_OUTOFMEMORY_RETURN):
            return "D3D12_MESSAGE_ID_MAP_OUTOFMEMORY_RETURN";
        case(D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_BUNDLENOTSUPPORTED):
            return "D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_BUNDLENOTSUPPORTED";
        case(D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_COMMANDLISTMISMATCH):
            return "D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_COMMANDLISTMISMATCH";
        case(D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_OPENCOMMANDLIST):
            return "D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_OPENCOMMANDLIST";
        case(D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_FAILEDCOMMANDLIST):
            return "D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_FAILEDCOMMANDLIST";
        case(D3D12_MESSAGE_ID_COPYBUFFERREGION_NULLDST):
            return "D3D12_MESSAGE_ID_COPYBUFFERREGION_NULLDST";
        case(D3D12_MESSAGE_ID_COPYBUFFERREGION_INVALIDDSTRESOURCEDIMENSION):
            return "D3D12_MESSAGE_ID_COPYBUFFERREGION_INVALIDDSTRESOURCEDIMENSION";
        case(D3D12_MESSAGE_ID_COPYBUFFERREGION_DSTRANGEOUTOFBOUNDS):
            return "D3D12_MESSAGE_ID_COPYBUFFERREGION_DSTRANGEOUTOFBOUNDS";
        case(D3D12_MESSAGE_ID_COPYBUFFERREGION_NULLSRC):
            return "D3D12_MESSAGE_ID_COPYBUFFERREGION_NULLSRC";
        case(D3D12_MESSAGE_ID_COPYBUFFERREGION_INVALIDSRCRESOURCEDIMENSION):
            return "D3D12_MESSAGE_ID_COPYBUFFERREGION_INVALIDSRCRESOURCEDIMENSION";
        case(D3D12_MESSAGE_ID_COPYBUFFERREGION_SRCRANGEOUTOFBOUNDS):
            return "D3D12_MESSAGE_ID_COPYBUFFERREGION_SRCRANGEOUTOFBOUNDS";
        case(D3D12_MESSAGE_ID_COPYBUFFERREGION_INVALIDCOPYFLAGS):
            return "D3D12_MESSAGE_ID_COPYBUFFERREGION_INVALIDCOPYFLAGS";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_NULLDST):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_NULLDST";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_UNRECOGNIZEDDSTTYPE):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_UNRECOGNIZEDDSTTYPE";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTRESOURCEDIMENSION):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTRESOURCEDIMENSION";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTRESOURCE):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTRESOURCE";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTSUBRESOURCE):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTSUBRESOURCE";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTOFFSET):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTOFFSET";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_UNRECOGNIZEDDSTFORMAT):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_UNRECOGNIZEDDSTFORMAT";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTFORMAT):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTFORMAT";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTDIMENSIONS):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTDIMENSIONS";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTROWPITCH):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTROWPITCH";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTPLACEMENT):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTPLACEMENT";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTDSPLACEDFOOTPRINTFORMAT):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTDSPLACEDFOOTPRINTFORMAT";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_DSTREGIONOUTOFBOUNDS):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_DSTREGIONOUTOFBOUNDS";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_NULLSRC):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_NULLSRC";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_UNRECOGNIZEDSRCTYPE):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_UNRECOGNIZEDSRCTYPE";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCRESOURCEDIMENSION):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCRESOURCEDIMENSION";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCRESOURCE):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCRESOURCE";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCSUBRESOURCE):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCSUBRESOURCE";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCOFFSET):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCOFFSET";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_UNRECOGNIZEDSRCFORMAT):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_UNRECOGNIZEDSRCFORMAT";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCFORMAT):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCFORMAT";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCDIMENSIONS):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCDIMENSIONS";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCROWPITCH):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCROWPITCH";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCPLACEMENT):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCPLACEMENT";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCDSPLACEDFOOTPRINTFORMAT):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCDSPLACEDFOOTPRINTFORMAT";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_SRCREGIONOUTOFBOUNDS):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_SRCREGIONOUTOFBOUNDS";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTCOORDINATES):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDDSTCOORDINATES";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCBOX):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDSRCBOX";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_FORMATMISMATCH):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_FORMATMISMATCH";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_EMPTYBOX):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_EMPTYBOX";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDCOPYFLAGS):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_INVALIDCOPYFLAGS";
        case(D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_INVALID_SUBRESOURCE_INDEX):
            return "D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_INVALID_SUBRESOURCE_INDEX";
        case(D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_INVALID_FORMAT):
            return "D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_INVALID_FORMAT";
        case(D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_RESOURCE_MISMATCH):
            return "D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_RESOURCE_MISMATCH";
        case(D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_INVALID_SAMPLE_COUNT):
            return "D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_INVALID_SAMPLE_COUNT";
        case(D3D12_MESSAGE_ID_CREATECOMPUTEPIPELINESTATE_INVALID_SHADER):
            return "D3D12_MESSAGE_ID_CREATECOMPUTEPIPELINESTATE_INVALID_SHADER";
        case(D3D12_MESSAGE_ID_CREATECOMPUTEPIPELINESTATE_CS_ROOT_SIGNATURE_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATECOMPUTEPIPELINESTATE_CS_ROOT_SIGNATURE_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATECOMPUTEPIPELINESTATE_MISSING_ROOT_SIGNATURE):
            return "D3D12_MESSAGE_ID_CREATECOMPUTEPIPELINESTATE_MISSING_ROOT_SIGNATURE";
        case(D3D12_MESSAGE_ID_CREATEPIPELINESTATE_INVALIDCACHEDBLOB):
            return "D3D12_MESSAGE_ID_CREATEPIPELINESTATE_INVALIDCACHEDBLOB";
        case(D3D12_MESSAGE_ID_CREATEPIPELINESTATE_CACHEDBLOBADAPTERMISMATCH):
            return "D3D12_MESSAGE_ID_CREATEPIPELINESTATE_CACHEDBLOBADAPTERMISMATCH";
        case(D3D12_MESSAGE_ID_CREATEPIPELINESTATE_CACHEDBLOBDRIVERVERSIONMISMATCH):
            return "D3D12_MESSAGE_ID_CREATEPIPELINESTATE_CACHEDBLOBDRIVERVERSIONMISMATCH";
        case(D3D12_MESSAGE_ID_CREATEPIPELINESTATE_CACHEDBLOBDESCMISMATCH):
            return "D3D12_MESSAGE_ID_CREATEPIPELINESTATE_CACHEDBLOBDESCMISMATCH";
        case(D3D12_MESSAGE_ID_CREATEPIPELINESTATE_CACHEDBLOBIGNORED):
            return "D3D12_MESSAGE_ID_CREATEPIPELINESTATE_CACHEDBLOBIGNORED";
        case(D3D12_MESSAGE_ID_WRITETOSUBRESOURCE_INVALIDHEAP):
            return "D3D12_MESSAGE_ID_WRITETOSUBRESOURCE_INVALIDHEAP";
        case(D3D12_MESSAGE_ID_WRITETOSUBRESOURCE_INVALIDRESOURCE):
            return "D3D12_MESSAGE_ID_WRITETOSUBRESOURCE_INVALIDRESOURCE";
        case(D3D12_MESSAGE_ID_WRITETOSUBRESOURCE_INVALIDBOX):
            return "D3D12_MESSAGE_ID_WRITETOSUBRESOURCE_INVALIDBOX";
        case(D3D12_MESSAGE_ID_WRITETOSUBRESOURCE_INVALIDSUBRESOURCE):
            return "D3D12_MESSAGE_ID_WRITETOSUBRESOURCE_INVALIDSUBRESOURCE";
        case(D3D12_MESSAGE_ID_WRITETOSUBRESOURCE_EMPTYBOX):
            return "D3D12_MESSAGE_ID_WRITETOSUBRESOURCE_EMPTYBOX";
        case(D3D12_MESSAGE_ID_READFROMSUBRESOURCE_INVALIDHEAP):
            return "D3D12_MESSAGE_ID_READFROMSUBRESOURCE_INVALIDHEAP";
        case(D3D12_MESSAGE_ID_READFROMSUBRESOURCE_INVALIDRESOURCE):
            return "D3D12_MESSAGE_ID_READFROMSUBRESOURCE_INVALIDRESOURCE";
        case(D3D12_MESSAGE_ID_READFROMSUBRESOURCE_INVALIDBOX):
            return "D3D12_MESSAGE_ID_READFROMSUBRESOURCE_INVALIDBOX";
        case(D3D12_MESSAGE_ID_READFROMSUBRESOURCE_INVALIDSUBRESOURCE):
            return "D3D12_MESSAGE_ID_READFROMSUBRESOURCE_INVALIDSUBRESOURCE";
        case(D3D12_MESSAGE_ID_READFROMSUBRESOURCE_EMPTYBOX):
            return "D3D12_MESSAGE_ID_READFROMSUBRESOURCE_EMPTYBOX";
        case(D3D12_MESSAGE_ID_TOO_MANY_NODES_SPECIFIED):
            return "D3D12_MESSAGE_ID_TOO_MANY_NODES_SPECIFIED";
        case(D3D12_MESSAGE_ID_INVALID_NODE_INDEX):
            return "D3D12_MESSAGE_ID_INVALID_NODE_INDEX";
        case(D3D12_MESSAGE_ID_GETHEAPPROPERTIES_INVALIDRESOURCE):
            return "D3D12_MESSAGE_ID_GETHEAPPROPERTIES_INVALIDRESOURCE";
        case(D3D12_MESSAGE_ID_NODE_MASK_MISMATCH):
            return "D3D12_MESSAGE_ID_NODE_MASK_MISMATCH";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_OUTOFMEMORY):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_OUTOFMEMORY";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_MULTIPLE_SWAPCHAIN_BUFFER_REFERENCES):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_MULTIPLE_SWAPCHAIN_BUFFER_REFERENCES";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_TOO_MANY_SWAPCHAIN_REFERENCES):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_TOO_MANY_SWAPCHAIN_REFERENCES";
        case(D3D12_MESSAGE_ID_COMMAND_QUEUE_TOO_MANY_SWAPCHAIN_REFERENCES):
            return "D3D12_MESSAGE_ID_COMMAND_QUEUE_TOO_MANY_SWAPCHAIN_REFERENCES";
        case(D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_WRONGSWAPCHAINBUFFERREFERENCE):
            return "D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_WRONGSWAPCHAINBUFFERREFERENCE";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_SETRENDERTARGETS_INVALIDNUMRENDERTARGETS):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_SETRENDERTARGETS_INVALIDNUMRENDERTARGETS";
        case(D3D12_MESSAGE_ID_CREATE_QUEUE_INVALID_TYPE):
            return "D3D12_MESSAGE_ID_CREATE_QUEUE_INVALID_TYPE";
        case(D3D12_MESSAGE_ID_CREATE_QUEUE_INVALID_FLAGS):
            return "D3D12_MESSAGE_ID_CREATE_QUEUE_INVALID_FLAGS";
        case(D3D12_MESSAGE_ID_CREATESHAREDRESOURCE_INVALIDFLAGS):
            return "D3D12_MESSAGE_ID_CREATESHAREDRESOURCE_INVALIDFLAGS";
        case(D3D12_MESSAGE_ID_CREATESHAREDRESOURCE_INVALIDFORMAT):
            return "D3D12_MESSAGE_ID_CREATESHAREDRESOURCE_INVALIDFORMAT";
        case(D3D12_MESSAGE_ID_CREATESHAREDHEAP_INVALIDFLAGS):
            return "D3D12_MESSAGE_ID_CREATESHAREDHEAP_INVALIDFLAGS";
        case(D3D12_MESSAGE_ID_REFLECTSHAREDPROPERTIES_UNRECOGNIZEDPROPERTIES):
            return "D3D12_MESSAGE_ID_REFLECTSHAREDPROPERTIES_UNRECOGNIZEDPROPERTIES";
        case(D3D12_MESSAGE_ID_REFLECTSHAREDPROPERTIES_INVALIDSIZE):
            return "D3D12_MESSAGE_ID_REFLECTSHAREDPROPERTIES_INVALIDSIZE";
        case(D3D12_MESSAGE_ID_REFLECTSHAREDPROPERTIES_INVALIDOBJECT):
            return "D3D12_MESSAGE_ID_REFLECTSHAREDPROPERTIES_INVALIDOBJECT";
        case(D3D12_MESSAGE_ID_KEYEDMUTEX_INVALIDOBJECT):
            return "D3D12_MESSAGE_ID_KEYEDMUTEX_INVALIDOBJECT";
        case(D3D12_MESSAGE_ID_KEYEDMUTEX_INVALIDKEY):
            return "D3D12_MESSAGE_ID_KEYEDMUTEX_INVALIDKEY";
        case(D3D12_MESSAGE_ID_KEYEDMUTEX_WRONGSTATE):
            return "D3D12_MESSAGE_ID_KEYEDMUTEX_WRONGSTATE";
        case(D3D12_MESSAGE_ID_CREATE_QUEUE_INVALID_PRIORITY):
            return "D3D12_MESSAGE_ID_CREATE_QUEUE_INVALID_PRIORITY";
        case(D3D12_MESSAGE_ID_OBJECT_DELETED_WHILE_STILL_IN_USE):
            return "D3D12_MESSAGE_ID_OBJECT_DELETED_WHILE_STILL_IN_USE";
        case(D3D12_MESSAGE_ID_CREATEPIPELINESTATE_INVALID_FLAGS):
            return "D3D12_MESSAGE_ID_CREATEPIPELINESTATE_INVALID_FLAGS";
        case(D3D12_MESSAGE_ID_HEAP_ADDRESS_RANGE_HAS_NO_RESOURCE):
            return "D3D12_MESSAGE_ID_HEAP_ADDRESS_RANGE_HAS_NO_RESOURCE";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_RENDER_TARGET_DELETED):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DRAW_RENDER_TARGET_DELETED";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_ALL_RENDER_TARGETS_HAVE_UNKNOWN_FORMAT):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_ALL_RENDER_TARGETS_HAVE_UNKNOWN_FORMAT";
        case(D3D12_MESSAGE_ID_HEAP_ADDRESS_RANGE_INTERSECTS_MULTIPLE_BUFFERS):
            return "D3D12_MESSAGE_ID_HEAP_ADDRESS_RANGE_INTERSECTS_MULTIPLE_BUFFERS";
        case(D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_GPU_WRITTEN_READBACK_RESOURCE_MAPPED):
            return "D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_GPU_WRITTEN_READBACK_RESOURCE_MAPPED";
        case(D3D12_MESSAGE_ID_UNMAP_RANGE_NOT_EMPTY):
            return "D3D12_MESSAGE_ID_UNMAP_RANGE_NOT_EMPTY";
        case(D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE):
            return "D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE";
        case(D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE):
            return "D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE";
        case(D3D12_MESSAGE_ID_NO_GRAPHICS_API_SUPPORT):
            return "D3D12_MESSAGE_ID_NO_GRAPHICS_API_SUPPORT";
        case(D3D12_MESSAGE_ID_NO_COMPUTE_API_SUPPORT):
            return "D3D12_MESSAGE_ID_NO_COMPUTE_API_SUPPORT";
        case(D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_RESOURCE_FLAGS_NOT_SUPPORTED):
            return "D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_RESOURCE_FLAGS_NOT_SUPPORTED";
        case(D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_ROOT_ARGUMENT_UNINITIALIZED):
            return "D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_ROOT_ARGUMENT_UNINITIALIZED";
        case(D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_DESCRIPTOR_HEAP_INDEX_OUT_OF_BOUNDS):
            return "D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_DESCRIPTOR_HEAP_INDEX_OUT_OF_BOUNDS";
        case(D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_DESCRIPTOR_TABLE_REGISTER_INDEX_OUT_OF_BOUNDS):
            return "D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_DESCRIPTOR_TABLE_REGISTER_INDEX_OUT_OF_BOUNDS";
        case(D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_DESCRIPTOR_UNINITIALIZED):
            return "D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_DESCRIPTOR_UNINITIALIZED";
        case(D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_DESCRIPTOR_TYPE_MISMATCH):
            return "D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_DESCRIPTOR_TYPE_MISMATCH";
        case(D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_SRV_RESOURCE_DIMENSION_MISMATCH):
            return "D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_SRV_RESOURCE_DIMENSION_MISMATCH";
        case(D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_UAV_RESOURCE_DIMENSION_MISMATCH):
            return "D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_UAV_RESOURCE_DIMENSION_MISMATCH";
        case(D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_INCOMPATIBLE_RESOURCE_STATE):
            return "D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_INCOMPATIBLE_RESOURCE_STATE";
        case(D3D12_MESSAGE_ID_COPYRESOURCE_NULLDST):
            return "D3D12_MESSAGE_ID_COPYRESOURCE_NULLDST";
        case(D3D12_MESSAGE_ID_COPYRESOURCE_INVALIDDSTRESOURCE):
            return "D3D12_MESSAGE_ID_COPYRESOURCE_INVALIDDSTRESOURCE";
        case(D3D12_MESSAGE_ID_COPYRESOURCE_NULLSRC):
            return "D3D12_MESSAGE_ID_COPYRESOURCE_NULLSRC";
        case(D3D12_MESSAGE_ID_COPYRESOURCE_INVALIDSRCRESOURCE):
            return "D3D12_MESSAGE_ID_COPYRESOURCE_INVALIDSRCRESOURCE";
        case(D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_NULLDST):
            return "D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_NULLDST";
        case(D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_INVALIDDSTRESOURCE):
            return "D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_INVALIDDSTRESOURCE";
        case(D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_NULLSRC):
            return "D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_NULLSRC";
        case(D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_INVALIDSRCRESOURCE):
            return "D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_INVALIDSRCRESOURCE";
        case(D3D12_MESSAGE_ID_PIPELINE_STATE_TYPE_MISMATCH):
            return "D3D12_MESSAGE_ID_PIPELINE_STATE_TYPE_MISMATCH";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DISPATCH_ROOT_SIGNATURE_NOT_SET):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DISPATCH_ROOT_SIGNATURE_NOT_SET";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DISPATCH_ROOT_SIGNATURE_MISMATCH):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DISPATCH_ROOT_SIGNATURE_MISMATCH";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_ZERO_BARRIERS):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_ZERO_BARRIERS";
        case(D3D12_MESSAGE_ID_BEGIN_END_EVENT_MISMATCH):
            return "D3D12_MESSAGE_ID_BEGIN_END_EVENT_MISMATCH";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_POSSIBLE_BEFORE_AFTER_MISMATCH):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_POSSIBLE_BEFORE_AFTER_MISMATCH";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_BEGIN_END):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_BEGIN_END";
        case(D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_INVALID_RESOURCE):
            return "D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_INVALID_RESOURCE";
        case(D3D12_MESSAGE_ID_USE_OF_ZERO_REFCOUNT_OBJECT):
            return "D3D12_MESSAGE_ID_USE_OF_ZERO_REFCOUNT_OBJECT";
        case(D3D12_MESSAGE_ID_OBJECT_EVICTED_WHILE_STILL_IN_USE):
            return "D3D12_MESSAGE_ID_OBJECT_EVICTED_WHILE_STILL_IN_USE";
        case(D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_ROOT_DESCRIPTOR_ACCESS_OUT_OF_BOUNDS):
            return "D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_ROOT_DESCRIPTOR_ACCESS_OUT_OF_BOUNDS";
        case(D3D12_MESSAGE_ID_CREATEPIPELINELIBRARY_INVALIDLIBRARYBLOB):
            return "D3D12_MESSAGE_ID_CREATEPIPELINELIBRARY_INVALIDLIBRARYBLOB";
        case(D3D12_MESSAGE_ID_CREATEPIPELINELIBRARY_DRIVERVERSIONMISMATCH):
            return "D3D12_MESSAGE_ID_CREATEPIPELINELIBRARY_DRIVERVERSIONMISMATCH";
        case(D3D12_MESSAGE_ID_CREATEPIPELINELIBRARY_ADAPTERVERSIONMISMATCH):
            return "D3D12_MESSAGE_ID_CREATEPIPELINELIBRARY_ADAPTERVERSIONMISMATCH";
        case(D3D12_MESSAGE_ID_CREATEPIPELINELIBRARY_UNSUPPORTED):
            return "D3D12_MESSAGE_ID_CREATEPIPELINELIBRARY_UNSUPPORTED";
        case(D3D12_MESSAGE_ID_CREATE_PIPELINELIBRARY):
            return "D3D12_MESSAGE_ID_CREATE_PIPELINELIBRARY";
        case(D3D12_MESSAGE_ID_LIVE_PIPELINELIBRARY):
            return "D3D12_MESSAGE_ID_LIVE_PIPELINELIBRARY";
        case(D3D12_MESSAGE_ID_DESTROY_PIPELINELIBRARY):
            return "D3D12_MESSAGE_ID_DESTROY_PIPELINELIBRARY";
        case(D3D12_MESSAGE_ID_STOREPIPELINE_NONAME):
            return "D3D12_MESSAGE_ID_STOREPIPELINE_NONAME";
        case(D3D12_MESSAGE_ID_STOREPIPELINE_DUPLICATENAME):
            return "D3D12_MESSAGE_ID_STOREPIPELINE_DUPLICATENAME";
        case(D3D12_MESSAGE_ID_LOADPIPELINE_NAMENOTFOUND):
            return "D3D12_MESSAGE_ID_LOADPIPELINE_NAMENOTFOUND";
        case(D3D12_MESSAGE_ID_LOADPIPELINE_INVALIDDESC):
            return "D3D12_MESSAGE_ID_LOADPIPELINE_INVALIDDESC";
        case(D3D12_MESSAGE_ID_PIPELINELIBRARY_SERIALIZE_NOTENOUGHMEMORY):
            return "D3D12_MESSAGE_ID_PIPELINELIBRARY_SERIALIZE_NOTENOUGHMEMORY";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_PS_OUTPUT_RT_OUTPUT_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_PS_OUTPUT_RT_OUTPUT_MISMATCH";
        case(D3D12_MESSAGE_ID_SETEVENTONMULTIPLEFENCECOMPLETION_INVALIDFLAGS):
            return "D3D12_MESSAGE_ID_SETEVENTONMULTIPLEFENCECOMPLETION_INVALIDFLAGS";
        case(D3D12_MESSAGE_ID_CREATE_QUEUE_VIDEO_NOT_SUPPORTED):
            return "D3D12_MESSAGE_ID_CREATE_QUEUE_VIDEO_NOT_SUPPORTED";
        case(D3D12_MESSAGE_ID_CREATE_COMMAND_ALLOCATOR_VIDEO_NOT_SUPPORTED):
            return "D3D12_MESSAGE_ID_CREATE_COMMAND_ALLOCATOR_VIDEO_NOT_SUPPORTED";
        case(D3D12_MESSAGE_ID_CREATEQUERY_HEAP_VIDEO_DECODE_STATISTICS_NOT_SUPPORTED):
            return "D3D12_MESSAGE_ID_CREATEQUERY_HEAP_VIDEO_DECODE_STATISTICS_NOT_SUPPORTED";
        case(D3D12_MESSAGE_ID_CREATE_VIDEODECODECOMMANDLIST):
            return "D3D12_MESSAGE_ID_CREATE_VIDEODECODECOMMANDLIST";
        case(D3D12_MESSAGE_ID_CREATE_VIDEODECODER):
            return "D3D12_MESSAGE_ID_CREATE_VIDEODECODER";
        case(D3D12_MESSAGE_ID_CREATE_VIDEODECODESTREAM):
            return "D3D12_MESSAGE_ID_CREATE_VIDEODECODESTREAM";
        case(D3D12_MESSAGE_ID_LIVE_VIDEODECODECOMMANDLIST):
            return "D3D12_MESSAGE_ID_LIVE_VIDEODECODECOMMANDLIST";
        case(D3D12_MESSAGE_ID_LIVE_VIDEODECODER):
            return "D3D12_MESSAGE_ID_LIVE_VIDEODECODER";
        case(D3D12_MESSAGE_ID_LIVE_VIDEODECODESTREAM):
            return "D3D12_MESSAGE_ID_LIVE_VIDEODECODESTREAM";
        case(D3D12_MESSAGE_ID_DESTROY_VIDEODECODECOMMANDLIST):
            return "D3D12_MESSAGE_ID_DESTROY_VIDEODECODECOMMANDLIST";
        case(D3D12_MESSAGE_ID_DESTROY_VIDEODECODER):
            return "D3D12_MESSAGE_ID_DESTROY_VIDEODECODER";
        case(D3D12_MESSAGE_ID_DESTROY_VIDEODECODESTREAM):
            return "D3D12_MESSAGE_ID_DESTROY_VIDEODECODESTREAM";
        case(D3D12_MESSAGE_ID_DECODE_FRAME_INVALID_PARAMETERS):
            return "D3D12_MESSAGE_ID_DECODE_FRAME_INVALID_PARAMETERS";
        case(D3D12_MESSAGE_ID_DEPRECATED_API):
            return "D3D12_MESSAGE_ID_DEPRECATED_API";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_DESCRIPTOR_TABLE_NOT_SET):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_DESCRIPTOR_TABLE_NOT_SET";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_ROOT_CONSTANT_BUFFER_VIEW_NOT_SET):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_ROOT_CONSTANT_BUFFER_VIEW_NOT_SET";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_ROOT_SHADER_RESOURCE_VIEW_NOT_SET):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_ROOT_SHADER_RESOURCE_VIEW_NOT_SET";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_ROOT_UNORDERED_ACCESS_VIEW_NOT_SET):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_ROOT_UNORDERED_ACCESS_VIEW_NOT_SET";
        case(D3D12_MESSAGE_ID_DISCARD_INVALID_SUBRESOURCE_RANGE):
            return "D3D12_MESSAGE_ID_DISCARD_INVALID_SUBRESOURCE_RANGE";
        case(D3D12_MESSAGE_ID_DISCARD_ONE_SUBRESOURCE_FOR_MIPS_WITH_RECTS):
            return "D3D12_MESSAGE_ID_DISCARD_ONE_SUBRESOURCE_FOR_MIPS_WITH_RECTS";
        case(D3D12_MESSAGE_ID_DISCARD_NO_RECTS_FOR_NON_TEXTURE2D):
            return "D3D12_MESSAGE_ID_DISCARD_NO_RECTS_FOR_NON_TEXTURE2D";
        case(D3D12_MESSAGE_ID_COPY_ON_SAME_SUBRESOURCE):
            return "D3D12_MESSAGE_ID_COPY_ON_SAME_SUBRESOURCE";
        case(D3D12_MESSAGE_ID_SETRESIDENCYPRIORITY_INVALID_PAGEABLE):
            return "D3D12_MESSAGE_ID_SETRESIDENCYPRIORITY_INVALID_PAGEABLE";
        case(D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_UNSUPPORTED):
            return "D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_UNSUPPORTED";
        case(D3D12_MESSAGE_ID_STATIC_DESCRIPTOR_INVALID_DESCRIPTOR_CHANGE):
            return "D3D12_MESSAGE_ID_STATIC_DESCRIPTOR_INVALID_DESCRIPTOR_CHANGE";
        case(D3D12_MESSAGE_ID_DATA_STATIC_DESCRIPTOR_INVALID_DATA_CHANGE):
            return "D3D12_MESSAGE_ID_DATA_STATIC_DESCRIPTOR_INVALID_DATA_CHANGE";
        case(D3D12_MESSAGE_ID_DATA_STATIC_WHILE_SET_AT_EXECUTE_DESCRIPTOR_INVALID_DATA_CHANGE):
            return "D3D12_MESSAGE_ID_DATA_STATIC_WHILE_SET_AT_EXECUTE_DESCRIPTOR_INVALID_DATA_CHANGE";
        case(D3D12_MESSAGE_ID_EXECUTE_BUNDLE_STATIC_DESCRIPTOR_DATA_STATIC_NOT_SET):
            return "D3D12_MESSAGE_ID_EXECUTE_BUNDLE_STATIC_DESCRIPTOR_DATA_STATIC_NOT_SET";
        case(D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_RESOURCE_ACCESS_OUT_OF_BOUNDS):
            return "D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_RESOURCE_ACCESS_OUT_OF_BOUNDS";
        case(D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_SAMPLER_MODE_MISMATCH):
            return "D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_SAMPLER_MODE_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATE_FENCE_INVALID_FLAGS):
            return "D3D12_MESSAGE_ID_CREATE_FENCE_INVALID_FLAGS";
        case(D3D12_MESSAGE_ID_RESOURCE_BARRIER_DUPLICATE_SUBRESOURCE_TRANSITIONS):
            return "D3D12_MESSAGE_ID_RESOURCE_BARRIER_DUPLICATE_SUBRESOURCE_TRANSITIONS";
        case(D3D12_MESSAGE_ID_SETRESIDENCYPRIORITY_INVALID_PRIORITY):
            return "D3D12_MESSAGE_ID_SETRESIDENCYPRIORITY_INVALID_PRIORITY";
        case(D3D12_MESSAGE_ID_CREATE_DESCRIPTOR_HEAP_LARGE_NUM_DESCRIPTORS):
            return "D3D12_MESSAGE_ID_CREATE_DESCRIPTOR_HEAP_LARGE_NUM_DESCRIPTORS";
        case(D3D12_MESSAGE_ID_BEGIN_EVENT):
            return "D3D12_MESSAGE_ID_BEGIN_EVENT";
        case(D3D12_MESSAGE_ID_END_EVENT):
            return "D3D12_MESSAGE_ID_END_EVENT";
        case(D3D12_MESSAGE_ID_CREATEDEVICE_DEBUG_LAYER_STARTUP_OPTIONS):
            return "D3D12_MESSAGE_ID_CREATEDEVICE_DEBUG_LAYER_STARTUP_OPTIONS";
        case(D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_DEPTHBOUNDSTEST_UNSUPPORTED):
            return "D3D12_MESSAGE_ID_CREATEDEPTHSTENCILSTATE_DEPTHBOUNDSTEST_UNSUPPORTED";
        case(D3D12_MESSAGE_ID_CREATEPIPELINESTATE_DUPLICATE_SUBOBJECT):
            return "D3D12_MESSAGE_ID_CREATEPIPELINESTATE_DUPLICATE_SUBOBJECT";
        case(D3D12_MESSAGE_ID_CREATEPIPELINESTATE_UNKNOWN_SUBOBJECT):
            return "D3D12_MESSAGE_ID_CREATEPIPELINESTATE_UNKNOWN_SUBOBJECT";
        case(D3D12_MESSAGE_ID_CREATEPIPELINESTATE_ZERO_SIZE_STREAM):
            return "D3D12_MESSAGE_ID_CREATEPIPELINESTATE_ZERO_SIZE_STREAM";
        case(D3D12_MESSAGE_ID_CREATEPIPELINESTATE_INVALID_STREAM):
            return "D3D12_MESSAGE_ID_CREATEPIPELINESTATE_INVALID_STREAM";
        case(D3D12_MESSAGE_ID_CREATEPIPELINESTATE_CANNOT_DEDUCE_TYPE):
            return "D3D12_MESSAGE_ID_CREATEPIPELINESTATE_CANNOT_DEDUCE_TYPE";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_STATIC_DESCRIPTOR_RESOURCE_DIMENSION_MISMATCH):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_STATIC_DESCRIPTOR_RESOURCE_DIMENSION_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATE_COMMAND_QUEUE_INSUFFICIENT_PRIVILEGE_FOR_GLOBAL_REALTIME):
            return "D3D12_MESSAGE_ID_CREATE_COMMAND_QUEUE_INSUFFICIENT_PRIVILEGE_FOR_GLOBAL_REALTIME";
        case(D3D12_MESSAGE_ID_CREATE_COMMAND_QUEUE_INSUFFICIENT_HARDWARE_SUPPORT_FOR_GLOBAL_REALTIME):
            return "D3D12_MESSAGE_ID_CREATE_COMMAND_QUEUE_INSUFFICIENT_HARDWARE_SUPPORT_FOR_GLOBAL_REALTIME";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_ARCHITECTURE):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_ARCHITECTURE";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_NULL_DST):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_NULL_DST";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_DST_RESOURCE_DIMENSION):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_DST_RESOURCE_DIMENSION";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_DST_RANGE_OUT_OF_BOUNDS):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_DST_RANGE_OUT_OF_BOUNDS";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_NULL_SRC):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_NULL_SRC";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_SRC_RESOURCE_DIMENSION):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_SRC_RESOURCE_DIMENSION";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_SRC_RANGE_OUT_OF_BOUNDS):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_SRC_RANGE_OUT_OF_BOUNDS";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_OFFSET_ALIGNMENT):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_OFFSET_ALIGNMENT";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_NULL_DEPENDENT_RESOURCES):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_NULL_DEPENDENT_RESOURCES";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_NULL_DEPENDENT_SUBRESOURCE_RANGES):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_NULL_DEPENDENT_SUBRESOURCE_RANGES";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_DEPENDENT_RESOURCE):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_DEPENDENT_RESOURCE";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_DEPENDENT_SUBRESOURCE_RANGE):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_DEPENDENT_SUBRESOURCE_RANGE";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_DEPENDENT_SUBRESOURCE_OUT_OF_BOUNDS):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_DEPENDENT_SUBRESOURCE_OUT_OF_BOUNDS";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_DEPENDENT_RANGE_OUT_OF_BOUNDS):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_DEPENDENT_RANGE_OUT_OF_BOUNDS";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_ZERO_DEPENDENCIES):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_ZERO_DEPENDENCIES";
        case(D3D12_MESSAGE_ID_DEVICE_CREATE_SHARED_HANDLE_INVALIDARG):
            return "D3D12_MESSAGE_ID_DEVICE_CREATE_SHARED_HANDLE_INVALIDARG";
        case(D3D12_MESSAGE_ID_DESCRIPTOR_HANDLE_WITH_INVALID_RESOURCE):
            return "D3D12_MESSAGE_ID_DESCRIPTOR_HANDLE_WITH_INVALID_RESOURCE";
        case(D3D12_MESSAGE_ID_SETDEPTHBOUNDS_INVALIDARGS):
            return "D3D12_MESSAGE_ID_SETDEPTHBOUNDS_INVALIDARGS";
        case(D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_RESOURCE_STATE_IMPRECISE):
            return "D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_RESOURCE_STATE_IMPRECISE";
        case(D3D12_MESSAGE_ID_COMMAND_LIST_PIPELINE_STATE_NOT_SET):
            return "D3D12_MESSAGE_ID_COMMAND_LIST_PIPELINE_STATE_NOT_SET";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_MODEL_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_SHADER_MODEL_MISMATCH";
        case(D3D12_MESSAGE_ID_OBJECT_ACCESSED_WHILE_STILL_IN_USE):
            return "D3D12_MESSAGE_ID_OBJECT_ACCESSED_WHILE_STILL_IN_USE";
        case(D3D12_MESSAGE_ID_PROGRAMMABLE_MSAA_UNSUPPORTED):
            return "D3D12_MESSAGE_ID_PROGRAMMABLE_MSAA_UNSUPPORTED";
        case(D3D12_MESSAGE_ID_SETSAMPLEPOSITIONS_INVALIDARGS):
            return "D3D12_MESSAGE_ID_SETSAMPLEPOSITIONS_INVALIDARGS";
        case(D3D12_MESSAGE_ID_RESOLVESUBRESOURCEREGION_INVALID_RECT):
            return "D3D12_MESSAGE_ID_RESOLVESUBRESOURCEREGION_INVALID_RECT";
        case(D3D12_MESSAGE_ID_CREATE_VIDEODECODECOMMANDQUEUE):
            return "D3D12_MESSAGE_ID_CREATE_VIDEODECODECOMMANDQUEUE";
        case(D3D12_MESSAGE_ID_CREATE_VIDEOPROCESSCOMMANDLIST):
            return "D3D12_MESSAGE_ID_CREATE_VIDEOPROCESSCOMMANDLIST";
        case(D3D12_MESSAGE_ID_CREATE_VIDEOPROCESSCOMMANDQUEUE):
            return "D3D12_MESSAGE_ID_CREATE_VIDEOPROCESSCOMMANDQUEUE";
        case(D3D12_MESSAGE_ID_LIVE_VIDEODECODECOMMANDQUEUE):
            return "D3D12_MESSAGE_ID_LIVE_VIDEODECODECOMMANDQUEUE";
        case(D3D12_MESSAGE_ID_LIVE_VIDEOPROCESSCOMMANDLIST):
            return "D3D12_MESSAGE_ID_LIVE_VIDEOPROCESSCOMMANDLIST";
        case(D3D12_MESSAGE_ID_LIVE_VIDEOPROCESSCOMMANDQUEUE):
            return "D3D12_MESSAGE_ID_LIVE_VIDEOPROCESSCOMMANDQUEUE";
        case(D3D12_MESSAGE_ID_DESTROY_VIDEODECODECOMMANDQUEUE):
            return "D3D12_MESSAGE_ID_DESTROY_VIDEODECODECOMMANDQUEUE";
        case(D3D12_MESSAGE_ID_DESTROY_VIDEOPROCESSCOMMANDLIST):
            return "D3D12_MESSAGE_ID_DESTROY_VIDEOPROCESSCOMMANDLIST";
        case(D3D12_MESSAGE_ID_DESTROY_VIDEOPROCESSCOMMANDQUEUE):
            return "D3D12_MESSAGE_ID_DESTROY_VIDEOPROCESSCOMMANDQUEUE";
        case(D3D12_MESSAGE_ID_CREATE_VIDEOPROCESSOR):
            return "D3D12_MESSAGE_ID_CREATE_VIDEOPROCESSOR";
        case(D3D12_MESSAGE_ID_CREATE_VIDEOPROCESSSTREAM):
            return "D3D12_MESSAGE_ID_CREATE_VIDEOPROCESSSTREAM";
        case(D3D12_MESSAGE_ID_LIVE_VIDEOPROCESSOR):
            return "D3D12_MESSAGE_ID_LIVE_VIDEOPROCESSOR";
        case(D3D12_MESSAGE_ID_LIVE_VIDEOPROCESSSTREAM):
            return "D3D12_MESSAGE_ID_LIVE_VIDEOPROCESSSTREAM";
        case(D3D12_MESSAGE_ID_DESTROY_VIDEOPROCESSOR):
            return "D3D12_MESSAGE_ID_DESTROY_VIDEOPROCESSOR";
        case(D3D12_MESSAGE_ID_DESTROY_VIDEOPROCESSSTREAM):
            return "D3D12_MESSAGE_ID_DESTROY_VIDEOPROCESSSTREAM";
        case(D3D12_MESSAGE_ID_PROCESS_FRAME_INVALID_PARAMETERS):
            return "D3D12_MESSAGE_ID_PROCESS_FRAME_INVALID_PARAMETERS";
        case(D3D12_MESSAGE_ID_COPY_INVALIDLAYOUT):
            return "D3D12_MESSAGE_ID_COPY_INVALIDLAYOUT";
        case(D3D12_MESSAGE_ID_CREATE_CRYPTO_SESSION):
            return "D3D12_MESSAGE_ID_CREATE_CRYPTO_SESSION";
        case(D3D12_MESSAGE_ID_CREATE_CRYPTO_SESSION_POLICY):
            return "D3D12_MESSAGE_ID_CREATE_CRYPTO_SESSION_POLICY";
        case(D3D12_MESSAGE_ID_CREATE_PROTECTED_RESOURCE_SESSION):
            return "D3D12_MESSAGE_ID_CREATE_PROTECTED_RESOURCE_SESSION";
        case(D3D12_MESSAGE_ID_LIVE_CRYPTO_SESSION):
            return "D3D12_MESSAGE_ID_LIVE_CRYPTO_SESSION";
        case(D3D12_MESSAGE_ID_LIVE_CRYPTO_SESSION_POLICY):
            return "D3D12_MESSAGE_ID_LIVE_CRYPTO_SESSION_POLICY";
        case(D3D12_MESSAGE_ID_LIVE_PROTECTED_RESOURCE_SESSION):
            return "D3D12_MESSAGE_ID_LIVE_PROTECTED_RESOURCE_SESSION";
        case(D3D12_MESSAGE_ID_DESTROY_CRYPTO_SESSION):
            return "D3D12_MESSAGE_ID_DESTROY_CRYPTO_SESSION";
        case(D3D12_MESSAGE_ID_DESTROY_CRYPTO_SESSION_POLICY):
            return "D3D12_MESSAGE_ID_DESTROY_CRYPTO_SESSION_POLICY";
        case(D3D12_MESSAGE_ID_DESTROY_PROTECTED_RESOURCE_SESSION):
            return "D3D12_MESSAGE_ID_DESTROY_PROTECTED_RESOURCE_SESSION";
        case(D3D12_MESSAGE_ID_PROTECTED_RESOURCE_SESSION_UNSUPPORTED):
            return "D3D12_MESSAGE_ID_PROTECTED_RESOURCE_SESSION_UNSUPPORTED";
        case(D3D12_MESSAGE_ID_FENCE_INVALIDOPERATION):
            return "D3D12_MESSAGE_ID_FENCE_INVALIDOPERATION";
        case(D3D12_MESSAGE_ID_CREATEQUERY_HEAP_COPY_QUEUE_TIMESTAMPS_NOT_SUPPORTED):
            return "D3D12_MESSAGE_ID_CREATEQUERY_HEAP_COPY_QUEUE_TIMESTAMPS_NOT_SUPPORTED";
        case(D3D12_MESSAGE_ID_SAMPLEPOSITIONS_MISMATCH_DEFERRED):
            return "D3D12_MESSAGE_ID_SAMPLEPOSITIONS_MISMATCH_DEFERRED";
        case(D3D12_MESSAGE_ID_SAMPLEPOSITIONS_MISMATCH_RECORDTIME_ASSUMEDFROMFIRSTUSE):
            return "D3D12_MESSAGE_ID_SAMPLEPOSITIONS_MISMATCH_RECORDTIME_ASSUMEDFROMFIRSTUSE";
        case(D3D12_MESSAGE_ID_SAMPLEPOSITIONS_MISMATCH_RECORDTIME_ASSUMEDFROMCLEAR):
            return "D3D12_MESSAGE_ID_SAMPLEPOSITIONS_MISMATCH_RECORDTIME_ASSUMEDFROMCLEAR";
        case(D3D12_MESSAGE_ID_CREATE_VIDEODECODERHEAP):
            return "D3D12_MESSAGE_ID_CREATE_VIDEODECODERHEAP";
        case(D3D12_MESSAGE_ID_LIVE_VIDEODECODERHEAP):
            return "D3D12_MESSAGE_ID_LIVE_VIDEODECODERHEAP";
        case(D3D12_MESSAGE_ID_DESTROY_VIDEODECODERHEAP):
            return "D3D12_MESSAGE_ID_DESTROY_VIDEODECODERHEAP";
        case(D3D12_MESSAGE_ID_OPENEXISTINGHEAP_INVALIDARG_RETURN):
            return "D3D12_MESSAGE_ID_OPENEXISTINGHEAP_INVALIDARG_RETURN";
        case(D3D12_MESSAGE_ID_OPENEXISTINGHEAP_OUTOFMEMORY_RETURN):
            return "D3D12_MESSAGE_ID_OPENEXISTINGHEAP_OUTOFMEMORY_RETURN";
        case(D3D12_MESSAGE_ID_OPENEXISTINGHEAP_INVALIDADDRESS):
            return "D3D12_MESSAGE_ID_OPENEXISTINGHEAP_INVALIDADDRESS";
        case(D3D12_MESSAGE_ID_OPENEXISTINGHEAP_INVALIDHANDLE):
            return "D3D12_MESSAGE_ID_OPENEXISTINGHEAP_INVALIDHANDLE";
        case(D3D12_MESSAGE_ID_WRITEBUFFERIMMEDIATE_INVALID_DEST):
            return "D3D12_MESSAGE_ID_WRITEBUFFERIMMEDIATE_INVALID_DEST";
        case(D3D12_MESSAGE_ID_WRITEBUFFERIMMEDIATE_INVALID_MODE):
            return "D3D12_MESSAGE_ID_WRITEBUFFERIMMEDIATE_INVALID_MODE";
        case(D3D12_MESSAGE_ID_WRITEBUFFERIMMEDIATE_INVALID_ALIGNMENT):
            return "D3D12_MESSAGE_ID_WRITEBUFFERIMMEDIATE_INVALID_ALIGNMENT";
        case(D3D12_MESSAGE_ID_WRITEBUFFERIMMEDIATE_NOT_SUPPORTED):
            return "D3D12_MESSAGE_ID_WRITEBUFFERIMMEDIATE_NOT_SUPPORTED";
        case(D3D12_MESSAGE_ID_SETVIEWINSTANCEMASK_INVALIDARGS):
            return "D3D12_MESSAGE_ID_SETVIEWINSTANCEMASK_INVALIDARGS";
        case(D3D12_MESSAGE_ID_VIEW_INSTANCING_UNSUPPORTED):
            return "D3D12_MESSAGE_ID_VIEW_INSTANCING_UNSUPPORTED";
        case(D3D12_MESSAGE_ID_VIEW_INSTANCING_INVALIDARGS):
            return "D3D12_MESSAGE_ID_VIEW_INSTANCING_INVALIDARGS";
        case(D3D12_MESSAGE_ID_COPYTEXTUREREGION_MISMATCH_DECODE_REFERENCE_ONLY_FLAG):
            return "D3D12_MESSAGE_ID_COPYTEXTUREREGION_MISMATCH_DECODE_REFERENCE_ONLY_FLAG";
        case(D3D12_MESSAGE_ID_COPYRESOURCE_MISMATCH_DECODE_REFERENCE_ONLY_FLAG):
            return "D3D12_MESSAGE_ID_COPYRESOURCE_MISMATCH_DECODE_REFERENCE_ONLY_FLAG";
        case(D3D12_MESSAGE_ID_CREATE_VIDEO_DECODE_HEAP_CAPS_FAILURE):
            return "D3D12_MESSAGE_ID_CREATE_VIDEO_DECODE_HEAP_CAPS_FAILURE";
        case(D3D12_MESSAGE_ID_CREATE_VIDEO_DECODE_HEAP_CAPS_UNSUPPORTED):
            return "D3D12_MESSAGE_ID_CREATE_VIDEO_DECODE_HEAP_CAPS_UNSUPPORTED";
        case(D3D12_MESSAGE_ID_VIDEO_DECODE_SUPPORT_INVALID_INPUT):
            return "D3D12_MESSAGE_ID_VIDEO_DECODE_SUPPORT_INVALID_INPUT";
        case(D3D12_MESSAGE_ID_CREATE_VIDEO_DECODER_UNSUPPORTED):
            return "D3D12_MESSAGE_ID_CREATE_VIDEO_DECODER_UNSUPPORTED";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_METADATA_ERROR):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_METADATA_ERROR";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_VIEW_INSTANCING_VERTEX_SIZE_EXCEEDED):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_VIEW_INSTANCING_VERTEX_SIZE_EXCEEDED";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_RUNTIME_INTERNAL_ERROR):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_RUNTIME_INTERNAL_ERROR";
        case(D3D12_MESSAGE_ID_NO_VIDEO_API_SUPPORT):
            return "D3D12_MESSAGE_ID_NO_VIDEO_API_SUPPORT";
        case(D3D12_MESSAGE_ID_VIDEO_PROCESS_SUPPORT_INVALID_INPUT):
            return "D3D12_MESSAGE_ID_VIDEO_PROCESS_SUPPORT_INVALID_INPUT";
        case(D3D12_MESSAGE_ID_CREATE_VIDEO_PROCESSOR_CAPS_FAILURE):
            return "D3D12_MESSAGE_ID_CREATE_VIDEO_PROCESSOR_CAPS_FAILURE";
        case(D3D12_MESSAGE_ID_VIDEO_PROCESS_SUPPORT_UNSUPPORTED_FORMAT):
            return "D3D12_MESSAGE_ID_VIDEO_PROCESS_SUPPORT_UNSUPPORTED_FORMAT";
        case(D3D12_MESSAGE_ID_VIDEO_DECODE_FRAME_INVALID_ARGUMENT):
            return "D3D12_MESSAGE_ID_VIDEO_DECODE_FRAME_INVALID_ARGUMENT";
        case(D3D12_MESSAGE_ID_ENQUEUE_MAKE_RESIDENT_INVALID_FLAGS):
            return "D3D12_MESSAGE_ID_ENQUEUE_MAKE_RESIDENT_INVALID_FLAGS";
        case(D3D12_MESSAGE_ID_OPENEXISTINGHEAP_UNSUPPORTED):
            return "D3D12_MESSAGE_ID_OPENEXISTINGHEAP_UNSUPPORTED";
        case(D3D12_MESSAGE_ID_VIDEO_PROCESS_FRAMES_INVALID_ARGUMENT):
            return "D3D12_MESSAGE_ID_VIDEO_PROCESS_FRAMES_INVALID_ARGUMENT";
        case(D3D12_MESSAGE_ID_VIDEO_DECODE_SUPPORT_UNSUPPORTED):
            return "D3D12_MESSAGE_ID_VIDEO_DECODE_SUPPORT_UNSUPPORTED";
        case(D3D12_MESSAGE_ID_CREATE_COMMANDRECORDER):
            return "D3D12_MESSAGE_ID_CREATE_COMMANDRECORDER";
        case(D3D12_MESSAGE_ID_LIVE_COMMANDRECORDER):
            return "D3D12_MESSAGE_ID_LIVE_COMMANDRECORDER";
        case(D3D12_MESSAGE_ID_DESTROY_COMMANDRECORDER):
            return "D3D12_MESSAGE_ID_DESTROY_COMMANDRECORDER";
        case(D3D12_MESSAGE_ID_CREATE_COMMAND_RECORDER_VIDEO_NOT_SUPPORTED):
            return "D3D12_MESSAGE_ID_CREATE_COMMAND_RECORDER_VIDEO_NOT_SUPPORTED";
        case(D3D12_MESSAGE_ID_CREATE_COMMAND_RECORDER_INVALID_SUPPORT_FLAGS):
            return "D3D12_MESSAGE_ID_CREATE_COMMAND_RECORDER_INVALID_SUPPORT_FLAGS";
        case(D3D12_MESSAGE_ID_CREATE_COMMAND_RECORDER_INVALID_FLAGS):
            return "D3D12_MESSAGE_ID_CREATE_COMMAND_RECORDER_INVALID_FLAGS";
        case(D3D12_MESSAGE_ID_CREATE_COMMAND_RECORDER_MORE_RECORDERS_THAN_LOGICAL_PROCESSORS):
            return "D3D12_MESSAGE_ID_CREATE_COMMAND_RECORDER_MORE_RECORDERS_THAN_LOGICAL_PROCESSORS";
        case(D3D12_MESSAGE_ID_CREATE_COMMANDPOOL):
            return "D3D12_MESSAGE_ID_CREATE_COMMANDPOOL";
        case(D3D12_MESSAGE_ID_LIVE_COMMANDPOOL):
            return "D3D12_MESSAGE_ID_LIVE_COMMANDPOOL";
        case(D3D12_MESSAGE_ID_DESTROY_COMMANDPOOL):
            return "D3D12_MESSAGE_ID_DESTROY_COMMANDPOOL";
        case(D3D12_MESSAGE_ID_CREATE_COMMAND_POOL_INVALID_FLAGS):
            return "D3D12_MESSAGE_ID_CREATE_COMMAND_POOL_INVALID_FLAGS";
        case(D3D12_MESSAGE_ID_CREATE_COMMAND_LIST_VIDEO_NOT_SUPPORTED):
            return "D3D12_MESSAGE_ID_CREATE_COMMAND_LIST_VIDEO_NOT_SUPPORTED";
        case(D3D12_MESSAGE_ID_COMMAND_RECORDER_SUPPORT_FLAGS_MISMATCH):
            return "D3D12_MESSAGE_ID_COMMAND_RECORDER_SUPPORT_FLAGS_MISMATCH";
        case(D3D12_MESSAGE_ID_COMMAND_RECORDER_CONTENTION):
            return "D3D12_MESSAGE_ID_COMMAND_RECORDER_CONTENTION";
        case(D3D12_MESSAGE_ID_COMMAND_RECORDER_USAGE_WITH_CREATECOMMANDLIST_COMMAND_LIST):
            return "D3D12_MESSAGE_ID_COMMAND_RECORDER_USAGE_WITH_CREATECOMMANDLIST_COMMAND_LIST";
        case(D3D12_MESSAGE_ID_COMMAND_ALLOCATOR_USAGE_WITH_CREATECOMMANDLIST1_COMMAND_LIST):
            return "D3D12_MESSAGE_ID_COMMAND_ALLOCATOR_USAGE_WITH_CREATECOMMANDLIST1_COMMAND_LIST";
        case(D3D12_MESSAGE_ID_CANNOT_EXECUTE_EMPTY_COMMAND_LIST):
            return "D3D12_MESSAGE_ID_CANNOT_EXECUTE_EMPTY_COMMAND_LIST";
        case(D3D12_MESSAGE_ID_CANNOT_RESET_COMMAND_POOL_WITH_OPEN_COMMAND_LISTS):
            return "D3D12_MESSAGE_ID_CANNOT_RESET_COMMAND_POOL_WITH_OPEN_COMMAND_LISTS";
        case(D3D12_MESSAGE_ID_CANNOT_USE_COMMAND_RECORDER_WITHOUT_CURRENT_TARGET):
            return "D3D12_MESSAGE_ID_CANNOT_USE_COMMAND_RECORDER_WITHOUT_CURRENT_TARGET";
        case(D3D12_MESSAGE_ID_CANNOT_CHANGE_COMMAND_RECORDER_TARGET_WHILE_RECORDING):
            return "D3D12_MESSAGE_ID_CANNOT_CHANGE_COMMAND_RECORDER_TARGET_WHILE_RECORDING";
        case(D3D12_MESSAGE_ID_COMMAND_POOL_SYNC):
            return "D3D12_MESSAGE_ID_COMMAND_POOL_SYNC";
        case(D3D12_MESSAGE_ID_EVICT_UNDERFLOW):
            return "D3D12_MESSAGE_ID_EVICT_UNDERFLOW";
        case(D3D12_MESSAGE_ID_CREATE_META_COMMAND):
            return "D3D12_MESSAGE_ID_CREATE_META_COMMAND";
        case(D3D12_MESSAGE_ID_LIVE_META_COMMAND):
            return "D3D12_MESSAGE_ID_LIVE_META_COMMAND";
        case(D3D12_MESSAGE_ID_DESTROY_META_COMMAND):
            return "D3D12_MESSAGE_ID_DESTROY_META_COMMAND";
        case(D3D12_MESSAGE_ID_COPYBUFFERREGION_INVALID_DST_RESOURCE):
            return "D3D12_MESSAGE_ID_COPYBUFFERREGION_INVALID_DST_RESOURCE";
        case(D3D12_MESSAGE_ID_COPYBUFFERREGION_INVALID_SRC_RESOURCE):
            return "D3D12_MESSAGE_ID_COPYBUFFERREGION_INVALID_SRC_RESOURCE";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_DST_RESOURCE):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_DST_RESOURCE";
        case(D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_SRC_RESOURCE):
            return "D3D12_MESSAGE_ID_ATOMICCOPYBUFFER_INVALID_SRC_RESOURCE";
        case(D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_NULL_BUFFER):
            return "D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_NULL_BUFFER";
        case(D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_NULL_RESOURCE_DESC):
            return "D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_NULL_RESOURCE_DESC";
        case(D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_UNSUPPORTED):
            return "D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_UNSUPPORTED";
        case(D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_INVALID_BUFFER_DIMENSION):
            return "D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_INVALID_BUFFER_DIMENSION";
        case(D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_INVALID_BUFFER_FLAGS):
            return "D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_INVALID_BUFFER_FLAGS";
        case(D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_INVALID_BUFFER_OFFSET):
            return "D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_INVALID_BUFFER_OFFSET";
        case(D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_INVALID_RESOURCE_DIMENSION):
            return "D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_INVALID_RESOURCE_DIMENSION";
        case(D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_INVALID_RESOURCE_FLAGS):
            return "D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_INVALID_RESOURCE_FLAGS";
        case(D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_OUTOFMEMORY_RETURN):
            return "D3D12_MESSAGE_ID_CREATEPLACEDRESOURCEONBUFFER_OUTOFMEMORY_RETURN";
        case(D3D12_MESSAGE_ID_CANNOT_CREATE_GRAPHICS_AND_VIDEO_COMMAND_RECORDER):
            return "D3D12_MESSAGE_ID_CANNOT_CREATE_GRAPHICS_AND_VIDEO_COMMAND_RECORDER";
        case(D3D12_MESSAGE_ID_UPDATETILEMAPPINGS_POSSIBLY_MISMATCHING_PROPERTIES):
            return "D3D12_MESSAGE_ID_UPDATETILEMAPPINGS_POSSIBLY_MISMATCHING_PROPERTIES";
        case(D3D12_MESSAGE_ID_CREATE_COMMAND_LIST_INVALID_COMMAND_LIST_TYPE):
            return "D3D12_MESSAGE_ID_CREATE_COMMAND_LIST_INVALID_COMMAND_LIST_TYPE";
        case(D3D12_MESSAGE_ID_CLEARUNORDEREDACCESSVIEW_INCOMPATIBLE_WITH_STRUCTURED_BUFFERS):
            return "D3D12_MESSAGE_ID_CLEARUNORDEREDACCESSVIEW_INCOMPATIBLE_WITH_STRUCTURED_BUFFERS";
        case(D3D12_MESSAGE_ID_COMPUTE_ONLY_DEVICE_OPERATION_UNSUPPORTED):
            return "D3D12_MESSAGE_ID_COMPUTE_ONLY_DEVICE_OPERATION_UNSUPPORTED";
        case(D3D12_MESSAGE_ID_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INVALID):
            return "D3D12_MESSAGE_ID_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INVALID";
        case(D3D12_MESSAGE_ID_EMIT_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_INVALID):
            return "D3D12_MESSAGE_ID_EMIT_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_INVALID";
        case(D3D12_MESSAGE_ID_COPY_RAYTRACING_ACCELERATION_STRUCTURE_INVALID):
            return "D3D12_MESSAGE_ID_COPY_RAYTRACING_ACCELERATION_STRUCTURE_INVALID";
        case(D3D12_MESSAGE_ID_DISPATCH_RAYS_INVALID):
            return "D3D12_MESSAGE_ID_DISPATCH_RAYS_INVALID";
        case(D3D12_MESSAGE_ID_GET_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO_INVALID):
            return "D3D12_MESSAGE_ID_GET_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO_INVALID";
        case(D3D12_MESSAGE_ID_CREATE_LIFETIMETRACKER):
            return "D3D12_MESSAGE_ID_CREATE_LIFETIMETRACKER";
        case(D3D12_MESSAGE_ID_LIVE_LIFETIMETRACKER):
            return "D3D12_MESSAGE_ID_LIVE_LIFETIMETRACKER";
        case(D3D12_MESSAGE_ID_DESTROY_LIFETIMETRACKER):
            return "D3D12_MESSAGE_ID_DESTROY_LIFETIMETRACKER";
        case(D3D12_MESSAGE_ID_DESTROYOWNEDOBJECT_OBJECTNOTOWNED):
            return "D3D12_MESSAGE_ID_DESTROYOWNEDOBJECT_OBJECTNOTOWNED";
        case(D3D12_MESSAGE_ID_CREATE_TRACKEDWORKLOAD):
            return "D3D12_MESSAGE_ID_CREATE_TRACKEDWORKLOAD";
        case(D3D12_MESSAGE_ID_LIVE_TRACKEDWORKLOAD):
            return "D3D12_MESSAGE_ID_LIVE_TRACKEDWORKLOAD";
        case(D3D12_MESSAGE_ID_DESTROY_TRACKEDWORKLOAD):
            return "D3D12_MESSAGE_ID_DESTROY_TRACKEDWORKLOAD";
        case(D3D12_MESSAGE_ID_RENDER_PASS_ERROR):
            return "D3D12_MESSAGE_ID_RENDER_PASS_ERROR";
        case(D3D12_MESSAGE_ID_META_COMMAND_ID_INVALID):
            return "D3D12_MESSAGE_ID_META_COMMAND_ID_INVALID";
        case(D3D12_MESSAGE_ID_META_COMMAND_UNSUPPORTED_PARAMS):
            return "D3D12_MESSAGE_ID_META_COMMAND_UNSUPPORTED_PARAMS";
        case(D3D12_MESSAGE_ID_META_COMMAND_FAILED_ENUMERATION):
            return "D3D12_MESSAGE_ID_META_COMMAND_FAILED_ENUMERATION";
        case(D3D12_MESSAGE_ID_META_COMMAND_PARAMETER_SIZE_MISMATCH):
            return "D3D12_MESSAGE_ID_META_COMMAND_PARAMETER_SIZE_MISMATCH";
        case(D3D12_MESSAGE_ID_UNINITIALIZED_META_COMMAND):
            return "D3D12_MESSAGE_ID_UNINITIALIZED_META_COMMAND";
        case(D3D12_MESSAGE_ID_META_COMMAND_INVALID_GPU_VIRTUAL_ADDRESS):
            return "D3D12_MESSAGE_ID_META_COMMAND_INVALID_GPU_VIRTUAL_ADDRESS";
        case(D3D12_MESSAGE_ID_CREATE_VIDEOENCODECOMMANDLIST):
            return "D3D12_MESSAGE_ID_CREATE_VIDEOENCODECOMMANDLIST";
        case(D3D12_MESSAGE_ID_LIVE_VIDEOENCODECOMMANDLIST):
            return "D3D12_MESSAGE_ID_LIVE_VIDEOENCODECOMMANDLIST";
        case(D3D12_MESSAGE_ID_DESTROY_VIDEOENCODECOMMANDLIST):
            return "D3D12_MESSAGE_ID_DESTROY_VIDEOENCODECOMMANDLIST";
        case(D3D12_MESSAGE_ID_CREATE_VIDEOENCODECOMMANDQUEUE):
            return "D3D12_MESSAGE_ID_CREATE_VIDEOENCODECOMMANDQUEUE";
        case(D3D12_MESSAGE_ID_LIVE_VIDEOENCODECOMMANDQUEUE):
            return "D3D12_MESSAGE_ID_LIVE_VIDEOENCODECOMMANDQUEUE";
        case(D3D12_MESSAGE_ID_DESTROY_VIDEOENCODECOMMANDQUEUE):
            return "D3D12_MESSAGE_ID_DESTROY_VIDEOENCODECOMMANDQUEUE";
        case(D3D12_MESSAGE_ID_CREATE_VIDEOMOTIONESTIMATOR):
            return "D3D12_MESSAGE_ID_CREATE_VIDEOMOTIONESTIMATOR";
        case(D3D12_MESSAGE_ID_LIVE_VIDEOMOTIONESTIMATOR):
            return "D3D12_MESSAGE_ID_LIVE_VIDEOMOTIONESTIMATOR";
        case(D3D12_MESSAGE_ID_DESTROY_VIDEOMOTIONESTIMATOR):
            return "D3D12_MESSAGE_ID_DESTROY_VIDEOMOTIONESTIMATOR";
        case(D3D12_MESSAGE_ID_CREATE_VIDEOMOTIONVECTORHEAP):
            return "D3D12_MESSAGE_ID_CREATE_VIDEOMOTIONVECTORHEAP";
        case(D3D12_MESSAGE_ID_LIVE_VIDEOMOTIONVECTORHEAP):
            return "D3D12_MESSAGE_ID_LIVE_VIDEOMOTIONVECTORHEAP";
        case(D3D12_MESSAGE_ID_DESTROY_VIDEOMOTIONVECTORHEAP):
            return "D3D12_MESSAGE_ID_DESTROY_VIDEOMOTIONVECTORHEAP";
        case(D3D12_MESSAGE_ID_MULTIPLE_TRACKED_WORKLOADS):
            return "D3D12_MESSAGE_ID_MULTIPLE_TRACKED_WORKLOADS";
        case(D3D12_MESSAGE_ID_MULTIPLE_TRACKED_WORKLOAD_PAIRS):
            return "D3D12_MESSAGE_ID_MULTIPLE_TRACKED_WORKLOAD_PAIRS";
        case(D3D12_MESSAGE_ID_OUT_OF_ORDER_TRACKED_WORKLOAD_PAIR):
            return "D3D12_MESSAGE_ID_OUT_OF_ORDER_TRACKED_WORKLOAD_PAIR";
        case(D3D12_MESSAGE_ID_CANNOT_ADD_TRACKED_WORKLOAD):
            return "D3D12_MESSAGE_ID_CANNOT_ADD_TRACKED_WORKLOAD";
        case(D3D12_MESSAGE_ID_INCOMPLETE_TRACKED_WORKLOAD_PAIR):
            return "D3D12_MESSAGE_ID_INCOMPLETE_TRACKED_WORKLOAD_PAIR";
        case(D3D12_MESSAGE_ID_CREATE_STATE_OBJECT_ERROR):
            return "D3D12_MESSAGE_ID_CREATE_STATE_OBJECT_ERROR";
        case(D3D12_MESSAGE_ID_GET_SHADER_IDENTIFIER_ERROR):
            return "D3D12_MESSAGE_ID_GET_SHADER_IDENTIFIER_ERROR";
        case(D3D12_MESSAGE_ID_GET_SHADER_STACK_SIZE_ERROR):
            return "D3D12_MESSAGE_ID_GET_SHADER_STACK_SIZE_ERROR";
        case(D3D12_MESSAGE_ID_GET_PIPELINE_STACK_SIZE_ERROR):
            return "D3D12_MESSAGE_ID_GET_PIPELINE_STACK_SIZE_ERROR";
        case(D3D12_MESSAGE_ID_SET_PIPELINE_STACK_SIZE_ERROR):
            return "D3D12_MESSAGE_ID_SET_PIPELINE_STACK_SIZE_ERROR";
        case(D3D12_MESSAGE_ID_GET_SHADER_IDENTIFIER_SIZE_INVALID):
            return "D3D12_MESSAGE_ID_GET_SHADER_IDENTIFIER_SIZE_INVALID";
        case(D3D12_MESSAGE_ID_CHECK_DRIVER_MATCHING_IDENTIFIER_INVALID):
            return "D3D12_MESSAGE_ID_CHECK_DRIVER_MATCHING_IDENTIFIER_INVALID";
        case(D3D12_MESSAGE_ID_CHECK_DRIVER_MATCHING_IDENTIFIER_DRIVER_REPORTED_ISSUE):
            return "D3D12_MESSAGE_ID_CHECK_DRIVER_MATCHING_IDENTIFIER_DRIVER_REPORTED_ISSUE";
        case(D3D12_MESSAGE_ID_RENDER_PASS_INVALID_RESOURCE_BARRIER):
            return "D3D12_MESSAGE_ID_RENDER_PASS_INVALID_RESOURCE_BARRIER";
        case(D3D12_MESSAGE_ID_RENDER_PASS_DISALLOWED_API_CALLED):
            return "D3D12_MESSAGE_ID_RENDER_PASS_DISALLOWED_API_CALLED";
        case(D3D12_MESSAGE_ID_RENDER_PASS_CANNOT_NEST_RENDER_PASSES):
            return "D3D12_MESSAGE_ID_RENDER_PASS_CANNOT_NEST_RENDER_PASSES";
        case(D3D12_MESSAGE_ID_RENDER_PASS_CANNOT_END_WITHOUT_BEGIN):
            return "D3D12_MESSAGE_ID_RENDER_PASS_CANNOT_END_WITHOUT_BEGIN";
        case(D3D12_MESSAGE_ID_RENDER_PASS_CANNOT_CLOSE_COMMAND_LIST):
            return "D3D12_MESSAGE_ID_RENDER_PASS_CANNOT_CLOSE_COMMAND_LIST";
        case(D3D12_MESSAGE_ID_RENDER_PASS_GPU_WORK_WHILE_SUSPENDED):
            return "D3D12_MESSAGE_ID_RENDER_PASS_GPU_WORK_WHILE_SUSPENDED";
        case(D3D12_MESSAGE_ID_RENDER_PASS_MISMATCHING_SUSPEND_RESUME):
            return "D3D12_MESSAGE_ID_RENDER_PASS_MISMATCHING_SUSPEND_RESUME";
        case(D3D12_MESSAGE_ID_RENDER_PASS_NO_PRIOR_SUSPEND_WITHIN_EXECUTECOMMANDLISTS):
            return "D3D12_MESSAGE_ID_RENDER_PASS_NO_PRIOR_SUSPEND_WITHIN_EXECUTECOMMANDLISTS";
        case(D3D12_MESSAGE_ID_RENDER_PASS_NO_SUBSEQUENT_RESUME_WITHIN_EXECUTECOMMANDLISTS):
            return "D3D12_MESSAGE_ID_RENDER_PASS_NO_SUBSEQUENT_RESUME_WITHIN_EXECUTECOMMANDLISTS";
        case(D3D12_MESSAGE_ID_TRACKED_WORKLOAD_COMMAND_QUEUE_MISMATCH):
            return "D3D12_MESSAGE_ID_TRACKED_WORKLOAD_COMMAND_QUEUE_MISMATCH";
        case(D3D12_MESSAGE_ID_TRACKED_WORKLOAD_NOT_SUPPORTED):
            return "D3D12_MESSAGE_ID_TRACKED_WORKLOAD_NOT_SUPPORTED";
        case(D3D12_MESSAGE_ID_RENDER_PASS_MISMATCHING_NO_ACCESS):
            return "D3D12_MESSAGE_ID_RENDER_PASS_MISMATCHING_NO_ACCESS";
        case(D3D12_MESSAGE_ID_RENDER_PASS_UNSUPPORTED_RESOLVE):
            return "D3D12_MESSAGE_ID_RENDER_PASS_UNSUPPORTED_RESOLVE";
        case(D3D12_MESSAGE_ID_CLEARUNORDEREDACCESSVIEW_INVALID_RESOURCE_PTR):
            return "D3D12_MESSAGE_ID_CLEARUNORDEREDACCESSVIEW_INVALID_RESOURCE_PTR";
        case(D3D12_MESSAGE_ID_WINDOWS7_FENCE_OUTOFORDER_SIGNAL):
            return "D3D12_MESSAGE_ID_WINDOWS7_FENCE_OUTOFORDER_SIGNAL";
        case(D3D12_MESSAGE_ID_WINDOWS7_FENCE_OUTOFORDER_WAIT):
            return "D3D12_MESSAGE_ID_WINDOWS7_FENCE_OUTOFORDER_WAIT";
        case(D3D12_MESSAGE_ID_VIDEO_CREATE_MOTION_ESTIMATOR_INVALID_ARGUMENT):
            return "D3D12_MESSAGE_ID_VIDEO_CREATE_MOTION_ESTIMATOR_INVALID_ARGUMENT";
        case(D3D12_MESSAGE_ID_VIDEO_CREATE_MOTION_VECTOR_HEAP_INVALID_ARGUMENT):
            return "D3D12_MESSAGE_ID_VIDEO_CREATE_MOTION_VECTOR_HEAP_INVALID_ARGUMENT";
        case(D3D12_MESSAGE_ID_ESTIMATE_MOTION_INVALID_ARGUMENT):
            return "D3D12_MESSAGE_ID_ESTIMATE_MOTION_INVALID_ARGUMENT";
        case(D3D12_MESSAGE_ID_RESOLVE_MOTION_VECTOR_HEAP_INVALID_ARGUMENT):
            return "D3D12_MESSAGE_ID_RESOLVE_MOTION_VECTOR_HEAP_INVALID_ARGUMENT";
        case(D3D12_MESSAGE_ID_GETGPUVIRTUALADDRESS_INVALID_HEAP_TYPE):
            return "D3D12_MESSAGE_ID_GETGPUVIRTUALADDRESS_INVALID_HEAP_TYPE";
        case(D3D12_MESSAGE_ID_SET_BACKGROUND_PROCESSING_MODE_INVALID_ARGUMENT):
            return "D3D12_MESSAGE_ID_SET_BACKGROUND_PROCESSING_MODE_INVALID_ARGUMENT";
        case(D3D12_MESSAGE_ID_CREATE_COMMAND_LIST_INVALID_COMMAND_LIST_TYPE_FOR_FEATURE_LEVEL):
            return "D3D12_MESSAGE_ID_CREATE_COMMAND_LIST_INVALID_COMMAND_LIST_TYPE_FOR_FEATURE_LEVEL";
        case(D3D12_MESSAGE_ID_CREATE_VIDEOEXTENSIONCOMMAND):
            return "D3D12_MESSAGE_ID_CREATE_VIDEOEXTENSIONCOMMAND";
        case(D3D12_MESSAGE_ID_LIVE_VIDEOEXTENSIONCOMMAND):
            return "D3D12_MESSAGE_ID_LIVE_VIDEOEXTENSIONCOMMAND";
        case(D3D12_MESSAGE_ID_DESTROY_VIDEOEXTENSIONCOMMAND):
            return "D3D12_MESSAGE_ID_DESTROY_VIDEOEXTENSIONCOMMAND";
        case(D3D12_MESSAGE_ID_INVALID_VIDEO_EXTENSION_COMMAND_ID):
            return "D3D12_MESSAGE_ID_INVALID_VIDEO_EXTENSION_COMMAND_ID";
        case(D3D12_MESSAGE_ID_VIDEO_EXTENSION_COMMAND_INVALID_ARGUMENT):
            return "D3D12_MESSAGE_ID_VIDEO_EXTENSION_COMMAND_INVALID_ARGUMENT";
        case(D3D12_MESSAGE_ID_CREATE_ROOT_SIGNATURE_NOT_UNIQUE_IN_DXIL_LIBRARY):
            return "D3D12_MESSAGE_ID_CREATE_ROOT_SIGNATURE_NOT_UNIQUE_IN_DXIL_LIBRARY";
        case(D3D12_MESSAGE_ID_VARIABLE_SHADING_RATE_NOT_ALLOWED_WITH_TIR):
            return "D3D12_MESSAGE_ID_VARIABLE_SHADING_RATE_NOT_ALLOWED_WITH_TIR";
        case(D3D12_MESSAGE_ID_GEOMETRY_SHADER_OUTPUTTING_BOTH_VIEWPORT_ARRAY_INDEX_AND_SHADING_RATE_NOT_SUPPORTED_ON_DEVICE):
            return "D3D12_MESSAGE_ID_GEOMETRY_SHADER_OUTPUTTING_BOTH_VIEWPORT_ARRAY_INDEX_AND_SHADING_RATE_NOT_SUPPORTED_ON_DEVICE";
        case(D3D12_MESSAGE_ID_RSSETSHADING_RATE_INVALID_SHADING_RATE):
            return "D3D12_MESSAGE_ID_RSSETSHADING_RATE_INVALID_SHADING_RATE";
        case(D3D12_MESSAGE_ID_RSSETSHADING_RATE_SHADING_RATE_NOT_PERMITTED_BY_CAP):
            return "D3D12_MESSAGE_ID_RSSETSHADING_RATE_SHADING_RATE_NOT_PERMITTED_BY_CAP";
        case(D3D12_MESSAGE_ID_RSSETSHADING_RATE_INVALID_COMBINER):
            return "D3D12_MESSAGE_ID_RSSETSHADING_RATE_INVALID_COMBINER";
        case(D3D12_MESSAGE_ID_RSSETSHADINGRATEIMAGE_REQUIRES_TIER_2):
            return "D3D12_MESSAGE_ID_RSSETSHADINGRATEIMAGE_REQUIRES_TIER_2";
        case(D3D12_MESSAGE_ID_RSSETSHADINGRATE_REQUIRES_TIER_1):
            return "D3D12_MESSAGE_ID_RSSETSHADINGRATE_REQUIRES_TIER_1";
        case(D3D12_MESSAGE_ID_SHADING_RATE_IMAGE_INCORRECT_FORMAT):
            return "D3D12_MESSAGE_ID_SHADING_RATE_IMAGE_INCORRECT_FORMAT";
        case(D3D12_MESSAGE_ID_SHADING_RATE_IMAGE_INCORRECT_ARRAY_SIZE):
            return "D3D12_MESSAGE_ID_SHADING_RATE_IMAGE_INCORRECT_ARRAY_SIZE";
        case(D3D12_MESSAGE_ID_SHADING_RATE_IMAGE_INCORRECT_MIP_LEVEL):
            return "D3D12_MESSAGE_ID_SHADING_RATE_IMAGE_INCORRECT_MIP_LEVEL";
        case(D3D12_MESSAGE_ID_SHADING_RATE_IMAGE_INCORRECT_SAMPLE_COUNT):
            return "D3D12_MESSAGE_ID_SHADING_RATE_IMAGE_INCORRECT_SAMPLE_COUNT";
        case(D3D12_MESSAGE_ID_SHADING_RATE_IMAGE_INCORRECT_SAMPLE_QUALITY):
            return "D3D12_MESSAGE_ID_SHADING_RATE_IMAGE_INCORRECT_SAMPLE_QUALITY";
        case(D3D12_MESSAGE_ID_NON_RETAIL_SHADER_MODEL_WONT_VALIDATE):
            return "D3D12_MESSAGE_ID_NON_RETAIL_SHADER_MODEL_WONT_VALIDATE";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_AS_ROOT_SIGNATURE_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_AS_ROOT_SIGNATURE_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_MS_ROOT_SIGNATURE_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_MS_ROOT_SIGNATURE_MISMATCH";
        case(D3D12_MESSAGE_ID_ADD_TO_STATE_OBJECT_ERROR):
            return "D3D12_MESSAGE_ID_ADD_TO_STATE_OBJECT_ERROR";
        case(D3D12_MESSAGE_ID_CREATE_PROTECTED_RESOURCE_SESSION_INVALID_ARGUMENT):
            return "D3D12_MESSAGE_ID_CREATE_PROTECTED_RESOURCE_SESSION_INVALID_ARGUMENT";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_MS_PSO_DESC_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_MS_PSO_DESC_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEPIPELINESTATE_MS_INCOMPLETE_TYPE):
            return "D3D12_MESSAGE_ID_CREATEPIPELINESTATE_MS_INCOMPLETE_TYPE";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_AS_NOT_MS_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_AS_NOT_MS_MISMATCH";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_MS_NOT_PS_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_MS_NOT_PS_MISMATCH";
        case(D3D12_MESSAGE_ID_NONZERO_SAMPLER_FEEDBACK_MIP_REGION_WITH_INCOMPATIBLE_FORMAT):
            return "D3D12_MESSAGE_ID_NONZERO_SAMPLER_FEEDBACK_MIP_REGION_WITH_INCOMPATIBLE_FORMAT";
        case(D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INPUTLAYOUT_SHADER_MISMATCH):
            return "D3D12_MESSAGE_ID_CREATEGRAPHICSPIPELINESTATE_INPUTLAYOUT_SHADER_MISMATCH";
        case(D3D12_MESSAGE_ID_EMPTY_DISPATCH):
            return "D3D12_MESSAGE_ID_EMPTY_DISPATCH";
        case(D3D12_MESSAGE_ID_RESOURCE_FORMAT_REQUIRES_SAMPLER_FEEDBACK_CAPABILITY):
            return "D3D12_MESSAGE_ID_RESOURCE_FORMAT_REQUIRES_SAMPLER_FEEDBACK_CAPABILITY";
        case(D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_MAP_INVALID_MIP_REGION):
            return "D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_MAP_INVALID_MIP_REGION";
        case(D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_MAP_INVALID_DIMENSION):
            return "D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_MAP_INVALID_DIMENSION";
        case(D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_MAP_INVALID_SAMPLE_COUNT):
            return "D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_MAP_INVALID_SAMPLE_COUNT";
        case(D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_MAP_INVALID_SAMPLE_QUALITY):
            return "D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_MAP_INVALID_SAMPLE_QUALITY";
        case(D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_MAP_INVALID_LAYOUT):
            return "D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_MAP_INVALID_LAYOUT";
        case(D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_MAP_REQUIRES_UNORDERED_ACCESS_FLAG):
            return "D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_MAP_REQUIRES_UNORDERED_ACCESS_FLAG";
        case(D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_CREATE_UAV_NULL_ARGUMENTS):
            return "D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_CREATE_UAV_NULL_ARGUMENTS";
        case(D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_UAV_REQUIRES_SAMPLER_FEEDBACK_CAPABILITY):
            return "D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_UAV_REQUIRES_SAMPLER_FEEDBACK_CAPABILITY";
        case(D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_CREATE_UAV_REQUIRES_FEEDBACK_MAP_FORMAT):
            return "D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_CREATE_UAV_REQUIRES_FEEDBACK_MAP_FORMAT";
        case(D3D12_MESSAGE_ID_CREATEMESHSHADER_INVALIDSHADERBYTECODE):
            return "D3D12_MESSAGE_ID_CREATEMESHSHADER_INVALIDSHADERBYTECODE";
        case(D3D12_MESSAGE_ID_CREATEMESHSHADER_OUTOFMEMORY):
            return "D3D12_MESSAGE_ID_CREATEMESHSHADER_OUTOFMEMORY";
        case(D3D12_MESSAGE_ID_CREATEMESHSHADERWITHSTREAMOUTPUT_INVALIDSHADERTYPE):
            return "D3D12_MESSAGE_ID_CREATEMESHSHADERWITHSTREAMOUTPUT_INVALIDSHADERTYPE";
        case(D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_SAMPLER_FEEDBACK_TRANSCODE_INVALID_FORMAT):
            return "D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_SAMPLER_FEEDBACK_TRANSCODE_INVALID_FORMAT";
        case(D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_SAMPLER_FEEDBACK_INVALID_MIP_LEVEL_COUNT):
            return "D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_SAMPLER_FEEDBACK_INVALID_MIP_LEVEL_COUNT";
        case(D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_SAMPLER_FEEDBACK_TRANSCODE_ARRAY_SIZE_MISMATCH):
            return "D3D12_MESSAGE_ID_RESOLVESUBRESOURCE_SAMPLER_FEEDBACK_TRANSCODE_ARRAY_SIZE_MISMATCH";
        case(D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_CREATE_UAV_MISMATCHING_TARGETED_RESOURCE):
            return "D3D12_MESSAGE_ID_SAMPLER_FEEDBACK_CREATE_UAV_MISMATCHING_TARGETED_RESOURCE";
        case(D3D12_MESSAGE_ID_CREATEMESHSHADER_OUTPUTEXCEEDSMAXSIZE):
            return "D3D12_MESSAGE_ID_CREATEMESHSHADER_OUTPUTEXCEEDSMAXSIZE";
        case(D3D12_MESSAGE_ID_CREATEMESHSHADER_GROUPSHAREDEXCEEDSMAXSIZE):
            return "D3D12_MESSAGE_ID_CREATEMESHSHADER_GROUPSHAREDEXCEEDSMAXSIZE";
        case(D3D12_MESSAGE_ID_VERTEX_SHADER_OUTPUTTING_BOTH_VIEWPORT_ARRAY_INDEX_AND_SHADING_RATE_NOT_SUPPORTED_ON_DEVICE):
            return "D3D12_MESSAGE_ID_VERTEX_SHADER_OUTPUTTING_BOTH_VIEWPORT_ARRAY_INDEX_AND_SHADING_RATE_NOT_SUPPORTED_ON_DEVICE";
        case(D3D12_MESSAGE_ID_MESH_SHADER_OUTPUTTING_BOTH_VIEWPORT_ARRAY_INDEX_AND_SHADING_RATE_NOT_SUPPORTED_ON_DEVICE):
            return "D3D12_MESSAGE_ID_MESH_SHADER_OUTPUTTING_BOTH_VIEWPORT_ARRAY_INDEX_AND_SHADING_RATE_NOT_SUPPORTED_ON_DEVICE";
        case(D3D12_MESSAGE_ID_CREATEMESHSHADER_MISMATCHEDASMSPAYLOADSIZE):
            return "D3D12_MESSAGE_ID_CREATEMESHSHADER_MISMATCHEDASMSPAYLOADSIZE";
        case(D3D12_MESSAGE_ID_CREATE_ROOT_SIGNATURE_UNBOUNDED_STATIC_DESCRIPTORS):
            return "D3D12_MESSAGE_ID_CREATE_ROOT_SIGNATURE_UNBOUNDED_STATIC_DESCRIPTORS";
        case(D3D12_MESSAGE_ID_CREATEAMPLIFICATIONSHADER_INVALIDSHADERBYTECODE):
            return "D3D12_MESSAGE_ID_CREATEAMPLIFICATIONSHADER_INVALIDSHADERBYTECODE";
        case(D3D12_MESSAGE_ID_CREATEAMPLIFICATIONSHADER_OUTOFMEMORY):
            return "D3D12_MESSAGE_ID_CREATEAMPLIFICATIONSHADER_OUTOFMEMORY";
        case(D3D12_MESSAGE_ID_D3D12_MESSAGES_END):
            return "D3D12_MESSAGE_ID_D3D12_MESSAGES_END";
        default:
            {
                std::string code = "Invalid D3D12_MESSAGE_ID(";
                code.append(std::to_string(value));
                code.append(")");
                return code;
            }
    }
}

static std::string ConverttoText(REFIID value)
{
    if(value == IID_IDXGIObject)
    {
        return "IID_IDXGIObject";
    }
    if(value == IID_IDXGIDeviceSubObject)
    {
        return "IID_IDXGIDeviceSubObject";
    }
    if(value == IID_IDXGIResource)
    {
        return "IID_IDXGIResource";
    }
    if(value == IID_IDXGIKeyedMutex)
    {
        return "IID_IDXGIKeyedMutex";
    }
    if(value == IID_IDXGISurface)
    {
        return "IID_IDXGISurface";
    }
    if(value == IID_IDXGISurface1)
    {
        return "IID_IDXGISurface1";
    }
    if(value == IID_IDXGIAdapter)
    {
        return "IID_IDXGIAdapter";
    }
    if(value == IID_IDXGIOutput)
    {
        return "IID_IDXGIOutput";
    }
    if(value == IID_IDXGISwapChain)
    {
        return "IID_IDXGISwapChain";
    }
    if(value == IID_IDXGIFactory)
    {
        return "IID_IDXGIFactory";
    }
    if(value == IID_IDXGIDevice)
    {
        return "IID_IDXGIDevice";
    }
    if(value == IID_IDXGIFactory1)
    {
        return "IID_IDXGIFactory1";
    }
    if(value == IID_IDXGIAdapter1)
    {
        return "IID_IDXGIAdapter1";
    }
    if(value == IID_IDXGIDevice1)
    {
        return "IID_IDXGIDevice1";
    }
    if(value == IID_IDXGIDisplayControl)
    {
        return "IID_IDXGIDisplayControl";
    }
    if(value == IID_IDXGIOutputDuplication)
    {
        return "IID_IDXGIOutputDuplication";
    }
    if(value == IID_IDXGISurface2)
    {
        return "IID_IDXGISurface2";
    }
    if(value == IID_IDXGIResource1)
    {
        return "IID_IDXGIResource1";
    }
    if(value == IID_IDXGIDevice2)
    {
        return "IID_IDXGIDevice2";
    }
    if(value == IID_IDXGISwapChain1)
    {
        return "IID_IDXGISwapChain1";
    }
    if(value == IID_IDXGIFactory2)
    {
        return "IID_IDXGIFactory2";
    }
    if(value == IID_IDXGIAdapter2)
    {
        return "IID_IDXGIAdapter2";
    }
    if(value == IID_IDXGIOutput1)
    {
        return "IID_IDXGIOutput1";
    }
    if(value == IID_IDXGIDevice3)
    {
        return "IID_IDXGIDevice3";
    }
    if(value == IID_IDXGISwapChain2)
    {
        return "IID_IDXGISwapChain2";
    }
    if(value == IID_IDXGIOutput2)
    {
        return "IID_IDXGIOutput2";
    }
    if(value == IID_IDXGIFactory3)
    {
        return "IID_IDXGIFactory3";
    }
    if(value == IID_IDXGIDecodeSwapChain)
    {
        return "IID_IDXGIDecodeSwapChain";
    }
    if(value == IID_IDXGIFactoryMedia)
    {
        return "IID_IDXGIFactoryMedia";
    }
    if(value == IID_IDXGISwapChainMedia)
    {
        return "IID_IDXGISwapChainMedia";
    }
    if(value == IID_IDXGIOutput3)
    {
        return "IID_IDXGIOutput3";
    }
    if(value == IID_IDXGISwapChain3)
    {
        return "IID_IDXGISwapChain3";
    }
    if(value == IID_IDXGIOutput4)
    {
        return "IID_IDXGIOutput4";
    }
    if(value == IID_IDXGIFactory4)
    {
        return "IID_IDXGIFactory4";
    }
    if(value == IID_IDXGIAdapter3)
    {
        return "IID_IDXGIAdapter3";
    }
    if(value == IID_IDXGIOutput5)
    {
        return "IID_IDXGIOutput5";
    }
    if(value == IID_IDXGISwapChain4)
    {
        return "IID_IDXGISwapChain4";
    }
    if(value == IID_IDXGIDevice4)
    {
        return "IID_IDXGIDevice4";
    }
    if(value == IID_IDXGIFactory5)
    {
        return "IID_IDXGIFactory5";
    }
    if(value == IID_IDXGIAdapter4)
    {
        return "IID_IDXGIAdapter4";
    }
    if(value == IID_IDXGIOutput6)
    {
        return "IID_IDXGIOutput6";
    }
    if(value == IID_IDXGIFactory6)
    {
        return "IID_IDXGIFactory6";
    }
    if(value == IID_IDXGIFactory7)
    {
        return "IID_IDXGIFactory7";
    }
    if(value == D3D12_PROTECTED_RESOURCES_SESSION_HARDWARE_PROTECTED)
    {
        return "D3D12_PROTECTED_RESOURCES_SESSION_HARDWARE_PROTECTED";
    }
    if(value == IID_ID3D12Object)
    {
        return "IID_ID3D12Object";
    }
    if(value == IID_ID3D12DeviceChild)
    {
        return "IID_ID3D12DeviceChild";
    }
    if(value == IID_ID3D12RootSignature)
    {
        return "IID_ID3D12RootSignature";
    }
    if(value == IID_ID3D12RootSignatureDeserializer)
    {
        return "IID_ID3D12RootSignatureDeserializer";
    }
    if(value == IID_ID3D12VersionedRootSignatureDeserializer)
    {
        return "IID_ID3D12VersionedRootSignatureDeserializer";
    }
    if(value == IID_ID3D12Pageable)
    {
        return "IID_ID3D12Pageable";
    }
    if(value == IID_ID3D12Heap)
    {
        return "IID_ID3D12Heap";
    }
    if(value == IID_ID3D12Resource)
    {
        return "IID_ID3D12Resource";
    }
    if(value == IID_ID3D12CommandAllocator)
    {
        return "IID_ID3D12CommandAllocator";
    }
    if(value == IID_ID3D12Fence)
    {
        return "IID_ID3D12Fence";
    }
    if(value == IID_ID3D12Fence1)
    {
        return "IID_ID3D12Fence1";
    }
    if(value == IID_ID3D12PipelineState)
    {
        return "IID_ID3D12PipelineState";
    }
    if(value == IID_ID3D12DescriptorHeap)
    {
        return "IID_ID3D12DescriptorHeap";
    }
    if(value == IID_ID3D12QueryHeap)
    {
        return "IID_ID3D12QueryHeap";
    }
    if(value == IID_ID3D12CommandSignature)
    {
        return "IID_ID3D12CommandSignature";
    }
    if(value == IID_ID3D12CommandList)
    {
        return "IID_ID3D12CommandList";
    }
    if(value == IID_ID3D12GraphicsCommandList)
    {
        return "IID_ID3D12GraphicsCommandList";
    }
    if(value == IID_ID3D12GraphicsCommandList1)
    {
        return "IID_ID3D12GraphicsCommandList1";
    }
    if(value == IID_ID3D12GraphicsCommandList2)
    {
        return "IID_ID3D12GraphicsCommandList2";
    }
    if(value == IID_ID3D12CommandQueue)
    {
        return "IID_ID3D12CommandQueue";
    }
    if(value == IID_ID3D12Device)
    {
        return "IID_ID3D12Device";
    }
    if(value == IID_ID3D12PipelineLibrary)
    {
        return "IID_ID3D12PipelineLibrary";
    }
    if(value == IID_ID3D12PipelineLibrary1)
    {
        return "IID_ID3D12PipelineLibrary1";
    }
    if(value == IID_ID3D12Device1)
    {
        return "IID_ID3D12Device1";
    }
    if(value == IID_ID3D12Device2)
    {
        return "IID_ID3D12Device2";
    }
    if(value == IID_ID3D12Device3)
    {
        return "IID_ID3D12Device3";
    }
    if(value == IID_ID3D12ProtectedSession)
    {
        return "IID_ID3D12ProtectedSession";
    }
    if(value == IID_ID3D12ProtectedResourceSession)
    {
        return "IID_ID3D12ProtectedResourceSession";
    }
    if(value == IID_ID3D12Device4)
    {
        return "IID_ID3D12Device4";
    }
    if(value == IID_ID3D12LifetimeOwner)
    {
        return "IID_ID3D12LifetimeOwner";
    }
    if(value == IID_ID3D12SwapChainAssistant)
    {
        return "IID_ID3D12SwapChainAssistant";
    }
    if(value == IID_ID3D12LifetimeTracker)
    {
        return "IID_ID3D12LifetimeTracker";
    }
    if(value == IID_ID3D12StateObject)
    {
        return "IID_ID3D12StateObject";
    }
    if(value == IID_ID3D12StateObjectProperties)
    {
        return "IID_ID3D12StateObjectProperties";
    }
    if(value == IID_ID3D12Device5)
    {
        return "IID_ID3D12Device5";
    }
    if(value == IID_ID3D12DeviceRemovedExtendedDataSettings)
    {
        return "IID_ID3D12DeviceRemovedExtendedDataSettings";
    }
    if(value == IID_ID3D12DeviceRemovedExtendedDataSettings1)
    {
        return "IID_ID3D12DeviceRemovedExtendedDataSettings1";
    }
    if(value == IID_ID3D12DeviceRemovedExtendedData)
    {
        return "IID_ID3D12DeviceRemovedExtendedData";
    }
    if(value == IID_ID3D12DeviceRemovedExtendedData1)
    {
        return "IID_ID3D12DeviceRemovedExtendedData1";
    }
    if(value == IID_ID3D12Device6)
    {
        return "IID_ID3D12Device6";
    }
    if(value == IID_ID3D12ProtectedResourceSession1)
    {
        return "IID_ID3D12ProtectedResourceSession1";
    }
    if(value == IID_ID3D12Device7)
    {
        return "IID_ID3D12Device7";
    }
    if(value == IID_ID3D12Device8)
    {
        return "IID_ID3D12Device8";
    }
    if(value == IID_ID3D12Resource1)
    {
        return "IID_ID3D12Resource1";
    }
    if(value == IID_ID3D12Resource2)
    {
        return "IID_ID3D12Resource2";
    }
    if(value == IID_ID3D12Heap1)
    {
        return "IID_ID3D12Heap1";
    }
    if(value == IID_ID3D12GraphicsCommandList3)
    {
        return "IID_ID3D12GraphicsCommandList3";
    }
    if(value == IID_ID3D12MetaCommand)
    {
        return "IID_ID3D12MetaCommand";
    }
    if(value == IID_ID3D12GraphicsCommandList4)
    {
        return "IID_ID3D12GraphicsCommandList4";
    }
    if(value == IID_ID3D12Tools)
    {
        return "IID_ID3D12Tools";
    }
    if(value == IID_ID3D12GraphicsCommandList5)
    {
        return "IID_ID3D12GraphicsCommandList5";
    }
    if(value == IID_ID3D12GraphicsCommandList6)
    {
        return "IID_ID3D12GraphicsCommandList6";
    }
    if(value == IID_ID3D10Blob)
    {
        return "IID_ID3D10Blob";
    }
    if(value == WKPDID_D3DDebugObjectName)
    {
        return "WKPDID_D3DDebugObjectName";
    }
    if(value == WKPDID_D3DDebugObjectNameW)
    {
        return "WKPDID_D3DDebugObjectNameW";
    }
    if(value == WKPDID_CommentStringW)
    {
        return "WKPDID_CommentStringW";
    }
    if(value == D3D_TEXTURE_LAYOUT_ROW_MAJOR)
    {
        return "D3D_TEXTURE_LAYOUT_ROW_MAJOR";
    }
    if(value == D3D_TEXTURE_LAYOUT_64KB_STANDARD_SWIZZLE)
    {
        return "D3D_TEXTURE_LAYOUT_64KB_STANDARD_SWIZZLE";
    }
    if(value == DXGI_DEBUG_D3D12)
    {
        return "DXGI_DEBUG_D3D12";
    }
    if(value == IID_ID3D12Debug)
    {
        return "IID_ID3D12Debug";
    }
    if(value == IID_ID3D12Debug1)
    {
        return "IID_ID3D12Debug1";
    }
    if(value == IID_ID3D12Debug2)
    {
        return "IID_ID3D12Debug2";
    }
    if(value == IID_ID3D12Debug3)
    {
        return "IID_ID3D12Debug3";
    }
    if(value == IID_ID3D12DebugDevice1)
    {
        return "IID_ID3D12DebugDevice1";
    }
    if(value == IID_ID3D12DebugDevice)
    {
        return "IID_ID3D12DebugDevice";
    }
    if(value == IID_ID3D12DebugDevice2)
    {
        return "IID_ID3D12DebugDevice2";
    }
    if(value == IID_ID3D12DebugCommandQueue)
    {
        return "IID_ID3D12DebugCommandQueue";
    }
    if(value == IID_ID3D12DebugCommandList1)
    {
        return "IID_ID3D12DebugCommandList1";
    }
    if(value == IID_ID3D12DebugCommandList)
    {
        return "IID_ID3D12DebugCommandList";
    }
    if(value == IID_ID3D12DebugCommandList2)
    {
        return "IID_ID3D12DebugCommandList2";
    }
    if(value == IID_ID3D12SharingContract)
    {
        return "IID_ID3D12SharingContract";
    }
    if(value == IID_ID3D12InfoQueue)
    {
        return "IID_ID3D12InfoQueue";
    }
    return "Invalid IID";
}


GFXRECON_END_NAMESPACE(decode)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif
