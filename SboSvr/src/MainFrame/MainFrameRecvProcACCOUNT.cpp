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
	pInfoAccount	= pLibInfoAccount->GetPtr(Packet.m_dwAccountID);
	if (pInfoAccount == NULL) {
		return;
	}

	PacketRES_ACCOUNTINFO.Make(pInfoAccount);
	m_pSock->SendTo(dwSessionID, &PacketRES_ACCOUNTINFO);
}

void CMainFrame::RecvProcACCOUNT_REQ_MAKECHAR(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	int nResult;
	DWORD dwCharID;
	PCLibInfoAccount pLibInfoAccount;
	PCInfoCharBase pInfoChar, pInfoCharPacket;
	PCInfoAccount pInfoAccount;
	CPacketACCOUNT_REQ_MAKECHAR Packet;
	CPacketACCOUNT_RES_MAKECHAR PacketRES_MAKECHAR;
	CmyString strName;

	nResult	= MAKECHARRES_NG_USE;
	dwCharID	= 0;

	Packet.Set(pData);
	pInfoCharPacket = Packet.m_pInfoCharBase;

	pLibInfoAccount	= m_pMgrData->GetLibInfoAccount();
	pInfoAccount	= pLibInfoAccount->GetPtr(Packet.m_dwAccountID);
	if (pInfoAccount == NULL) {
		return;
	}

	TrimViewString(strName, (LPCTSTR)pInfoCharPacket->m_strCharName);
	bResult = m_pLibInfoChar->IsUseName(strName);
	if (bResult) {
		goto Exit;
	}

	bResult = m_pLibInfoChar->NameCheck(pInfoCharPacket->m_strCharName);
	if (bResult == FALSE) {
		nResult = MAKECHARRES_NG_SPACE;
		goto Exit;
	}

	nResult = MAKECHARRES_OK;
	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetNew(pInfoCharPacket->m_nMoveType);
	m_pLibInfoChar->SetInitStatus((PCInfoCharSvr)pInfoChar, TRUE);

	pInfoChar->m_strCharName	= pInfoCharPacket->m_strCharName;
	pInfoChar->m_nSex	= pInfoCharPacket->m_nSex;
	pInfoChar->m_wFamilyID	= pInfoCharPacket->m_wFamilyID;
	pInfoChar->m_wGrpIDCloth	= pInfoCharPacket->m_wGrpIDCloth;
	pInfoChar->m_wGrpIDEye	= pInfoCharPacket->m_wGrpIDEye;
	pInfoChar->m_wGrpIDEyeColor	= pInfoCharPacket->m_wGrpIDEyeColor;
	pInfoChar->m_wGrpIDHairType	= pInfoCharPacket->m_wGrpIDHairType;
	pInfoChar->m_wGrpIDHairColor	= pInfoCharPacket->m_wGrpIDHairColor;
	pInfoChar->m_dwMotionTypeID	= pInfoCharPacket->m_dwMotionTypeID;
	if (pInfoCharPacket->m_strCharName == "春うらら") {
		pInfoChar->m_wGrpIDSP = 1;
		pInfoChar->m_clName	= RGB(255, 200, 100);
		pInfoChar->m_clSpeak	= RGB(255, 200, 100);
		pInfoChar->m_abyMark.push_back(2);
	} else if (pInfoCharPacket->m_strCharName == "VeLTiNA") {
		pInfoChar->m_wGrpIDSP = 2;
		pInfoChar->m_clName	= RGB(255, 150, 150);
		pInfoChar->m_clSpeak	= RGB(255, 150, 150);
		pInfoChar->m_abyMark.push_back(3);
	} else if (pInfoCharPacket->m_strCharName == "あやしいの") {
		pInfoChar->m_wGrpIDSP = 3;
	}
	pInfoChar->m_abyMark.push_back(1);
#if 0
	switch (pInfoChar->m_wFamilyID) {
	case FAMILYTYPE_HUMAN:	// ニンゲン
		pInfoChar->m_wGrpIDCloth = 1;
		break;
	case FAMILYTYPE_ELF:	// エルフ
		pInfoChar->m_wGrpIDCloth = 2;
		break;
	case FAMILYTYPE_BST:	// ジュウジン
		pInfoChar->m_wGrpIDCloth = 3;
		break;
	case FAMILYTYPE_DAEMON:	// マゾク
		pInfoChar->m_wGrpIDCloth = 4;
		break;
	}
#endif
	pInfoChar->m_wGrpIDCloth = 0;
	pInfoChar->m_wGrpIDInitNPC	= pInfoChar->m_wGrpIDNPC;
	pInfoChar->m_wGrpIDInitCloth	= pInfoChar->m_wGrpIDCloth;
	pInfoChar->m_wGrpIDInitEye	= pInfoChar->m_wGrpIDEye;
	pInfoChar->m_wGrpIDInitEyeColor	= pInfoChar->m_wGrpIDEyeColor;
	pInfoChar->m_wGrpIDInitHairType	= pInfoChar->m_wGrpIDHairType;
	pInfoChar->m_wGrpIDInitHairColor	= pInfoChar->m_wGrpIDHairColor;
	pInfoChar->m_wGrpIDInitSP	= pInfoChar->m_wGrpIDSP;
	dwCharID = m_pLibInfoChar->Add(pInfoChar);
	pInfoAccount->m_adwCharID.push_back(dwCharID);

Exit:
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
	pInfoAccount	= pLibInfoAccount->GetPtr(Packet.m_dwAccountID);
	if (pInfoAccount == NULL) {
		goto Exit;
	}

	// m_adwCharID から該当 CharID を検索して削除
	nCount = (int)pInfoAccount->m_adwCharID.size();
	for (i = 0; i < nCount; i++) {
		if (pInfoAccount->m_adwCharID[i] == Packet.m_dwCharID) {
			pInfoAccount->m_adwCharID.erase(pInfoAccount->m_adwCharID.begin() + i);
			break;
		}
	}

	m_pLibInfoChar->Delete(Packet.m_dwCharID);
	nResult = DELETECHARRES_OK;

Exit:
	PacketRES_DELETECHAR.Make(nResult, Packet.m_dwCharID);
	m_pSock->SendTo(dwSessionID, &PacketRES_DELETECHAR);
}
