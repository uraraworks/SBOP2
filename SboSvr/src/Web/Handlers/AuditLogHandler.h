#pragma once

#include "Web/ApiHandler.h"

class CMgrData;

// 監査ログ一覧 API ハンドラ
//   GET /api/audit-logs
//     クエリ:
//       actor=...       実行者 loginId 完全一致
//       method=POST     HTTP メソッド完全一致
//       path=/api/maps  パス前方一致
//       limit=100       最大件数（既定 100、上限 500）
class CAuditLogListHandler : public IApiHandler
{
public:
        explicit CAuditLogListHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};
