/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_RENEWMAPOBJECT.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�I�u�W�F�N�g�X�V) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/01													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapObject.h"
#include "Command.h"
#include "PacketADMIN_MAP_RENEWMAPOBJECT.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_RENEWMAPOBJECT::CPacketADMIN_MAP_RENEWMAPOBJECT */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

CPacketADMIN_MAP_RENEWMAPOBJECT::CPacketADMIN_MAP_RENEWMAPOBJECT()
{
	m_pInfoMapObject = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_RENEWMAPOBJECT::~CPacketADMIN_MAP_RENEWMAPOBJECT */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

CPacketADMIN_MAP_RENEWMAPOBJECT::~CPacketADMIN_MAP_RENEWMAPOBJECT()
{
	SAFE_DELETE (m_pInfoMapObject);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_RENEWMAPOBJECT::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CPacketADMIN_MAP_RENEWMAPOBJECT::Make(
	CInfoMapObject *pInfo)		/* [in] �}�b�v�I�u�W�F�N�g��� */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_RENEWMAPOBJECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �}�b�v�I�u�W�F�N�g��� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_RENEWMAPOBJECT::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_RENEWMAPOBJECT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfoMapObject);
	m_pInfoMapObject	= new CInfoMapObject;
	pDataTmp			= m_pInfoMapObject->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
