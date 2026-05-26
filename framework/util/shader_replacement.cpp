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

#include "util/shader_replacement.h"

#include "util/logging.h"

#include "spirv/unified1/spirv.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(util)

namespace
{
constexpr uint32_t kSpirvMagic       = 0x07230203u;
constexpr size_t   kSpirvHeaderWords = 5;

bool ParseHexU64(const std::string& s, uint64_t* out)
{
    if (s.size() != 16)
    {
        return false;
    }
    uint64_t v = 0;
    for (char c : s)
    {
        uint64_t d;
        if (c >= '0' && c <= '9')
            d = static_cast<uint64_t>(c - '0');
        else if (c >= 'a' && c <= 'f')
            d = static_cast<uint64_t>(c - 'a' + 10);
        else if (c >= 'A' && c <= 'F')
            d = static_cast<uint64_t>(c - 'A' + 10);
        else
            return false;
        v = (v << 4) | d;
    }
    *out = v;
    return true;
}

// Read the literal string operand starting at the given word. SPIR-V packs
// strings as little-endian byte sequences within 32-bit words, terminated
// by a 0x00 byte. `available_words` bounds how many words may be consumed.
std::string ReadLiteralString(const uint32_t* words, size_t available_words)
{
    std::string out;
    for (size_t i = 0; i < available_words; ++i)
    {
        const uint32_t w = words[i];
        for (int b = 0; b < 4; ++b)
        {
            const char c = static_cast<char>((w >> (b * 8)) & 0xff);
            if (c == 0)
            {
                return out;
            }
            out.push_back(c);
        }
    }
    return out;
}
}  // namespace

uint64_t ShaderReplaceMap::HashSpirV(const void* data, size_t bytes)
{
    const uint8_t* p = static_cast<const uint8_t*>(data);
    uint64_t       h = 0xcbf29ce484222325ULL;
    for (size_t i = 0; i < bytes; ++i)
    {
        h = (h ^ p[i]) * 0x100000001b3ULL;
    }
    return h;
}

bool ShaderReplaceMap::ParseProfile(const void* data, size_t bytes, SpirvProfile* out, std::string* err)
{
    auto fail = [&](const char* msg) {
        if (err)
        {
            *err = msg;
        }
        return false;
    };

    if (out == nullptr)
    {
        return fail("out is null");
    }
    *out = {};

    if (bytes < kSpirvHeaderWords * sizeof(uint32_t))
    {
        return fail("buffer smaller than SPIR-V header");
    }
    if ((bytes % sizeof(uint32_t)) != 0)
    {
        return fail("buffer size not a multiple of 4");
    }

    const uint32_t* words      = static_cast<const uint32_t*>(data);
    const size_t    word_count = bytes / sizeof(uint32_t);

    if (words[0] != kSpirvMagic)
    {
        return fail("magic mismatch");
    }

    size_t i = kSpirvHeaderWords;
    while (i < word_count)
    {
        const uint32_t inst       = words[i];
        const uint32_t word_count_field = inst >> 16;
        const uint32_t opcode     = inst & 0xffff;

        if (word_count_field == 0)
        {
            return fail("zero-length instruction");
        }
        if (i + word_count_field > word_count)
        {
            return fail("instruction overruns buffer");
        }

        switch (opcode)
        {
            case spv::OpCapability:
                if (word_count_field >= 2)
                {
                    out->capabilities.push_back(words[i + 1]);
                }
                break;
            case spv::OpEntryPoint:
                if (word_count_field >= 4)
                {
                    const uint32_t exec_model = words[i + 1];
                    // operand layout: exec_model, entry_id, name (literal string), interfaces...
                    const uint32_t* name_start  = &words[i + 3];
                    const size_t    avail       = word_count_field - 3;
                    std::string     name        = ReadLiteralString(name_start, avail);
                    out->entry_points.emplace_back(exec_model, std::move(name));
                }
                break;
            default:
                // Capabilities and entry points always appear in the header
                // section before any function definitions, so we can stop
                // walking once we see a function.
                if (opcode == spv::OpFunction)
                {
                    return true;
                }
                break;
        }

        i += word_count_field;
    }
    return true;
}

bool ShaderReplaceMap::IsCompatibleProfile(const SpirvProfile& original,
                                           const SpirvProfile& patched,
                                           std::string*        err)
{
    auto fail = [&](const std::string& msg) {
        if (err)
        {
            *err = msg;
        }
        return false;
    };

    // Entry point set must match (order-insensitive).
    if (original.entry_points.size() != patched.entry_points.size())
    {
        return fail("entry-point count differs");
    }
    auto sorted_eps = [](std::vector<std::pair<uint32_t, std::string>> v) {
        std::sort(v.begin(), v.end());
        return v;
    };
    if (sorted_eps(original.entry_points) != sorted_eps(patched.entry_points))
    {
        return fail("entry-point set differs");
    }

    // Patched capabilities must be a subset of the original.
    for (uint32_t cap : patched.capabilities)
    {
        if (std::find(original.capabilities.begin(), original.capabilities.end(), cap) ==
            original.capabilities.end())
        {
            return fail("patched module declares capability not present in original (id=" +
                        std::to_string(cap) + ")");
        }
    }

    return true;
}

size_t ShaderReplaceMap::LoadDir(const std::string& dir)
{
    entries_.clear();

    std::error_code ec;
    if (!std::filesystem::is_directory(dir, ec))
    {
        GFXRECON_LOG_ERROR("ShaderReplace: '%s' is not a directory", dir.c_str());
        return 0;
    }

    size_t loaded = 0;
    for (const auto& entry : std::filesystem::directory_iterator(dir, ec))
    {
        if (!entry.is_regular_file())
        {
            continue;
        }
        const std::filesystem::path& p   = entry.path();
        const std::string            ext = p.extension().string();
        if (ext != ".spv")
        {
            continue;
        }
        const std::string stem = p.stem().string();
        uint64_t          hash = 0;
        if (!ParseHexU64(stem, &hash))
        {
            GFXRECON_LOG_WARNING("ShaderReplace: skipping '%s' (filename stem is not a 16-hex hash)",
                                 p.string().c_str());
            continue;
        }

        std::ifstream f(p, std::ios::binary);
        if (!f)
        {
            GFXRECON_LOG_ERROR("ShaderReplace: failed to open '%s'", p.string().c_str());
            continue;
        }
        std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

        SpirvProfile profile;
        std::string  err;
        if (!ParseProfile(bytes.data(), bytes.size(), &profile, &err))
        {
            GFXRECON_LOG_ERROR("ShaderReplace: refusing '%s' (not valid SPIR-V: %s)",
                               p.string().c_str(),
                               err.c_str());
            continue;
        }

        ShaderReplacement rep;
        rep.bytes       = std::move(bytes);
        rep.profile     = std::move(profile);
        rep.source_file = p.string();
        entries_.emplace(hash, std::move(rep));
        ++loaded;
    }

    GFXRECON_LOG_INFO("ShaderReplace: loaded %zu replacement shader(s) from '%s'", loaded, dir.c_str());
    return loaded;
}

const ShaderReplacement* ShaderReplaceMap::LookupByHash(uint64_t original_hash) const
{
    auto it = entries_.find(original_hash);
    return it == entries_.end() ? nullptr : &it->second;
}

const ShaderReplacement* ShaderReplaceMap::Lookup(const void* original_spirv, size_t bytes) const
{
    if (entries_.empty() || original_spirv == nullptr || bytes == 0)
    {
        return nullptr;
    }
    return LookupByHash(HashSpirV(original_spirv, bytes));
}

GFXRECON_END_NAMESPACE(util)
GFXRECON_END_NAMESPACE(gfxrecon)
