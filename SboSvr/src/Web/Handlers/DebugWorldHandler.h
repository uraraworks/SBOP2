/// @file DebugWorldHandler.h
/// @brief /api/debug/npc・/api/debug/char-status (テスト準備API) 定義ファイル
/// @details _DEBUG ビルドにのみ存在する。E2E の自動確認やスクショ撮影のために、
///          空の DB でも戦闘を試せるよう、敵を置いたりキャラの HP を変えたりする。
///          /api/debug/fixture と同じく、実行時にも loopback からの直接リクエスト
///          以外は 404 として扱う(DebugFixtureGuard::IsAllowedRequest 参照)。
///          Release ビルドの .vcxproj からは除外してある。

#pragma once

#ifdef _DEBUG

#include "Web/ApiHandler.h"

class CMgrData;

/// POST /api/debug/npc : mapId, x, y, charName?, grpIdNpc?, moveType?, hp?, atack?,
///                       defense?, searchX?, searchY? -> charId, mapId, x, y, hp
/// POST /api/debug/char-status : charId, hp?, sp? -> charId, hp, maxHp, sp, maxSp
class CDebugWorldHandler : public IApiHandler
{
public:
	explicit CDebugWorldHandler(CMgrData *pMgrData);
	virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
	void HandleNpc(const HttpRequest &request, HttpResponse &response);
	void HandleCharStatus(const HttpRequest &request, HttpResponse &response);

	CMgrData *m_pMgrData;
};

#endif // _DEBUG
