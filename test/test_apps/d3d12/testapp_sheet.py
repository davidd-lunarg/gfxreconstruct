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
Write a results file of testapp_suite.py as a spreadsheet beside it:

  python testapp_sheet.py <output-dir>/<label>.json

writes <output-dir>/<label>.xlsx and refuses one that exists. The sheet opens
with the run's information (revisions, date, GPU and driver; no machine or
user name), then the verdict of every tile at every step as a colored code
with the details in a cell note, every log message, and the legend of the
codes. testapp_suite.py --xlsx writes the same at the end of a run. Needs
openpyxl.
'''

import argparse
import importlib.util
import json
import os
import re
import subprocess
import sys

# Sheet code: label, fill and font colors.
STYLE = {
    'PASS': ('PASS', 'C6EFCE', '006100'),
    'PASS*': ('PASS*', 'FFEB9C', '9C5700'),
    'FAIL': ('FAIL', 'FFC7CE', '9C0006'),
    'BLACK': ('BLACK', 'FFC7CE', '9C0006'),
    'HANG': ('HANG', 'FFC7CE', '9C0006'),
    'REMOVED': ('REMOVED', 'FFC7CE', '9C0006'),
    'CRASH': ('CRASH', 'FFC7CE', '9C0006'),
    '-': ('-', 'F2F2F2', 'A6A6A6'),
    'NB': ('n/b', 'D9D9D9', '808080'),
}
LEGEND = (
    'PASS: the tile passed on every frame. PASS*: it passed, and the step '
    'logged an ERROR or FATAL for the app. FAIL: a fail color or a mixed '
    'tile. BLACK: the tile drew nothing. REMOVED, HANG, CRASH: the step lost '
    'the device or ended early. n/b: not built. -: not run, or left out by '
    '--case. Hover a cell for details.')


def sheet_code(step, tile, info):
    '''Sheet code and cell note of one tile at one step.'''
    if info is None:
        return '-', None
    verdict = tile.get(step)
    fault = info.get('fault') or ''
    errors = {
        k: n
        for k, n in (info.get('messages') or {}).items()
        if k.startswith(('ERROR', 'FATAL'))
    }
    built = 'not built' not in (tile.get('native'), tile.get('capture'))
    selected = 'disabled' not in (tile.get('native'), tile.get('capture'))
    error = info.get('error') or ''
    if (verdict is None) or verdict.startswith(('incomplete', 'no frames')):
        if (verdict is None) and not fault and not error:
            return '-', None
        code = 'CRASH'
        if 'HUNG' in fault:
            code = 'HANG'
        elif ('REMOVED' in fault) or ('0x887a0005' in fault):
            code = 'REMOVED'
        return code, '; '.join(x for x in (verdict, fault, error) if x)
    if (verdict in ('not built', 'disabled')) or (
        (verdict == 'empty') and not (built and selected)):
        return ('NB', None) if not built else ('-', None)
    code = {
        'pass': 'PASS',
        'not run': 'BLACK',
        'empty': 'BLACK',
        'mixed': 'FAIL'
    }.get(verdict, 'FAIL')
    notes = [verdict] if verdict.startswith('fail') else []
    if (code == 'PASS') and errors:
        code = 'PASS*'
    frames = tile.get(step + '_frames')
    if frames and (set(frames) != {'pass'}):
        notes.append('frames: ' + ', '.join('%s x%d' % kv
                                            for kv in frames.items()))
    if errors:
        notes.append('log (whole app): ' +
                     '; '.join('%dx %s' % (n, k) for k, n in errors.items()))
    return code, '. '.join(notes) or None


def adapter_of(result):
    '''(name, driver) of the adapter the apps reported.'''
    for entry in result['apps'].values():
        for info in entry['steps'].values():
            for line in info.get('report', []):
                m = re.search(r': adapter (.+)$', line)
                if m:
                    name = m.group(1)
                    for gpu in result.get('gpus') or []:
                        v = re.match(
                            re.escape(name) + r', ver: (.*), date: (\d*)$',
                            gpu)
                        if v:
                            version, date = v.groups()
                            if len(date) == 8:
                                version += ' (%s-%s-%s)' % (
                                    date[:4], date[4:6], date[6:])
                            return name, version
                    return name, 'not listed by the system'
    return 'not reported', ''


def revision_rows(what, rev):
    '''The branch and commit rows of one checkout.'''
    if rev is None:
        return [(what + ' branch', 'not a git checkout')]
    commit = '%s %s' % (rev['commit'], rev['subject'])
    if rev['modified']:
        commit += ' (with local changes)'
    return [(what + ' branch', rev['branch']), (what + ' commit', commit)]


def run_rows(result):
    '''
    The (key, value) rows of run information the sheet opens with. No host,
    user or path: a sheet may be shared.
    '''
    rows = revision_rows('GFXR', result.get('gfxr'))
    if result.get('testapps') != result.get('gfxr'):
        rows += revision_rows('Test apps', result.get('testapps'))
    name, driver = adapter_of(result)
    if result['warp']:
        name += ' (WARP)'
        warp_dll = result.get('warp_dll', 'not recorded')
        driver = (warp_dll + ' beside the apps'
                  if warp_dll else 'the WARP that ships with Windows')
    rows += [('Date', result['started'].replace('T', ' ')), ('GPU', name),
             ('Driver', driver), ('Config', result.get('config', '')),
             ('Replay args', result['replay_args'])]
    if result['stopped']:
        rows.append(
            ('Stopped',
             'after a device loss in %(app)s %(step)s' % result['stopped']))
    return rows


def write_xlsx(result, path):
    '''Write the Results sheet of result to the new file path.'''
    from openpyxl import Workbook
    from openpyxl.comments import Comment
    from openpyxl.styles import Alignment, Border, Font, PatternFill, Side

    thin = Side(style='thin', color='BFBFBF')
    box = Border(left=thin, right=thin, top=thin, bottom=thin)
    head_fill = PatternFill('solid', fgColor='404040')
    head_font = Font(bold=True, color='FFFFFF')
    center = Alignment(horizontal='center', vertical='center', wrap_text=True)
    wrap = Alignment(wrap_text=True, vertical='top')

    wb = Workbook()
    ws = wb.active
    ws.title = 'Results'
    ws['A1'] = 'D3D12 test app suite'
    ws['A1'].font = Font(bold=True, size=12)
    row = 2
    for key, value in run_rows(result):
        ws.cell(row=row, column=1, value=key).font = Font(bold=True)
        ws.cell(row=row, column=2, value=value)
        row += 1

    row += 1
    steps = result['steps']
    header = ['App', 'Tile', 'Name'] + [s.capitalize() for s in steps]
    for col, text in enumerate(header, start=1):
        cell = ws.cell(row=row, column=col, value=text)
        cell.fill, cell.font, cell.alignment = head_fill, head_font, center
    ws.freeze_panes = 'A%d' % (row + 1)
    row += 1
    for app, entry in result['apps'].items():
        for index, tile in sorted(entry['tiles'].items(),
                                  key=lambda kv: int(kv[0])):
            ws.cell(row=row, column=1, value=app).border = box
            ws.cell(row=row, column=2, value=int(index)).border = box
            ws.cell(row=row, column=3, value=tile['name']).border = box
            for col, step in enumerate(steps, start=4):
                code, detail = sheet_code(step, tile, entry['steps'].get(step))
                text, bg, fg = STYLE[code]
                cell = ws.cell(row=row, column=col, value=text)
                cell.fill = PatternFill('solid', fgColor=bg)
                cell.font = Font(bold=code not in ('-', 'NB'), color=fg)
                cell.alignment, cell.border = center, box
                if detail:
                    cell.comment = Comment(detail, 'testapp_suite')
                    cell.comment.width, cell.comment.height = 360, 140
            row += 1

    # Every message and fault, since the cell notes show them only on hover.
    row += 1
    for col, text in enumerate(['App', 'Step', 'Count', 'Log message'],
                               start=1):
        cell = ws.cell(row=row, column=col, value=text)
        cell.fill, cell.font, cell.alignment = head_fill, head_font, center
    for app, entry in result['apps'].items():
        for step, info in entry['steps'].items():
            lines = sorted((info.get('messages') or {}).items())
            if info.get('fault'):
                lines.append(('FAULT: ' + info['fault'], 1))
            if info.get('error'):
                lines.append((info['error'], 1))
            for text, count in lines:
                row += 1
                ws.cell(row=row, column=1, value=app)
                ws.cell(row=row, column=2, value=step)
                ws.cell(row=row, column=3, value=count)
                ws.cell(row=row, column=4, value=text)
                ws.merge_cells(start_row=row,
                               start_column=4,
                               end_row=row,
                               end_column=7)

    row += 2
    ws.cell(row=row, column=1, value=LEGEND).alignment = wrap
    ws.merge_cells(start_row=row, start_column=1, end_row=row, end_column=7)
    ws.row_dimensions[row].height = 45

    for col, width in zip('ABCDEFG', (28, 6, 22, 12, 12, 12, 12)):
        ws.column_dimensions[col].width = width
    with open(path, 'xb') as f:
        wb.save(f)


def main():
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('results',
                        metavar='JSON',
                        help='a results file of testapp_suite.py')
    args = parser.parse_args()
    if importlib.util.find_spec('openpyxl') is None:
        print('testapp_sheet: needs openpyxl, which this Python lacks. '
              'Install it with:\n  %s' % subprocess.list2cmdline(
                  [sys.executable, '-m', 'pip', 'install', 'openpyxl']),
              file=sys.stderr)
        return 2
    xlsx_path = os.path.splitext(args.results)[0] + '.xlsx'
    if os.path.lexists(xlsx_path):
        print('%s already exists; move it away first' % xlsx_path)
        return 2
    with open(args.results) as f:
        result = json.load(f)
    write_xlsx(result, xlsx_path)
    print('wrote ' + xlsx_path)
    return 0


if __name__ == '__main__':
    sys.exit(main())
