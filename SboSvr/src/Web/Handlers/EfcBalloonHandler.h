#pragma once

#include "Web/ApiHandler.h"

class CMgrData;

// ---------------------------------------------------------------------------
// /api/efc-balloons — 噴出し（吹き出し）情報 CRUD ハンドラ
//
// GET    /api/efc-balloons        一覧取得（コマ単位の flat 配列）
// POST   /api/efc-balloons        新規作成（body: listId 必須、他は任意）
// PUT    /api/efc-balloons        更新（body.efcBalloonId 必須）
// DELETE /api/efc-balloons        削除（body.efcBalloonId 必須）
//
// 旧 MFC の IDM_EFC_BALLOON_LIST / DlgAdminEfcBalloonList /
// DlgAdminEfcBalloon 相当。
//
// 噴出しは「種別ID (listId) 単位のアニメーションコマ集合」のため、
// 変更時はその listId 全コマを束ねた CPacketEFFECT_BALLOONINFO を
// 全クライアントへブロードキャストする（ネイティブ版 RecvProcADMIN_EFC_RENEWBALLOON と同等）。
//
// フィールド:
//   efcBalloonId   噴出しID（主キー、POST では自動採番）
//   listId         種別ID
//   animeId        コマ番号
//   wait           待ち時間（×10ms）
//   loop           ループ再生フラグ
//   soundId        効果音ID
//   grpId          画像ID
//   name           噴出し名
// ---------------------------------------------------------------------------

class CEfcBalloonListHandler : public IApiHandler
{
public:
        CEfcBalloonListHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CEfcBalloonCreateHandler : public IApiHandler
{
public:
        CEfcBalloonCreateHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CEfcBalloonUpdateHandler : public IApiHandler
{
public:
        CEfcBalloonUpdateHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CEfcBalloonDeleteHandler : public IApiHandler
{
public:
        CEfcBalloonDeleteHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};
