/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_RES_MAPINFO.cpp									 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v��񉞓�) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/04													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapBase.h"
#include "Command.h"
#include "PacketMAP_RES_MAPINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketMAP_RES_MAPINFO::CPacketMAP_RES_MAPINFO					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

CPacketMAP_RES_MAPINFO::CPacketMAP_RES_MAPINFO()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_RES_MAPINFO::~CPacketMAP_RES_MAPINFO				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

CPacketMAP_RES_MAPINFO::~CPacketMAP_RES_MAPINFO()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_RES_MAPINFO::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

void CPacketMAP_RES_MAPINFO::Make(
	CInfoMapBase *pInfo)		/* [in] �}�b�v��� */
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_RES_MAPINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �}�b�v��� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_RES_MAPINFO::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

PBYTE CPacketMAP_RES_MAPINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoMapBase;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
