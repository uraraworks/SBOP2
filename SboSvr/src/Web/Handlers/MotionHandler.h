#pragma once

#include "Web/ApiHandler.h"

#include <sstream>
#include <string>

class CMgrData;

// ---------------------------------------------------------------------------
// /api/motion-types  — モーション種別 CRUD ハンドラ
// /api/motions       — モーション個別 CRUD ハンドラ
//
// GET    /api/motion-types          一覧取得
// POST   /api/motion-types          新規作成
// PUT    /api/motion-types          更新（body.motionTypeId 必須）
// DELETE /api/motion-types          削除（body.motionTypeId 必須）
//
// GET    /api/motions?motionTypeId=N  指定種別のモーション一覧（省略時は全件）
// POST   /api/motions                 新規作成（body.motionTypeId, motionListId 必須）
// PUT    /api/motions                 更新（body.motionId 必須）
// DELETE /api/motions                 削除（body.motionId 必須）
//
// 旧 MFC の DlgAdminCharMotionTypeList / DlgAdminCharMotionList 相当。
// ---------------------------------------------------------------------------

// ---- モーション種別 --------------------------------------------------------

class CMotionTypeListHandler : public IApiHandler
{
public:
        CMotionTypeListHandler(CMgrData *pMgrData);
        void Handle(const HttpRequest &request, HttpResponse &response) override;
private:
        CMgrData *m_pMgrData;
};

class CMotionTypeCreateHandler : public IApiHandler
{
public:
        CMotionTypeCreateHandler(CMgrData *pMgrData);
        void Handle(const HttpRequest &request, HttpResponse &response) override;
private:
        CMgrData *m_pMgrData;
};

class CMotionTypeUpdateHandler : public IApiHandler
{
public:
        CMotionTypeUpdateHandler(CMgrData *pMgrData);
        void Handle(const HttpRequest &request, HttpResponse &response) override;
private:
        CMgrData *m_pMgrData;
};

class CMotionTypeDeleteHandler : public IApiHandler
{
public:
        CMotionTypeDeleteHandler(CMgrData *pMgrData);
        void Handle(const HttpRequest &request, HttpResponse &response) override;
private:
        CMgrData *m_pMgrData;
};

// ---- モーション個別 --------------------------------------------------------

class CMotionListHandler : public IApiHandler
{
public:
        CMotionListHandler(CMgrData *pMgrData);
        void Handle(const HttpRequest &request, HttpResponse &response) override;
private:
        CMgrData *m_pMgrData;
};

class CMotionCreateHandler : public IApiHandler
{
public:
        CMotionCreateHandler(CMgrData *pMgrData);
        void Handle(const HttpRequest &request, HttpResponse &response) override;
private:
        CMgrData *m_pMgrData;
};

class CMotionUpdateHandler : public IApiHandler
{
public:
        CMotionUpdateHandler(CMgrData *pMgrData);
        void Handle(const HttpRequest &request, HttpResponse &response) override;
private:
        CMgrData *m_pMgrData;
};

class CMotionDeleteHandler : public IApiHandler
{
public:
        CMotionDeleteHandler(CMgrData *pMgrData);
        void Handle(const HttpRequest &request, HttpResponse &response) override;
private:
        CMgrData *m_pMgrData;
};
