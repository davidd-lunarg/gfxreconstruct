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

// Holds the DXGI objects a D3D12 trim state block has to order around their sources: factories re-obtained through
// IDXGIObject::GetParent after the app dropped its own references (one from the adapter, one from the swap chain), and
// an output from IDXGISwapChain::GetContainingOutput. All three are set up before the first frame and used every
// frame, so a trim taken anywhere in the run (GFXRECON_CAPTURE_FRAMES) must recreate them from their GetParent and
// GetContainingOutput calls.
//
// usage: gfxrecon-testapp-d3d12-dxgi-object-order [--frames N] [--warp]   (default 300 frames, vsync; exits on close)

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

using Microsoft::WRL::ComPtr;

namespace
{

const wchar_t kWindowTitle[] = L"DXGI object order test";
const UINT    kBufferCount   = 2;
const UINT    kWidth         = 1280;
const UINT    kHeight        = 720;

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

void Check(HRESULT result, const char* what)
{
    if (FAILED(result))
    {
        printf("dxgi-object-order: FAIL %s returned 0x%08lx\n", what, static_cast<unsigned long>(result));
        fflush(stdout);
        ExitProcess(1);
    }
}

HWND CreateAppWindow()
{
    WNDCLASSEXW window_class   = {};
    window_class.cbSize        = sizeof(window_class);
    window_class.style         = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc   = WindowProc;
    window_class.hInstance     = GetModuleHandleW(nullptr);
    window_class.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    window_class.lpszClassName = L"DxgiObjectOrderTest";
    RegisterClassExW(&window_class);

    RECT rect = { 0, 0, static_cast<LONG>(kWidth), static_cast<LONG>(kHeight) };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    HWND hwnd = CreateWindowExW(0,
                                window_class.lpszClassName,
                                kWindowTitle,
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                rect.right - rect.left,
                                rect.bottom - rect.top,
                                nullptr,
                                nullptr,
                                window_class.hInstance,
                                nullptr);
    if (hwnd == nullptr)
    {
        Check(HRESULT_FROM_WIN32(GetLastError()), "CreateWindowExW");
    }
    ShowWindow(hwnd, SW_SHOW);
    return hwnd;
}

} // namespace

int main(int argc, char** argv)
{
    // Options from the command line and, for a launcher that cannot pass them, GFXRECON_TESTAPP_ARGS.
    std::vector<std::string> args(argv + 1, argv + argc);
    const char*              env = std::getenv("GFXRECON_TESTAPP_ARGS");
    if (env != nullptr)
    {
        std::istringstream stream(env);
        for (std::string token; stream >> token;)
        {
            args.push_back(token);
        }
    }
    UINT frame_count = 300;
    bool use_warp    = false;
    for (size_t i = 0; i < args.size(); ++i)
    {
        if ((args[i] == "--frames") && (i + 1 < args.size()))
        {
            frame_count = static_cast<UINT>(strtoul(args[++i].c_str(), nullptr, 10));
        }
        else if (args[i] == "--warp")
        {
            use_warp = true;
        }
        else
        {
            printf("dxgi-object-order: unknown option %s\nusage: [--frames N] [--warp]\n", args[i].c_str());
            return 101;
        }
    }

    HWND hwnd = CreateAppWindow();

    // Two root factories, so the adapter's parent and the swap chain's parent are distinct objects: had they been
    // one, the second GetParent would return the first one's wrapper and record nothing.
    ComPtr<IDXGIFactory6> enum_factory;
    Check(CreateDXGIFactory2(0, IID_PPV_ARGS(&enum_factory)), "CreateDXGIFactory2 (adapter)");
    ComPtr<IDXGIAdapter1> adapter;
    if (use_warp)
    {
        Check(enum_factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter)), "EnumWarpAdapter");
    }
    else
    {
        Check(enum_factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)),
              "EnumAdapterByGpuPreference");
    }
    DXGI_ADAPTER_DESC1 adapter_desc = {};
    adapter->GetDesc1(&adapter_desc);
    printf("dxgi-object-order: adapter %ls\n", adapter_desc.Description);

    ComPtr<ID3D12Device> device;
    Check(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)), "D3D12CreateDevice");

    D3D12_COMMAND_QUEUE_DESC queue_desc = {};
    queue_desc.Type                     = D3D12_COMMAND_LIST_TYPE_DIRECT;
    ComPtr<ID3D12CommandQueue> queue;
    Check(device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&queue)), "CreateCommandQueue");

    ComPtr<IDXGIFactory2> swapchain_factory;
    Check(CreateDXGIFactory2(0, IID_PPV_ARGS(&swapchain_factory)), "CreateDXGIFactory2 (swap chain)");
    DXGI_SWAP_CHAIN_DESC1 swapchain_desc = {};
    swapchain_desc.Width                 = kWidth;
    swapchain_desc.Height                = kHeight;
    swapchain_desc.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchain_desc.SampleDesc.Count      = 1;
    swapchain_desc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_desc.BufferCount           = kBufferCount;
    swapchain_desc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    ComPtr<IDXGISwapChain1> swapchain1;
    Check(swapchain_factory->CreateSwapChainForHwnd(queue.Get(), hwnd, &swapchain_desc, nullptr, nullptr, &swapchain1),
          "CreateSwapChainForHwnd");
    Check(swapchain_factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER), "MakeWindowAssociation");
    ComPtr<IDXGISwapChain3> swapchain;
    Check(swapchain1.As(&swapchain), "QueryInterface IDXGISwapChain3");

    // The runtime keeps both factory objects alive through the adapter and the swap chain; GFXR destroys their
    // wrappers here, so GetParent below creates fresh ones whose only creation record is the GetParent call.
    enum_factory.Reset();
    swapchain_factory.Reset();
    printf("dxgi-object-order: released both root factories\n");

    ComPtr<IDXGIFactory4> adapter_parent_factory;
    Check(adapter->GetParent(IID_PPV_ARGS(&adapter_parent_factory)), "IDXGIAdapter::GetParent");
    printf("dxgi-object-order: factory from IDXGIAdapter::GetParent ok\n");

    ComPtr<IDXGIFactory2> swapchain_parent_factory;
    Check(swapchain1->GetParent(IID_PPV_ARGS(&swapchain_parent_factory)), "IDXGISwapChain::GetParent");
    printf("dxgi-object-order: factory from IDXGISwapChain::GetParent ok\n");

    // WARP has no outputs; the other two cases still run.
    ComPtr<IDXGIOutput> swapchain_output;
    HRESULT             output_result = swapchain1->GetContainingOutput(&swapchain_output);
    printf("dxgi-object-order: output from IDXGISwapChain::GetContainingOutput %s (0x%08lx)\n",
           SUCCEEDED(output_result) ? "ok" : "unavailable",
           static_cast<unsigned long>(output_result));
    fflush(stdout);

    D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
    rtv_heap_desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtv_heap_desc.NumDescriptors             = kBufferCount;
    ComPtr<ID3D12DescriptorHeap> rtv_heap;
    Check(device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&rtv_heap)), "CreateDescriptorHeap");
    UINT rtv_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    ComPtr<ID3D12Resource> back_buffers[kBufferCount];
    for (UINT i = 0; i < kBufferCount; ++i)
    {
        Check(swapchain->GetBuffer(i, IID_PPV_ARGS(&back_buffers[i])), "GetBuffer");
        D3D12_CPU_DESCRIPTOR_HANDLE rtv = rtv_heap->GetCPUDescriptorHandleForHeapStart();
        rtv.ptr += static_cast<SIZE_T>(i) * rtv_size;
        device->CreateRenderTargetView(back_buffers[i].Get(), nullptr, rtv);
    }

    ComPtr<ID3D12CommandAllocator> allocator;
    Check(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator)),
          "CreateCommandAllocator");
    ComPtr<ID3D12GraphicsCommandList> list;
    Check(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator.Get(), nullptr, IID_PPV_ARGS(&list)),
          "CreateCommandList");
    Check(list->Close(), "Close");

    ComPtr<ID3D12Fence> fence;
    Check(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)), "CreateFence");
    UINT64 fence_value = 0;
    HANDLE fence_event = CreateEventW(nullptr, FALSE, FALSE, nullptr);

    UINT frame = 0;
    while ((frame < frame_count) && !quit_requested)
    {
        MSG msg = {};
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        // Use every re-obtained object each frame, so the trimmed frames reference them too.
        HWND associated = nullptr;
        adapter_parent_factory->GetWindowAssociation(&associated);
        swapchain_parent_factory->GetWindowAssociation(&associated);
        if (swapchain_output != nullptr)
        {
            DXGI_OUTPUT_DESC output_desc = {};
            swapchain_output->GetDesc(&output_desc);
        }

        UINT buffer_index = swapchain->GetCurrentBackBufferIndex();
        Check(allocator->Reset(), "CommandAllocator::Reset");
        Check(list->Reset(allocator.Get(), nullptr), "CommandList::Reset");

        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource   = back_buffers[buffer_index].Get();
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
        list->ResourceBarrier(1, &barrier);

        D3D12_CPU_DESCRIPTOR_HANDLE rtv = rtv_heap->GetCPUDescriptorHandleForHeapStart();
        rtv.ptr += static_cast<SIZE_T>(buffer_index) * rtv_size;
        const float color[4] = { static_cast<float>(frame % 120) / 120.0f, 0.3f, 0.6f, 1.0f };
        list->ClearRenderTargetView(rtv, color, 0, nullptr);

        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
        list->ResourceBarrier(1, &barrier);
        Check(list->Close(), "Close");

        ID3D12CommandList* lists[] = { list.Get() };
        queue->ExecuteCommandLists(1, lists);
        Check(swapchain->Present(1, 0), "Present");

        Check(queue->Signal(fence.Get(), ++fence_value), "Signal");
        Check(fence->SetEventOnCompletion(fence_value, fence_event), "SetEventOnCompletion");
        WaitForSingleObject(fence_event, INFINITE);
        ++frame;
    }

    CloseHandle(fence_event);
    DestroyWindow(hwnd);
    printf("dxgi-object-order: rendered %u frames\n", frame);
    fflush(stdout);
    return 0;
}
