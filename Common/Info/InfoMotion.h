/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoMotion.h												 */
/* ���e			:���[�V�������N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/29													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* ���[�V�������X�gID */
enum {
	CHARMOTIONLISTID_NONE = 0,
	CHARMOTIONLISTID_STAND_UP,					/* ����(��) */
	CHARMOTIONLISTID_STAND_DOWN,				/* ����(��) */
	CHARMOTIONLISTID_STAND_LEFT,				/* ����(��) */
	CHARMOTIONLISTID_STAND_RIGHT,				/* ����(�E) */
	CHARMOTIONLISTID_WALK_UP,					/* ����(��) */
	CHARMOTIONLISTID_WALK_DOWN,					/* ����(��) */
	CHARMOTIONLISTID_WALK_LEFT,					/* ����(��) */
	CHARMOTIONLISTID_WALK_RIGHT,				/* ����(�E) */
	CHARMOTIONLISTID_SIT_UP,					/* ����(��) */
	CHARMOTIONLISTID_SIT_DOWN,					/* ����(��) */
	CHARMOTIONLISTID_SIT_LEFT,					/* ����(��) */
	CHARMOTIONLISTID_SIT_RIGHT,					/* ����(�E) */
	CHARMOTIONLISTID_BATTLESTAND_UP,			/* �퓬����(��) */
	CHARMOTIONLISTID_BATTLESTAND_DOWN,			/* �퓬����(��) */
	CHARMOTIONLISTID_BATTLESTAND_LEFT,			/* �퓬����(��) */
	CHARMOTIONLISTID_BATTLESTAND_RIGHT,			/* �퓬����(�E) */
	CHARMOTIONLISTID_BATTLEWALK_UP,				/* ���葫(��) */
	CHARMOTIONLISTID_BATTLEWALK_DOWN,			/* ���葫(��) */
	CHARMOTIONLISTID_BATTLEWALK_LEFT,			/* ���葫(��) */
	CHARMOTIONLISTID_BATTLEWALK_RIGHT,			/* ���葫(�E) */
	CHARMOTIONLISTID_SWING_UP,					/* �U��(��) */
	CHARMOTIONLISTID_SWING_DOWN,				/* �U��(��) */
	CHARMOTIONLISTID_SWING_LEFT,				/* �U��(��) */
	CHARMOTIONLISTID_SWING_RIGHT,				/* �U��(�E) */
	CHARMOTIONLISTID_POKE_UP,					/* �˂�(��) */
	CHARMOTIONLISTID_POKE_DOWN,					/* �˂�(��) */
	CHARMOTIONLISTID_POKE_LEFT,					/* �˂�(��) */
	CHARMOTIONLISTID_POKE_RIGHT,				/* �˂�(�E) */
	CHARMOTIONLISTID_BOWWALK_UP,				/* �|�p���葫(��) */
	CHARMOTIONLISTID_BOWWALK_DOWN,				/* �|�p���葫(��) */
	CHARMOTIONLISTID_BOWWALK_LEFT,				/* �|�p���葫(��) */
	CHARMOTIONLISTID_BOWWALK_RIGHT,				/* �|�p���葫(�E) */
	CHARMOTIONLISTID_BOW_UP,					/* �|(��) */
	CHARMOTIONLISTID_BOW_DOWN,					/* �|(��) */
	CHARMOTIONLISTID_BOW_LEFT,					/* �|(��) */
	CHARMOTIONLISTID_BOW_RIGHT,					/* �|(�E) */
	CHARMOTIONLISTID_GLOVEWALK_UP,				/* �Ō��p���葫(��) */
	CHARMOTIONLISTID_GLOVEWALK_DOWN,			/* �Ō��p���葫(��) */
	CHARMOTIONLISTID_GLOVEWALK_LEFT,			/* �Ō��p���葫(��) */
	CHARMOTIONLISTID_GLOVEWALK_RIGHT,			/* �Ō��p���葫(�E) */
	CHARMOTIONLISTID_GLOVE_UP,					/* �Ō�(��) */
	CHARMOTIONLISTID_GLOVE_DOWN,				/* �Ō�(��) */
	CHARMOTIONLISTID_GLOVE_LEFT,				/* �Ō�(��) */
	CHARMOTIONLISTID_GLOVE_RIGHT,				/* �Ō�(�E) */
	CHARMOTIONLISTID_FISHINGWALK_UP,			/* �ނ�p���葫(��) */
	CHARMOTIONLISTID_FISHINGWALK_DOWN,			/* �ނ�p���葫(��) */
	CHARMOTIONLISTID_FISHINGWALK_LEFT,			/* �ނ�p���葫(��) */
	CHARMOTIONLISTID_FISHINGWALK_RIGHT,			/* �ނ�p���葫(�E) */
	CHARMOTIONLISTID_FISHING_UP,				/* �ނ�(��) */
	CHARMOTIONLISTID_FISHING_DOWN,				/* �ނ�(��) */
	CHARMOTIONLISTID_FISHING_LEFT,				/* �ނ�(��) */
	CHARMOTIONLISTID_FISHING_RIGHT,				/* �ނ�(�E) */
	CHARMOTIONLISTID_FISHING_HIT_UP,			/* �ނ�q�b�g(��) */
	CHARMOTIONLISTID_FISHING_HIT_DOWN,			/* �ނ�q�b�g(��) */
	CHARMOTIONLISTID_FISHING_HIT_LEFT,			/* �ނ�q�b�g(��) */
	CHARMOTIONLISTID_FISHING_HIT_RIGHT,			/* �ނ�q�b�g(�E) */
	CHARMOTIONLISTID_BOWBATTLESTAND_UP,			/* �|�p�퓬����(��) */
	CHARMOTIONLISTID_BOWBATTLESTAND_DOWN,		/* �|�p�퓬����(��) */
	CHARMOTIONLISTID_BOWBATTLESTAND_LEFT,		/* �|�p�퓬����(��) */
	CHARMOTIONLISTID_BOWBATTLESTAND_RIGHT,		/* �|�p�퓬����(�E) */
	CHARMOTIONLISTID_GLOVEBATTLESTAND_UP,		/* �Ō��p�퓬����(��) */
	CHARMOTIONLISTID_GLOVEBATTLESTAND_DOWN,		/* �Ō��p�퓬����(��) */
	CHARMOTIONLISTID_GLOVEBATTLESTAND_LEFT,		/* �Ō��p�퓬����(��) */
	CHARMOTIONLISTID_GLOVEBATTLESTAND_RIGHT,	/* �Ō��p�퓬����(�E) */
	CHARMOTIONLISTID_FISHINGBATTLESTAND_UP,		/* �ނ�p�퓬����(��) */
	CHARMOTIONLISTID_FISHINGBATTLESTAND_DOWN,	/* �ނ�p�퓬����(��) */
	CHARMOTIONLISTID_FISHINGBATTLESTAND_LEFT,	/* �ނ�p�퓬����(��) */
	CHARMOTIONLISTID_FISHINGBATTLESTAND_RIGHT,	/* �ނ�p�퓬����(�E) */
	CHARMOTIONLISTID_STAND,						/* ���� */
	CHARMOTIONLISTID_STAND_SIMPLE,				/* ����(�㉺���E1�R�}����) */
	CHARMOTIONLISTID_WALK,						/* ����(��������) */
	CHARMOTIONLISTID_DESTROY,					/* �j�� */
	CHARMOTIONLISTID_SIMPLE,					/* 16�R�}�A�� */
	CHARMOTIONLISTID_DEFENSE_UP,				/* �h��(��) */
	CHARMOTIONLISTID_DEFENSE_DOWN,				/* �h��(��) */
	CHARMOTIONLISTID_DEFENSE_LEFT,				/* �h��(��) */
	CHARMOTIONLISTID_DEFENSE_RIGHT,				/* �h��(�E) */
	CHARMOTIONLISTID_DEFENSE_SUCCESS_UP,		/* �h�䐬��(��) */
	CHARMOTIONLISTID_DEFENSE_SUCCESS_DOWN,		/* �h�䐬��(��) */
	CHARMOTIONLISTID_DEFENSE_SUCCESS_LEFT,		/* �h�䐬��(��) */
	CHARMOTIONLISTID_DEFENSE_SUCCESS_RIGHT,		/* �h�䐬��(�E) */
	CHARMOTIONLISTID_BOWDEFENSE_UP,				/* �|�p�h��(��) */
	CHARMOTIONLISTID_BOWDEFENSE_DOWN,			/* �|�p�h��(��) */
	CHARMOTIONLISTID_BOWDEFENSE_LEFT,			/* �|�p�h��(��) */
	CHARMOTIONLISTID_BOWDEFENSE_RIGHT,			/* �|�p�h��(�E) */
	CHARMOTIONLISTID_DAMAGE_UP,					/* �_���[�W(��) */
	CHARMOTIONLISTID_DAMAGE_DOWN,				/* �_���[�W(��) */
	CHARMOTIONLISTID_DAMAGE_LEFT,				/* �_���[�W(��) */
	CHARMOTIONLISTID_DAMAGE_RIGHT,				/* �_���[�W(�E) */
	CHARMOTIONLISTID_DEAD_UP,					/* ���S(��) */
	CHARMOTIONLISTID_DEAD_DOWN,					/* ���S(��) */
	CHARMOTIONLISTID_DEAD_LEFT,					/* ���S(��) */
	CHARMOTIONLISTID_DEAD_RIGHT,				/* ���S(�E) */
	CHARMOTIONLISTID_MAX
};

/* �s��ID */
enum {
	CHARMOTIONPROCID_NONE = 0,					/* �������Ȃ� */
	CHARMOTIONPROCID_ATACK,						/* �U�� */
	CHARMOTIONPROCID_FISHING,					/* �ނ� */
	CHARMOTIONPROCID_MAX
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoMotion : public CInfoBase
{
public:
			CInfoMotion();									/* �R���X�g���N�^ */
	virtual ~CInfoMotion();									/* �f�X�g���N�^ */

	int		GetElementCount		(void);								/* �v�f�����擾 */
	int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	void	Copy				(CInfoMotion *pSrc);				/* �R�s�[ */


public:
	int			m_nElementCount;				/* �v�f�� */

	/* �ۑ����Ȃ��f�[�^ */

	/* �ۑ�����f�[�^ */
	BYTE		m_byWait,						/* �҂�����(�~�P�O�~���b) */
				m_byLevel1,						/* �����x1 */
				m_byLevel2,						/* �����x2 */
				m_byLevel3;						/* �����x3 */
	BOOL		m_bPile,						/* �d�ˉ摜���ɕ`�� */
				m_bRedrawHand,					/* �����O�ɕ`�� */
				m_bLoop;						/* ���[�v�Đ� */
	WORD		m_wGrpIDMainBase,				/* �O���t�B�b�NID���C��(���n) */
				m_wGrpIDMainPile1,				/* �O���t�B�b�NID���C��(�d�ˍ��킹1) */
				m_wGrpIDMainPile2,				/* �O���t�B�b�NID���C��(�d�ˍ��킹2) */
				m_wGrpIDMainPile3,				/* �O���t�B�b�NID���C��(�d�ˍ��킹3) */
				m_wGrpIDSubBase,				/* �O���t�B�b�NID�T�u(���n) */
				m_wGrpIDSubPile1,				/* �O���t�B�b�NID�T�u(�d�ˍ��킹1) */
				m_wGrpIDSubPile2,				/* �O���t�B�b�NID�T�u(�d�ˍ��킹2) */
				m_wGrpIDSubPile3;				/* �O���t�B�b�NID�T�u(�d�ˍ��킹3) */
	DWORD		m_dwMotionID,					/* ���[�V����ID */
				m_dwMotionTypeID,				/* ���[�V�������ID */
				m_dwMotionListID,				/* ���[�V�������X�gID */
				m_dwSoundID,					/* ���ʉ�ID */
				m_dwProcID;						/* �s��ID */
	POINT		m_ptDrawPosPile0,				/* �`��ʒu(���n) */
				m_ptDrawPosPile1,				/* �`��ʒu(�d�ˍ��킹1) */
				m_ptDrawPosPile2,				/* �`��ʒu(�d�ˍ��킹2) */
				m_ptDrawPosPile3;				/* �`��ʒu(�d�ˍ��킹3) */
	ARRAYINT	m_anDrawList;					/* �`�揇���X�g */
} CInfoMotion, *PCInfoMotion;
typedef CmyArray<PCInfoMotion, PCInfoMotion>	   ARRAYMOTIONINFO;
typedef CmyArray<PCInfoMotion, PCInfoMotion>	 *PARRAYMOTIONINFO;

/* Copyright(C)URARA-works 2007 */
