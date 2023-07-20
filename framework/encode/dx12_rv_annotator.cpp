/*
** Copyright (c) 2023 LunarG, Inc.
** Copyright (c) 2023 Advanced Micro Devices, Inc. All rights reserved.
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

#include "encode/d3d12_capture_manager.h"
#include "encode/dx12_rv_annotator.h"
#include "encode/dx12_rv_annotation_util.h"
#include "util/logging.h"

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(encode)

thread_local std::vector<uint8_t> Dx12ResourceValueAnnotator::scratch_space_;

void Dx12ResourceValueAnnotator::RemoveObjectGPUVA(IUnknown_Wrapper* wrapper)
{
    auto riid = wrapper->GetRiid();
    if (IsEqualGUID(riid, __uuidof(ID3D12Resource)) || IsEqualGUID(riid, __uuidof(ID3D12Resource1)) ||
        IsEqualGUID(riid, __uuidof(ID3D12Resource2)))
    {
        RemoveGPUVA(wrapper->GetCaptureId(),
                    reinterpret_cast<ID3D12Resource_Wrapper*>(wrapper)->GetObjectInfo()->gpu_va);
    }
    else if (IsEqualGUID(riid, __uuidof(ID3D12Heap)) || IsEqualGUID(riid, __uuidof(ID3D12Heap1)))
    {
        RemoveGPUVA(wrapper->GetCaptureId(), reinterpret_cast<ID3D12Heap_Wrapper*>(wrapper)->GetObjectInfo()->gpu_va);
    }
}

void Dx12ResourceValueAnnotator::AddNewRangeofGPUVA(format::HandleId resource_id,
                                                    uint64_t         start_address,
                                                    uint64_t         width)
{
    if (low_gpuva == 0 || low_gpuva > start_address)
    {
        low_gpuva = start_address;
    }
    if (high_gpuva == 0 || high_gpuva < (start_address + width))
    {
        high_gpuva = (start_address + width);
    }

    gpu_va_map_.Add(resource_id, start_address, width, start_address);

    /*auto gpuva = gpuva_map_.find(start_address);
    if (gpuva == gpuva_map_.end())
    {
#ifdef _DEBUG
        auto va_entry = gpuva_map_.lower_bound(start_address);
        if (va_entry != gpuva_map_.end())
        {
            _ASSERT(va_entry->second <= start_address);
        }
#endif
        gpuva_map_.insert(std::pair<uint64_t, uint64_t>(start_address, end_address));
    }
#ifdef _DEBUG
    else
    {
        _ASSERT(gpuva->second >= end_address);
    }
#endif*/
}

void Dx12ResourceValueAnnotator::SetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type,
                                                                  uint8_t                    increment_size)
{
    increment_size_[type] = increment_size;
}

void Dx12ResourceValueAnnotator::AddDescriptorHandleStart(ID3D12DescriptorHeap_Wrapper* wrapper,
                                                          D3D12_GPU_DESCRIPTOR_HANDLE   result)
{

    uint8_t                            increment_size = 0;
    graphics::dx12::ID3D12DeviceComPtr device         = nullptr;
    D3D12_DESCRIPTOR_HEAP_DESC         desc           = wrapper->GetDesc();
    if (SUCCEEDED(wrapper->GetDevice(IID_PPV_ARGS(&device))))
    {
        increment_size = device->GetDescriptorHandleIncrementSize(desc.Type);
        if (result.ptr != 0)
        {
            if (low_handle == 0 || low_handle > result.ptr)
            {
                low_handle = result.ptr;
            }
            descriptor_map_.insert(std::pair<uint64_t, D3D12_DESCRIPTOR_HEAP_DESC>(result.ptr, desc));
            increment_size_[desc.Type] = increment_size;
            uint64_t end_handle        = result.ptr + (uint64_t)increment_size * desc.NumDescriptors;
            if (high_handle == 0 || high_handle < end_handle)
            {
                high_handle = end_handle;
            }
        }
    }
}

void Dx12ResourceValueAnnotator::AddShaderID(graphics::Dx12ShaderIdentifier shader_id)
{
    constexpr graphics::Dx12ShaderIdentifier zero_id = { 0 };
    if (shader_id != zero_id)
    {
        shader_ids_.insert(shader_id);
    }
}

void Dx12ResourceValueAnnotator::RemoveGPUVA(format::HandleId resource_id, D3D12_GPU_VIRTUAL_ADDRESS start_address)
{
    gpu_va_map_.Remove(resource_id, start_address);
    // auto va_entry = gpuva_map_.find(start_address);
    // if (va_entry != gpuva_map_.end())
    //{
    //    gpuva_map_.erase(start_address);
    //}
}

bool Dx12ResourceValueAnnotator::IsValidGpuVa(D3D12_GPU_VIRTUAL_ADDRESS address)
{
    bool found;
    gpu_va_map_.Map(address, nullptr, &found);
    // if (address >= low_gpuva && address <= high_gpuva)
    //{
    //    auto va_entry = gpuva_map_.lower_bound(address);
    //    if (va_entry != gpuva_map_.end())
    //    {
    //        if (address < va_entry->second)
    //        {
    //            return true;
    //        }
    //    }
    //}
    return found;
}

bool Dx12ResourceValueAnnotator::IsValidGPUDescriptorHandle(D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
    if (handle.ptr >= low_handle && handle.ptr <= high_handle)
    {
        for (auto& it : descriptor_map_)
        {
            uint64_t diff = handle.ptr - it.first;
            if ((diff % increment_size_[it.second.Type]) == 0)
            {
                if (diff / increment_size_[it.second.Type] <= it.second.NumDescriptors)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Dx12ResourceValueAnnotator::MatchShaderIdentifier(const uint8_t* data)
{
    for (const auto& shader_id : shader_ids_)
    {
        bool match = true;
        for (uint64_t i = 0; (i < D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES - sizeof(uint64_t)) && match; ++i)
        {
            match = match && (shader_id.data()[i] == data[i]);
        }
        if (match)
        {
            return true;
        }
    }
    return false;
}

void Dx12ResourceValueAnnotator::PostProcessGetGPUVirtualAddress(ID3D12Resource_Wrapper*   wrapper,
                                                                 D3D12_GPU_VIRTUAL_ADDRESS result)
{
    // static bool bCheckedGPUVABits = false;
    // if (bCheckedGPUVABits == false)
    //{
    //    ID3D12Device* device = nullptr;
    //    HRESULT       result = wrapper->GetDevice(IID_PPV_ARGS(&device));
    //    if (result == S_OK)
    //    {
    //        D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT feature_data{};
    //        device->CheckFeatureSupport(D3D12_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT,
    //                                    &feature_data,
    //                                    sizeof(D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT));
    //        UINT maxGPUVAbits =
    //            feature_data.MaxGPUVirtualAddressBitsPerResource >= feature_data.MaxGPUVirtualAddressBitsPerProcess
    //                ? feature_data.MaxGPUVirtualAddressBitsPerResource
    //                : feature_data.MaxGPUVirtualAddressBitsPerProcess;
    //        if (maxGPUVAbits <= (64 - RvAnnotationUtil::kMaskSizeOfBits))
    //        {
    //            bCheckedGPUVABits = true;
    //        }
    //        else
    //        {
    //            GFXRECON_LOG_ERROR_ONCE("Insufficient bits available in GPUVA for RV annotation");
    //        }
    //        device->Release();
    //    }
    //}

    format::HandleId resource_id   = wrapper->GetCaptureId();
    auto resource_info = wrapper->GetObjectInfo();
    if (resource_info->dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
    {
        uint64_t width        = resource_info->subresource_sizes[0];
        resource_info->gpu_va = result;
        // if (resource_info->heap_wrapper != nullptr)
        //{
        //    auto heap_info    = resource_info->heap_wrapper->GetObjectInfo();
        //    heap_info->gpu_va = result - resource_info->heap_offset;
        //    width             = heap_info->heap_size;
        //}
        AddNewRangeofGPUVA(resource_id, result, width);
    }
}

static size_t GetResourceValueSize(format::ResourceValueType type)
{
    switch (type)
    {
        case format::ResourceValueType::kGpuVirtualAddress:
            return sizeof(D3D12_GPU_VIRTUAL_ADDRESS);
        case format::ResourceValueType::kGpuDescriptorHandle:
            return sizeof(D3D12_GPU_DESCRIPTOR_HANDLE::ptr);
        case format::ResourceValueType::kShaderIdentifier:
            return D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
        default:
            GFXRECON_ASSERT(false && "Unrecognized resource value type.");
            return 0;
    }
}

void Dx12ResourceValueAnnotator::ScanForGPUVA(
    uint64_t memory_id, const uint8_t* src_data, uint8_t* dst_data, uint64_t size, uint64_t offset)
{
    bool                                             memory_modified = false;
    std::map<uint64_t, Dx12FillCommandResourceValue> resource_values;
    auto                                             manager                      = D3D12CaptureManager::Get();
    auto&                                            existing_resource_values_map = resource_values_map_[memory_id];

    if (scratch_space_.size() < size)
    {
        scratch_space_.resize(size);
    }
    memcpy(scratch_space_.data(), src_data, size);

    if (size >= sizeof(uint64_t))
    {
        constexpr uint64_t stride = 2;
        for (uint64_t index = 0; index <= size - sizeof(uint64_t); index += stride)
        {
            if (index <= size - D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES)
            {
                uint64_t shader_id_mask;
                memcpy(&shader_id_mask, scratch_space_.data() + index + 3 * sizeof(uint64_t), sizeof(uint64_t));
                if (shader_id_mask == manager->GetShaderIDMask())
                {
                    std::array<uint8_t, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES> shader_id;
                    memcpy(shader_id.data(), scratch_space_.data() + index, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);

                    if (MatchShaderIdentifier(shader_id.data()))
                    {
                        memory_modified = true;
                        memset(shader_id.data() + D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES - sizeof(uint64_t),
                               0x0,
                               sizeof(uint64_t));
                        memcpy(scratch_space_.data() + index, shader_id.data(), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);

                        Dx12FillCommandResourceValue rv = { index + offset,
                                                            format::ResourceValueType::kShaderIdentifier };
                        std::memcpy(rv.value.data(),
                                    shader_id.data(),
                                    GetResourceValueSize(format::ResourceValueType::kShaderIdentifier));

                        resource_values[index + offset] = std::move(rv);

                        index += D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES - stride;
                        continue;
                    }
                }
            }

            uint64_t data_value;
            memcpy(&data_value, scratch_space_.data() + index, sizeof(uint64_t));
            if (data_value != 0x0)
            {
                uint64_t mask = data_value >> (64 - RvAnnotationUtil::kMaskSizeOfBits);
                if (mask == manager->GetGPUVAMask())
                {
                    uint64_t gpu_va = data_value & (~0x0ui64 >> RvAnnotationUtil::kMaskSizeOfBits);
                    if (gpu_va != 0x0 && IsValidGpuVa(gpu_va) == true)
                    {
                        memory_modified = true;
                        memcpy(scratch_space_.data() + index, &gpu_va, sizeof(uint64_t));

                        Dx12FillCommandResourceValue rv = { index + offset,
                                                            format::ResourceValueType::kGpuVirtualAddress };
                        std::memcpy(rv.value.data(),
                                    &gpu_va,
                                    GetResourceValueSize(format::ResourceValueType::kGpuVirtualAddress));

                        resource_values[index + offset] = std::move(rv);
                        index += sizeof(uint64_t) - stride;
                    }
                }
                else if (mask == manager->GetDescriptorMask())
                {
                    uint64_t gpu_desc_ptr              = data_value & (~0x0ui64 >> RvAnnotationUtil::kMaskSizeOfBits);
                    D3D12_GPU_DESCRIPTOR_HANDLE handle = { gpu_desc_ptr };
                    if (handle.ptr != 0x0 && IsValidGPUDescriptorHandle(handle) == true)
                    {
                        memory_modified = true;
                        memcpy(scratch_space_.data() + index, &gpu_desc_ptr, sizeof(uint64_t));

                        Dx12FillCommandResourceValue rv = { index + offset,
                                                            format::ResourceValueType::kGpuDescriptorHandle };
                        std::memcpy(rv.value.data(),
                                    &gpu_desc_ptr,
                                    GetResourceValueSize(format::ResourceValueType::kGpuDescriptorHandle));

                        resource_values[index + offset] = std::move(rv);
                        index += sizeof(uint64_t) - stride;
                    }
                }
            }
        }
    }

    memcpy(dst_data, scratch_space_.data(), size);
    // std::shared_lock<std::shared_mutex> shared_write_lock(write_mutex_);
    // std::unique_lock<std::shared_mutex> unique_write_lock;
    if (resource_values.size() > 0)
    {
        // shared_write_lock.unlock();
        // unique_write_lock = std::unique_lock<std::shared_mutex>(write_mutex_);
        manager->AddFillMemoryResourceValueCommand(resource_values);
    }
    D3D12CaptureManager::Get()->WriteFillMemoryCmd(memory_id, offset, size, scratch_space_.data());
}

GFXRECON_END_NAMESPACE(encode)
GFXRECON_END_NAMESPACE(gfxrecon)
