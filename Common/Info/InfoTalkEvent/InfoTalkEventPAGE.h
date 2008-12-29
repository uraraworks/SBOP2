/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoTalkEventPAGE.h										 */
/* ���e			:��b�C�x���g���(�y�[�W�؂�ւ�)�N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/29													 */
/* ========================================================================= */

#pragma once

#include "InfoTalkEventBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �y�[�W�؂�ւ����� */
enum {
	CHGPAGECONDITION_NONE = 0,
	CHGPAGECONDITION_ITEM,		/* �A�C�e������ */
	CHGPAGECONDITION_NOITEM,	/* �A�C�e���Ȃ� */
	CHGPAGECONDITION_MAX
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoTalkEventPAGE : public CInfoTalkEventBase
{
public:
			CInfoTalkEventPAGE();									/* �R���X�g���N�^ */
	virtual ~CInfoTalkEventPAGE();									/* �f�X�g���N�^ */

	virtual int		GetElementCount		(void);								/* �v�f�����擾 */
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
	int		m_nPageChgCondition,			/* �y�[�W�؂�ւ����� */
			m_nPageJump;					/* �y�[�W�؂�ւ��� */
} CInfoTalkEventPAGE, *PCInfoTalkEventPAGE;

/* Copyright(C)URARA-works 2008 */
