#pragma once

// PublicAssetHandler.h
// ゲームクライアント向けの「認証不要」な画像配信エンドポイント。
//
// 管理画面用の GET /api/assets/sprites/{cat}/{idx} は IMAGE_EDIT ロール必須で、
// /api/ 配下は HttpServer.cpp の認証ゲートを通る。ゲームをプレイするだけの
// ユーザーは管理者ではないためこの口は使えない。
//
// 画像はビルド同梱で全プレイヤーに配られているものであり秘密ではないため、
// /api/ の外（/assets/ プレフィックス）に読み取り専用の公開エンドポイントを
// 新設する。管理用の SpriteSheetHandler とは別ファイルにして混同を避ける。
//
// 提供する2本:
//   GET /assets/manifest        - grp_sheet 全行のメタ一覧（png 本体は含まない）
//   GET /assets/sprite/{resName} - res_name の PNG を「保存された生バイト列のまま」返す
//
// 重要: この2本は CGrpImageStore::GetPng() / GetAllSummaries() が返す生データを
// そのまま返す。管理用ハンドラ（SpriteSheetHandler）が行っている
// MakeTransparentPng（パレット0の透過化、<img> 表示向け）は一切適用しない。
// ゲームクライアントは res/*.png の生データを前提に自前でパレット処理をしている。

#include "Web/ApiHandler.h"

#include <string>

// ---------------------------------------------------------------------------
// CPublicAssetManifestHandler: GET /assets/manifest
// ---------------------------------------------------------------------------
class CPublicAssetManifestHandler : public IApiHandler
{
public:
    void Handle(const HttpRequest &request, HttpResponse &response) override;
};

// ---------------------------------------------------------------------------
// CPublicAssetSpriteHandler: GET /assets/sprite/{resName}
// ---------------------------------------------------------------------------
class CPublicAssetSpriteHandler : public IApiHandler
{
public:
    // pathPrefix 例: "/assets/sprite/"
    explicit CPublicAssetSpriteHandler(std::string pathPrefix);

    void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
    std::string m_pathPrefix;
};
