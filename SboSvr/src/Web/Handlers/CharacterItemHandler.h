#pragma once

#include "Web/ApiHandler.h"

#include <string>
#include <memory>

class CMgrData;
class IApiHandler;

// キャラクター所持アイテム / 所持スキル API ハンドラ
//
// アイテム:
//   GET    /api/characters/{charId}/items          → [{slot, itemId}]
//   POST   /api/characters/{charId}/items          → 201 {slot, itemId}
//   DELETE /api/characters/{charId}/items/{slot}   → 204
//
// スキル:
//   GET    /api/characters/{charId}/skills         → [{slot, skillId}]
//   POST   /api/characters/{charId}/skills         → 201 {slot, skillId}
//   DELETE /api/characters/{charId}/skills/{slot}  → 204
//
// スロット上限:
//   アイテム: m_nMaxItemCount (キャラデータのフィールド値)
//   スキル  : MAX_CHAR_SKILL_SLOTS (= 64, 専用フィールド無しのため実装上の固定値)
//
// pFallbackHandler: items/skills 以外のサブパスが来た場合に委譲するハンドラ（所有権を持つ）
//                   NULL の場合は 404 を返す

class CCharacterItemHandler : public IApiHandler
{
public:
        // pFallbackHandler: items/skills 以外の GET リクエストに委譲するハンドラ（所有権を受け取る）
        explicit CCharacterItemHandler(CMgrData *pMgrData, IApiHandler *pFallbackHandler = NULL);
        virtual ~CCharacterItemHandler();
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
        IApiHandler *m_pFallbackHandler;    // items/skills 以外の GET を委譲するハンドラ（所有）

        // GET /api/characters/{charId}/items
        void HandleGetItems(const HttpRequest &request, HttpResponse &response, int nCharId);
        // POST /api/characters/{charId}/items
        void HandlePostItem(const HttpRequest &request, HttpResponse &response, int nCharId);
        // DELETE /api/characters/{charId}/items/{slot}
        void HandleDeleteItem(const HttpRequest &request, HttpResponse &response, int nCharId, int nSlot);

        // GET /api/characters/{charId}/skills
        void HandleGetSkills(const HttpRequest &request, HttpResponse &response, int nCharId);
        // POST /api/characters/{charId}/skills
        void HandlePostSkill(const HttpRequest &request, HttpResponse &response, int nCharId);
        // DELETE /api/characters/{charId}/skills/{slot}
        void HandleDeleteSkill(const HttpRequest &request, HttpResponse &response, int nCharId, int nSlot);
};
