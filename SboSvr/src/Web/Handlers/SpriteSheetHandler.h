#pragma once

// SpriteSheetHandler.h
// 汎用スプライトシート配信プロバイダ + ハンドラ
//
// レイアウト定義（セルサイズ・枚数・リソース名パターン）は
// Common/GrpLayout.h の SGrpLayoutDef / GrpLayout_* に一本化されている。
// レイアウトを変更する場合は Common/GrpLayout.cpp のテーブルだけを直せばよい。

#include "Web/ApiHandler.h"

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include <windows.h>

#include "GlobalDefine.h"
#include "GrpLayout.h"

// ---------------------------------------------------------------------------
// CGrpResourceProvider: SboGrpData.dll からスプライトシートを配信する汎用プロバイダ
// ---------------------------------------------------------------------------
class CGrpResourceProvider
{
public:
    CGrpResourceProvider();
    ~CGrpResourceProvider();

    // カテゴリキーとシートインデックス(0始まり)から PNG バイト列を取得する。
    // 成功すれば true を返し outData に PNG、outETag に ETag 文字列（ダブルクォート込み）を格納する。
    //
    // 取得経路は以下の3段フォールバック（優先順）:
    //   1. 画像ストア（SBODATA/SboGrpData.db, CGrpImageStore）
    //   2. ファイル（SboGrpData/res/ 配下、開発時のみヒットする想定）
    //   3. SboGrpData.dll のリソース（従来経路）
    // いずれの経路でもパレット0の透過化（MakeTransparentPng）を同じように適用する。
    bool GetSheetPng(const std::string &categoryKey, int sheetIndex,
                      std::vector<unsigned char> &outData, std::string &outETag);

    // res_name を指定してメモリキャッシュを破棄する（NULL/空なら全破棄）。
    // S2 で画像ストアへの PUT 後にキャッシュを追従させるために使う想定。
    // 今回は呼び出し元を作らない。
    void InvalidateCache(const char *pszResName);

    // カテゴリキーに対応するシート総数を返す（存在確認プローブで遅延カウント）。
    // B2 フェーズで ImageCatalogHandler から参照できるようシングルトンを公開する。
    int GetSheetCount(const std::string &categoryKey);

    // categoryKey + sheetIndex から res_name（画像ストア/GrpLayout 共通の ASCII キー）を返す。
    // アップロードハンドラが CGrpImageStore::PutPng() のキーを組み立てるために使う。
    bool ResolveResourceName(const std::string &categoryKey, int sheetIndex, std::string &outResName) const;

    // カテゴリキーに対応するレイアウト情報を返す。
    // Common/GrpLayout.h のテーブルに存在すれば true を返し、各出力引数に値を格納する。
    // 存在しなければ false を返す。
    bool GetCategoryLayout(const char *pszKey,
                           int &nCellSize,
                           int &nCountX,
                           int &nCountY) const;

    // シングルトンインスタンス取得
    static CGrpResourceProvider &GetInstance();

private:
    // DLL ロード保証（m_mutex 保持中に呼ぶこと）
    bool EnsureLibraryLocked();
    bool ResolveLibraryPath(std::wstring &outPath) const;

    // カテゴリ定義検索
    const SGrpLayoutDef *FindCategory(const std::string &key) const;

    // リソース名を生成する（固定名テーブル or printf パターン）
    bool BuildResourceName(const SGrpLayoutDef &cat, int sheetIndex, std::wstring &outName) const;

    // 実際のロード処理（m_mutex 保持中に呼ぶこと）。
    // resName（BuildResourceName で得た wchar_t 名。ASCII 前提）に対して
    // 画像ストア → ファイル → DLL リソースの順に探し、見つかった生 PNG を
    // MakeTransparentPng に通した結果と ETag を返す。
    bool LoadSheetLocked(const SGrpLayoutDef &cat, int sheetIndex,
                         std::vector<unsigned char> &outData, std::string &outETag);

    // 画像ストア（SboGrpData.db）から取得を試みる
    bool TryLoadFromImageStoreLocked(const std::wstring &resourceName,
                                     std::vector<unsigned char> &outRawPng, std::string &outETag);
    // SboGrpData/res/ 配下のファイルから取得を試みる
    bool TryLoadFromFileLocked(const std::wstring &resourceName,
                               std::vector<unsigned char> &outRawPng, std::string &outETag);
    // SboGrpData.dll のリソースから取得を試みる（従来経路）
    bool TryLoadFromDllLocked(const std::wstring &resourceName,
                              std::vector<unsigned char> &outRawPng, std::string &outETag);

    // resourceName が 画像ストア/ファイル/DLL のいずれかに存在するかを判定する
    // （GetSheetCount の存在確認プローブを読み取り側の3経路と揃えるため）
    bool SheetExistsLocked(const std::wstring &resourceName);

    // パレット 0 を透過化して PNG を返す（MapPartsHandler と同じロジック）
    bool MakeTransparentPng(const unsigned char *pSrc, size_t nSrcSize, std::vector<unsigned char> &outData);

    std::mutex  m_mutex;
    HMODULE     m_hModule;

    struct SSheetCacheEntry
    {
        std::vector<unsigned char> png;
        std::string                etag;
        std::string                resName;   // InvalidateCache の逆引き用
    };

    // キャッシュキー: (categoryKey, sheetIndex)
    std::map<std::pair<std::string, int>, SSheetCacheEntry> m_sheetCache;
    // シート総数キャッシュ: categoryKey -> count (-1 で未確認)
    std::map<std::string, int> m_sheetCountCache;
};

// ---------------------------------------------------------------------------
// CSpriteSheetHandler: GET /api/assets/sprites/{categoryKey}/{sheetIndex}
// ---------------------------------------------------------------------------
class CSpriteSheetHandler : public IApiHandler
{
public:
    // pathPrefix 例: "/api/assets/sprites/"
    explicit CSpriteSheetHandler(std::string pathPrefix);

    void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
    // パスから (categoryKey, sheetIndex) を解析する
    bool TryParsePath(const std::string &path, std::string &outKey, int &outIndex) const;

    std::string m_pathPrefix;
};

// ---------------------------------------------------------------------------
// CSpriteSheetUploadHandler: PUT /api/assets/sprites/{categoryKey}/{sheetIndex}
// ---------------------------------------------------------------------------
// リクエストボディは生 PNG バイト列（Content-Type: image/png 必須）。
// 認証・権限チェックは ServerInfoHandler と同じ流儀（ハンドラ内で
// AuthProvider::Authenticate を呼び、503/401/403 を返す）。必要ロールは IMAGE_EDIT。
class CSpriteSheetUploadHandler : public IApiHandler
{
public:
    // pathPrefix 例: "/api/assets/sprites/"
    CSpriteSheetUploadHandler(std::string pathPrefix, class CMgrData *pMgrData);

    void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
    // パス解析は CSpriteSheetHandler と同じロジックを流用する
    bool TryParsePath(const std::string &path, std::string &outKey, int &outIndex) const;

    std::string m_pathPrefix;
    class CMgrData *m_pMgrData;
};
