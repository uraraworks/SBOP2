/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoTalkEventMENU.h										 */
/* ���e			:��b�C�x���g���(���ڑI��)�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/28													 */
/* ========================================================================= */

#pragma once

#include "InfoTalkEventBase.h"

/* ========================================================================= */
/* �\���̒�`																 */
/* ========================================================================= */

/* ���ڏ�� */
typedef struct _STTALKEVENTMENUINFO {
	int			nPage;		/* �W�����v��y�[�W�ԍ� */
	CmyString	strName;	/* ���ږ� */
} STTALKEVENTMENUINFO, *PSTTALKEVENTMENUINFO;
typedef CmyArray<PSTTALKEVENTMENUINFO, PSTTALKEVENTMENUINFO>	  ARRAYTALKEVENTMENUINFO;
typedef CmyArray<PSTTALKEVENTMENUINFO, PSTTALKEVENTMENUINFO>	*PARRAYTALKEVENTMENUINFO;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoTalkEventMENU : public CInfoTalkEventBase
{
public:
			CInfoTalkEventMENU();									/* �R���X�g���N�^ */
	virtual ~CInfoTalkEventMENU();									/* �f�X�g���N�^ */

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

	void DeleteMenuInfo		(int nNo);						/* ���ڏ����폜 */
	void DeleteAllMenuInfo	(void);							/* ���ڏ���S�č폜 */
	void AddMenuInfo		(int nPage, LPCSTR pszName);	/* ���ڏ���ǉ� */
	int  GetMenuInfoCount	(void);							/* ���ڐ����擾 */

	PSTTALKEVENTMENUINFO	GetPtr	(int nNo);				/* ���ڏ����擾 */


public:
	ARRAYTALKEVENTMENUINFO	m_aMenuInfo;		/* ���ڏ�� */
} CInfoTalkEventMENU, *PCInfoTalkEventMENU;

/* Copyright(C)URARA-works 2008 */
