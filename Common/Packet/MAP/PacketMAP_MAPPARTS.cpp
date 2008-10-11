/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_MAPPARTS.cpp										 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�p�[�c���ʒm) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/30													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMapParts.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketMAP_MAPPARTS.h"

/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPPARTS::CPacketMAP_MAPPARTS						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

CPacketMAP_MAPPARTS::CPacketMAP_MAPPARTS()
{
	m_pLibInfoMapParts = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPPARTS::~CPacketMAP_MAPPARTS						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

CPacketMAP_MAPPARTS::~CPacketMAP_MAPPARTS()
{
	SAFE_DELETE (m_pLibInfoMapParts);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPPARTS::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CPacketMAP_MAPPARTS::Make(
	CLibInfoMapParts *pLibInfo)		/* [in] �}�b�v�p�[�c��� */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pLibInfo->GetSendDataSize ();
	pInfoDataTmp	= pLibInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE) +
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_MAPPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �}�b�v�p�[�c��� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPPARTS::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CPacketMAP_MAPPARTS::Make(
	CInfoMapParts *pInfo)		/* [in] �}�b�v�p�[�c��� */
{
	PCInfoMapParts pInfoTmp;
	CLibInfoMapParts LibInfoMapParts;

	LibInfoMapParts.Create ();

	pInfoTmp = (PCInfoMapParts)LibInfoMapParts.GetNew ();
	pInfoTmp->Copy (pInfo);
	LibInfoMapParts.Add (pInfoTmp);

	Make (&LibInfoMapParts);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPPARTS::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

PBYTE CPacketMAP_MAPPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfoMapParts);
	m_pLibInfoMapParts	= new CLibInfoMapParts;
	m_pLibInfoMapParts->Create ();
	pDataTmp			= m_pLibInfoMapParts->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
