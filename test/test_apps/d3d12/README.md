# D3D12 test apps

Small D3D12 applications that exercise the DXR replay paths of GFXReconstruct and judge the result on the GPU. Each
DXR app draws a 4x2 grid of 128x128 tiles, one tile per variant under test. A tile's shader reads known values through
the addresses under test (a TLAS, a shader table, a root argument in a hit record, an indirect argument) and writes
pass green, a fail color carrying a bit mask the app documents, or leaves the tile black. A replay of a capture of an
app is judged from a screenshot, on any adapter, without a reference image. The app itself judges its last frame on
the CPU, prints a line per tile and returns the failing tile count.

The apps test the replayer, not the driver: a tile that passes in the app's own report while it is captured and fails
at replay shows a replay defect or a limitation of the replay mode. A tile that fails or removes the device in the
app's own run says nothing about replay on that adapter; leave it out with `--case`.

## Building

- The apps are built in every Windows build with `D3D12_SUPPORT`, independent of `GFXRECON_INCLUDE_TEST_APPS` (the
  Vulkan test apps).
- The DXR apps compile their shaders at build time with the DXC SDK's `dxc.exe` (`cmake/FindDXC.cmake`,
  `DXC_EXE_PATH`) and are skipped with a CMake message when it is missing. `dxgi-object-order` builds regardless.
- Executables: `build\test\test_apps\d3d12\<app>\<Config>\gfxrecon-testapp-d3d12-<app>.exe`, with the Agility SDK
  runtime under `D3D12\` beside each, as for `gfxrecon-replay.exe` (`cmake/AgilitySDK.cmake`).
- `GFXRECON_D3D12_TESTAPP_GPU_WRITTEN_TILES` (default OFF) builds the tiles marked GPU-written in the tables below.
  Their DXR addresses are written by compute shaders from root constants, so a replay that does not translate values
  written on the GPU hands the GPU capture-time addresses, which can hang it. Neither replay mode on dev translates
  them.
- `GFXRECON_D3D12_TESTAPP_KNOWN_DEFECT_TILES` (default OFF) builds the tiles marked known-defect. They reproduce replay
  defects that leave shader records untranslated, which can fault the GPU on a build without the fix. Turn the option
  on to test a fix for one of them.
- A tile left out of the build stays black and the app reports it as "not built".

## Running

```
gfxrecon-testapp-d3d12-<app>.exe [--warp] [--frames N] [--case a,b,...] [--debug-layer] [--ballast-mb N] [--no-vsync]
```

- `--frames N` renders N frames and judges the last one. 0, the default, runs until the window is closed and judges
  the last frame then.
- `--case` runs a subset of tiles, by the names in the tables below.
- `--debug-layer` enables the D3D12 debug layer and prints its warnings and errors at exit.
- `--warp` picks the WARP adapter instead of the first high-performance adapter with raytracing tier 1.1.
- A value may be joined (`--case=a,b`). The environment variable `GFXRECON_TESTAPP_ARGS` adds options for a launcher
  that cannot pass them.
- Output: one line per tile, `<app>: tile <i> <name>: pass | fail bits=0x.. | not run | not built`, the adapter
  name, the frame count, and an exit code equal to the number of failing tiles.

## Capturing

Capture the apps as frame-range trims. During initialization, before any test resource exists, an app allocates
ballast (a buffer of `--ballast-mb` MB, 64 by default, and a descriptor heap of 4096 descriptors) and releases it once
the test resources exist. A trim taken later never recreates the ballast, so at replay every GPU virtual address and
descriptor handle of the test resources lands elsewhere than at capture, and a value the replayer failed to translate
reads the wrong memory or faults. A full capture replays the ballast too and loses that property.

1. Copy the capture libraries beside the executable (see USAGE_desktop_D3D12.md, "Enabling D3D12 Capture").
2. `set GFXRECON_CAPTURE_FRAMES=30-60`. Frame 1 does the initialization, so any range after it works.
3. Run the app with `--frames 90`, or close its window after the range.

The app's own report during the capture run is the capture-time reference: judge a replay on the tiles that pass there.

## Replaying

| Mode | Command |
|------|---------|
| Replay | `gfxrecon-replay.exe <capture>`: Dx12ResourceValueMapper maps the resource values a submission uses before the submission executes. |
| Optimized | `gfxrecon-optimize.exe --dxr <capture> <optimized>`, then `gfxrecon-replay.exe <optimized>`: the optimizer records which writes carry the values, so replay maps them as the data is written. |

Recommended replay options for both modes:

- `--create-dummy-allocations`, so an untranslated address faults instead of reading whatever the capture-time
  address holds at replay.
- `--debug-device-lost`, so a fault produces DRED output naming the faulting command.
- `--screenshot-all --screenshot-format png`, or `--screenshots N-M`, for the verdict.
- `--validate` once per mode, for the D3D12 debug layer.

## Reading the verdict

A replay has no CPU-side report: the verdict is the replayed image. Every screenshot is the 512x256 verdict image, a
tile per 128x128 cell, numbered row-major from the top left:

- pass: every pixel is (0, 255, 0).
- fail: pixels are (255, 0, bits * 8). The blue channel divided by 8 is the failure mask the app documents, OR'd over
  the tile.
- not run or not built: the tile is black. Every frame starts by clearing the output, so a tile is green only if this
  replay painted it, even though a trim's state block seeds the output with the captured image.
- A replay fault shows as a device removal in the replay log, with DRED output under `--debug-device-lost`.

## Expected results

The tables give the expected result of each tile on a trimmed capture in both modes, and the GFXR revision the pass is
expected on. GPU-written marks the tiles behind `GFXRECON_D3D12_TESTAPP_GPU_WRITTEN_TILES`, known-defect the tiles
behind `GFXRECON_D3D12_TESTAPP_KNOWN_DEFECT_TILES`. Every tile marked "pass" and built by default was measured on dev
63f86f2e, at capture and in both replay modes, with a screenshot of every replayed frame. `--validate` reported no D3D12
debug-layer message except the one noted for dxr-pipelines tile 7.

### dxr-basic (3 tiles)

The shared scene (`common/dxr_scene.*`): one quad in a BLAS, a TLAS over one instance, and a state object whose hit
group record carries a root constant, a root CBV address and a descriptor-table handle. Fail bits: 1 root constant,
2 root CBV, 4 descriptor table, 8 InstanceID, 16 miss.

| Tile | Exercises | Replay | Optimized | Expected to pass on |
|------|-----------|--------|-----------|---------------------|
| 0 `direct-static` | DispatchRays over the TLAS built at initialization and restored from the trim's state block | pass | pass | dev |
| 1 `direct-rebuilt` | DispatchRays over a TLAS rebuilt every frame from an instance desc the CPU rewrites | pass | pass | dev |
| 2 `inline` | RayQuery (cs_6_5) against the static TLAS through an acceleration structure SRV descriptor | pass | pass | dev |

### dxr-indirect (6 tiles)

ExecuteIndirect on the shared scene. Fail bits of the DXR tiles as dxr-basic. Tile 3: 1 CBV, 2 SRV, 4 UAV. Tile 4:
1 CBV, nothing drawn leaves the tile black.

| Tile | Exercises | Replay | Optimized | Expected to pass on |
|------|-----------|--------|-----------|---------------------|
| 0 `indirect-cpu` | signature {DISPATCH_RAYS}, the desc CPU-written into an upload buffer | pass | pass | dev |
| 1 `indirect-gpu-desc` (GPU-written) | the desc composed by a compute shader each frame from root constants carrying the table addresses | black, a fault is possible | black | a replay that translates GPU-written values; not dev |
| 2 `indirect-count` | {CONSTANT, DISPATCH_RAYS} x 4 with a count buffer holding 3; the fourth command is valid and paints the tile red if it runs | pass | pass | dev |
| 3 `indirect-root-views` | compute signature {CBV, SRV, UAV, DISPATCH}, a magic value read through each root view | pass | pass | dev |
| 4 `indirect-ia-views` | graphics signature {VBV, IBV, CBV, DRAW_INDEXED}, a quad drawn into the tile | pass | pass | dev |
| 5 `indirect-cross-list` | the argument buffer filled and transitioned by a command list submitted ahead of the frame's list in the same ExecuteCommandLists | pass | pass | dev |

`gfxrecon-optimize --dxr` logs "Failed to find the required data for DXR optimization" on captures that include tiles
2-4; the optimized replay passes regardless.

### dxr-acceleration-structures (8 tiles)

Top-level build inputs in the forms a replay must follow to the GPU, on the app's own scene: a quad per BLAS at two
depths, and a tile of four horizontal bands, each an instance of the quad. The closest hit shader reports the instance
and the hit distance, which names the BLAS. Fail bits: 1 a hit on an inactive band, 2 wrong hit distance (another
BLAS), 4 InstanceIndex, 8 InstanceID, 16 miss on an active band.

| Tile | Exercises | Replay | Optimized | Expected to pass on |
|------|-----------|--------|-----------|---------------------|
| 0 `pointers-cpu` | ARRAY_OF_POINTERS: the pointer array and the four descs CPU-written (a buffer per desc), rebuilt every frame | pass | pass | dev at or after 6b880c2d (#3373) |
| 1 `pointers-gpu` (GPU-written) | the pointer array written by a compute shader | GPU hang possible | GPU hang possible | a replay that translates GPU-written values; not dev |
| 2 `descs-gpu` (GPU-written) | ARRAY: the descs composed by a compute shader from root constants carrying the BLAS addresses | as tile 1 | as tile 1 | as tile 1 |
| 3 `pointers-gpu-descs` (GPU-written) | ARRAY_OF_POINTERS: CPU-written pointers into GPU-composed descs | as tile 1 | as tile 1 | as tile 1 |
| 4 `pointers-inactive` | ARRAY_OF_POINTERS whose bands 1 and 2 point at descs without a BLAS (legal, inactive) | pass | pass | dev at or after 6b880c2d (#3373) |
| 5 `update` | a TLAS built at initialization with ALLOW_UPDATE and its instances off the tile, refit in place every frame (PERFORM_UPDATE) | pass | pass | dev |
| 6 `compact` | even bands on a compacted BLAS (COMPACTED_SIZE postbuild info, COPY_MODE_COMPACT), odd bands on a COPY_MODE_CLONE of another compacted BLAS | pass | pass | dev |
| 7 `compute-queue` | tile 0's build recorded on a compute command list and executed on a compute queue, the direct queue waiting on its fence | pass | pass | dev at or after 6b880c2d (#3373) |

Before 6b880c2d, the state block's restore of an ARRAY_OF_POINTERS top-level build kept the captured layout over the
decoded desc array and hung the GPU, so any trim of the default run, which builds tiles 0, 4 and 7 every frame, hung
at load. Tile 6's compacted sizes are the capturing adapter's, so replay that tile on the same adapter.

### dxr-pipelines (8 tiles)

The ways a raytracing state object is assembled, each tile a pipeline of its own over the shared scene, with hit
records whose local root arguments the replay has to translate. A local root signature the replay fails to resolve
shows as a red tile or a fault. Root signature A is the scene's (a root constant, a root CBV, a descriptor table). B
declares the same three parameters in the opposite order, so a record laid out for the wrong one fails. Fail bits as
dxr-basic, with 2 also set when the callable shader returns the wrong value (tile 6).

| Tile | Exercises | Replay | Optimized | Expected to pass on |
|------|-----------|--------|-----------|---------------------|
| 0 `collection-hitgroup` (known-defect) | the closest hit shader, root signature A and their association live in a collection; the pipeline includes it and defines the hit group importing the shader, whose root signature is therefore the import's | fail: the hit group gets no root signature and its record stays untranslated | fail | #3379 |
| 1 `collection-two-lrs` (known-defect) | two collections export the shader under two names, one with A, one with B; two hit groups over two instances | fail, as tile 0 | fail | #3379 |
| 2 `collection-explicit` | the collection leaves its shader unassociated (ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITIONS); the pipeline associates A to the hit group export | pass | pass | dev |
| 3 `collection-released` (known-defect) | tile 0's shape with the collection released once the pipeline exists | fail: the trim's state block holds no collection, so CreateStateObject fails and the dispatch on the missing pipeline may crash the replayer | fail | none yet: the state writer does not write a pipeline's collections |
| 4 `add-to-state-object` | a pipeline grown with AddToStateObject, the original released: the original's hit group with A, the addition's with B, both dispatched | pass | pass | dev |
| 5 `dxil-defined-lrs` (known-defect) | the local root signature and its association declared in the DXIL library, no LOCAL_ROOT_SIGNATURE subobject in the description | fail: the association parser skips subobjects inside DXIL libraries | fail | none yet |
| 6 `callable` | the closest hit shader calls a callable shader that reads the root CBV of its own local root signature; a callable shader table in the dispatch | pass | pass | dev |
| 7 `implicit-default` | root signature A with no association, the default for every export, so the raygen and miss records carry the arguments too | pass | pass | dev |

Tile 2's collection makes replay and the optimizer log, once per flag, "State object created with flag
D3D12_STATE_OBJECT_FLAG_ALLOW_..._DEPENDENCIES_... Dependencies between state objects may not be fully supported".
Under `--validate`, tile 7 draws D3D12 ERROR ID 1195 from GetShaderIdentifier on `ClosestHit`: replay asks for the
identifier of every export the default root signature covers, including a hit group's component shader, which has
none. The call returns null and the tile passes.

### dxr-command-contexts (7 tiles)

The command contexts a DispatchRays or an acceleration structure build can reach the GPU from, each tile dispatching
the scene's pipeline over a TLAS of the scene's quad. Fail bits as dxr-basic. Render passes have no tile: the render
pass spec lists Dispatch* among the calls a pass forbids.

| Tile | Exercises | Replay | Optimized | Expected to pass on |
|------|-----------|--------|-----------|---------------------|
| 0 `bundle-dispatch` (known-defect) | DispatchRays recorded in a bundle, executed by the frame's list | fail: Dx12ResourceValueMapper maps the records of the lists a queue executes and never a bundle's, so the shader table stays untranslated | fail | none yet |
| 1 `bundle-indirect` (known-defect) | ExecuteIndirect with a DISPATCH_RAYS signature recorded in a bundle | fail, as tile 0 | fail | none yet |
| 2 `compute-queue` | DispatchRays on a compute list executed on a compute queue into a texture of its own, copied into the tile by the frame's list behind a fence | pass | pass | dev |
| 3 `copy-queue` | the CPU-written instance desc copied by a copy queue into the default-heap buffer the frame's TLAS build reads, behind a fence | pass | pass | dev |
| 4 `enhanced-barriers` | the shader table copied into place and the TLAS rebuilt every frame behind ID3D12GraphicsCommandList7::Barrier only, no legacy ResourceBarrier | pass | pass | dev |
| 5 `cross-list-build` | the TLAS built on a list submitted ahead of the frame's list in the same ExecuteCommandLists, dispatched over by the frame's list | pass | pass | dev |
| 6 `reused-list` | a list recorded once at initialization and executed every frame without a reset, into a texture of its own | pass | pass | dev |

### dxgi-object-order

Not a tile app. It holds DXGI objects a trim's state block has to order around their sources: factories re-obtained
through `IDXGIObject::GetParent` after the app dropped its own references (one from the adapter, one from the swap
chain) and an output from `IDXGISwapChain::GetContainingOutput`, all created before the first frame and used every
frame. Replay exits 0 even when an object is missing, because calls on an unmapped object are skipped silently, so
the check is on the converted capture: convert the trim with `gfxrecon-convert` and verify that every method call in
the state block is on an object created earlier in the block, and that no frame call is on an object never created.

| Exercises | Expected | Expected to pass on |
|-----------|----------|---------------------|
| DXGI factories and outputs re-obtained from other objects, in a trim's state block | all objects created before use, except one known case: the factory obtained from the swap chain through GetParent is written before the swap chain (1 misordered object, 11 frame calls on it lost) | dev at or after 404005e8 (#3268), with the known case |

### Planned

- dxr-pipelines: a collection without a root signature inside a pipeline with a default association, and two
  LOCAL_ROOT_SIGNATURE subobjects sharing one root signature with only one of them associated.
- DXR 1.2 (opacity micromaps, shader execution reordering) once drivers and the replayer support them.

## WARP

`--warp` selects the WARP adapter. Use the `d3d10warp.dll` of the `Microsoft.Direct3D.WARP` NuGet package, 1.0.22 or
later, beside the executable and beside `gfxrecon-replay.exe` for the replay. The WARP that ships with Windows can be
older and crash on dxr-acceleration-structures tile 4 (`pointers-inactive`), whose instances have a null BLAS.

## Adding an app

Derive from `D3d12TestApp` (`common/d3d12_test_app.h`): name the tiles, create the test resources in `Initialize()` on
the open command list, record one frame in `RecordFrame()` writing only enabled tiles, and document the fail bits at
the top of the source. The shared scene (`common/dxr_scene.h`) provides a quad, a TLAS, a state object with a local
root signature and the three shader tables. Add the executable to `CMakeLists.txt` next to the others, compile its
shaders with `gfxrecon_d3d12_shader`, and add its table to this file with the GFXR revision its expectations hold on.
