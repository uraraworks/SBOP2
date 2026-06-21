/// @file MainFrameRecvProcACCOUNT.cpp
/// @brief クライアントメインフレーム(アカウント系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/08
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoAccount.h"
#include "InfoAccount.h"
#include "LibInfoCharCli.h"
#include "MgrData.h"
#include "MainFrame.h"


void CMainFrame::RecvProcACCOUNT(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_ACCOUNT_RES_ACCOUNTINFO: RecvProcACCOUNT_RES_ACCOUNTINFO(pData); break; // アカウント情報応答
	case SBOCOMMANDID_SUB_ACCOUNT_RES_MAKECHAR: RecvProcACCOUNT_RES_MAKECHAR(pData); break; // キャラ作成応答
	case SBOCOMMANDID_SUB_ACCOUNT_RES_DELETECHAR: RecvProcACCOUNT_RES_DELETECHAR(pData); break; // キャラ削除応答
	}
}


void CMainFrame::RecvProcACCOUNT_RES_ACCOUNTINFO(PBYTE pData)
{
	CPacketACCOUNT_RES_ACCOUNTINFO Packet;

	Packet.Set(pData);

	m_pMgrData->SetAccount(Packet.m_pInfo);
	PostMainFrameMessage(MAINFRAMEMSG_RENEWACCOUNTINFO, 0);
}


void CMainFrame::RecvProcACCOUNT_RES_MAKECHAR(PBYTE pData)
{
	CPacketACCOUNT_RES_MAKECHAR Packet;
	PCInfoAccount pAccountInfo;

	Packet.Set(pData);

	switch (Packet.m_nResult) {
	case MAKECHARRES_OK: // 問題無し
		pAccountInfo = m_pMgrData->GetAccount();
		pAccountInfo->m_adwCharID.push_back(Packet.m_dwCharID);
		PostMainFrameMessage(MAINFRAMEMSG_RENEWACCOUNTINFO, 0);
		break;
	case MAKECHARRES_NG_USE: // 使用済み
	case MAKECHARRES_NG_SPACE: // 空白が使用されている
		break;
	}
	PostMainFrameMessage(MAINFRAMEMSG_RES_MAKECHAR, Packet.m_nResult);
}


void CMainFrame::RecvProcACCOUNT_RES_DELETECHAR(PBYTE pData)
{
	int i, nCount;
	CPacketACCOUNT_RES_DELETECHAR Packet;
	PCInfoAccount pAccountInfo;

	Packet.Set(pData);

	if (Packet.m_nResult == DELETECHARRES_OK) {
		// m_adwCharID から該当 CharID を削除
		pAccountInfo = m_pMgrData->GetAccount();
		nCount = (int)pAccountInfo->m_adwCharID.size();
		for (i = 0; i < nCount; i++) {
			if (pAccountInfo->m_adwCharID[i] == Packet.m_dwCharID) {
				pAccountInfo->m_adwCharID.erase(pAccountInfo->m_adwCharID.begin() + i);
				break;
			}
		}
		// LibInfoChar からキャラ情報を削除
		m_pMgrData->GetLibInfoChar()->Delete(Packet.m_dwCharID);
	}
	PostMainFrameMessage(MAINFRAMEMSG_RES_DELETECHAR, Packet.m_nResult);
}
