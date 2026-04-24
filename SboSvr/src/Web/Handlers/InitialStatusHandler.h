#pragma once

#include "Web/ApiHandler.h"

class CMgrData;

// ---------------------------------------------------------------------------
// /api/initial-status — キャラクター初期ステータス GET / PUT ハンドラ
//
// GET /api/initial-status        現在値の取得
// PUT /api/initial-status        値の更新（全フィールド指定可、未指定は据え置き）
//
// 旧 MFC の IDM_SYSTEM_SET_INITCHARSTATUS / DlgAdminSystemSetInitCharStatus 相当。
// ネイティブ版の RecvProcADMIN_SYSTEM_RENEWINFO と同等に、更新後は
// CPacketSYSTEM_INFO を全クライアントへ配信する。
//
// データはシステム情報ライブラリ (CLibInfoSystem) が保持する単一レコード
// STSYSTEM_INITCHARSTATUS。POST/DELETE は提供しない。
//
// フィールド:
//   mapId                  初期位置マップID (DWORD)
//   posX / posY            初期位置座標 (LONG)
//   maxHp / maxSp          最大HP/SP (DWORD)
//   stamina                スタミナ (WORD)
//   power                  腕力 (WORD)
//   strength               体力 (WORD)
//   magic                  魔力 (WORD)
//   skillful               器用 (WORD)
//   abilityAt / abilityDf  攻撃技能 / 防御技能 (WORD)
//   pAtack / pDefense      攻撃力 / 防御力 (WORD)
//   pMagic / pMagicDefense 魔法力 / 魔法防御力 (WORD)
//   pHitAverage            命中率 (WORD)
//   pAvoidAverage          回避率 (WORD)
//   pCriticalAverage       クリティカル率 (WORD)
//   attrFire / attrWind    属性 火 / 風 (WORD)
//   attrWater / attrEarth  属性 水 / 土 (WORD)
//   attrLight / attrDark   属性 光 / 闇 (WORD)
// ---------------------------------------------------------------------------

class CInitialStatusGetHandler : public IApiHandler
{
public:
        CInitialStatusGetHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CInitialStatusUpdateHandler : public IApiHandler
{
public:
        CInitialStatusUpdateHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};
