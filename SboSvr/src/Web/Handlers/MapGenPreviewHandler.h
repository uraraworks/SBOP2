#pragma once

#include "Web/ApiHandler.h"

#include <string>

class CMgrData;

// ---------------------------------------------------------------------------
// /api/map-gen-patterns/preview — 洞窟マップ生成プレビュー API
//
// POST /api/map-gen-patterns/preview
//   リクエスト JSON:
//     {
//       "patternId": <int, 省略可>,
//       "params":    <object, 省略可 — width/height/floorAreaMin/blockMin/blockMax/cutoffPercent/maxRetries>,
//       "roleMap":   <object, 省略可 — floor/wall の grpId>,
//       "seed":      <int, 省略可 — 0 or 省略でランダム>
//     }
//   patternId が来た場合は DB から既定値を読み込み、params/roleMap/seed で上書き。
//   patternId も params も無い場合は 400 を返す。
//
//   レスポンス JSON:
//     {
//       "width":     <int>,
//       "height":    <int>,
//       "seed":      <int>,
//       "tilesBase": [<grpId>, ...]   // length = width * height, row-major
//     }
//
//   エラー:
//     400 — パラメータ不正
//     422 — リトライ上限超過（生成失敗）
//     500 — サーバエラー
// ---------------------------------------------------------------------------

class CMapGenPreviewHandler : public IApiHandler
{
public:
    explicit CMapGenPreviewHandler(CMgrData *pMgrData);
    void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
    CMgrData *m_pMgrData;
};
