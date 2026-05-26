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

// Hash-keyed SPIR-V replacement map shared by capture and replay.
//
// The on-disk format is a directory of files named "<16-hex-hash>.spv",
// where the hash is FNV-1a 64-bit over the SPIR-V bytes of the *original*
// module the file replaces. At lookup time, callers hash the incoming
// SPIR-V; on a hit, the patched bytes are returned.
//
// Validation is split into two stages:
//   (1) LoadDir: every file must parse as well-formed SPIR-V (magic +
//       header + walkable instruction stream), or it is rejected.
//   (2) IsCompatibleProfile: at swap time, the patched module's
//       (entry_points, capabilities) must be compatible with the
//       original's. This guards against silently substituting a module
//       that the driver / pipeline-stage will reject.

#ifndef GFXRECON_UTIL_SHADER_REPLACEMENT_H
#define GFXRECON_UTIL_SHADER_REPLACEMENT_H

#include "util/defines.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(util)

struct SpirvProfile
{
    std::vector<uint32_t>                          capabilities;  // OpCapability operand IDs
    std::vector<std::pair<uint32_t, std::string>> entry_points;  // (ExecutionModel, name)
};

struct ShaderReplacement
{
    std::vector<uint8_t> bytes;
    SpirvProfile         profile;
    std::string          source_file;  // diagnostic
};

class ShaderReplaceMap
{
  public:
    // FNV-1a 64-bit over the raw SPIR-V byte buffer.
    static uint64_t HashSpirV(const void* data, size_t bytes);

    // Parse a SPIR-V byte buffer, populating the profile (entry points and
    // capabilities). Returns false (and a human-readable message) if the
    // input is malformed.
    static bool ParseProfile(const void* data, size_t bytes, SpirvProfile* out, std::string* err);

    // True if `patched` can stand in for `original`:
    //   - same set of (execution_model, name) entry points
    //   - patched.capabilities is a subset of original.capabilities
    static bool IsCompatibleProfile(const SpirvProfile& original,
                                    const SpirvProfile& patched,
                                    std::string*        err);

    // Walk `dir` and add every "<16-hex>.spv" file as a replacement keyed
    // on the parsed hash. Files that fail SPIR-V parsing or whose name is
    // not 16 hex digits are rejected and logged. Returns the number of
    // entries successfully loaded.
    size_t LoadDir(const std::string& dir);

    // Returns nullptr if no replacement is registered for `original_hash`.
    const ShaderReplacement* LookupByHash(uint64_t original_hash) const;

    // Convenience: hash + lookup in one call.
    const ShaderReplacement* Lookup(const void* original_spirv, size_t bytes) const;

    bool   empty() const { return entries_.empty(); }
    size_t size() const { return entries_.size(); }

  private:
    std::unordered_map<uint64_t, ShaderReplacement> entries_;
};

GFXRECON_END_NAMESPACE(util)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif  // GFXRECON_UTIL_SHADER_REPLACEMENT_H
