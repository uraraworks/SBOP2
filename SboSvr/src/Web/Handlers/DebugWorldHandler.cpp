/// @file DebugWorldHandler.cpp
/// @brief /api/debug/npc・/api/debug/char-status (テスト準備API) 実装ファイル

#include "StdAfx.h"

#ifdef _DEBUG

#include "DebugWorldHandler.h"

#include <sstream>

#include "Web/DebugFixtureGuard.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "GlobalDefine.h"
#include "LibInfo/LibInfoCharSvr.h"
#include "LibInfo/LibInfoMapBase.h"
#include "Info/InfoCharBase.h"
#include "Info/InfoCharSvr.h"
#include "Info/InfoMapBase.h"
#include "TextOutput.h"
#include "myLib/myString.h"

namespace
{
	void SetErrorResponse(HttpResponse &response, const char *pszStatusLine, const char *pszError)
	{
		response.statusLine = pszStatusLine;
		std::ostringstream oss;
		oss << "{\"error\":\"" << JsonUtils::Escape(pszError) << "\"}";
		response.SetJsonBody(oss.str());
	}

	/// @brief 任意の整数パラメータを読む(無い・範囲外なら既定値)。
	int GetIntOr(const std::string &json, const char *pszKey, int nDefault, int nMin, int nMax)
	{
		int nValue = 0;
		if (!JsonUtils::TryGetInt(json, pszKey, nValue)) {
			return nDefault;
		}
		if ((nValue < nMin) || (nValue > nMax)) {
			return nDefault;
		}
		return nValue;
	}

	// charId で CInfoCharBase* を検索する(Lock済みの状態で呼ぶこと)。
	// DebugFixtureHandler.cpp の FindCharByID と同じ実装(既存ハンドラ群の流儀にそろえ共有しない)。
	CInfoCharBase *FindCharByID(CLibInfoCharSvr *pCharLib, DWORD dwCharID)
	{
		int nTotal = pCharLib->GetCount();
		for (int i = 0; i < nTotal; ++i) {
			CInfoCharBase *pTmp = static_cast<CInfoCharBase *>(pCharLib->GetPtr(i));
			if ((pTmp != NULL) && (pTmp->m_dwCharID == dwCharID)) {
				return pTmp;
			}
		}
		return NULL;
	}
}

CDebugWorldHandler::CDebugWorldHandler(CMgrData *pMgrData)
	: m_pMgrData(pMgrData)
{
}

void CDebugWorldHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
	if (m_pMgrData == NULL) {
		SetErrorResponse(response, "HTTP/1.1 503 Service Unavailable", "backend_unavailable");
		return;
	}

	// 安全策: loopbackからの直接リクエスト以外は存在しないAPIとして扱う(404)。
	if (!DebugFixtureGuard::IsAllowedRequest(request)) {
		response.statusLine = "HTTP/1.1 404 Not Found";
		response.SetJsonBody("{\"error\":\"not_found\"}");
		return;
	}

	std::string strPath = request.path;
	size_t nQuery = strPath.find('?');
	if (nQuery != std::string::npos) {
		strPath = strPath.substr(0, nQuery);
	}

	if (strPath == "/api/debug/npc") {
		HandleNpc(request, response);
	} else if (strPath == "/api/debug/char-status") {
		HandleCharStatus(request, response);
	} else {
		response.statusLine = "HTTP/1.1 404 Not Found";
		response.SetJsonBody("{\"error\":\"not_found\"}");
	}
}

/// @brief 敵(NPC)を1体置く。
/// @details スポーナー(CLibInfoCharSvr の PUTNPC 処理)が敵を湧かせる時と同じく、
///          AddNPC の後に SetMap・SetLibInfoChar を呼んで、移動・攻撃の処理が動く状態にする。
///          座標はマップ上のピクセル座標(キャラ情報の m_nMapX/m_nMapY と同じ単位)。
void CDebugWorldHandler::HandleNpc(const HttpRequest &request, HttpResponse &response)
{
	int nMapId = 0, nX = 0, nY = 0;
	if (!JsonUtils::TryGetInt(request.body, "mapId", nMapId) ||
	    !JsonUtils::TryGetInt(request.body, "x", nX) ||
	    !JsonUtils::TryGetInt(request.body, "y", nY) ||
	    (nMapId <= 0) || (nX < 0) || (nY < 0)) {
		SetErrorResponse(response, "HTTP/1.1 400 Bad Request", "mapId_x_y_required");
		return;
	}

	std::string strCharNameUtf8;
	JsonUtils::TryGetString(request.body, "charName", strCharNameUtf8);
	if (strCharNameUtf8.empty()) {
		strCharNameUtf8 = "DebugEnemy";
	}

	// 既定は「攻撃されたら反撃してくる敵」(戦闘1)。画像は NPC 画像の行番号。
	int nMoveType = GetIntOr(request.body, "moveType", CHARMOVETYPE_BATTLE1, 0, CHARMOVETYPE_MAX - 1);
	int nGrpIdNpc = GetIntOr(request.body, "grpIdNpc", 1, 0, 0xFFFF);
	int nMotionTypeId = GetIntOr(request.body, "motionTypeId", 0, 0, 0x7FFFFFFF);
	int nHP = GetIntOr(request.body, "hp", 30, 1, 0x7FFFFFFF);
	int nAtack = GetIntOr(request.body, "atack", 10, 0, 0xFFFF);
	int nDefense = GetIntOr(request.body, "defense", 0, 0, 0xFFFF);
	// 索敵範囲(タイル数)。0 なら自分から襲ってこず、攻撃された時だけ反撃する。
	int nSearchX = GetIntOr(request.body, "searchX", 0, 0, 255);
	int nSearchY = GetIntOr(request.body, "searchY", nSearchX, 0, 255);

	CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
	CLibInfoMapBase *pMapLib = m_pMgrData->GetLibInfoMap();
	if ((pCharLib == NULL) || (pMapLib == NULL)) {
		SetErrorResponse(response, "HTTP/1.1 503 Service Unavailable", "backend_unavailable");
		return;
	}
	CInfoMapBase *pInfoMap = (CInfoMapBase *)pMapLib->GetPtr((DWORD)nMapId);
	if (pInfoMap == NULL) {
		SetErrorResponse(response, "HTTP/1.1 404 Not Found", "map_not_found");
		return;
	}

	CInfoCharSvr infoTmp;
	infoTmp.m_strCharName = (LPCTSTR)Utf8ToTString(strCharNameUtf8.c_str());
	infoTmp.m_nMoveType = nMoveType;
	infoTmp.m_dwMapID = (DWORD)nMapId;
	infoTmp.m_nMapX = nX;
	infoTmp.m_nMapY = nY;
	infoTmp.m_ptStartPos.x = nX;
	infoTmp.m_ptStartPos.y = nY;
	infoTmp.m_wGrpIDNPC = (WORD)nGrpIdNpc;
	infoTmp.m_wGrpIDInitNPC = (WORD)nGrpIdNpc;
	infoTmp.m_dwMotionTypeID = (DWORD)nMotionTypeId;
	infoTmp.m_dwHP = (DWORD)nHP;
	infoTmp.m_dwMaxHP = (DWORD)nHP;
	infoTmp.m_wPAtack = (WORD)nAtack;
	infoTmp.m_wPDefense = (WORD)nDefense;
	infoTmp.m_wPHitAverage = 90;
	infoTmp.m_sizeSearchDistance.cx = nSearchX;
	infoTmp.m_sizeSearchDistance.cy = nSearchY;
	switch (nMoveType) {
	case CHARMOVETYPE_BATTLE1:
	case CHARMOVETYPE_BATTLE2:
		// スポーナーと同じく、戦闘NPCは必ずぶつかり判定を持たせる
		infoTmp.m_bBlock = TRUE;
		break;
	}

	pCharLib->Enter();
	CInfoCharSvr *pAdded = (CInfoCharSvr *)pCharLib->AddNPC(&infoTmp);
	if (pAdded == NULL) {
		pCharLib->Leave();
		SetErrorResponse(response, "HTTP/1.1 503 Service Unavailable", "add_npc_failed");
		return;
	}
	pAdded->SetMap(pInfoMap);
	pAdded->SetLibInfoChar(pCharLib);

	DWORD dwCharID = pAdded->m_dwCharID;
	pCharLib->Leave();

	if (m_pMgrData->GetLog() != NULL) {
		m_pMgrData->GetLog()->Write("[DebugWorld] npc added charId=%u mapId=%d pos=(%d,%d) moveType=%d grpIdNpc=%d hp=%d",
			(unsigned int)dwCharID, nMapId, nX, nY, nMoveType, nGrpIdNpc, nHP);
	}

	std::ostringstream oss;
	oss << "{"
	    << "\"charId\":" << dwCharID << ","
	    << "\"mapId\":" << nMapId << ","
	    << "\"x\":" << nX << ","
	    << "\"y\":" << nY << ","
	    << "\"moveType\":" << nMoveType << ","
	    << "\"hp\":" << nHP
	    << "}";
	response.statusLine = "HTTP/1.1 201 Created";
	response.SetJsonBody(oss.str());
}

/// @brief キャラの HP / SP を書き換える。
/// @details サーバー側の値だけを変える(クライアントの表示は次にダメージ等で
///          ステータスが届いた時に揃う)。hp: 1 にして敵に殴られれば気絶を試せる。
void CDebugWorldHandler::HandleCharStatus(const HttpRequest &request, HttpResponse &response)
{
	int nCharId = 0;
	if (!JsonUtils::TryGetInt(request.body, "charId", nCharId) || (nCharId <= 0)) {
		SetErrorResponse(response, "HTTP/1.1 400 Bad Request", "charId_required");
		return;
	}

	CLibInfoCharSvr *pCharLib = m_pMgrData->GetLibInfoChar();
	if (pCharLib == NULL) {
		SetErrorResponse(response, "HTTP/1.1 503 Service Unavailable", "backend_unavailable");
		return;
	}

	pCharLib->Enter();
	CInfoCharBase *pChar = FindCharByID(pCharLib, (DWORD)nCharId);
	if (pChar == NULL) {
		pCharLib->Leave();
		SetErrorResponse(response, "HTTP/1.1 404 Not Found", "char_not_found");
		return;
	}

	int nValue = 0;
	if (JsonUtils::TryGetInt(request.body, "hp", nValue) && (nValue >= 0)) {
		pChar->m_dwHP = min((DWORD)nValue, pChar->m_dwMaxHP);
	}
	if (JsonUtils::TryGetInt(request.body, "sp", nValue) && (nValue >= 0)) {
		pChar->m_dwSP = min((DWORD)nValue, pChar->m_dwMaxSP);
	}

	std::ostringstream oss;
	oss << "{"
	    << "\"charId\":" << nCharId << ","
	    << "\"hp\":" << pChar->m_dwHP << ","
	    << "\"maxHp\":" << pChar->m_dwMaxHP << ","
	    << "\"sp\":" << pChar->m_dwSP << ","
	    << "\"maxSp\":" << pChar->m_dwMaxSP
	    << "}";
	pCharLib->Leave();

	response.statusLine = "HTTP/1.1 200 OK";
	response.SetJsonBody(oss.str());
}

#endif // _DEBUG
