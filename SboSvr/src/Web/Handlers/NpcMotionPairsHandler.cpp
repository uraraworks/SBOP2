#include "StdAfx.h"
#include "NpcMotionPairsHandler.h"

#include <sstream>
#include <vector>

#include "Web/AuthProvider.h"
#include "MgrData.h"
#include "LibInfo/LibInfoMotion.h"
#include "LibInfo/LibInfoMotionType.h"
#include "Info/InfoMotion.h"
#include "Info/InfoMotionType.h"
#include "GlobalDefine.h"
#include "Web/Handlers/SpriteSheetHandler.h"

// ---------------------------------------------------------------------------
// 内部ヘルパー
// ---------------------------------------------------------------------------

namespace
{

// 指定 MotionTypeID の指定 ListID サブベースを解決する
// 0 = 未定義
WORD ResolveSubBase(CLibInfoMotion *pMotionLib, DWORD dwMotionTypeId, DWORD dwListId)
{
        if (pMotionLib == NULL) {
                return 0;
        }
        ARRAYMOTIONINFO motions;
        pMotionLib->GetMotionInfo(dwMotionTypeId, dwListId, motions);
        if (motions.empty()) {
                return 0;
        }
        const CInfoMotion *pMotion = motions[0];
        return (pMotion != NULL) ? pMotion->m_wGrpIDSubBase : 0;
}

} // namespace

// ---------------------------------------------------------------------------
// CNpcMotionPairsHandler
// ---------------------------------------------------------------------------

CNpcMotionPairsHandler::CNpcMotionPairsHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CNpcMotionPairsHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        // 認証チェック
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        if (m_pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        CLibInfoMotionType *pTypeLib = m_pMgrData->GetLibInfoMotionType();
        CLibInfoMotion *pMotionLib = m_pMgrData->GetLibInfoMotion();
        if ((pTypeLib == NULL) || (pMotionLib == NULL)) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        // 全モーション種別を列挙し、それぞれの STAND_UP/DOWN/LEFT/RIGHT サブベースを返す。
        // フロント側ではファイル × motion 種別の組合せをプローブし、
        // その motion が定義している方向のセル全てに描画があるものだけを picker に並べる。
        // (= ファイル内の各 NPC を、その NPC を意図したモーション種別で正しく拾う)
        struct MotionTypeEntry {
                DWORD motionTypeId;
                WORD  targetGrpIdNpc;  // sys_motion_type.GrpIDSub = この種別が想定する NPC ファイル index
                WORD  subUp;
                WORD  subDown;
                WORD  subLeft;
                WORD  subRight;
        };
        std::vector<MotionTypeEntry> motionTypes;

        pTypeLib->Enter();
        int nTypeCount = pTypeLib->GetCount();
        for (int i = 0; i < nTypeCount; ++i) {
                const CInfoMotionType *pInfo =
                        static_cast<const CInfoMotionType *>(pTypeLib->GetPtr(i));
                if (pInfo == NULL) { continue; }
                MotionTypeEntry e;
                e.motionTypeId   = pInfo->m_dwMotionTypeID;
                e.targetGrpIdNpc = pInfo->m_wGrpIDSub;
                e.subUp    = ResolveSubBase(pMotionLib, e.motionTypeId, CHARMOTIONLISTID_STAND_UP);
                e.subDown  = ResolveSubBase(pMotionLib, e.motionTypeId, CHARMOTIONLISTID_STAND_DOWN);
                e.subLeft  = ResolveSubBase(pMotionLib, e.motionTypeId, CHARMOTIONLISTID_STAND_LEFT);
                e.subRight = ResolveSubBase(pMotionLib, e.motionTypeId, CHARMOTIONLISTID_STAND_RIGHT);
                // STAND 方向が 1 つも無いものは NPC として描画に使われない
                if ((e.subUp == 0) && (e.subDown == 0) && (e.subLeft == 0) && (e.subRight == 0)) {
                        continue;
                }
                motionTypes.push_back(e);
        }
        pTypeLib->Leave();

        // 利用可能な NPC スプライト枚数を取得 (DB で誰も使っていないファイルも picker に出すため)
        int nFileCount = CGrpResourceProvider::GetInstance().GetSheetCount("npc2x2");

        // JSON 構築
        std::ostringstream oss;
        oss << "{\"fileCount\":" << nFileCount << ",\"motionTypes\":[";
        bool bFirst = true;
        for (size_t i = 0; i < motionTypes.size(); ++i) {
                if (!bFirst) { oss << ','; }
                bFirst = false;
                const MotionTypeEntry &e = motionTypes[i];
                oss << '{';
                oss << "\"motionTypeId\":"   << e.motionTypeId   << ',';
                oss << "\"targetGrpIdNpc\":" << e.targetGrpIdNpc << ',';
                oss << "\"subUp\":"          << e.subUp          << ',';
                oss << "\"subDown\":"        << e.subDown        << ',';
                oss << "\"subLeft\":"        << e.subLeft        << ',';
                oss << "\"subRight\":"       << e.subRight;
                oss << '}';
        }
        oss << "]}";

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
