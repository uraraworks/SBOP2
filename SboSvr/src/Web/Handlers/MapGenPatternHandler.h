#pragma once

#include "Web/ApiHandler.h"

#include <string>

class CMgrData;

// ---------------------------------------------------------------------------
// /api/map-gen-patterns — 自動生成マップパターン CRUD ハンドラ
//
// GET    /api/map-gen-patterns  一覧取得
// POST   /api/map-gen-patterns  新規作成（body: patternName, algoType, paramsJson, roleMapJson）
// PUT    /api/map-gen-patterns  更新（body: patternId 必須 + 上記フィールド）
// DELETE /api/map-gen-patterns  削除（body: patternId 必須）
//
// S1 フェーズ: 生成ロジックは持たない。パラメータを JSON 文字列のまま保存するのみ。
// ---------------------------------------------------------------------------

class CMapGenPatternListHandler : public IApiHandler
{
public:
    explicit CMapGenPatternListHandler(CMgrData *pMgrData);
    void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
    CMgrData *m_pMgrData;
};

class CMapGenPatternCreateHandler : public IApiHandler
{
public:
    explicit CMapGenPatternCreateHandler(CMgrData *pMgrData);
    void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
    CMgrData *m_pMgrData;
};

class CMapGenPatternUpdateHandler : public IApiHandler
{
public:
    explicit CMapGenPatternUpdateHandler(CMgrData *pMgrData);
    void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
    CMgrData *m_pMgrData;
};

class CMapGenPatternDeleteHandler : public IApiHandler
{
public:
    explicit CMapGenPatternDeleteHandler(CMgrData *pMgrData);
    void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
    CMgrData *m_pMgrData;
};
