/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_RES_TALKEVENT.cpp								 */
/* ���e			:�R�}���h(��b�C�x���g�n:��b�C�x���g��񉞓�) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/27													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoTalkEvent.h"
#include "Command.h"
#include "PacketCHAR_RES_TALKEVENT.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_TALKEVENT::CPacketCHAR_RES_TALKEVENT			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

CPacketCHAR_RES_TALKEVENT::CPacketCHAR_RES_TALKEVENT()
{
	m_dwParam	= 0;
	m_pInfo		= NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_TALKEVENT::~CPacketCHAR_RES_TALKEVENT			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

CPacketCHAR_RES_TALKEVENT::~CPacketCHAR_RES_TALKEVENT()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_TALKEVENT::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

void CPacketCHAR_RES_TALKEVENT::Make(
	CInfoTalkEvent *pInfo,		/* [in] ��b�C�x���g��� */
	DWORD dwParam)				/* [in] �p�����[�^ */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwParam)		+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_RES_TALKEVENT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwParam,		sizeof (dwParam),	pDataTmp);	/* �p�����[�^ */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,		pDataTmp);	/* ��b�C�x���g��� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_TALKEVENT::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

PBYTE CPacketCHAR_RES_TALKEVENT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwParam, pDataTmp, sizeof (m_dwParam), pDataTmp);	/* �p�����[�^ */

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoTalkEvent;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
