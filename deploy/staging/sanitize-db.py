#!/usr/bin/env python3
# 本番の SBODATA から個人情報を除いたステージング用 DB を作る(docs/cloud-dev-staging-plan.md の S6)。
#
#   python3 deploy/staging/sanitize-db.py <本番 SBODATA のコピー> <出力先ディレクトリ> [--admin 名前]
#
# - 入力ディレクトリの SboData.db(と -wal/-shm)、SboGrpData.db を読み、出力先に同名で書く。
#   入力は変更しない(一時ディレクトリにコピーしてから開く)。
# - 消すもの: アカウント・端末トークン・アカウントコード・BAN 情報・アクセス拒否 MAC、
#   プレイヤーキャラ(sys_char の MoveType=0 とアカウントに紐づくもの)と、その所持品・スキル、
#   旧形式 sbo_data の 'Account'/'Char'/'Disable' 行、画像編集者名(updated_by/saved_by)、画像の下書き。
# - 残すもの: マップ・アイテム種別・NPC・エフェクト・モーション・スキル・会話イベント・システム設定・画像。
# - --admin を付けると、ステージング用の管理者アカウント(AdminLevel=1)を1件作る。
#   パスワードは環境変数 SBOP2_ADMIN_PASSWORD か、無ければ対話入力で受け取る(コマンド履歴に残さないため)。
#   平文で入れておくと SboSvr の起動時にハッシュ化される。
#
# 標準ライブラリだけで動く(Windows の Python でも可)。
import argparse
import getpass
import os
import shutil
import sqlite3
import sys
import tempfile

MAIN_DB = 'SboData.db'
GRP_DB = 'SboGrpData.db'

# 丸ごと空にするテーブル(アカウントに属するもの)
ACCOUNT_TABLES = [
    'sys_account',
    'sys_account_char',
    'sys_account_code',
    'sys_account_device',
    'sys_account_admin',
    'sys_disable',
]

# 旧形式(sbo_data)に残っていると、空テーブルの時に読み直されてしまうキー
LEGACY_KEYS = ['Account', 'Char', 'Disable']

# CHARMOVETYPE_PC(Common/Info/InfoCharBase.h)
MOVETYPE_PC = 0


def log(msg):
    print(f'[sanitize] {msg}')


def table_exists(db, name):
    row = db.execute("SELECT 1 FROM sqlite_master WHERE type='table' AND name=?", (name,)).fetchone()
    return row is not None


def columns(db, table):
    return [r[1] for r in db.execute(f'PRAGMA table_info({table})')]


def copy_db(src_dir, name, work_dir):
    """-wal/-shm ごと一時ディレクトリにコピーし、チェックポイント済みの単一ファイルにする。"""
    src = os.path.join(src_dir, name)
    if not os.path.isfile(src):
        return None
    for suffix in ('', '-wal', '-shm'):
        if os.path.isfile(src + suffix):
            shutil.copy2(src + suffix, os.path.join(work_dir, name + suffix))
    path = os.path.join(work_dir, name)
    db = sqlite3.connect(path)
    db.execute('PRAGMA wal_checkpoint(TRUNCATE)')
    db.execute('PRAGMA journal_mode=DELETE')
    db.close()
    return path


def sanitize_main(path, admin_name, admin_password):
    db = sqlite3.connect(path)
    cur = db.cursor()

    # プレイヤーキャラの CharID を集める(アカウントを消す前に)
    pc_ids = set()
    if table_exists(db, 'sys_char'):
        pc_ids.update(r[0] for r in cur.execute('SELECT CharID FROM sys_char WHERE MoveType=?', (MOVETYPE_PC,)))
    if table_exists(db, 'sys_account_char'):
        pc_ids.update(r[0] for r in cur.execute('SELECT CharID FROM sys_account_char'))
    if table_exists(db, 'sys_account') and 'CharID' in columns(db, 'sys_account'):
        pc_ids.update(r[0] for r in cur.execute('SELECT CharID FROM sys_account WHERE CharID<>0'))
    pc_ids.discard(0)
    pc_ids.discard(None)
    log(f'プレイヤーキャラ: {len(pc_ids)} 件')

    cur.execute('CREATE TEMP TABLE pc_ids(CharID INTEGER PRIMARY KEY)')
    cur.executemany('INSERT INTO pc_ids VALUES(?)', [(i,) for i in pc_ids])

    for table in ('sys_char', 'sys_char_item', 'sys_char_skill', 'sys_item'):
        if table_exists(db, table) and 'CharID' in columns(db, table):
            n = cur.execute(f'DELETE FROM {table} WHERE CharID IN (SELECT CharID FROM pc_ids)').rowcount
            log(f'{table}: プレイヤーキャラ分 {n} 行を削除')

    for table in ACCOUNT_TABLES:
        if table_exists(db, table):
            n = cur.execute(f'DELETE FROM {table}').rowcount
            log(f'{table}: {n} 行を削除')

    if table_exists(db, 'sbo_data'):
        marks = ','.join('?' * len(LEGACY_KEYS))
        n = cur.execute(f'DELETE FROM sbo_data WHERE name IN ({marks})', LEGACY_KEYS).rowcount
        log(f'sbo_data: 旧形式のアカウント/キャラ/拒否MAC {n} 行を削除')

    if admin_name:
        if not table_exists(db, 'sys_account'):
            sys.exit('sys_account がありません。本番 DB を指定しているか確認してください。')
        # ログイン時にアカウント名は小文字化されるので小文字で入れる
        name = admin_name.lower()
        cols = columns(db, 'sys_account')
        values = {
            'AccountID': 1, 'Disable': 0, 'CharID': 0, 'TimeLastLogin': 0, 'TimeMakeAccount': 0,
            'LoginCount': 0, 'AdminLevel': 1, 'Account': name, 'Password': admin_password, 'MacAddr': '',
        }
        use = [c for c in cols if c in values]
        cur.execute(
            f'INSERT INTO sys_account({",".join(use)}) VALUES({",".join("?" * len(use))})',
            [values[c] for c in use])
        log(f'管理者アカウント {name} を作成(パスワードは起動時にハッシュ化されます)')

    db.commit()
    db.execute('VACUUM')
    db.close()


def sanitize_grp(path):
    db = sqlite3.connect(path)
    cur = db.cursor()
    for table, col in (('grp_sheet', 'updated_by'), ('grp_sheet_history', 'saved_by')):
        if table_exists(db, table) and col in columns(db, table):
            n = cur.execute(f'UPDATE {table} SET {col}=NULL WHERE {col} IS NOT NULL').rowcount
            log(f'{table}.{col}: 編集者名 {n} 行を消去')
    if table_exists(db, 'grp_draft'):
        n = cur.execute('DELETE FROM grp_draft').rowcount
        log(f'grp_draft: 下書き {n} 行を削除')
    db.commit()
    db.execute('VACUUM')
    db.close()


def main():
    ap = argparse.ArgumentParser(description='本番 SBODATA から個人情報を除いたステージング用 DB を作る')
    ap.add_argument('src', help='本番 SBODATA のコピー(SboData.db があるディレクトリ)')
    ap.add_argument('dst', help='出力先ディレクトリ')
    ap.add_argument('--admin', help='作成するステージング用管理者のアカウント名')
    args = ap.parse_args()

    if not os.path.isfile(os.path.join(args.src, MAIN_DB)):
        sys.exit(f'{os.path.join(args.src, MAIN_DB)} が見つかりません')
    if os.path.abspath(args.src) == os.path.abspath(args.dst):
        sys.exit('入力と出力に同じディレクトリは指定できません')

    admin_password = None
    if args.admin:
        admin_password = os.environ.get('SBOP2_ADMIN_PASSWORD') or getpass.getpass('管理者パスワード: ')
        if not admin_password:
            sys.exit('管理者パスワードが空です')

    os.makedirs(args.dst, exist_ok=True)
    with tempfile.TemporaryDirectory() as work:
        main_path = copy_db(args.src, MAIN_DB, work)
        sanitize_main(main_path, args.admin, admin_password)
        shutil.copy2(main_path, os.path.join(args.dst, MAIN_DB))

        grp_path = copy_db(args.src, GRP_DB, work)
        if grp_path:
            sanitize_grp(grp_path)
            shutil.copy2(grp_path, os.path.join(args.dst, GRP_DB))
        else:
            log(f'{GRP_DB} は無いのでスキップ')

    log(f'完了: {args.dst}')
    log('ログ(SboSvrLog.txt)・ini・旧 .dat は含めていません。')


if __name__ == '__main__':
    main()
