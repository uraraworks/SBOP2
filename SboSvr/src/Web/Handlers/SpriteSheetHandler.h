#pragma once

// SpriteSheetHandler.h
// 汎用スプライトシート配信プロバイダ + ハンドラ
//
// 注意: kSpriteCategories[] のセルサイズ・枚数レイアウトは
//   SboCli/src/MgrGrpData.cpp の
//     GetGrpSize / GetGrpCountX / GetGrpCountY / 読込ループ
//   と同期が必須。MgrGrpData.cpp を変更した場合は必ずこちらも更新すること。

#include "Web/ApiHandler.h"

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include <windows.h>

#include "GlobalDefine.h"

// ---------------------------------------------------------------------------
// カテゴリ定義レコード
// ---------------------------------------------------------------------------
struct SSpriteCategoryDef
{
    int         nIDMain;            // GRPIDMAIN_* 値
    const char *pszKey;             // URL / ImageCatalog と共通のキー文字列
    const wchar_t *pszResourcePattern; // _snwprintf で使う wchar_t フォーマット文字列
                                       // (NULL の場合は apszFixedNames を使う)
    const wchar_t * const *apszFixedNames; // 固定名配列 (NULL 終端)。pattern が NULL の時のみ参照
    int         nCellSize;          // 1セルのピクセル幅・高さ
    int         nCountX;            // 横セル数
    int         nCountY;            // 縦セル数
    int         nFirstResourceIndex; // sheetIndex=0 に対応するリソース番号
};

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
    // kSpriteCategories に存在すれば true を返し、各出力引数に値を格納する。
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
    const SSpriteCategoryDef *FindCategory(const std::string &key) const;

    // リソース名を生成する（固定名テーブル or printf パターン）
    bool BuildResourceName(const SSpriteCategoryDef &cat, int sheetIndex, std::wstring &outName) const;

    // 実際のロード処理（m_mutex 保持中に呼ぶこと）
    bool LoadSheetLocked(const SSpriteCategoryDef &cat, int sheetIndex, std::vector<unsigned char> &outData);

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
