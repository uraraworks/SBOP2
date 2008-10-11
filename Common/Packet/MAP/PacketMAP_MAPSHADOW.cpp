/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_MAPSHADOW.cpp									 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�e���ʒm) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMapShadow.h"
#include "InfoMapShadow.h"
#include "Command.h"
#include "PacketMAP_MAPSHADOW.h"

/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPSHADOW::CPacketMAP_MAPSHADOW						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

CPacketMAP_MAPSHADOW::CPacketMAP_MAPSHADOW()
{
	m_pLibInfoMapShadow = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPSHADOW::~CPacketMAP_MAPSHADOW					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

CPacketMAP_MAPSHADOW::~CPacketMAP_MAPSHADOW()
{
	SAFE_DELETE (m_pLibInfoMapShadow);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPSHADOW::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

void CPacketMAP_MAPSHADOW::Make(
	CLibInfoMapShadow *pLibInfo)		/* [in] �}�b�v�e��� */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_MAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �}�b�v�e��� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPSHADOW::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

void CPacketMAP_MAPSHADOW::Make(
	CInfoMapShadow *pInfo)		/* [in] �}�b�v�e��� */
{
	PCInfoMapShadow pInfoTmp;
	CLibInfoMapShadow LibInfoMapShadow;

	LibInfoMapShadow.Create ();

	pInfoTmp = (PCInfoMapShadow)LibInfoMapShadow.GetNew ();
	pInfoTmp->Copy (pInfo);
	LibInfoMapShadow.Add (pInfoTmp);

	Make (&LibInfoMapShadow);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPSHADOW::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

PBYTE CPacketMAP_MAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfoMapShadow);
	m_pLibInfoMapShadow	= new CLibInfoMapShadow;
	m_pLibInfoMapShadow->Create ();
	pDataTmp			= m_pLibInfoMapShadow->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
