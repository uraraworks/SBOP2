#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
SboCliAdminMfc コードスタイル整理スクリプト
以下の6項目を適用する:
  1. ファイルヘッダをDoxygen形式に変換
  2. 関数ヘッダコメントの削除
  3. 関数呼び出し時の空白を削除
  4. タブによる位置合わせを解除
  5. セクションコメントの簡略化
  6. /* */ コメントを // に統一
"""

import re
import sys
import os

# ========== 項目1: ファイルヘッダをDoxygen形式に変換 ==========

def convert_file_header(text):
    """
    ファイル先頭の旧式ブロックヘッダをDoxygen形式 /// に変換する。
    ファイル末尾の /* Copyright(...) */ 行も削除する。
    """

    # ファイル末尾の /* Copyright(C)... */ 行を削除（空行も含む末尾の整理）
    text = re.sub(r'\n/\* Copyright\(C\)[^\n]*\*/[ \t]*\n?$', '', text)
    # 行単独で存在する場合も削除
    text = re.sub(r'^/\* Copyright\(C\)[^\n]*\*/[ \t]*\n', '', text, flags=re.MULTILINE)

    # ファイル先頭ブロックヘッダのパターン
    # /* Copyright(C)URARA-works YYYY */
    # /* ===...=== */
    # /* ファイル名 :XXX */
    # /* 内容       :YYY */
    # /* 作成       :ZZZ */   (省略されることもある)
    # /* 作成開始日 :YYYY/MM/DD */ (省略されることもある)
    # /* ===...=== */

    pattern = re.compile(
        r'^/\* Copyright\(C\)([^\n]*?) \*/\s*\n'   # group1: copyright holder+year
        r'/\* =+[^\n]* \*/\s*\n'                    # 区切り線
        r'(?:/\*[ \t]*ファイル名[ \t]*:([^\n]*?)\s*\*/\s*\n)?'  # group2: ファイル名(省略可)
        r'(?:/\*[ \t]*内容[ \t]*:([^\n]*?)\s*\*/\s*\n)?'        # group3: 内容(省略可)
        r'(?:/\*[ \t]*作成[ \t]*:([^\n]*?)\s*\*/\s*\n)?'        # group4: 作成者(省略可)
        r'(?:/\*[ \t]*作成開始日[ \t]*:([^\n]*?)\s*\*/\s*\n)?'  # group5: 作成開始日(省略可)
        r'/\* =+[^\n]* \*/\s*\n',                   # 区切り線
        re.MULTILINE
    )

    def replace_header(m):
        copyright_str = m.group(1).strip()  # e.g. "URARA-works 2008"
        filename = m.group(2).strip() if m.group(2) else None
        content  = m.group(3).strip() if m.group(3) else None
        author   = m.group(4).strip() if m.group(4) else None
        date     = m.group(5).strip() if m.group(5) else None

        lines = []
        if filename:
            lines.append(f'/// @file {filename}')
        if content:
            lines.append(f'/// @brief {content}')
        if author:
            lines.append(f'/// @author {author}')
        if date:
            lines.append(f'/// @date {date}')
        lines.append(f'/// @copyright Copyright(C){copyright_str}')
        lines.append('')  # 空行1つ

        return '\n'.join(lines) + '\n'

    text = pattern.sub(replace_header, text, count=1)
    return text


# ========== 項目2: 関数ヘッダコメントの削除 ==========

def remove_function_headers(text):
    """
    関数ごとの定型コメントブロック（/* ==...== */ 〜 /* ==...== */）とその前後の
    余分な空行を削除し、関数間は空行1行だけ残す。
    セクションコメントブロック（内容が短い見出し）は項目5で処理するので、
    ここでは関数名・内容・日付行を持つブロックを対象にする。
    """

    # パターン: (空行複数) /* ===...=== */\n/* 関数名... */\n... /* ===...=== */\n(空行0以上)
    # 関数ヘッダブロックは「関数名」「内容」「日付」のいずれかを含む
    func_header_pattern = re.compile(
        r'\n{2,}'                          # 前の空行(2行以上)
        r'(?P<block>'
            r'/\* =+[^\n]* \*/\s*\n'       # 区切り線1
            r'(?:/\*[^\n]+\*/\s*\n)+'      # 内容行(1行以上)
            r'/\* =+[^\n]* \*/\s*\n'       # 区切り線2
        r')'
        r'\n*',                            # 後の空行
    )

    def should_remove(block_text):
        # 「関数名」または「内容」または「日付」を含む → 関数ヘッダ → 削除
        if re.search(r'関数名|内容|日付', block_text):
            return True
        return False

    def replace_func_header(m):
        block = m.group('block')
        if should_remove(block):
            return '\n\n'  # 空行1行だけ残す
        # 削除しない場合はそのまま返す(セクション等は項目5で処理)
        return m.group(0)

    text = func_header_pattern.sub(replace_func_header, text)

    # ファイル先頭直後に関数ヘッダが来る場合(空行なし)も処理
    func_header_start = re.compile(
        r'^(?P<block>/\* =+[^\n]* \*/\s*\n(?:/\*[^\n]+\*/\s*\n)+/\* =+[^\n]* \*/\s*\n)\n*',
        re.MULTILINE
    )
    def replace_func_header_start(m):
        block = m.group('block')
        if should_remove(block):
            return ''
        return m.group(0)
    text = func_header_start.sub(replace_func_header_start, text)

    return text


# ========== 項目3: 関数呼び出し時の空白を削除 ==========

def remove_call_spaces(text):
    """
    関数名と ( の間の空白を削除する。
    制御構文 (if/for/while/switch/catch/return) の空白は残す。
    sizeof の空白も削除する。
    """
    # 制御構文キーワード
    control_keywords = {
        'if', 'else', 'for', 'while', 'switch', 'case', 'catch',
        'return', 'throw', 'do', 'try', 'new', 'delete',
        '__if_exists', '__if_not_exists',
    }

    # 関数呼び出し空白除去: 識別子 + 空白 + ( のパターン
    # ただし制御構文キーワードは除く
    def remove_space(m):
        word = m.group(1)
        if word in control_keywords:
            return m.group(0)  # そのまま
        return word + '('

    # 識別子（英数字+アンダースコア）の後の空白1つ以上 + (
    text = re.sub(r'\b([A-Za-z_]\w*)\s+\(', remove_space, text)

    return text


# ========== 項目4: タブによる位置合わせを解除 ==========

def remove_tab_alignment(text):
    """
    複数タブによる桁揃えをタブ1つに変更する。
    コメント内や文字列内の複数タブも対象（ただし慎重に）。
    行内で「タブが2つ以上連続」する箇所をタブ1つに変換。
    """
    # 2つ以上連続するタブをタブ1つに変換
    text = re.sub(r'\t{2,}', '\t', text)
    return text


# ========== 項目5: セクションコメントの簡略化 ==========

def simplify_section_comments(text):
    """
    3行の区切りブロック（/* ===...=== */\n/* 見出し */\n/* ===...=== */）を
    // 見出し の1行に変換する。
    """

    # セクションコメントパターン（関数ヘッダではない、短い見出し行を持つもの）
    pattern = re.compile(
        r'/\* =+[^\n]* \*/\s*\n'              # 区切り線1
        r'/\*[ \t]*([^\n]+?)[ \t]*\*/\s*\n'   # 見出し行 (group1)
        r'/\* =+[^\n]* \*/[ \t]*\n?',         # 区切り線2
    )

    def replace_section(m):
        heading = m.group(1).strip()
        return f'// {heading}\n'

    text = pattern.sub(replace_section, text)
    return text


# ========== 項目6: /* */ コメントを // に統一 ==========

def convert_block_comments(text):
    """
    /* ... */ 形式の1行コメントを // に変換する。
    複数行にまたがるブロックコメントはそのまま（分割が複雑なため）。
    ただし、行末の /* xxx */ スタイルや行単独の /* xxx */ を変換する。
    """

    lines = text.split('\n')
    result = []
    for line in lines:
        result.append(convert_line_block_comment(line))
    return '\n'.join(result)


def convert_line_block_comment(line):
    """
    1行内の /* ... */ コメントを // に変換する。
    文字列リテラル内は対象外（簡易処理）。
    """
    # 既に // コメントが含まれる行はそのまま
    # 行全体が /* ... */ の場合
    # 行末に /* ... */ がある場合

    # 文字列・既存の//コメントがある行の検出
    # 簡易的に: //が先に来る行はスキップ
    stripped = line.lstrip()
    if stripped.startswith('//'):
        return line  # 既に//コメント

    # /* ... */ を // ... に変換（1行内に収まる場合のみ）
    # パターン: 行頭に /* ... */ のみ（他にコードがない）
    # または行末に /* ... */ がある

    # 行全体が /* ... */ (インデント含む)
    m = re.match(r'^(\s*)/\*\s*(.*?)\s*\*/\s*$', line)
    if m:
        indent = m.group(1)
        comment = m.group(2)
        # 空コメントは空行に
        if not comment:
            return ''
        return f'{indent}// {comment}'

    # 行末コメント: code /* comment */
    # 文字列内の /* を誤変換しないよう、簡易的に処理
    # ※ 文字列内の /* は無視（完全な解析は行わない）
    m = re.search(r'\s*/\*\s*(.*?)\s*\*/\s*$', line)
    if m:
        comment_part = m.group(1)
        before = line[:m.start()]
        # 文字列リテラル内かチェック（簡易: " の数が偶数かどうか）
        quote_count = before.count('"') - before.count('\\"')
        if quote_count % 2 == 0:  # 文字列外
            # タブ/スペースの調整
            before_stripped = before.rstrip()
            if comment_part:
                return f'{before_stripped}\t// {comment_part}'
            else:
                return before_stripped

    return line


# ========== メイン処理 ==========

def process_file(filepath):
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        text = f.read()

    original = text

    # 各変換を順番に適用
    text = convert_file_header(text)
    text = remove_function_headers(text)
    text = simplify_section_comments(text)
    text = convert_block_comments(text)
    text = remove_call_spaces(text)
    text = remove_tab_alignment(text)

    # 末尾の過剰な空行を整理（最大1行）
    text = re.sub(r'\n{3,}', '\n\n', text)
    # ファイル末尾は改行1つで終わるように
    text = text.rstrip('\n') + '\n'

    if text != original:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(text)
        return True
    return False


def main():
    import glob

    base = 'C:/Users/masakazu/MyProject/SBOP2/SboCliAdminMfc'
    files = []
    for pattern in ['**/*.h', '**/*.cpp']:
        files.extend(glob.glob(os.path.join(base, pattern), recursive=True))
    files.sort()

    print(f'対象ファイル数: {len(files)}')
    changed = 0
    for f in files:
        try:
            if process_file(f):
                print(f'  変更: {os.path.relpath(f, base)}')
                changed += 1
            else:
                print(f'  変更なし: {os.path.relpath(f, base)}')
        except Exception as e:
            print(f'  エラー: {os.path.relpath(f, base)}: {e}')

    print(f'\n完了: {changed}/{len(files)} ファイルを変更しました。')


if __name__ == '__main__':
    main()
