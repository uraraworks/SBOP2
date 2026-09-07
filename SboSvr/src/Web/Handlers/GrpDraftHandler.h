#pragma once

// GrpDraftHandler.h
// /api/assets/drafts — 画像エディタの途中セーブ（下書き）API
//
//   GET    /api/assets/drafts        一覧（png は含まない）
//   GET    /api/assets/drafts/{id}   1件取得（png を base64 で含む）
//   POST   /api/assets/drafts        新規保存 → { "id": N }
//   PUT    /api/assets/drafts/{id}   上書き保存
//   DELETE /api/assets/drafts/{id}   削除
//
// いずれも IMAGE_EDIT ロールを要求する。
//
// 下書きは grp_draft テーブルに入り、配信経路（GetPng / 公開マニフェスト）からは
// 参照されない。つまり途中セーブしてもゲームには反映されない。ゲームへ出すのは
// 従来どおり PUT /api/assets/sprites/{cat}/{index} だけ。
//
// 保存単位はペイント 1 セル分の PNG。シート 1 枚ではなくセル 1 個にしているのは、
// 32x32 のパレット PNG なら 1KB 前後に収まり、既定のボディ上限 64KB の内側で
// 扱えるため（スプライト差し替えのように上限を 2MB へ緩める必要がない）。
//
// PNG はバイナリのままだと JSON に載らないので base64 でやり取りする。
// 名前は日本語が入るため、クエリ文字列ではなく JSON ボディで受ける
// （このプロジェクトには URL デコードの共通実装が無いため）。

#include "Web/ApiHandler.h"

#include <string>

class CMgrData;

class CGrpDraftHandler : public IApiHandler
{
public:
    // pathPrefix 例: "/api/assets/drafts"
    CGrpDraftHandler(std::string pathPrefix, CMgrData *pMgrData);

    void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
    void HandleList(const HttpRequest &request, HttpResponse &response);
    void HandleGetOne(long long nId, HttpResponse &response);
    void HandleCreate(const HttpRequest &request, HttpResponse &response,
                      const std::string &updatedBy);
    void HandleUpdate(long long nId, const HttpRequest &request, HttpResponse &response,
                      const std::string &updatedBy);
    void HandleDelete(long long nId, HttpResponse &response);

    // パス末尾の /{id} を取り出す。付いていなければ 0 を返す（一覧・新規作成）。
    // 数値以外が付いていた場合は -1（不正）を返す。
    long long ParseIdFromPath(const std::string &path) const;

    std::string m_pathPrefix;
    CMgrData   *m_pMgrData;
};
