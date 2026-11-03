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

#ifndef GFXRECON_TESTAPP_D3D12_TEST_APP_H
#define GFXRECON_TESTAPP_D3D12_TEST_APP_H

// The frame of the D3D12 test apps: a window, a DXR-capable device, a 512x256 output image split into a 4x2 grid of
// 128x128 tiles, and a verdict per tile. Each app draws one tile per variant under test; a tile's shader checks what
// it read through the addresses under test and writes pass green or a fail color carrying a bit mask, so any replay
// (whichever translation mode, hardware or WARP) judges itself from a screenshot, and the app itself judges its
// last frame on the CPU, prints a line per tile and returns the failing tile count.
//
// Options: --warp, --frames N (0 = until the window closes), --case a,b (tiles to run, default all),
// --debug-layer (its messages printed at exit), --ballast-mb N (default 64, 0 = off), --no-vsync. A value may be
// joined (--case=a,b); GFXRECON_TESTAPP_ARGS in the environment adds options for a launcher that cannot pass them.
//
// Ballast: before any test resource exists the app creates a buffer and a shader-visible descriptor heap and
// releases them once the test resources are created (still frame 0). A trim taken later never recreates them, so
// on replay the test resources' GPU addresses and descriptor handles land elsewhere than at capture and an
// untranslated value reads the wrong memory or faults.
//
// Every frame starts by clearing the output to black. A trim's state block seeds the output with the captured
// image, so without the clear a tile whose work silently does nothing at replay would keep its captured verdict. The
// clear paints the slots of the tiles that do not run gray, so black is only ever a tile that ran and drew nothing.
//
// Tiles whose DXR addresses are written on the GPU are compiled only with GFXRECON_TESTAPP_GPU_WRITTEN_TILES (CMake
// option GFXRECON_D3D12_TESTAPP_GPU_WRITTEN_TILES): replayed without a translation that follows GPU writes, they hand
// the GPU capture-time addresses, which can hang it. Tiles that reproduce known replay defects are compiled only with
// GFXRECON_TESTAPP_KNOWN_DEFECT_TILES (option GFXRECON_D3D12_TESTAPP_KNOWN_DEFECT_TILES), so a default build passes on
// dev. A tile left out stays gray and is reported "not built".
//
// Verdict colors: pass (0, 255, 0, 255); fail (255, 0, bits * 8, 255) with bits a 5-bit mask the app documents;
// not run (0, 0, 0, 0); no tile in the slot this run (128, 128, 128, 255): past the app's last tile, not built, or
// left out by --case.

#include "util/defines.h"

#include <windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace gfxrecon
{
namespace testapp
{

using Microsoft::WRL::ComPtr;

struct Options
{
    bool                     warp{ false };
    uint32_t                 frames{ 0 };
    std::vector<std::string> cases;
    bool                     debug_layer{ false };
    uint32_t                 ballast_mb{ 64 };
    bool                     vsync{ true };
};

struct DescriptorHandle
{
    D3D12_CPU_DESCRIPTOR_HANDLE cpu{};
    D3D12_GPU_DESCRIPTOR_HANDLE gpu{};
    uint32_t                    index{ 0 };
};

struct AccelerationStructure
{
    ComPtr<ID3D12Resource> result;
    ComPtr<ID3D12Resource> scratch;
};

// Builds the bytes of a shader table: records of a shader identifier followed by local root arguments, each padded
// to the stride (a multiple of 32).
class ShaderTable
{
  public:
    explicit ShaderTable(uint32_t stride) : stride_(stride) {}

    void AddRecord(const void* identifier, const void* arguments = nullptr, size_t arguments_size = 0);

    const std::vector<uint8_t>& Bytes() const { return bytes_; }
    uint32_t                    Stride() const { return stride_; }
    uint32_t                    Size() const { return static_cast<uint32_t>(bytes_.size()); }

  private:
    uint32_t             stride_;
    std::vector<uint8_t> bytes_;
};

struct DxrScene;

class D3d12TestApp
{
  public:
    static constexpr uint32_t kTileSize       = 128;
    static constexpr uint32_t kTilesX         = 4;
    static constexpr uint32_t kTilesY         = 2;
    static constexpr uint32_t kMaxTiles       = kTilesX * kTilesY;
    static constexpr uint32_t kWidth          = kTilesX * kTileSize;
    static constexpr uint32_t kHeight         = kTilesY * kTileSize;
    static constexpr uint32_t kFramesInFlight = 2;
    // Exit codes past any failing tile count: the app itself failed (a FAIL line says what), or an option was bad.
    static constexpr int kExitFailure = 100;
    static constexpr int kExitUsage   = 101;

    using Wait = std::pair<ID3D12Fence*, uint64_t>;

    virtual ~D3d12TestApp() {}

    // Runs the app: options, window, device, Initialize(), the frame loop, the report of the last frame. Returns
    // the failing tile count.
    int Run(int argc, char** argv);

  protected:
    // The app's name, for messages, and its window title.
    virtual const char*    Name() const        = 0;
    virtual const wchar_t* WindowTitle() const = 0;

    // The tiles the app draws, in grid order; --case names them.
    virtual uint32_t    TileCount() const          = 0;
    virtual const char* TileName(uint32_t i) const = 0;
    // Whether tile i is compiled in (GFXRECON_TESTAPP_GPU_WRITTEN_TILES, GFXRECON_TESTAPP_KNOWN_DEFECT_TILES); a tile
    // left out of the build is never enabled and reports "not built".
    virtual bool TileBuilt(uint32_t i) const
    {
        GFXRECON_UNREFERENCED_PARAMETER(i);
        return true;
    }

    // Creates the test resources. The device, the descriptor heap and the output exist; the command list is open
    // for uploads and builds and is executed and waited for afterwards (ExecuteAndWait() does it earlier).
    virtual void Initialize() = 0;

    // Records one frame on the open command list: the descriptor heap is set and the output is cleared, in UAV
    // state. `slot` cycles through kFramesInFlight; resources rewritten by the CPU each frame need one per slot.
    virtual void RecordFrame(uint32_t frame, uint32_t slot) = 0;

    bool     TileEnabled(uint32_t tile) const;
    uint32_t TileX(uint32_t tile) const { return (tile % kTilesX) * kTileSize; }
    uint32_t TileY(uint32_t tile) const { return (tile / kTilesX) * kTileSize; }

    const Options&              GetOptions() const { return options_; }
    ID3D12Device5*              Device() const { return device_.Get(); }
    ID3D12CommandQueue*         Queue() const { return queue_.Get(); }
    ID3D12GraphicsCommandList4* List() const { return list_.Get(); }
    ID3D12DescriptorHeap*       DescriptorHeap() const { return descriptor_heap_.Get(); }
    ID3D12Resource*             Output() const { return output_.Get(); }
    const DescriptorHandle&     OutputUav() const { return output_uav_; }
    // The output as a render target; transition it UNORDERED_ACCESS <-> RENDER_TARGET around a draw.
    D3D12_CPU_DESCRIPTOR_HANDLE OutputRtv() const { return output_rtv_; }

    // A closed command list the app recorded itself, executed before this frame's list in the same
    // ExecuteCommandLists, in the order given; the set is cleared after the submission.
    void SubmitBefore(ID3D12CommandList* list) { pre_lists_.push_back(list); }

    // A fence the queue waits for right before this frame's submission (work the app executed on another queue);
    // the set is cleared after the submission.
    void WaitBefore(ID3D12Fence* fence, uint64_t value) { pre_waits_.push_back({ fence, value }); }

    // The shared quad / TLAS / state object / shader tables (dxr_scene.h), created on the open command list.
    DxrScene CreateDxrScene();

    // One slot of the shader-visible CBV/SRV/UAV heap.
    DescriptorHandle AllocateDescriptor();

    ComPtr<ID3D12Resource> CreateBuffer(uint64_t              size,
                                        D3D12_HEAP_TYPE       heap_type,
                                        D3D12_RESOURCE_STATES state,
                                        D3D12_RESOURCE_FLAGS  flags,
                                        const wchar_t*        name);
    ComPtr<ID3D12Resource> CreateUploadBuffer(const void* data, uint64_t size, const wchar_t* name);
    // A default-heap buffer filled from `data` by a copy on the open command list; `state` is its state after the
    // copy.
    ComPtr<ID3D12Resource> CreateDefaultBuffer(
        const void* data, uint64_t size, D3D12_RESOURCE_STATES state, D3D12_RESOURCE_FLAGS flags, const wchar_t* name);
    ComPtr<ID3D12RootSignature> CreateRootSignature(const D3D12_ROOT_SIGNATURE_DESC& desc);

    void Transition(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
    void UavBarrier(ID3D12Resource* resource = nullptr);
    // Clears a 2D texture of the output's format to black on the open list; it is in UNORDERED_ACCESS state and
    // `uav` is its descriptor in the shader-visible heap. A texture an app draws a tile into and copies from needs
    // the clear every frame for the reason the output does.
    void ClearTexture(ID3D12Resource* texture, const DescriptorHandle& uav);

    // Result and scratch buffers sized by GetRaytracingAccelerationStructurePrebuildInfo (scratch for the build or
    // an update, whichever is larger), the result in RAYTRACING_ACCELERATION_STRUCTURE state.
    AccelerationStructure
    CreateAccelerationStructure(const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& inputs,
                                const wchar_t*                                              name);
    // Records the build into `as` on the open command list, followed by a UAV barrier on the result; `source` is the
    // structure a PERFORM_UPDATE refits (its own result for an update in place).
    void BuildAccelerationStructure(const AccelerationStructure&                                as,
                                    const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& inputs,
                                    D3D12_GPU_VIRTUAL_ADDRESS                                   source = 0);

    // Closes, executes and waits for the open command list, then reopens it. For Initialize().
    void ExecuteAndWait();

    // Prints "<name>: FAIL <what> returned 0x..." and exits with kExitFailure on failure.
    void Check(HRESULT result, const char* what) const;

  private:
    void ParseOptions(int argc, char** argv);
    void CreateWindowAndDevice();
    void CreateFrameResources();
    void ClearOutput();
    void PresentFrame(uint32_t slot, bool readback);
    int  Report();
    void ReportDebugMessages();

    Options                             options_;
    HWND                                hwnd_{ nullptr };
    ComPtr<IDXGIFactory6>               factory_;
    ComPtr<ID3D12Device5>               device_;
    ComPtr<ID3D12CommandQueue>          queue_;
    ComPtr<IDXGISwapChain3>             swapchain_;
    ComPtr<ID3D12Resource>              back_buffers_[kFramesInFlight];
    ComPtr<ID3D12CommandAllocator>      allocators_[kFramesInFlight];
    ComPtr<ID3D12GraphicsCommandList4>  list_;
    ComPtr<ID3D12Fence>                 fence_;
    uint64_t                            fence_value_{ 0 };
    uint64_t                            slot_fence_values_[kFramesInFlight]{};
    HANDLE                              fence_event_{ nullptr };
    ComPtr<ID3D12DescriptorHeap>        descriptor_heap_;
    uint32_t                            descriptor_size_{ 0 };
    uint32_t                            descriptors_used_{ 0 };
    ComPtr<ID3D12Resource>              output_;
    DescriptorHandle                    output_uav_;
    ComPtr<ID3D12DescriptorHeap>        rtv_heap_;
    D3D12_CPU_DESCRIPTOR_HANDLE         output_rtv_{};
    ComPtr<ID3D12DescriptorHeap>        clear_heap_;
    D3D12_CPU_DESCRIPTOR_HANDLE         output_clear_uav_{};
    std::vector<ID3D12CommandList*>     pre_lists_;
    std::vector<Wait>                   pre_waits_;
    ComPtr<ID3D12Resource>              readback_;
    std::vector<ComPtr<ID3D12Resource>> init_uploads_; // kept until the init list completed
    ComPtr<ID3D12Resource>              ballast_buffer_;
    ComPtr<ID3D12DescriptorHeap>        ballast_heap_;
};

} // namespace testapp
} // namespace gfxrecon

// Defines main() and the Agility SDK exports (the runtime next to the exe under D3D12/, as for gfxrecon-replay).
#define GFXRECON_D3D12_TEST_APP_MAIN(AppType)                                        \
    extern "C"                                                                       \
    {                                                                                \
        __declspec(dllexport) extern const UINT D3D12SDKVersion = D3D12_SDK_VERSION; \
        __declspec(dllexport) extern const char* D3D12SDKPath   = ".\\D3D12\\";      \
    }                                                                                \
    int main(int argc, char** argv)                                                  \
    {                                                                                \
        AppType app;                                                                 \
        return app.Run(argc, argv);                                                  \
    }

#endif // GFXRECON_TESTAPP_D3D12_TEST_APP_H
