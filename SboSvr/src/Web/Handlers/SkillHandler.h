#pragma once

#include "Web/ApiHandler.h"

#include <sstream>
#include <string>

class CMgrData;
class CInfoSkillBase;

// ---------------------------------------------------------------------------
// /api/skills — スキルマスタ CRUD ハンドラ
//
// GET    /api/skills           一覧取得
// POST   /api/skills           新規作成（body で typeMain, typeSub と各種パラメータ）
// PUT    /api/skills           更新（body.skillId 必須）
// DELETE /api/skills           削除（body.skillId 必須）
//
// 旧 MFC の DlgAdminCharSkillList / DlgAdminCharSkillBase 相当。
// 基底フィールド（skillId, name, sp, iconId, typeMain, typeSub, use）+
// 派生クラス固有フィールド（MOVEATACK 系: targetType, hitEffectId 等、
// HEAL 系: area, healType 等）を JSON で扱う。
// ---------------------------------------------------------------------------

class CSkillListHandler : public IApiHandler
{
public:
        CSkillListHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CSkillCreateHandler : public IApiHandler
{
public:
        CSkillCreateHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CSkillUpdateHandler : public IApiHandler
{
public:
        CSkillUpdateHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};

class CSkillDeleteHandler : public IApiHandler
{
public:
        CSkillDeleteHandler(CMgrData *pMgrData);

        void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
        CMgrData *m_pMgrData;
};
