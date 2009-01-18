/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_MODIFYSKILL.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�����X�L���̕ύX) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/01/18													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketADMIN_CHAR_MODIFYSKILL.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_MODIFYSKILL::CPacketADMIN_CHAR_MODIFYSKILL	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/01/18														 */
/* ========================================================================= */

CPacketADMIN_CHAR_MODIFYSKILL::CPacketADMIN_CHAR_MODIFYSKILL()
{
	m_dwCharID	= 0;
	m_dwSkillID	= 0;
	m_nType		= CHARMODIFYSKILLTYPE_NONE;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_MODIFYSKILL::~CPacketADMIN_CHAR_MODIFYSKILL	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/01/18														 */
/* ========================================================================= */

CPacketADMIN_CHAR_MODIFYSKILL::~CPacketADMIN_CHAR_MODIFYSKILL()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_MODIFYSKILL::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2009/01/18														 */
/* ========================================================================= */

void CPacketADMIN_CHAR_MODIFYSKILL::Make(
	DWORD dwCharID,			/* [in] �L����ID */
	DWORD dwSkillID,		/* [in] �X�L��ID */
	int nType)				/* [in] ��� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwSkillID)		+
			 sizeof (nType);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_MODIFYSKILL;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwSkillID,	sizeof (dwSkillID),	pDataTmp);	/* �X�L��ID */
	CopyMemoryRenew (pDataTmp, &nType,		sizeof (nType),		pDataTmp);	/* ��� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_MODIFYSKILL::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2009/01/18														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHAR_MODIFYSKILL::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwSkillID,	pDataTmp, sizeof (m_dwSkillID),	pDataTmp);	/* �X�L��ID */
	CopyMemoryRenew (&m_nType,		pDataTmp, sizeof (m_nType),		pDataTmp);	/* ��� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2009 */
