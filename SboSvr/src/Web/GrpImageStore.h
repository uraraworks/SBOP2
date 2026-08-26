#pragma once

// GrpImageStore.h
// 画像ストア（SBODATA/SboGrpData.db）から res_name をキーに PNG を取得・保存するクラス。
//
// SboData.db とは完全に別ファイル。SboData.db は CMgrData::Save() が
// 起動時からのメモリ像を丸ごと書き戻す構造のため、ここに画像を混ぜると
// 管理画面からの書き込みが消えてしまう。そのため画像専用の DB ファイルを持つ。
//
// DB が存在しない・オープンに失敗する場合は「エラー」ではなく「ヒットしなかった」
// として扱う。サーバは画像ストアが無くても従来どおり起動・動作しなければならない
// （プロジェクトの「データファイル不在許容」方針）。

#include <mutex>
#include <string>
#include <vector>

struct sqlite3;

class CGrpImageStore
{
public:
    // シングルトンインスタンス取得
    static CGrpImageStore &GetInstance();

    // res_name の PNG を取得する。
    // 見つからなければ false（DB 未作成・オープン失敗も false）。
    // outETag には ETag 文字列（ダブルクォート込み、例 "gs-1-12345"）を格納する。
    bool GetPng(const char *pszResName, std::vector<unsigned char> &outPng, std::string &outETag);

    // res_name の PNG を保存する。
    // 既存行があれば grp_sheet_history へ退避してから revision を +1 して UPSERT する。
    // 新規なら revision = 1。
    // ※ S2 でルーティングされる。今回は呼び出し元を作らない。
    bool PutPng(const char *pszResName,
                const unsigned char *pPng, size_t nPngSize,
                int nWidth, int nHeight,
                const char *pszUpdatedBy,
                std::string &outError);

private:
    CGrpImageStore();
    ~CGrpImageStore();

    // 未実装（コピー禁止）
    CGrpImageStore(const CGrpImageStore &);
    CGrpImageStore &operator=(const CGrpImageStore &);

    // DB オープン保証（m_mutex 保持中に呼ぶこと）。
    // 一度失敗したら以後は毎回開き直さず false を返す。
    bool EnsureOpenLocked();

    // 実行ファイルのディレクトリを基準に SBODATA\SboGrpData.db のパスを解決する
    bool ResolveDbPathLocked(std::string &outPath) const;

    std::mutex m_mutex;
    sqlite3   *m_pDb;
    bool       m_bOpenFailed;
};
