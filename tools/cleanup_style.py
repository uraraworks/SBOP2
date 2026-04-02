#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
コードスタイル整理スクリプト
対象: SboCli/src/Info/, SboCli/src/LibInfo/, SboCli/src/AdminApi/

適用項目:
  1. ファイルヘッダをDoxygen形式に変換
  2. 関数ヘッダコメントブロックを削除
  3. 関数名と ( の間の空白を削除
  4. 複数タブによる桁揃えをタブ1つに変更
  5. セクションコメントの簡略化
  6. /* */ コメントを // に統一
"""
import re
import os
import glob


# 制御構文キーワード（空白を保持する）
CONTROL_KEYWORDS = frozenset({
    'if', 'for', 'while', 'switch', 'return', 'case', 'catch',
    'else', 'do', 'try', 'new', 'delete', 'throw', 'sizeof',
    '__if_exists', '__if_not_exists', 'static_assert',
    # マクロ系
    'ASSERT', '_ASSERT', 'SAFE_DELETE', 'SAFE_RELEASE',
    # Windows API マクロ
    'PostMessage', 'SendMessage',
})


def convert_file_header(lines):
    """項目1: ファイルヘッダをDoxygen形式に変換"""
    if not lines:
        return lines

    first_line = lines[0].rstrip('\r\n')
    copyright_match = re.match(r'^/\* (Copyright\(C\).+?) \*/', first_line)
    if not copyright_match:
        return lines

    copyright_text = copyright_match.group(1)

    # 2行目が /* ===... */ ブロックの開始行か確認
    if len(lines) < 2:
        return lines
    if not re.match(r'^/\* =+', lines[1].rstrip('\r\n')):
        return lines

    # ブロック内の情報を抽出
    file_name = None
    content = None
    author = None
    date = None

    i = 2
    while i < len(lines):
        line = lines[i].rstrip('\r\n')
        # 閉じ区切り行
        if re.match(r'^/\* =+\s*\*/', line):
            i += 1
            break
        # ファイル名
        m = re.match(r'^/\*\s*ファイル名\s*:(.*?)\s*\*/', line)
        if m:
            file_name = m.group(1).strip()
        # 内容
        m = re.match(r'^/\*\s*内容\s*:(.*?)\s*\*/', line)
        if m:
            content = m.group(1).strip()
        # 作成
        m = re.match(r'^/\*\s*作成\s*:(.*?)\s*\*/', line)
        if m:
            author = m.group(1).strip()
        # 作成開始日
        m = re.match(r'^/\*\s*作成開始日\s*:(.*?)\s*\*/', line)
        if m:
            date = m.group(1).strip()
        i += 1

    # 新しいDoxygenヘッダを生成
    new_header = []
    if file_name:
        new_header.append(f'/// @file {file_name}\n')
    if content:
        new_header.append(f'/// @brief {content}\n')
    if author:
        new_header.append(f'/// @author {author}\n')
    if date:
        new_header.append(f'/// @date {date}\n')
    new_header.append(f'/// @copyright {copyright_text}\n')

    return new_header + lines[i:]


def remove_file_footer_copyright(lines):
    """ファイル末尾の /* Copyright(C)... */ 行を削除"""
    result = []
    for line in lines:
        if re.match(r'^/\* Copyright\(C\).+\*/$', line.rstrip('\r\n')):
            continue
        result.append(line)
    # 末尾の余分な空行を1行に
    while len(result) > 1 and result[-1].strip() == '' and result[-2].strip() == '':
        result.pop()
    if not result or result[-1].strip() != '':
        result.append('\n')
    return result


def remove_function_headers(lines):
    """項目2: 関数ヘッダコメントブロック（関数名/内容/日付を含む）を削除"""
    result = []
    i = 0
    while i < len(lines):
        line = lines[i].rstrip('\r\n')

        # /* ===... */ の開始行を検出
        if re.match(r'^/\* =+', line):
            block_start = i
            block_lines = [line]
            j = i + 1
            found_close = False

            while j < len(lines):
                bl = lines[j].rstrip('\r\n')
                block_lines.append(bl)
                if re.match(r'^/\* =+\s*\*/', bl):
                    found_close = True
                    j += 1
                    break
                j += 1

            if not found_close:
                # 閉じが見つからない → そのまま出力
                result.append(lines[i])
                i += 1
                continue

            # 関数ヘッダか判定
            is_func_header = any(
                re.match(r'^/\*\s*(関数名|内容|日付|戻り値)\s*:', bl)
                for bl in block_lines
            )

            if is_func_header:
                # 直前の余分な空行を削除
                while result and result[-1].strip() == '':
                    result.pop()
                # 空行1行を追加
                result.append('\n')
                i = j
                continue
            else:
                # セクションコメントなどはそのまま
                result.append(lines[i])
                i += 1
                continue
        else:
            result.append(lines[i])
            i += 1

    return result


def fix_function_call_spaces(lines):
    """項目3: 関数名と ( の間の空白を削除（制御構文は除く）"""
    # 制御構文パターン（これらの後の空白は保持）
    ctrl_pattern = re.compile(
        r'^(if|for|while|switch|return|case|catch|else|do|try|'
        r'sizeof|new|delete|throw|__if_exists|__if_not_exists|'
        r'static_assert)\s*\('
    )

    # 識別子 + スペース + ( のパターン
    # グループ1: 識別子
    func_space_pattern = re.compile(r'([A-Za-z_][A-Za-z0-9_]*) \(')

    result = []
    for line in lines:
        stripped = line.rstrip('\r\n')
        eol = '\n' if line.endswith('\n') else ''

        # // コメント行はそのまま
        if re.match(r'^\s*//', stripped):
            result.append(line)
            continue

        def replace_func_space(m):
            name = m.group(1)
            # 制御構文キーワードは空白保持
            if name in CONTROL_KEYWORDS:
                return m.group(0)
            return name + '('

        new_line = func_space_pattern.sub(replace_func_space, stripped)
        result.append(new_line + eol)

    return result


def fix_tab_alignment(lines):
    """項目4: コード行内の複数タブを1タブに変更（インデントは保持）"""
    result = []
    for line in lines:
        stripped = line.rstrip('\r\n')
        eol = '\n' if line.endswith('\n') else ''

        # インデント部分（先頭タブ）を保持
        m = re.match(r'^(\t*)', stripped)
        indent_tabs = m.group(1) if m else ''
        rest = stripped[len(indent_tabs):]

        # インデント以降の複数タブを1つに
        rest_new = re.sub(r'\t{2,}', '\t', rest)

        result.append(indent_tabs + rest_new + eol)

    return result


def simplify_section_comments(lines):
    """項目5: 3行のセクションコメントブロックを1行の // コメントに"""
    result = []
    i = 0
    while i < len(lines):
        line = lines[i].rstrip('\r\n')

        # /* ===...=== */ 単独行を検出
        if re.match(r'^/\* =+\s*\*/$', line):
            # 次の2行を確認
            if i + 2 < len(lines):
                mid_line = lines[i + 1].rstrip('\r\n')
                end_line = lines[i + 2].rstrip('\r\n')
                m = re.match(r'^/\*\s+(.+?)\s*\*/$', mid_line)
                if m and re.match(r'^/\* =+\s*\*/$', end_line):
                    heading = m.group(1).strip()
                    result.append(f'// {heading}\n')
                    i += 3
                    continue

        result.append(lines[i])
        i += 1

    return result


def convert_block_comments(lines):
    """項目6: 単独行の /* ... */ コメントを // に変換"""
    result = []
    for line in lines:
        stripped = line.rstrip('\r\n')
        eol = '\n' if line.endswith('\n') else ''

        # 行全体が /* ... */ コメント（インデントあり可）
        m = re.match(r'^(\s*)/\*\s*(.*?)\s*\*/\s*$', stripped)
        if m:
            ind = m.group(1)
            content = m.group(2)
            result.append(f'{ind}// {content}{eol}')
            continue

        # コード + 末尾コメント: code;   /* コメント */
        # タブ区切りで末尾に /* ... */ がある場合
        m2 = re.match(r'^(.*\S)(\s+)/\*\s*(.*?)\s*\*/\s*$', stripped)
        if m2:
            code_part = m2.group(1)
            space_part = m2.group(2)
            comment = m2.group(3)
            # タブを含むスペース区切りの場合のみ変換
            if '\t' in space_part:
                result.append(f'{code_part}{space_part}// {comment}{eol}')
                continue

        result.append(line)

    return result


def process_file(filepath):
    """ファイルを読み込んで全変換を適用して書き戻す"""
    # AdminUiApi.h は内容が短くヘッダのみなのでスキップ判定不要
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        encoding = 'utf-8'
    except UnicodeDecodeError:
        with open(filepath, 'r', encoding='cp932') as f:
            content = f.read()
        encoding = 'cp932'

    lines = content.splitlines(keepends=True)
    original_count = len(lines)

    print(f'処理中: {os.path.basename(filepath)} ({original_count}行, {encoding})')

    lines = convert_file_header(lines)
    lines = remove_file_footer_copyright(lines)
    lines = remove_function_headers(lines)
    lines = simplify_section_comments(lines)
    lines = convert_block_comments(lines)
    lines = fix_tab_alignment(lines)
    lines = fix_function_call_spaces(lines)

    with open(filepath, 'w', encoding=encoding) as f:
        f.writelines(lines)

    print(f'  完了: {len(lines)}行 (元: {original_count}行)')


if __name__ == '__main__':
    base = 'C:/Users/masakazu/MyProject/SBOP2/SboCli/src'
    target_dirs = [
        f'{base}/Info',
        f'{base}/Info/InfoChar',
        f'{base}/LibInfo',
        f'{base}/AdminApi',
    ]

    files = []
    for d in target_dirs:
        for ext in ('*.h', '*.cpp'):
            files.extend(glob.glob(os.path.join(d, ext)))

    for f in sorted(files):
        process_file(f)

    print('\n全ファイル処理完了')
