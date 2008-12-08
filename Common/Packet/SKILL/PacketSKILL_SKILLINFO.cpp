/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketSKILL_SKILLINFO.cpp									 */
/* ���e			:�R�}���h(�X�L���n:�X�L�����ʒm) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/06													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketSKILL_SKILLINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketSKILL_SKILLINFO::CPacketSKILL_SKILLINFO					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/06														 */
/* ========================================================================= */

CPacketSKILL_SKILLINFO::CPacketSKILL_SKILLINFO()
{
	m_pLibInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketSKILL_SKILLINFO::~CPacketSKILL_SKILLINFO				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/06														 */
/* ========================================================================= */

CPacketSKILL_SKILLINFO::~CPacketSKILL_SKILLINFO()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketSKILL_SKILLINFO::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/12/06														 */
/* ========================================================================= */

void CPacketSKILL_SKILLINFO::Make(
	PCLibInfoSkill pInfo)		/* [in] �X�L����� */
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_SKILL;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_SKILL_SKILLINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �X�L����� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketSKILL_SKILLINFO::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/12/08														 */
/* ========================================================================= */

void CPacketSKILL_SKILLINFO::Make(
	CInfoSkillBase *pInfo)		/* [in] �X�L����� */
{
	PCInfoSkillBase pInfoTmp;
	CLibInfoSkill LibInfoSkill;

	LibInfoSkill.Create ();

	pInfoTmp = (PCInfoSkillBase)LibInfoSkill.GetNew (pInfo->m_nType);
	pInfoTmp->Copy (pInfo);
	LibInfoSkill.Add (pInfoTmp);

	Make (&LibInfoSkill);
}


/* ========================================================================= */
/* �֐���	:CPacketSKILL_SKILLINFO::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/12/06														 */
/* ========================================================================= */

PBYTE CPacketSKILL_SKILLINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoSkill;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
