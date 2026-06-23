#pragma once

#include "Web/ApiHandler.h"

#include <string>

class CMgrData;

// キャラクター更新ディスパッチャ
// PUT /api/characters/{charId}             → 基本情報更新
// PUT /api/characters/{charId}/status      → ステータス更新
// PUT /api/characters/{charId}/equipment   → 装備更新
// PUT /api/characters/{charId}/graphics    → グラフィック更新
// PUT /api/characters/{charId}/movement    → 移動・探索設定更新
// PUT /api/characters/{charId}/account     → アカウント紐付け変更
// PUT /api/characters/{charId}/admin       → 管理者権限レベル設定
// PUT /api/characters/{charId}/disabled    → キャラ無効化/解除（アカウント m_bDisable）
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
        // 移動・探索設定更新
        void HandleMovement(const HttpRequest &request, HttpResponse &response, int nCharId);
        // アカウント紐付け変更
        void HandleAccount(const HttpRequest &request, HttpResponse &response, int nCharId);
        // 管理者権限レベル設定
        void HandleAdmin(const HttpRequest &request, HttpResponse &response, int nCharId);
        // キャラ無効化/解除
        void HandleDisabled(const HttpRequest &request, HttpResponse &response, int nCharId);
};
