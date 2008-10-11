/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_STATUS.cpp										 */
/* ���e			:�R�}���h(�L�����n:�X�e�[�^�X���ʒm) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/29													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketCHAR_STATUS.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_STATUS::CPacketCHAR_STATUS							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/29														 */
/* ========================================================================= */

CPacketCHAR_STATUS::CPacketCHAR_STATUS()
{
	m_byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	m_byCmdSub	= SBOCOMMANDID_SUB_CHAR_STATUS;

	m_wAtackGauge			= 0;	/* �A�^�b�N�Q�[�W */
	m_wDefenseGauge			= 0;	/* �f�B�t�F���X�Q�[�W */
	m_wStamina				= 0;	/* �X�^�~�i */
	m_wPower				= 0;	/* �r�� */
	m_wStrength				= 0;	/* �̗� */
	m_wMagic				= 0;	/* ���� */
	m_wSkillful				= 0;	/* ��p */
	m_wAbillityAT			= 0;	/* �U���Z�\ */
	m_wAbillityDF			= 0;	/* �h��Z�\ */
	m_wPAtack				= 0;	/* �U���� */
	m_wPDefense				= 0;	/* �h��� */
	m_wPMagic				= 0;	/* ���@�� */
	m_wPMagicDefense		= 0;	/* ���@�h��� */
	m_wPHitAverage			= 0;	/* ������ */
	m_wPAvoidAverage		= 0;	/* ��� */
	m_wPCriticalAverage		= 0;	/* �N���e�B�J���� */
	m_wAttrFire				= 0;	/* ����[��] */
	m_wAttrWind				= 0;	/* ����[��] */
	m_wAttrWater			= 0;	/* ����[��] */
	m_wAttrEarth			= 0;	/* ����[�y] */
	m_wAttrLight			= 0;	/* ����[��] */
	m_wAttrDark				= 0;	/* ����[��] */
	m_dwCharID				= 0;	/* �L����ID */
	m_dwMoveWait			= 0;	/* �ړ��҂����� */
	m_dwMoveWaitBattle		= 0;	/* �퓬���ړ��҂����� */
	m_dwHP					= 0;	/* HP */
	m_dwMaxHP				= 0;	/* �ő�HP */
	m_dwSP					= 0;	/* SP */
	m_dwMaxSP				= 0;	/* �ő�SP */
	m_nDropItemAverage		= 0;	/* �A�C�e���h���b�v�� */
	m_nMoveAverage			= 0;	/* �ړ��m�� */
	m_nMoveAverageBattle	= 0;	/* �퓬���ړ��m�� */
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_STATUS::~CPacketCHAR_STATUS						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/29														 */
/* ========================================================================= */

CPacketCHAR_STATUS::~CPacketCHAR_STATUS()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_STATUS::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/06/29														 */
/* ========================================================================= */

void CPacketCHAR_STATUS::Make(
	CInfoCharBase *pInfo)		/* [in] �L������� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)				+
			 sizeof (pInfo->m_dwCharID)			+
			 sizeof (m_wAtackGauge)				+	/* �A�^�b�N�Q�[�W */
			 sizeof (m_wDefenseGauge)			+	/* �f�B�t�F���X�Q�[�W */
			 sizeof (m_wStamina)				+	/* �X�^�~�i */
			 sizeof (m_wPower)					+	/* �r�� */
			 sizeof (m_wStrength)				+	/* �̗� */
			 sizeof (m_wMagic)					+	/* ���� */
			 sizeof (m_wSkillful)				+	/* ��p */
			 sizeof (m_wAbillityAT)				+	/* �U���Z�\ */
			 sizeof (m_wAbillityDF)				+	/* �h��Z�\ */
			 sizeof (m_wPAtack)					+	/* �U���� */
			 sizeof (m_wPDefense)				+	/* �h��� */
			 sizeof (m_wPMagic)					+	/* ���@�� */
			 sizeof (m_wPMagicDefense)			+	/* ���@�h��� */
			 sizeof (m_wPHitAverage)			+	/* ������ */
			 sizeof (m_wPAvoidAverage)			+	/* ��� */
			 sizeof (m_wPCriticalAverage)		+	/* �N���e�B�J���� */
			 sizeof (m_wAttrFire)				+	/* ����[��] */
			 sizeof (m_wAttrWind)				+	/* ����[��] */
			 sizeof (m_wAttrWater)				+	/* ����[��] */
			 sizeof (m_wAttrEarth)				+	/* ����[�y] */
			 sizeof (m_wAttrLight)				+	/* ����[��] */
			 sizeof (m_wAttrDark)				+	/* ����[��] */
			 sizeof (pInfo->m_dwMoveWait)		+
			 sizeof (pInfo->m_dwMoveWaitBattle)	+
			 sizeof (pInfo->m_dwHP)				+
			 sizeof (pInfo->m_dwMaxHP)			+
			 sizeof (pInfo->m_dwSP)				+
			 sizeof (pInfo->m_dwMaxSP)			+
			 sizeof (pInfo->m_nDropItemAverage)	+
			 sizeof (pInfo->m_nMoveAverage)		+
			 sizeof (pInfo->m_nMoveAverageBattle);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= m_byCmdMain;
	pPacketBase->byCmdSub	= m_byCmdSub;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwCharID,				sizeof (m_dwCharID),			pDataTmp);	/* �L����ID */
//	CopyMemoryRenew (pDataTmp, &pInfo->m_wAtackGauge,			sizeof (m_wAtackGauge),			pDataTmp);	/* �A�^�b�N�Q�[�W */
//	CopyMemoryRenew (pDataTmp, &pInfo->m_wDefenseGauge,			sizeof (m_wDefenseGauge),		pDataTmp);	/* �f�B�t�F���X�Q�[�W */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wStamina,				sizeof (m_wStamina),			pDataTmp);	/* �X�^�~�i */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPower,				sizeof (m_wPower),				pDataTmp);	/* �r�� */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wStrength,				sizeof (m_wStrength),			pDataTmp);	/* �̗� */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wMagic,				sizeof (m_wMagic),				pDataTmp);	/* ���� */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wSkillful,				sizeof (m_wSkillful),			pDataTmp);	/* ��p */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAbillityAT,			sizeof (m_wAbillityAT),			pDataTmp);	/* �U���Z�\ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAbillityDF,			sizeof (m_wAbillityDF),			pDataTmp);	/* �h��Z�\ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPAtack,				sizeof (m_wPAtack),				pDataTmp);	/* �U���� */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPDefense,				sizeof (m_wPDefense),			pDataTmp);	/* �h��� */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPMagic,				sizeof (m_wPMagic),				pDataTmp);	/* ���@�� */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPMagicDefense,		sizeof (m_wPMagicDefense),		pDataTmp);	/* ���@�h��� */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPHitAverage,			sizeof (m_wPHitAverage),		pDataTmp);	/* ������ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPAvoidAverage,		sizeof (m_wPAvoidAverage),		pDataTmp);	/* ��� */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPCriticalAverage,		sizeof (m_wPCriticalAverage),	pDataTmp);	/* �N���e�B�J���� */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrFire,				sizeof (m_wAttrFire),			pDataTmp);	/* ����[��] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrWind,				sizeof (m_wAttrWind),			pDataTmp);	/* ����[��] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrWater,			sizeof (m_wAttrWater),			pDataTmp);	/* ����[��] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrEarth,			sizeof (m_wAttrEarth),			pDataTmp);	/* ����[�y] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrLight,			sizeof (m_wAttrLight),			pDataTmp);	/* ����[��] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrDark,				sizeof (m_wAttrDark),			pDataTmp);	/* ����[��] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwMoveWait,			sizeof (m_dwMoveWait),			pDataTmp);	/* �ړ��҂����� */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwMoveWaitBattle,		sizeof (m_dwMoveWaitBattle),	pDataTmp);	/* �퓬���ړ��҂����� */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwHP,					sizeof (m_dwHP),				pDataTmp);	/* HP */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwMaxHP,				sizeof (m_dwMaxHP),				pDataTmp);	/* �ő�HP */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwSP,					sizeof (m_dwSP),				pDataTmp);	/* SP */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwMaxSP,				sizeof (m_dwMaxSP),				pDataTmp);	/* �ő�SP */
	CopyMemoryRenew (pDataTmp, &pInfo->m_nDropItemAverage,		sizeof (m_nDropItemAverage),	pDataTmp);	/* �A�C�e���h���b�v�� */
	CopyMemoryRenew (pDataTmp, &pInfo->m_nMoveAverage,			sizeof (m_nMoveAverage),		pDataTmp);	/* �ړ��m�� */
	CopyMemoryRenew (pDataTmp, &pInfo->m_nMoveAverageBattle,	sizeof (m_nMoveAverageBattle),	pDataTmp);	/* �퓬���ړ��m�� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_STATUS::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/06/29														 */
/* ========================================================================= */

PBYTE CPacketCHAR_STATUS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,				pDataTmp, sizeof (m_dwCharID),				pDataTmp);	/* �L����ID */
//	CopyMemoryRenew (&m_wAtackGauge,			pDataTmp, sizeof (m_wAtackGauge),			pDataTmp);	/* �A�^�b�N�Q�[�W */
//	CopyMemoryRenew (&m_wDefenseGauge,			pDataTmp, sizeof (m_wDefenseGauge),			pDataTmp);	/* �f�B�t�F���X�Q�[�W */
	CopyMemoryRenew (&m_wStamina,				pDataTmp, sizeof (m_wStamina),				pDataTmp);	/* �X�^�~�i */
	CopyMemoryRenew (&m_wPower,					pDataTmp, sizeof (m_wPower),				pDataTmp);	/* �r�� */
	CopyMemoryRenew (&m_wStrength,				pDataTmp, sizeof (m_wStrength),				pDataTmp);	/* �̗� */
	CopyMemoryRenew (&m_wMagic,					pDataTmp, sizeof (m_wMagic),				pDataTmp);	/* ���� */
	CopyMemoryRenew (&m_wSkillful,				pDataTmp, sizeof (m_wSkillful),				pDataTmp);	/* ��p */
	CopyMemoryRenew (&m_wAbillityAT,			pDataTmp, sizeof (m_wAbillityAT),			pDataTmp);	/* �U���Z�\ */
	CopyMemoryRenew (&m_wAbillityDF,			pDataTmp, sizeof (m_wAbillityDF),			pDataTmp);	/* �h��Z�\ */
	CopyMemoryRenew (&m_wPAtack,				pDataTmp, sizeof (m_wPAtack),				pDataTmp);	/* �U���� */
	CopyMemoryRenew (&m_wPDefense,				pDataTmp, sizeof (m_wPDefense),				pDataTmp);	/* �h��� */
	CopyMemoryRenew (&m_wPMagic,				pDataTmp, sizeof (m_wPMagic),				pDataTmp);	/* ���@�� */
	CopyMemoryRenew (&m_wPMagicDefense,			pDataTmp, sizeof (m_wPMagicDefense),		pDataTmp);	/* ���@�h��� */
	CopyMemoryRenew (&m_wPHitAverage,			pDataTmp, sizeof (m_wPHitAverage),			pDataTmp);	/* ������ */
	CopyMemoryRenew (&m_wPAvoidAverage,			pDataTmp, sizeof (m_wPAvoidAverage),		pDataTmp);	/* ��� */
	CopyMemoryRenew (&m_wPCriticalAverage,		pDataTmp, sizeof (m_wPCriticalAverage),		pDataTmp);	/* �N���e�B�J���� */
	CopyMemoryRenew (&m_wAttrFire,				pDataTmp, sizeof (m_wAttrFire),				pDataTmp);	/* ����[��] */
	CopyMemoryRenew (&m_wAttrWind,				pDataTmp, sizeof (m_wAttrWind),				pDataTmp);	/* ����[��] */
	CopyMemoryRenew (&m_wAttrWater,				pDataTmp, sizeof (m_wAttrWater),			pDataTmp);	/* ����[��] */
	CopyMemoryRenew (&m_wAttrEarth,				pDataTmp, sizeof (m_wAttrEarth),			pDataTmp);	/* ����[�y] */
	CopyMemoryRenew (&m_wAttrLight,				pDataTmp, sizeof (m_wAttrLight),			pDataTmp);	/* ����[��] */
	CopyMemoryRenew (&m_wAttrDark,				pDataTmp, sizeof (m_wAttrDark),				pDataTmp);	/* ����[��] */
	CopyMemoryRenew (&m_dwMoveWait,				pDataTmp, sizeof (m_dwMoveWait),			pDataTmp);	/* �ړ��҂����� */
	CopyMemoryRenew (&m_dwMoveWaitBattle,		pDataTmp, sizeof (m_dwMoveWaitBattle),		pDataTmp);	/* �퓬���ړ��҂����� */
	CopyMemoryRenew (&m_dwHP,					pDataTmp, sizeof (m_dwHP),					pDataTmp);	/* HP */
	CopyMemoryRenew (&m_dwMaxHP,				pDataTmp, sizeof (m_dwMaxHP),				pDataTmp);	/* �ő�HP */
	CopyMemoryRenew (&m_dwSP,					pDataTmp, sizeof (m_dwSP),					pDataTmp);	/* SP */
	CopyMemoryRenew (&m_dwMaxSP,				pDataTmp, sizeof (m_dwMaxSP),				pDataTmp);	/* �ő�SP */
	CopyMemoryRenew (&m_nDropItemAverage,		pDataTmp, sizeof (m_nDropItemAverage),		pDataTmp);	/* �A�C�e���h���b�v�� */
	CopyMemoryRenew (&m_nMoveAverage,			pDataTmp, sizeof (m_nMoveAverage),			pDataTmp);	/* �ړ��m�� */
	CopyMemoryRenew (&m_nMoveAverageBattle,		pDataTmp, sizeof (m_nMoveAverageBattle),	pDataTmp);	/* �퓬���ړ��m�� */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_STATUS::SetParam									 */
/* ���e		:�w��L�������փp�����[�^�𔽉f								 */
/* ���t		:2008/06/29														 */
/* ========================================================================= */

void CPacketCHAR_STATUS::SetParam(CInfoCharBase *pInfo)
{
	if (pInfo == NULL) {
		return;
	}

//	pInfo->m_wAtackGauge		= m_wAtackGauge;		/* �A�^�b�N�Q�[�W */
//	pInfo->m_wDefenseGauge		= m_wDefenseGauge;		/* �f�B�t�F���X�Q�[�W */
	pInfo->m_wStamina			= m_wStamina;			/* �X�^�~�i */
	pInfo->m_wPower				= m_wPower;				/* �r�� */
	pInfo->m_wStrength			= m_wStrength;			/* �̗� */
	pInfo->m_wMagic				= m_wMagic;				/* ���� */
	pInfo->m_wSkillful			= m_wSkillful;			/* ��p */
	pInfo->m_wAbillityAT		= m_wAbillityAT;		/* �U���Z�\ */
	pInfo->m_wAbillityDF		= m_wAbillityDF;		/* �h��Z�\ */
	pInfo->m_wPAtack			= m_wPAtack;			/* �U���� */
	pInfo->m_wPDefense			= m_wPDefense;			/* �h��� */
	pInfo->m_wPMagic			= m_wPMagic;			/* ���@�� */
	pInfo->m_wPMagicDefense		= m_wPMagicDefense;		/* ���@�h��� */
	pInfo->m_wPHitAverage		= m_wPHitAverage;		/* ������ */
	pInfo->m_wPAvoidAverage		= m_wPAvoidAverage;		/* ��� */
	pInfo->m_wPCriticalAverage	= m_wPCriticalAverage;	/* �N���e�B�J���� */
	pInfo->m_wAttrFire			= m_wAttrFire;			/* ����[��] */
	pInfo->m_wAttrWind			= m_wAttrWind;			/* ����[��] */
	pInfo->m_wAttrWater			= m_wAttrWater;			/* ����[��] */
	pInfo->m_wAttrEarth			= m_wAttrEarth;			/* ����[�y] */
	pInfo->m_wAttrLight			= m_wAttrLight;			/* ����[��] */
	pInfo->m_wAttrDark			= m_wAttrDark;			/* ����[��] */
	pInfo->m_dwMoveWait			= m_dwMoveWait;			/* �ړ��҂����� */
	pInfo->m_dwMoveWaitBattle	= m_dwMoveWaitBattle;	/* �퓬���ړ��҂����� */
	pInfo->m_dwHP				= m_dwHP;				/* HP */
	pInfo->m_dwMaxHP			= m_dwMaxHP;			/* �ő�HP */
	pInfo->m_dwSP				= m_dwSP;				/* SP */
	pInfo->m_dwMaxSP			= m_dwMaxSP;			/* �ő�SP */
	pInfo->m_nDropItemAverage	= m_nDropItemAverage;	/* �A�C�e���h���b�v�� */
	pInfo->m_nMoveAverage		= m_nMoveAverage;		/* �ړ��m�� */
	pInfo->m_nMoveAverageBattle	= m_nMoveAverageBattle;	/* �퓬���ړ��m�� */
}

/* Copyright(C)URARA-works 2008 */
