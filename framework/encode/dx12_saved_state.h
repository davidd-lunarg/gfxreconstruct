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
    std::shared_ptr<const DxWrapperInfo> saved_object_info{ nullptr };
    void*                                extra_saved_state{ nullptr };
};

class Dx12SavedState
{
  public:
    void SaveStateTable(const Dx12StateTable& state_table) { saved_state_table_ = state_table; }

    // Combines the wrappers of type Wrapper in state_table with those in saved_state_table_ and visits each unique
    // wrapper.
    template <typename Wrapper>
    void VisitWrappersForReset(const Dx12StateTable&                                    current_state_table,
                               std::function<void(format::HandleId id, const Wrapper*)> visitor)
    {
        temp_combined_wrappers.clear();

        saved_state_table_.VisitWrappers([&](format::HandleId id, const Wrapper* wrapper) {
            // Wrappers in the saved state table may no longer be valid. Mark as nullptr here and only set when visiting
            // the wrappers in the current state table.
            temp_combined_wrappers[id] = nullptr;
        });
        current_state_table.VisitWrappers([&](format::HandleId id, const Wrapper* wrapper) {
            temp_combined_wrappers[id] = reinterpret_cast<const void*>(wrapper);
        });

        for (const auto& id_wrapper_pair : temp_combined_wrappers)
        {
            visitor(id_wrapper_pair.first, reinterpret_cast<const Wrapper*>(id_wrapper_pair.second));
        }
    }

    const Dx12StateTable& GetSavedStateTable() { return saved_state_table_; }

    void SaveObjectState(format::HandleId                     object_id,
                         unsigned long                        ref_count,
                         std::shared_ptr<const DxWrapperInfo> wrapper_info)
    {
        saved_object_states_[object_id] = Dx12SavedObjectState{ ref_count, wrapper_info, nullptr };
    }

    const Dx12SavedObjectState* GetSavedObjectState(format::HandleId object_id) const
    {
        return saved_object_states_.count(object_id) > 0 ? &saved_object_states_.at(object_id) : nullptr;
    }

  private:
    Dx12StateTable                                   saved_state_table_;
    Dx12StateTable                                   combined_state_table_;
    std::map<format::HandleId, Dx12SavedObjectState> saved_object_states_;

    std::map<format::HandleId, const void*> temp_combined_wrappers;
};

GFXRECON_END_NAMESPACE(encode)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif // GFXRECON_GENERATED_DX12_SAVED_STATE_H
