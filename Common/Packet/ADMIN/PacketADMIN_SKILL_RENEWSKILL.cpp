/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_SKILL_RENEWSKILL.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�X�L�����ʒm) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoSkill.h"
#include "Command.h"
#include "PacketADMIN_SKILL_RENEWSKILL.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_SKILL_RENEWSKILL::CPacketADMIN_SKILL_RENEWSKILL	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/08														 */
/* ========================================================================= */

CPacketADMIN_SKILL_RENEWSKILL::CPacketADMIN_SKILL_RENEWSKILL()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_SKILL_RENEWSKILL::~CPacketADMIN_SKILL_RENEWSKILL	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/08														 */
/* ========================================================================= */

CPacketADMIN_SKILL_RENEWSKILL::~CPacketADMIN_SKILL_RENEWSKILL()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_SKILL_RENEWSKILL::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/12/08														 */
/* ========================================================================= */

void CPacketADMIN_SKILL_RENEWSKILL::Make(
	CInfoSkillBase *pInfo)		/* [in] �X�L����� */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE)	+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_SKILL_RENEWSKILL;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �X�L����� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_SKILL_RENEWSKILL::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/12/08														 */
/* ========================================================================= */

PBYTE CPacketADMIN_SKILL_RENEWSKILL::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp, pDataTmpBack;
	CLibInfoSkill LibInfo;
	CInfoSkillBase InfoTmp;

	pRet		 = pPacket;
	pDataTmp	 = CPacketBase::Set (pPacket);

	pDataTmpBack = pDataTmp;
	InfoTmp.SetSendData (pDataTmp);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= (PCInfoSkillBase)LibInfo.GetNew (InfoTmp.m_nType);
	pDataTmp	= m_pInfo->SetSendData (pDataTmpBack);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
