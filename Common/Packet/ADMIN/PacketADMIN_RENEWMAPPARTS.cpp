/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_RENEWMAPPARTS.cpp								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�p�[�c�X�V) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/29													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketADMIN_RENEWMAPPARTS.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEWMAPPARTS::CPacketADMIN_RENEWMAPPARTS			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/04/29														 */
/* ========================================================================= */

CPacketADMIN_RENEWMAPPARTS::CPacketADMIN_RENEWMAPPARTS()
{
	m_pInfoMapParts = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEWMAPPARTS::~CPacketADMIN_RENEWMAPPARTS		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/04/29														 */
/* ========================================================================= */

CPacketADMIN_RENEWMAPPARTS::~CPacketADMIN_RENEWMAPPARTS()
{
	SAFE_DELETE (m_pInfoMapParts);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEWMAPPARTS::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/04/29														 */
/* ========================================================================= */

void CPacketADMIN_RENEWMAPPARTS::Make(
	CInfoMapParts *pInfo)		/* [in] �}�b�v�p�[�c��� */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE) +
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_RENEWMAPPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �}�b�v�p�[�c��� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEWMAPPARTS::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/04/29														 */
/* ========================================================================= */

PBYTE CPacketADMIN_RENEWMAPPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfoMapParts);
	m_pInfoMapParts	= new CInfoMapParts;
	pDataTmp		= m_pInfoMapParts->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
