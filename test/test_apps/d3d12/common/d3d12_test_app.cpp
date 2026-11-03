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

#include "d3d12_test_app.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace gfxrecon
{
namespace testapp
{

namespace
{

constexpr uint32_t    kDescriptorHeapSize = 64;
constexpr uint32_t    kBallastDescriptors = 4096;
constexpr uint32_t    kOutputRowPitch     = D3d12TestApp::kWidth * 4; // a multiple of 256, so one footprint fits
constexpr DXGI_FORMAT kOutputFormat       = DXGI_FORMAT_R8G8B8A8_UNORM;
constexpr float       kEmptyColor[4]      = { 128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f };

bool quit_requested = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
        case WM_CLOSE:
            quit_requested = true;
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(hwnd, message, wparam, lparam);
    }
}

void PumpMessages()
{
    MSG msg = {};
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

D3D12_RESOURCE_BARRIER
TransitionBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource   = resource;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = before;
    barrier.Transition.StateAfter  = after;
    return barrier;
}

D3D12_TEXTURE_COPY_LOCATION OutputFootprint(ID3D12Resource* buffer)
{
    D3D12_TEXTURE_COPY_LOCATION location        = {};
    location.pResource                          = buffer;
    location.Type                               = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    location.PlacedFootprint.Offset             = 0;
    location.PlacedFootprint.Footprint.Format   = kOutputFormat;
    location.PlacedFootprint.Footprint.Width    = D3d12TestApp::kWidth;
    location.PlacedFootprint.Footprint.Height   = D3d12TestApp::kHeight;
    location.PlacedFootprint.Footprint.Depth    = 1;
    location.PlacedFootprint.Footprint.RowPitch = kOutputRowPitch;
    return location;
}

D3D12_TEXTURE_COPY_LOCATION OutputSubresource(ID3D12Resource* texture)
{
    D3D12_TEXTURE_COPY_LOCATION location = {};
    location.pResource                   = texture;
    location.Type                        = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    location.SubresourceIndex            = 0;
    return location;
}

} // namespace

void ShaderTable::AddRecord(const void* identifier, const void* arguments, size_t arguments_size)
{
    size_t start = bytes_.size();
    bytes_.resize(start + stride_, 0);
    std::memcpy(bytes_.data() + start, identifier, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
    if ((arguments != nullptr) && (arguments_size > 0))
    {
        std::memcpy(bytes_.data() + start + D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES, arguments, arguments_size);
    }
}

int D3d12TestApp::Run(int argc, char** argv)
{
    std::vector<std::string> env_args;
    const char*              env = std::getenv("GFXRECON_TESTAPP_ARGS");
    if (env != nullptr)
    {
        for (const char* p = env; *p != '\0';)
        {
            const char* end = p;
            while ((*end != '\0') && (*end != ' '))
            {
                ++end;
            }
            if (end > p)
            {
                env_args.emplace_back(p, end);
            }
            p = (*end == ' ') ? end + 1 : end;
        }
    }
    std::vector<char*> all_args(argv, argv + argc);
    for (std::string& arg : env_args)
    {
        all_args.push_back(&arg[0]);
    }
    ParseOptions(static_cast<int>(all_args.size()), all_args.data());
    for (const std::string& name : options_.cases)
    {
        bool known = false;
        for (uint32_t tile = 0; (tile < TileCount()) && !known; ++tile)
        {
            known = (name == TileName(tile));
        }
        if (!known)
        {
            printf("%s: unknown tile %s\n", Name(), name.c_str());
            fflush(stdout);
            ExitProcess(kExitUsage);
        }
    }
    CreateWindowAndDevice();

    if (options_.ballast_mb > 0)
    {
        ballast_buffer_                      = CreateBuffer(uint64_t(options_.ballast_mb) << 20,
                                       D3D12_HEAP_TYPE_DEFAULT,
                                       D3D12_RESOURCE_STATE_COMMON,
                                       D3D12_RESOURCE_FLAG_NONE,
                                       L"ballast buffer");
        D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
        heap_desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        heap_desc.NumDescriptors             = kBallastDescriptors;
        heap_desc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        Check(device_->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&ballast_heap_)),
              "CreateDescriptorHeap (ballast)");
    }

    CreateFrameResources();
    Initialize();
    ExecuteAndWait();
    init_uploads_.clear();
    ballast_buffer_.Reset();
    ballast_heap_.Reset();
    Check(list_->Close(), "Close (init)");

    // The last frame, the --frames'th or the first after the window was closed, is read back and judged.
    uint32_t frame = 0;
    bool     last  = false;
    while (!last)
    {
        PumpMessages();
        last          = quit_requested || ((options_.frames != 0) && (frame + 1 == options_.frames));
        uint32_t slot = frame % kFramesInFlight;
        if (fence_->GetCompletedValue() < slot_fence_values_[slot])
        {
            Check(fence_->SetEventOnCompletion(slot_fence_values_[slot], fence_event_), "SetEventOnCompletion");
            WaitForSingleObject(fence_event_, INFINITE);
        }
        Check(allocators_[slot]->Reset(), "CommandAllocator::Reset");
        Check(list_->Reset(allocators_[slot].Get(), nullptr), "CommandList::Reset");
        ID3D12DescriptorHeap* heaps[] = { descriptor_heap_.Get() };
        list_->SetDescriptorHeaps(1, heaps);
        ClearOutput();
        RecordFrame(frame, slot);
        PresentFrame(slot, last);
        ++frame;
    }

    Check(queue_->Signal(fence_.Get(), ++fence_value_), "Signal (idle)");
    Check(fence_->SetEventOnCompletion(fence_value_, fence_event_), "SetEventOnCompletion (idle)");
    WaitForSingleObject(fence_event_, INFINITE);

    int failures = Report();
    if (options_.debug_layer)
    {
        ReportDebugMessages();
    }
    printf("%s: rendered %u frames\n", Name(), frame);
    fflush(stdout);
    CloseHandle(fence_event_);
    DestroyWindow(hwnd_);
    return failures;
}

void D3d12TestApp::ParseOptions(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i)
    {
        // A value comes as "--opt value" or "--opt=value" (a value from the environment cannot contain a space);
        // take() consumes the next argument unless the value was joined.
        std::string arg(argv[i]);
        std::string joined;
        const char* value  = (i + 1 < argc) ? argv[i + 1] : nullptr;
        size_t      equals = arg.find('=');
        if (equals != std::string::npos)
        {
            joined = arg.substr(equals + 1);
            arg.resize(equals);
            value = joined.c_str();
        }
        auto take = [&]() {
            if (equals == std::string::npos)
            {
                ++i;
            }
            return value;
        };
        if (arg == "--warp")
        {
            options_.warp = true;
        }
        else if ((arg == "--frames") && (value != nullptr))
        {
            options_.frames = static_cast<uint32_t>(strtoul(take(), nullptr, 10));
        }
        else if ((arg == "--case") && (value != nullptr))
        {
            std::string list = take();
            size_t      pos  = 0;
            while (pos <= list.size())
            {
                size_t comma = list.find(',', pos);
                if (comma == std::string::npos)
                {
                    comma = list.size();
                }
                if (comma > pos)
                {
                    options_.cases.push_back(list.substr(pos, comma - pos));
                }
                pos = comma + 1;
            }
        }
        else if (arg == "--debug-layer")
        {
            options_.debug_layer = true;
        }
        else if ((arg == "--ballast-mb") && (value != nullptr))
        {
            options_.ballast_mb = static_cast<uint32_t>(strtoul(take(), nullptr, 10));
        }
        else if (arg == "--no-vsync")
        {
            options_.vsync = false;
        }
        else
        {
            printf("%s: unknown option %s\n", Name(), argv[i]);
            printf("usage: [--warp] [--frames N] [--case a,b,...] [--debug-layer] [--ballast-mb N] [--no-vsync]\n");
            fflush(stdout);
            ExitProcess(kExitUsage);
        }
    }
}

bool D3d12TestApp::TileEnabled(uint32_t tile) const
{
    if ((tile >= TileCount()) || !TileBuilt(tile))
    {
        return false;
    }
    if (options_.cases.empty())
    {
        return true;
    }
    for (const std::string& name : options_.cases)
    {
        if (name == TileName(tile))
        {
            return true;
        }
    }
    return false;
}

void D3d12TestApp::Check(HRESULT result, const char* what) const
{
    if (FAILED(result))
    {
        printf("%s: FAIL %s returned 0x%08lx\n", Name(), what, static_cast<unsigned long>(result));
        fflush(stdout);
        ExitProcess(kExitFailure);
    }
}

void D3d12TestApp::CreateWindowAndDevice()
{
    std::wstring class_name = L"GfxrTestApp-";
    for (const char* c = Name(); *c != '\0'; ++c)
    {
        class_name.push_back(static_cast<wchar_t>(*c));
    }
    WNDCLASSEXW window_class   = {};
    window_class.cbSize        = sizeof(window_class);
    window_class.style         = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc   = WindowProc;
    window_class.hInstance     = GetModuleHandleW(nullptr);
    window_class.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    window_class.lpszClassName = class_name.c_str();
    RegisterClassExW(&window_class);

    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT  rect  = { 0, 0, static_cast<LONG>(kWidth), static_cast<LONG>(kHeight) };
    AdjustWindowRect(&rect, style, FALSE);
    hwnd_ = CreateWindowExW(0,
                            class_name.c_str(),
                            WindowTitle(),
                            style,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            rect.right - rect.left,
                            rect.bottom - rect.top,
                            nullptr,
                            nullptr,
                            window_class.hInstance,
                            nullptr);
    if (hwnd_ == nullptr)
    {
        Check(HRESULT_FROM_WIN32(GetLastError()), "CreateWindowExW");
    }
    ShowWindow(hwnd_, SW_SHOW);

    UINT factory_flags = 0;
    if (options_.debug_layer)
    {
        ComPtr<ID3D12Debug> debug;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
        {
            debug->EnableDebugLayer();
            factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
            printf("%s: debug layer on\n", Name());
        }
        else
        {
            printf("%s: debug layer unavailable\n", Name());
        }
    }
    Check(CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&factory_)), "CreateDXGIFactory2");

    ComPtr<IDXGIAdapter1> adapter;
    if (options_.warp)
    {
        Check(factory_->EnumWarpAdapter(IID_PPV_ARGS(&adapter)), "EnumWarpAdapter");
        Check(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device_)),
              "D3D12CreateDevice (WARP)");
    }
    else
    {
        // The first adapter, in performance order, that makes a raytracing tier 1.1 device.
        for (UINT i = 0; SUCCEEDED(
                 factory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)));
             ++i)
        {
            ComPtr<ID3D12Device5> candidate;
            if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&candidate))))
            {
                continue;
            }
            D3D12_FEATURE_DATA_D3D12_OPTIONS5 options5 = {};
            if (SUCCEEDED(candidate->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options5, sizeof(options5))) &&
                (options5.RaytracingTier >= D3D12_RAYTRACING_TIER_1_1))
            {
                device_ = candidate;
                break;
            }
        }
        if (device_ == nullptr)
        {
            printf("%s: FAIL no adapter with raytracing tier 1.1\n", Name());
            fflush(stdout);
            ExitProcess(kExitFailure);
        }
    }
    DXGI_ADAPTER_DESC1 adapter_desc = {};
    adapter->GetDesc1(&adapter_desc);
    printf("%s: adapter %ls\n", Name(), adapter_desc.Description);
    fflush(stdout);

    if (options_.debug_layer)
    {
        // Store warnings and errors only; ReportDebugMessages() prints them at exit.
        ComPtr<ID3D12InfoQueue> info_queue;
        if (SUCCEEDED(device_.As(&info_queue)))
        {
            D3D12_MESSAGE_SEVERITY  denied[] = { D3D12_MESSAGE_SEVERITY_INFO, D3D12_MESSAGE_SEVERITY_MESSAGE };
            D3D12_INFO_QUEUE_FILTER filter   = {};
            filter.DenyList.NumSeverities    = 2;
            filter.DenyList.pSeverityList    = denied;
            info_queue->PushStorageFilter(&filter);
        }
    }

    D3D12_COMMAND_QUEUE_DESC queue_desc = {};
    queue_desc.Type                     = D3D12_COMMAND_LIST_TYPE_DIRECT;
    Check(device_->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&queue_)), "CreateCommandQueue");
    Check(device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)), "CreateFence");
    fence_event_ = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    if (fence_event_ == nullptr)
    {
        Check(HRESULT_FROM_WIN32(GetLastError()), "CreateEventW");
    }

    DXGI_SWAP_CHAIN_DESC1 swapchain_desc = {};
    swapchain_desc.Width                 = kWidth;
    swapchain_desc.Height                = kHeight;
    swapchain_desc.Format                = kOutputFormat;
    swapchain_desc.SampleDesc.Count      = 1;
    swapchain_desc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_desc.BufferCount           = kFramesInFlight;
    swapchain_desc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    ComPtr<IDXGISwapChain1> swapchain1;
    Check(factory_->CreateSwapChainForHwnd(queue_.Get(), hwnd_, &swapchain_desc, nullptr, nullptr, &swapchain1),
          "CreateSwapChainForHwnd");
    Check(factory_->MakeWindowAssociation(hwnd_, DXGI_MWA_NO_ALT_ENTER), "MakeWindowAssociation");
    Check(swapchain1.As(&swapchain_), "QueryInterface IDXGISwapChain3");
    for (uint32_t i = 0; i < kFramesInFlight; ++i)
    {
        Check(swapchain_->GetBuffer(i, IID_PPV_ARGS(&back_buffers_[i])), "GetBuffer");
        Check(device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocators_[i])),
              "CreateCommandAllocator");
    }
    // Open: Initialize() records its uploads and builds on it.
    Check(device_->CreateCommandList(
              0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocators_[0].Get(), nullptr, IID_PPV_ARGS(&list_)),
          "CreateCommandList");
}

void D3d12TestApp::CreateFrameResources()
{
    D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
    heap_desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heap_desc.NumDescriptors             = kDescriptorHeapSize;
    heap_desc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    Check(device_->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&descriptor_heap_)), "CreateDescriptorHeap");
    descriptor_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_HEAP_PROPERTIES heap_props = {};
    heap_props.Type                  = D3D12_HEAP_TYPE_DEFAULT;
    D3D12_RESOURCE_DESC desc         = {};
    desc.Dimension                   = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Width                       = kWidth;
    desc.Height                      = kHeight;
    desc.DepthOrArraySize            = 1;
    desc.MipLevels                   = 1;
    desc.Format                      = kOutputFormat;
    desc.SampleDesc.Count            = 1;
    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS | D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    D3D12_CLEAR_VALUE clear_value = {};
    clear_value.Format            = kOutputFormat;
    Check(device_->CreateCommittedResource(&heap_props,
                                           D3D12_HEAP_FLAG_NONE,
                                           &desc,
                                           D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
                                           &clear_value,
                                           IID_PPV_ARGS(&output_)),
          "CreateCommittedResource (output)");
    output_->SetName(L"output");

    D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
    rtv_heap_desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtv_heap_desc.NumDescriptors             = 1;
    Check(device_->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&rtv_heap_)), "CreateDescriptorHeap (RTV)");
    output_rtv_ = rtv_heap_->GetCPUDescriptorHandleForHeapStart();
    device_->CreateRenderTargetView(output_.Get(), nullptr, output_rtv_);

    output_uav_                          = AllocateDescriptor();
    D3D12_UNORDERED_ACCESS_VIEW_DESC uav = {};
    uav.Format                           = kOutputFormat;
    uav.ViewDimension                    = D3D12_UAV_DIMENSION_TEXTURE2D;
    device_->CreateUnorderedAccessView(output_.Get(), nullptr, &uav, output_uav_.cpu);

    // ClearUnorderedAccessViewFloat also takes the UAV in a heap that is not shader visible: slot 0 is the output's,
    // slot 1 is rewritten by ClearTexture().
    D3D12_DESCRIPTOR_HEAP_DESC clear_heap_desc = {};
    clear_heap_desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    clear_heap_desc.NumDescriptors             = 2;
    Check(device_->CreateDescriptorHeap(&clear_heap_desc, IID_PPV_ARGS(&clear_heap_)), "CreateDescriptorHeap (clear)");
    output_clear_uav_ = clear_heap_->GetCPUDescriptorHandleForHeapStart();
    device_->CreateUnorderedAccessView(output_.Get(), nullptr, &uav, output_clear_uav_);

    readback_ = CreateBuffer(uint64_t(kOutputRowPitch) * kHeight,
                             D3D12_HEAP_TYPE_READBACK,
                             D3D12_RESOURCE_STATE_COPY_DEST,
                             D3D12_RESOURCE_FLAG_NONE,
                             L"readback");
}

// Clears the output to all zero, the "not run" color, and the slots of the tiles that do not run to gray, and leaves it
// in UAV state.
void D3d12TestApp::ClearOutput()
{
    const float zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    Transition(output_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_RENDER_TARGET);
    list_->ClearRenderTargetView(output_rtv_, zero, 0, nullptr);
    Transition(output_.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

    // A render target clear to a color other than the optimized clear value draws a debug layer warning, so the gray
    // goes through the UAV.
    D3D12_RECT empty[kMaxTiles];
    UINT       empty_count = 0;
    for (uint32_t tile = 0; tile < kMaxTiles; ++tile)
    {
        if (!TileEnabled(tile))
        {
            LONG x               = static_cast<LONG>(TileX(tile));
            LONG y               = static_cast<LONG>(TileY(tile));
            empty[empty_count++] = { x, y, x + static_cast<LONG>(kTileSize), y + static_cast<LONG>(kTileSize) };
        }
    }
    if (empty_count > 0)
    {
        list_->ClearUnorderedAccessViewFloat(
            output_uav_.gpu, output_clear_uav_, output_.Get(), kEmptyColor, empty_count, empty);
    }
}

void D3d12TestApp::ClearTexture(ID3D12Resource* texture, const DescriptorHandle& uav)
{
    D3D12_UNORDERED_ACCESS_VIEW_DESC desc = {};
    desc.Format                           = kOutputFormat;
    desc.ViewDimension                    = D3D12_UAV_DIMENSION_TEXTURE2D;
    D3D12_CPU_DESCRIPTOR_HANDLE clear_uav = output_clear_uav_;
    clear_uav.ptr += descriptor_size_;
    device_->CreateUnorderedAccessView(texture, nullptr, &desc, clear_uav);
    const float zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    list_->ClearUnorderedAccessViewFloat(uav.gpu, clear_uav, texture, zero, 0, nullptr);
}

void D3d12TestApp::PresentFrame(uint32_t slot, bool readback)
{
    ID3D12Resource*        back       = back_buffers_[swapchain_->GetCurrentBackBufferIndex()].Get();
    D3D12_RESOURCE_BARRIER to_copy[2] = {
        TransitionBarrier(output_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE),
        TransitionBarrier(back, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_COPY_DEST),
    };
    list_->ResourceBarrier(2, to_copy);
    list_->CopyResource(back, output_.Get());
    if (readback)
    {
        D3D12_TEXTURE_COPY_LOCATION dst = OutputFootprint(readback_.Get());
        D3D12_TEXTURE_COPY_LOCATION src = OutputSubresource(output_.Get());
        list_->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
    }
    D3D12_RESOURCE_BARRIER from_copy[2] = {
        TransitionBarrier(output_.Get(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS),
        TransitionBarrier(back, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT),
    };
    list_->ResourceBarrier(2, from_copy);
    Check(list_->Close(), "Close");

    std::vector<ID3D12CommandList*> lists = pre_lists_;
    lists.push_back(list_.Get());
    pre_lists_.clear();
    for (const Wait& wait : pre_waits_)
    {
        Check(queue_->Wait(wait.first, wait.second), "Wait (WaitBefore)");
    }
    pre_waits_.clear();
    queue_->ExecuteCommandLists(static_cast<UINT>(lists.size()), lists.data());
    Check(swapchain_->Present(options_.vsync ? 1 : 0, 0), "Present");
    Check(queue_->Signal(fence_.Get(), ++fence_value_), "Signal");
    slot_fence_values_[slot] = fence_value_;
}

// Classifies every tile of the last frame by RGB: pass needs every pixel pass-green; any fail pixel fails the tile with
// the OR of the pixels' bits; all-black is "not run"; anything else is "mixed". A tile left out of the build is "not
// built" and one --case left out is "disabled"; neither counts as failing.
int D3d12TestApp::Report()
{
    uint8_t*    pixels = nullptr;
    D3D12_RANGE range  = { 0, size_t(kOutputRowPitch) * kHeight };
    Check(readback_->Map(0, &range, reinterpret_cast<void**>(&pixels)), "Map (readback)");
    int failures = 0;
    for (uint32_t tile = 0; tile < kMaxTiles; ++tile)
    {
        if (tile >= TileCount())
        {
            break;
        }
        if (!TileBuilt(tile))
        {
            printf("%s: tile %u %s: not built\n", Name(), tile, TileName(tile));
            continue;
        }
        if (!TileEnabled(tile))
        {
            printf("%s: tile %u %s: disabled\n", Name(), tile, TileName(tile));
            continue;
        }
        uint32_t pass = 0, fail = 0, zero = 0, bits = 0;
        for (uint32_t y = 0; y < kTileSize; ++y)
        {
            const uint8_t* row = pixels + size_t(TileY(tile) + y) * kOutputRowPitch + size_t(TileX(tile)) * 4;
            for (uint32_t x = 0; x < kTileSize; ++x)
            {
                const uint8_t* p = row + x * 4;
                if ((p[0] == 0) && (p[1] == 255) && (p[2] == 0))
                {
                    ++pass;
                }
                else if ((p[0] == 255) && (p[1] == 0))
                {
                    ++fail;
                    bits |= p[2] / 8;
                }
                else if ((p[0] == 0) && (p[1] == 0) && (p[2] == 0))
                {
                    ++zero;
                }
            }
        }
        const uint32_t total   = kTileSize * kTileSize;
        const char*    verdict = (pass == total) ? "pass" : (fail > 0) ? "fail" : (zero == total) ? "not run" : "mixed";
        if (pass != total)
        {
            ++failures;
        }
        if (fail > 0)
        {
            printf("%s: tile %u %s: %s bits=0x%02x\n", Name(), tile, TileName(tile), verdict, bits);
        }
        else
        {
            printf("%s: tile %u %s: %s\n", Name(), tile, TileName(tile), verdict);
        }
    }
    D3D12_RANGE no_write = { 0, 0 };
    readback_->Unmap(0, &no_write);
    printf("%s: %d failing tile(s)\n", Name(), failures);
    fflush(stdout);
    return failures;
}

// Prints what the debug layer stored (--debug-layer), so a run without a debugger shows its warnings and errors.
void D3d12TestApp::ReportDebugMessages()
{
    ComPtr<ID3D12InfoQueue> info_queue;
    if (FAILED(device_.As(&info_queue)))
    {
        printf("%s: no ID3D12InfoQueue\n", Name());
        return;
    }
    const UINT64         count = info_queue->GetNumStoredMessages();
    std::vector<uint8_t> bytes;
    for (UINT64 i = 0; i < count; ++i)
    {
        SIZE_T length = 0;
        info_queue->GetMessage(i, nullptr, &length);
        bytes.resize(length);
        D3D12_MESSAGE* message = reinterpret_cast<D3D12_MESSAGE*>(bytes.data());
        if (SUCCEEDED(info_queue->GetMessage(i, message, &length)))
        {
            const char* severity = (message->Severity == D3D12_MESSAGE_SEVERITY_CORRUPTION) ? "CORRUPTION"
                                   : (message->Severity == D3D12_MESSAGE_SEVERITY_ERROR)    ? "ERROR"
                                   : (message->Severity == D3D12_MESSAGE_SEVERITY_WARNING)  ? "WARNING"
                                                                                            : "INFO";
            printf("%s: D3D12 %s %d: %s\n", Name(), severity, static_cast<int>(message->ID), message->pDescription);
        }
    }
    printf("%s: %llu debug layer message(s)\n", Name(), static_cast<unsigned long long>(count));
    fflush(stdout);
}

DescriptorHandle D3d12TestApp::AllocateDescriptor()
{
    if (descriptors_used_ >= kDescriptorHeapSize)
    {
        printf("%s: FAIL descriptor heap full\n", Name());
        fflush(stdout);
        ExitProcess(kExitFailure);
    }
    DescriptorHandle handle;
    handle.index = descriptors_used_++;
    handle.cpu   = descriptor_heap_->GetCPUDescriptorHandleForHeapStart();
    handle.cpu.ptr += SIZE_T(handle.index) * descriptor_size_;
    handle.gpu = descriptor_heap_->GetGPUDescriptorHandleForHeapStart();
    handle.gpu.ptr += UINT64(handle.index) * descriptor_size_;
    return handle;
}

ComPtr<ID3D12Resource> D3d12TestApp::CreateBuffer(uint64_t              size,
                                                  D3D12_HEAP_TYPE       heap_type,
                                                  D3D12_RESOURCE_STATES state,
                                                  D3D12_RESOURCE_FLAGS  flags,
                                                  const wchar_t*        name)
{
    D3D12_HEAP_PROPERTIES heap_props = {};
    heap_props.Type                  = heap_type;
    D3D12_RESOURCE_DESC desc         = {};
    desc.Dimension                   = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Width                       = size;
    desc.Height                      = 1;
    desc.DepthOrArraySize            = 1;
    desc.MipLevels                   = 1;
    desc.SampleDesc.Count            = 1;
    desc.Layout                      = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags                       = flags;
    // A buffer starts in COMMON whatever is asked, in every heap (the debug layer says so, ID 1328), and is promoted
    // by its first use; the one state that must be named at creation is RAYTRACING_ACCELERATION_STRUCTURE.
    if (state != D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE)
    {
        state = D3D12_RESOURCE_STATE_COMMON;
    }
    ComPtr<ID3D12Resource> buffer;
    Check(device_->CreateCommittedResource(
              &heap_props, D3D12_HEAP_FLAG_NONE, &desc, state, nullptr, IID_PPV_ARGS(&buffer)),
          "CreateCommittedResource (buffer)");
    buffer->SetName(name);
    return buffer;
}

ComPtr<ID3D12Resource> D3d12TestApp::CreateUploadBuffer(const void* data, uint64_t size, const wchar_t* name)
{
    ComPtr<ID3D12Resource> buffer =
        CreateBuffer(size, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_FLAG_NONE, name);
    if (data != nullptr)
    {
        void*       mapped  = nullptr;
        D3D12_RANGE no_read = { 0, 0 };
        Check(buffer->Map(0, &no_read, &mapped), "Map (upload)");
        std::memcpy(mapped, data, size);
        buffer->Unmap(0, nullptr);
    }
    return buffer;
}

ComPtr<ID3D12Resource> D3d12TestApp::CreateDefaultBuffer(
    const void* data, uint64_t size, D3D12_RESOURCE_STATES state, D3D12_RESOURCE_FLAGS flags, const wchar_t* name)
{
    ComPtr<ID3D12Resource> buffer =
        CreateBuffer(size, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COPY_DEST, flags, name);
    ComPtr<ID3D12Resource> upload = CreateUploadBuffer(data, size, name);
    list_->CopyBufferRegion(buffer.Get(), 0, upload.Get(), 0, size);
    Transition(buffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, state);
    init_uploads_.push_back(upload);
    return buffer;
}

ComPtr<ID3D12RootSignature> D3d12TestApp::CreateRootSignature(const D3D12_ROOT_SIGNATURE_DESC& desc)
{
    ComPtr<ID3DBlob> blob;
    ComPtr<ID3DBlob> error;
    HRESULT          result = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &blob, &error);
    if (FAILED(result))
    {
        printf("%s: root signature: %s\n",
               Name(),
               error ? static_cast<const char*>(error->GetBufferPointer()) : "no message");
        Check(result, "D3D12SerializeRootSignature");
    }
    ComPtr<ID3D12RootSignature> root_signature;
    Check(
        device_->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&root_signature)),
        "CreateRootSignature");
    return root_signature;
}

void D3d12TestApp::Transition(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
    D3D12_RESOURCE_BARRIER barrier = TransitionBarrier(resource, before, after);
    list_->ResourceBarrier(1, &barrier);
}

void D3d12TestApp::UavBarrier(ID3D12Resource* resource)
{
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_UAV;
    barrier.UAV.pResource          = resource;
    list_->ResourceBarrier(1, &barrier);
}

AccelerationStructure
D3d12TestApp::CreateAccelerationStructure(const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& inputs,
                                          const wchar_t*                                              name)
{
    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info = {};
    device_->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &info);
    if (info.ResultDataMaxSizeInBytes == 0)
    {
        printf("%s: FAIL prebuild info for %ls\n", Name(), name);
        fflush(stdout);
        ExitProcess(kExitFailure);
    }
    AccelerationStructure as;
    as.result = CreateBuffer(info.ResultDataMaxSizeInBytes,
                             D3D12_HEAP_TYPE_DEFAULT,
                             D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
                             D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
                             name);
    // An update may need more scratch than the build (UpdateScratchDataSizeInBytes is 0 without ALLOW_UPDATE).
    uint64_t scratch_bytes = (info.UpdateScratchDataSizeInBytes > info.ScratchDataSizeInBytes)
                                 ? info.UpdateScratchDataSizeInBytes
                                 : info.ScratchDataSizeInBytes;
    as.scratch             = CreateBuffer(scratch_bytes,
                              D3D12_HEAP_TYPE_DEFAULT,
                              D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
                              D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
                              L"scratch");
    return as;
}

void D3d12TestApp::BuildAccelerationStructure(const AccelerationStructure&                                as,
                                              const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& inputs,
                                              D3D12_GPU_VIRTUAL_ADDRESS                                   source)
{
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc = {};
    desc.Inputs                                             = inputs;
    desc.DestAccelerationStructureData                      = as.result->GetGPUVirtualAddress();
    desc.SourceAccelerationStructureData                    = source;
    desc.ScratchAccelerationStructureData                   = as.scratch->GetGPUVirtualAddress();
    list_->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
    UavBarrier(as.result.Get());
}

void D3d12TestApp::ExecuteAndWait()
{
    Check(list_->Close(), "Close (ExecuteAndWait)");
    ID3D12CommandList* lists[] = { list_.Get() };
    queue_->ExecuteCommandLists(1, lists);
    Check(queue_->Signal(fence_.Get(), ++fence_value_), "Signal (ExecuteAndWait)");
    Check(fence_->SetEventOnCompletion(fence_value_, fence_event_), "SetEventOnCompletion (ExecuteAndWait)");
    WaitForSingleObject(fence_event_, INFINITE);
    Check(allocators_[0]->Reset(), "CommandAllocator::Reset (ExecuteAndWait)");
    Check(list_->Reset(allocators_[0].Get(), nullptr), "CommandList::Reset (ExecuteAndWait)");
}

} // namespace testapp
} // namespace gfxrecon
