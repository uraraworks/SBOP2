#!/usr/bin/env python3
"""
db_migrate_utf8.py
CP932 (Shift_JIS) で書かれた SQLite DB の文字列列を UTF-8 に変換する。
再実行しても安全（既に UTF-8 な値はスキップ）。
"""

import sqlite3
import sys
import os

# デフォルト DB パス
DEFAULT_DB = os.path.join(
    os.path.dirname(__file__), "..", "SboSvr", "Debug", "SBODATA", "SboData.db"
)

# 対象テーブルと列の定義
TARGET_COLUMNS = {
    "sys_account":          ["Account", "Password", "MacAddr"],
    "sys_char":             ["CharName"],
    "sys_disable":          ["MacAddress"],
    "sys_efc_balloon":      ["Name"],
    "sys_effect":           ["Name"],
    "sys_item":             ["Name"],
    "sys_item_type":        ["Name"],
    "sys_item_weapon":      ["Name"],
    "sys_map":              ["MapName"],
    "sys_map_object":       ["Name"],
    "sys_motion_type":      ["Name"],
    "sys_skill":            ["Name"],
    "sys_talk_event_event": ["Text"],
    "sys_talk_event_menu":  ["ItemName"],
}


def try_decode(raw: bytes) -> str | None:
    """
    バイト列を文字列に変換する。
    1. UTF-8 デコード可能 → None を返す（スキップ）
    2. ASCII のみ（全バイト ≤ 127） → None を返す（スキップ）
    3. CP932 でデコード → UTF-8 文字列を返す
    4. 失敗 → None を返し、呼び出し側で警告
    """
    # 1. すでに UTF-8 か確認
    try:
        raw.decode("utf-8")
        return None  # スキップ
    except UnicodeDecodeError:
        pass

    # 2. ASCII のみかチェック
    if all(b <= 127 for b in raw):
        return None  # スキップ

    # 3. CP932 でデコード
    try:
        return raw.decode("cp932")
    except (UnicodeDecodeError, Exception):
        return None  # 呼び出し側で警告処理


def migrate(db_path: str) -> None:
    db_path = os.path.abspath(db_path)
    if not os.path.exists(db_path):
        print(f"[ERROR] DB が見つかりません: {db_path}")
        sys.exit(1)

    print(f"DB: {db_path}")
    conn = sqlite3.connect(db_path)
    conn.text_factory = bytes  # 生バイトで取得

    cur = conn.cursor()
    cur.execute("BEGIN")

    try:
        for table, columns in TARGET_COLUMNS.items():
            # テーブルが存在するか確認
            cur.execute(
                "SELECT name FROM sqlite_master WHERE type='table' AND name=?",
                (table,),
            )
            if cur.fetchone() is None:
                print(f"  [{table}] テーブルなし → スキップ")
                continue

            # PK を取得（rowid を使う）
            converted_total = 0
            warned_total = 0

            for col in columns:
                cur.execute(f"SELECT rowid, {col} FROM {table}")
                rows = cur.fetchall()
                converted = 0
                warned = 0

                for rowid, raw in rows:
                    if raw is None:
                        continue
                    if not isinstance(raw, bytes):
                        continue

                    new_val = try_decode(raw)
                    if new_val is None:
                        # スキップ（UTF-8 済み or ASCII）
                        # ただし完全に変換できなかった場合（cp932 も失敗）は警告
                        try:
                            raw.decode("utf-8")
                        except UnicodeDecodeError:
                            if not all(b <= 127 for b in raw):
                                # cp932 失敗ケース
                                print(
                                    f"  [WARN] {table}.{col} rowid={rowid}: "
                                    f"cp932 変換失敗 raw={raw!r}"
                                )
                                warned += 1
                        continue

                    cur.execute(
                        f"UPDATE {table} SET {col}=? WHERE rowid=?",
                        (new_val, rowid),
                    )
                    converted += 1

                converted_total += converted
                warned_total += warned

            print(
                f"  [{table}] 変換: {converted_total} 件"
                + (f"  警告: {warned_total} 件" if warned_total else "")
            )

        cur.execute("COMMIT")
        print("\n変換完了。")

    except Exception as e:
        cur.execute("ROLLBACK")
        print(f"[ERROR] ロールバックしました: {e}")
        raise


if __name__ == "__main__":
    db_path = sys.argv[1] if len(sys.argv) > 1 else DEFAULT_DB
    migrate(db_path)
