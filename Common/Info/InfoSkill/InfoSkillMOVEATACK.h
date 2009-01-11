/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:InfoSkillMOVEATACK.h										 */
/* ���e			:�X�L�����(�ړ����čU��)�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/01/08													 */
/* ========================================================================= */

#pragma once

#include "InfoSkillBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �U���Ώ� */
enum {
	SKILLMOVEATACKTARGETTYPE_NONE = 0,		/* ���ݒ� */
	SKILLMOVEATACKTARGETTYPE_PC,			/* �v���C���[ */
	SKILLMOVEATACKTARGETTYPE_NPC,			/* NPC */
	SKILLMOVEATACKTARGETTYPE_ALL,			/* �S�� */
	SKILLMOVEATACKTARGETTYPE_MAX
};

/* ���ˎ�� */
enum {
	SKILLMOVEATACKPUTTYPE_NONE = 0,			/* ���ݒ� */
	SKILLMOVEATACKPUTTYPE_FRONT,			/* �O�� */
	SKILLMOVEATACKPUTTYPE_CROSS,			/* �㉺���E */
	SKILLMOVEATACKPUTTYPE_MAX
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoSkillMOVEATACK : public CInfoSkillBase
{
public:
			CInfoSkillMOVEATACK();									/* �R���X�g���N�^ */
	virtual ~CInfoSkillMOVEATACK();									/* �f�X�g���N�^ */

	virtual int		GetElementNoTmp		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	virtual DWORD	GetDataSizeTmp		(void);								/* �f�[�^�T�C�Y���擾 */
	virtual DWORD	GetDataSizeNoTmp	(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	virtual LPCSTR	GetNameTmp			(int nNo);							/* �v�f�����擾 */
	virtual PBYTE	GetWriteDataTmp		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	virtual DWORD	ReadElementDataTmp	(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	virtual DWORD	GetDerivationSize		(void);							/* �h���f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetDerivationWriteData	(PDWORD pdwSize);				/* �h���f�[�^�̕ۑ��p�f�[�^���擾 */
	virtual DWORD	ReadDerivationData		(PBYTE pSrc);					/* �h���f�[�^��ǂݍ��� */

	virtual DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	virtual void	Copy				(CInfoSkillBase *pSrc);				/* �R�s�[ */


public:
	int		m_nElementCountDerivation;	/* �v�f�� */
	DWORD	m_dwTartgetType,		/* �U���Ώ� */
			m_adwEffectID[4],		/* �����ɂ��\���G�t�F�N�g */
			m_dwPutType,			/* ���ˎ�� */
			m_dwAliveTime,			/* �ϋv���� */
			m_dwWaitTime,			/* �ړ����x */
			m_dwValue,				/* ���� */
			m_dwDistance;			/* �˒����� */
	BOOL	m_bHitQuit;				/* �q�b�g����Ə��� */
} CInfoSkillMOVEATACK, *PCInfoSkillMOVEATACK;
typedef CmyArray<PCInfoSkillMOVEATACK, PCInfoSkillMOVEATACK>	  ARRAYSKILLMOVEATACKINFO;
typedef CmyArray<PCInfoSkillMOVEATACK, PCInfoSkillMOVEATACK>	*PARRAYSKILLMOVEATACKINFO;

/* Copyright(C)URARA-works 2009 */
