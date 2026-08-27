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

// 履歴の1件分（png 本体は含めない。一覧表示用のメタのみ）
struct SGrpSheetHistoryEntry
{
    long long   nId;         // grp_sheet_history.id
    int         nRevision;
    long long   nSavedAt;    // Unix epoch 秒
    std::string strSavedBy;
    size_t      nBytes;      // png のバイト数
};

// grp_sheet 1行分のメタ（png 本体は含めない。公開マニフェスト用）
struct SGrpSheetSummary
{
    std::string strResName;
    int         nRevision;
    size_t      nBytes;
    long long   nUpdatedAt;
};

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

    // 現在の版のメタを取得する。DB に行が無ければ false（= res/ か DLL が配信されている状態）。
    bool GetCurrentMeta(const char *pszResName, int &outRevision, int &outWidth, int &outHeight,
                        long long &outUpdatedAt, std::string &outUpdatedBy, size_t &outBytes);

    // 履歴を新しい順（revision 降順）で取得する。DB が無ければ空を返して true。
    bool GetHistory(const char *pszResName, std::vector<SGrpSheetHistoryEntry> &outEntries);

    // grp_sheet の全行のメタを res_name 昇順で返す（png 本体は SELECT しない）。
    // 認証不要の公開マニフェスト（/assets/manifest）用。DB が無ければ空を返して true。
    bool GetAllSummaries(std::vector<SGrpSheetSummary> &outList);

    // 指定 revision の PNG バイト列を履歴から取得する。
    bool GetHistoryPng(const char *pszResName, int nRevision, std::vector<unsigned char> &outPng);

    // 上書きを解除する（grp_sheet と grp_sheet_history から res_name の行を全削除）。
    // 削除後は res/ か DLL の出荷時イメージが配信される状態に戻る。
    // 行が無かった場合も true を返す（冪等）。
    bool ClearOverride(const char *pszResName, std::string &outError);

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
