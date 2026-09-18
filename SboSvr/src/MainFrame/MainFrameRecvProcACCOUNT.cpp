/// @file MainFrameRecvProcACCOUNT.cpp
/// @brief サーバーメインフレーム(アカウント系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/07
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoAccount.h"
#include "InfoAccount.h"
#include "InfoCharSvr.h"
#include "LibInfoCharSvr.h"
#include "TextOutput.h"
#include "MgrData.h"
#include "MainFrame.h"

void CMainFrame::RecvProcACCOUNT(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_ACCOUNT_REQ_ACCOUNTINFO:	RecvProcACCOUNT_REQ_ACCOUNTINFO(pData, dwSessionID);	break;	// アカウント情報要求
	case SBOCOMMANDID_SUB_ACCOUNT_REQ_MAKECHAR:	RecvProcACCOUNT_REQ_MAKECHAR(pData, dwSessionID);	break;	// キャラ作成要求
	case SBOCOMMANDID_SUB_ACCOUNT_REQ_DELETECHAR:	RecvProcACCOUNT_REQ_DELETECHAR(pData, dwSessionID);	break;	// キャラ削除要求
	}
}

void CMainFrame::RecvProcACCOUNT_REQ_ACCOUNTINFO(PBYTE pData, DWORD dwSessionID)
{
	PCLibInfoAccount pLibInfoAccount;
	PCInfoAccount pInfoAccount;
	CPacketACCOUNT_REQ_ACCOUNTINFO Packet;
	CPacketACCOUNT_RES_ACCOUNTINFO PacketRES_ACCOUNTINFO;

	Packet.Set(pData);

	pLibInfoAccount	= m_pMgrData->GetLibInfoAccount();
	pInfoAccount	= pLibInfoAccount->GetPtrSessionID(dwSessionID);
	if (pInfoAccount == NULL) {
		return;
	}
	if (pInfoAccount->m_dwAccountID != Packet.m_dwAccountID) {
		m_pLog->Write("■ 他アカウント指定(ACCOUNTINFO) dwSessionID:%u 要求AccountID:%u 本人AccountID:%u", dwSessionID, Packet.m_dwAccountID, pInfoAccount->m_dwAccountID);
		RequestDisconnect(dwSessionID);
		return;
	}

	PacketRES_ACCOUNTINFO.Make(pInfoAccount);
	m_pSock->SendTo(dwSessionID, &PacketRES_ACCOUNTINFO);
}

void CMainFrame::RecvProcACCOUNT_REQ_MAKECHAR(PBYTE pData, DWORD dwSessionID)
{
	int nResult;
	DWORD dwCharID;
	PCLibInfoAccount pLibInfoAccount;
	PCInfoCharBase pInfoCharPacket;
	PCInfoAccount pInfoAccount;
	CPacketACCOUNT_REQ_MAKECHAR Packet;
	CPacketACCOUNT_RES_MAKECHAR PacketRES_MAKECHAR;
	CmyString strName;

	dwCharID	= 0;

	Packet.Set(pData);
	pInfoCharPacket = Packet.m_pInfoCharBase;

	pLibInfoAccount	= m_pMgrData->GetLibInfoAccount();
	pInfoAccount	= pLibInfoAccount->GetPtrSessionID(dwSessionID);
	if (pInfoAccount == NULL) {
		return;
	}
	if (pInfoAccount->m_dwAccountID != Packet.m_dwAccountID) {
		m_pLog->Write("■ 他アカウント指定(MAKECHAR) dwSessionID:%u 要求AccountID:%u 本人AccountID:%u", dwSessionID, Packet.m_dwAccountID, pInfoAccount->m_dwAccountID);
		RequestDisconnect(dwSessionID);
		return;
	}

	TrimViewString(strName, (LPCTSTR)pInfoCharPacket->m_strCharName);

	// 名前検証込みのキャラ作成本体は CLibInfoCharSvr::CreatePlayerCharacter に切り出し、
	// /api/debug/fixture(_DEBUG限定のHTTPテスト準備API)と共用している。
	// 種族別の初期服装分岐(#if 0)が元々無効だった点も含め、挙動は変えていない。
	nResult = m_pLibInfoChar->CreatePlayerCharacter(
		pInfoCharPacket->m_strCharName, strName,
		pInfoCharPacket->m_nMoveType, pInfoCharPacket->m_nSex, pInfoCharPacket->m_wFamilyID,
		pInfoCharPacket->m_wGrpIDEye, pInfoCharPacket->m_wGrpIDEyeColor,
		pInfoCharPacket->m_wGrpIDHairType, pInfoCharPacket->m_wGrpIDHairColor,
		pInfoCharPacket->m_dwMotionTypeID, pInfoAccount, dwCharID);

	PacketRES_MAKECHAR.Make(nResult, dwCharID);
	m_pSock->SendTo(dwSessionID, &PacketRES_MAKECHAR);
}

void CMainFrame::RecvProcACCOUNT_REQ_DELETECHAR(PBYTE pData, DWORD dwSessionID)
{
	int i, nCount, nResult;
	PCLibInfoAccount pLibInfoAccount;
	PCInfoAccount pInfoAccount;
	CPacketACCOUNT_REQ_DELETECHAR Packet;
	CPacketACCOUNT_RES_DELETECHAR PacketRES_DELETECHAR;

	nResult	= DELETECHARRES_NG;

	Packet.Set(pData);

	pLibInfoAccount	= m_pMgrData->GetLibInfoAccount();
	pInfoAccount	= pLibInfoAccount->GetPtrSessionID(dwSessionID);
	if (pInfoAccount == NULL) {
		goto Exit;
	}
	if (pInfoAccount->m_dwAccountID != Packet.m_dwAccountID) {
		m_pLog->Write("■ 他アカウント指定(DELETECHAR) dwSessionID:%u 要求AccountID:%u 本人AccountID:%u", dwSessionID, Packet.m_dwAccountID, pInfoAccount->m_dwAccountID);
		RequestDisconnect(dwSessionID);
		return;
	}

	// m_adwCharID から該当 CharID を検索して削除(本人所有のキャラのみ)
	nCount = (int)pInfoAccount->m_adwCharID.size();
	for (i = 0; i < nCount; i++) {
		if (pInfoAccount->m_adwCharID[i] == Packet.m_dwCharID) {
			pInfoAccount->m_adwCharID.erase(pInfoAccount->m_adwCharID.begin() + i);
			m_pLibInfoChar->Delete(Packet.m_dwCharID);
			nResult = DELETECHARRES_OK;
			break;
		}
	}

Exit:
	PacketRES_DELETECHAR.Make(nResult, Packet.m_dwCharID);
	m_pSock->SendTo(dwSessionID, &PacketRES_DELETECHAR);
}
