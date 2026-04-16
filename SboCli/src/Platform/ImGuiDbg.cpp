/// @file ImGuiDbg.cpp
/// @brief ImGui版デバッグ情報表示 実装

#include "stdafx.h"
#include <imgui.h>
#include "ImGuiDbg.h"
#include "MgrData.h"
#include "UraraSockTCPSBO.h"
#include "InfoCharCli.h"

// SDLApp.cpp で定義されたグローバル FPS カウンタ
extern BYTE g_byFpsLast;

CImGuiDbg::CImGuiDbg()
    : m_pMgrData(NULL)
    , m_pSock(NULL)
    , m_bVisible(true)
{
}

CImGuiDbg::~CImGuiDbg()
{
}

void CImGuiDbg::Init(CMgrData *pMgrData)
{
    m_pMgrData = pMgrData;
    if (pMgrData != NULL) {
        m_pSock = pMgrData->GetUraraSockTCP();
    }
}

void CImGuiDbg::Draw()
{
    if (!m_bVisible || m_pMgrData == NULL) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(320, 160), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin(u8"デバッグ", &m_bVisible)) {
        ImGui::End();
        return;
    }

    ImGui::Text(u8"オンライン：%d", m_pMgrData->GetOnlineCount());
    ImGui::Text(u8"キャラ数：%d", m_pMgrData->GetCharCount());
    ImGui::Text(u8"FPS：%d  Ping：%dms 描画時間:%4dms", (int)g_byFpsLast, m_pMgrData->GetPing(), m_pMgrData->GetDrawTime());

    PCInfoCharCli pChar = m_pMgrData->GetPlayerChar();
    if (pChar != NULL) {
        ImGui::Text(u8"座標：MAP:%d X:%3d Y:%3d", pChar->m_dwMapID, pChar->m_nMapX, pChar->m_nMapY);
    } else {
        ImGui::Text(u8"座標：");
    }

    if (m_pSock != NULL) {
        ImGui::Text(u8"送信：%5dBps 受信：%5dBps",
            m_pSock->GetThrowghPutSend(0),
            m_pSock->GetThrowghPutRecv(0));
    }

    ImGui::End();
}
