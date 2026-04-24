#pragma once

#include "Web/ApiHandler.h"

#include <sstream>
#include <string>

class CMgrData;
class CInfoItemTypeBase;

// ---------------------------------------------------------------------------
// /api/item-types — アイテム種別 CRUD ハンドラ
//
// GET    /api/item-types           一覧取得
// POST   /api/item-types           新規作成（body で itemTypeId と各種パラメータ）
// PUT    /api/item-types           更新（body.typeId 必須）
// DELETE /api/item-types           削除（body.typeId 必須）
//
// 旧 MFC の IDM_ITEM_TYPELIST / DlgAdminItemTypeList 相当。
// DlgAdminItemTypeNew の基本フィールド + ARMS/HP/LIGHT 系サブクラスの任意フィールド
// （value, value2, moveWait, moveCount, weaponInfoId, target, area）を扱う。
// ---------------------------------------------------------------------------

class CItemTypeListHandler : public IApiHandler
{
public:
        CItemTypeListHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CItemTypeCreateHandler : public IApiHandler
{
public:
        CItemTypeCreateHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CItemTypeUpdateHandler : public IApiHandler
{
public:
        CItemTypeUpdateHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CItemTypeDeleteHandler : public IApiHandler
{
public:
        CItemTypeDeleteHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};
