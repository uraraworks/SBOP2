/// @file TalkEventHandler.h
/// @brief 会話イベント情報 Web API ハンドラ 定義ファイル
///        /api/talk-events （一覧 / 取得 / 更新 / 削除）を提供する
///
/// 会話イベント (CInfoTalkEvent) はキャラクターID = TalkEventID で紐づく
/// ネイティブ版 DlgAdminTalkEvent* 系ダイアログの Web 置き換え。
/// 将来的に NPC / マップイベント など複数機能から editor として呼び出される想定。
#pragma once

#include "Web/ApiHandler.h"

#include <string>

class CMgrData;

// 会話イベント一覧取得 GET /api/talk-events
//   - id 指定時は単一会話イベントを返す
//   - 指定なしは全 TalkEventID の概要を返す
class CTalkEventListHandler : public IApiHandler
{
public:
        explicit CTalkEventListHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};

// 会話イベント更新（なければ新規追加） PUT /api/talk-events
//   リクエストボディに TalkEventID + events[] を丸ごと受け取り差し替える
class CTalkEventUpdateHandler : public IApiHandler
{
public:
        explicit CTalkEventUpdateHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};

// 会話イベント削除 DELETE /api/talk-events?id=<talkEventId>
class CTalkEventDeleteHandler : public IApiHandler
{
public:
        explicit CTalkEventDeleteHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};
