/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_GRP.cpp											 */
/* ���e			:�R�}���h(�L�����n:�摜�ʒm) �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketCHAR_GRP.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_GRP::CPacketCHAR_GRP								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

CPacketCHAR_GRP::CPacketCHAR_GRP()
{
	m_dwCharID					= 0;	/* �L����ID */
	m_dwEquipItemIDCloth		= 0;	/* �����A�C�e��ID(��) */
	m_dwEquipItemIDAcce1		= 0;	/* �����A�C�e��ID(�A�N�Z�T��) */
	m_dwEquipItemIDArmsRight	= 0;	/* �����A�C�e��ID(������) */
	m_dwEquipItemIDArmsLeft		= 0;	/* �����A�C�e��ID:���� */
	m_wGrpIDNPC					= 0;	/* �摜ID(NPC) */
	m_wGrpIDCloth				= 0;	/* �摜ID(��) */
	m_wGrpIDEye					= 0;	/* �摜ID(��) */
	m_wGrpIDEyeColor			= 0;	/* �摜ID(�ڐF) */
	m_wGrpIDHairType			= 0;	/* �摜ID(��) */
	m_wGrpIDHairColor			= 0;	/* �摜ID(���F) */
	m_wGrpIDSP					= 0;	/* �摜ID(���ꕞ) */
	m_wGrpIDAcce				= 0;	/* �摜ID(�A�N�Z�T��) */
	m_wGrpIDArmsMain			= 0;	/* �摜ID(������:���C��) */
	m_wGrpIDArmsSub				= 0;	/* �摜ID(������:�T�u) */
	m_wGrpIDArmsLeftMain		= 0;	/* �摜ID(��:���C��) */
	m_wGrpIDArmsLeftSub			= 0;	/* �摜ID(��:�T�u) */
	m_wGrpIDTmpMain				= 0;	/* �摜ID(�ꎞ��:���C��) */
	m_wGrpIDTmpSub				= 0;	/* �摜ID(�ꎞ��:�T�u) */
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_GRP::~CPacketCHAR_GRP								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

CPacketCHAR_GRP::~CPacketCHAR_GRP()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_GRP::Make											 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

void CPacketCHAR_GRP::Make(
	CInfoCharBase *pInfo)		/* [in] �L������� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)						+
			 sizeof (pInfo->m_dwCharID)					+
			 sizeof (pInfo->m_dwEquipItemIDCloth)		+
			 sizeof (pInfo->m_dwEquipItemIDAcce1)		+
			 sizeof (pInfo->m_dwEquipItemIDArmsRight)	+
			 sizeof (pInfo->m_dwEquipItemIDArmsLeft)	+
			 sizeof (pInfo->m_wGrpIDNPC)				+
			 sizeof (pInfo->m_wGrpIDCloth)				+
			 sizeof (pInfo->m_wGrpIDEye)				+
			 sizeof (pInfo->m_wGrpIDEyeColor)			+
			 sizeof (pInfo->m_wGrpIDHairType)			+
			 sizeof (pInfo->m_wGrpIDHairColor)			+
			 sizeof (pInfo->m_wGrpIDSP)					+
			 sizeof (pInfo->m_wGrpIDAcce)				+
			 sizeof (pInfo->m_wGrpIDArmsMain)			+
			 sizeof (pInfo->m_wGrpIDArmsSub)			+
			 sizeof (pInfo->m_wGrpIDArmsLeftMain)		+
			 sizeof (pInfo->m_wGrpIDArmsLeftSub)		+
			 sizeof (pInfo->m_wGrpIDTmpMain)			+
			 sizeof (pInfo->m_wGrpIDTmpSub);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_GRP;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwCharID,					sizeof (pInfo->m_dwCharID),					pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwEquipItemIDCloth,		sizeof (pInfo->m_dwEquipItemIDCloth),		pDataTmp);	/* �����A�C�e��ID(��) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwEquipItemIDAcce1,		sizeof (pInfo->m_dwEquipItemIDAcce1),		pDataTmp);	/* �����A�C�e��ID(�A�N�Z�T��) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwEquipItemIDArmsRight,	sizeof (pInfo->m_dwEquipItemIDArmsRight),	pDataTmp);	/* �����A�C�e��ID(������) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDNPC,					sizeof (pInfo->m_wGrpIDNPC),				pDataTmp);	/* �摜ID(NPC) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDCloth,				sizeof (pInfo->m_wGrpIDCloth),				pDataTmp);	/* �摜ID(��) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDEye,					sizeof (pInfo->m_wGrpIDEye),				pDataTmp);	/* �摜ID(��) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDEyeColor,			sizeof (pInfo->m_wGrpIDEyeColor),			pDataTmp);	/* �摜ID(�ڐF) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDHairType,			sizeof (pInfo->m_wGrpIDHairType),			pDataTmp);	/* �摜ID(��) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDHairColor,			sizeof (pInfo->m_wGrpIDHairColor),			pDataTmp);	/* �摜ID(���F) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDSP,					sizeof (pInfo->m_wGrpIDSP),					pDataTmp);	/* �摜ID(���ꕞ) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDAcce,				sizeof (pInfo->m_wGrpIDAcce),				pDataTmp);	/* �摜ID(�A�N�Z�T��) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDArmsMain,			sizeof (pInfo->m_wGrpIDArmsMain),			pDataTmp);	/* �摜ID(������:���C��) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDArmsSub,				sizeof (pInfo->m_wGrpIDArmsSub),			pDataTmp);	/* �摜ID(������:�T�u) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDArmsLeftMain,		sizeof (pInfo->m_wGrpIDArmsLeftMain),		pDataTmp);	/* �摜ID(��:���C��) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDArmsLeftSub,			sizeof (pInfo->m_wGrpIDArmsLeftSub),		pDataTmp);	/* �摜ID(��:�T�u) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDTmpMain,				sizeof (pInfo->m_wGrpIDTmpMain),			pDataTmp);	/* �摜ID(�ꎞ��:���C��) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDTmpSub,				sizeof (pInfo->m_wGrpIDTmpSub),				pDataTmp);	/* �摜ID(�ꎞ��:�T�u) */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_GRP::Set											 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

PBYTE CPacketCHAR_GRP::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,					pDataTmp, sizeof (m_dwCharID),					pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwEquipItemIDCloth,			pDataTmp, sizeof (m_dwEquipItemIDCloth),		pDataTmp);	/* �����A�C�e��ID(��) */
	CopyMemoryRenew (&m_dwEquipItemIDAcce1,			pDataTmp, sizeof (m_dwEquipItemIDAcce1),		pDataTmp);	/* �����A�C�e��ID(�A�N�Z�T��) */
	CopyMemoryRenew (&m_dwEquipItemIDArmsRight,		pDataTmp, sizeof (m_dwEquipItemIDArmsRight),	pDataTmp);	/* �����A�C�e��ID(������) */
	CopyMemoryRenew (&m_wGrpIDNPC,					pDataTmp, sizeof (m_wGrpIDNPC),					pDataTmp);	/* �摜ID(NPC) */
	CopyMemoryRenew (&m_wGrpIDCloth,				pDataTmp, sizeof (m_wGrpIDCloth),				pDataTmp);	/* �摜ID(��) */
	CopyMemoryRenew (&m_wGrpIDEye,					pDataTmp, sizeof (m_wGrpIDEye),					pDataTmp);	/* �摜ID(��) */
	CopyMemoryRenew (&m_wGrpIDEyeColor,				pDataTmp, sizeof (m_wGrpIDEyeColor),			pDataTmp);	/* �摜ID(�ڐF) */
	CopyMemoryRenew (&m_wGrpIDHairType,				pDataTmp, sizeof (m_wGrpIDHairType),			pDataTmp);	/* �摜ID(��) */
	CopyMemoryRenew (&m_wGrpIDHairColor,			pDataTmp, sizeof (m_wGrpIDHairColor),			pDataTmp);	/* �摜ID(���F) */
	CopyMemoryRenew (&m_wGrpIDSP,					pDataTmp, sizeof (m_wGrpIDSP),					pDataTmp);	/* �摜ID(���ꕞ) */
	CopyMemoryRenew (&m_wGrpIDAcce,					pDataTmp, sizeof (m_wGrpIDAcce),				pDataTmp);	/* �摜ID(�A�N�Z�T��) */
	CopyMemoryRenew (&m_wGrpIDArmsMain,				pDataTmp, sizeof (m_wGrpIDArmsMain),			pDataTmp);	/* �摜ID(������:���C��) */
	CopyMemoryRenew (&m_wGrpIDArmsSub,				pDataTmp, sizeof (m_wGrpIDArmsSub),				pDataTmp);	/* �摜ID(������:�T�u) */
	CopyMemoryRenew (&m_wGrpIDArmsLeftMain,			pDataTmp, sizeof (m_wGrpIDArmsLeftMain),		pDataTmp);	/* �摜ID(��:���C��) */
	CopyMemoryRenew (&m_wGrpIDArmsLeftSub,			pDataTmp, sizeof (m_wGrpIDArmsLeftSub),			pDataTmp);	/* �摜ID(��:�T�u) */
	CopyMemoryRenew (&m_wGrpIDTmpMain,				pDataTmp, sizeof (m_wGrpIDTmpMain),				pDataTmp);	/* �摜ID(�ꎞ��:���C��) */
	CopyMemoryRenew (&m_wGrpIDTmpSub,				pDataTmp, sizeof (m_wGrpIDTmpSub),				pDataTmp);	/* �摜ID(�ꎞ��:�T�u) */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_GRP::SetParam										 */
/* ���e		:�w��L�������փp�����[�^�𔽉f								 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

void CPacketCHAR_GRP::SetParam(CInfoCharBase *pInfo)
{
	if (pInfo == NULL) {
		return;
	}

	pInfo->m_dwEquipItemIDCloth		= m_dwEquipItemIDCloth;		/* �����A�C�e��ID(��) */
	pInfo->m_dwEquipItemIDAcce1		= m_dwEquipItemIDAcce1;		/* �����A�C�e��ID(�A�N�Z�T��) */
	pInfo->m_dwEquipItemIDArmsRight	= m_dwEquipItemIDArmsRight;	/* �����A�C�e��ID(������) */
	pInfo->m_dwEquipItemIDArmsLeft	= m_dwEquipItemIDArmsLeft;	/* �����A�C�e��ID:���� */
	pInfo->m_wGrpIDNPC				= m_wGrpIDNPC;				/* �摜ID(NPC) */
	pInfo->m_wGrpIDCloth			= m_wGrpIDCloth;			/* �摜ID(��) */
	pInfo->m_wGrpIDEye				= m_wGrpIDEye;				/* �摜ID(��) */
	pInfo->m_wGrpIDEyeColor			= m_wGrpIDEyeColor;			/* �摜ID(�ڐF) */
	pInfo->m_wGrpIDHairType			= m_wGrpIDHairType;			/* �摜ID(��) */
	pInfo->m_wGrpIDHairColor		= m_wGrpIDHairColor;		/* �摜ID(���F) */
	pInfo->m_wGrpIDSP				= m_wGrpIDSP;				/* �摜ID(���ꕞ) */
	pInfo->m_wGrpIDAcce				= m_wGrpIDAcce;				/* �摜ID(�A�N�Z�T��) */
	pInfo->m_wGrpIDArmsMain			= m_wGrpIDArmsMain;			/* �摜ID(������:���C��) */
	pInfo->m_wGrpIDArmsSub			= m_wGrpIDArmsSub;			/* �摜ID(������:�T�u) */
	pInfo->m_wGrpIDArmsLeftMain		= m_wGrpIDArmsLeftMain;		/* �摜ID(��:���C��) */
	pInfo->m_wGrpIDArmsLeftSub		= m_wGrpIDArmsLeftSub;		/* �摜ID(��:�T�u) */
	pInfo->m_wGrpIDTmpMain			= m_wGrpIDTmpMain;			/* �摜ID(�ꎞ��:���C��) */
	pInfo->m_wGrpIDTmpSub			= m_wGrpIDTmpSub;			/* �摜ID(�ꎞ��:�T�u) */
}

/* Copyright(C)URARA-works 2007 */
