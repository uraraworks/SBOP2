/// @file ImGuiDbg.h
/// @brief ImGui版デバッグ情報表示

#pragma once

class CMgrData;
class CUraraSockTCPSBO;

class CImGuiDbg
{
public:
    CImGuiDbg();
    ~CImGuiDbg();

    void Init(CMgrData *pMgrData);
    void Draw();  // ImGuiウィジェット描画（毎フレーム呼ぶ）

private:
    CMgrData *m_pMgrData;
    CUraraSockTCPSBO *m_pSock;
    bool m_bVisible;
};
