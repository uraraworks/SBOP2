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

/* �X�L����� */
enum {
	SKILLTYPE_NONE = 0,
	SKILLTYPE_FISHING,			/* �ނ� */
	SKILLTYPE_MAX
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

	virtual DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	virtual void	Copy				(CInfoSkillBase *pSrc);			/* �R�s�[ */


public:
	int			m_nElementCountBase;			/* ���N���X�̗v�f�� */

	DWORD		m_dwSkillID,					/* �X�L��ID */
				m_dwSP;							/* ����SP */
	int			m_nType;						/* �X�L����� */
	CmyString	m_strName;						/* �X�L���� */
} CInfoSkillBase, *PCInfoSkillBase;
typedef CmyArray<PCInfoSkillBase, PCInfoSkillBase>	  ARRAYSKILLBASEINFO;
typedef CmyArray<PCInfoSkillBase, PCInfoSkillBase>	*PARRAYSKILLBASEINFO;

/* Copyright(C)URARA-works 2008 */
