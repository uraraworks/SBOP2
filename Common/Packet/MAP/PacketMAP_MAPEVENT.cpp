/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_MAPEVENT.cpp										 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�C�x���g���ʒm) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/25													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMapEvent.h"
#include "InfoMapEvent/InfoMapEventBase.h"
#include "Command.h"
#include "PacketMAP_MAPEVENT.h"

/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPEVENT::CPacketMAP_MAPEVENT						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

CPacketMAP_MAPEVENT::CPacketMAP_MAPEVENT()
{
	m_pLibInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPEVENT::~CPacketMAP_MAPEVENT						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

CPacketMAP_MAPEVENT::~CPacketMAP_MAPEVENT()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPEVENT::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

void CPacketMAP_MAPEVENT::Make(
	DWORD dwMapID,					/* [in] �}�b�vID */
	CLibInfoMapEvent *pLibInfo)		/* [in] �}�b�v�C�x���g��� */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pLibInfo->GetSendDataSize ();
	pInfoDataTmp	= pLibInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMapID)		+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_MAPEVENT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID, sizeof (dwMapID), pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �}�b�v�C�x���g��� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPEVENT::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

void CPacketMAP_MAPEVENT::Make(
	DWORD dwMapID,					/* [in] �}�b�vID */
	CInfoMapEventBase *pInfo)		/* [in] �}�b�v�C�x���g��� */
{
	PCInfoMapEventBase pInfoTmp;
	CLibInfoMapEvent LibInfoMapEvent;

	LibInfoMapEvent.Create ();

	pInfoTmp = (PCInfoMapEventBase)LibInfoMapEvent.GetNew (pInfo->m_nType);
	pInfoTmp->Copy (pInfo);
	LibInfoMapEvent.Add (pInfoTmp);

	Make (dwMapID, &LibInfoMapEvent);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPEVENT::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

PBYTE CPacketMAP_MAPEVENT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID, pDataTmp, sizeof (m_dwMapID), pDataTmp);	/* �}�b�vID */

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoMapEvent;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
