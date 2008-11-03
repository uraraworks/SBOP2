/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_MAPOBJECTDATA.cpp								 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�I�u�W�F�N�g�z�u�f�[�^�ʒm) �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/03													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMapObjectData.h"
#include "Command.h"
#include "PacketMAP_MAPOBJECTDATA.h"

/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPOBJECTDATA::CPacketMAP_MAPOBJECTDATA				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

CPacketMAP_MAPOBJECTDATA::CPacketMAP_MAPOBJECTDATA()
{
	m_pLibInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPOBJECTDATA::~CPacketMAP_MAPOBJECTDATA			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

CPacketMAP_MAPOBJECTDATA::~CPacketMAP_MAPOBJECTDATA()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPOBJECTDATA::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CPacketMAP_MAPOBJECTDATA::Make(
	DWORD dwMapID,						/* [in] �}�b�vID */
	CLibInfoMapObjectData *pLibInfo)	/* [in] �}�b�v�I�u�W�F�N�g�z�u�f�[�^ */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_MAPOBJECTDATA;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID, sizeof (dwMapID), pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^ */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPOBJECTDATA::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CPacketMAP_MAPOBJECTDATA::Make(
	DWORD dwMapID,					/* [in] �}�b�vID */
	CInfoMapObjectData *pInfo)		/* [in] �}�b�v�I�u�W�F�N�g�z�u�f�[�^ */
{
	PCInfoMapObjectData pInfoTmp;
	CLibInfoMapObjectData LibInfoMapObjectData;

	LibInfoMapObjectData.Create ();

	pInfoTmp = (PCInfoMapObjectData)LibInfoMapObjectData.GetNew ();
	pInfoTmp->Copy (pInfo);
	LibInfoMapObjectData.Add (pInfoTmp);

	Make (dwMapID, &LibInfoMapObjectData);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPOBJECTDATA::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

PBYTE CPacketMAP_MAPOBJECTDATA::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID, pDataTmp, sizeof (m_dwMapID), pDataTmp);	/* �}�b�vID */

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoMapObjectData;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
