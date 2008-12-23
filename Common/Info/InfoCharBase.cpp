/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharBase.cpp											 */
/* ���e			:�L���������N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoCharBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_nMapX",					/* �}�b�v���W(��) */
	"m_nMapY",					/* �}�b�v���W(�c) */
	"m_nMoveState",				/* �ړ���� */
	"m_nMoveType",				/* �ړ���� */
	"m_nDirection",				/* ���� */
	"m_nGrpSize",				/* �摜�T�C�Y */
	"m_nSex",					/* ���� */
	"m_nMaxItemCount",			/* �ő�A�C�e�������� */
	"m_nDropItemAverage",		/* �A�C�e���h���b�v�� */
	"m_nMoveAverage",			/* �ړ��m�� */
	"m_nMoveAverageBattle",		/* �퓬���ړ��m�� */
	"m_wFamilyID",				/* �푰ID */
	"m_wGrpIDNPC",				/* �摜ID(NPC) */
	"m_wGrpIDCloth",			/* �摜ID(��) */
	"m_wGrpIDEye",				/* �摜ID(��) */
	"m_wGrpIDEyeColor",			/* �摜ID(�ڐF) */
	"m_wGrpIDHairType",			/* �摜ID(��) */
	"m_wGrpIDHairColor",		/* �摜ID(���F) */
	"m_wGrpIDSP",				/* �摜ID(���ꕞ) */
	"m_wGrpIDTmpMain",			/* �摜ID(�ꎞ��:���C��) */
	"m_wGrpIDTmpSub",			/* �摜ID(�ꎞ��:�T�u) */
	"m_wGrpIDAcce",				/* �摜ID(�A�N�Z�T��) */
	"m_wGrpIDArmsMain",			/* �摜ID(������:���C��) */
	"m_wGrpIDArmsSub",			/* �摜ID(������:�T�u) */
	"m_wGrpIDArmsLeftMain",		/* �摜ID(��:���C��) */
	"m_wGrpIDArmsLeftSub",		/* �摜ID(��:�T�u) */
	"m_dwCharID",				/* �L����ID */
	"m_dwMapID",				/* �}�b�vID */
	"m_dwMotionTypeID",			/* ���[�V�������ID */
	"m_clName",					/* ���O�̕`��F */
	"m_clSpeak",				/* �����̕`��F */
	"m_strCharName",			/* �L������ */
	"m_strSpeak",				/* �������e */
	"m_strTalk",				/* ��b�f�[�^ */
	"m_adwItemID",				/* �����A�C�e�� */
	"m_adwSkillID",				/* �����X�L�� */
	"m_bBlock",					/* �Ԃ��锻�� */
	"m_bPush",					/* �����锻�� */
	"m_dwEquipItemIDCloth",		/* �����A�C�e��ID:�� */
	"m_dwEquipItemIDAcce1",		/* �����A�C�e��ID:�A�N�Z�T��1 */
	"m_dwEquipItemIDAcce2",		/* �����A�C�e��ID:�A�N�Z�T��2 */
	"m_dwEquipItemIDArmsRight",	/* �����A�C�e��ID:�E�� */
	"m_dwEquipItemIDArmsLeft",	/* �����A�C�e��ID:���� */
	"m_dwEquipItemIDHead",		/* �����A�C�e��ID:�� */
	"m_wGrpIDInitNPC",			/* �����摜ID(NPC) */
	"m_wGrpIDInitCloth",		/* �����摜ID(��) */
	"m_wGrpIDInitEye",			/* �����摜ID(��) */
	"m_wGrpIDInitEyeColor",		/* �����摜ID(�ڐF) */
	"m_wGrpIDInitHairType",		/* �����摜ID(��) */
	"m_wGrpIDInitHairColor",	/* �����摜ID(���F) */
	"m_wGrpIDInitSP",			/* �����摜ID(���ꕞ) */
	"m_wLevel",					/* ���x�� */
	"m_wStamina",				/* �X�^�~�i */
	"m_wPower",					/* �r�� */
	"m_wStrength",				/* �̗� */
	"m_wMagic",					/* ���� */
	"m_wSkillful",				/* ��p */
	"m_wAbillityAT",			/* �U���Z�\ */
	"m_wAbillityDF",			/* �h��Z�\ */
	"m_wPAtack",				/* �U���� */
	"m_wPDefense",				/* �h��� */
	"m_wPMagic",				/* ���@�� */
	"m_wPMagicDefense",			/* ���@�h��� */
	"m_wPHitAverage",			/* ������ */
	"m_wPAvoidAverage",			/* ��� */
	"m_wPCriticalAverage",		/* �N���e�B�J���� */
	"m_wAttrFire",				/* ����[��] */
	"m_wAttrWind",				/* ����[��] */
	"m_wAttrWater",				/* ����[��] */
	"m_wAttrEarth",				/* ����[�y] */
	"m_wAttrLight",				/* ����[��] */
	"m_wAttrDark",				/* ����[��] */
	"m_dwMoveWait",				/* �ړ��҂����� */
	"m_dwMoveWaitBattle",		/* �퓬���ړ��҂����� */
	"m_dwExp",					/* �o���l */
	"m_dwHP",					/* HP */
	"m_dwMaxHP",				/* �ő�HP */
	"m_dwSP",					/* SP */
	"m_dwMaxSP",				/* �ő�SP */
	/* NPC���� */
	"m_dwPutCycle",				/* �������� */
	"m_nPutMoveType",			/* ����������ړ���� */
	"m_nMaxPutCount",			/* ���������� */
	"m_nPutAverage",			/* �����m�� */
	"m_ptPutArea",				/* �����͈�(���a) */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoCharBase::CInfoCharBase									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

CInfoCharBase::CInfoCharBase()
{
	m_bDelete				= FALSE;
	m_bBlock				= FALSE;
	m_bPush					= FALSE;
	m_nAnime				= 0;
	m_nDirectionBack		= 0;
	m_nMapX					= 1;
	m_nMapY					= 1;
	m_nMoveState			= CHARMOVESTATE_STAND;
	m_nProcState			= CHARPROCSTATEID_NORMAL;
	m_nMoveDirection		= -1;
	m_nLightLevel			= 0;				/* ���背�x�� */
	m_nMoveType				= CHARMOVETYPE_PC;
	m_nDirection			= 1;
	m_nGrpSize				= 16;
	m_nSex					= SEX_MALE;
	m_nMaxItemCount			= 25;
	m_nDropItemAverage		= 0;
	m_nMoveAverage			= 0;				/* �ړ��m�� */
	m_nMoveAverageBattle	= 0;				/* �퓬���ړ��m�� */
	m_bNPC					= FALSE;
	m_bChargeAtack			= FALSE;			/* ���ߍU�� */
	m_wFamilyID				= FAMILYTYPE_HUMAN;
	m_wGrpIDNPC				= 0;
	m_wGrpIDCloth			= 0;
	m_wGrpIDEye				= 1;
	m_wGrpIDEyeColor		= 1;
	m_wGrpIDHairType		= 1;
	m_wGrpIDHairColor		= 1;
	m_wGrpIDSP				= 0;
	m_wGrpIDTmpMain			= 0;
	m_wGrpIDTmpSub			= 0;
	m_wGrpIDAcce			= 0;
	m_wGrpIDArmsMain		= 0;
	m_wGrpIDArmsSub			= 0;
	m_wGrpIDArmsLeftMain	= 0;
	m_wGrpIDArmsLeftSub		= 0;
	m_wGrpIDInitNPC			= m_wGrpIDNPC;
	m_wGrpIDInitCloth		= m_wGrpIDCloth;
	m_wGrpIDInitEye			= m_wGrpIDEye;
	m_wGrpIDInitEyeColor	= m_wGrpIDEyeColor;
	m_wGrpIDInitHairType	= m_wGrpIDHairType;
	m_wGrpIDInitHairColor	= m_wGrpIDHairColor;
	m_wGrpIDInitSP			= m_wGrpIDSP;
	m_wAtackGauge			= 0;					/* �A�^�b�N�Q�[�W */
	m_wDefenseGauge			= 0;					/* �f�B�t�F���X�Q�[�W */
	m_wLevel				= 1;					/* ���x�� */
	m_wStamina				= 0;					/* �X�^�~�i */
	m_wPower				= 0;					/* �r�� */
	m_wStrength				= 0;					/* �̗� */
	m_wMagic				= 0;					/* ���� */
	m_wSkillful				= 0;					/* ��p */
	m_wAbillityAT			= 0;					/* �U���Z�\ */
	m_wAbillityDF			= 0;					/* �h��Z�\ */
	m_wPAtack				= 0;					/* �U���� */
	m_wPDefense				= 0;					/* �h��� */
	m_wPMagic				= 0;					/* ���@�� */
	m_wPMagicDefense		= 0;					/* ���@�h��� */
	m_wPHitAverage			= 0;					/* ������ */
	m_wPAvoidAverage		= 0;					/* ��� */
	m_wPCriticalAverage		= 0;					/* �N���e�B�J���� */
	m_wAttrFire				= 0;					/* ����[��] */
	m_wAttrWind				= 0;					/* ����[��] */
	m_wAttrWater			= 0;					/* ����[��] */
	m_wAttrEarth			= 0;					/* ����[�y] */
	m_wAttrLight			= 0;					/* ����[��] */
	m_wAttrDark				= 0;					/* ����[��] */
	m_dwCharID				= 0;
	m_dwMapID				= 1;
	m_dwMotionTypeID		= 1;
	m_dwSessionID			= 0;
	m_dwAccountID			= 0;
	m_dwLastTimeSpeak		= 0;
	m_dwLastTimeMove		= 0;
	m_dwTailCharID			= 0;
	m_dwFrontCharID			= 0;
	m_dwParentCharID		= 0;
	m_dwTargetCharID		= 0;
	m_dwLightTime			= 0;					/* ����L������ */

	m_dwEquipItemIDCloth		= 0;
	m_dwEquipItemIDAcce1		= 0;
	m_dwEquipItemIDAcce2		= 0;
	m_dwEquipItemIDArmsRight	= 0;
	m_dwEquipItemIDArmsLeft		= 0;
	m_dwEquipItemIDHead			= 0;
	m_dwMoveWait				= MOVEWAIT;
	m_dwMoveWaitBattle			= 0;
	m_dwExp		= 0;
	m_dwHP		= 10;
	m_dwMaxHP	= 10;
	m_dwSP		= 0;
	m_dwMaxSP	= 0;

	ZeroMemory (m_adwMotionID, sizeof (m_adwMotionID));
	m_adwMotionID[CHARMOTIONID_STAND]		= CHARMOTIONLISTID_STAND_UP;		/* ���[�V����ID(����) */
	m_adwMotionID[CHARMOTIONID_WALK]		= CHARMOTIONLISTID_WALK_UP;			/* ���[�V����ID(����) */
	m_adwMotionID[CHARMOTIONID_SIT]			= CHARMOTIONLISTID_SIT_UP;			/* ���[�V����ID(����) */
	m_adwMotionID[CHARMOTIONID_BATTLESTAND]	= CHARMOTIONLISTID_BATTLESTAND_UP;	/* ���[�V����ID(�퓬����) */
	m_adwMotionID[CHARMOTIONID_BATTLEWALK]	= CHARMOTIONLISTID_BATTLEWALK_UP;	/* ���[�V����ID(���葫) */
	m_adwMotionID[CHARMOTIONID_ATACK]		= CHARMOTIONLISTID_SWING_UP;		/* ���[�V����ID(�U��) */

	m_clName	= RGB (255, 255, 255);
	m_clSpeak	= RGB (255, 255, 255);
	m_ptCharBack.x = m_ptCharBack.y = 0;
	m_ptViewCharPos.x = m_ptViewCharPos.y = 0;

	/* NPC���� */
	m_dwPutCycle		= 0;				/* �������� */
	m_nPutMoveType		= 0;				/* ����������ړ���� */
	m_nMaxPutCount		= 0;				/* ���������� */
	m_nPutAverage		= 0;				/* �����m�� */
	m_ptPutArea.x = m_ptPutArea.y = 0;		/* �����͈�(���a) */

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::~CInfoCharBase									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

CInfoCharBase::~CInfoCharBase()
{
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetElementNo									 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

int CInfoCharBase::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = -1;

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}

	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetDataSize										 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

DWORD CInfoCharBase::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_bBlock);
	dwRet += sizeof (m_bPush);
	dwRet += sizeof (m_nMapX);
	dwRet += sizeof (m_nMapY);
	dwRet += sizeof (m_nMoveState);
	dwRet += sizeof (m_nMoveType);
	dwRet += sizeof (m_nDirection);
	dwRet += sizeof (m_nGrpSize);
	dwRet += sizeof (m_nSex);
	dwRet += sizeof (m_nMaxItemCount);
	dwRet += sizeof (m_nDropItemAverage);
	dwRet += sizeof (m_nMoveAverage),			/* �ړ��m�� */
	dwRet += sizeof (m_nMoveAverageBattle),		/* �퓬���ړ��m�� */
	dwRet += sizeof (m_wFamilyID);
	dwRet += sizeof (m_wGrpIDNPC);
	dwRet += sizeof (m_wGrpIDCloth);
	dwRet += sizeof (m_wGrpIDEye);
	dwRet += sizeof (m_wGrpIDEyeColor);
	dwRet += sizeof (m_wGrpIDHairType);
	dwRet += sizeof (m_wGrpIDHairColor);
	dwRet += sizeof (m_wGrpIDSP);
	dwRet += sizeof (m_wGrpIDTmpMain);
	dwRet += sizeof (m_wGrpIDTmpSub);
	dwRet += sizeof (m_wGrpIDAcce);
	dwRet += sizeof (m_wGrpIDArmsMain);
	dwRet += sizeof (m_wGrpIDArmsSub);
	dwRet += sizeof (m_wGrpIDArmsLeftMain);
	dwRet += sizeof (m_wGrpIDArmsLeftSub);
	dwRet += sizeof (m_wGrpIDInitNPC);
	dwRet += sizeof (m_wGrpIDInitCloth);
	dwRet += sizeof (m_wGrpIDInitEye);
	dwRet += sizeof (m_wGrpIDInitEyeColor);
	dwRet += sizeof (m_wGrpIDInitHairType);
	dwRet += sizeof (m_wGrpIDInitHairColor);
	dwRet += sizeof (m_wGrpIDInitSP);
	dwRet += sizeof (m_wLevel);					/* ���x�� */
	dwRet += sizeof (m_wStamina);				/* �X�^�~�i */
	dwRet += sizeof (m_wPower);					/* �r�� */
	dwRet += sizeof (m_wStrength);				/* �̗� */
	dwRet += sizeof (m_wMagic);					/* ���� */
	dwRet += sizeof (m_wSkillful);				/* ��p */
	dwRet += sizeof (m_wAbillityAT);			/* �U���Z�\ */
	dwRet += sizeof (m_wAbillityDF);			/* �h��Z�\ */
	dwRet += sizeof (m_wPAtack);				/* �U���� */
	dwRet += sizeof (m_wPDefense);				/* �h��� */
	dwRet += sizeof (m_wPMagic);				/* ���@�� */
	dwRet += sizeof (m_wPMagicDefense);			/* ���@�h��� */
	dwRet += sizeof (m_wPHitAverage);			/* ������ */
	dwRet += sizeof (m_wPAvoidAverage);			/* ��� */
	dwRet += sizeof (m_wPCriticalAverage);		/* �N���e�B�J���� */
	dwRet += sizeof (m_wAttrFire);				/* ����[��] */
	dwRet += sizeof (m_wAttrWind);				/* ����[��] */
	dwRet += sizeof (m_wAttrWater);				/* ����[��] */
	dwRet += sizeof (m_wAttrEarth);				/* ����[�y] */
	dwRet += sizeof (m_wAttrLight);				/* ����[��] */
	dwRet += sizeof (m_wAttrDark);				/* ����[��] */
	dwRet += sizeof (m_dwCharID);
	dwRet += sizeof (m_dwMapID);
	dwRet += sizeof (m_dwMotionTypeID);
	dwRet += sizeof (m_dwEquipItemIDCloth);
	dwRet += sizeof (m_dwEquipItemIDAcce1);
	dwRet += sizeof (m_dwEquipItemIDAcce2);
	dwRet += sizeof (m_dwEquipItemIDArmsRight);
	dwRet += sizeof (m_dwEquipItemIDArmsLeft);
	dwRet += sizeof (m_dwEquipItemIDHead);
	dwRet += sizeof (m_dwMoveWait);				/* �ړ��҂����� */
	dwRet += sizeof (m_dwMoveWaitBattle);		/* �퓬���ړ��҂����� */
	dwRet += sizeof (m_dwExp);					/* �o���l */
	dwRet += sizeof (m_dwHP);
	dwRet += sizeof (m_dwMaxHP);
	dwRet += sizeof (m_dwSP);
	dwRet += sizeof (m_dwMaxSP);
	dwRet += sizeof (m_clName);
	dwRet += sizeof (m_clSpeak);
	dwRet += (m_strCharName.GetLength () + 1);
	dwRet += (m_strSpeak.GetLength () + 1);
	dwRet += (m_strTalk.GetLength () + 1);
	dwRet += ((m_adwItemID.GetSize () + 1) * sizeof (DWORD));
	dwRet += ((m_adwSkillID.GetSize () + 1) * sizeof (DWORD));
	/* NPC���� */
	dwRet += sizeof (m_dwPutCycle);				/* �������� */
	dwRet += sizeof (m_nPutMoveType);			/* ����������ړ���� */
	dwRet += sizeof (m_nMaxPutCount);			/* ���������� */
	dwRet += sizeof (m_nPutAverage);			/* �����m�� */
	dwRet += sizeof (m_ptPutArea);				/* �����͈�(���a) */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetDataSizeNo									 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

DWORD CInfoCharBase::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_nMapX);					break;
	case 1:		dwRet = sizeof (m_nMapY);					break;
	case 2:		dwRet = sizeof (m_nMoveState);				break;
	case 3:		dwRet = sizeof (m_nMoveType);				break;
	case 4:		dwRet = sizeof (m_nDirection);				break;
	case 5:		dwRet = sizeof (m_nGrpSize);				break;
	case 6:		dwRet = sizeof (m_nSex);					break;
	case 7:		dwRet = sizeof (m_nMaxItemCount);			break;
	case 8:		dwRet = sizeof (m_nDropItemAverage);		break;		/* �A�C�e���h���b�v�� */
	case 9:		dwRet = sizeof (m_nMoveAverage);			break;		/* �ړ��m�� */
	case 10:	dwRet = sizeof (m_nMoveAverageBattle);		break;		/* �퓬���ړ��m�� */
	case 11:	dwRet = sizeof (m_wFamilyID);				break;
	case 12:	dwRet = sizeof (m_wGrpIDNPC);				break;
	case 13:	dwRet = sizeof (m_wGrpIDCloth);				break;
	case 14:	dwRet = sizeof (m_wGrpIDEye);				break;
	case 15:	dwRet = sizeof (m_wGrpIDEyeColor);			break;
	case 16:	dwRet = sizeof (m_wGrpIDHairType);			break;
	case 17:	dwRet = sizeof (m_wGrpIDHairColor);			break;
	case 18:	dwRet = sizeof (m_wGrpIDSP);				break;
	case 19:	dwRet = sizeof (m_wGrpIDTmpMain);			break;		/* �摜ID(�ꎞ��:���C��) */
	case 20:	dwRet = sizeof (m_wGrpIDTmpSub);			break;		/* �摜ID(�ꎞ��:�T�u) */
	case 21:	dwRet = sizeof (m_wGrpIDAcce);				break;
	case 22:	dwRet = sizeof (m_wGrpIDArmsMain);			break;
	case 23:	dwRet = sizeof (m_wGrpIDArmsSub);			break;
	case 24:	dwRet = sizeof (m_wGrpIDArmsLeftMain);		break;
	case 25:	dwRet = sizeof (m_wGrpIDArmsLeftSub);		break;
	case 26:	dwRet = sizeof (m_dwCharID);				break;
	case 27:	dwRet = sizeof (m_dwMapID);					break;
	case 28:	dwRet = sizeof (m_dwMotionTypeID);			break;
	case 29:	dwRet = sizeof (m_clName);					break;
	case 30:	dwRet = sizeof (m_clSpeak);					break;
	case 31:	dwRet = (m_strCharName.	GetLength () + 1);	break;
	case 32:	dwRet = (m_strSpeak.	GetLength () + 1);	break;
	case 33:	dwRet = (m_strTalk.		GetLength () + 1);	break;
	case 34:	dwRet = ((m_adwItemID.GetSize () + 1) * sizeof (DWORD));	break;
	case 35:	dwRet = ((m_adwSkillID.GetSize () + 1) * sizeof (DWORD));	break;
	case 36:	dwRet = sizeof (m_bBlock);					break;
	case 37:	dwRet = sizeof (m_bPush);					break;
	case 38:	dwRet = sizeof (m_dwEquipItemIDCloth);		break;
	case 39:	dwRet = sizeof (m_dwEquipItemIDAcce1);		break;
	case 40:	dwRet = sizeof (m_dwEquipItemIDAcce2);		break;
	case 41:	dwRet = sizeof (m_dwEquipItemIDArmsRight);	break;
	case 42:	dwRet = sizeof (m_dwEquipItemIDArmsLeft);	break;
	case 43:	dwRet = sizeof (m_dwEquipItemIDHead);		break;
	case 44:	dwRet = sizeof (m_wGrpIDInitNPC);			break;
	case 45:	dwRet = sizeof (m_wGrpIDInitCloth);			break;
	case 46:	dwRet = sizeof (m_wGrpIDInitEye);			break;
	case 47:	dwRet = sizeof (m_wGrpIDInitEyeColor);		break;
	case 48:	dwRet = sizeof (m_wGrpIDInitHairType);		break;
	case 49:	dwRet = sizeof (m_wGrpIDInitHairColor);		break;
	case 50:	dwRet = sizeof (m_wGrpIDInitSP);			break;
	case 51:	dwRet = sizeof (m_wLevel);					break;	/* ���x�� */
	case 52:	dwRet = sizeof (m_wStamina);				break;	/* �X�^�~�i */
	case 53:	dwRet = sizeof (m_wPower);					break;	/* �r�� */
	case 54:	dwRet = sizeof (m_wStrength);				break;	/* �̗� */
	case 55:	dwRet = sizeof (m_wMagic);					break;	/* ���� */
	case 56:	dwRet = sizeof (m_wSkillful);				break;	/* ��p */
	case 57:	dwRet = sizeof (m_wAbillityAT);				break;	/* �U���Z�\ */
	case 58:	dwRet = sizeof (m_wAbillityDF);				break;	/* �h��Z�\ */
	case 59:	dwRet = sizeof (m_wPAtack);					break;	/* �U���� */
	case 60:	dwRet = sizeof (m_wPDefense);				break;	/* �h��� */
	case 61:	dwRet = sizeof (m_wPMagic);					break;	/* ���@�� */
	case 62:	dwRet = sizeof (m_wPMagicDefense);			break;	/* ���@�h��� */
	case 63:	dwRet = sizeof (m_wPHitAverage);			break;	/* ������ */
	case 64:	dwRet = sizeof (m_wPAvoidAverage);			break;	/* ��� */
	case 65:	dwRet = sizeof (m_wPCriticalAverage);		break;	/* �N���e�B�J���� */
	case 66:	dwRet = sizeof (m_wAttrFire);				break;	/* ����[��] */
	case 67:	dwRet = sizeof (m_wAttrWind);				break;	/* ����[��] */
	case 68:	dwRet = sizeof (m_wAttrWater);				break;	/* ����[��] */
	case 69:	dwRet = sizeof (m_wAttrEarth);				break;	/* ����[�y] */
	case 70:	dwRet = sizeof (m_wAttrLight);				break;	/* ����[��] */
	case 71:	dwRet = sizeof (m_wAttrDark);				break;	/* ����[��] */
	case 72:	dwRet = sizeof (m_dwMoveWait);				break;	/* �ړ��҂����� */
	case 73:	dwRet = sizeof (m_dwMoveWaitBattle);		break;	/* �퓬���ړ��҂����� */
	case 74:	dwRet = sizeof (m_dwExp);					break;	/* �o���l */
	case 75:	dwRet = sizeof (m_dwHP);					break;
	case 76:	dwRet = sizeof (m_dwMaxHP);					break;
	case 77:	dwRet = sizeof (m_dwSP);					break;
	case 78:	dwRet = sizeof (m_dwMaxSP);					break;
	/* NPC���� */
	case 79:	dwRet = sizeof (m_dwPutCycle);				break;	/* �������� */
	case 80:	dwRet = sizeof (m_nPutMoveType);			break;	/* ����������ړ���� */
	case 81:	dwRet = sizeof (m_nMaxPutCount);			break;	/* ���������� */
	case 82:	dwRet = sizeof (m_nPutAverage);				break;	/* �����m�� */
	case 83:	dwRet = sizeof (m_ptPutArea);				break;	/* �����͈�(���a) */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetName											 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

LPCSTR CInfoCharBase::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetWriteData									 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

PBYTE CInfoCharBase::GetWriteData(int nNo, PDWORD pdwSize)
{
	int i, nCount;
	PBYTE pRet, pSrc, pTmp;
	DWORD dwSize, dwTmp;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:		pSrc = (PBYTE)&m_nMapX;					break;
	case 1:		pSrc = (PBYTE)&m_nMapY;					break;
	case 2:		pSrc = (PBYTE)&m_nMoveState;			break;
	case 3:		pSrc = (PBYTE)&m_nMoveType;				break;
	case 4:		pSrc = (PBYTE)&m_nDirection;			break;
	case 5:		pSrc = (PBYTE)&m_nGrpSize;				break;
	case 6:		pSrc = (PBYTE)&m_nSex;					break;
	case 7:		pSrc = (PBYTE)&m_nMaxItemCount;			break;
	case 8:		pSrc = (PBYTE)&m_nDropItemAverage;		break;		/* �A�C�e���h���b�v�� */
	case 9:		pSrc = (PBYTE)&m_nMoveAverage;			break;		/* �ړ��m�� */
	case 10:	pSrc = (PBYTE)&m_nMoveAverageBattle;	break;		/* �擪���ړ��m�� */
	case 11:	pSrc = (PBYTE)&m_wFamilyID;				break;
	case 12:	pSrc = (PBYTE)&m_wGrpIDNPC;				break;
	case 13:	pSrc = (PBYTE)&m_wGrpIDCloth;			break;
	case 14:	pSrc = (PBYTE)&m_wGrpIDEye;				break;
	case 15:	pSrc = (PBYTE)&m_wGrpIDEyeColor;		break;
	case 16:	pSrc = (PBYTE)&m_wGrpIDHairType;		break;
	case 17:	pSrc = (PBYTE)&m_wGrpIDHairColor;		break;
	case 18:	pSrc = (PBYTE)&m_wGrpIDSP;				break;
	case 19:	pSrc = (PBYTE)&m_wGrpIDTmpMain;			break;		/* �摜ID(�ꎞ��:���C��) */
	case 20:	pSrc = (PBYTE)&m_wGrpIDTmpSub;			break;		/* �摜ID(�ꎞ��:�T�u) */
	case 21:	pSrc = (PBYTE)&m_wGrpIDAcce;			break;
	case 22:	pSrc = (PBYTE)&m_wGrpIDArmsMain;		break;
	case 23:	pSrc = (PBYTE)&m_wGrpIDArmsSub;			break;
	case 24:	pSrc = (PBYTE)&m_wGrpIDArmsLeftMain;	break;
	case 25:	pSrc = (PBYTE)&m_wGrpIDArmsLeftSub;		break;
	case 26:	pSrc = (PBYTE)&m_dwCharID;				break;
	case 27:	pSrc = (PBYTE)&m_dwMapID;				break;
	case 28:	pSrc = (PBYTE)&m_dwMotionTypeID;		break;
	case 29:	pSrc = (PBYTE)&m_clName;				break;
	case 30:	pSrc = (PBYTE)&m_clSpeak;				break;
	case 31:	pSrc = (PBYTE)(LPCSTR)m_strCharName;	break;
	case 32:	pSrc = (PBYTE)(LPCSTR)m_strSpeak;		break;
	case 33:	pSrc = (PBYTE)(LPCSTR)m_strTalk;		break;
	case 34:
		pTmp	= pRet;
		nCount	= m_adwItemID.GetSize ();

		for (i = 0; i < nCount; i ++) {
			dwTmp = m_adwItemID[i];
			CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		}
		dwTmp = 0;
		CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		break;
	case 35:
		pTmp	= pRet;
		nCount	= m_adwSkillID.GetSize ();

		for (i = 0; i < nCount; i ++) {
			dwTmp = m_adwSkillID[i];
			CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		}
		dwTmp = 0;
		CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		break;
	case 36:	pSrc = (PBYTE)&m_bBlock;					break;
	case 37:	pSrc = (PBYTE)&m_bPush;						break;
	case 38:	pSrc = (PBYTE)&m_dwEquipItemIDCloth;		break;
	case 39:	pSrc = (PBYTE)&m_dwEquipItemIDAcce1;		break;
	case 40:	pSrc = (PBYTE)&m_dwEquipItemIDAcce2;		break;
	case 41:	pSrc = (PBYTE)&m_dwEquipItemIDArmsRight;	break;
	case 42:	pSrc = (PBYTE)&m_dwEquipItemIDArmsLeft;		break;
	case 43:	pSrc = (PBYTE)&m_dwEquipItemIDHead;			break;
	case 44:	pSrc = (PBYTE)&m_wGrpIDInitNPC;				break;
	case 45:	pSrc = (PBYTE)&m_wGrpIDInitCloth;			break;
	case 46:	pSrc = (PBYTE)&m_wGrpIDInitEye;				break;
	case 47:	pSrc = (PBYTE)&m_wGrpIDInitEyeColor;		break;
	case 48:	pSrc = (PBYTE)&m_wGrpIDInitHairType;		break;
	case 49:	pSrc = (PBYTE)&m_wGrpIDInitHairColor;		break;
	case 50:	pSrc = (PBYTE)&m_wGrpIDInitSP;				break;
	case 51:	pSrc = (PBYTE)&m_wLevel;					break;	/* ���x�� */
	case 52:	pSrc = (PBYTE)&m_wStamina;					break;	/* �X�^�~�i */
	case 53:	pSrc = (PBYTE)&m_wPower;					break;	/* �r�� */
	case 54:	pSrc = (PBYTE)&m_wStrength;					break;	/* �̗� */
	case 55:	pSrc = (PBYTE)&m_wMagic;					break;	/* ���� */
	case 56:	pSrc = (PBYTE)&m_wSkillful;					break;	/* ��p */
	case 57:	pSrc = (PBYTE)&m_wAbillityAT;				break;	/* �U���Z�\ */
	case 58:	pSrc = (PBYTE)&m_wAbillityDF;				break;	/* �h��Z�\ */
	case 59:	pSrc = (PBYTE)&m_wPAtack;					break;	/* �U���� */
	case 60:	pSrc = (PBYTE)&m_wPDefense;					break;	/* �h��� */
	case 61:	pSrc = (PBYTE)&m_wPMagic;					break;	/* ���@�� */
	case 62:	pSrc = (PBYTE)&m_wPMagicDefense;			break;	/* ���@�h��� */
	case 63:	pSrc = (PBYTE)&m_wPHitAverage;				break;	/* ������ */
	case 64:	pSrc = (PBYTE)&m_wPAvoidAverage;			break;	/* ��� */
	case 65:	pSrc = (PBYTE)&m_wPCriticalAverage;			break;	/* �N���e�B�J���� */
	case 66:	pSrc = (PBYTE)&m_wAttrFire;					break;	/* ����[��] */
	case 67:	pSrc = (PBYTE)&m_wAttrWind;					break;	/* ����[��] */
	case 68:	pSrc = (PBYTE)&m_wAttrWater;				break;	/* ����[��] */
	case 69:	pSrc = (PBYTE)&m_wAttrEarth;				break;	/* ����[�y] */
	case 70:	pSrc = (PBYTE)&m_wAttrLight;				break;	/* ����[��] */
	case 71:	pSrc = (PBYTE)&m_wAttrDark;					break;	/* ����[��] */
	case 72:	pSrc = (PBYTE)&m_dwMoveWait;				break;	/* �ړ��҂����� */
	case 73:	pSrc = (PBYTE)&m_dwMoveWaitBattle;			break;	/* �擪���ړ��҂����� */
	case 74:	pSrc = (PBYTE)&m_dwExp;						break;	/* �o���l */
	case 75:	pSrc = (PBYTE)&m_dwHP;						break;
	case 76:	pSrc = (PBYTE)&m_dwMaxHP;					break;
	case 77:	pSrc = (PBYTE)&m_dwSP;						break;
	case 78:	pSrc = (PBYTE)&m_dwMaxSP;					break;
	/* NPC���� */
	case 79:	pSrc = (PBYTE)&m_dwPutCycle;				break;	/* �������� */
	case 80:	pSrc = (PBYTE)&m_nPutMoveType;				break;	/* ����������ړ���� */
	case 81:	pSrc = (PBYTE)&m_nMaxPutCount;				break;	/* ���������� */
	case 82:	pSrc = (PBYTE)&m_nPutAverage;				break;	/* �����m�� */
	case 83:	pSrc = (PBYTE)&m_ptPutArea;					break;	/* �����͈�(���a) */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::ReadElementData									 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

DWORD CInfoCharBase::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst, pTmp;
	DWORD dwSize, dwTmp;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_nMapX;					dwSize = sizeof (m_nMapX);				break;
	case 1:		pDst = (PBYTE)&m_nMapY;					dwSize = sizeof (m_nMapY);				break;
	case 2:												dwSize = sizeof (m_nMoveState);			break;
	case 3:		pDst = (PBYTE)&m_nMoveType;				dwSize = sizeof (m_nMoveType);			break;
	case 4:		pDst = (PBYTE)&m_nDirection;			dwSize = sizeof (m_nDirection);			break;
	case 5:		pDst = (PBYTE)&m_nGrpSize;				dwSize = sizeof (m_nGrpSize);			break;
	case 6:		pDst = (PBYTE)&m_nSex;					dwSize = sizeof (m_nSex);				break;
	case 7:		pDst = (PBYTE)&m_nMaxItemCount;			dwSize = sizeof (m_nMaxItemCount);		break;
	case 8:		pDst = (PBYTE)&m_nDropItemAverage;		dwSize = sizeof (m_nDropItemAverage);	break;	/* �A�C�e���h���b�v�� */
	case 9:		pDst = (PBYTE)&m_nMoveAverage;			dwSize = sizeof (m_nMoveAverage);		break;	/* �ړ��m�� */
	case 10:	pDst = (PBYTE)&m_nMoveAverageBattle;	dwSize = sizeof (m_nMoveAverageBattle);	break;	/* �퓬���ړ��m�� */
	case 11:	pDst = (PBYTE)&m_wFamilyID;				dwSize = sizeof (m_wFamilyID);			break;
	case 12:	pDst = (PBYTE)&m_wGrpIDNPC;				dwSize = sizeof (m_wGrpIDNPC);			break;
	case 13:	pDst = (PBYTE)&m_wGrpIDCloth;			dwSize = sizeof (m_wGrpIDCloth);		break;
	case 14:	pDst = (PBYTE)&m_wGrpIDEye;				dwSize = sizeof (m_wGrpIDEye);			break;
	case 15:	pDst = (PBYTE)&m_wGrpIDEyeColor;		dwSize = sizeof (m_wGrpIDEyeColor);		break;
	case 16:	pDst = (PBYTE)&m_wGrpIDHairType;		dwSize = sizeof (m_wGrpIDHairType);		break;
	case 17:	pDst = (PBYTE)&m_wGrpIDHairColor;		dwSize = sizeof (m_wGrpIDHairColor);	break;
	case 18:	pDst = (PBYTE)&m_wGrpIDSP;				dwSize = sizeof (m_wGrpIDSP);			break;
	case 19:	pDst = (PBYTE)&m_wGrpIDTmpMain;			dwSize = sizeof (m_wGrpIDTmpMain);		break;	/* �摜ID(�ꎞ��:���C��) */
	case 20:	pDst = (PBYTE)&m_wGrpIDTmpSub;			dwSize = sizeof (m_wGrpIDTmpSub);		break;	/* �摜ID(�ꎞ��:�T�u) */
	case 21:	pDst = (PBYTE)&m_wGrpIDAcce;			dwSize = sizeof (m_wGrpIDAcce);			break;
	case 22:	pDst = (PBYTE)&m_wGrpIDArmsMain;		dwSize = sizeof (m_wGrpIDArmsMain);		break;
	case 23:	pDst = (PBYTE)&m_wGrpIDArmsSub;			dwSize = sizeof (m_wGrpIDArmsSub);		break;
	case 24:	pDst = (PBYTE)&m_wGrpIDArmsLeftMain;	dwSize = sizeof (m_wGrpIDArmsLeftMain);	break;
	case 25:	pDst = (PBYTE)&m_wGrpIDArmsLeftSub;		dwSize = sizeof (m_wGrpIDArmsLeftSub);	break;
	case 26:	pDst = (PBYTE)&m_dwCharID;				dwSize = sizeof (m_dwCharID);			break;
	case 27:	pDst = (PBYTE)&m_dwMapID;				dwSize = sizeof (m_dwMapID);			break;
	case 28:	pDst = (PBYTE)&m_dwMotionTypeID;		dwSize = sizeof (m_dwMotionTypeID);		break;
	case 29:	pDst = (PBYTE)&m_clName;				dwSize = sizeof (m_clName);				break;
	case 30:	pDst = (PBYTE)&m_clSpeak;				dwSize = sizeof (m_clSpeak);			break;
	case 31:
		m_strCharName = (LPCSTR)pSrc;
		dwSize = m_strCharName.GetLength () + 1;
		break;
	case 32:
		m_strSpeak = (LPCSTR)pSrc;
		dwSize = m_strSpeak.GetLength () + 1;
		break;
	case 33:
		m_strTalk = (LPCSTR)pSrc;
		dwSize = m_strTalk.GetLength () + 1;
		break;
	case 34:
		pTmp	= pSrc;
		dwTmp	= 0;

		while (1) {
			CopyMemoryRenew (&dwTmp, pTmp, sizeof (DWORD), pTmp);
			if (dwTmp == 0) {
				break;
			}
			m_adwItemID.Add (dwTmp);
		}
		dwSize = (m_adwItemID.GetSize () + 1) * sizeof (DWORD);
		break;
	case 35:
		pTmp	= pSrc;
		dwTmp	= 0;

		while (1) {
			CopyMemoryRenew (&dwTmp, pTmp, sizeof (DWORD), pTmp);
			if (dwTmp == 0) {
				break;
			}
			m_adwSkillID.Add (dwTmp);
		}
		dwSize = (m_adwSkillID.GetSize () + 1) * sizeof (DWORD);
		break;
	case 36:	pDst = (PBYTE)&m_bBlock;					dwSize = sizeof (m_bBlock);					break;
	case 37:	pDst = (PBYTE)&m_bPush;						dwSize = sizeof (m_bPush);					break;
	case 38:	pDst = (PBYTE)&m_dwEquipItemIDCloth;		dwSize = sizeof (m_dwEquipItemIDCloth);		break;
	case 39:	pDst = (PBYTE)&m_dwEquipItemIDAcce1;		dwSize = sizeof (m_dwEquipItemIDAcce1);		break;
	case 40:	pDst = (PBYTE)&m_dwEquipItemIDAcce2;		dwSize = sizeof (m_dwEquipItemIDAcce2);		break;
	case 41:	pDst = (PBYTE)&m_dwEquipItemIDArmsRight;	dwSize = sizeof (m_dwEquipItemIDArmsRight);	break;
	case 42:	pDst = (PBYTE)&m_dwEquipItemIDArmsLeft;		dwSize = sizeof (m_dwEquipItemIDArmsLeft);	break;
	case 43:	pDst = (PBYTE)&m_dwEquipItemIDHead;			dwSize = sizeof (m_dwEquipItemIDHead);		break;
	case 44:	pDst = (PBYTE)&m_wGrpIDInitNPC;				dwSize = sizeof (m_wGrpIDInitNPC);			break;
	case 45:	pDst = (PBYTE)&m_wGrpIDInitCloth;			dwSize = sizeof (m_wGrpIDInitCloth);		break;
	case 46:	pDst = (PBYTE)&m_wGrpIDInitEye;				dwSize = sizeof (m_wGrpIDInitEye);			break;
	case 47:	pDst = (PBYTE)&m_wGrpIDInitEyeColor;		dwSize = sizeof (m_wGrpIDInitEyeColor);		break;
	case 48:	pDst = (PBYTE)&m_wGrpIDInitHairType;		dwSize = sizeof (m_wGrpIDInitHairType);		break;
	case 49:	pDst = (PBYTE)&m_wGrpIDInitHairColor;		dwSize = sizeof (m_wGrpIDInitHairColor);	break;
	case 50:	pDst = (PBYTE)&m_wGrpIDInitSP;				dwSize = sizeof (m_wGrpIDInitSP);			break;
	case 51:	pDst = (PBYTE)&m_wLevel;					dwSize = sizeof (m_wLevel);					break;	/* ���x�� */
	case 52:	pDst = (PBYTE)&m_wStamina;					dwSize = sizeof (m_wStamina);				break;	/* �X�^�~�i */
	case 53:	pDst = (PBYTE)&m_wPower;					dwSize = sizeof (m_wPower);					break;	/* �r�� */
	case 54:	pDst = (PBYTE)&m_wStrength;					dwSize = sizeof (m_wStrength);				break;	/* �̗� */
	case 55:	pDst = (PBYTE)&m_wMagic;					dwSize = sizeof (m_wMagic);					break;	/* ���� */
	case 56:	pDst = (PBYTE)&m_wSkillful;					dwSize = sizeof (m_wSkillful);				break;	/* ��p */
	case 57:	pDst = (PBYTE)&m_wAbillityAT;				dwSize = sizeof (m_wAbillityAT);			break;	/* �U���Z�\ */
	case 58:	pDst = (PBYTE)&m_wAbillityDF;				dwSize = sizeof (m_wAbillityDF);			break;	/* �h��Z�\ */
	case 59:	pDst = (PBYTE)&m_wPAtack;					dwSize = sizeof (m_wPAtack);				break;	/* �U���� */
	case 60:	pDst = (PBYTE)&m_wPDefense;					dwSize = sizeof (m_wPDefense);				break;	/* �h��� */
	case 61:	pDst = (PBYTE)&m_wPMagic;					dwSize = sizeof (m_wPMagic);				break;	/* ���@�� */
	case 62:	pDst = (PBYTE)&m_wPMagicDefense;			dwSize = sizeof (m_wPMagicDefense);			break;	/* ���@�h��� */
	case 63:	pDst = (PBYTE)&m_wPHitAverage;				dwSize = sizeof (m_wPHitAverage);			break;	/* ������ */
	case 64:	pDst = (PBYTE)&m_wPAvoidAverage;			dwSize = sizeof (m_wPAvoidAverage);			break;	/* ��� */
	case 65:	pDst = (PBYTE)&m_wPCriticalAverage;			dwSize = sizeof (m_wPCriticalAverage);		break;	/* �N���e�B�J���� */
	case 66:	pDst = (PBYTE)&m_wAttrFire;					dwSize = sizeof (m_wAttrFire);				break;	/* ����[��] */
	case 67:	pDst = (PBYTE)&m_wAttrWind;					dwSize = sizeof (m_wAttrWind);				break;	/* ����[��] */
	case 68:	pDst = (PBYTE)&m_wAttrWater;				dwSize = sizeof (m_wAttrWater);				break;	/* ����[��] */
	case 69:	pDst = (PBYTE)&m_wAttrEarth;				dwSize = sizeof (m_wAttrEarth);				break;	/* ����[�y] */
	case 70:	pDst = (PBYTE)&m_wAttrLight;				dwSize = sizeof (m_wAttrLight);				break;	/* ����[��] */
	case 71:	pDst = (PBYTE)&m_wAttrDark;					dwSize = sizeof (m_wAttrDark);				break;	/* ����[��] */
	case 72:	pDst = (PBYTE)&m_dwMoveWait;				dwSize = sizeof (m_dwMoveWait);				break;	/* �ړ��҂����� */
	case 73:	pDst = (PBYTE)&m_dwMoveWaitBattle;			dwSize = sizeof (m_dwMoveWaitBattle);		break;	/* �퓬���ړ��҂����� */
	case 74:	pDst = (PBYTE)&m_dwExp;						dwSize = sizeof (m_dwExp);					break;	/* �o���l */
	case 75:	pDst = (PBYTE)&m_dwHP;						dwSize = sizeof (m_dwHP);					break;
	case 76:	pDst = (PBYTE)&m_dwMaxHP;					dwSize = sizeof (m_dwMaxHP);				break;
	case 77:	pDst = (PBYTE)&m_dwSP;						dwSize = sizeof (m_dwSP);					break;
	case 78:	pDst = (PBYTE)&m_dwMaxSP;					dwSize = sizeof (m_dwMaxSP);				break;
	/* NPC���� */
	case 79:	pDst = (PBYTE)&m_dwPutCycle;				dwSize = sizeof (m_dwPutCycle);				break;	/* �������� */
	case 80:	pDst = (PBYTE)&m_nPutMoveType;				dwSize = sizeof (m_nPutMoveType);			break;	/* ����������ړ���� */
	case 81:	pDst = (PBYTE)&m_nMaxPutCount;				dwSize = sizeof (m_nMaxPutCount);			break;	/* ���������� */
	case 82:	pDst = (PBYTE)&m_nPutAverage;				dwSize = sizeof (m_nPutAverage);			break;	/* �����m�� */
	case 83:	pDst = (PBYTE)&m_ptPutArea;					dwSize = sizeof (m_ptPutArea);				break;	/* �����͈�(���a) */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
//		if (nNo == 5) m_nGrpSize /= 2;
//		if (nNo == 7) m_nMaxItemCount = 25;
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetSendDataSize									 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2006/11/06														 */
/* ========================================================================= */

DWORD CInfoCharBase::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwCharID);
	dwRet += sizeof (m_dwAccountID);
	dwRet += sizeof (m_dwTailCharID);
	dwRet += sizeof (m_dwFrontCharID);
	dwRet += sizeof (m_dwParentCharID);
	dwRet += sizeof (m_dwMapID);
	dwRet += sizeof (m_dwMotionTypeID);
	dwRet += sizeof (m_bBlock);
	dwRet += sizeof (m_bPush);
	dwRet += sizeof (m_nAnime);
	dwRet += sizeof (m_nMapX);
	dwRet += sizeof (m_nMapY);
	dwRet += sizeof (m_nMoveState);
	dwRet += sizeof (m_nMoveType);
	dwRet += sizeof (m_nDirection);
	dwRet += sizeof (m_nSex);
	dwRet += sizeof (m_nMaxItemCount);
	dwRet += sizeof (m_nDropItemAverage);		/* �A�C�e���h���b�v�� */
	dwRet += sizeof (m_nMoveAverage);			/* �ړ��m�� */
	dwRet += sizeof (m_nMoveAverageBattle);		/* �퓬���ړ��m�� */
	dwRet += sizeof (m_wFamilyID);
	dwRet += sizeof (m_wGrpIDNPC);
	dwRet += sizeof (m_wGrpIDCloth);
	dwRet += sizeof (m_wGrpIDEye);
	dwRet += sizeof (m_wGrpIDEyeColor);
	dwRet += sizeof (m_wGrpIDHairType);
	dwRet += sizeof (m_wGrpIDHairColor);
	dwRet += sizeof (m_wGrpIDSP);
	dwRet += sizeof (m_wGrpIDTmpMain);			/* �摜ID(�ꎞ��:���C��) */
	dwRet += sizeof (m_wGrpIDTmpSub);			/* �摜ID(�ꎞ��:�T�u) */
	dwRet += sizeof (m_wGrpIDAcce);
	dwRet += sizeof (m_wGrpIDArmsMain);
	dwRet += sizeof (m_wGrpIDArmsSub);
	dwRet += sizeof (m_wGrpIDArmsLeftMain);
	dwRet += sizeof (m_wGrpIDArmsLeftSub);
	dwRet += sizeof (m_wGrpIDInitNPC);
	dwRet += sizeof (m_wGrpIDInitCloth);
	dwRet += sizeof (m_wGrpIDInitEye);
	dwRet += sizeof (m_wGrpIDInitEyeColor);
	dwRet += sizeof (m_wGrpIDInitHairType);
	dwRet += sizeof (m_wGrpIDInitHairColor);
	dwRet += sizeof (m_wGrpIDInitSP);
	dwRet += sizeof (m_wAtackGauge);			/* �A�^�b�N�Q�[�W */
	dwRet += sizeof (m_wDefenseGauge);			/* �f�B�t�F���X�Q�[�W */
	dwRet += sizeof (m_wLevel);					/* ���x�� */
	dwRet += sizeof (m_wStamina);				/* �X�^�~�i */
	dwRet += sizeof (m_wPower);					/* �r�� */
	dwRet += sizeof (m_wStrength);				/* �̗� */
	dwRet += sizeof (m_wMagic);					/* ���� */
	dwRet += sizeof (m_wSkillful);				/* ��p */
	dwRet += sizeof (m_wAbillityAT);			/* �U���Z�\ */
	dwRet += sizeof (m_wAbillityDF);			/* �h��Z�\ */
	dwRet += sizeof (m_wPAtack);				/* �U���� */
	dwRet += sizeof (m_wPDefense);				/* �h��� */
	dwRet += sizeof (m_wPMagic);				/* ���@�� */
	dwRet += sizeof (m_wPMagicDefense);			/* ���@�h��� */
	dwRet += sizeof (m_wPHitAverage);			/* ������ */
	dwRet += sizeof (m_wPAvoidAverage);			/* ��� */
	dwRet += sizeof (m_wPCriticalAverage);		/* �N���e�B�J���� */
	dwRet += sizeof (m_wAttrFire);				/* ����[��] */
	dwRet += sizeof (m_wAttrWind);				/* ����[��] */
	dwRet += sizeof (m_wAttrWater);				/* ����[��] */
	dwRet += sizeof (m_wAttrEarth);				/* ����[�y] */
	dwRet += sizeof (m_wAttrLight);				/* ����[��] */
	dwRet += sizeof (m_wAttrDark);				/* ����[��] */
	dwRet += sizeof (m_dwEquipItemIDCloth);
	dwRet += sizeof (m_dwEquipItemIDAcce1);
	dwRet += sizeof (m_dwEquipItemIDAcce2);
	dwRet += sizeof (m_dwEquipItemIDArmsRight);
	dwRet += sizeof (m_dwEquipItemIDArmsLeft);
	dwRet += sizeof (m_dwEquipItemIDHead);
	dwRet += sizeof (m_dwMoveWait);				/* �ړ��҂����� */
	dwRet += sizeof (m_dwMoveWaitBattle);		/* �퓬���ړ��҂����� */
	dwRet += sizeof (m_dwExp);					/* �o���l */
	dwRet += sizeof (m_dwHP);
	dwRet += sizeof (m_dwMaxHP);
	dwRet += sizeof (m_dwSP);
	dwRet += sizeof (m_dwMaxSP);
	dwRet += sizeof (m_clName);
	dwRet += sizeof (m_clSpeak);
	dwRet += (m_strCharName.GetLength () + 1);
	dwRet += (m_strSpeak.	GetLength () + 1);
	dwRet += (m_strTalk.	GetLength () + 1);
	dwRet += (m_abyMark.	GetSize () + 1);
	dwRet += ((m_adwItemID.	GetSize () + 1) * sizeof (DWORD));
	dwRet += ((m_adwSkillID.GetSize () + 1) * sizeof (DWORD));
	/* NPC���� */
	dwRet += sizeof (m_dwPutCycle);				/* �������� */
	dwRet += sizeof (m_nPutMoveType);			/* ����������ړ���� */
	dwRet += sizeof (m_nMaxPutCount);			/* ���������� */
	dwRet += sizeof (m_nPutAverage);			/* �����m�� */
	dwRet += sizeof (m_ptPutArea);				/* �����͈�(���a) */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetSendData										 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2006/11/06														 */
/* ========================================================================= */

PBYTE CInfoCharBase::GetSendData(void)
{
	BYTE byTmp;
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwSize, dwTmp;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	pDataTmp	= pData;

	CopyMemoryRenew (pDataTmp, &m_dwCharID,					sizeof (m_dwCharID),				pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &m_dwAccountID,				sizeof (m_dwAccountID),				pDataTmp);	/* �A�J�E���gID */
	CopyMemoryRenew (pDataTmp, &m_dwTailCharID,				sizeof (m_dwTailCharID),			pDataTmp);	/* �t���ė��Ă���L����ID */
	CopyMemoryRenew (pDataTmp, &m_dwFrontCharID,			sizeof (m_dwFrontCharID),			pDataTmp);	/* �t���Ă���L����ID */
	CopyMemoryRenew (pDataTmp, &m_dwParentCharID,			sizeof (m_dwParentCharID),			pDataTmp);	/* �e�̃L����ID */
	CopyMemoryRenew (pDataTmp, &m_bBlock,					sizeof (m_bBlock),					pDataTmp);	/* �Ԃ��锻�� */
	CopyMemoryRenew (pDataTmp, &m_bPush,					sizeof (m_bPush),					pDataTmp);	/* �����锻�� */
	CopyMemoryRenew (pDataTmp, &m_dwMapID,					sizeof (m_dwMapID),					pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (pDataTmp, &m_dwMotionTypeID,			sizeof (m_dwMotionTypeID),			pDataTmp);	/* ���[�V�������ID */
	CopyMemoryRenew (pDataTmp, &m_nAnime,					sizeof (m_nAnime),					pDataTmp);	/* �A�j���[�V�����ԍ� */
	CopyMemoryRenew (pDataTmp, &m_nMapX,					sizeof (m_nMapX),					pDataTmp);	/* �}�b�v���W(��) */
	CopyMemoryRenew (pDataTmp, &m_nMapY,					sizeof (m_nMapY),					pDataTmp);	/* �}�b�v���W(�c) */
	CopyMemoryRenew (pDataTmp, &m_nMoveState,				sizeof (m_nMoveState),				pDataTmp);	/* �ړ���� */
	CopyMemoryRenew (pDataTmp, &m_nMoveType,				sizeof (m_nMoveType),				pDataTmp);	/* �ړ���� */
	CopyMemoryRenew (pDataTmp, &m_nDirection,				sizeof (m_nDirection),				pDataTmp);	/* ���� */
	CopyMemoryRenew (pDataTmp, &m_nSex,						sizeof (m_nSex),					pDataTmp);	/* ���� */
	CopyMemoryRenew (pDataTmp, &m_nMaxItemCount,			sizeof (m_nMaxItemCount),			pDataTmp);	/* �ő�A�C�e�������� */
	CopyMemoryRenew (pDataTmp, &m_nDropItemAverage,			sizeof (m_nDropItemAverage),		pDataTmp);	/* �A�C�e���h���b�v�� */
	CopyMemoryRenew (pDataTmp, &m_nMoveAverage,				sizeof (m_nMoveAverage),			pDataTmp);	/* �ړ��m�� */
	CopyMemoryRenew (pDataTmp, &m_nMoveAverageBattle,		sizeof (m_nMoveAverageBattle),		pDataTmp);	/* �퓬���ړ��m�� */
	CopyMemoryRenew (pDataTmp, &m_wFamilyID,				sizeof (m_wFamilyID),				pDataTmp);	/* �푰ID */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDNPC,				sizeof (m_wGrpIDNPC),				pDataTmp);	/* �摜ID(NPC) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDCloth,				sizeof (m_wGrpIDCloth),				pDataTmp);	/* �摜ID(��) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDEye,				sizeof (m_wGrpIDEye),				pDataTmp);	/* �摜ID(��) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDEyeColor,			sizeof (m_wGrpIDEyeColor),			pDataTmp);	/* �摜ID(�ڐF) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDHairType,			sizeof (m_wGrpIDHairType),			pDataTmp);	/* �摜ID(��) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDHairColor,			sizeof (m_wGrpIDHairColor),			pDataTmp);	/* �摜ID(���F) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDSP,					sizeof (m_wGrpIDSP),				pDataTmp);	/* �摜ID(���ꕞ) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDTmpMain,			sizeof (m_wGrpIDTmpMain),			pDataTmp);	/* �摜ID(�ꎞ��:���C��) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDTmpSub,				sizeof (m_wGrpIDTmpSub),			pDataTmp);	/* �摜ID(�ꎞ��:�T�u) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDAcce,				sizeof (m_wGrpIDAcce),				pDataTmp);	/* �摜ID(�A�N�Z�T��) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDArmsMain,			sizeof (m_wGrpIDArmsMain),			pDataTmp);	/* �摜ID(������:���C��) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDArmsSub,			sizeof (m_wGrpIDArmsSub),			pDataTmp);	/* �摜ID(������:�T�u) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDArmsLeftMain,		sizeof (m_wGrpIDArmsLeftMain),		pDataTmp);	/* �摜ID(��:���C��) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDArmsLeftSub,		sizeof (m_wGrpIDArmsLeftSub),		pDataTmp);	/* �摜ID(��:�T�u) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDInitNPC,			sizeof (m_wGrpIDInitNPC),			pDataTmp);	/* �����摜ID(NPC) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDInitCloth,			sizeof (m_wGrpIDInitCloth),			pDataTmp);	/* �����摜ID(��) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDInitEye,			sizeof (m_wGrpIDInitEye),			pDataTmp);	/* �����摜ID(��) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDInitEyeColor,		sizeof (m_wGrpIDInitEyeColor),		pDataTmp);	/* �����摜ID(�ڐF) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDInitHairType,		sizeof (m_wGrpIDInitHairType),		pDataTmp);	/* �����摜ID(��) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDInitHairColor,		sizeof (m_wGrpIDInitHairColor),		pDataTmp);	/* �����摜ID(���F) */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDInitSP,				sizeof (m_wGrpIDInitSP),			pDataTmp);	/* �����摜ID(���ꕞ) */
	CopyMemoryRenew (pDataTmp, &m_wAtackGauge,				sizeof (m_wAtackGauge),				pDataTmp);	/* �A�^�b�N�Q�[�W */
	CopyMemoryRenew (pDataTmp, &m_wDefenseGauge,			sizeof (m_wDefenseGauge),			pDataTmp);	/* �f�B�t�F���X�Q�[�W */
	CopyMemoryRenew (pDataTmp, &m_wLevel,					sizeof (m_wLevel),					pDataTmp);	/* ���x�� */
	CopyMemoryRenew (pDataTmp, &m_wStamina,					sizeof (m_wStamina),				pDataTmp);	/* �X�^�~�i */
	CopyMemoryRenew (pDataTmp, &m_wPower,					sizeof (m_wPower),					pDataTmp);	/* �r�� */
	CopyMemoryRenew (pDataTmp, &m_wStrength,				sizeof (m_wStrength),				pDataTmp);	/* �̗� */
	CopyMemoryRenew (pDataTmp, &m_wMagic,					sizeof (m_wMagic),					pDataTmp);	/* ���� */
	CopyMemoryRenew (pDataTmp, &m_wSkillful,				sizeof (m_wSkillful),				pDataTmp);	/* ��p */
	CopyMemoryRenew (pDataTmp, &m_wAbillityAT,				sizeof (m_wAbillityAT),				pDataTmp);	/* �U���Z�\ */
	CopyMemoryRenew (pDataTmp, &m_wAbillityDF,				sizeof (m_wAbillityDF),				pDataTmp);	/* �h��Z�\ */
	CopyMemoryRenew (pDataTmp, &m_wPAtack,					sizeof (m_wPAtack),					pDataTmp);	/* �U���� */
	CopyMemoryRenew (pDataTmp, &m_wPDefense,				sizeof (m_wPDefense),				pDataTmp);	/* �h��� */
	CopyMemoryRenew (pDataTmp, &m_wPMagic,					sizeof (m_wPMagic),					pDataTmp);	/* ���@�� */
	CopyMemoryRenew (pDataTmp, &m_wPMagicDefense,			sizeof (m_wPMagicDefense),			pDataTmp);	/* ���@�h��� */
	CopyMemoryRenew (pDataTmp, &m_wPHitAverage,				sizeof (m_wPHitAverage),			pDataTmp);	/* ������ */
	CopyMemoryRenew (pDataTmp, &m_wPAvoidAverage,			sizeof (m_wPAvoidAverage),			pDataTmp);	/* ��� */
	CopyMemoryRenew (pDataTmp, &m_wPCriticalAverage,		sizeof (m_wPCriticalAverage),		pDataTmp);	/* �N���e�B�J���� */
	CopyMemoryRenew (pDataTmp, &m_wAttrFire,				sizeof (m_wAttrFire),				pDataTmp);	/* ����[��] */
	CopyMemoryRenew (pDataTmp, &m_wAttrWind,				sizeof (m_wAttrWind),				pDataTmp);	/* ����[��] */
	CopyMemoryRenew (pDataTmp, &m_wAttrWater,				sizeof (m_wAttrWater),				pDataTmp);	/* ����[��] */
	CopyMemoryRenew (pDataTmp, &m_wAttrEarth,				sizeof (m_wAttrEarth),				pDataTmp);	/* ����[�y] */
	CopyMemoryRenew (pDataTmp, &m_wAttrLight,				sizeof (m_wAttrLight),				pDataTmp);	/* ����[��] */
	CopyMemoryRenew (pDataTmp, &m_wAttrDark,				sizeof (m_wAttrDark),				pDataTmp);	/* ����[��] */
	CopyMemoryRenew (pDataTmp, &m_dwEquipItemIDCloth,		sizeof (m_dwEquipItemIDCloth),		pDataTmp);	/* �����A�C�e��ID:�� */
	CopyMemoryRenew (pDataTmp, &m_dwEquipItemIDAcce1,		sizeof (m_dwEquipItemIDAcce1),		pDataTmp);	/* �����A�C�e��ID:�A�N�Z�T��1 */
	CopyMemoryRenew (pDataTmp, &m_dwEquipItemIDAcce2,		sizeof (m_dwEquipItemIDAcce2),		pDataTmp);	/* �����A�C�e��ID:�A�N�Z�T��2 */
	CopyMemoryRenew (pDataTmp, &m_dwEquipItemIDArmsRight,	sizeof (m_dwEquipItemIDArmsRight),	pDataTmp);	/* �����A�C�e��ID:�E�� */
	CopyMemoryRenew (pDataTmp, &m_dwEquipItemIDArmsLeft,	sizeof (m_dwEquipItemIDArmsLeft),	pDataTmp);	/* �����A�C�e��ID:���� */
	CopyMemoryRenew (pDataTmp, &m_dwEquipItemIDHead,		sizeof (m_dwEquipItemIDHead),		pDataTmp);	/* �����A�C�e��ID:�� */
	CopyMemoryRenew (pDataTmp, &m_dwMoveWait,				sizeof (m_dwMoveWait),				pDataTmp);	/* �ړ��҂����� */
	CopyMemoryRenew (pDataTmp, &m_dwMoveWaitBattle,			sizeof (m_dwMoveWaitBattle),		pDataTmp);	/* �퓬���ړ��҂����� */
	CopyMemoryRenew (pDataTmp, &m_dwExp,					sizeof (m_dwExp),					pDataTmp);	/* �o���l */
	CopyMemoryRenew (pDataTmp, &m_dwHP,						sizeof (m_dwHP),					pDataTmp);	/* HP */
	CopyMemoryRenew (pDataTmp, &m_dwMaxHP,					sizeof (m_dwMaxHP),					pDataTmp);	/* �ő�HP */
	CopyMemoryRenew (pDataTmp, &m_dwSP,						sizeof (m_dwSP),					pDataTmp);	/* SP */
	CopyMemoryRenew (pDataTmp, &m_dwMaxSP,					sizeof (m_dwMaxSP),					pDataTmp);	/* �ő�SP */
	CopyMemoryRenew (pDataTmp, &m_clName,					sizeof (m_clName),					pDataTmp);	/* ���O�̕`��F */
	CopyMemoryRenew (pDataTmp, &m_clSpeak,					sizeof (m_clSpeak),					pDataTmp);	/* �����̕`��F */
	strcpyRenew ((LPSTR)pDataTmp, m_strCharName, pDataTmp);													/* �L������ */
	strcpyRenew ((LPSTR)pDataTmp, m_strSpeak, pDataTmp);													/* �������e */
	strcpyRenew ((LPSTR)pDataTmp, m_strTalk,  pDataTmp);													/* ��b�f�[�^ */

	nCount = m_abyMark.GetSize ();
	for (i = 0; i < nCount; i ++) {
		byTmp = m_abyMark[i];
		CopyMemoryRenew (pDataTmp, &byTmp, sizeof (byTmp), pDataTmp);	/* ���O�̑O�ɕ\������}�[�N�ԍ� */
	}
	byTmp = 0;
	CopyMemoryRenew (pDataTmp, &byTmp, sizeof (byTmp), pDataTmp);
	nCount = m_adwItemID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwTmp = m_adwItemID[i];
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);	/* �����A�C�e�� */
	}
	dwTmp = 0;
	CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);
	nCount = m_adwSkillID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwTmp = m_adwSkillID[i];
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);	/* �����X�L�� */
	}
	dwTmp = 0;
	CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);

	/* NPC���� */
	CopyMemoryRenew (pDataTmp, &m_dwPutCycle,	sizeof (m_dwPutCycle),		pDataTmp);		/* �������� */
	CopyMemoryRenew (pDataTmp, &m_nPutMoveType,	sizeof (m_nPutMoveType),	pDataTmp);		/* ����������ړ���� */
	CopyMemoryRenew (pDataTmp, &m_nMaxPutCount,	sizeof (m_nMaxPutCount),	pDataTmp);		/* ���������� */
	CopyMemoryRenew (pDataTmp, &m_nPutAverage,	sizeof (m_nPutAverage),		pDataTmp);		/* �����m�� */
	CopyMemoryRenew (pDataTmp, &m_ptPutArea,	sizeof (m_ptPutArea),		pDataTmp);		/* �����͈�(���a) */

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::SetSendData										 */
/* ���e		:���M�f�[�^��ݒ�												 */
/* ���t		:2006/11/06														 */
/* �߂�l	:����������̃A�h���X											 */
/* ========================================================================= */

PBYTE CInfoCharBase::SetSendData(PBYTE pSrc)
{
	BYTE byTmp;
	DWORD dwTmp;
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	m_abyMark.RemoveAll ();
	m_adwItemID.RemoveAll ();
	m_adwSkillID.RemoveAll ();

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwCharID,				pDataTmp, sizeof (m_dwCharID),					pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwAccountID,			pDataTmp, sizeof (m_dwAccountID),				pDataTmp);	/* �A�J�E���gID */
	CopyMemoryRenew (&m_dwTailCharID,			pDataTmp, sizeof (m_dwTailCharID),				pDataTmp);	/* �t���ė��Ă���L����ID */
	CopyMemoryRenew (&m_dwFrontCharID,			pDataTmp, sizeof (m_dwFrontCharID),				pDataTmp);	/* �t���Ă���L����ID */
	CopyMemoryRenew (&m_dwParentCharID,			pDataTmp, sizeof (m_dwParentCharID),			pDataTmp);	/* �e�̃L����ID */
	CopyMemoryRenew (&m_bBlock,					pDataTmp, sizeof (m_bBlock),					pDataTmp);	/* �Ԃ��锻�� */
	CopyMemoryRenew (&m_bPush,					pDataTmp, sizeof (m_bPush),						pDataTmp);	/* �����锻�� */
	CopyMemoryRenew (&m_dwMapID,				pDataTmp, sizeof (m_dwMapID),					pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (&m_dwMotionTypeID,			pDataTmp, sizeof (m_dwMotionTypeID),			pDataTmp);	/* ���[�V�������ID */
	CopyMemoryRenew (&m_nAnime,					pDataTmp, sizeof (m_nAnime),					pDataTmp);	/* �A�j���[�V�����ԍ� */
	CopyMemoryRenew (&m_nMapX,					pDataTmp, sizeof (m_nMapX),						pDataTmp);	/* �}�b�v���W(��) */
	CopyMemoryRenew (&m_nMapY,					pDataTmp, sizeof (m_nMapY),						pDataTmp);	/* �}�b�v���W(�c) */
	CopyMemoryRenew (&m_nMoveState,				pDataTmp, sizeof (m_nMoveState),				pDataTmp);	/* �ړ���� */
	CopyMemoryRenew (&m_nMoveType,				pDataTmp, sizeof (m_nMoveType),					pDataTmp);	/* �ړ���� */
	CopyMemoryRenew (&m_nDirection,				pDataTmp, sizeof (m_nDirection),				pDataTmp);	/* ���� */
	CopyMemoryRenew (&m_nSex,					pDataTmp, sizeof (m_nSex),						pDataTmp);	/* ���� */
	CopyMemoryRenew (&m_nMaxItemCount,			pDataTmp, sizeof (m_nMaxItemCount),				pDataTmp);	/* �ő�A�C�e�������� */
	CopyMemoryRenew (&m_nDropItemAverage,		pDataTmp, sizeof (m_nDropItemAverage),			pDataTmp);	/* �A�C�e���h���b�v�� */
	CopyMemoryRenew (&m_nMoveAverage,			pDataTmp, sizeof (m_nMoveAverage),				pDataTmp);	/* �ړ��m�� */
	CopyMemoryRenew (&m_nMoveAverageBattle,		pDataTmp, sizeof (m_nMoveAverageBattle),		pDataTmp);	/* �퓬���ړ��m�� */
	CopyMemoryRenew (&m_wFamilyID,				pDataTmp, sizeof (m_wFamilyID),					pDataTmp);	/* �푰ID */
	CopyMemoryRenew (&m_wGrpIDNPC,				pDataTmp, sizeof (m_wGrpIDNPC),					pDataTmp);	/* �摜ID(NPC) */
	CopyMemoryRenew (&m_wGrpIDCloth,			pDataTmp, sizeof (m_wGrpIDCloth),				pDataTmp);	/* �摜ID(��) */
	CopyMemoryRenew (&m_wGrpIDEye,				pDataTmp, sizeof (m_wGrpIDEye),					pDataTmp);	/* �摜ID(��) */
	CopyMemoryRenew (&m_wGrpIDEyeColor,			pDataTmp, sizeof (m_wGrpIDEyeColor),			pDataTmp);	/* �摜ID(�ڐF) */
	CopyMemoryRenew (&m_wGrpIDHairType,			pDataTmp, sizeof (m_wGrpIDHairType),			pDataTmp);	/* �摜ID(��) */
	CopyMemoryRenew (&m_wGrpIDHairColor,		pDataTmp, sizeof (m_wGrpIDHairColor),			pDataTmp);	/* �摜ID(���F) */
	CopyMemoryRenew (&m_wGrpIDSP,				pDataTmp, sizeof (m_wGrpIDSP),					pDataTmp);	/* �摜ID(���ꕞ) */
	CopyMemoryRenew (&m_wGrpIDTmpMain,			pDataTmp, sizeof (m_wGrpIDTmpMain),				pDataTmp);	/* �摜ID(�ꎞ��:���C��) */
	CopyMemoryRenew (&m_wGrpIDTmpSub,			pDataTmp, sizeof (m_wGrpIDTmpSub),				pDataTmp);	/* �摜ID(�ꎞ��:�T�u) */
	CopyMemoryRenew (&m_wGrpIDAcce,				pDataTmp, sizeof (m_wGrpIDAcce),				pDataTmp);	/* �摜ID(�A�N�Z�T��) */
	CopyMemoryRenew (&m_wGrpIDArmsMain,			pDataTmp, sizeof (m_wGrpIDArmsMain),			pDataTmp);	/* �摜ID(������:���C��) */
	CopyMemoryRenew (&m_wGrpIDArmsSub,			pDataTmp, sizeof (m_wGrpIDArmsSub),				pDataTmp);	/* �摜ID(������:�T�u) */
	CopyMemoryRenew (&m_wGrpIDArmsLeftMain,		pDataTmp, sizeof (m_wGrpIDArmsLeftMain),		pDataTmp);	/* �摜ID(��:���C��) */
	CopyMemoryRenew (&m_wGrpIDArmsLeftSub,		pDataTmp, sizeof (m_wGrpIDArmsLeftSub),			pDataTmp);	/* �摜ID(��:�T�u) */
	CopyMemoryRenew (&m_wGrpIDInitNPC,			pDataTmp, sizeof (m_wGrpIDInitNPC),				pDataTmp);	/* �����摜ID(NPC) */
	CopyMemoryRenew (&m_wGrpIDInitCloth,		pDataTmp, sizeof (m_wGrpIDInitCloth),			pDataTmp);	/* �����摜ID(��) */
	CopyMemoryRenew (&m_wGrpIDInitEye,			pDataTmp, sizeof (m_wGrpIDInitEye),				pDataTmp);	/* �����摜ID(��) */
	CopyMemoryRenew (&m_wGrpIDInitEyeColor,		pDataTmp, sizeof (m_wGrpIDInitEyeColor),		pDataTmp);	/* �����摜ID(�ڐF) */
	CopyMemoryRenew (&m_wGrpIDInitHairType,		pDataTmp, sizeof (m_wGrpIDInitHairType),		pDataTmp);	/* �����摜ID(��) */
	CopyMemoryRenew (&m_wGrpIDInitHairColor,	pDataTmp, sizeof (m_wGrpIDInitHairColor),		pDataTmp);	/* �����摜ID(���F) */
	CopyMemoryRenew (&m_wGrpIDInitSP,			pDataTmp, sizeof (m_wGrpIDInitSP),				pDataTmp);	/* �����摜ID(���ꕞ) */
	CopyMemoryRenew (&m_wAtackGauge,			pDataTmp, sizeof (m_wAtackGauge),				pDataTmp);	/* �A�^�b�N�Q�[�W */
	CopyMemoryRenew (&m_wDefenseGauge,			pDataTmp, sizeof (m_wDefenseGauge),				pDataTmp);	/* �f�B�t�F���X�Q�[�W */
	CopyMemoryRenew (&m_wLevel,					pDataTmp, sizeof (m_wLevel),					pDataTmp);	/* ���x�� */
	CopyMemoryRenew (&m_wStamina,				pDataTmp, sizeof (m_wStamina),					pDataTmp);	/* �X�^�~�i */
	CopyMemoryRenew (&m_wPower,					pDataTmp, sizeof (m_wPower),					pDataTmp);	/* �r�� */
	CopyMemoryRenew (&m_wStrength,				pDataTmp, sizeof (m_wStrength),					pDataTmp);	/* �̗� */
	CopyMemoryRenew (&m_wMagic,					pDataTmp, sizeof (m_wMagic),					pDataTmp);	/* ���� */
	CopyMemoryRenew (&m_wSkillful,				pDataTmp, sizeof (m_wSkillful),					pDataTmp);	/* ��p */
	CopyMemoryRenew (&m_wAbillityAT,			pDataTmp, sizeof (m_wAbillityAT),				pDataTmp);	/* �U���Z�\ */
	CopyMemoryRenew (&m_wAbillityDF,			pDataTmp, sizeof (m_wAbillityDF),				pDataTmp);	/* �h��Z�\ */
	CopyMemoryRenew (&m_wPAtack,				pDataTmp, sizeof (m_wPAtack),					pDataTmp);	/* �U���� */
	CopyMemoryRenew (&m_wPDefense,				pDataTmp, sizeof (m_wPDefense),					pDataTmp);	/* �h��� */
	CopyMemoryRenew (&m_wPMagic,				pDataTmp, sizeof (m_wPMagic),					pDataTmp);	/* ���@�� */
	CopyMemoryRenew (&m_wPMagicDefense,			pDataTmp, sizeof (m_wPMagicDefense),			pDataTmp);	/* ���@�h��� */
	CopyMemoryRenew (&m_wPHitAverage,			pDataTmp, sizeof (m_wPHitAverage),				pDataTmp);	/* ������ */
	CopyMemoryRenew (&m_wPAvoidAverage,			pDataTmp, sizeof (m_wPAvoidAverage),			pDataTmp);	/* ��� */
	CopyMemoryRenew (&m_wPCriticalAverage,		pDataTmp, sizeof (m_wPCriticalAverage),			pDataTmp);	/* �N���e�B�J���� */
	CopyMemoryRenew (&m_wAttrFire,				pDataTmp, sizeof (m_wAttrFire),					pDataTmp);	/* ����[��] */
	CopyMemoryRenew (&m_wAttrWind,				pDataTmp, sizeof (m_wAttrWind),					pDataTmp);	/* ����[��] */
	CopyMemoryRenew (&m_wAttrWater,				pDataTmp, sizeof (m_wAttrWater),				pDataTmp);	/* ����[��] */
	CopyMemoryRenew (&m_wAttrEarth,				pDataTmp, sizeof (m_wAttrEarth),				pDataTmp);	/* ����[�y] */
	CopyMemoryRenew (&m_wAttrLight,				pDataTmp, sizeof (m_wAttrLight),				pDataTmp);	/* ����[��] */
	CopyMemoryRenew (&m_wAttrDark,				pDataTmp, sizeof (m_wAttrDark),					pDataTmp);	/* ����[��] */
	CopyMemoryRenew (&m_dwEquipItemIDCloth,		pDataTmp, sizeof (m_dwEquipItemIDCloth),		pDataTmp);	/* �����A�C�e��ID:�� */
	CopyMemoryRenew (&m_dwEquipItemIDAcce1,		pDataTmp, sizeof (m_dwEquipItemIDAcce1),		pDataTmp);	/* �����A�C�e��ID:�A�N�Z�T��1 */
	CopyMemoryRenew (&m_dwEquipItemIDAcce2,		pDataTmp, sizeof (m_dwEquipItemIDAcce2),		pDataTmp);	/* �����A�C�e��ID:�A�N�Z�T��2 */
	CopyMemoryRenew (&m_dwEquipItemIDArmsRight,	pDataTmp, sizeof (m_dwEquipItemIDArmsRight),	pDataTmp);	/* �����A�C�e��ID:�E�� */
	CopyMemoryRenew (&m_dwEquipItemIDArmsLeft,	pDataTmp, sizeof (m_dwEquipItemIDArmsLeft),		pDataTmp);	/* �����A�C�e��ID:���� */
	CopyMemoryRenew (&m_dwEquipItemIDHead,		pDataTmp, sizeof (m_dwEquipItemIDHead),			pDataTmp);	/* �����A�C�e��ID:�� */
	CopyMemoryRenew (&m_dwMoveWait,				pDataTmp, sizeof (m_dwMoveWait),				pDataTmp);	/* �ړ��҂����� */
	CopyMemoryRenew (&m_dwMoveWaitBattle,		pDataTmp, sizeof (m_dwMoveWaitBattle),			pDataTmp);	/* �퓬���ړ��҂����� */
	CopyMemoryRenew (&m_dwExp,					pDataTmp, sizeof (m_dwExp),						pDataTmp);	/* �o���l */
	CopyMemoryRenew (&m_dwHP,					pDataTmp, sizeof (m_dwHP),						pDataTmp);	/* HP */
	CopyMemoryRenew (&m_dwMaxHP,				pDataTmp, sizeof (m_dwMaxHP),					pDataTmp);	/* �ő�HP */
	CopyMemoryRenew (&m_dwSP,					pDataTmp, sizeof (m_dwSP),						pDataTmp);	/* SP */
	CopyMemoryRenew (&m_dwMaxSP,				pDataTmp, sizeof (m_dwMaxSP),					pDataTmp);	/* �ő�SP */
	CopyMemoryRenew (&m_clName,					pDataTmp, sizeof (m_clName),					pDataTmp);	/* ���O�̕`��F */
	CopyMemoryRenew (&m_clSpeak,				pDataTmp, sizeof (m_clSpeak),					pDataTmp);	/* �����̕`��F */
	StoreRenew (m_strCharName,	(LPCSTR)pDataTmp, pDataTmp);												/* �L������ */
	StoreRenew (m_strSpeak,		(LPCSTR)pDataTmp, pDataTmp);												/* �������e */
	StoreRenew (m_strTalk,		(LPCSTR)pDataTmp, pDataTmp);												/* ��b�f�[�^ */

	SetName (m_strCharName);
	SetSpeak (m_strSpeak);

	while (1) {
		CopyMemoryRenew (&byTmp, pDataTmp, sizeof (byTmp), pDataTmp);	/* ���O�̑O�ɕ\������}�[�N�ԍ� */
		if (byTmp == 0) {
			break;
		}
		m_abyMark.Add (byTmp);
	}
	while (1) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);	/* �����A�C�e�� */
		if (dwTmp == 0) {
			break;
		}
		m_adwItemID.Add (dwTmp);
	}
	while (1) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);	/* �����X�L�� */
		if (dwTmp == 0) {
			break;
		}
		m_adwSkillID.Add (dwTmp);
	}

	/* NPC���� */
	CopyMemoryRenew (&m_dwPutCycle,		pDataTmp, sizeof (m_dwPutCycle),	pDataTmp);		/* �������� */
	CopyMemoryRenew (&m_nPutMoveType,	pDataTmp, sizeof (m_nPutMoveType),	pDataTmp);		/* ����������ړ���� */
	CopyMemoryRenew (&m_nMaxPutCount,	pDataTmp, sizeof (m_nMaxPutCount),	pDataTmp);		/* ���������� */
	CopyMemoryRenew (&m_nPutAverage,	pDataTmp, sizeof (m_nPutAverage),	pDataTmp);		/* �����m�� */
	CopyMemoryRenew (&m_ptPutArea,		pDataTmp, sizeof (m_ptPutArea),		pDataTmp);		/* �����͈�(���a) */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::IsLogin											 */
/* ���e		:���O�C����������												 */
/* ���t		:2007/02/12														 */
/* �߂�l	:TRUE:���O�C���� FALS:���O�A�E�g��								 */
/* ========================================================================= */

BOOL CInfoCharBase::IsLogin(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nMoveType != CHARMOVETYPE_PC) {
		return TRUE;
	}
	if (m_bNPC) {
		bRet = TRUE;
		goto Exit;
	}
	if (m_dwAccountID == 0) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::IsStateBattle									 */
/* ���e		:�퓬���[�h������												 */
/* ���t		:2007/07/21														 */
/* �߂�l	:TRUE:�퓬���[�h��												 */
/* ========================================================================= */

BOOL CInfoCharBase::IsStateBattle(void)
{
	BOOL bRet;

	bRet = FALSE;

	switch (m_nMoveState) {
	case CHARMOVESTATE_BATTLE:			/* �퓬�� */
	case CHARMOVESTATE_BATTLEATACK:		/* �퓬�U���� */
	case CHARMOVESTATE_BATTLEMOVE:		/* �퓬�ړ��� */
	case CHARMOVESTATE_BATTLE_DEFENSE:	/* �h�䒆 */
		break;
	default:
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::IsStateMove										 */
/* ���e		:�ړ���������													 */
/* ���t		:2008/08/13														 */
/* �߂�l	:TRUE:�ړ���													 */
/* ========================================================================= */

BOOL CInfoCharBase::IsStateMove(void)
{
	BOOL bRet;

	bRet = FALSE;

	switch (m_nMoveState) {
	case CHARMOVESTATE_MOVE:			/* �ړ��� */
	case CHARMOVESTATE_BATTLEMOVE:		/* �퓬�ړ��� */
		break;
	default:
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::IsViewArea										 */
/* ���e		:������͈͂�����												 */
/* ���t		:2007/09/08														 */
/* �߂�l	:TRUE:������͈�												 */
/* ========================================================================= */

BOOL CInfoCharBase::IsViewArea(DWORD dwMapID, POINT *pptPos)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_dwMapID != dwMapID) {
		goto Exit;
	}
	if (!((abs (pptPos->x - m_nMapX) <= (DRAW_PARTS_X * 2) + 1) &&
		(abs (pptPos->y - m_nMapY) <= (DRAW_PARTS_Y * 2) + 1))) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::IsNPC											 */
/* ���e		:NPC������														 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

BOOL CInfoCharBase::IsNPC(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nMoveType == CHARMOVETYPE_PC) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::CheckSessionID									 */
/* ���e		:�Z�b�V����ID���`�F�b�N											 */
/* ���t		:2008/01/05														 */
/* ========================================================================= */

BOOL CInfoCharBase::CheckSessionID(DWORD dwSessionID)
{
	BOOL bRet;

	bRet = FALSE;

	if (dwSessionID != m_dwSessionID) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetFrontPos										 */
/* ���e		:����O�̃L�������W���擾										 */
/* ���t		:2007/07/27														 */
/* ========================================================================= */

void CInfoCharBase::GetFrontPos(
	POINT &ptDst,					/* [ou] �擾�� */
	int nDirection	/*-1*/,			/* [in] ���� */
	BOOL bMove		/*FALSE*/)		/* [in] TRUE:�ړ���p�̃L�������W */
{
	int nFrontPosX[] = {0, 0, -1, 1, 1, 1, -1, -1}, nFrontPosY[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	SIZE sizeChar;

	if (nDirection == -1) {
		nDirection = m_nDirection;
	}

	sizeChar.cx = sizeChar.cy = 1;
	if (bMove == FALSE) {
		GetCharSize (sizeChar);
	}
	ptDst.x = m_nMapX + (nFrontPosX[nDirection] * sizeChar.cx);
	ptDst.y = m_nMapY + (nFrontPosY[nDirection] * sizeChar.cy);
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetFrontPos										 */
/* ���e		:����O�̃L�������W���擾										 */
/* ���t		:2008/05/01														 */
/* ========================================================================= */

void CInfoCharBase::GetFrontPos(
	CmyArray<POINT, POINT> &aptPos,		/* [ou] �擾�� */
	int nDirection	/*-1*/)				/* [in] ���� */
{
	int i, nCount;
	SIZE sizeChar;
	POINT ptFront, ptTmp;
	int nFrontPosX[] = {0, 0, 0, 1, 1, 1, 0, 0}, nFrontPosY[] = {0, 1, 0, 0, 0, 1, 1, 0};

	aptPos.RemoveAll ();
	if (nDirection == -1) {
		nDirection = m_nDirection;
	}
	GetFrontPos (ptFront, nDirection, FALSE);

	sizeChar.cx = sizeChar.cy = 1;
	GetCharSize (sizeChar);

	ptTmp = ptFront;
	switch (nDirection) {
	case 0:
	case 1:
	case 2:
	case 3:
		nCount = sizeChar.cx;
		if (nDirection == 3) {
			ptTmp.x -= sizeChar.cx;
		}
		if (nDirection < 2) {
			ptTmp.x -= (sizeChar.cx - 1);
			nCount ++;
		}
		ptTmp.x += nFrontPosX[nDirection];
		ptTmp.y += nFrontPosY[nDirection];
		for (i = 0; i < nCount; i ++) {
			aptPos.Add (ptTmp);
			ptTmp.x ++;
		}
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		ptTmp  = ptFront;
		nCount = sizeChar.cx;
		ptTmp.x --;
		ptTmp.y += nFrontPosY[nDirection];
		for (i = 0; i < nCount + 1; i ++) {
			aptPos.Add (ptTmp);
			ptTmp.x ++;
		}
		ptTmp  = ptFront;
		nCount = sizeChar.cy;
		ptTmp.x += nFrontPosX[nDirection];
		for (i = 0; i < nCount; i ++) {
			aptPos.Add (ptTmp);
			ptTmp.y ++;
		}
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetFrontMapPos									 */
/* ���e		:����O�̃}�b�v���W���擾										 */
/* ���t		:2008/05/24														 */
/* ========================================================================= */

void CInfoCharBase::GetFrontMapPos(
	POINT &ptDst,				/* [ou] �擾�� */
	int nDirection	/*-1*/)		/* [in] ���� */
{
	int x, y, nFrontPosX[] = {0, 0, -1, 1, 1, 1, -1, -1}, nFrontPosY[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	SIZE sizeChar;

	if (nDirection == -1) {
		nDirection = m_nDirection;
	}

	sizeChar.cx = sizeChar.cy = 1;
	GetCharSize (sizeChar);
	if (sizeChar.cx > 1) {
		sizeChar.cx --;
	}
	if (sizeChar.cy > 1) {
		sizeChar.cy --;
	}
	x = m_nMapX;
	y = m_nMapY;
	if (nFrontPosX[nDirection] == 1) {
		x += sizeChar.cx;
	}
	if (nFrontPosY[nDirection] == 1) {
		y += sizeChar.cy;
	}
	if (x % 2) {
		x -= (nFrontPosX[nDirection] * sizeChar.cx);
	}
	if (y % 2) {
		y -= (nFrontPosY[nDirection] * sizeChar.cy);
	}
	ptDst.x = x / 2 + nFrontPosX[nDirection];
	ptDst.y = y / 2 + nFrontPosY[nDirection];
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetDirection									 */
/* ���e		:�w����W�̌������擾											 */
/* ���t		:2007/09/16														 */
/* ========================================================================= */

int CInfoCharBase::GetDirection(
	int x,		/* [in] �ړ���̍��W(X) */
	int y)		/* [in] �ړ���̍��W(Y) */
{
	int nRet;

	nRet = -1;

	if (y < m_nMapY) {
		if (x < m_nMapX) {
			nRet = 7;
		} else if (x > m_nMapX) {
			nRet = 4;
		} else {
			nRet = 0;
		}

	} else if (y > m_nMapY) {
		if (x < m_nMapX) {
			nRet = 6;
		} else if (x > m_nMapX) {
			nRet = 5;
		} else {
			nRet = 1;
		}
	} else {
		if (x < m_nMapX) {
			nRet = 2;
		} else if (x > m_nMapX) {
			nRet = 3;
		}
	}

	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetAtackType									 */
/* ���e		:�U����ʂ��擾													 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

int CInfoCharBase::GetAtackType(void)
{
	int nRet;

	nRet = 0;

//Todo:�摜�ł͂Ȃ�����̑���������悤��
	switch (m_wGrpIDArmsMain) {
	case 0:		/* ���� */
		nRet = WEAPONTYPE_SWING;
		break;
//	case 1:		/* �U��E�˂� */
//		nRet = WEAPONTYPE_POKE;
//		break;
	case 1:		/* �| */
		nRet = WEAPONTYPE_BOW;
		break;
	}

	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::SetAtackMotion									 */
/* ���e		:�U�����[�V�����̐ݒ�											 */
/* ���t		:2007/12/05														 */
/* ========================================================================= */

void CInfoCharBase::SetAtackMotion(
	int nMotionID)		/* [in] ���[�V����ID(4�����̐擪) */
{
	m_adwMotionID[CHARMOTIONID_ATACK] = nMotionID;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetDrawDirection								 */
/* ���e		:�`��p��4�����Ō������擾										 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

int CInfoCharBase::GetDrawDirection(
	int nDirection/*-1*/)		/* [in] ���� */
{
	int anDirection[] = {0, 1, 2, 3, 0, 1, 1, 0};

	if (nDirection == -1) {
		nDirection = m_nDirection;
	}

	return anDirection[nDirection];
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetBackDirection								 */
/* ���e		:���Ε������擾													 */
/* ���t		:2008/11/24														 */
/* ========================================================================= */

int CInfoCharBase::GetBackDirection(void)
{
	int anDirection[] = {1, 0, 3, 2, 6, 7, 5, 4};

	return anDirection[m_nDirection];
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetMoveWait										 */
/* ���e		:��Ԃɉ������ړ����x���擾										 */
/* ���t		:2008/08/09														 */
/* ========================================================================= */

DWORD CInfoCharBase::GetMoveWait(void)
{
	DWORD dwRet;

	dwRet = m_dwMoveWait;
	if (IsStateBattle ()) {
		if (m_dwMoveWaitBattle == 0) {
			dwRet = (DWORD)((float)dwRet * 1.4f);
		} else {
			dwRet = m_dwMoveWaitBattle;
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::SetItem											 */
/* ���e		:�A�C�e������ݒ�												 */
/* ���t		:2007/08/05														 */
/* ========================================================================= */

void CInfoCharBase::SetItem(ARRAYDWORD *padwItemID)
{
	m_adwItemID.Copy (padwItemID);
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::IsItemAdd										 */
/* ���e		:�A�C�e����ǉ��ł��邩����										 */
/* ���t		:2007/08/11														 */
/* ========================================================================= */

BOOL CInfoCharBase::IsItemAdd(void)
{
	BOOL bRet;
	int nCount;

	bRet = FALSE;

	nCount = m_adwItemID.GetSize ();
	/* �������ĂȂ��H */
	if (nCount >= m_nMaxItemCount) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::DeleteItem										 */
/* ���e		:�����A�C�e�����폜												 */
/* ���t		:2007/08/19														 */
/* ========================================================================= */

void CInfoCharBase::DeleteItem(DWORD dwItemID)
{
	//Todo:�����i�̉����Ȃǂ̏�������������
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::HaveItem										 */
/* ���e		:�w��A�C�e���������Ă��邩����									 */
/* ���t		:2008/06/10														 */
/* ========================================================================= */

BOOL CInfoCharBase::HaveItem(DWORD dwItemID)
{
	BOOL bRet;
	int i, nCount;

	bRet = FALSE;
	nCount = m_adwItemID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		if (m_adwItemID[i] == dwItemID) {
			break;
		}
	}
	if (i >= nCount) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetTargetPos									 */
/* ���e		:�w����W���擾													 */
/* ���t		:2008/08/09														 */
/* ========================================================================= */

void CInfoCharBase::GetTargetPos(
	POINT *ptTarget,		/* [in] �ŏI�ړ��� */
	POINT &ptDst,			/* [out] ���� */
	int nCount)				/* [in] �w����� */
{
	int i, E, dx, dy, sx, sy, x0, x1, y0, y1, nCountTmp;

	ptDst.x = ptDst.y = -1;
	x0 = m_nMapX;
	y0 = m_nMapY;
	x1 = ptTarget->x;
	y1 = ptTarget->y;

	dx = (x1 > x0) ? x1 - x0 : x0 - x1;
	dy = (y1 > y0) ? y1 - y0 : y0 - y1;
	sx = (x1 > x0) ? 1 : -1;
	sy = (y1 > y0) ? 1 : -1;

	nCountTmp = 0;
	/* �X����1�ȉ��̏ꍇ */
	if (dx >= dy) {
		E = -dx;
		for (i = 0 ; i <= dx ; i ++) {
			if (nCount >= 0) {
				if (nCountTmp >= nCount) {
					ptDst.x = x0;
					ptDst.y = y0;
					break;
				}
			}
			x0 += sx;
			E += 2 * dy;
			if (E >= 0) {
				y0 += sy;
				E -= 2 * dx;
			}
			nCountTmp ++;
		}

	/* �X����1���傫���ꍇ */
	} else {
		E = -dy;
		for (i = 0 ; i <= dy ; i ++) {
			if (nCount >= 0) {
				if (nCountTmp >= nCount) {
					ptDst.x = x0;
					ptDst.y = y0;
					break;
				}
			}
			y0 += sy;
			E += 2 * dx;
			if (E >= 0) {
				x0 += sx;
				E -= 2 * dy;
			}
			nCountTmp ++;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::SetTarget										 */
/* ���e		:�^�[�Q�b�g�L������ݒ�											 */
/* ���t		:2008/08/09														 */
/* ========================================================================= */

void CInfoCharBase::SetTarget(CInfoCharBase *pCharTarget)
{
	BOOL bClear;

	bClear = FALSE;
	if (pCharTarget == NULL) {
		bClear = TRUE;
	} else {
		if (pCharTarget->IsLogin () == FALSE) {
			bClear = TRUE;
		}
		if (pCharTarget->IsAtackTarget () == FALSE) {
			bClear = TRUE;
		}
		if (pCharTarget->m_dwMapID != m_dwMapID) {
			bClear = TRUE;
		}
	}

	if (bClear) {
		m_dwTargetCharID = 0;
		m_ptTargetPos.x = m_ptTargetPos.y = -1;
	} else {
		m_dwTargetCharID	= pCharTarget->m_dwCharID;
		m_ptTargetPos.x		= pCharTarget->m_nMapX;
		m_ptTargetPos.y		= pCharTarget->m_nMapY;
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::SetPos											 */
/* ���e		:���W���w��														 */
/* ���t		:2008/04/29														 */
/* �߂�l	:�ړ�����														 */
/* ========================================================================= */

int CInfoCharBase::SetPos(int x, int y, BOOL bBack/*FALSE*/)
{
	int nRet;

	nRet = -1;
	if ((x == m_nMapX) && (y < m_nMapY)) {
		nRet = 0;
	} else if ((x == m_nMapX) && (y > m_nMapY)) {
		nRet = 1;
	} else if ((x < m_nMapX) && (y == m_nMapY)) {
		nRet = 2;
	} else if ((x > m_nMapX) && (y == m_nMapY)) {
		nRet = 3;
	} else if ((x > m_nMapX) && (y < m_nMapY)) {
		nRet = 4;
	} else if ((x > m_nMapX) && (y > m_nMapY)) {
		nRet = 5;
	} else if ((x < m_nMapX) && (y > m_nMapY)) {
		nRet = 6;
	} else if ((x < m_nMapX) && (y < m_nMapY)) {
		nRet = 7;
	}

	if ((m_nMapX != x) || (m_nMapY != y)) {
		m_ptCharBack.x = m_nMapX;
		m_ptCharBack.y = m_nMapY;
	}
	m_nMapX = x;
	m_nMapY = y;
	if (bBack) {
		m_ptCharBack.x = x;
		m_ptCharBack.y = y;
	}
	m_dwLastTimeMove = timeGetTime ();

	m_nMoveDirection = nRet;
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::IsMove											 */
/* ���e		:�ړ���������													 */
/* ���t		:2007/01/21														 */
/* �߂�l	:TRUE:�ړ��� FALSE:��~��										 */
/* ========================================================================= */

BOOL CInfoCharBase::IsMove(void)
{
	BOOL bRet;

	bRet = FALSE;

	switch (m_nMoveState) {
	case CHARMOVESTATE_MOVE:		/* �ړ��� */
	case CHARMOVESTATE_BATTLEMOVE:	/* �퓬�ړ��� */
	case CHARMOVESTATE_BATTLEATACK:	/* �퓬�U���� (�ړ��ł��Ȃ��Ƃ����Ӗ���) */
		bRet = TRUE;
		break;
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::SetDirection									 */
/* ���e		:�������w��														 */
/* ���t		:2008/04/29														 */
/* ========================================================================= */

void CInfoCharBase::SetDirection(int nDirection)
{
	if (m_nDirection != nDirection) {
		m_nDirectionBack = m_nDirection;
	}
	m_nDirection = nDirection;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::SetMoveState									 */
/* ���e		:�ړ���Ԃ�ύX													 */
/* ���t		:2007/02/26														 */
/* ========================================================================= */

void CInfoCharBase::SetMoveState(int nMoveState)
{
	m_nMoveState = nMoveState;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::SetProcState									 */
/* ���e		:�s����Ԃ�ύX													 */
/* ���t		:2008/06/11														 */
/* ========================================================================= */

void CInfoCharBase::SetProcState(int nProcState)
{
	m_nProcState = nProcState;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::SetName											 */
/* ���e		:�L���������X�V													 */
/* ���t		:2007/01/28														 */
/* ========================================================================= */

void CInfoCharBase::SetName(LPCSTR pszName)
{
	m_strCharName = pszName;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::SetSpeak										 */
/* ���e		:�������e���X�V													 */
/* ���t		:2007/02/04														 */
/* ========================================================================= */

void CInfoCharBase::SetSpeak(LPCSTR pszSpeak)
{
	m_strSpeak = pszSpeak;

	if (m_strSpeak.IsEmpty ()) {
		m_dwLastTimeSpeak = 0;

	} else {
		m_dwLastTimeSpeak = timeGetTime ();
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::Copy											 */
/* ���e		:�R�s�[															 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

void CInfoCharBase::Copy(CInfoCharBase *pSrc)
{
	int i, nCount;

	m_dwLightTime				= m_dwLightTime;				/* ����L������ */
	m_nLightLevel				= m_nLightLevel;				/* ���背�x�� */

	m_dwAccountID				= pSrc->m_dwAccountID;
	m_dwTailCharID				= pSrc->m_dwTailCharID;
	m_dwFrontCharID				= pSrc->m_dwFrontCharID;
	m_dwParentCharID			= pSrc->m_dwParentCharID;
	m_bBlock					= pSrc->m_bBlock;
	m_bPush						= pSrc->m_bPush;
	m_nAnime					= pSrc->m_nAnime;
	m_nMapX						= pSrc->m_nMapX;
	m_nMapY						= pSrc->m_nMapY;
	m_nMoveState				= pSrc->m_nMoveState;
	m_nMoveType					= pSrc->m_nMoveType;
	m_nDirection				= pSrc->m_nDirection;
	m_nGrpSize					= pSrc->m_nGrpSize;
	m_nSex						= pSrc->m_nSex;
	m_nMaxItemCount				= pSrc->m_nMaxItemCount;
	m_nDropItemAverage			= pSrc->m_nDropItemAverage;		/* �A�C�e���h���b�v�� */
	m_nMoveAverage				= pSrc->m_nMoveAverage;			/* �ړ��m�� */
	m_nMoveAverageBattle		= pSrc->m_nMoveAverageBattle;	/* �퓬���ړ��m�� */
	m_bChargeAtack				= pSrc->m_bChargeAtack;			/* ���ߍU�� */
	m_wFamilyID					= pSrc->m_wFamilyID;
	m_wGrpIDNPC					= pSrc->m_wGrpIDNPC;
	m_wGrpIDCloth				= pSrc->m_wGrpIDCloth;
	m_wGrpIDEye					= pSrc->m_wGrpIDEye;
	m_wGrpIDEyeColor			= pSrc->m_wGrpIDEyeColor;
	m_wGrpIDHairType			= pSrc->m_wGrpIDHairType;
	m_wGrpIDHairColor			= pSrc->m_wGrpIDHairColor;
	m_wGrpIDSP					= pSrc->m_wGrpIDSP;
	m_wGrpIDTmpMain				= pSrc->m_wGrpIDTmpMain;		/* �摜ID(�ꎞ��:���C��) */
	m_wGrpIDTmpSub				= pSrc->m_wGrpIDTmpSub;			/* �摜ID(�ꎞ��:�T�u) */
	m_wGrpIDAcce				= pSrc->m_wGrpIDAcce;
	m_wGrpIDArmsMain			= pSrc->m_wGrpIDArmsMain;
	m_wGrpIDArmsSub				= pSrc->m_wGrpIDArmsSub;
	m_wGrpIDArmsLeftMain		= pSrc->m_wGrpIDArmsLeftMain;
	m_wGrpIDArmsLeftSub			= pSrc->m_wGrpIDArmsLeftSub;
	m_wGrpIDInitNPC				= pSrc->m_wGrpIDInitNPC;
	m_wGrpIDInitCloth			= pSrc->m_wGrpIDInitCloth;
	m_wGrpIDInitEye				= pSrc->m_wGrpIDInitEye;
	m_wGrpIDInitEyeColor		= pSrc->m_wGrpIDInitEyeColor;
	m_wGrpIDInitHairType		= pSrc->m_wGrpIDInitHairType;
	m_wGrpIDInitHairColor		= pSrc->m_wGrpIDInitHairColor;
	m_wGrpIDInitSP				= pSrc->m_wGrpIDInitSP;
	m_wAtackGauge				= pSrc->m_wAtackGauge;			/* �A�^�b�N�Q�[�W */
	m_wDefenseGauge				= pSrc->m_wDefenseGauge;		/* �f�B�t�F���X�Q�[�W */
	m_wLevel					= pSrc->m_wLevel;				/* ���x�� */
	m_wStamina					= pSrc->m_wStamina;				/* �X�^�~�i */
	m_wPower					= pSrc->m_wPower;				/* �r�� */
	m_wStrength					= pSrc->m_wStrength;			/* �̗� */
	m_wMagic					= pSrc->m_wMagic;				/* ���� */
	m_wSkillful					= pSrc->m_wSkillful;			/* ��p */
	m_wAbillityAT				= pSrc->m_wAbillityAT;			/* �U���Z�\ */
	m_wAbillityDF				= pSrc->m_wAbillityDF;			/* �h��Z�\ */
	m_wPAtack					= pSrc->m_wPAtack;				/* �U���� */
	m_wPDefense					= pSrc->m_wPDefense;			/* �h��� */
	m_wPMagic					= pSrc->m_wPMagic;				/* ���@�� */
	m_wPMagicDefense			= pSrc->m_wPMagicDefense;		/* ���@�h��� */
	m_wPHitAverage				= pSrc->m_wPHitAverage;			/* ������ */
	m_wPAvoidAverage			= pSrc->m_wPAvoidAverage;		/* ��� */
	m_wPCriticalAverage			= pSrc->m_wPCriticalAverage;	/* �N���e�B�J���� */
	m_wAttrFire					= pSrc->m_wAttrFire;			/* ����[��] */
	m_wAttrWind					= pSrc->m_wAttrWind;			/* ����[��] */
	m_wAttrWater				= pSrc->m_wAttrWater;			/* ����[��] */
	m_wAttrEarth				= pSrc->m_wAttrEarth;			/* ����[�y] */
	m_wAttrLight				= pSrc->m_wAttrLight;			/* ����[��] */
	m_wAttrDark					= pSrc->m_wAttrDark;			/* ����[��] */
	m_dwCharID					= pSrc->m_dwCharID;
	m_dwMapID					= pSrc->m_dwMapID;
	m_dwMotionTypeID			= pSrc->m_dwMotionTypeID;
	m_dwEquipItemIDCloth		= pSrc->m_dwEquipItemIDCloth;
	m_dwEquipItemIDAcce1		= pSrc->m_dwEquipItemIDAcce1;
	m_dwEquipItemIDAcce2		= pSrc->m_dwEquipItemIDAcce2;
	m_dwEquipItemIDArmsRight	= pSrc->m_dwEquipItemIDArmsRight;
	m_dwEquipItemIDArmsLeft		= pSrc->m_dwEquipItemIDArmsLeft;
	m_dwEquipItemIDHead			= pSrc->m_dwEquipItemIDHead;
	m_dwMoveWait				= pSrc->m_dwMoveWait;			/* �ړ��҂����� */
	m_dwMoveWaitBattle			= pSrc->m_dwMoveWaitBattle;		/* �퓬���ړ��҂����� */
	m_dwExp						= pSrc->m_dwExp;				/* �o���l */
	m_dwHP						= pSrc->m_dwHP;
	m_dwMaxHP					= pSrc->m_dwMaxHP;
	m_dwSP						= pSrc->m_dwSP;
	m_dwMaxSP					= pSrc->m_dwMaxSP;
	m_clName					= pSrc->m_clName;
	m_clSpeak					= pSrc->m_clSpeak;
	m_strTalk					= pSrc->m_strTalk;				/* ��b�f�[�^ */

	m_abyMark.RemoveAll ();
	nCount = pSrc->m_abyMark.GetSize ();
	for (i = 0; i < nCount; i ++) {
		m_abyMark.Add (pSrc->m_abyMark[i]);
	}
	m_adwItemID.RemoveAll ();
	nCount = pSrc->m_adwItemID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		m_adwItemID.Add (pSrc->m_adwItemID[i]);
	}
	m_adwSkillID.RemoveAll ();
	nCount = pSrc->m_adwSkillID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		m_adwSkillID.Add (pSrc->m_adwSkillID[i]);
	}

	SetName (pSrc->m_strCharName);
	SetSpeak (pSrc->m_strSpeak);

	m_dwPutCycle		= pSrc->m_dwPutCycle;		/* �������� */
	m_nPutMoveType		= pSrc->m_nPutMoveType;		/* ����������ړ���� */
	m_nMaxPutCount		= pSrc->m_nMaxPutCount;		/* ���������� */
	m_nPutAverage		= pSrc->m_nPutAverage;		/* �����m�� */
	m_ptPutArea			= pSrc->m_ptPutArea;		/* �����͈�(���a) */
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::TimerProc										 */
/* ���e		:���ԏ���														 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CInfoCharBase::TimerProc(DWORD dwTime)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_dwLastTimeSpeak > 0) {
		if (timeGetTime () - m_dwLastTimeSpeak > 1000 * 10) {
			/* �P�O�b�o�߂����̂ŏ��� */
			SetSpeak ("");
			bRet = TRUE;
		}
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::RenewBlockMapArea								 */
/* ���e		:�����蔻��Ώۂ̃}�b�v���W���X�V								 */
/* ���t		:2008/04/13														 */
/* ========================================================================= */

void CInfoCharBase::RenewBlockMapArea(
	int x,				/* [in] �ړ���̃L�������W(��) */
	int y,				/* [in] �ړ���̃L�������W(�c) */
	int nDirection,		/* [in] ���ׂ���� */
	BOOL bMoveOut)		/* [in] TRUE:�O�ւ̈ړ��p�ɋ������̂ݑΏۂƂ��� */
{
	int anPosX[] = {0, 0, -1, 1, 1, 1, -1, -1}, anPosY[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	POINT ptTmp, ptTmpBack;

	m_aposBockMapArea.RemoveAll ();
	if (nDirection < 0) {
		/* �N���A�̂� */
		return;
	}

//todo:�L�����T�C�Y

	if (bMoveOut) {
		if (nDirection < 4) {
			if (anPosY[nDirection] != 0) {
				if (y % 2) {
					return;
				}
			} else if (anPosX[nDirection] != 0) {
				if (x % 2) {
					return;
				}
			}
		} else {
			if (anPosY[nDirection] != 0) {
				if (y % 2) {
					if (anPosX[nDirection] != 0) {
						if (x % 2) {
							return;
						}
					}
				}
			}
		}
	}

	ptTmp.x = x / 2;
	if (x < 0) {
		ptTmp.x = -1;
	}
	switch (nDirection) {
	case 3:
	case 4:
	case 5:
		if (x % 2) {
			ptTmp.x += anPosX[nDirection];
		}
		break;
	}
	if (y % 2) {
		y ++;
	}
	ptTmp.y = y / 2;
	if (y < 0) {
		ptTmp.y = -1;
	}
	switch (nDirection) {
	case 1:
	case 5:
	case 6:
		if (y % 2) {
			ptTmp.y += anPosY[nDirection];
		}
		break;
	}
	m_aposBockMapArea.Add (ptTmp);

	ptTmpBack = ptTmp;
	if (x % 2) {
		switch (nDirection) {
		case 0:
		case 1:
		case 4:
		case 5:
			ptTmp.x ++;
			m_aposBockMapArea.Add (ptTmp);
			break;
		}
	}
}

/* ========================================================================= */
/* �֐���	:CInfoCharBase::IsHitCharPos									 */
/* ���e		:�L�������W�Ƃ̓����蔻��										 */
/* �߂�l	:TRUE:������													 */
/* ���t		:2008/04/29														 */
/* ========================================================================= */

BOOL CInfoCharBase::IsHitCharPos(
	int x,					/* [in] �L�������W(��) */
	int y,					/* [in] �L�������W(�c) */
	SIZE *psize/*NULL*/)	/* [in] �L�����T�C�Y */
{
	BOOL bRet;
	int nPos;
	SIZE size;
	RECT rcSrc, rcTarget;

	bRet = FALSE;

	if (psize == NULL) {
		nPos = abs (x - m_nMapX);
		if (nPos != 0) {
			goto Exit;
		}
		nPos = y - m_nMapY;
		if (nPos != 0) {
			goto Exit;
		}

	} else {
		GetCharSize (size);
		SetRect (&rcSrc, m_nMapX, m_nMapY, m_nMapX + size.cx - 1, m_nMapY + size.cy - 1);
		SetRect (&rcTarget, x, y, x + psize->cx - 1, y + psize->cy - 1);
		if (!((rcSrc.left <= rcTarget.right) && (rcTarget.left <= rcSrc.right) &&
			(rcSrc.top <= rcTarget.bottom) && (rcTarget.top <= rcSrc.bottom))) {
				goto Exit;
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetViewCharPos									 */
/* ���e		:�\���p�̍��W�␳�l���擾										 */
/* ���t		:2008/05/01														 */
/* ����		:�\�����鎞�ɍ��W���炸�炷�l���擾����							 */
/* ========================================================================= */

void CInfoCharBase::GetViewCharPos(POINT &ptDst)
{
	ptDst = m_ptViewCharPos;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::GetCharSize										 */
/* ���e		:�L�����̕��ƍ������擾											 */
/* ���t		:2008/05/01														 */
/* ========================================================================= */

void CInfoCharBase::GetCharSize(SIZE &sizeDst)
{
	sizeDst.cx = 2;
	sizeDst.cy = 1;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::IsEnableMove									 */
/* ���e		:�ړ��ł����Ԃ�����											 */
/* ���t		:2008/06/11														 */
/* �߂�l	:TRUE:�ړ���													 */
/* ========================================================================= */

BOOL CInfoCharBase::IsEnableMove(void)
{
	BOOL bRet;

	bRet = FALSE;
	if (m_nProcState == CHARPROCSTATEID_FISHING) {
		goto Exit;
	}
	if (IsMove ()) {
		goto Exit;
	}
	/* ���Ă����Ă���H */
	if (m_dwFrontCharID) {
		goto Exit;
	}

	switch (m_nMoveState) {
	case CHARMOVESTATE_SWOON:				/* �C�� */
	case CHARMOVESTATE_BATTLEATACK:			/* �퓬�U���� */
	case CHARMOVESTATE_BATTLEATACK_WAIT:	/* �퓬�U����̑҂����� */
	case CHARMOVESTATE_BATTLE_DEFENSE:		/* �h�䒆 */
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::IsEnableAtack									 */
/* ���e		:�U���ł����Ԃ�����											 */
/* ���t		:2008/07/26														 */
/* �߂�l	:TRUE:�U����													 */
/* ========================================================================= */

BOOL CInfoCharBase::IsEnableAtack(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_dwHP == 0) {
		goto Exit;
	}
	switch (m_nMoveState) {
	case CHARMOVESTATE_SWOON:				/* �C�� */
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::IsAtackTarget									 */
/* ���e		:�U���ΏۂƂȂ邩����											 */
/* ���t		:2008/07/12														 */
/* �߂�l	:TRUE:�U����													 */
/* ========================================================================= */

BOOL CInfoCharBase::IsAtackTarget(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_dwHP == 0) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::IsLogoutDelete									 */
/* ���e		:���O�A�E�g���ɍ폜���邩����									 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

BOOL CInfoCharBase::IsLogoutDelete(void)
{
	return m_bDelete;
}

/* Copyright(C)URARA-works 2006 */
