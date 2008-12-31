/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoTalkEventBase.h										 */
/* ���e			:��b�C�x���g�����N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/16													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* ��b�C�x���g��� */
enum {
	TALKEVENTTYPE_NONE = 0,
	TALKEVENTTYPE_PAGE,			/* �y�[�W�؂�ւ� */
	TALKEVENTTYPE_MSG,			/* ���b�Z�[�W�\�� */
	TALKEVENTTYPE_MENU,			/* ���ڑI�� */
	TALKEVENTTYPE_ADDSKILL,		/* �X�L���ǉ� */
	TALKEVENTTYPE_MAX
};

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoTalkEventBase : public CInfoBase
{
public:
			CInfoTalkEventBase();									/* �R���X�g���N�^ */
	virtual ~CInfoTalkEventBase();									/* �f�X�g���N�^ */

	virtual int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	virtual DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	virtual LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	virtual DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	virtual void	Copy				(CInfoTalkEventBase *pSrc);			/* �R�s�[ */


public:
	int			m_nElementCountBase,			/* ���N���X�̗v�f�� */
				m_nEventType,					/* ��b�C�x���g��� */
				m_nPage;						/* �����y�[�W�ԍ� */
	DWORD		m_dwData;						/* �o�C�i���f�[�^ */
	CmyString	m_strText;						/* ������f�[�^ */
} CInfoTalkEventBase, *PCInfoTalkEventBase;
typedef CmyArray<PCInfoTalkEventBase, PCInfoTalkEventBase>	  ARRAYTALKEVENTBASEINFO;
typedef CmyArray<PCInfoTalkEventBase, PCInfoTalkEventBase>	*PARRAYTALKEVENTBASEINFO;

/* Copyright(C)URARA-works 2008 */
