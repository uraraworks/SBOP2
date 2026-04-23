#pragma once

#include "Web/ApiHandler.h"

#include <string>

class CMgrData;

// キャラクター更新ディスパッチャ
// PUT /api/characters/{charId}             → 基本情報更新
// PUT /api/characters/{charId}/status      → ステータス更新
// PUT /api/characters/{charId}/equipment   → 装備更新
// PUT /api/characters/{charId}/graphics    → グラフィック更新
// URL のサブパスを解析して内部で振り分ける
class CCharacterUpdateHandler : public IApiHandler
{
public:
        explicit CCharacterUpdateHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;

        // 基本情報更新
        void HandleBasic(const HttpRequest &request, HttpResponse &response, int nCharId);
        // ステータス更新
        void HandleStatus(const HttpRequest &request, HttpResponse &response, int nCharId);
        // 装備更新
        void HandleEquipment(const HttpRequest &request, HttpResponse &response, int nCharId);
        // グラフィック更新
        void HandleGraphics(const HttpRequest &request, HttpResponse &response, int nCharId);
};
