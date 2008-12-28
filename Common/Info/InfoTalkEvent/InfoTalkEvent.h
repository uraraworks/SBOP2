/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoTalkEvent.h											 */
/* ���e			:��b�C�x���g���N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/16													 */
/* ========================================================================= */

#pragma once

#include "InfoTalkEventBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoTalkEvent : public CInfoBase
{
public:
			CInfoTalkEvent();										/* �R���X�g���N�^ */
	virtual ~CInfoTalkEvent();										/* �f�X�g���N�^ */

	virtual int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	virtual DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	virtual LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	virtual DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	virtual void	Copy				(CInfoTalkEvent *pSrc);				/* �R�s�[ */

	PCInfoTalkEventBase	GetNew			(int nType);									/* �V������b�C�x���g���擾 */
	PCInfoTalkEventBase	GetPtr			(int nPage, int nNo);							/* ��b�C�x���g���擾 */
	void	SetPtr						(int nPage, int nNo, CInfoTalkEventBase *pInfo);/* ��b�C�x���g�������ւ� */
	void	GetEventArray				(int nPage, ARRAYTALKEVENTBASEINFO &aDst);		/* �w��y�[�W�ԍ��̃C�x���g�ꗗ���擾 */
	int		GetPageCount				(void);								/* ��b�C�x���g�y�[�W�����擾 */
	void	AddTalkEvent				(CInfoTalkEventBase *pInfo);		/* ��b�C�x���g��ǉ� */
	void	DeleteTalkEvent				(int nNo);							/* ��b�C�x���g���폜 */
	void	DeleteTalkEvent				(int nPage, int nNo);				/* ��b�C�x���g���폜 */
	void	DeleteTalkEvent				(CInfoTalkEventBase *pInfo);		/* ��b�C�x���g���폜 */
	void	DeleteAllTalkEvent			(void);								/* ��b�C�x���g��S�č폜 */


public:
	DWORD	m_dwTalkEventID;						/* ��b�C�x���gID */
	ARRAYTALKEVENTBASEINFO		m_apTalkEvent;		/* ��b�C�x���g */
} CInfoTalkEvent, *PCInfoTalkEvent;
typedef CmyArray<PCInfoTalkEvent, PCInfoTalkEvent>	  ARRAYTALKEVENTINFO;
typedef CmyArray<PCInfoTalkEvent, PCInfoTalkEvent>	*PARRAYTALKEVENTINFO;

/* Copyright(C)URARA-works 2008 */
