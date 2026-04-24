#pragma once

#include "Web/ApiHandler.h"

#include <sstream>
#include <string>

class CMgrData;
class CInfoItem;

// ---------------------------------------------------------------------------
// /api/items — アイテム（インスタンス）CRUD ハンドラ
//
// GET    /api/items      一覧取得（検索パラメータ: ?mapId=N / ?charId=N / ?drop=1）
// POST   /api/items      新規作成（body で itemTypeId + 任意フィールド）
// PUT    /api/items      更新（body.itemId 必須）
// DELETE /api/items      削除（body.itemId 必須、または body.itemIds: [...]）
//
// 旧 MFC の IDM_ITEM_LIST / DlgAdminItemList / DlgAdminItemNew 相当。
// ---------------------------------------------------------------------------

class CItemListHandler : public IApiHandler
{
public:
        CItemListHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CItemCreateHandler : public IApiHandler
{
public:
        CItemCreateHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CItemUpdateHandler : public IApiHandler
{
public:
        CItemUpdateHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CItemDeleteHandler : public IApiHandler
{
public:
        CItemDeleteHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};
