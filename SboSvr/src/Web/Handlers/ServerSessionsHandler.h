/// @file ServerSessionsHandler.h
/// @brief 接続中プレイヤー一覧 API(GET /api/server/sessions) 定義ファイル
/// @copyright Copyright(C)URARA-works
///
/// Web管理画面のサーバー情報ダッシュボード
/// (docs/web-admin-usability-proposals.md「5. 運用・権限系」server-dashboard.js)
/// が使う「接続中プレイヤー一覧」だけを提供する。キック・ブロードキャスト等は含まない。
///
/// JSON組み立ての純粋関数(テスト対象)は Web/SessionsJsonBuilder.h へ分離してある。

#pragma once

#include "Web/ApiHandler.h"
#include "Web/SessionsJsonBuilder.h"
#include <vector>

class CMgrData;

class CServerSessionsHandler : public IApiHandler
{
public:
        explicit CServerSessionsHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        std::vector<SboSessionRow> CollectSessions() const;
        static std::string GetTimestampNow();

        CMgrData *m_pMgrData;
};
