/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharSvr.h												 */
/* ���e			:�L�������T�[�o�[�N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/14													 */
/* ========================================================================= */

#pragma once

#include "InfoCharBase.h"

class CInfoMapBase;

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �s��ID */
enum {
	CHARPROCID_NONE = 0,
	CHARPROCID_FISHING,								/* �ނ� */
	CHARPROCID_FISHING_HIT,							/* �ނ�(�q�b�g) */
	CHARPROCID_MAPMOVEIN,							/* �}�b�v���ړ� */
	CHARPROCID_MAPMOVEOUT,							/* �}�b�v�O�ړ� */
	CHARPROCID_SWOON,								/* �C�� */
	CHARPROCID_INVINCIBLE,							/* ���G */
	CHARPROCID_MAX
};

/* �s����� */
typedef struct _CHARPROCINFO {
	DWORD	dwProcID,								/* �s��ID */
			dwProcSetTime,							/* �����ݒ莞�� */
			dwProcStartTime,						/* �����J�n���� */
			dwPara;									/* �p�����[�^ */
} CHARPROCINFO, *PCHARPROCINFO;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoCharSvr : public CInfoCharBase
{
public:
			CInfoCharSvr();								/* �R���X�g���N�^ */
	virtual ~CInfoCharSvr();							/* �f�X�g���N�^ */

			void SetSpeak		(LPCSTR pszSpeak);				/* �������e���X�V */
	virtual void SetMoveState	(int nMoveState);				/* �ړ���Ԃ�ύX */
	virtual void SetProcState	(int nProcState);				/* �s����Ԃ�ύX */
			void SetMap			(CInfoMapBase *pInfoMap);		/* �}�b�v����ݒ� */
			void SetEfcBalloon	(DWORD dwEfcBalloonID);			/* ���o����ݒ� */
			void SetMotion		(DWORD dwMotionID);				/* ���[�V������ݒ� */

	virtual BOOL IsEnableBattle	(void);							/* �퓬��ԂɑJ�ڂł��邩���� */
	virtual BOOL IsEnableMove	(void);							/* �ړ��ł����Ԃ����� */
	virtual void CopyAll		(CInfoCharSvr *pSrc);			/* �h����̏����S�ăR�s�[ */
	virtual void Copy			(CInfoCharBase *pSrc);			/* �R�s�[ */
	virtual void ProcAtack		(void);							/* ����(�U��������) */
	virtual BOOL ProcHit		(CInfoCharSvr *pInfoChar);		/* ����(�U�����󂯂���) */
	virtual BOOL TimerProc		(DWORD dwTime);					/* ���ԏ��� */
	virtual BOOL TimerProcMOVE	(DWORD dwTime);					/* ���ԏ���(�ړ�) */
	virtual BOOL IsAtackTarget	(void);							/* �U���ΏۂƂȂ邩���� */

			void Proc				(DWORD dwTime);				/* �s������ */
	virtual BOOL ProcFISHING		(DWORD dwPara);				/* �s������(�ނ�) */
	virtual BOOL ProcFISHING_HIT	(DWORD dwPara);				/* �s������(�ނ�(�q�b�g)) */
	virtual BOOL ProcMAPMOVEIN		(DWORD dwPara);				/* �s������(�}�b�v���ړ�) */
	virtual BOOL ProcMAPMOVEOUT		(DWORD dwPara);				/* �s������(�}�b�v�O�ړ�) */
	virtual BOOL ProcSWOON			(DWORD dwPara);				/* �s������(�C��) */
	virtual BOOL ProcINVINCIBLE		(DWORD dwPara);				/* �s������(���G) */

	void DeleteProcInfo		(int nNo);							/* �s�������폜 */
	void DeleteProcInfo		(DWORD dwProcID);					/* �s�������폜 */
	void DeleteAllProcInfo	(void);								/* �s������S�č폜 */
	void AddProcInfo		(DWORD dwProcID, DWORD dwStartTime, DWORD dwPara);	/* �s������ǉ� */

	/* �ړ���ʂɂ�鏈�� */
	virtual void IncPutCount	(void);							/* ����NPC���𑝉� */
	virtual void DecPutCount	(void);							/* ����NPC�������� */


public:
	int		m_nReserveChgEfect,			/* �G�t�F�N�g�ύX�\�� */
			m_nReserveChgMoveState,		/* �ړ���ʕύX�\�� */
			m_nMoveCount;				/* �ړ����� */
	BOOL	m_bChgPos,					/* ���W���ύX���ꂽ */
			m_bChgMap,					/* �}�b�v���ύX���ꂽ */
			m_bChgUpdatePos,			/* �N���C�A���g���Ŏ��L�����̍��W���X�V���� */
			m_bChgSpeak,				/* �������e���ύX���ꂽ */
			m_bChgInfo,					/* �L������񂪕ύX���ꂽ */
			m_bChgMoveState,			/* �ړ���Ԃ��ύX���ꂽ */
			m_bChgProcState,			/* �s����Ԃ��ύX���ꂽ */
			m_bChgScreenPos,			/* ��ʈʒu��ύX���� */
			m_bChgPosRenew,				/* ���W���ύX���ꂽ */
			m_bChgGrp,					/* �摜���ύX���ꂽ */
			m_bChgEfcBalloon,			/* ���o�����ύX���ꂽ */
			m_bChgMotion,				/* ���[�V�������ύX���ꂽ */
			m_bChgStatus,				/* �X�e�[�^�X���ύX���ꂽ */
			m_bChgFishingHit,			/* �ނꂽ */
			m_bChgMoveCount,			/* �w������̈ړ� */
			m_bChgPutNpc,				/* NPC���� */
			m_bWaitCheckMapEvent,		/* �}�b�v�C�x���g�`�F�b�N�҂� */
			m_bDropItem,				/* �A�C�e���h���b�v */
			m_bAtack,					/* �U�� */
			m_bRenewTargetPos,			/* �^�[�Q�b�g���W�X�V */
			m_bProcMoveMapIn,			/* �}�b�v���ړ� */
			m_bProcMoveMapOut,			/* �}�b�v�O�ړ� */
			m_bProcMoveMarkPos,			/* �L�^�ʒu�ֈړ� */
			m_bProcSwoon,				/* �C�� */
			m_bProcInvincible,			/* ���G */
			m_bStateFadeInOut,			/* �t�F�[�h�C���E�A�E�g�� */
			m_bStatusInvincible;		/* ���G�� */
	DWORD	m_dwLastTimeChg,			/* ��ԕύX���� */
			m_dwChgWait,				/* ��ԕύX�҂����� */
			m_dwEfcBalloonID,			/* ���o��ID */
			m_dwMotionID,				/* ���[�V����ID */
			m_dwMoveCount;				/* �ړ����� */

	CInfoMapBase	*m_pInfoMap;		/* �}�b�v��� */
	CmyArray<PCHARPROCINFO, PCHARPROCINFO>	m_apProcInfo;	/* �s����� */
} CInfoCharSvr, *PCInfoCharSvr;

/* Copyright(C)URARA-works 2007 */
