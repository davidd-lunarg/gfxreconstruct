/*
** Copyright (c) 2023 LunarG, Inc.
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
** This file is generated from dx12_state_table_header_generator.py.
**
*/

#ifndef GFXRECON_GENERATED_DX12_SAVED_STATE_H
#define GFXRECON_GENERATED_DX12_SAVED_STATE_H

#include "generated/generated_dx12_state_table.h"

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(encode)

struct Dx12SavedObjectState
{
    unsigned long                        ref_count{ 0 };
    std::shared_ptr<const DxWrapperInfo> object_info{ nullptr };
};

struct Dx12SavedSwapChainState : Dx12SavedObjectState
{
    DxResizeBuffersInfo resize_info{};
    uint32_t            last_presented_image{ 0 };
};

struct Dx12SavedFenceState : Dx12SavedObjectState
{
    UINT64 completed_value{ 0 };
};

struct Dx12SavedCommandListState : Dx12SavedObjectState
{
    D3D12_COMMAND_LIST_TYPE list_type{ D3D12_COMMAND_LIST_TYPE_NONE };
};

template <typename Wrapper, typename SavedState>
struct Dx12ObjectResetInfo
{
    format::HandleId  id{ format::kNullHandleId };
    Wrapper*          wrapper{ nullptr };
    const SavedState* saved_state{ nullptr };
};

class Dx12SavedState
{
  public:
    void SaveStateTable(const Dx12StateTable& state_table) { saved_state_table_ = state_table; }

    // Combines the wrappers of type Wrapper in state_table with those in saved_state_table_ and visits each unique
    // wrapper.
    template <typename Wrapper, typename SavedState>
    void VisitObjectsForReset(const Dx12StateTable& current_state_table,
                              std::function<void(Dx12ObjectResetInfo<Wrapper, SavedState> reset_info)> visitor)
    {
        temp_combined_wrappers.clear();

        saved_state_table_.VisitWrappers([&](format::HandleId id, Wrapper* wrapper) {
            // Wrappers in the saved state table may no longer be valid. Mark as nullptr here and only set when visiting
            // the wrappers in the current state table.
            temp_combined_wrappers[id] = nullptr;
        });
        current_state_table.VisitWrappers([&](format::HandleId id, Wrapper* wrapper) {
            temp_combined_wrappers[id] = reinterpret_cast<void*>(wrapper);
        });

        for (auto& id_wrapper_pair : temp_combined_wrappers)
        {
            Dx12ObjectResetInfo<Wrapper, SavedState> reset_info;
            reset_info.id          = id_wrapper_pair.first;
            reset_info.wrapper     = reinterpret_cast<Wrapper*>(id_wrapper_pair.second);
            reset_info.saved_state = GetSavedObjectState<SavedState>(id_wrapper_pair.first);
            visitor(reset_info);
        }
    }

    const Dx12StateTable& GetSavedStateTable() { return saved_state_table_; }

    void SaveObjectState(format::HandleId                     object_id,
                         unsigned long                        ref_count,
                         std::shared_ptr<const DxWrapperInfo> wrapper_info)
    {
        saved_object_states_[object_id] =
            std::shared_ptr<Dx12SavedObjectState>(new Dx12SavedObjectState{ ref_count, wrapper_info });
    }

    const Dx12SavedObjectState* GetSavedObjectState(format::HandleId object_id) const
    {
        return saved_object_states_.count(object_id) > 0 ? saved_object_states_.at(object_id).get() : nullptr;
    }

    void SaveSwapChainState(format::HandleId                          swapchain_id,
                            unsigned long                             ref_count,
                            std::shared_ptr<const IDXGISwapChainInfo> swapchain_info,
                            uint32_t                                  last_presented_image)
    {
        const auto&         resize_info       = swapchain_info->resize_info;
        DxResizeBuffersInfo saved_resize_info = DxResizeBuffersInfo();
        if (resize_info.call_id != format::ApiCall_Unknown)
        {
            saved_resize_info =
                DxResizeBuffersInfo{ resize_info.call_id,
                                     std::make_unique<util::MemoryOutputStream>(
                                         util::MemoryOutputStream(resize_info.call_parameters->GetData(),
                                                                  resize_info.call_parameters->GetDataSize())) };
        }
        saved_object_states_[swapchain_id] = std::shared_ptr<Dx12SavedSwapChainState>(new Dx12SavedSwapChainState{
            ref_count, swapchain_info, std::move(saved_resize_info), last_presented_image });
    }

    void SaveFenceState(format::HandleId                       fence_id,
                        unsigned long                          ref_count,
                        std::shared_ptr<const ID3D12FenceInfo> fence_info,
                        UINT64                                 completed_value)
    {
        auto info_copy = std::make_shared<ID3D12FenceInfo>(*fence_info.get());
        saved_object_states_[fence_id] =
            std::shared_ptr<Dx12SavedFenceState>(new Dx12SavedFenceState{ ref_count, info_copy, completed_value });
    }

    void SaveCommandListState(format::HandleId                       id,
                              std::shared_ptr<ID3D12CommandListInfo> list_info,
                              unsigned long                          ref_count,
                              D3D12_COMMAND_LIST_TYPE                list_type)
    {
        auto info_copy           = std::make_shared<ID3D12CommandListInfo>(*list_info.get());
        saved_object_states_[id] = std::shared_ptr<Dx12SavedCommandListState>(
            new Dx12SavedCommandListState{ ref_count, info_copy, list_type });
    }

    template <typename T>
    const T* GetSavedObjectState(format::HandleId list_id)
    {
        auto saved_object_state = GetSavedObjectState(list_id);
        return reinterpret_cast<const T*>(saved_object_state);
    }

  private:
    Dx12StateTable                                                    saved_state_table_;
    Dx12StateTable                                                    combined_state_table_;
    std::map<format::HandleId, std::shared_ptr<Dx12SavedObjectState>> saved_object_states_;

    std::map<format::HandleId, void*> temp_combined_wrappers;
};

GFXRECON_END_NAMESPACE(encode)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif // GFXRECON_GENERATED_DX12_SAVED_STATE_H
