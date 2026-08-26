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
    // 成功すれば true を返し outData に PNG を格納する。
    bool GetSheetPng(const std::string &categoryKey, int sheetIndex, std::vector<unsigned char> &outData);

    // カテゴリキーに対応するシート総数を返す（存在確認プローブで遅延カウント）。
    // B2 フェーズで ImageCatalogHandler から参照できるようシングルトンを公開する。
    int GetSheetCount(const std::string &categoryKey);

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

    // 実際のロード処理（m_mutex 保持中に呼ぶこと）
    bool LoadSheetLocked(const SGrpLayoutDef &cat, int sheetIndex, std::vector<unsigned char> &outData);

    // パレット 0 を透過化して PNG を返す（MapPartsHandler と同じロジック）
    bool MakeTransparentPng(const unsigned char *pSrc, size_t nSrcSize, std::vector<unsigned char> &outData);

    std::mutex  m_mutex;
    HMODULE     m_hModule;

    // キャッシュキー: (categoryKey, sheetIndex)
    std::map<std::pair<std::string, int>, std::vector<unsigned char> > m_sheetCache;
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
