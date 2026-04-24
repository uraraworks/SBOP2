#pragma once

#include "Web/ApiHandler.h"

class CMgrData;

// ---------------------------------------------------------------------------
// /api/effects — エフェクト情報 CRUD ハンドラ
//
// GET    /api/effects        一覧取得（エフェクト単位、アニメコマを nested 配列で含む）
// POST   /api/effects        新規作成（body: name 任意、各フィールド任意）
// PUT    /api/effects        更新（body.effectId 必須）
// DELETE /api/effects        削除（body.effectId 必須）
//
// 旧 MFC の IDM_EFC_EFFECT_LIST / DlgAdminEfcEffectList /
// DlgAdminEfcEffect 相当。
//
// 更新時はネイティブ版 RecvProcADMIN_EFC_RENEWEFFECT と同等に
// CPacketEFFECT_RES_EFFECTINFO（単体）を全クライアントへ配信。
// 削除時は CPacketEFFECT_DELETEEFFECTINFO を配信。
//
// フィールド（エフェクト本体）:
//   effectId       エフェクトID（主キー、POST では自動採番）
//   name           エフェクト名
//   soundId        効果音ID
//   grpIdMain      画像メインID
//   loop           ループ再生フラグ
//   loopSound      ループ時に効果音を再生するフラグ
//   animeCount     アニメーションコマ数（animes.length に同期）
//   animes         アニメーションコマ配列
//     wait         待ち時間（×10ms, 0-255）
//     level        透明度（0-255）
//     grpIdBase    画像ID（下地）
//     grpIdPile    画像ID（重ね合わせ）
// ---------------------------------------------------------------------------

class CEfcListHandler : public IApiHandler
{
public:
        CEfcListHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CEfcCreateHandler : public IApiHandler
{
public:
        CEfcCreateHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CEfcUpdateHandler : public IApiHandler
{
public:
        CEfcUpdateHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CEfcDeleteHandler : public IApiHandler
{
public:
        CEfcDeleteHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};
