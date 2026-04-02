/// @file MainFrameRecvProcVERSION.cpp
/// @brief クライアントメインフレーム(バージョン系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/05
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "Window/ILoginWindow.h"
#include "WindowLOGIN.h"
#include "MacAddr.h"
#include "MgrWindow.h"
#include "MainFrame.h"


void CMainFrame::RecvProcVERSION(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_VERSION_RES_VERSIONCHECK: RecvProcVERSION_RES_VERSIONCHECK(pData); break; // バージョンチェック応答
	case SBOCOMMANDID_SUB_VERSION_RES_FILELISTCHECK: RecvProcVERSION_RES_FILELISTCHECK(pData); break; // ファイルリストチェック応答
	}
}


void CMainFrame::RecvProcVERSION_RES_VERSIONCHECK(PBYTE pData)
{
	int nCount = 0;
	BYTE abyTmp[10];
	ILoginWindow *pWindow;
	CPacketVERSION_RES_VERSIONCHECK Packet;
	CPacketCONNECT_REQ_LOGIN PacketLOGIN;
	CMacAddr MacAddr;

	Packet.Set(pData);

	switch (Packet.m_nResult) {
	case VERSIONCHECKRES_OK: // 問題無し
		pWindow = m_pMgrWindow->GetLoginWindow();
		if (pWindow == NULL) {
			break;
		}
		nCount = MacAddr.GetCount();
		for (int i = 0; i < nCount; i++) {
			ZeroMemory(abyTmp, sizeof(abyTmp));
			BOOL bResult = MacAddr.Get(abyTmp, i);
			if (bResult) {
				break;
			}
		}
		PacketLOGIN.Make(pWindow->GetAccount(), pWindow->GetPassword(), abyTmp);
		m_pSock->Send(&PacketLOGIN);
		break;

	case VERSIONCHECKRES_NG_VERSION: // バージョン不一致
		m_pSock->Destroy();
		DisConnectProc(DISCONNECTID_VERSION);
		break;
	}
}


void CMainFrame::RecvProcVERSION_RES_FILELISTCHECK(PBYTE pData)
{
	CPacketVERSION_RES_FILELISTCHECK Packet;

	Packet.Set(pData);

	switch (Packet.m_nResult) {
	case FILELISTCHECKRES_OK: // 問題無し
		break;
	case FILELISTCHECKRES_NG: // 不一致
		break;
	}
}
