/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcMAP.cpp									 */
/* ���e			:�N���C�A���g���C���t���[��(�}�b�v�n��M����) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/04													 */
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
#include "LibInfoMapEvent.h"
#include "InfoMapBase.h"
#include "InfoMapEventBase.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "LayerMap.h"
#include "MgrLayer.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "MgrDraw.h"
#include "MgrWindow.h"
#include "MainFrame.h"


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP										 */
/* ���e		:��M����(�}�b�v�n)												 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_MAP_RES_MAPINFO:			RecvProcMAP_RES_MAPINFO			(pData);	break;	/* �}�b�v��񉞓� */
	case SBOCOMMANDID_SUB_MAP_ONLINE:				RecvProcMAP_ONLINE				(pData);	break;	/* �I�����C�����ʒm */
	case SBOCOMMANDID_SUB_MAP_SYSTEMMSG:			RecvProcMAP_SYSTEMMSG			(pData);	break;	/* �V�X�e�����b�Z�[�W�ʒm */
	case SBOCOMMANDID_SUB_MAP_FORMATMSG:			RecvProcMAP_FORMATMSG			(pData);	break;	/* �t�H�[�}�b�g���b�Z�[�W�ʒm */
	case SBOCOMMANDID_SUB_MAP_MAPOBJECT:			RecvProcMAP_MAPOBJECT			(pData);	break;	/* �}�b�v�I�u�W�F�N�g���ʒm */
	case SBOCOMMANDID_SUB_MAP_MAPOBJECTDATA:		RecvProcMAP_MAPOBJECTDATA		(pData);	break;	/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^�ʒm */
	case SBOCOMMANDID_SUB_MAP_DELETEMAPOBJECTDATA:	RecvProcMAP_DELETEMAPOBJECTDATA	(pData);	break;	/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^�폜 */
	case SBOCOMMANDID_SUB_MAP_MAPPARTS:				RecvProcMAP_MAPPARTS			(pData);	break;	/* �}�b�v�p�[�c���ʒm */
	case SBOCOMMANDID_SUB_MAP_SETPARTS:				RecvProcMAP_SETPARTS			(pData);	break;	/* �}�b�v�p�[�c�z�u */
	case SBOCOMMANDID_SUB_MAP_RENEWMAPSIZE:			RecvProcMAP_RENEWMAPSIZE		(pData);	break;	/* �}�b�v�T�C�Y�X�V */
	case SBOCOMMANDID_SUB_MAP_DELETEPARTS:			RecvProcMAP_DELETEPARTS			(pData);	break;	/* �}�b�v�p�[�c�폜 */
	case SBOCOMMANDID_SUB_MAP_MAPSHADOW:			RecvProcMAP_MAPSHADOW			(pData);	break;	/* �}�b�v�e���ʒm */
	case SBOCOMMANDID_SUB_MAP_SETMAPSHADOW:			RecvProcMAP_SETMAPSHADOW		(pData);	break;	/* �}�b�v�e�z�u */
	case SBOCOMMANDID_SUB_MAP_DELETEMAPSHADOW:		RecvProcMAP_DELETEMAPSHADOW		(pData);	break;	/* �}�b�v�e�폜 */
	case SBOCOMMANDID_SUB_MAP_MAPNAME:				RecvProcMAP_MAPNAME				(pData);	break;	/* �}�b�v���ʒm */
	case SBOCOMMANDID_SUB_MAP_MAPEVENT:				RecvProcMAP_MAPEVENT			(pData);	break;	/* �}�b�v�C�x���g���ʒm */
	case SBOCOMMANDID_SUB_MAP_DELETEEVENT:			RecvProcMAP_DELETEEVENT			(pData);	break;	/* �}�b�v�C�x���g���폜 */
	case SBOCOMMANDID_SUB_MAP_FADEINOUT:			RecvProcMAP_FADEINOUT			(pData);	break;	/* �t�F�[�h�C��/�A�E�g�ʒm */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_RES_MAPINFO							 */
/* ���e		:��M����(�}�b�v��񉞓�)										 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_RES_MAPINFO(PBYTE pData)
{
	PCInfoMapBase pInfoMap;
	PCInfoCharCli pInfoChar;
	PCLayerMap pLayerMap;
	CPacketMAP_RES_MAPINFO Packet;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_pInfo->m_dwMapID);
	if (pInfoMap == NULL) {
		pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetNew ();
		pInfoMap->Copy (Packet.m_pInfo);
		m_pLibInfoMap->Add (pInfoMap);
	} else {
		pInfoMap->Copy (Packet.m_pInfo);
	}

	pInfoChar	= m_pMgrData->GetPlayerChar ();
	pInfoMap	= (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap) {
		m_pMgrData->SetMap (pInfoMap);
		pInfoMap->RenewMapEvent ();
		m_pLibInfoChar->DeleteOutScreen (pInfoChar, TRUE);
		PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_LBUTTONDOWN, 0);
		m_pMgrSound->PlayBGM (pInfoMap->m_dwBGMID);
	}
	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
	if (pLayerMap) {
		pLayerMap->SetCenterPos (pInfoChar->m_nMapX, pInfoChar->m_nMapY);
	}
	RenewItemArea ();

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWMAPINFO, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_ONLINE									 */
/* ���e		:��M����(�I�����C�����ʒm)										 */
/* ���t		:2007/02/10														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_ONLINE(PBYTE pData)
{
	CPacketMAP_ONLINE Packet;

	Packet.Set (pData);
	m_pMgrData->SetOnlineCount (Packet.m_nCount);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWONLINECOUNT, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_SYSTEMMSG								 */
/* ���e		:��M����(�V�X�e�����b�Z�[�W�ʒm)								 */
/* ���t		:2007/02/10														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_SYSTEMMSG(PBYTE pData)
{
	BOOL bAddLog;
	CPacketMAP_SYSTEMMSG Packet;

	Packet.Set (pData);

	bAddLog = TRUE;
	if (Packet.m_nMsgType == SYSTEMMSGTYPE_NOLOG) {
		bAddLog = FALSE;
	} else if (Packet.m_nMsgType == SYSTEMMSGTYPE_BATTLE) {
		if (m_pMgrData->GetOptionBattleMsgLog () == FALSE) {
			bAddLog = FALSE;
		}
	}

	m_pMgrData->AddSystemMsg (bAddLog, Packet.m_strMsg, Packet.m_clMsg);
	if (Packet.m_bSound) {
		m_pMgrSound->PlaySound (SOUNDID_W_CHAT);
	}

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMMSG, (LPARAM)Packet.m_nMsgType);
	FlashMainWindow ();
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_FORMATMSG								 */
/* ���e		:��M����(�t�H�[�}�b�g���b�Z�[�W�ʒm)							 */
/* ���t		:2008/07/03														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_FORMATMSG(PBYTE pData)
{
	BOOL bAddLog;
	PCInfoCharCli pInfoChar, pInfoChar2, pInfoCharPlayer;
	PCInfoItem pInfoItem;
	CPacketMAP_FORMATMSG Packet;
	CmyString strMsg;

	Packet.Set (pData);
	pInfoCharPlayer = m_pMgrData->GetPlayerChar ();

	bAddLog = TRUE;
	if (Packet.m_nMsgType == SYSTEMMSGTYPE_NOLOG) {
		bAddLog = FALSE;
	} else if (Packet.m_nMsgType == SYSTEMMSGTYPE_BATTLE) {
		if (m_pMgrData->GetOptionBattleMsgLog () == FALSE) {
			bAddLog = FALSE;
		}
	}

	switch (Packet.m_dwMsgID) {
	case FORMATMSGID_DAMAGE1:				/* %s���� %d �̃_���[�W */
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwPara1);
		if (pInfoChar == NULL) {
			return;
		}
		strMsg.Format ("%s���� %d �̃_���[�W", (LPCSTR)pInfoChar->m_strCharName, Packet.m_dwPara2);
		break;
	case FORMATMSGID_DAMAGE2:				/* %s�� %d �̃_���[�W */
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwPara1);
		if (pInfoChar == NULL) {
			return;
		}
		strMsg.Format ("%s�� %d �̃_���[�W", (LPCSTR)pInfoChar->m_strCharName, Packet.m_dwPara2);
		break;
	case FORMATMSGID_SWOON1:				/* %s��%s��|���� */
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwPara1);
		if (pInfoChar == NULL) {
			return;
		}
		pInfoChar2 = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwPara2);
		if (pInfoChar2 == NULL) {
			return;
		}
		if ((pInfoChar == pInfoCharPlayer) || (pInfoChar2 == pInfoCharPlayer)) {
			return;
		}
		strMsg.Format ("%s��%s��|����", (LPCSTR)pInfoChar->m_strCharName, (LPCSTR)pInfoChar2->m_strCharName);
		break;
	case FORMATMSGID_SWOON2:				/* %s��|���� */
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwPara1);
		if (pInfoChar == NULL) {
			return;
		}
		strMsg.Format ("%s��|����", (LPCSTR)pInfoChar->m_strCharName);
		break;
	case FORMATMSGID_SWOON3:				/* %s�ɓ|���ꂽ */
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwPara1);
		if (pInfoChar == NULL) {
			return;
		}
		strMsg.Format ("%s�ɓ|���ꂽ", (LPCSTR)pInfoChar->m_strCharName);
		break;
	case FORMATMSGID_NOUSE:					/* %s�͌��ʂ�����܂���ł��� */
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (Packet.m_dwPara1);
		if (pInfoItem == NULL) {
			return;
		}
		strMsg.Format ("%s �͌��ʂ�����܂���ł���", (LPCSTR)pInfoItem->m_strName);
		break;
	case FORMATMSGID_NOUNEQUIP_ITEMFULL:	/* �������������ς��ő������O���܂��� */
		strMsg.Format ("�������������ς��ő������O���܂���");
		break;
	case FORMATMSGID_DROPITEM:				/* %s��%s�𗎂Ƃ��� */
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwPara1);
		if (pInfoChar == NULL) {
			return;
		}
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (Packet.m_dwPara2);
		if (pInfoItem == NULL) {
			return;
		}
		strMsg.Format ("%s�� %s �𗎂Ƃ���", (LPCSTR)pInfoChar->m_strCharName, (LPCSTR)pInfoItem->m_strName);
		break;
	default:
		return;
	}

	m_pMgrData->AddSystemMsg (bAddLog, strMsg, Packet.m_clMsg);
	if (Packet.m_bSound) {
		m_pMgrSound->PlaySound (SOUNDID_W_CHAT);
		FlashMainWindow ();
	}

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_MAPOBJECT								 */
/* ���e		:��M����(�}�b�v�I�u�W�F�N�g���ʒm)							 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_MAPOBJECT(PBYTE pData)
{
	CPacketMAP_MAPOBJECT Packet;

	Packet.Set (pData);

	m_pLibInfoMapObject->Merge (Packet.m_pLibInfoMapObject);

	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWMAPOBJECT, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_MAPOBJECTDATA							 */
/* ���e		:��M����(�}�b�v�I�u�W�F�N�g�z�u�f�[�^�ʒm)						 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_MAPOBJECTDATA(PBYTE pData)
{
	PCInfoMapBase pInfoMap;
	CPacketMAP_MAPOBJECTDATA Packet;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}

	pInfoMap->m_pLibInfoMapObjectData->Merge (Packet.m_pLibInfo);

	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWMAPINFO, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_DELETEMAPOBJECTDATA					 */
/* ���e		:��M����(�}�b�v�I�u�W�F�N�g�z�u�f�[�^�폜)						 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_DELETEMAPOBJECTDATA(PBYTE pData)
{
	PCInfoMapBase pInfoMap;
	CPacketMAP_PARA1 Packet;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwCharID);
	if (pInfoMap == NULL) {
		return;
	}

	pInfoMap->m_pLibInfoMapObjectData->Delete (Packet.m_dwPara);

	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWMAPINFO, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_MAPPARTS								 */
/* ���e		:��M����(�}�b�v�p�[�c���ʒm)									 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_MAPPARTS(PBYTE pData)
{
	CPacketMAP_MAPPARTS Packet;

	Packet.Set (pData);

	m_pLibInfoMapParts->Merge (Packet.m_pLibInfoMapParts);
	m_pLibInfoMapParts->ResetAnime ();

	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWMAPPARTS, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_SETPARTS								 */
/* ���e		:��M����(�}�b�v�p�[�c�z�u)										 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_SETPARTS(PBYTE pData)
{
	PCInfoMapBase pInfoMap;
	CPacketMAP_SETPARTS Packet;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}
	pInfoMap->SetParts (Packet.m_ptPos.x, Packet.m_ptPos.y, Packet.m_dwPartsID);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_RENEWMAPSIZE							 */
/* ���e		:��M����(�}�b�v�T�C�Y�X�V)										 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_RENEWMAPSIZE(PBYTE pData)
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

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWMAPINFO, 0);
	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWMAPINFO, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_DELETEPARTS							 */
/* ���e		:��M����(�}�b�v�p�[�c�폜)										 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_DELETEPARTS(PBYTE pData)
{
	CPacketMAP_DELETEPARTS Packet;

	Packet.Set (pData);

	m_pLibInfoMapParts->Delete (Packet.m_dwPartsID);
	m_pLibInfoMap->		DeleteParts (Packet.m_dwPartsID);

	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWMAPPARTS, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_MAPSHADOW								 */
/* ���e		:��M����(�}�b�v�e���ʒm)										 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_MAPSHADOW(PBYTE pData)
{
	CPacketMAP_MAPSHADOW Packet;

	Packet.Set (pData);

	m_pLibInfoMapShadow->Merge (Packet.m_pLibInfoMapShadow);
	m_pLibInfoMapShadow->ResetAnime ();
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_SETMAPSHADOW							 */
/* ���e		:��M����(�}�b�v�e�z�u)											 */
/* ���t		:2007/06/09														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_SETMAPSHADOW(PBYTE pData)
{
	PCInfoMapBase pInfoMap;
	CPacketMAP_SETMAPSHADOW Packet;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}
	pInfoMap->SetShadow (Packet.m_ptPos.x, Packet.m_ptPos.y, Packet.m_dwShadowID);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_DELETEMAPSHADOW						 */
/* ���e		:��M����(�}�b�v�e�폜)											 */
/* ���t		:2007/06/09														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_DELETEMAPSHADOW(PBYTE pData)
{
	CPacketMAP_DELETEMAPSHADOW Packet;

	Packet.Set (pData);

	m_pLibInfoMapShadow->	Delete (Packet.m_dwShadowID);
	m_pLibInfoMap->			DeleteShadow (Packet.m_dwShadowID);

	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWMAPSHADOW, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_MAPNAME								 */
/* ���e		:��M����(�}�b�v���ʒm)											 */
/* ���t		:2008/05/24														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_MAPNAME(PBYTE pData)
{
	PCInfoMapBase pInfoMap;
	CPacketMAP_MAPNAME Packet;
	CmyString strTmp, strMapNameBack;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}
	strMapNameBack = pInfoMap->m_strMapName;

	pInfoMap->m_strMapName		= Packet.m_strMapName;
	pInfoMap->m_dwBGMID			= Packet.m_dwBGMID;			/* BGMID */
	pInfoMap->m_dwWeatherType	= Packet.m_dwWeatherType;	/* �V�C��� */
	pInfoMap->m_byLevel			= Packet.m_byLevel;			/* ���邳���x�� */

	if (pInfoMap->m_strMapName != strMapNameBack) {
		strTmp.Format ("�}�b�v����[%s]�ɕύX����܂���", (LPCSTR)pInfoMap->m_strMapName);
		m_pMgrData->AddSystemMsg (TRUE, strTmp, RGB (0, 200, 255));
		m_pMgrSound->PlaySound (SOUNDID_W_CHAT);
	}
	m_pMgrSound->PlayBGM (pInfoMap->m_dwBGMID);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWMAPINFO, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_MAPEVENT								 */
/* ���e		:��M����(�}�b�v�C�x���g���ʒm)								 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_MAPEVENT(PBYTE pData)
{
	PCInfoMapBase pInfoMap;
	CPacketMAP_MAPEVENT Packet;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}

	pInfoMap->m_pLibInfoMapEvent->Merge (Packet.m_pLibInfo);
	pInfoMap->RenewMapEvent ();

	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWMAPINFO, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_DELETEEVENT							 */
/* ���e		:��M����(�}�b�v�C�x���g���폜)								 */
/* ���t		:2008/07/22														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_DELETEEVENT(PBYTE pData)
{
	PCInfoMapBase pInfoMap;
	CPacketMAP_PARA1 Packet;

	Packet.Set (pData);

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwCharID);
	if (pInfoMap == NULL) {
		return;
	}

	pInfoMap->m_pLibInfoMapEvent->Delete (Packet.m_dwPara);
	pInfoMap->RenewMapEvent ();

	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWMAPINFO, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_FADEINOUT								 */
/* ���e		:��M����(�t�F�[�h�C��/�A�E�g�ʒm)								 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_FADEINOUT(PBYTE pData)
{
	CPacketMAP_PARA1 Packet;

	Packet.Set (pData);

	/* �t�F�[�h�C���H */
	if (Packet.m_dwPara == 0) {
		m_pMgrDraw->SetFadeState (FADESTATE_FADEIN);

	/* �t�F�[�h�A�E�g */
	} else {
		m_pMgrDraw->SetFadeState (FADESTATE_FADEOUT);
		m_pMgrWindow->DeleteAll ();
	}
}

/* Copyright(C)URARA-works 2007 */
