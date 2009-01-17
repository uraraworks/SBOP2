/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:MainFrameRecvProcADMIN.cpp									 */
/* 内容			:サーバーメインフレーム(管理者系受信処理) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/03/18													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoMapBase.h"
#include "LibInfoMapObject.h"
#include "LibInfoMapObjectData.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapShadow.h"
#include "LibInfoAccount.h"
#include "LibInfoItem.h"
#include "LibInfoItemWeapon.h"
#include "LibInfoMotion.h"
#include "LibInfoMotionType.h"
#include "LibInfoEfcBalloon.h"
#include "LibInfoMapEvent.h"
#include "LibInfoSystem.h"
#include "LibInfoTalkEvent.h"
#include "InfoAccount.h"
#include "InfoCharSvr.h"
#include "LibInfoCharSvr.h"
#include "TextOutput.h"
#include "MgrData.h"
#include "MainFrame.h"


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN										 */
/* 内容		:受信処理(管理者系)												 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	PCInfoAccount pInfoAccount;

	pInfoAccount = m_pLibInfoAccount->GetPtrSessionID (dwSessionID);
	if (pInfoAccount == NULL) {
		return;
	}
	if (pInfoAccount->m_nAdminLevel == ADMINLEVEL_NONE) {
		m_pLog->Write ("■ 権限無しからの要求 dwSessionID:[%d] byCmdSub:[%d]", dwSessionID, byCmdSub);
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}

	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_ADMIN_CHARINFO:				RecvProcADMIN_CHARINFO				(pData, dwSessionID);	break;	/* キャラ情報通知 */
	case SBOCOMMANDID_SUB_ADMIN_DELETECHARINFO:			RecvProcADMIN_DELETECHARINFO		(pData, dwSessionID);	break;	/* キャラ情報削除 */
	case SBOCOMMANDID_SUB_ADMIN_MAP_RENEWMAPOBJECT:		RecvProcADMIN_MAP_RENEWMAPOBJECT	(pData, dwSessionID);	break;	/* マップオブジェクト更新 */
	case SBOCOMMANDID_SUB_ADMIN_MAP_RENEWOBJECTDATA:	RecvProcADMIN_MAP_RENEWOBJECTDATA	(pData, dwSessionID);	break;	/* マップオブジェクト配置データ更新 */
	case SBOCOMMANDID_SUB_ADMIN_MAP_DELETEOBJECTDATA:	RecvProcADMIN_MAP_DELETEOBJECTDATA	(pData, dwSessionID);	break;	/* マップオブジェクト配置データ削除 */
	case SBOCOMMANDID_SUB_ADMIN_RENEWMAPPARTS:			RecvProcADMIN_RENEWMAPPARTS			(pData, dwSessionID);	break;	/* マップパーツ更新 */
	case SBOCOMMANDID_SUB_ADMIN_MAP_SETPARTS:			RecvProcADMIN_MAP_SETPARTS			(pData, dwSessionID);	break;	/* マップパーツ配置 */
	case SBOCOMMANDID_SUB_ADMIN_MAP_RENEWMAPSIZE:		RecvProcADMIN_MAP_RENEWMAPSIZE		(pData, dwSessionID);	break;	/* マップサイズ更新 */
	case SBOCOMMANDID_SUB_ADMIN_MAP_DELETEPARTS:		RecvProcADMIN_MAP_DELETEPARTS		(pData, dwSessionID);	break;	/* マップパーツ削除 */
	case SBOCOMMANDID_SUB_ADMIN_MAP_COPYPARTS:			RecvProcADMIN_MAP_COPYPARTS			(pData, dwSessionID);	break;	/* マップパーツコピー */
	case SBOCOMMANDID_SUB_ADMIN_MAP_SETMAPNAME:			RecvProcADMIN_MAP_SETMAPNAME		(pData, dwSessionID);	break;	/* マップ名変更 */
	case SBOCOMMANDID_SUB_ADMIN_RENEWMAPSHADOW:			RecvProcADMIN_RENEWMAPSHADOW		(pData, dwSessionID);	break;	/* マップ影更新 */
	case SBOCOMMANDID_SUB_ADMIN_MAP_SETMAPSHADOW:		RecvProcADMIN_MAP_SETMAPSHADOW		(pData, dwSessionID);	break;	/* マップ影配置 */
	case SBOCOMMANDID_SUB_ADMIN_MAP_DELETEMAPSHADOW:	RecvProcADMIN_MAP_DELETEMAPSHADOW	(pData, dwSessionID);	break;	/* マップ影削除 */
	case SBOCOMMANDID_SUB_ADMIN_MAP_RENEWEVENT:			RecvProcADMIN_MAP_RENEWEVENT		(pData, dwSessionID);	break;	/* マップイベント情報更新 */
	case SBOCOMMANDID_SUB_ADMIN_MAP_DELETEEVENT:		RecvProcADMIN_MAP_DELETEEVENT		(pData, dwSessionID);	break;	/* マップイベント情報削除 */
	case SBOCOMMANDID_SUB_ADMIN_MAP_ADD:				RecvProcADMIN_MAP_ADD				(pData, dwSessionID);	break;	/* マップ追加 */
	case SBOCOMMANDID_SUB_ADMIN_REQ_ADMINLEVEL:			RecvProcADMIN_REQ_ADMINILEVEL		(pData, dwSessionID);	break;	/* 管理者権限レベル要求 */
	case SBOCOMMANDID_SUB_ADMIN_RENEWADMINLEVEL:		RecvProcADMIN_RENEWADMINILEVEL		(pData, dwSessionID);	break;	/* 管理者権限レベル更新 */
	case SBOCOMMANDID_SUB_ADMIN_SERVER_SAVEINFO:		RecvProcADMIN_SERVER_SAVEINFO		(pData, dwSessionID);	break;	/* サーバー情報保存 */
	case SBOCOMMANDID_SUB_ADMIN_ITEM_ADD:				RecvProcADMIN_ITEM_ADD				(pData, dwSessionID);	break;	/* アイテム情報追加 */
	case SBOCOMMANDID_SUB_ADMIN_ITEM_COPY:				RecvProcADMIN_ITEM_COPY				(pData, dwSessionID);	break;	/* アイテム情報コピー */
	case SBOCOMMANDID_SUB_ADMIN_ITEM_DELETE:			RecvProcADMIN_ITEM_DELETE			(pData, dwSessionID);	break;	/* アイテム情報削除 */
	case SBOCOMMANDID_SUB_ADMIN_ITEMTYPE_ADD:			RecvProcADMIN_ITEMTYPE_ADD			(pData, dwSessionID);	break;	/* アイテム種別情報追加 */
	case SBOCOMMANDID_SUB_ADMIN_ITEMTYPE_COPY:			RecvProcADMIN_ITEMTYPE_COPY			(pData, dwSessionID);	break;	/* アイテム種別情報コピー */
	case SBOCOMMANDID_SUB_ADMIN_ITEMTYPE_DELETE:		RecvProcADMIN_ITEMTYPE_DELETE		(pData, dwSessionID);	break;	/* アイテム種別情報削除 */
	case SBOCOMMANDID_SUB_ADMIN_ITEMWEAPON_ADD:			RecvProcADMIN_ITEMWEAPON_ADD		(pData, dwSessionID);	break;	/* 武器情報追加 */
	case SBOCOMMANDID_SUB_ADMIN_ITEMWEAPON_RENEW:		RecvProcADMIN_ITEMWEAPON_RENEW		(pData, dwSessionID);	break;	/* 武器情報更新 */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_ADDNPC:			RecvProcADMIN_CHAR_ADDNPC			(pData, dwSessionID);	break;	/* NPCの追加 */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_MODIFYITEM:		RecvProcADMIN_CHAR_MODIFYITEM		(pData, dwSessionID);	break;	/* 所持アイテムの変更 */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWMOTION:		RecvProcADMIN_CHAR_RENEWMOTION		(pData, dwSessionID);	break;	/* キャラモーション情報の更新 */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_ADDMOTIONTYPE:		RecvProcADMIN_CHAR_ADDMOTIONTYPE	(pData, dwSessionID);	break;	/* キャラモーション情報種別の追加 */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWMOTIONTYPE:	RecvProcADMIN_CHAR_RENEWMOTIONTYPE	(pData, dwSessionID);	break;	/* キャラモーション情報種別の更新 */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWSTATUS:		RecvProcADMIN_CHAR_RENEWSTATUS		(pData, dwSessionID);	break;	/* ステータス情報更新 */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_STATUS:		RecvProcADMIN_CHAR_REQ_STATUS		(pData, dwSessionID);	break;	/* ステータス情報要求 */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_ONLINE:		RecvProcADMIN_CHAR_REQ_ONLINE		(pData, dwSessionID);	break;	/* オンライン中キャラ一覧要求 */
	case SBOCOMMANDID_SUB_ADMIN_EFC_RENEWBALLOON:		RecvProcADMIN_EFC_RENEWBALLOON		(pData, dwSessionID);	break;	/* 噴出し情報の更新 */
	case SBOCOMMANDID_SUB_ADMIN_EFC_RENEWEFFECT:		RecvProcADMIN_EFC_RENEWEFFECT		(pData, dwSessionID);	break;	/* エフェクト情報の更新 */
	case SBOCOMMANDID_SUB_ADMIN_REQ_PLAYSOUND:			RecvProcADMIN_REQ_PLAYSOUND			(pData, dwSessionID);	break;	/* 効果音の再生要求 */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_ACCOUNT:		RecvProcADMIN_CHAR_REQ_ACCOUNT		(pData, dwSessionID);	break;	/* アカウント情報要求 */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_RENEW_ACCOUNT:		RecvProcADMIN_CHAR_RENEW_ACCOUNT	(pData, dwSessionID);	break;	/* アカウント情報更新 */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_RENEW_TALKEVENT:	RecvProcADMIN_CHAR_RENEW_TALKEVENT	(pData, dwSessionID);	break;	/* 会話イベント情報更新 */
	case SBOCOMMANDID_SUB_ADMIN_RENEW_CLIENTVERSION:	RecvProcADMIN_RENEW_CLIENTVERSION	(pData, dwSessionID);	break;	/* クライアントバージョン更新 */
	case SBOCOMMANDID_SUB_ADMIN_SYSTEM_REQ_INFO:		RecvProcADMIN_SYSTEM_REQ_INFO		(pData, dwSessionID);	break;	/* システム情報要求 */
	case SBOCOMMANDID_SUB_ADMIN_SYSTEM_RENEWINFO:		RecvProcADMIN_SYSTEM_RENEWINFO		(pData, dwSessionID);	break;	/* システム情報の更新 */
	case SBOCOMMANDID_SUB_ADMIN_SKILL_RENEWSKILL:		RecvProcADMIN_SKILL_RENEWSKILL		(pData, dwSessionID);	break;	/* スキル情報更新 */
	case SBOCOMMANDID_SUB_ADMIN_ACCOUNT_REQ_ADD:		RecvProcADMIN_ACCOUNT_REQ_ADD		(pData, dwSessionID);	break;	/* アカウントの追加要求 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_CHARINFO								 */
/* 内容		:受信処理(キャラ情報通知)										 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_CHARINFO(PBYTE pData, DWORD dwSessionID)
{
	DWORD dwMapIDBack;
	POINT ptPosBack;
	PCInfoCharSvr pInfoChar, pInfoCharTmp;
	CPacketADMIN_CHARINFO Packet;
	CPacketCHAR_MOVEPOS PacketCHAR_MOVEPOS;
	CPacketCHAR_CHARINFO PacketCHAR_CHARINFO;
	CPacketCHAR_STATE PacketCHAR_STATE;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtr (Packet.m_pInfoChar->m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	/* 移動種別が変わった？ */
	if (Packet.m_pInfoChar->m_nMoveType != pInfoChar->m_nMoveType) {
		pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetNew (Packet.m_pInfoChar->m_nMoveType);
		pInfoCharTmp->CopyAll (pInfoChar);
		pInfoChar = pInfoCharTmp;
		m_pLibInfoChar->SetPtr (Packet.m_pInfoChar->m_dwCharID, pInfoChar);
	}
	Packet.m_pInfoChar->m_dwSessionID		= pInfoChar->m_dwSessionID;
	Packet.m_pInfoChar->m_dwAccountID		= pInfoChar->m_dwAccountID;
	Packet.m_pInfoChar->m_dwLastTimeSpeak	= pInfoChar->m_dwLastTimeSpeak;
	Packet.m_pInfoChar->m_dwTailCharID		= pInfoChar->m_dwTailCharID;
	Packet.m_pInfoChar->m_dwFrontCharID		= pInfoChar->m_dwFrontCharID;
	Packet.m_pInfoChar->m_nMoveState		= pInfoChar->m_nMoveState;

	if (pInfoChar->m_dwMapID != Packet.m_pInfoChar->m_dwMapID) {
		/* 周りのキャラに通知 */
		PacketCHAR_STATE.Make (pInfoChar->m_dwCharID, CHARMOVESTATE_DELETE);
		SendToScreenChar (pInfoChar, &PacketCHAR_STATE);
	}

	dwMapIDBack = pInfoChar->m_dwMapID;
	ptPosBack.x = pInfoChar->m_nMapX;
	ptPosBack.y = pInfoChar->m_nMapY;

	pInfoChar->m_bChgInfo		= TRUE;
	pInfoChar->m_bChgScreenPos	= Packet.m_bChgScreenPos;

	/* 座標が変わった？ */
	if (!((Packet.m_pInfoChar->m_dwMapID == dwMapIDBack) &&
		(Packet.m_pInfoChar->m_nMapX == ptPosBack.x) &&
		(Packet.m_pInfoChar->m_nMapY == ptPosBack.y))) {
		PacketCHAR_MOVEPOS.Make (
				Packet.m_pInfoChar->m_dwMapID,
				Packet.m_pInfoChar->m_dwCharID,
				Packet.m_pInfoChar->m_nDirection,
				Packet.m_pInfoChar->m_nMapX,
				Packet.m_pInfoChar->m_nMapY,
				FALSE);
		/* 移動前の座標の周りにいるキャラへ通知 */
		SendToScreenChar (pInfoChar, &PacketCHAR_MOVEPOS);

		pInfoChar->m_bChgScreenPos = TRUE;
	}

	pInfoChar->Copy (Packet.m_pInfoChar);
	pInfoChar->m_bChgSpeak = FALSE;

	if (pInfoChar->m_bChgScreenPos) {
		pInfoChar->m_bChgPosRenew = TRUE;
	}
	/* マップ移動した？ */
	if (Packet.m_pInfoChar->m_dwMapID != dwMapIDBack) {
		pInfoChar->m_bChgMap = TRUE;
		pInfoChar->m_bChgPosRenew = FALSE;
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_DELETECHARINFO						 */
/* 内容		:受信処理(キャラ情報削除)										 */
/* 日付		:2008/11/28														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_DELETECHARINFO(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar;
	CPacketADMIN_PARA2 Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtr (Packet.m_dwPara1);
	if (pInfoChar == NULL) {
		return;
	}
	if (pInfoChar->IsNPC () == FALSE) {
		return;
	}
	pInfoChar->m_bDelete = TRUE;
	pInfoChar->SetMoveState (CHARMOVESTATE_DELETE);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_MAP_RENEWMAPOBJECT					 */
/* 内容		:受信処理(マップオブジェクト更新)								 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_MAP_RENEWMAPOBJECT(PBYTE pData, DWORD dwSessionID)
{
	DWORD dwObjectID;
	PCInfoMapObject pInfo;
	CPacketADMIN_MAP_RENEWMAPOBJECT Packet;
	CPacketMAP_MAPOBJECT PacketMAP_MAPOBJECT;

	Packet.Set (pData);

	pInfo = NULL;
	if (Packet.m_pInfoMapObject->m_dwObjectID != 0) {
		pInfo = (PCInfoMapObject)m_pLibInfoMapObject->GetPtr (Packet.m_pInfoMapObject->m_dwObjectID);
	}
	if (pInfo == NULL) {
		pInfo = (PCInfoMapObject)m_pLibInfoMapObject->GetNew ();
		m_pLibInfoMapObject->Add (pInfo);
	}
	dwObjectID = pInfo->m_dwObjectID;
	pInfo->Copy (Packet.m_pInfoMapObject);
	pInfo->m_dwObjectID = dwObjectID;

	m_pLibInfoMap->RenewHitTmp ();

	/* 更新されたマップオブジェクト情報を全クライアントへ通知 */
	PacketMAP_MAPOBJECT.Make (pInfo);
	m_pSock->SendTo (0, &PacketMAP_MAPOBJECT);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_MAP_RENEWOBJECTDATA					 */
/* 内容		:受信処理(マップオブジェクト配置データ更新)						 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_MAP_RENEWOBJECTDATA(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapBase pInfoMap;
	PCInfoMapObjectData pObjectData;
	PCLibInfoMapObjectData pLibInfo;
	CPacketADMIN_MAP_RENEWOBJECTDATA Packet;
	CPacketMAP_MAPOBJECTDATA PacketMAP_MAPOBJECTDATA;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}
	pLibInfo = pInfoMap->m_pLibInfoMapObjectData;
	if (pLibInfo == NULL) {
		return;
	}

	/* 追加？ */
	if (Packet.m_pInfo->m_dwDataID == 0) {
		pObjectData = (PCInfoMapObjectData)pLibInfo->GetNew ();
		pObjectData->Copy (Packet.m_pInfo);
		pLibInfo->Add (pObjectData);

	/* 更新 */
	} else {
		pObjectData = pLibInfo->Renew (Packet.m_pInfo->m_dwDataID, Packet.m_pInfo);
		if (pObjectData == NULL) {
			return;
		}
	}

	m_pLibInfoMap->RenewHitTmp ();

	PacketMAP_MAPOBJECTDATA.Make (Packet.m_dwMapID, pObjectData);
	SendToMapChar (Packet.m_dwMapID, &PacketMAP_MAPOBJECTDATA);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_MAP_DELETEOBJECTDATA					 */
/* 内容		:受信処理(マップオブジェクト配置データ削除)						 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_MAP_DELETEOBJECTDATA(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapBase pInfoMap;
	PCLibInfoMapObjectData pLibInfo;
	CPacketADMIN_PARA2 Packet;
	CPacketMAP_PARA1 PacketMAP_PARA1;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwPara1);
	if (pInfoMap == NULL) {
		return;
	}
	pLibInfo = pInfoMap->m_pLibInfoMapObjectData;
	if (pLibInfo == NULL) {
		return;
	}
	pLibInfo->Delete (Packet.m_dwPara2);

	m_pLibInfoMap->RenewHitTmp ();

	PacketMAP_PARA1.Make (SBOCOMMANDID_SUB_MAP_DELETEMAPOBJECTDATA, Packet.m_dwPara1, Packet.m_dwPara2);
	SendToMapChar (Packet.m_dwPara1, &PacketMAP_PARA1);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_RENEWMAPPARTS						 */
/* 内容		:受信処理(マップパーツ更新)										 */
/* 日付		:2007/04/29														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_RENEWMAPPARTS(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapParts pInfoMapParts;
	CPacketADMIN_RENEWMAPPARTS Packet;
	CPacketMAP_MAPPARTS PacketMAP_MAPPARTS;

	Packet.Set (pData);

	pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (Packet.m_pInfoMapParts->m_dwPartsID);
	if (pInfoMapParts == NULL) {
		pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetNew ();
		m_pLibInfoMapParts->Add (pInfoMapParts);
	}
	pInfoMapParts->Copy (Packet.m_pInfoMapParts);

	/* 更新されたマップパーツ情報を全クライアントへ通知 */
	PacketMAP_MAPPARTS.Make (pInfoMapParts);
	m_pSock->SendTo (0, &PacketMAP_MAPPARTS);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_MAP_SETPARTS							 */
/* 内容		:受信処理(マップパーツ配置)										 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_MAP_SETPARTS(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapBase pInfoMap;
	CPacketADMIN_MAP_SETPARTS Packet;
	CPacketMAP_SETPARTS PacketMAP_SETPARTS;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}
	if (Packet.m_bPile) {
		pInfoMap->SetPartsPile (Packet.m_ptPos.x, Packet.m_ptPos.y, Packet.m_dwPartsID);
	} else {
		pInfoMap->SetParts (Packet.m_ptPos.x, Packet.m_ptPos.y, Packet.m_dwPartsID);
	}

	PacketMAP_SETPARTS.Make (Packet.m_dwMapID, Packet.m_ptPos.x, Packet.m_ptPos.y, Packet.m_dwPartsID, Packet.m_bPile);
	SendToMapChar (Packet.m_dwMapID, &PacketMAP_SETPARTS);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_MAP_RENEWMAPSIZE						 */
/* 内容		:受信処理(マップサイズ更新)										 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_MAP_RENEWMAPSIZE(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapBase pInfoMap;
	CPacketADMIN_MAP_RENEWMAPSIZE Packet;
	CPacketMAP_RENEWMAPSIZE PacketMAP_RENEWMAPSIZE;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}
	pInfoMap->		RenewSize (Packet.m_nDirection, Packet.m_nSize);
	m_pLibInfoChar->RenewSize (Packet.m_dwMapID, Packet.m_nDirection, Packet.m_nSize);
	m_pLibInfoItem->RenewSize (Packet.m_dwMapID, Packet.m_nDirection, Packet.m_nSize);

	PacketMAP_RENEWMAPSIZE.Make (Packet.m_dwMapID, Packet.m_nDirection, Packet.m_nSize);
	SendToMapChar (Packet.m_dwMapID, &PacketMAP_RENEWMAPSIZE);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_MAP_DELETEPARTS						 */
/* 内容		:受信処理(マップパーツ削除)										 */
/* 日付		:2007/05/04														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_MAP_DELETEPARTS(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapParts pInfoMapParts;
	CPacketADMIN_MAP_DELETEPARTS Packet;
	CPacketMAP_DELETEPARTS PacketMAP_DELETEPARTS;

	Packet.Set (pData);

	pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (Packet.m_dwPartsID);
	if (pInfoMapParts == NULL) {
		return;
	}

	/* マップ内で使用している部分をクリアする */
	m_pLibInfoMap->DeleteParts (Packet.m_dwPartsID);
	/* パーツを削除する */
	m_pLibInfoMapParts->Delete (Packet.m_dwPartsID);
	/* 全クライアントへ通知 */
	PacketMAP_DELETEPARTS.Make (Packet.m_dwPartsID);
	m_pSock->SendTo (0, &PacketMAP_DELETEPARTS);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_MAP_COPYPARTS						 */
/* 内容		:受信処理(マップパーツコピー)									 */
/* 日付		:2007/06/14														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_MAP_COPYPARTS(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapParts pInfoMapPartsSrc, pInfoMapPartsTmp;
	CPacketADMIN_MAP_COPYPARTS Packet;
	CPacketMAP_MAPPARTS PacketMAP_MAPPARTS;

	Packet.Set (pData);

	pInfoMapPartsSrc = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (Packet.m_dwSrcPartsID);
	if (pInfoMapPartsSrc == NULL) {
		return;
	}
	pInfoMapPartsTmp = (PCInfoMapParts)m_pLibInfoMapParts->GetNew ();
	pInfoMapPartsTmp->Copy (pInfoMapPartsSrc);
	pInfoMapPartsTmp->m_dwPartsID = 0;
	pInfoMapPartsTmp->m_ptViewPos = Packet.m_ptViewPos;
	m_pLibInfoMapParts->Add (pInfoMapPartsTmp);

	/* 更新されたマップパーツ情報を全クライアントへ通知 */
	PacketMAP_MAPPARTS.Make (pInfoMapPartsTmp);
	m_pSock->SendTo (0, &PacketMAP_MAPPARTS);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_MAP_SETMAPNAME						 */
/* 内容		:受信処理(マップ名変更)											 */
/* 日付		:2008/05/24														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_MAP_SETMAPNAME(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapBase pInfoMap;
	CPacketADMIN_MAP_SETMAPNAME Packet;
	CPacketMAP_MAPNAME PacketMAP_MAPNAME;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}
	pInfoMap->m_strMapName		= Packet.m_strMapName;
	pInfoMap->m_dwBGMID			= Packet.m_dwBGMID;			/* BGMID */
	pInfoMap->m_dwWeatherType	= Packet.m_dwWeatherType;	/* 天気種別 */
	pInfoMap->m_bEnableBattle	= Packet.m_bEnableBattle;	/* 戦闘許可 */
	pInfoMap->m_bRecovery		= Packet.m_bRecovery;		/* 気絶後回復する */
	pInfoMap->m_byLevel			= Packet.m_byLevel;			/* 明るさレベル */

	PacketMAP_MAPNAME.Make (
			pInfoMap->m_dwMapID,
			pInfoMap->m_dwBGMID,
			pInfoMap->m_dwWeatherType,
			pInfoMap->m_bEnableBattle,
			pInfoMap->m_bRecovery,
			pInfoMap->m_byLevel,
			pInfoMap->m_strMapName);
	SendToMapChar (pInfoMap->m_dwMapID, &PacketMAP_MAPNAME);
	UpdateServerInfo (FALSE);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_RENEWMAPSHADOW						 */
/* 内容		:受信処理(マップ影更新)											 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_RENEWMAPSHADOW(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapShadow pInfoMapShadow;
	CPacketADMIN_RENEWMAPSHADOW Packet;
	CPacketMAP_MAPSHADOW PacketMAP_MAPSHADOW;

	Packet.Set (pData);

	pInfoMapShadow = (PCInfoMapShadow)m_pLibInfoMapShadow->GetPtr (Packet.m_pInfoMapShadow->m_dwShadowID);
	if (pInfoMapShadow == NULL) {
		pInfoMapShadow = (PCInfoMapShadow)m_pLibInfoMapShadow->GetNew ();
		m_pLibInfoMapShadow->Add (pInfoMapShadow);
	}
	pInfoMapShadow->Copy (Packet.m_pInfoMapShadow);

	/* 更新されたマップパーツ情報を全クライアントへ通知 */
	PacketMAP_MAPSHADOW.Make (pInfoMapShadow);
	m_pSock->SendTo (0, &PacketMAP_MAPSHADOW);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_MAP_SETMAPSHADOW						 */
/* 内容		:受信処理(マップ影配置)											 */
/* 日付		:2007/06/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_MAP_SETMAPSHADOW(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapBase pInfoMap;
	CPacketADMIN_MAP_SETMAPSHADOW Packet;
	CPacketMAP_SETMAPSHADOW PacketMAP_SETMAPSHADOW;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}
	pInfoMap->SetShadow (Packet.m_ptPos.x, Packet.m_ptPos.y, Packet.m_dwShadowID);

	PacketMAP_SETMAPSHADOW.Make (Packet.m_dwMapID, Packet.m_ptPos.x, Packet.m_ptPos.y, Packet.m_dwShadowID);
	SendToMapChar (Packet.m_dwMapID, &PacketMAP_SETMAPSHADOW);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_MAP_DELETEMAPSHADOW					 */
/* 内容		:受信処理(マップ影削除)											 */
/* 日付		:2007/06/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_MAP_DELETEMAPSHADOW(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapShadow pInfoMapShadow;
	CPacketADMIN_MAP_DELETEMAPSHADOW Packet;
	CPacketMAP_DELETEMAPSHADOW PacketMAP_DELETEMAPSHADOW;

	Packet.Set (pData);

	pInfoMapShadow = (PCInfoMapShadow)m_pLibInfoMapShadow->GetPtr (Packet.m_dwShadowID);
	if (pInfoMapShadow == NULL) {
		return;
	}

	/* マップ内で使用している部分をクリアする */
	m_pLibInfoMap->DeleteShadow (Packet.m_dwShadowID);
	/* マップ影を削除する */
	m_pLibInfoMapShadow->Delete (Packet.m_dwShadowID);
	/* 全クライアントへ通知 */
	PacketMAP_DELETEMAPSHADOW.Make (Packet.m_dwShadowID);
	m_pSock->SendTo (0, &PacketMAP_DELETEMAPSHADOW);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_MAP_RENEWEVENT						 */
/* 内容		:受信処理(マップイベント情報更新)								 */
/* 日付		:2008/06/25														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_MAP_RENEWEVENT(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapBase pInfoMap;
	PCInfoMapEventBase pInfoMapEvent;
	PCLibInfoMapEvent pLibInfo;
	CPacketADMIN_MAP_EVENT Packet;
	CPacketMAP_MAPEVENT PacketMAP_MAPEVENT;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}
	pLibInfo = pInfoMap->m_pLibInfoMapEvent;
	if (pLibInfo == NULL) {
		return;
	}

	/* 追加？ */
	if (Packet.m_pInfo->m_dwMapEventID == 0) {
		pInfoMapEvent = (PCInfoMapEventBase)pLibInfo->GetNew (Packet.m_pInfo->m_nType);
		pInfoMapEvent->Copy (Packet.m_pInfo);
		pLibInfo->Add (pInfoMapEvent);

	/* 更新 */
	} else {
		pInfoMapEvent = pLibInfo->Renew (Packet.m_pInfo->m_dwMapEventID, Packet.m_pInfo);
		if (pInfoMapEvent == NULL) {
			return;
		}
	}
	pInfoMap->RenewMapEvent ();

	PacketMAP_MAPEVENT.Make (Packet.m_dwMapID, pInfoMapEvent);
	SendToMapChar (Packet.m_dwMapID, &PacketMAP_MAPEVENT);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_MAP_DELETEEVENT						 */
/* 内容		:受信処理(マップイベント情報削除)								 */
/* 日付		:2008/07/22														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_MAP_DELETEEVENT(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapBase pInfoMap;
	PCLibInfoMapEvent pLibInfo;
	CPacketADMIN_PARA2 Packet;
	CPacketMAP_PARA1 PacketMAP_PARA1;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwPara1);
	if (pInfoMap == NULL) {
		return;
	}
	pLibInfo = pInfoMap->m_pLibInfoMapEvent;
	if (pLibInfo == NULL) {
		return;
	}
	pLibInfo->Delete (Packet.m_dwPara2);
	pInfoMap->RenewMapEvent ();

	PacketMAP_PARA1.Make (SBOCOMMANDID_SUB_MAP_DELETEEVENT, Packet.m_dwPara1, Packet.m_dwPara2);
	SendToMapChar (Packet.m_dwPara1, &PacketMAP_PARA1);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_MAP_ADD								 */
/* 内容		:受信処理(マップ追加)											 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_MAP_ADD(PBYTE pData, DWORD dwSessionID)
{
	DWORD dwMapID;
	PCInfoMapBase pInfoMap, pInfoMapTmp;
	CPacketADMIN_PARA2 Packet;
	CPacketMAP_SYSTEMMSG PacketMAP_SYSTEMMSG;
	CmyString strTmp;

	Packet.Set (pData);

	pInfoMapTmp = NULL;
	/* 既存マップからのコピー？ */
	if (Packet.m_dwPara1 != 0) {
		pInfoMapTmp = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwPara1);
		if (pInfoMapTmp == NULL) {
			return;
		}
	}

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetNew ();
	pInfoMap->Init (DRAW_PARTS_X, DRAW_PARTS_Y, 1);
	m_pLibInfoMap->Add (pInfoMap);
	m_pLibInfoMap->SetMapObject (m_pLibInfoMapObject);

	if (pInfoMapTmp) {
		dwMapID = pInfoMap->m_dwMapID;
		pInfoMap->Copy (pInfoMapTmp);
		pInfoMap->m_dwMapID = dwMapID;
	}

	strTmp.Format ("SYSTEM:マップID[%d]が追加されました", pInfoMap->m_dwMapID);
	PacketMAP_SYSTEMMSG.Make (strTmp);
	m_pSock->SendTo (0, &PacketMAP_SYSTEMMSG);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_REQ_ADMINILEVEL						 */
/* 内容		:受信処理(管理者権限レベル要求)									 */
/* 日付		:2007/07/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_REQ_ADMINILEVEL(PBYTE pData, DWORD dwSessionID)
{
	PCInfoAccount pInfoAccount;
	CPacketADMIN_REQ_ADMINLEVEL Packet;
	CPacketADMIN_RES_ADMINLEVEL PacketADMIN_RES_ADMINLEVEL;

	Packet.Set (pData);

	pInfoAccount = m_pLibInfoAccount->GetPtr (Packet.m_dwAccountID);
	if (pInfoAccount == NULL) {
		return;
	}
	PacketADMIN_RES_ADMINLEVEL.Make (pInfoAccount->m_dwAccountID, pInfoAccount->m_nAdminLevel);
	m_pSock->SendTo (dwSessionID, &PacketADMIN_RES_ADMINLEVEL);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_RENEWADMINILEVEL						 */
/* 内容		:受信処理(管理者権限レベル更新)									 */
/* 日付		:2007/07/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_RENEWADMINILEVEL(PBYTE pData, DWORD dwSessionID)
{
	PCInfoAccount pInfoAccount;
	CPacketADMIN_RENEWADMINLEVEL Packet;

	Packet.Set (pData);

	pInfoAccount = m_pLibInfoAccount->GetPtr (Packet.m_dwAccountID);
	if (pInfoAccount == NULL) {
		return;
	}

	/* 変化しない？ */
	if (pInfoAccount->m_nAdminLevel == Packet.m_nAdminLevel) {
		return;
	}
	pInfoAccount->m_nAdminLevel = Packet.m_nAdminLevel;
	if (pInfoAccount->m_dwSessionID == 0) {
		return;
	}
	Packet.Make (Packet.m_dwAccountID, Packet.m_nAdminLevel);
	m_pSock->SendTo (pInfoAccount->m_dwSessionID, &Packet);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_SERVER_SAVEINFO						 */
/* 内容		:受信処理(サーバー情報保存)										 */
/* 日付		:2007/07/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_SERVER_SAVEINFO(PBYTE pData, DWORD dwSessionID)
{
	CPacketMAP_SYSTEMMSG Packet;
	CmyString strTmp;

	m_pMgrData->Save ();
	strTmp.Format ("サーバー情報を保存しました");
	Packet.Make (strTmp);
	m_pSock->SendTo (0, &Packet);
	UpdateServerInfo (FALSE, TRUE);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_ITEM_ADD								 */
/* 内容		:受信処理(アイテム情報追加)										 */
/* 日付		:2007/08/16														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_ITEM_ADD(PBYTE pData, DWORD dwSessionID)
{
	PCInfoItem pInfoItem;
	PCInfoCharSvr pInfoChar;
	CPacketADMIN_ITEM_ADD Packet;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;
	CPacketCHAR_ITEMINFO PacketCHAR_ITEMINFO;

	Packet.Set (pData);

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetNew ();
	pInfoItem->Copy (Packet.m_pInfoItem);
	m_pLibInfoItem->Add (pInfoItem);

	/* 持たせるキャラが指定されている？ */
	if (pInfoItem->m_dwCharID) {
		pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtr (pInfoItem->m_dwCharID);
		if (pInfoChar) {
			/* アイテムを追加 */
			m_pLibInfoItem->AddItem (pInfoChar->m_dwCharID, pInfoItem->m_dwItemID, &pInfoChar->m_adwItemID);

			PacketCHAR_ITEMINFO.Make (pInfoChar->m_dwCharID, &pInfoChar->m_adwItemID);
			m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketCHAR_ITEMINFO);
		}
	}

	PacketITEM_RES_ITEMINFO.Make (pInfoItem);
	SendToMapChar (Packet.m_pInfoItem->m_dwMapID, &PacketITEM_RES_ITEMINFO);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_ITEM_COPY							 */
/* 内容		:受信処理(アイテム情報コピー)									 */
/* 日付		:2007/09/22														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_ITEM_COPY(PBYTE pData, DWORD dwSessionID)
{
	CPacketADMIN_ITEM_COPY Packet;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;
	PCInfoItem pInfoItem, pInfoItemTmp;

	Packet.Set (pData);

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (Packet.m_dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	pInfoItemTmp = (PCInfoItem)m_pLibInfoItem->GetNew ();
	pInfoItemTmp->Copy (pInfoItem);
	pInfoItemTmp->m_dwItemID		= 0;
	pInfoItemTmp->m_dwCharID		= 0;
	pInfoItemTmp->m_dwMapID			= 0;
	pInfoItemTmp->m_ptPos.x			= 0;
	pInfoItemTmp->m_ptPos.y			= 0;
	pInfoItemTmp->m_ptBackPack.x	= 0;
	pInfoItemTmp->m_ptBackPack.y	= 0;
	m_pLibInfoItem->Add (pInfoItemTmp);

	PacketITEM_RES_ITEMINFO.Make (pInfoItemTmp);
	SendToMapChar (pInfoItemTmp->m_dwMapID, &PacketITEM_RES_ITEMINFO);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_ITEM_DELETE							 */
/* 内容		:受信処理(アイテム情報削除)										 */
/* 日付		:2007/08/19														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_ITEM_DELETE(PBYTE pData, DWORD dwSessionID)
{
	int i, nCount;
	DWORD dwItemID;
	PCInfoItem pInfoItem;
	PCInfoCharSvr pInfoChar;
	CPacketADMIN_ITEM_DELETE Packet;
	CPacketITEM_DELETEITEMINFO PacketITEM_DELETEITEMINFO;
	CPacketCHAR_ITEMINFO PacketCHAR_ITEMINFO;

	Packet.Set (pData);

	nCount = Packet.m_adwItemID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwItemID = Packet.m_adwItemID[i];

		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
		if (pInfoItem == NULL) {
			continue;
		}
		if (pInfoItem->m_dwCharID) {
			pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (pInfoItem->m_dwCharID);
			if (pInfoChar) {
				/* キャラの所持アイテムから削除する */
				pInfoChar->DeleteItem (pInfoItem->m_dwItemID);
				m_pLibInfoItem->DeleteItem (pInfoItem->m_dwItemID, pInfoChar);

				PacketCHAR_ITEMINFO.Make (pInfoChar->m_dwCharID, &pInfoChar->m_adwItemID);
				m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketCHAR_ITEMINFO);
			}
		}

		PacketITEM_DELETEITEMINFO.Make (dwItemID);
		if (pInfoItem->m_dwMapID) {
			SendToMapChar (pInfoItem->m_dwMapID, &PacketITEM_DELETEITEMINFO);
		}
		SendToAdminChar (&PacketITEM_DELETEITEMINFO);

		m_pLibInfoItem->Delete (dwItemID);
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_ITEMTYPE_ADD							 */
/* 内容		:受信処理(アイテム種別情報追加)									 */
/* 日付		:2007/10/03														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_ITEMTYPE_ADD(PBYTE pData, DWORD dwSessionID)
{
	PCInfoItemTypeBase pInfoItem;
	CPacketADMIN_ITEMTYPE_ADD Packet;
	CPacketITEM_RES_ITEMTYPEINFO PacketITEM_RES_ITEMTYPEINFO;

	Packet.Set (pData);

	pInfoItem = (PCInfoItemTypeBase)m_pLibInfoItemType->GetNew (Packet.m_pInfoItem->m_dwItemTypeID);
	pInfoItem->Copy (Packet.m_pInfoItem);
	m_pLibInfoItemType->Add (pInfoItem);

	PacketITEM_RES_ITEMTYPEINFO.Make (pInfoItem);
	m_pSock->SendTo (0, &PacketITEM_RES_ITEMTYPEINFO);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_ITEMTYPE_COPY						 */
/* 内容		:受信処理(アイテム種別情報コピー)								 */
/* 日付		:2007/10/03														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_ITEMTYPE_COPY(PBYTE pData, DWORD dwSessionID)
{
	CPacketADMIN_ITEMTYPE_COPY Packet;
	CPacketITEM_RES_ITEMTYPEINFO PacketITEM_RES_ITEMTYPEINFO;
	PCInfoItemTypeBase pInfoItem, pInfoItemTmp;

	Packet.Set (pData);

	pInfoItem = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (Packet.m_dwTypeID);
	if (pInfoItem == NULL) {
		return;
	}

	pInfoItemTmp = (PCInfoItemTypeBase)m_pLibInfoItemType->GetNew (pInfoItem->m_dwItemTypeID);
	pInfoItemTmp->Copy (pInfoItem);
	pInfoItemTmp->m_dwTypeID = 0;
	m_pLibInfoItemType->Add (pInfoItemTmp);

	PacketITEM_RES_ITEMTYPEINFO.Make (pInfoItemTmp);
	m_pSock->SendTo (0, &PacketITEM_RES_ITEMTYPEINFO);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_ITEMTYPE_DELETE						 */
/* 内容		:受信処理(アイテム種別情報削除)									 */
/* 日付		:2007/10/03														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_ITEMTYPE_DELETE(PBYTE pData, DWORD dwSessionID)
{
	PCInfoItemTypeBase pInfoItem;
	CPacketADMIN_ITEMTYPE_DELETE Packet;
	CPacketITEM_DELETEITEMTYPEINFO PacketITEM_DELETEITEMTYPEINFO;

	Packet.Set (pData);

	pInfoItem = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (Packet.m_dwTypeID);
	if (pInfoItem == NULL) {
		return;
	}

	PacketITEM_DELETEITEMTYPEINFO.Make (Packet.m_dwTypeID);
	m_pSock->SendTo (0, &PacketITEM_DELETEITEMTYPEINFO);

	m_pLibInfoItemType->Delete (Packet.m_dwTypeID);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_ITEMWEAPON_ADD						 */
/* 内容		:受信処理(武器情報追加)											 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_ITEMWEAPON_ADD(PBYTE pData, DWORD dwSessionID)
{
	PCInfoItemWeapon pInfoItemWeapon;
	CPacketADMIN_ITEMWEAPON_ADD Packet;
	CPacketITEM_RES_ITEMWEAPONINFO PacketITEM_RES_ITEMWEAPONINFO;

	Packet.Set (pData);

	pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetNew ();
	pInfoItemWeapon->Copy (Packet.m_pInfo);
	m_pLibInfoItemWeapon->Add (pInfoItemWeapon);

	PacketITEM_RES_ITEMWEAPONINFO.Make (pInfoItemWeapon);
	m_pSock->SendTo (0, &PacketITEM_RES_ITEMWEAPONINFO);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_ITEMWEAPON_RENEW						 */
/* 内容		:受信処理(武器情報更新)											 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_ITEMWEAPON_RENEW(PBYTE pData, DWORD dwSessionID)
{
	CPacketADMIN_ITEMWEAPON_RENEW Packet;
	CPacketITEM_RES_ITEMWEAPONINFO PacketITEM_RES_ITEMWEAPONINFO;
	PCInfoItemWeapon pInfoItemWeapon;

	Packet.Set (pData);

	pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr (Packet.m_pInfo->m_dwWeaponInfoID);
	if (pInfoItemWeapon == NULL) {
		return;
	}
	pInfoItemWeapon->Copy (Packet.m_pInfo);
	PacketITEM_RES_ITEMWEAPONINFO.Make (pInfoItemWeapon);
	m_pSock->SendTo (0, &PacketITEM_RES_ITEMWEAPONINFO);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_CHAR_ADDNPC							 */
/* 内容		:受信処理(NPCの追加)											 */
/* 日付		:2007/09/01														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_CHAR_ADDNPC(PBYTE pData, DWORD dwSessionID)
{
	CPacketADMIN_CHAR_ADDNPC Packet;

	Packet.Set (pData);

	m_pLibInfoChar->AddNPC (Packet.m_pInfoChar);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_CHAR_MODIFYITEM						 */
/* 内容		:受信処理(所持アイテムの変更)									 */
/* 日付		:2007/09/24														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_CHAR_MODIFYITEM(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoItem pInfoItem;
	PCInfoCharSvr pInfoChar;
	CPacketADMIN_CHAR_MODIFYITEM Packet;
	CPacketCHAR_ITEMINFO PacketCHAR_ITEMINFO;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;

	Packet.Set (pData);

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (Packet.m_dwItemID);
	if (pInfoItem == NULL) {
		return;
	}
	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}

	switch (Packet.m_nType) {
	case CHARMODIFYITEMTYPE_ADD:		/* 追加 */
		bResult = pInfoChar->IsItemAdd ();
		if (bResult == FALSE) {
			return;
		}
		/* アイテムを追加 */
		m_pLibInfoItem->AddItem (pInfoChar->m_dwCharID, pInfoItem->m_dwItemID, &pInfoChar->m_adwItemID);
		break;

	case CHARMODIFYITEMTYPE_DELETE:		/* 削除 */
		/* アイテムをキャラから削除 */
		pInfoChar->DeleteItem (pInfoItem->m_dwItemID);
		m_pLibInfoItem->DeleteItem (pInfoItem->m_dwItemID, pInfoChar, TRUE);
		break;
	}
	PacketCHAR_ITEMINFO.Make (pInfoChar->m_dwCharID, &pInfoChar->m_adwItemID);
	m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketCHAR_ITEMINFO);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_ITEMINFO);

	PacketITEM_RES_ITEMINFO.Make (pInfoItem);
	SendToMapChar (pInfoChar->m_dwMapID, &PacketITEM_RES_ITEMINFO);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_CHAR_RENEWMOTION						 */
/* 内容		:受信処理(キャラモーション情報の更新)							 */
/* 日付		:2007/11/23														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_CHAR_RENEWMOTION(PBYTE pData, DWORD dwSessionID)
{
	CPacketADMIN_CHAR_RENEWMOTION Packet;
	CPacketCHAR_MOTION PacketCHAR_MOTION;

	Packet.Set (pData);

	m_pLibInfoMotion->SetList (Packet.m_dwMotionTypeID, Packet.m_dwMotionListID, Packet.m_pInfo);

	PacketCHAR_MOTION.Make (Packet.m_dwMotionTypeID, Packet.m_dwMotionListID, m_pLibInfoMotion);
	m_pSock->SendTo (0, &PacketCHAR_MOTION);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_CHAR_ADDMOTIONTYPE					 */
/* 内容		:受信処理(キャラモーション種別情報の追加)						 */
/* 日付		:2008/06/09														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_CHAR_ADDMOTIONTYPE(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMotionType pInfoMotionType;
	CPacketCHAR_MOTIONTYPE PacketCHAR_MOTIONTYPE;
	CPacketCHAR_MOTION PacketCHAR_MOTION;

	pInfoMotionType = (PCInfoMotionType)m_pLibInfoMotionType->GetNew ();
	m_pLibInfoMotionType->Add (pInfoMotionType);
	m_pLibInfoMotion->AddType (pInfoMotionType->m_dwMotionTypeID);

	PacketCHAR_MOTIONTYPE.Make (pInfoMotionType->m_dwMotionTypeID, m_pLibInfoMotionType);
	m_pSock->SendTo (0, &PacketCHAR_MOTIONTYPE);
	PacketCHAR_MOTION.Make (pInfoMotionType->m_dwMotionTypeID, 0, m_pLibInfoMotion);
	m_pSock->SendTo (0, &PacketCHAR_MOTION);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_CHAR_RENEWMOTIONTYPE					 */
/* 内容		:受信処理(キャラモーション種別情報の更新)						 */
/* 日付		:2008/06/09														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_CHAR_RENEWMOTIONTYPE(PBYTE pData, DWORD dwSessionID)
{
	PBYTE pTmp;
	CPacketADMIN_CHAR_RENEWMOTIONTYPE Packet;
	CPacketCHAR_MOTIONTYPE PacketCHAR_MOTIONTYPE;

	Packet.Set (pData);

	pTmp = Packet.m_pInfo->GetSendData (Packet.m_dwMotionTypeID);
	m_pLibInfoMotionType->SetSendData (pTmp);
	SAFE_DELETE_ARRAY (pTmp);

	PacketCHAR_MOTIONTYPE.Make (Packet.m_dwMotionTypeID, m_pLibInfoMotionType);
	m_pSock->SendTo (0, &PacketCHAR_MOTIONTYPE);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_CHAR_RENEWSTATUS						 */
/* 内容		:受信処理(ステータス情報更新)									 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_CHAR_RENEWSTATUS(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_STATUS Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}
	Packet.SetParam (pInfoChar);
	if (Packet.m_dwLightTime != 0) {
		pInfoChar->m_dwLightTime = timeGetTime () + Packet.m_dwLightTime;
	}

	pInfoChar->m_bChgStatus = TRUE;
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_CHAR_REQ_STATUS						 */
/* 内容		:受信処理(ステータス情報要求)									 */
/* 日付		:2008/07/22														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_CHAR_REQ_STATUS(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar;
	CPacketADMIN_PARA2 Packet;
	CPacketCHAR_STATUS PacketCHAR_STATUS;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtr (Packet.m_dwPara1);
	/* 知らないキャラ？ */
	if (pInfoChar == NULL) {
		return;
	}
	PacketCHAR_STATUS.Make (pInfoChar);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_STATUS);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_CHAR_REQ_ONLINE						 */
/* 内容		:受信処理(オンライン中キャラ一覧要求)							 */
/* 日付		:2008/12/01														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_CHAR_REQ_ONLINE(PBYTE pData, DWORD dwSessionID)
{
	int i, nCount;
	PCInfoCharSvr pInfoCharTmp, pInfoChar;
	CPacketADMIN_PARA2 Packet;
	CPacketADMIN_CHAR_RES_ONLINE PacketADMIN_CHAR_RES_ONLINE;
	CLibInfoCharSvr LibInfoCharTmp;

	Packet.Set (pData);

	LibInfoCharTmp.Create (m_pMgrData);
	nCount = m_pLibInfoChar->GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (i);
		if (pInfoChar->IsNPC ()) {
			continue;
		}
		pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetNew (pInfoChar->m_nMoveType);
		pInfoCharTmp->Copy (pInfoChar);
		LibInfoCharTmp.Add (pInfoCharTmp);
	}

	PacketADMIN_CHAR_RES_ONLINE.Make (&LibInfoCharTmp);
	m_pSock->SendTo (dwSessionID, &PacketADMIN_CHAR_RES_ONLINE);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_EFC_RENEWBALLOON						 */
/* 内容		:受信処理(噴出し情報の更新)										 */
/* 日付		:2007/12/30														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_EFC_RENEWBALLOON(PBYTE pData, DWORD dwSessionID)
{
	CPacketADMIN_EFC_RENEWBALLOON Packet;
	CPacketEFFECT_BALLOONINFO PacketEFFECT_BALLOONINFO;

	Packet.Set (pData);

	m_pLibInfoEfcBalloon->SetList (Packet.m_dwListID, Packet.m_pInfo);

	PacketEFFECT_BALLOONINFO.Make (Packet.m_dwListID, m_pLibInfoEfcBalloon);
	m_pSock->SendTo (0, &PacketEFFECT_BALLOONINFO);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_EFC_RENEWEFFECT						 */
/* 内容		:受信処理(エフェクト情報の更新)									 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_EFC_RENEWEFFECT(PBYTE pData, DWORD dwSessionID)
{
	PCInfoEffect pInfoEffect;
	CPacketADMIN_EFC_RENEWEFFECT Packet;
	CPacketEFFECT_RES_EFFECTINFO PacketEFFECT_RES_EFFECTINFO;

	Packet.Set (pData);

	pInfoEffect = NULL;
	if (Packet.m_dwEffectID) {
		pInfoEffect = (PCInfoEffect)m_pLibInfoEffect->GetPtr (Packet.m_dwEffectID);
	}
	if (pInfoEffect == NULL) {
		pInfoEffect = (PCInfoEffect)m_pLibInfoEffect->GetNew ();
	}
	pInfoEffect->Copy (Packet.m_pInfo);
	if (Packet.m_dwEffectID == 0) {
		m_pLibInfoEffect->Add (pInfoEffect);
	}

	PacketEFFECT_RES_EFFECTINFO.Make (pInfoEffect);
	m_pSock->SendTo (0, &PacketEFFECT_RES_EFFECTINFO);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_REQ_PLAYSOUND						 */
/* 内容		:受信処理(効果音の再生要求)										 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_REQ_PLAYSOUND(PBYTE pData, DWORD dwSessionID)
{
	CPacketADMIN_REQ_PLAYSOUND Packet;
	CPacketADMIN_PLAYSOUND PacketPLAYSOUND;

	Packet.Set (pData);

	PacketPLAYSOUND.Make (Packet.m_dwSoundID);
	m_pSock->SendTo (0, &PacketPLAYSOUND);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_CHAR_REQ_ACCOUNT						 */
/* 内容		:受信処理(アカウント情報要求)									 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_CHAR_REQ_ACCOUNT(PBYTE pData, DWORD dwSessionID)
{
	PCInfoAccount pInfoAccount;
	CPacketADMIN_CHAR_REQ_ACCOUNT Packet;
	CPacketADMIN_CHAR_RES_ACCOUNT PacketADMIN_CHAR_RES_ACCOUNT;

	Packet.Set (pData);

	pInfoAccount = m_pLibInfoAccount->GetPtr (Packet.m_dwAccountID);
	if (pInfoAccount == NULL) {
		return;
	}
	PacketADMIN_CHAR_RES_ACCOUNT.Make (pInfoAccount);
	m_pSock->SendTo (dwSessionID, &PacketADMIN_CHAR_RES_ACCOUNT);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_CHAR_RENEW_ACCOUNT					 */
/* 内容		:受信処理(アカウント情報更新)									 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_CHAR_RENEW_ACCOUNT(PBYTE pData, DWORD dwSessionID)
{
	PCInfoAccount pInfoAccount;
	CPacketADMIN_CHAR_RENEW_ACCOUNT Packet;
	CPacketMAP_SYSTEMMSG PacketMsg;
	CmyString strTmp;

	Packet.Set (pData);
	pInfoAccount = m_pLibInfoAccount->GetPtr (Packet.m_dwAccountID);
	if (pInfoAccount == NULL) {
		return;
	}

	pInfoAccount->m_strAccount	= Packet.m_strAccount;
	pInfoAccount->m_strPassword	= Packet.m_strPassword;

	strTmp.Format ("アカウントを[%s] パスワードを[%s]に変更しました",
			(LPCSTR)pInfoAccount->m_strAccount,
			(LPCSTR)pInfoAccount->m_strPassword);
	PacketMsg.Make (strTmp, RGB (255, 255, 255));
	m_pSock->SendTo (dwSessionID, &PacketMsg);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_CHAR_RENEW_TALKEVENT					 */
/* 内容		:受信処理(会話イベント情報更新)									 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_CHAR_RENEW_TALKEVENT(PBYTE pData, DWORD dwSessionID)
{
	PCInfoTalkEvent pInfo;
	CPacketADMIN_CHAR_RENEW_TALKEVENT Packet;

	Packet.Set (pData);

	if (Packet.m_pInfo->GetPageCount () <= 0) {
		return;
	}

	pInfo = (PCInfoTalkEvent)m_pLibInfoTalkEvent->GetPtr (Packet.m_pInfo->m_dwTalkEventID);
	if (pInfo) {
		m_pLibInfoTalkEvent->Renew (Packet.m_pInfo);
	} else {
		pInfo = (PCInfoTalkEvent)m_pLibInfoTalkEvent->GetNew ();
		pInfo->Copy (Packet.m_pInfo);
		m_pLibInfoTalkEvent->Add (pInfo);
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_RENEW_CLIENTVERSION					 */
/* 内容		:受信処理(クライアントバージョン更新)							 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_RENEW_CLIENTVERSION(PBYTE pData, DWORD dwSessionID)
{
	CPacketADMIN_RENEW_CLIENTVERSION Packet;
	CPacketMAP_SYSTEMMSG PacketMAP_SYSTEMMSG;
	CmyString strTmp;

	Packet.Set (pData);
	m_pMgrData->SetClientVersion ((LPCSTR)Packet.m_strClientVersion);

	strTmp.Format ("SYSTEM:クライアントファイルが[Ver%s]に更新されました", (LPCSTR)Packet.m_strClientVersion);
	PacketMAP_SYSTEMMSG.Make (strTmp);
	m_pSock->SendTo (0, &PacketMAP_SYSTEMMSG);
	strTmp.Format ("SYSTEM:クライアントを終了してランチャーにて更新してください");
	PacketMAP_SYSTEMMSG.Make (strTmp);
	m_pSock->SendTo (0, &PacketMAP_SYSTEMMSG);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_SYSTEM_REQ_INFO						 */
/* 内容		:受信処理(システム情報要求)										 */
/* 日付		:2008/10/03														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_SYSTEM_REQ_INFO(PBYTE pData, DWORD dwSessionID)
{
	PCInfoSystem pInfoSystem;
	CPacketSYSTEM_INFO PacketSYSTEM_INFO;

	pInfoSystem = (PCInfoSystem)m_pLibInfoSystem->GetPtr ();

	PacketSYSTEM_INFO.Make (pInfoSystem);
	m_pSock->SendTo (dwSessionID, &PacketSYSTEM_INFO);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_SYSTEM_RENEWINFO						 */
/* 内容		:受信処理(システム情報の更新)									 */
/* 日付		:2008/10/04														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_SYSTEM_RENEWINFO(PBYTE pData, DWORD dwSessionID)
{
	PCInfoSystem pInfoSystem;
	CPacketADMIN_SYSTEM_RENEWINFO Packet;
	CPacketSYSTEM_INFO PacketSYSTEM_INFO;

	Packet.Set (pData);

	pInfoSystem = (PCInfoSystem)m_pLibInfoSystem->GetPtr ();
	pInfoSystem->Copy (Packet.m_pInfoSystem);

	PacketSYSTEM_INFO.Make (pInfoSystem);
	m_pSock->SendTo (0, &PacketSYSTEM_INFO);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_SKILL_RENEWSKILL						 */
/* 内容		:受信処理(スキル情報更新)										 */
/* 日付		:2008/12/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_SKILL_RENEWSKILL(PBYTE pData, DWORD dwSessionID)
{
	PCInfoSkillBase pInfo;
	CPacketADMIN_SKILL_RENEWSKILL Packet;
	CPacketSKILL_SKILLINFO PacketSKILL_SKILLINFO;

	Packet.Set (pData);

	/* 追加？ */
	if (Packet.m_pInfo->m_dwSkillID == 0) {
		pInfo = (PCInfoSkillBase)m_pLibInfoSkill->GetNew (Packet.m_pInfo->m_nTypeMain, Packet.m_pInfo->m_nTypeSub);
		pInfo->Copy (Packet.m_pInfo);
		m_pLibInfoSkill->Add (pInfo);

	/* 更新 */
	} else {
		pInfo = m_pLibInfoSkill->Renew (Packet.m_pInfo);
		if (pInfo == NULL) {
			return;
		}
	}

	PacketSKILL_SKILLINFO.Make (pInfo);
	m_pSock->SendTo (0, &PacketSKILL_SKILLINFO);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_ACCOUNT_REQ_ADD						 */
/* 内容		:受信処理(アカウントの追加要求)									 */
/* 日付		:2009/01/17														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_ACCOUNT_REQ_ADD(PBYTE pData, DWORD dwSessionID)
{
	PCInfoAccount pInfoAccount;
	CPacketADMIN_ACCOUNT_REQ_ADD Packet;
	CPacketMAP_SYSTEMMSG PacketMAP_SYSTEMMSG;
	CmyString strTmp;

	Packet.Set (pData);

	pInfoAccount = m_pLibInfoAccount->GetPtr ((LPCSTR)Packet.m_strAccount);
	if (pInfoAccount) {
		strTmp = "そのアカウント名は登録済みです";
	} else {
		pInfoAccount = (PCInfoAccount)m_pLibInfoAccount->GetNew ();
		TrimViewString (pInfoAccount->m_strAccount,  Packet.m_strAccount);
		TrimViewString (pInfoAccount->m_strPassword, Packet.m_strPassword);
		m_pLibInfoAccount->Add (pInfoAccount);
		strTmp = "アカウントを登録しました";
	}

	PacketMAP_SYSTEMMSG.Make ((LPCSTR)strTmp, RGB (255, 255,255), TRUE, SYSTEMMSGTYPE_NOLOG);
	m_pSock->SendTo (dwSessionID, &PacketMAP_SYSTEMMSG);
}

/* Copyright(C)URARA-works 2007 */
