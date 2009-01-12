/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharBase.h												 */
/* ���e			:�L���������N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/01													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"
#include "InfoMotion.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* ���[�V�������ID */
enum {
	CHARMOTIONID_STAND = 0,		/* ���� */
	CHARMOTIONID_WALK,			/* ���� */
	CHARMOTIONID_SIT,			/* ���� */
	CHARMOTIONID_BATTLESTAND,	/* �퓬���� */
	CHARMOTIONID_BATTLEWALK,	/* ���葫 */
	CHARMOTIONID_ATACK,			/* �U�� */
	CHARMOTIONID_BATTLEDEFENSE,	/* �h�� */
	CHARMOTIONID_ANIME,			/* �A�j���[�V���� */
	CHARMOTIONID_INTERRUUPT,	/* ���荞�ݍĐ� */
	CHARMOTIONID_MAX
};

/* �s����� */
enum {
	CHARPROCSTATEID_NORMAL = 0,		/* �ʏ��� */
	CHARPROCSTATEID_FISHING,		/* �ނ��� */
	CHARPROCSTATEID_MAX
};

/* �L�����ړ���� */
enum {
	CHARMOVETYPE_PC = 0,							/* �v���C���[�L���� */
	CHARMOVETYPE_STAND,								/* �ړ����Ȃ� */
	CHARMOVETYPE_BALL,								/* �{�[�� */
	CHARMOVETYPE_SCORE,								/* ���_ */
	CHARMOVETYPE_STYLECOPY_PUT,						/* �e�p�R�s�[(��荞��) */
	CHARMOVETYPE_ATACKANIME,						/* �U���󂯂�ƃA�j���[�V���� */
	CHARMOVETYPE_MOVEATACK,							/* �ړ����čU�� */
	CHARMOVETYPE_STYLECOPY_GET,						/* �e�p�R�s�[(���f) */
	CHARMOVETYPE_PUTNPC,							/* NPC���� */
	CHARMOVETYPE_BATTLE1,							/* �퓬1 */
	CHARMOVETYPE_MAX
};

/* �L�����ړ���� */
enum {
	CHARMOVESTATE_STAND = 0,						/* ���� */
	CHARMOVESTATE_SIT,								/* ���蒆 */
	CHARMOVESTATE_MOVE,								/* �ړ��� */
	CHARMOVESTATE_DELETEREADY,						/* �������� */
	CHARMOVESTATE_DELETE,							/* ���� */
	CHARMOVESTATE_BATTLE,							/* �퓬�� */
	CHARMOVESTATE_BATTLEMOVE,						/* �퓬�ړ��� */
	CHARMOVESTATE_BATTLEATACK,						/* �퓬�U���� */
	CHARMOVESTATE_BATTLEATACK_WAIT,					/* �퓬�U����̑҂����� */
	CHARMOVESTATE_ANIME,							/* �A�j���[�V���� */
	CHARMOVESTATE_WALKANIME,						/* ������ */
	CHARMOVESTATE_SWOON,							/* �C�� */
	CHARMOVESTATE_DAMAGE,							/* �_���[�W */
	CHARMOVESTATE_BATTLE_DEFENSE,					/* �h�䒆 */
	CHARMOVESTATE_SLEEPTIMER,						/* ���₷�݃^�C�}�[�� */
	CHARMOVESTATE_MAX
};

#define MAX_ATACKGAUGE		100						/* �A�^�b�N�Q�[�W�ő�l */
#define MAX_DEFENSEGAUGE	100						/* �K�[�h�Q�[�W�ő�l */


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoCharBase : public CInfoBase
{
public:
			CInfoCharBase();								/* �R���X�g���N�^ */
	virtual ~CInfoCharBase();								/* �f�X�g���N�^ */

	int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^��ݒ� */

	BOOL	IsLogin				(void);								/* ���O�C���������� */
	BOOL	IsStateBattle		(void);								/* �퓬���[�h������ */
	BOOL	IsStateMove			(void);								/* �ړ��������� */
	BOOL	IsViewArea			(DWORD dwMapID, POINT *pptPos);		/* ������͈͂����� */
	BOOL	IsNPC				(void);								/* NPC������ */
	BOOL	CheckSessionID		(DWORD dwSessionID);				/* �Z�b�V����ID���`�F�b�N */
	void	GetFrontPos			(POINT &ptDst, int nDirection = -1, BOOL bMove=FALSE);	/* ����O�̃L�������W���擾 */
	void	GetFrontPos			(CmyArray<POINT, POINT> &aptPos, int nDirection = -1);	/* ����O�̃L�������W���擾 */
	void	GetFrontMapPos		(POINT &ptDst, int nDirection = -1);					/* ����O�̃}�b�v���W���擾 */
	int		GetDirection		(int x, int y);						/* �w����W�̌������擾 */
	int		GetAtackType		(void);								/* �U����ʂ��擾 */
	void	SetAtackMotion		(int nMotionID);					/* �U�����[�V�����̐ݒ� */
	int		GetDrawDirection	(int nDirection = -1);				/* �`��p��4�����Ō������擾 */
	int		GetBackDirection	(void);								/* ���Ε������擾 */
	DWORD	GetMoveWait			(void);								/* ��Ԃɉ������ړ����x���擾 */

	void		SetItem			(ARRAYDWORD *padwItemID);						/* �A�C�e������ݒ� */
	ARRAYDWORD	*GetItem		(void)	{ return &m_adwItemID; }				/* �A�C�e�������擾 */
	BOOL		IsItemAdd		(void);											/* �A�C�e����ǉ��ł��邩���� */
	void		DeleteItem		(DWORD dwItemID);								/* �����A�C�e�����폜 */
	BOOL		HaveItem		(DWORD dwItemID);								/* �w��A�C�e���������Ă��邩���� */
	void		GetTargetPos	(POINT *ptTarget, POINT &ptDst, int nCount);	/* �w����W���擾 */
	void		SetTarget		(CInfoCharBase *pCharTarget);					/* �^�[�Q�b�g�L������ݒ� */
	void		SetSkill		(ARRAYDWORD *padwSkillID);						/* �X�L������ݒ� */
	ARRAYDWORD	*GetSkill		(void)	{ return &m_adwSkillID; }				/* �X�L�������擾 */
	BOOL		HaveSkill		(DWORD dwSkillID);								/* �w��X�L���������Ă��邩���� */

	virtual BOOL IsMove				(void);							/* �ړ��������� */
	virtual int  SetPos				(int x, int y, BOOL bBack = FALSE);	/* ���W���w�� */
	virtual void SetDirection		(int nDirection);				/* �������w�� */
	virtual void SetMoveState		(int nMoveState);				/* �ړ���Ԃ�ύX */
	virtual void SetProcState		(int nProcState);				/* �s����Ԃ�ύX */
	virtual void SetName			(LPCSTR pszName);				/* �L���������X�V */
	virtual void SetSpeak			(LPCSTR pszSpeak);				/* �������e���X�V */
	virtual void Copy				(CInfoCharBase *pSrc);			/* �R�s�[ */
	virtual BOOL TimerProc			(DWORD dwTime);					/* ���ԏ��� */
	virtual void RenewBlockMapArea	(int x, int y, int nDirection, BOOL bMoveOut = FALSE);	/* �����蔻��Ώۂ̃}�b�v���W���X�V */
	virtual BOOL IsHitCharPos		(int x, int y, SIZE *psize = NULL);						/* �L�������W�Ƃ̓����蔻�� */
	virtual void GetViewCharPos		(POINT &ptDst);					/* �\���p�̍��W�␳�l���擾 */
	virtual void GetCharSize		(SIZE &sizeDst);				/* �L�����̕��ƍ������擾 */
	virtual BOOL IsEnableMove		(void);							/* �ړ��ł����Ԃ����� */
	virtual BOOL IsEnableAtack		(void);							/* �U���ł����Ԃ����� */
	virtual BOOL IsAtackTarget		(void);							/* �U���ΏۂƂȂ邩���� */
	virtual BOOL IsLogoutDelete		(void);							/* ���O�A�E�g���ɍ폜���邩���� */


public:
	/* �ۑ����Ȃ��f�[�^ */
	DWORD		m_dwSessionID,						/* ���쒆�̃Z�b�V����ID */
				m_dwAccountID,						/* �A�J�E���gID */
				m_dwLastTimeSpeak,					/* �ŏI�������� */
				m_dwLastTimeMove,					/* �ŏI�ړ��������� */
				m_dwTailCharID,						/* �t���ė��Ă���L����ID */
				m_dwFrontCharID,					/* �t���Ă���L����ID */
				m_dwParentCharID,					/* �e�̃L����ID */
				m_dwTargetCharID,					/* �^�[�Q�b�g�L����ID */
				m_adwMotionID[CHARMOTIONID_MAX],	/* ���[�V����ID */
				m_dwLightTime;						/* ����L������ */
	WORD		m_wAtackGauge,						/* �A�^�b�N�Q�[�W */
				m_wDefenseGauge;					/* �K�[�h�Q�[�W */
	POINT		m_ptCharBack,						/* �O��̃L�������W */
				m_ptViewCharPos,					/* �\���p�̍��W�␳�l */
				m_ptTargetPos;						/* �^�[�Q�b�g���W */
	int			m_nAnime,							/* �A�j���[�V�����ԍ� */
				m_nDirectionBack,					/* �O��̌��� */
				m_nProcState,						/* �s����� */
				m_nMoveDirection,					/* �ړ����� */
				m_nLightLevel,						/* ���背�x�� */
				m_nAtackTarget;						/* �U���Ώ� */
	BOOL		m_bDelete,							/* �폜�t���O */
				m_bNPC,								/* NPC */
				m_bChargeAtack,						/* ���ߍU�� */
				m_bParentInfo;						/* �e�̏����Q�Ƃ��� */
	ARRAYBYTE	m_abyMark;							/* ���O�̑O�ɕ\������}�[�N�ԍ� */
	CmyArray<POINT, POINT> m_aposBockMapArea;		/* �����蔻��Ώۂ̃}�b�v���W */

	/* �ۑ�����f�[�^ */
	BOOL		m_bBlock,					/* �Ԃ��锻�� */
				m_bPush;					/* �����锻�� */
	int			m_nMapX,					/* �}�b�v���W(��) */
				m_nMapY,					/* �}�b�v���W(�c) */
				m_nMoveState,				/* �ړ���� */
				m_nMoveType,				/* �ړ���� */
				m_nDirection,				/* ���� */
				m_nGrpSize,					/* �摜�T�C�Y */
				m_nSex,						/* ���� */
				m_nMaxItemCount,			/* �ő�A�C�e�������� */
				m_nDropItemAverage,			/* �A�C�e���h���b�v�� */
				m_nMoveAverage,				/* �ړ��m�� */
				m_nMoveAverageBattle;		/* �퓬���ړ��m�� */
	WORD		m_wFamilyID,				/* �푰ID */
				m_wGrpIDNPC,				/* �摜ID(NPC) */
				m_wGrpIDCloth,				/* �摜ID(��) */
				m_wGrpIDEye,				/* �摜ID(��) */
				m_wGrpIDEyeColor,			/* �摜ID(�ڐF) */
				m_wGrpIDHairType,			/* �摜ID(��) */
				m_wGrpIDHairColor,			/* �摜ID(���F) */
				m_wGrpIDSP,					/* �摜ID(���ꕞ) */
				m_wGrpIDTmpMain,			/* �摜ID(�ꎞ��:���C��) */
				m_wGrpIDTmpSub,				/* �摜ID(�ꎞ��:�T�u) */
				m_wGrpIDAcce,				/* �摜ID(�A�N�Z�T��) */
				m_wGrpIDArmsMain,			/* �摜ID(������:���C��) */
				m_wGrpIDArmsSub,			/* �摜ID(������:�T�u) */
				m_wGrpIDArmsLeftMain,		/* �摜ID(��:���C��) */
				m_wGrpIDArmsLeftSub,		/* �摜ID(��:�T�u) */
				m_wGrpIDInitNPC,			/* �����摜ID(NPC) */
				m_wGrpIDInitCloth,			/* �����摜ID(��) */
				m_wGrpIDInitEye,			/* �����摜ID(��) */
				m_wGrpIDInitEyeColor,		/* �����摜ID(�ڐF) */
				m_wGrpIDInitHairType,		/* �����摜ID(��) */
				m_wGrpIDInitHairColor,		/* �����摜ID(���F) */
				m_wGrpIDInitSP,				/* �����摜ID(���ꕞ) */
				m_wLevel,					/* ���x�� */
				m_wStamina,					/* �X�^�~�i */
				m_wPower,					/* �r�� */
				m_wStrength,				/* �̗� */
				m_wMagic,					/* ���� */
				m_wSkillful,				/* ��p */
				m_wAbillityAT,				/* �U���Z�\ */
				m_wAbillityDF,				/* �h��Z�\ */
				m_wPAtack,					/* �U���� */
				m_wPDefense,				/* �h��� */
				m_wPMagic,					/* ���@�� */
				m_wPMagicDefense,			/* ���@�h��� */
				m_wPHitAverage,				/* ������ */
				m_wPAvoidAverage,			/* ��� */
				m_wPCriticalAverage,		/* �N���e�B�J���� */
				m_wAttrFire,				/* ����[��] */
				m_wAttrWind,				/* ����[��] */
				m_wAttrWater,				/* ����[��] */
				m_wAttrEarth,				/* ����[�y] */
				m_wAttrLight,				/* ����[��] */
				m_wAttrDark;				/* ����[��] */
	DWORD		m_dwCharID,					/* �L����ID */
				m_dwMapID,					/* �}�b�vID */
				m_dwMotionTypeID,			/* ���[�V�������ID */
				m_dwEquipItemIDCloth,		/* �����A�C�e��ID:�� */
				m_dwEquipItemIDAcce1,		/* �����A�C�e��ID:�A�N�Z�T��1 */
				m_dwEquipItemIDAcce2,		/* �����A�C�e��ID:�A�N�Z�T��2 */
				m_dwEquipItemIDArmsRight,	/* �����A�C�e��ID:�E�� */
				m_dwEquipItemIDArmsLeft,	/* �����A�C�e��ID:���� */
				m_dwEquipItemIDHead,		/* �����A�C�e��ID:�� */
				m_dwMoveWait,				/* �ړ��҂����� */
				m_dwMoveWaitBattle,			/* �퓬���ړ��҂����� */
				m_dwExp,					/* �o���l */
				m_dwHP,						/* HP */
				m_dwMaxHP,					/* �ő�HP */
				m_dwSP,						/* SP */
				m_dwMaxSP;					/* �ő�SP */
	COLORREF	m_clName,					/* ���O�̕`��F */
				m_clSpeak;					/* �����̕`��F */
	CmyString	m_strCharName,				/* �L������ */
				m_strSpeak,					/* �������e */
				m_strTalk;					/* ��b�f�[�^ */
	ARRAYDWORD	m_adwItemID,				/* �����A�C�e�� */
				m_adwSkillID;				/* �����X�L�� */

	/* �ړ���ʂɂ��ʏ�� */

	/* NPC���� */
	DWORD		m_dwPutCycle;				/* �������� */
	int			m_nPutMoveType;				/* ����������ړ���� */
	int			m_nMaxPutCount;				/* ���������� */
	int			m_nPutAverage;				/* �����m�� */
	POINT		m_ptPutArea;				/* �����͈�(���a) */
} CInfoCharBase, *PCInfoCharBase;
typedef CmyArray<PCInfoCharBase, PCInfoCharBase>	  ARRAYINFOCHARBASE;
typedef CmyArray<PCInfoCharBase, PCInfoCharBase>	*PARRAYINFOCHARBASE;

/* Copyright(C)URARA-works 2006 */
