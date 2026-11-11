#!/usr/bin/env python3

# Copyright (c) 2026 LunarG, Inc.
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
'''
Run the D3D12 DXR test apps on a GFXR build in four steps, judging every tile
at every step:

  native     the app without GFXR, judged by its own report on its last frame
  capture    the app under the GFXR capture libraries with the D3D12 debug
             layer, trimmed to frames 30-60
  replay     gfxrecon-replay --validate of the trim, every frame screenshotted
             and judged
  optimized  gfxrecon-optimize on the trim, then its output replayed as for
             replay

--gfxr-build-dir names the GFXR build under test, which the suite only reads.
The apps are built in --build-dir, build/ of this checkout by default; when
that is the GFXR build itself nothing is built, and the suite may write there
(the results, a --warp-dll beside the apps). The suite stops at the first sign
of a device loss, so nothing runs on a GPU in an unknown state. Results go to
<output-dir>/<label>.json, with the logs, captures and screenshots under
<output-dir>/<label>/<app>/. The output directory defaults to
<build-dir>/testapp_suite and the label to the tested branch and the time.
The suite refuses a destination that exists and never deletes or overwrites
a file it did not create.
'''

import argparse
import collections
import datetime
import filecmp
import glob
import json
import os
import re
import shutil
import socket
import subprocess
import sys
import time
import traceback

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, '..', '..', '..'))

# Tile names in tile order, as each app prints them.
APPS = {
    'dxr-basic': ['direct-static', 'direct-rebuilt', 'inline'],
    'dxr-indirect': [
        'indirect-cpu', 'indirect-gpu-desc', 'indirect-count',
        'indirect-root-views', 'indirect-ia-views', 'indirect-cross-list'
    ],
    'dxr-acceleration-structures': [
        'pointers-cpu', 'pointers-gpu', 'descs-gpu', 'pointers-gpu-descs',
        'pointers-inactive', 'update', 'compact', 'compute-queue'
    ],
    'dxr-pipelines': [
        'collection-hitgroup', 'collection-two-lrs', 'collection-explicit',
        'collection-released', 'add-to-state-object', 'dxil-defined-lrs',
        'callable', 'implicit-default'
    ],
    'dxr-command-contexts': [
        'bundle-dispatch', 'bundle-indirect', 'compute-queue', 'copy-queue',
        'enhanced-barriers', 'cross-list-build', 'reused-list'
    ],
}
STEPS = ['native', 'capture', 'replay', 'optimized']
TOOLS = [('tools/replay', 'gfxrecon-replay.exe'),
         ('tools/optimize', 'gfxrecon-optimize.exe')]
GFXR_DLLS = [('layer/d3d12_capture', 'd3d12_capture.dll'),
             ('layer/d3d12', 'd3d12.dll'), ('layer/dxgi', 'dxgi.dll')]
WARP_DLL = 'd3d10warp.dll'
REPLAY_ARGS = ['--validate', '--create-dummy-allocations']

# The trim, and the frames an app renders: one past the trim, judging the last.
FRAMES = '30-60'
EXPECTED_FRAMES = 31
APP_FRAMES = 61
TIMEOUT_S = 600

# The verdict image is a 4x2 grid of 128x128 tiles, numbered row-major.
TILE_SIZE = 128
TILES_X = 4

# Signs of a device loss in a step's output. A FATAL counts too: the GPU's
# state after an abort is unknown.
DEVICE_LOSS = ('DXGI_ERROR_DEVICE_REMOVED', 'DXGI_ERROR_DEVICE_HUNG',
               'DXGI_ERROR_DEVICE_RESET', '[gfxrecon] FATAL',
               'returned 0x887a0005', 'returned 0x887a0006',
               'returned 0x887a0007')
MESSAGE = re.compile(r'\[gfxrecon\] (ERROR|WARNING|FATAL) - '
                     r'(D3D12 (?:ERROR|WARNING|CORRUPTION): \[ID \d+\]|'
                     r'[^.\r\n]{0,90})')
APP_TILE = re.compile(r'^[\w-]+: tile (\d+) (\S+): (.+?)\s*$')
# The debug layer messages an app prints at exit with --debug-layer.
APP_MESSAGE = re.compile(r'^[\w-]+: D3D12 (ERROR|WARNING|CORRUPTION) (\d+): ',
                         re.M)
LABEL = re.compile(r'[A-Za-z0-9][A-Za-z0-9._-]{0,127}')

VERBOSE = False


def say(msg):
    print(msg, flush=True)


def pip_line(*packages):
    return subprocess.list2cmdline([sys.executable, '-m', 'pip', 'install'] +
                                   list(packages))


def require(module, package, what, pip_args):
    '''
    True if module imports; otherwise prints what needs its package and the
    pip command that installs it.
    '''
    try:
        __import__(module)
        return True
    except ImportError:
        print('testapp_suite: %s needs %s, which this Python lacks. Install '
              'it with:\n  %s' % (what, package, pip_line(*pip_args)),
              file=sys.stderr)
        return False


def exe_name(app):
    return 'gfxrecon-testapp-d3d12-%s.exe' % app


def run(argv, log_path, cwd=None, env=None):
    '''
    Run argv with its output in the new file log_path, after the command
    line; returns (exit code, output). The process is killed after TIMEOUT_S,
    for an exit code of None.
    '''
    with open(log_path, 'x', errors='replace') as log_file:
        log_file.write('$ %s\n\n' % subprocess.list2cmdline(argv))
        log_file.flush()
        try:
            rc = subprocess.run(argv,
                                cwd=cwd,
                                env=env,
                                stdout=log_file,
                                stderr=subprocess.STDOUT,
                                timeout=TIMEOUT_S).returncode
        except subprocess.TimeoutExpired:
            log_file.write('\ntestapp_suite: killed after %d s\n' % TIMEOUT_S)
            rc = None
    output = read(log_path).partition('\n\n')[2]
    if VERBOSE:
        say('$ ' + subprocess.list2cmdline(argv))
        say(output)
    return rc, output


def read(path):
    try:
        with open(path, errors='replace') as f:
            return f.read().replace('\r', '')
    except OSError:
        return ''


def messages(*texts):
    '''
    Count the ERROR, WARNING and FATAL lines of GFXR logs, and the debug
    layer messages an app printed, by text.
    '''
    tally = collections.Counter()
    for text in texts:
        for m in MESSAGE.finditer(text):
            tally['%s %s' % (m.group(1), m.group(2).strip())] += 1
        for m in APP_MESSAGE.finditer(text):
            kind = 'ERROR' if m.group(1) == 'CORRUPTION' else m.group(1)
            tally['%s D3D12 %s: [ID %s]' % (kind, m.group(1), m.group(2))] += 1
    return dict(tally)


def fault_line(*texts):
    for text in texts:
        for line in text.splitlines():
            if any(sign in line for sign in DEVICE_LOSS):
                return line.strip()[:200]
    return None


def app_report(text):
    '''
    The app's own report: {tile index: (name, verdict)}, and its other lines
    (adapter, failing tile count, frame count).
    '''
    tiles, other = {}, []
    for line in text.splitlines():
        m = APP_TILE.match(line)
        if m:
            tiles[int(m.group(1))] = (m.group(2), m.group(3))
        elif line.strip(
        ) and '[gfxrecon]' not in line and not APP_MESSAGE.match(line):
            other.append(line.strip())
    return tiles, other


def classify(image, tile):
    '''
    Verdict of one tile of a screenshot, as the app judges its own frame:
    (verdict, fail bits). A gray slot has no tile this run.
    '''
    x = (tile % TILES_X) * TILE_SIZE
    y = (tile // TILES_X) * TILE_SIZE
    total = TILE_SIZE * TILE_SIZE
    colors = image.crop((x, y, x + TILE_SIZE, y + TILE_SIZE)).getcolors(total)
    passed = failed = black = empty = 0
    bits = 0
    for count, (r, g, b) in colors:
        if (r, g, b) == (0, 255, 0):
            passed += count
        elif (r == 255) and (g == 0):
            failed += count
            bits |= b // 8
        elif (r, g, b) == (0, 0, 0):
            black += count
        elif (r, g, b) == (128, 128, 128):
            empty += count
    if passed == total:
        return 'pass', 0
    if failed > 0:
        return 'fail', bits
    if black == total:
        return 'not run', 0
    if empty == total:
        return 'empty', 0
    return 'mixed', 0


def judge_frames(shot_dir, app):
    '''
    Per tile, the verdict counts over every screenshot in shot_dir; returns
    (frame count, {tile: Counter}).
    '''
    from PIL import Image

    pngs = glob.glob(
        os.path.join(glob.escape(shot_dir), 'screenshot_frame_*.png'))
    pngs.sort(key=lambda p: int(re.search(r'(\d+)\.png$', p).group(1)))
    counts = {i: collections.Counter() for i in range(len(APPS[app]))}
    for png in pngs:
        try:
            with Image.open(png) as image:
                rgb = image.convert('RGB')
        except OSError:
            # A damaged file counts against every tile, like a failed frame.
            for i in counts:
                counts[i]['unreadable'] += 1
            continue
        for i in counts:
            verdict, bits = classify(rgb, i)
            counts[i]['fail bits=0x%02x' %
                      bits if verdict == 'fail' else verdict] += 1
    return len(pngs), counts


def frame_verdict(counter, frames):
    '''
    One verdict for a tile over a replay: pass or empty only when every
    expected frame says so.
    '''
    if frames == 0:
        return 'no frames'
    kinds = set(counter)
    if (kinds == {'pass'}) and (frames == EXPECTED_FRAMES):
        return 'pass'
    if kinds == {'empty'}:
        return 'empty'
    if frames < EXPECTED_FRAMES:
        return 'incomplete (%d of %d frames)' % (frames, EXPECTED_FRAMES)
    worst = [k for k in counter if k.startswith('fail')] or [
        k for k in ('mixed', 'not run', 'unreadable') if k in counter
    ]
    return worst[0] if worst else 'mixed'


def gpu_info():
    '''Every display adapter with its driver version and date, or None.'''
    # Every adapter, not the first: which one ran the apps is what a result
    # needs to say. Windows 11 has no wmic, so CIM is read through PowerShell.
    ps = ("Get-CimInstance Win32_VideoController | ForEach-Object { "
          "'{0}|{1}|{2}' -f $_.Description, $_.DriverVersion, "
          "$(if ($_.DriverDate) { Get-Date $_.DriverDate -Format 'yyyyMMdd' }"
          " else { '' }) }")
    try:
        done = subprocess.run(
            ['powershell', '-NoProfile', '-NonInteractive', '-Command', ps],
            capture_output=True,
            encoding='utf-8',
            errors='replace',
            timeout=30)
    except (OSError, subprocess.TimeoutExpired):
        return None
    gpus = []
    for line in done.stdout.splitlines():
        if line.strip():
            description, _, rest = line.partition('|')
            version, _, date = rest.partition('|')
            gpus.append('%s, ver: %s, date: %s' % (description, version, date))
    return gpus or None


def revision(root):
    '''The revision checked out at root, or None outside a git checkout.'''
    if root is None:
        return None

    # --no-optional-locks keeps status from refreshing the index, so the
    # checkout is only read.
    def git(*args):
        return subprocess.run(['git', '--no-optional-locks'] + list(args),
                              cwd=root,
                              capture_output=True,
                              encoding='utf-8',
                              errors='replace',
                              check=True).stdout.strip()

    try:
        branch = git('rev-parse', '--abbrev-ref', 'HEAD')
        commit, _, subject = git('log', '-1', '--format=%h %s').partition(' ')
        modified = git('status', '--porcelain', '--untracked-files=no') != ''
    except (OSError, subprocess.CalledProcessError):
        return None
    return {
        'branch': branch,
        'commit': commit,
        'subject': subject,
        'modified': modified
    }


def source_dir(build_dir):
    '''The source directory a CMake build was configured from, or None.'''
    cache = read(os.path.join(build_dir, 'CMakeCache.txt'))
    m = re.search(r'^CMAKE_HOME_DIRECTORY:INTERNAL=(.+)$', cache, re.M)
    return m.group(1).strip() if m else None


def describe(rev):
    if rev is None:
        return 'not a git checkout'
    return '%s %s%s' % (rev['branch'], rev['commit'],
                        ' (modified)' if rev['modified'] else '')


def default_label(rev, warp):
    '''<branch>-<time>[-warp], with the branch name reduced to a file name.'''
    branch = 'gfxr'
    if rev is not None:
        branch = rev['branch']
        if branch == 'HEAD':
            branch = 'detached-' + rev['commit']
    branch = re.sub(r'[^A-Za-z0-9._-]+', '-', branch).strip('.-') or 'gfxr'
    label = '%s-%s' % (branch[:100], time.strftime('%Y%m%dT%H%M%S'))
    return label + '-warp' if warp else label


def build(build_dir, config, apps, log_path):
    '''
    Configure build_dir if it has no CMake cache, then build the apps, with
    the output in the new file log_path; returns True on success.
    '''
    commands = []
    status = subprocess.run(['git', 'submodule', 'status'],
                            cwd=ROOT,
                            capture_output=True,
                            encoding='utf-8',
                            errors='replace')
    if any(line.startswith('-') for line in status.stdout.splitlines()):
        commands.append(['git', 'submodule', 'update', '--init'])
    if not os.path.isfile(os.path.join(build_dir, 'CMakeCache.txt')):
        commands.append(['cmake', '-S', ROOT, '-B', build_dir, '-A', 'x64'])
    commands.append([
        'cmake', '--build', build_dir, '--config', config, '--parallel',
        '--target'
    ] + [os.path.splitext(exe_name(app))[0] for app in apps])
    say('building the test apps in %s (%s)' % (build_dir, log_path))
    with open(log_path, 'x', errors='replace') as log_file:
        for argv in commands:
            log_file.write('$ %s\n' % subprocess.list2cmdline(argv))
            log_file.flush()
            if VERBOSE:
                say('$ ' + subprocess.list2cmdline(argv))
            done = subprocess.run(argv,
                                  cwd=ROOT,
                                  stdout=None if VERBOSE else log_file,
                                  stderr=subprocess.STDOUT)
            if done.returncode != 0:
                say('%s failed; the end of %s:' % (argv[0], log_path))
                say('\n'.join(read(log_path).splitlines()[-30:]))
                return False
    return True


def inside(path, root):
    '''True if path is root or below it.'''
    path, root = os.path.normcase(path), os.path.normcase(root)
    return (path == root) or path.startswith(root.rstrip(os.sep) + os.sep)


def find_binaries(build_dir, config, wanted):
    '''
    {name: path} of the build outputs wanted as (subdir, name), under
    <subdir>/<config>/ or, for a single-config generator, <subdir>/; a
    missing one maps to None.
    '''
    found = {}
    for subdir, name in wanted:
        found[name] = None
        for path in (os.path.join(build_dir, subdir, config,
                                  name), os.path.join(build_dir, subdir,
                                                      name)):
            if os.path.isfile(path):
                found[name] = path
                break
    return found


def stage(exe_path, dest, files):
    '''
    Copy exe_path, the DLLs beside it but the GFXR capture libraries, and its
    Agility SDK runtime to the new folder dest, then files; returns the copied
    exe. The apps and tools run from such copies, so no run depends on, or
    adds to, what lies beside the build outputs.
    '''
    skip = [name for _, name in GFXR_DLLS]
    os.makedirs(dest)
    src = os.path.dirname(exe_path)
    shutil.copy2(exe_path, dest)
    for name in os.listdir(src):
        if name.lower().endswith('.dll') and (name.lower() not in skip):
            shutil.copy2(os.path.join(src, name), dest)
    agility = os.path.join(src, 'D3D12')
    if os.path.isdir(agility):
        shutil.copytree(agility, os.path.join(dest, 'D3D12'))
    for path in files:
        shutil.copy2(path, dest)
    return os.path.join(dest, os.path.basename(exe_path))


def unstage(path):
    '''Remove a staging folder, retrying while Windows releases its DLLs.'''
    for _ in range(20):
        try:
            shutil.rmtree(path)
            return
        except FileNotFoundError:
            return
        except OSError:
            time.sleep(0.5)
    say('could not remove %s; delete it by hand' % path)


def place_warp_dll(warp_dll, app_exes):
    '''
    Copy warp_dll beside each app, so the app alone with --warp runs the same
    WARP as the suite; a different d3d10warp.dll already there is an error.
    '''
    for exe in app_exes.values():
        dest = os.path.join(os.path.dirname(exe), WARP_DLL)
        if not os.path.exists(dest):
            shutil.copy2(warp_dll, dest)
        elif not filecmp.cmp(warp_dll, dest, shallow=False):
            say('a different %s is already beside %s; remove it or leave '
                'out --warp-dll' % (WARP_DLL, exe))
            return False
    return True


def step_line(step, entry, seconds):
    '''One console line per step: tile verdict counts, frames, messages.'''
    info = entry['steps'][step]
    counts = collections.Counter(t[step] for t in entry['tiles'].values()
                                 if step in t)
    parts = ['%d %s' % (n, v) for v, n in counts.most_common()]
    if 'frames' in info:
        parts.append('%d frames' % info['frames'])
    kinds = collections.Counter()
    for text, n in info['messages'].items():
        kinds[text.split(' ', 1)[0]] += n
    parts += ['%d %s' % (n, k) for k, n in sorted(kinds.items())]
    if info.get('error'):
        parts.append(info['error'])
    say('  %-10s %-56s %5.1f s' % (step, ', '.join(parts), seconds))
    if info.get('error') and not counts:
        for line in info.get('report', []):
            say('    ' + line)
    if info.get('fault'):
        say('  DEVICE LOSS: ' + info['fault'])


def main():
    global VERBOSE
    sys.stdout.reconfigure(errors='replace')
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--gfxr-build-dir',
                        help='the GFXR build to test, only read unless it is '
                        'also --build-dir: its capture '
                        'libraries, gfxrecon-replay and gfxrecon-optimize')
    parser.add_argument('--build-dir',
                        default=os.path.join(ROOT, 'build'),
                        help='CMake build directory of the test apps '
                        '(default: build)')
    parser.add_argument('--config',
                        default='Release',
                        choices=['Release', 'Debug'],
                        help='configuration of both builds (default: Release)')
    parser.add_argument('--no-build',
                        action='store_true',
                        help='use the test app build as it is')
    parser.add_argument('--apps',
                        default=','.join(APPS),
                        help='comma-separated test apps (default: all)')
    parser.add_argument('--case',
                        action='append',
                        default=[],
                        help='app=tile,tile: run only these tiles of an app; '
                        'repeat for other apps')
    parser.add_argument('--steps',
                        default=','.join(STEPS),
                        help='comma-separated subset of ' + ','.join(STEPS))
    parser.add_argument('--replay-args',
                        default='',
                        help='extra gfxrecon-replay arguments for both replay '
                        'steps, e.g. --replay-args="--debug-device-lost"')
    parser.add_argument('--warp',
                        action='store_true',
                        help='use the software adapter')
    parser.add_argument('--warp-dll',
                        help='a d3d10warp.dll to put beside the apps and use '
                        'for the tools')
    parser.add_argument('--output-dir',
                        help='results directory (default: '
                        '<build-dir>/testapp_suite)')
    parser.add_argument('--label',
                        default='',
                        help='name of the results (default: branch and time)')
    parser.add_argument('--verbose',
                        action='store_true',
                        help='print every command and its output')
    args = parser.parse_args()
    VERBOSE = args.verbose

    if not require('PIL', 'Pillow', 'testapp_suite.py',
                   ['-r', os.path.join(SCRIPT_DIR, 'requirements.txt')]):
        return 2

    if not args.gfxr_build_dir:
        parser.error('--gfxr-build-dir names the GFXR build to test')
    gfxr_build_dir = os.path.abspath(args.gfxr_build_dir)
    if not os.path.isdir(gfxr_build_dir):
        parser.error('no directory %s' % gfxr_build_dir)
    build_dir = os.path.abspath(args.build_dir)
    same_build = os.path.normcase(build_dir) == os.path.normcase(
        gfxr_build_dir)
    if not same_build and (inside(build_dir, gfxr_build_dir)
                           or inside(gfxr_build_dir, build_dir)):
        parser.error('--gfxr-build-dir and --build-dir must not hold one '
                     'another')
    output_dir = os.path.abspath(args.output_dir
                                 or os.path.join(build_dir, 'testapp_suite'))
    if inside(output_dir, gfxr_build_dir) and not same_build:
        parser.error('--output-dir is inside --gfxr-build-dir, which the '
                     'suite only reads')
    apps = [a for a in args.apps.split(',') if a]
    steps = [s for s in args.steps.split(',') if s]
    bad = [a for a in apps if a not in APPS]
    bad += [s for s in steps if s not in STEPS]
    if bad:
        parser.error('unknown app or step: %s' % ', '.join(bad))
    steps = [s for s in STEPS if s in steps]
    if ({'replay', 'optimized'} & set(steps)) and ('capture' not in steps):
        parser.error('replay and optimized need capture in the same run')
    cases = {}
    for spec in args.case:
        app, _, tiles = spec.partition('=')
        if (app not in APPS) or not tiles:
            parser.error('--case wants app=tile,tile; got %r' % spec)
        unknown = [t for t in tiles.split(',') if t not in APPS[app]]
        if unknown:
            parser.error('%s has no tile %s' % (app, ', '.join(unknown)))
        cases[app] = tiles
    if args.warp_dll and ((os.path.basename(args.warp_dll).lower() != WARP_DLL)
                          or not os.path.isfile(args.warp_dll)):
        parser.error('--warp-dll wants the path of a %s' % WARP_DLL)
    if args.label and (not LABEL.fullmatch(args.label)
                       or args.label.endswith('.')):
        parser.error('a label starts with a letter or digit and holds only '
                     'letters, digits, ".", "_" and "-" (128 at most), and '
                     'does not end with "."')

    # Name the results and refuse any that exist, before the build.
    gfxr_rev = revision(source_dir(gfxr_build_dir))
    testapps_rev = revision(ROOT)
    label = args.label or default_label(gfxr_rev, args.warp)
    run_dir = os.path.join(output_dir, label)
    out_path = run_dir + '.json'
    for path in (run_dir, out_path):
        if os.path.lexists(path):
            say('%s already exists; pick another --label or --output-dir' %
                path)
            return 2
    try:
        os.makedirs(run_dir)
    except FileExistsError:
        say('%s already exists; pick another --label or --output-dir' %
            run_dir)
        return 2
    say('GFXR %s in %s' % (describe(gfxr_rev), gfxr_build_dir))
    say('test apps %s' % describe(testapps_rev))

    # Build the apps, then locate the binaries.
    if same_build:
        say('the apps run from the GFXR build; nothing is built')
    elif not args.no_build and not build(build_dir, args.config, apps,
                                         os.path.join(run_dir, 'build.log')):
        return 2
    tools = find_binaries(gfxr_build_dir, args.config, TOOLS + GFXR_DLLS)
    app_exes = find_binaries(build_dir, args.config,
                             [('test/test_apps/d3d12/' + app, exe_name(app))
                              for app in apps])
    missing = [
        '%s in %s' % (n, gfxr_build_dir) for n, p in tools.items() if p is None
    ]
    missing += [
        '%s in %s' % (n, build_dir) for n, p in app_exes.items() if p is None
    ]
    if missing:
        say('not found (%s): %s' % (args.config, '; '.join(missing)))
        return 2
    app_exes = {app: app_exes[exe_name(app)] for app in apps}
    gfxr_dlls = [tools[name] for _, name in GFXR_DLLS]

    # The WARP the apps have beside them is the one the tools get.
    warp_dll = args.warp_dll
    if warp_dll and not place_warp_dll(warp_dll, app_exes):
        return 2
    if args.warp and not warp_dll:
        beside = os.path.join(os.path.dirname(app_exes[apps[0]]), WARP_DLL)
        warp_dll = beside if os.path.isfile(beside) else None
        if warp_dll is None:
            say('no %s beside the apps: the WARP that ships with Windows '
                'runs, which can be too old for them (README.md, WARP)' %
                WARP_DLL)
    tool_dlls = [warp_dll] if warp_dll else []

    started = time.time()
    result = {
        'label': label,
        'host': socket.gethostname(),
        'started': datetime.datetime.now().isoformat(timespec='seconds'),
        'warp': args.warp,
        'warp_dll': os.path.basename(warp_dll) if warp_dll else None,
        'frames': FRAMES,
        'replay_args': ' '.join(REPLAY_ARGS + args.replay_args.split()),
        'steps': steps,
        'cases': cases,
        'config': args.config,
        'gfxr': gfxr_rev,
        'gfxr_build_dir': gfxr_build_dir,
        'testapps': testapps_rev,
        'gpus': gpu_info(),
        'stopped': None,
        'apps': {}
    }

    def save(mode='w'):
        with open(out_path, mode) as f:
            json.dump(result, f, indent=1)

    save('x')

    # An inherited GFXRECON_* variable would change the capture, the log format
    # the messages are parsed from, or the apps' arguments.
    base_env = {
        k: v
        for k, v in os.environ.items() if not k.startswith('GFXRECON_')
    }

    tools_dir = os.path.join(run_dir, 'tools_bin')
    try:
        replay_exe = stage(tools['gfxrecon-replay.exe'],
                           os.path.join(tools_dir, 'replay'), tool_dlls)
        optimize_exe = stage(tools['gfxrecon-optimize.exe'],
                             os.path.join(tools_dir, 'optimize'), tool_dlls)
        for app in apps:
            if result['stopped']:
                break
            run_app(app, args, cases, app_exes[app], gfxr_dlls, replay_exe,
                    optimize_exe, base_env, run_dir, steps, result)
            save()
    except Exception:
        # The results so far are kept; the exit code says the runner failed.
        result['error'] = traceback.format_exc()
        say(result['error'])
    finally:
        unstage(tools_dir)
        result['finished'] = datetime.datetime.now().isoformat(
            timespec='seconds')
        save()

    summary(result)
    say('%.0f s; results: %s' % (time.time() - started, out_path))
    if result['stopped']:
        return 1
    if any('error' in info for entry in result['apps'].values()
           for info in entry['steps'].values()):
        return 3
    return 4 if 'error' in result else 0


def run_app(app, args, cases, app_exe, gfxr_dlls, replay_exe, optimize_exe,
            base_env, run_dir, steps, result):
    '''Every step of one app, recorded in result['apps'][app].'''
    exe = exe_name(app)
    app_dir = os.path.join(run_dir, app)
    os.makedirs(app_dir)
    app_args = ['--warp'] if args.warp else []
    if app in cases:
        app_args.append('--case=' + cases[app])
    # Every tile is listed, so an app that dies before its report shows them.
    tiles = {str(i): {'name': name} for i, name in enumerate(APPS[app])}
    entry = {'app_args': ' '.join(app_args), 'tiles': tiles, 'steps': {}}
    result['apps'][app] = entry
    say(app + (' ' + entry['app_args'] if app_args else ''))

    def tile(i, name):
        return entry['tiles'].setdefault(str(i), {'name': name})

    def app_step(step, env, dlls, layer_log):
        '''native or capture: records the app's own report per tile.'''
        bin_dir = os.path.join(app_dir, step + '_bin')
        try:
            staged = stage(app_exe, bin_dir, dlls)
            argv = [staged] + app_args
            argv += ['--frames=%d' % APP_FRAMES, '--no-vsync']
            rc, output = run(argv,
                             os.path.join(app_dir, step + '.log'),
                             cwd=bin_dir,
                             env=env)
        finally:
            unstage(bin_dir)
        layer_text = read(layer_log) if layer_log else ''
        tiles, other = app_report(output)
        for i, (name, verdict) in tiles.items():
            tile(i, name)[step] = verdict
        entry['steps'][step] = {
            'exit': rc,
            'report': other,
            'messages': messages(layer_text, output),
            'fault': fault_line(output, layer_text)
        }
        if not tiles:
            entry['steps'][step]['error'] = 'no verdict: %s' % (
                'killed after %d s' %
                TIMEOUT_S if rc is None else 'exit code %d' % rc)

    def replay_step(step, capture_file, logs):
        '''
        replay or optimized: replays capture_file, if it exists, and judges
        every frame.
        '''
        shots = os.path.join(app_dir, step + '_screenshots')
        os.makedirs(shots)
        rc, output = None, ''
        if os.path.isfile(capture_file):
            argv = [replay_exe] + REPLAY_ARGS + args.replay_args.split() + [
                '--screenshot-prefix',
                os.path.join(shots, 'screenshot'), '--screenshot-all',
                '--screenshot-format', 'png', capture_file
            ]
            log_name = 'replay.log' if step == 'replay' else (
                'optimized_replay.log')
            rc, output = run(argv,
                             os.path.join(app_dir, log_name),
                             cwd=app_dir)
        frames, counts = judge_frames(shots, app)
        for i, name in enumerate(APPS[app]):
            tile(i, name)[step] = frame_verdict(counts[i], frames)
            tile(i, name)[step + '_frames'] = dict(counts[i])
        entry['steps'][step] = {
            'replay_exit': rc,
            'optimize_exit': None,
            'frames': frames,
            'messages': messages(*logs, output),
            'fault': fault_line(output, *logs)
        }

    def go(step):
        return (step in steps) and not stopped_by(entry, result)

    capture_file = None
    if go('native'):
        started = time.time()
        app_step('native', base_env, [], None)
        step_line('native', entry, time.time() - started)
    if go('capture'):
        started = time.time()
        layer_log = os.path.join(app_dir, 'capture_gfxr.log')
        env = dict(base_env)
        env['GFXRECON_CAPTURE_FILE'] = os.path.join(app_dir, exe + '.gfxr')
        env['GFXRECON_CAPTURE_FILE_TIMESTAMP'] = 'true'
        env['GFXRECON_CAPTURE_FRAMES'] = FRAMES
        env['GFXRECON_LOG_FILE'] = layer_log
        env['GFXRECON_DEBUG_LAYER'] = 'true'
        app_args.append('--debug-layer')
        app_step('capture', env, gfxr_dlls, layer_log)
        app_args.pop()
        # GFXR names the trim <exe>_frames_<first>_through_<last>_<time>.
        captures = glob.glob(
            os.path.join(glob.escape(app_dir), exe + '_frames_*.gfxr'))
        if captures:
            capture_file = max(captures, key=os.path.getmtime)
            entry['capture'] = os.path.splitext(
                os.path.basename(capture_file))[0]
        else:
            entry['steps']['capture']['error'] = 'no capture file written'
        step_line('capture', entry, time.time() - started)
    if capture_file and go('replay'):
        started = time.time()
        replay_step('replay', capture_file, [])
        step_line('replay', entry, time.time() - started)
    if capture_file and go('optimized'):
        started = time.time()
        optimized_file = os.path.splitext(capture_file)[0] + '.optimized.gfxr'
        optimize_rc, optimize_output = run(
            [optimize_exe, capture_file, optimized_file],
            os.path.join(app_dir, 'optimize.log'),
            cwd=app_dir)
        replay_step('optimized', optimized_file, [optimize_output])
        entry['steps']['optimized']['optimize_exit'] = optimize_rc
        step_line('optimized', entry, time.time() - started)
    stopped_by(entry, result)


def stopped_by(entry, result):
    '''True once the run has shown a device loss; records where.'''
    if result['stopped']:
        return True
    for step, info in entry['steps'].items():
        if info.get('fault'):
            app = [a for a, e in result['apps'].items() if e is entry][0]
            result['stopped'] = {
                'app': app,
                'step': step,
                'line': info['fault']
            }
            say('device loss in %s %s, stopping: %s' %
                (app, step, info['fault']))
            return True
    return False


def summary(result):
    '''The verdict of every tile at every step, and the messages per step.'''
    say('')
    for app, entry in result['apps'].items():
        say(('%s %s' % (app, entry['app_args'])).rstrip())
        for i, t in sorted(entry['tiles'].items(), key=lambda kv: int(kv[0])):
            say('  %s %-22s %s' % (i, t['name'], '  '.join('%s=%s' %
                                                           (s, t.get(s, '-'))
                                                           for s in STEPS)))
        for step, info in entry['steps'].items():
            for text, n in sorted(info['messages'].items()):
                say('  %s: %dx %s' % (step, n, text))
            if info.get('error'):
                say('  %s: %s' % (step, info['error']))
            if info.get('fault'):
                say('  %s: DEVICE LOSS: %s' % (step, info['fault']))
    if result['stopped']:
        say('STOPPED after a device loss in %(app)s %(step)s' %
            result['stopped'])


if __name__ == '__main__':
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        say('interrupted')
        sys.exit(130)
