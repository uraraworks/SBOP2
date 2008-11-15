/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharCli.h												 */
/* ���e			:�L�������N���C�A���g�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/01													 */
/* ========================================================================= */

#pragma once

#include "InfoCharBase.h"
#include "InfoTextEffect.h"

class CMgrData;
class CMgrSound;
class CImg32;
class CInfoEffect;
class CUraraSockTCPSBO;

/* ========================================================================= */
/* �萔�錾																	 */
/* ========================================================================= */

enum {
	INFOCHARCLI_VIEWSTATE_NONE = 0,
	INFOCHARCLI_VIEWSTATE_FADEIN,						/* �t�F�[�h�C�� */
	INFOCHARCLI_VIEWSTATE_FADEOUT,						/* �t�F�[�h�A�E�g */
};

/* ========================================================================= */
/* �\���̐錾																 */
/* ========================================================================= */

/* ���W�ύX�L���[ */
typedef struct _MOVEPOSQUE {
	int		nState,			/* ��� */
			nDirection;		/* ���� */
	POINT	ptPos;			/* ���W */
} MOVEPOSQUE, *PMOVEPOSQUE;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoCharCli : public CInfoCharBase
{
public:
			CInfoCharCli();								/* �R���X�g���N�^ */
	virtual ~CInfoCharCli();							/* �f�X�g���N�^ */

	void Create			(CMgrData *pMgrData);					/* �쐬 */

			void	ChgDirection	(int nDirection);			/* �����ύX */
	virtual void	ChgMoveState	(int nMoveState);			/* �ړ���ԕύX */
			BOOL	TimerProc		(DWORD dwTime);				/* ���ԏ��� */

	void		SetMoveState		(int nMoveState);			/* �ړ���Ԃ�ύX */
	void		MakeCharGrp			(void);						/* �L�����摜���쐬 */
	void		SetName				(LPCSTR pszName);			/* �L���������X�V */
	void		SetSpeak			(LPCSTR pszSpeak);			/* �������e���X�V */
	void		SetSleepTimer		(BOOL bSleepTimer);			/* ���₷�݃^�C�}�[��ݒ� */

	virtual void		SetViewState	(int nViewState);		/* �\����Ԑݒ� */
	virtual CInfoMotion *GetMotionInfo	(int *pnCount = NULL);	/* ���[�V���������擾 */

	BOOL		IsChgWait			(void);								/* ��ԕύX�҂������� */
	void		SetEffectID			(DWORD dwEffectID);					/* �G�t�F�N�gID���w�� */
	void		SetBalloonID		(DWORD dwBalloonID);				/* ���o��ID���w�� */
	void		GetGrpID			(WORD &wGrpIDMain, WORD &wGrpIDSub);/* �O���t�B�b�NID���擾 */
	void		InitMotionInfo		(DWORD dwMotionID);					/* ���[�V�������̏����� */

	void		GetEfcGrpPos		(POINT &ptDst);						/* �G�t�F�N�g�摜�̊J�n���W���擾 */
	CImg32		*GetEfcImg			(void);								/* �`�挳�̃G�t�F�N�g�摜�C���[�W���擾 */
	int			GetEfcLevel			(void);								/* �G�t�F�N�g�摜�̓��߃��x�����擾 */
	int			GetEfcGrpSize		(void);								/* �G�t�F�N�g�摜�̃T�C�Y���擾 */
	void		GetEfcDrawPos		(POINT &ptDst);						/* �G�t�F�N�g�摜�̕`��␳�����擾 */

	DWORD		GetBalloonGrpID		(void);								/* ���o���摜ID���擾 */
	BOOL		IsEnableMove		(void);								/* �ړ��ł����Ԃ����� */
	BOOL		IsDamage			(void);								/* �_���[�W�󂯒������� */

	void	SetChgWait		(BOOL bChgWait)	{ m_bChgWait = bChgWait; }	/* ��ԕύX�҂����ݒ� */

	/* �����G�t�F�N�g�֘A */
	int		GetTextEffectCount		(void);									/* �o�^�����擾 */
	void	AddTextEffect			(DWORD dwData, int nColor, int nSize);	/* �ǉ� */
	void	AddTextEffect			(LPCSTR pszData, int nColor, int nSize);/* �ǉ� */
	void	DeleteAllTextEffect		(void);									/* �S�č폜 */
	int		GetTextEffectGrpCount	(int nIndex);							/* �摜�����擾 */
	int		GetTextEffectGrpNo		(int nIndex, int nNo);					/* �w��ԍ��̐��l���擾 */
	BOOL	GetTextEffectDrawPos	(int nIndex, int nNo, POINT &ptDst, BYTE &byLevel);	/* �w��ԍ��̕\���ʒu�␳�l���擾 */
	DWORD	GetTextEffectData		(int nIndex);							/* ���l�f�[�^���擾 */
	CInfoTextEffect	*GetTextEffect	(int nIndex);							/* �����G�t�F�N�g���擾 */

	/* ���W�ύX�L���[�֘A */
	void	AddMovePosQue			(int nState, int nDirection, int x, int y);	/* �ǉ� */
	void	DeleteMovePosQue		(int nNo);									/* �폜 */
	void	DeleteAllMovePosQue		(void);										/* �S�č폜 */


protected:
	virtual BOOL RenewAnime			(DWORD dwTime, int nAdd = 1);	/* �A�j���[�V�����̍X�V */

	BOOL TimerProcMove				(DWORD dwTime);				/* �ړ����� */
	BOOL TimerProcViewState			(DWORD dwTime);				/* �\����ԏ��� */
	BOOL TimerProcAtack				(DWORD dwTime);				/* �U������ */
	BOOL TimerProcBalloon			(DWORD dwTime);				/* ���o������ */
	void MotionProc					(DWORD dwProcID);			/* ���[�V�������� */


public:
	BOOL	m_bRedraw,						/* �ĕ`�悳���� */
			m_bChgWait,						/* ��ԕύX�҂� */
			m_bSkipMove,					/* �ړ����Ԃɍ���Ȃ��ꍇ�͑҂����Ԃ𖳎����� */
			m_bWaitCheckMapEvent,			/* �}�b�v�C�x���g�`�F�b�N�҂� */
			m_bMotionDirection,				/* �����ɉ����ă��[�V������؂�ւ��� */
			m_bMotionInterrupt,				/* ���[�V�������荞�ݍĐ� */
			m_bModeSleepTimer;				/* ���₷�݃^�C�}�[ */
	int		m_nAnimeBack,					/* �O��̃A�j���[�V�����R�}�ԍ� */
			m_nViewState,					/* �\����� */
			m_nFadeLevel,					/* ���߃��x�� */
			m_nMoveStateReserve,			/* ���[�V�����I�������ɕύX����ړ���� */
			m_nMoveStateBack;				/* �O��̈ړ���� */
	POINT	m_ptMove;						/* �ړ����̍��W */
	DWORD	m_dwMoveWaitOnce,				/* ��x�����̈ړ��҂����� */
			m_dwLastTimeAnime,				/* �ŏI�A�j���[�V������������ */
			m_dwLastTimeViewState,			/* �ŏI�\������ */
			m_dwBalloonID,					/* ���o��ID */
			m_dwBalloonGrpID,				/* ���o���摜ID */
			m_dwBalloonAnimeID,				/* ���o���R�}�ԍ� */
			m_dwLastTimeBalloon,			/* �ŏI���o���X�V���� */
			m_dwLastTimeDamage;				/* �ŏI�_���[�W�󂯂����� */
	HFONT	m_hFont;						/* �`��Ɏg���t�H���g */

	CMgrData			*m_pMgrData;		/* �f�[�^�}�l�[�W�� */
	CMgrSound			*m_pMgrSound;		/* �T�E���h�Ǘ� */
	CImg32				*m_pDibChar,		/* �L�����摜 */
						*m_pDibName,		/* �L�������摜 */
						*m_pDibSpeak,		/* �������e�摜 */
						*m_pDibBattle;		/* �퓬�p�摜 */
	CInfoEffect			*m_pInfoEffect;		/* �G�t�F�N�g��� */
	CUraraSockTCPSBO	*m_pSock;			/* �ʐM�}�l�[�W�� */
	ARRAYMOTIONINFO		m_aMotion[CHARMOTIONID_MAX][4];	/* ���[�V������� */
	ARRAYTEXTEFFECT		m_aTextEffect;		/* �����G�t�F�N�g */
	CArray<PMOVEPOSQUE, PMOVEPOSQUE>	m_apMovePosQue;	/* ���W�ύX�L���[ */
} CInfoCharCli, *PCInfoCharCli;

/* Copyright(C)URARA-works 2006 */
