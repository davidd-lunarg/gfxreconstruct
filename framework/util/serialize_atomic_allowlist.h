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
*/

#ifndef GFXRECONSTRUCT_UTIL_SERIALIZE_ATOMIC_ALLOWLIST_H
#define GFXRECONSTRUCT_UTIL_SERIALIZE_ATOMIC_ALLOWLIST_H

#include "util/defines.h"

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(util)

// =====================================================================
// Compile-time allow-list for --serialize-atomic-dispatches
// =====================================================================
//
// Edit the array below and rebuild to control which atomic-using compute
// modules get workgroup-serialised. Each entry is a 16-hex FNV-1a64 hash
// of the ORIGINAL (unpatched) SPIR-V byte stream, matching the format
// used by --replace-shaders.
//
// Semantics:
//   - Empty array AND no env var -> patch every atomic-using compute
//     module (the pre-allow-list default behaviour).
//   - One or more entries -> patch ONLY modules whose hash matches an
//     entry; every other atomic-using module is passed through
//     unchanged. To "patch nothing" while keeping the allow-list active,
//     use a sentinel value like "0000000000000000" -- no real module
//     hashes to all-zero.
//
// Precedence at runtime (highest first):
//   1. --serialize-atomic-allowlist <h1,h2,...>   (replay CLI flag)
//   2. GFXR_SERIALIZE_ATOMIC_ALLOWLIST            (env var, same syntax)
//   3. this compile-time list
//   4. empty -> patch every atomic-using module
//
// Prefer the CLI flag for ad-hoc bisection work -- no recompile needed.
// =====================================================================

// Default: empty list -> patch every atomic-using compute module. Replace
// with a brace-init list of "<16-hex>" strings (and update
// kSerializeAtomicAllowlistCount to match) to restrict patching at compile
// time. Most users should reach for the --serialize-atomic-allowlist CLI
// flag instead so no rebuild is needed.
inline constexpr const char* const* kSerializeAtomicAllowlistHexHashes = nullptr;
inline constexpr std::size_t        kSerializeAtomicAllowlistCount     = 0;

GFXRECON_END_NAMESPACE(util)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif // GFXRECONSTRUCT_UTIL_SERIALIZE_ATOMIC_ALLOWLIST_H
