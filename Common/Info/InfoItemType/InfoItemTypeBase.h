/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoItemTypeBase.h											 */
/* ���e			:�A�C�e����ʊ����N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/26													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �A�C�e����� */
enum {
	ITEMTYPEID_NONE = 0,				/* ���ʖ��� */
	ITEMTYPEID_CLOTH,					/* �� */
	ITEMTYPEID_ACCE,					/* �A�N�Z�T�� */
	ITEMTYPEID_ARMS,					/* ������ */
	ITEMTYPEID_SHIELD,					/* �� */
	ITEMTYPEID_HP,						/* HP���� */
	ITEMTYPEID_LIGHT,					/* ���� */
	ITEMTYPEID_MAX
};

/* �摜ID���C��(��) */
enum {
	ITEMGRPIDMAIN_CLOTH_NONE = 0,		/* ���ݒ� */
	ITEMGRPIDMAIN_CLOTH_SP,				/* ���ꕞ */
	ITEMGRPIDMAIN_CLOTH_MAX
};

/* �摜ID���C��(������) */
enum {
	ITEMGRPIDMAIN_ARMS_NONE = 0,		/* ���ݒ� */
	ITEMGRPIDMAIN_ARMS_BOW,				/* �| */
	ITEMGRPIDMAIN_WEAPON_GLOVE,			/* ����(�Ō�) */
	ITEMGRPIDMAIN_WEAPON_ETC,			/* ����(���̑�) */
	ITEMGRPIDMAIN_2X2_ARMS,				/* ������(2x2) */
	ITEMGRPIDMAIN_2X2_SHIELD,			/* ��(2x2) */
	ITEMGRPIDMAIN_2X2_ARMSSP,			/* ���ꎝ����(2x2) */
	ITEMGRPIDMAIN_2X2_BOW,				/* �|(2x2) */
	ITEMGRPIDMAIN_ARMS_MAX
};

/* ���[�V������� */
#define INFOITEMARMS_MOTION_NONE		0x00000000
#define INFOITEMARMS_MOTION_SWING		0x00000001		/* �U�� */
#define INFOITEMARMS_MOTION_POKE		0x00000002		/* �˂� */
#define INFOITEMARMS_MOTION_BOW			0x00000004		/* �| */
#define INFOITEMARMS_MOTION_BLOW		0x00000008		/* �Ō� */
#define INFOITEMARMS_MOTION_FISHING		0x00000010		/* �ނ� */


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoItemTypeBase : public CInfoBase
{
public:
			CInfoItemTypeBase();									/* �R���X�g���N�^ */
	virtual ~CInfoItemTypeBase();									/* �f�X�g���N�^ */

	virtual int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	virtual DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	virtual LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	virtual DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	virtual void	Copy				(CInfoItemTypeBase *pSrc);			/* �R�s�[ */
	virtual void	SetGrpID			(WORD *pwDst1, WORD *pwDst2);		/* �摜ID��ݒ� */


public:
	/* �ۑ����Ȃ��f�[�^ */

	/* �ۑ�����f�[�^ */
	BYTE		m_byDelAverage,					/* �g���ĂȂ��Ȃ�m�� */
				m_byTarget,						/* �g�p�Ώ� */
				m_byArea;						/* �g�p�͈� */
	BOOL		m_bPutOn;						/* ��ɒu���邩���� */
	WORD		m_wGrpIDMain,					/* �摜ID���C�� */
				m_wGrpIDSub;					/* �摜ID�T�u */
	DWORD		m_dwTypeID,						/* ���ID */
				m_dwItemTypeID,					/* �A�C�e�����ID */
				m_dwGrpID,						/* �n�ʂɂ��鎞�̉摜ID */
				m_dwIconGrpID,					/* �o�b�N�p�b�N���̉摜ID */
				m_dwDropSoundID,				/* �������Ƃ��̌��ʉ�ID */
				m_dwUseEffectID,				/* �g�������ɍĐ�����G�t�F�N�gID */
				m_dwUseSoundID;					/* �g�������ɍĐ�������ʉ�ID */
	CmyString	m_strName;						/* �A�C�e���� */

	/* ������ */
	DWORD		m_dwWeaponInfoID,				/* ������ID */
				m_dwValue,						/* �U���� */
				m_dwMoveWait,					/* ���x */
				m_dwMoveCount;					/* �򋗗� */
	/* HP���� */
	DWORD		m_dwValue2;						/* ����(�ő�)�ŏ���m_dwValue */
	/* ���� */
//	DWORD		m_dwValue,						/* ���背�x�� */
//				m_dwValue2;						/* �������� */
} CInfoItemTypeBase, *PCInfoItemTypeBase;
typedef CmyArray<PCInfoItemTypeBase, PCInfoItemTypeBase>	   ARRAYITEMTYPEINFO;
typedef CmyArray<PCInfoItemTypeBase, PCInfoItemTypeBase>	 *PARRAYITEMTYPEINFO;

/* Copyright(C)URARA-works 2007 */
