#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
SboSvr/ 以下の全 .h/.cpp ファイルのコードスタイルを整理するスクリプト

適用する整理項目:
  1. ファイルヘッダをDoxygen形式に変換
  2. 関数ヘッダコメントの削除
  3. 関数呼び出し時の空白を削除（制御構文は除く）
  4. タブによる位置合わせを解除（複数タブ → 1タブ）
  5. セクションコメントの簡略化（3行ブロック → 1行）
  6. /* */ コメントを // に統一
"""

import re
import os
import sys

# 変換対象ディレクトリ
TARGET_DIR = r"C:/Users/masakazu/MyProject/SBOP2/SboSvr"

# 制御構文キーワード（空白を残す対象）
CONTROL_KEYWORDS = {"if", "else", "for", "while", "do", "switch", "return",
                    "catch", "throw", "delete", "new", "case", "typedef", "struct"}


def parse_file_header(lines):
    """
    先頭の旧式ヘッダを解析してDoxygen形式の文字列を返す。
    戻り値: (doxygen_str, consumed_line_count)
    解析失敗時は (None, 0)
    """
    # 1行目: /* Copyright(C)URARA-works YYYY */
    if len(lines) < 1:
        return None, 0

    m_copy = re.match(r'/\*\s*Copyright\(C\)(.*?)\s*\*/', lines[0].rstrip())
    if not m_copy:
        return None, 0

    copyright_text = "Copyright(C)" + m_copy.group(1).strip()
    # 2行目: /* ===...=== */
    if len(lines) < 2 or not re.match(r'/\*\s*=+\s*\*/', lines[1].rstrip()):
        return None, 0

    # 3行目〜: /* キー : 値 */ 形式の行を収集、最後の /* ===...=== */ まで
    idx = 2
    fields = {}
    field_order = []
    while idx < len(lines):
        line = lines[idx].rstrip()
        # 終端の区切り行
        if re.match(r'/\*\s*=+\s*\*/', line):
            idx += 1  # 区切り行を消費
            break
        # フィールド行: /* キー\t:値\t */
        m = re.match(r'/\*\s*([\u3000-\u9fff\w]+)\s+(.*?)\s*\*/', line)
        if m:
            key = m.group(1).strip()
            val = m.group(2).strip()
            # タブで区切られている場合は : の後を値として取る
            # パターン: ファイル名\t:SboSvr.h\t... → key=ファイル名, val=:SboSvr.h
            val = re.sub(r'^:\s*', '', val)
            # 末尾の空白/タブを除去
            val = val.rstrip()
            fields[key] = val
            field_order.append(key)
        idx += 1

    # Doxygen形式の文字列を組み立て
    lines_out = []

    filename = fields.get("ファイル名", "")
    brief = fields.get("内容", "")
    author = fields.get("作成", "")
    date = fields.get("作成開始日", "")

    if filename:
        lines_out.append(f"/// @file {filename}")
    if brief:
        lines_out.append(f"/// @brief {brief}")
    if author:
        lines_out.append(f"/// @author {author}")
    if date:
        lines_out.append(f"/// @date {date}")
    lines_out.append(f"/// @copyright {copyright_text}")

    return "\n".join(lines_out), idx


def remove_func_header(lines):
    """
    関数ヘッダコメントブロック（/* ===...=== */ ... /* ===...=== */）を削除する。
    関数間は空行1行だけ残す。
    """
    result = []
    i = 0
    n = len(lines)
    while i < n:
        line = lines[i]
        stripped = line.strip()
        # セクション区切り（定数定義、クラス宣言など）はスキップしない
        # 関数ヘッダパターン: /* ===...=== */ で始まり、内部に 関数名/内容/日付 などを含む
        if re.match(r'/\*\s*=+\s*\*/', stripped):
            # 次の /* ===...=== */ までを読んで、関数ヘッダかどうか判定
            j = i + 1
            block_lines = [line]
            is_func_header = False
            while j < n:
                bline = lines[j].strip()
                block_lines.append(lines[j])
                if re.match(r'/\*\s*=+\s*\*/', bline):
                    # ブロック終端
                    # 内部に 関数名 キーワードがあれば関数ヘッダとみなす
                    block_text = "".join(block_lines)
                    if re.search(r'関数名|Function|function', block_text):
                        is_func_header = True
                    j += 1
                    break
                j += 1
            if is_func_header:
                # 関数ヘッダを削除（ブロック全体をスキップ）
                i = j
                # 直前の余分な空行も削除（resultの末尾の空行を1行にする）
                while result and result[-1].strip() == "":
                    result.pop()
                # 空行を1行追加（次の関数との区切り）
                result.append("\n")
                # ブロック後の空行もスキップ
                while i < n and lines[i].strip() == "":
                    i += 1
                continue
            else:
                # 関数ヘッダでない（セクションヘッダ等）はそのまま保持
                result.extend(block_lines)
                i = j
                continue
        result.append(line)
        i += 1
    return result


def simplify_section_comments(lines):
    """
    3行の区切りブロック（/* ===...=== */ / /* 見出し */ / /* ===...=== */）
    を // 見出し 1行に変換する。
    ただし関数ヘッダは既に削除済みなので、残るのはセクションヘッダのみ。
    """
    result = []
    i = 0
    n = len(lines)
    while i < n:
        line = lines[i]
        stripped = line.strip()
        if re.match(r'/\*\s*=+\s*\*/', stripped):
            # 3行パターン: 区切り / 内容 / 区切り
            if i + 2 < n:
                mid = lines[i + 1].strip()
                end = lines[i + 2].strip()
                if re.match(r'/\*\s*=+\s*\*/', end):
                    # 内容行からテキストを取り出す
                    m = re.match(r'/\*+\s*(.*?)\s*\*/', mid)
                    if m:
                        heading = m.group(1).strip()
                        if heading:
                            result.append(f"// {heading}\n")
                        i += 3
                        continue
                    else:
                        # 空の中間行
                        i += 3
                        continue
            # 単独の区切り行はそのまま（後で /* */ -> // 変換で処理）
        result.append(line)
        i += 1
    return result


def convert_block_comments(text):
    """
    /* ... */ コメントを // コメントに変換する。
    ただし、文字列リテラル内、マクロ継続行、プリプロセッサ行内のものは除外。
    単一行の /* ... */ かつ行末にあるものを変換。
    複数行ブロックコメントは行ごとに // に変換。
    """
    lines = text.split("\n")
    result = []
    in_block_comment = False
    block_content = []
    block_indent = ""

    for line in lines:
        stripped = line.rstrip()

        if in_block_comment:
            # ブロックコメント内
            end_idx = stripped.find("*/")
            if end_idx != -1:
                # ブロックコメント終了
                content = stripped[:end_idx].strip().lstrip("*").strip()
                if content:
                    block_content.append(content)
                in_block_comment = False
                # ブロックコメント後の残りのコード
                after = stripped[end_idx + 2:].strip()
                # 複数行ブロックコメントを // 行に変換
                for bc in block_content:
                    result.append(f"{block_indent}// {bc}")
                block_content = []
                if after:
                    # コメントの後にコードが続く場合はそのまま追加
                    result.append(f"{block_indent}{after}")
            else:
                content = stripped.strip().lstrip("*").strip()
                if content:
                    block_content.append(content)
            continue

        # 行内に /* があるか確認
        # まず文字列リテラルの外かどうかを簡易チェック
        # 文字列やプリプロセッサのケースを除外（完全ではないが実用的）

        # 行全体が /* で始まるブロックコメント開始
        if re.match(r'^\s*/\*', stripped):
            # 同じ行で */ で終わるか？
            content_after_open = stripped[stripped.index("/*") + 2:]
            end_idx = content_after_open.find("*/")
            if end_idx != -1:
                # 1行完結ブロックコメント
                comment_body = content_after_open[:end_idx].strip()
                indent = re.match(r'^(\s*)', stripped).group(1)
                after = content_after_open[end_idx + 2:].strip()
                if comment_body:
                    result.append(f"{indent}// {comment_body}")
                if after:
                    result.append(f"{indent}{after}")
            else:
                # 複数行ブロックコメント開始
                indent = re.match(r'^(\s*)', stripped).group(1)
                block_indent = indent
                in_block_comment = True
                content = content_after_open.strip().lstrip("*").strip()
                if content:
                    block_content.append(content)
            continue

        # 行末の /* ... */ コメント（コード行の後）
        # 例: int x;  /* 変数 */
        # インライン /* */ を // に変換
        new_line = convert_inline_block_comment(stripped)
        result.append(new_line)

    return "\n".join(result)


def convert_inline_block_comment(line):
    """
    行内の /* ... */ を // ... に変換する。
    文字列リテラル内のものは除外（簡易実装）。
    """
    # 既に // コメントがある行はスキップ
    # 文字列リテラルを考慮した簡易パース
    result = []
    i = 0
    n = len(line)
    in_string = False
    in_char = False
    string_char = None

    while i < n:
        c = line[i]

        if in_string:
            result.append(c)
            if c == '\\':
                i += 1
                if i < n:
                    result.append(line[i])
            elif c == string_char:
                in_string = False
            i += 1
            continue

        if c in ('"', "'"):
            in_string = True
            string_char = c
            result.append(c)
            i += 1
            continue

        # // コメント開始
        if c == '/' and i + 1 < n and line[i + 1] == '/':
            # 残りはそのまま
            result.append(line[i:])
            break

        # /* コメント開始
        if c == '/' and i + 1 < n and line[i + 1] == '*':
            end_idx = line.find("*/", i + 2)
            if end_idx != -1:
                comment_body = line[i + 2:end_idx].strip()
                after = line[end_idx + 2:].strip()
                # インラインコメントの前の部分を確認
                code_part = "".join(result).rstrip()
                if code_part:
                    # コードの後のコメント: code  /* comment */ → code  // comment
                    result = list(code_part)
                    if comment_body:
                        result.append(f"\t// {comment_body}")
                    if after:
                        result.append(f" {after}")
                else:
                    # コードなしで /* comment */ → // comment
                    if comment_body:
                        # インデントを保持
                        orig_indent = re.match(r'^(\s*)', line).group(1)
                        result = list(orig_indent)
                        result.append(f"// {comment_body}")
                    if after:
                        result.append(f" {after}")
                i = end_idx + 2
                # 残りの文字列を処理
                rest = line[i:].strip()
                if rest:
                    result.append(f" {rest}")
                break
            else:
                # */ がない場合はそのまま
                result.append(c)
                i += 1
                continue

        result.append(c)
        i += 1

    return "".join(result)


def remove_func_call_spaces(line):
    """
    関数呼び出し時の空白を削除する。
    制御構文キーワードの後の空白は残す。
    例: MainLoop (hInstance) → MainLoop(hInstance)
    例: sizeof (int) → sizeof(int)
    例: if (x) → そのまま（制御構文）
    """
    # パターン: 識別子の後にスペース + ( が続く場合
    # ただし制御構文は除く
    def replace_func_space(m):
        word = m.group(1)
        if word in CONTROL_KEYWORDS:
            return m.group(0)  # そのまま返す
        return word + "("

    # 識別子 + 1つ以上のスペース + (
    result = re.sub(r'\b(\w+)\s+\(', replace_func_space, line)
    return result


def collapse_alignment_tabs(line):
    """
    複数タブによる桁揃えをタブ1つに変換する。
    ただし行頭のインデントタブは保持する。
    コメント内の調整は対象外（簡易実装として行全体に適用）。
    """
    # 行頭インデントを保持
    m = re.match(r'^(\t*)(.*)', line, re.DOTALL)
    if not m:
        return line
    indent = m.group(1)
    rest = m.group(2)

    # rest 内の複数タブを1タブに変換
    # ただし // コメント内は変換しない
    # 簡易実装: // の前後に分けて処理
    comment_idx = rest.find("//")
    if comment_idx >= 0:
        code_part = rest[:comment_idx]
        comment_part = rest[comment_idx:]
        code_part = re.sub(r'\t{2,}', '\t', code_part)
        rest = code_part + comment_part
    else:
        rest = re.sub(r'\t{2,}', '\t', rest)

    return indent + rest


def remove_trailing_footer(lines):
    """
    ファイル末尾の /* Copyright(C)URARA-works YYYY */ 行を削除する。
    """
    result = list(lines)
    # 末尾から空行を含めて確認
    while result and result[-1].strip() == "":
        result.pop()
    if result:
        last = result[-1].strip()
        if re.match(r'/\*\s*Copyright\(C\).*\*/', last):
            result.pop()
    # 末尾の余分な空行を除去
    while result and result[-1].strip() == "":
        result.pop()
    # ファイル末尾に改行を1つ追加
    return result


def process_file(filepath):
    """ファイルを読み込んで整形して書き戻す"""
    try:
        with open(filepath, "r", encoding="utf-8-sig", errors="replace") as f:
            content = f.read()
    except Exception as e:
        print(f"読み込みエラー: {filepath}: {e}")
        return False

    lines = content.split("\n")
    # 行末の \r を除去
    lines = [l.rstrip("\r") for l in lines]

    # --- 項目1: ファイルヘッダをDoxygen形式に変換 ---
    doxygen_header, consumed = parse_file_header(lines)
    if doxygen_header and consumed > 0:
        lines = doxygen_header.split("\n") + [""] + lines[consumed:]

    # --- 項目5: セクションコメントの簡略化 ---
    # (関数ヘッダ削除の前にセクションヘッダを簡略化)
    lines = simplify_section_comments(lines)

    # --- 項目2: 関数ヘッダコメントの削除 ---
    lines = remove_func_header(lines)

    # --- 項目6: /* */ コメントを // に統一 ---
    text = "\n".join(lines)
    text = convert_block_comments(text)
    lines = text.split("\n")

    # --- 項目3: 関数呼び出し時の空白を削除 ---
    new_lines = []
    for line in lines:
        new_lines.append(remove_func_call_spaces(line))
    lines = new_lines

    # --- 項目4: タブによる位置合わせを解除 ---
    new_lines = []
    for line in lines:
        new_lines.append(collapse_alignment_tabs(line))
    lines = new_lines

    # --- 末尾フッターの削除 ---
    lines = remove_trailing_footer(lines)

    # 連続する空行を最大2行に制限
    new_lines = []
    blank_count = 0
    for line in lines:
        if line.strip() == "":
            blank_count += 1
            if blank_count <= 1:
                new_lines.append(line)
        else:
            blank_count = 0
            new_lines.append(line)
    lines = new_lines

    new_content = "\n".join(lines) + "\n"

    # 変更がある場合のみ書き戻す
    if new_content != content:
        try:
            with open(filepath, "w", encoding="utf-8", newline="\n") as f:
                f.write(new_content)
            return True
        except Exception as e:
            print(f"書き込みエラー: {filepath}: {e}")
            return False
    return False


def main():
    files = []
    for root, dirs, filenames in os.walk(TARGET_DIR):
        for fname in filenames:
            if fname.endswith(".h") or fname.endswith(".cpp"):
                files.append(os.path.join(root, fname))

    files.sort()
    print(f"対象ファイル数: {len(files)}")

    changed = 0
    for fpath in files:
        modified = process_file(fpath)
        if modified:
            changed += 1
            print(f"  変更: {fpath}")
        else:
            print(f"  スキップ: {fpath}")

    print(f"\n完了: {changed}/{len(files)} ファイルを変更しました")


if __name__ == "__main__":
    main()
