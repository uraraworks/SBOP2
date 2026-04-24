#pragma once

#include "Web/ApiHandler.h"

#include <sstream>
#include <string>

class CMgrData;
class CInfoItemWeapon;

// ---------------------------------------------------------------------------
// /api/weapons — 武器情報 CRUD ハンドラ
//
// GET    /api/weapons           一覧取得
// POST   /api/weapons           新規作成（body で各種パラメータ、ID は自動採番）
// PUT    /api/weapons           更新（body.weaponInfoId 必須）
// DELETE /api/weapons           削除（body.weaponInfoId 必須）
//
// 旧 MFC の IDM_ITEM_WEAPONLIST / DlgAdminItemWeaponList /
// DlgAdminItemWeaponNew 相当。
// フィールド: weaponInfoId / name / motionType (bit flag) /
//             motionTypeStand / motionTypeWalk /
//             effectIdAtack[] / effectIdCritical[]
// ---------------------------------------------------------------------------

class CWeaponListHandler : public IApiHandler
{
public:
        CWeaponListHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CWeaponCreateHandler : public IApiHandler
{
public:
        CWeaponCreateHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CWeaponUpdateHandler : public IApiHandler
{
public:
        CWeaponUpdateHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CWeaponDeleteHandler : public IApiHandler
{
public:
        CWeaponDeleteHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};
