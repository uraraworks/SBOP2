/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoSkillBase.h											 */
/* ���e			:�X�L�������N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/03													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �X�L�����(���C��) */
enum {
	SKILLTYPEMAIN_NONE = 0,			/* �\�� */
	SKILLTYPEMAIN_BATTLE,			/* �퓬 */
	SKILLTYPEMAIN_LIFE,				/* ���� */
	SKILLTYPEMAIN_MAX
};

/* �X�L�����(�T�u:�퓬) */
enum {
	SKILLTYPESUB_BATTLE_NONE = 0,
	SKILLTYPESUB_BATTLE_MOVEATACK,	/* �ړ����čU�� */
	SKILLTYPESUB_BATTLE_MAX
};

/* �X�L�����(�T�u:����) */
enum {
	SKILLTYPESUB_LIFE_NONE = 0,
	SKILLTYPESUB_LIFE_FISHING,		/* �ނ� */
	SKILLTYPESUB_LIFE_MAX
};

/* �g�p���� */
enum {
	SKILLUSE_ANY = 0,			/* �������� */
	SKILLUSE_NORMAL,			/* �ʏ펞 */
	SKILLUSE_BATTLE,			/* �퓬���[�h�� */
	SKILLUSE_MAX
};

/* �N���X��� */
enum {
	INFOSKILLTYPE_BASE = 0,		/* ��� */
	INFOSKILLTYPE_MOVEATACK,	/* �ړ����čU�� */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoSkillBase : public CInfoBase
{
public:
			CInfoSkillBase();									/* �R���X�g���N�^ */
	virtual ~CInfoSkillBase();									/* �f�X�g���N�^ */

	virtual int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	virtual DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	virtual LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	virtual DWORD	GetDerivationSize		(void);							/* �h���f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetDerivationWriteData	(PDWORD pdwSize);				/* �h���f�[�^�̕ۑ��p�f�[�^���擾 */
	virtual DWORD	ReadDerivationData		(PBYTE pSrc);					/* �h���f�[�^��ǂݍ��� */

	virtual DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	virtual void	Copy				(CInfoSkillBase *pSrc);			/* �R�s�[ */


public:
	int			m_nElementCountBase,			/* ���N���X�̗v�f�� */
				m_nClassType;					/* �N���X��� */

	DWORD		m_dwSkillID,					/* �X�L��ID */
				m_dwSP;							/* ����SP */
	int			m_nTypeMain,					/* �X�L�����(���C��) */
				m_nTypeSub,						/* �X�L�����(�T�u) */
				m_nUse;							/* �g�p���� */
	CmyString	m_strName;						/* �X�L���� */
} CInfoSkillBase, *PCInfoSkillBase;
typedef CmyArray<PCInfoSkillBase, PCInfoSkillBase>	  ARRAYSKILLBASEINFO;
typedef CmyArray<PCInfoSkillBase, PCInfoSkillBase>	*PARRAYSKILLBASEINFO;

/* Copyright(C)URARA-works 2008 */
