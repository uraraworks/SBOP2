/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_RENEWOBJECTDATA.cpp						 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�I�u�W�F�N�g�z�u�f�[�^�ʒm) �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/03													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMapObjectData.h"
#include "Command.h"
#include "PacketADMIN_MAP_RENEWOBJECTDATA.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_RENEWOBJECTDATA::CPacketADMIN_MAP_RENEWOBJECTDATA */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

CPacketADMIN_MAP_RENEWOBJECTDATA::CPacketADMIN_MAP_RENEWOBJECTDATA()
{
	m_dwMapID	= 0;
	m_pInfo		= NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_RENEWOBJECTDATA::~CPacketADMIN_MAP_RENEWOBJECTDATA */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

CPacketADMIN_MAP_RENEWOBJECTDATA::~CPacketADMIN_MAP_RENEWOBJECTDATA()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_RENEWOBJECTDATA::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CPacketADMIN_MAP_RENEWOBJECTDATA::Make(
	DWORD dwMapID,					/* [in] �}�b�vID */
	CInfoMapObjectData *pInfo)		/* [in] �z�u�f�[�^ */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMapID)		+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_RENEWOBJECTDATA;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,		sizeof (dwMapID),	pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,		pDataTmp);	/* �z�u�f�[�^ */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_RENEWOBJECTDATA::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_RENEWOBJECTDATA::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp, pDataTmpBack;
	CLibInfoMapObjectData LibInfo;
	CInfoMapObjectData InfoTmp;

	pRet		 = pPacket;
	pDataTmp	 = CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID, pDataTmp, sizeof (m_dwMapID), pDataTmp);	/* �}�b�vID */
	pDataTmpBack = pDataTmp;
	InfoTmp.SetSendData (pDataTmp);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= (PCInfoMapObjectData)LibInfo.GetNew ();
	pDataTmp	= m_pInfo->SetSendData (pDataTmpBack);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
