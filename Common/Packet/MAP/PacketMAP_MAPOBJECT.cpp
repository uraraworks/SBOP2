/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_MAPOBJECT.cpp									 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�I�u�W�F�N�g���ʒm) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/01													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMapObject.h"
#include "InfoMapObject.h"
#include "Command.h"
#include "PacketMAP_MAPOBJECT.h"

/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPOBJECT::CPacketMAP_MAPOBJECT						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

CPacketMAP_MAPOBJECT::CPacketMAP_MAPOBJECT()
{
	m_pLibInfoMapObject = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPOBJECT::~CPacketMAP_MAPOBJECT					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

CPacketMAP_MAPOBJECT::~CPacketMAP_MAPOBJECT()
{
	SAFE_DELETE (m_pLibInfoMapObject);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPOBJECT::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CPacketMAP_MAPOBJECT::Make(
	CLibInfoMapObject *pLibInfo)		/* [in] �}�b�v�I�u�W�F�N�g��� */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_MAPOBJECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �}�b�v�I�u�W�F�N�g��� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPOBJECT::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CPacketMAP_MAPOBJECT::Make(
	CInfoMapObject *pInfo)		/* [in] �}�b�v�I�u�W�F�N�g��� */
{
	PCInfoMapObject pInfoTmp;
	CLibInfoMapObject LibInfoMapObject;

	LibInfoMapObject.Create ();

	pInfoTmp = (PCInfoMapObject)LibInfoMapObject.GetNew ();
	pInfoTmp->Copy (pInfo);
	LibInfoMapObject.Add (pInfoTmp);

	Make (&LibInfoMapObject);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPOBJECT::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

PBYTE CPacketMAP_MAPOBJECT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfoMapObject);
	m_pLibInfoMapObject	= new CLibInfoMapObject;
	m_pLibInfoMapObject->Create ();
	pDataTmp			= m_pLibInfoMapObject->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
