#pragma once

#include "Web/ApiHandler.h"

#include <map>
#include <mutex>
#include <string>

class CMgrData;

// 選択対象の種別
enum SelectionType
{
    SelectionTypeMapCell,    // マップセル
    SelectionTypeChar,       // キャラクター
    SelectionTypePlacement,  // 配置オブジェクト
    SelectionTypeEvent,      // イベント
};

// 選択状態を表す構造体
struct Selection
{
    SelectionType   type;
    int             nMapId;
    int             nX;
    int             nY;
    int             nRefId;     // オプション（キャラID / 配置ID / イベントID 等）
    bool            bHasRefId;  // refId が指定されているか
    std::string     updatedAt;  // ISO8601 形式の更新日時
};

// セッション単位の選択状態ストア（シングルトン）
class CSelectionStore
{
public:
    static CSelectionStore &GetInstance();

    // 選択状態を設定する（sessionId をキーに上書き）
    void Set(const std::string &sessionId, const Selection &selection);

    // 選択状態を取得する。存在しない場合は false を返す
    bool Get(const std::string &sessionId, Selection &outSelection) const;

    // 選択状態を削除する
    void Clear(const std::string &sessionId);

private:
    CSelectionStore() {}
    CSelectionStore(const CSelectionStore &);
    CSelectionStore &operator=(const CSelectionStore &);

    mutable std::mutex              m_mutex;
    std::map<std::string, Selection> m_store;
};

// POST /api/selection/pick — 対象を選択する
class CSelectionPickHandler : public IApiHandler
{
public:
    explicit CSelectionPickHandler(CMgrData *pMgrData);
    virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
    bool ParseSelection(const std::string &body, Selection &outSelection) const;
    std::string BuildSelectionJson(const Selection &selection) const;
    std::string GetCurrentIso8601() const;
    CMgrData *m_pMgrData;
};

// GET /api/selection — 現在の選択状態を返す
class CSelectionGetHandler : public IApiHandler
{
public:
    explicit CSelectionGetHandler(CMgrData *pMgrData);
    virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
    std::string BuildSelectionJson(const Selection &selection) const;
    CMgrData *m_pMgrData;
};

// DELETE /api/selection — 選択を解除する
class CSelectionDeleteHandler : public IApiHandler
{
public:
    explicit CSelectionDeleteHandler(CMgrData *pMgrData);
    virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
    CMgrData *m_pMgrData;
};
