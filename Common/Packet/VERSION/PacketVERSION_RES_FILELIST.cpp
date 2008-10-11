/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_RES_FILELIST.cpp								 */
/* ���e			:�R�}���h(�o�[�W�����n:�t�@�C�����X�g����) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/03/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "InfoFileList.h"
#include "PacketVERSION_RES_FILELIST.h"

/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_FILELIST::CPacketVERSION_RES_FILELIST		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

CPacketVERSION_RES_FILELIST::CPacketVERSION_RES_FILELIST()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_FILELIST::~CPacketVERSION_RES_FILELIST		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

CPacketVERSION_RES_FILELIST::~CPacketVERSION_RES_FILELIST()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_FILELIST::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

void CPacketVERSION_RES_FILELIST::Make(CInfoFileList *pSrc)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwSizeInfo;
	PPACKETBASE pPacketBase;

	dwSizeInfo	= pSrc->GetSendDataSize ();
	pDataTmp	= pSrc->GetSendData ();
	dwSize		= sizeof (PACKETBASE) + dwSizeInfo;

	pData = ZeroNew (dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_RES_FILELIST;

	CopyMemory ((pPacketBase + 1), pDataTmp, dwSizeInfo);

	RenewPacket (pData, dwSize);
	SAFE_DELETE (pDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_FILE::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

PBYTE CPacketVERSION_RES_FILELIST::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfo);
	m_pInfo  = new CInfoFileList;
	pDataTmp = m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
