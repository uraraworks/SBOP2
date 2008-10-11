/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcMAP.cpp									 */
/* ���e			:�T�[�o�[���C���t���[��(�}�b�v�n��M����) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/04													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "InfoMapBase.h"
#include "LibInfoMapBase.h"
#include "MgrData.h"
#include "MainFrame.h"


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP										 */
/* ���e		:��M����(�}�b�v�n)												 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_MAP_REQ_MAPINFO:	RecvProcMAP_REQ_MAPINFO (pData, dwSessionID);	break;	/* �}�b�v���v�� */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMAP_REQ_MAPINFO							 */
/* ���e		:��M����(�}�b�v���v��)										 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_REQ_MAPINFO(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapBase pInfoMap;
	CPacketMAP_REQ_MAPINFO Packet;
	CPacketMAP_RES_MAPINFO PacketRES_MAPINFO;

	Packet.Set (pData);

	pInfoMap	= (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}

	PacketRES_MAPINFO.Make (pInfoMap);
	m_pSock->SendTo (dwSessionID, &PacketRES_MAPINFO);
}

/* Copyright(C)URARA-works 2007 */
