#!/usr/bin/env python3

# Copyright (c) 2026 LunarG, Inc. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

"""Compute the FNV-1a 64-bit hash of a SPIR-V (or any binary) file.

Prints a 16-hex lowercase digest matching the algorithm used by
ShaderReplaceMap::HashSpirV in framework/util/shader_replacement.cpp.
That hash is the key that gfxrecon-replay's --replace-shaders <dir>
expects on `<16-hex>.spv` filenames.

Usage:
  scripts/hash_spirv_module.py PATH

Example:
  scripts/hash_spirv_module.py claude_files/shaders_out/sh255516
  -> f2930129a3e87aa1
"""

import sys


def fnv1a64(data: bytes) -> int:
    h = 0xCBF29CE484222325
    for b in data:
        h = ((h ^ b) * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
    return h


def main(argv):
    if len(argv) != 2:
        print(__doc__, file=sys.stderr)
        return 2
    with open(argv[1], 'rb') as f:
        data = f.read()
    print(f'{fnv1a64(data):016x}')
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
