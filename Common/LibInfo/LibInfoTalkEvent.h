/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoTalkEvent.h											 */
/* ���e			:��b�C�x���g��񃉃C�u�����N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/27													 */
/* ========================================================================= */

#pragma once

#include "InfoTalkEvent.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoTalkEvent : public CLibInfoBase
{
public:
			CLibInfoTalkEvent();						/* �R���X�g���N�^ */
	virtual ~CLibInfoTalkEvent();						/* �f�X�g���N�^ */

	void Create			(void);									/* �쐬 */
	void Destroy		(void);									/* �j�� */

	PCInfoBase GetNew	(void);									/* �V�K�f�[�^���擾 */

	int		GetCount	(void);									/* �f�[�^�����擾 */
	void	Add			(PCInfoBase pInfo);						/* �ǉ� */
	void	Delete		(int nNo);								/* �폜 */
	void	Delete		(DWORD dwTalkEventID);					/* �폜 */
	void	DeleteAll	(void);									/* �S�č폜 */
	void	Merge		(CLibInfoTalkEvent *pSrc);				/* ��荞�� */

	CInfoTalkEvent	*Renew	(CInfoTalkEvent *pSrc);				/* �X�V */
	PCInfoBase		GetPtr	(int nNo);							/* �����擾 */
	PCInfoBase		GetPtr	(DWORD dwTalkEventID);				/* �����擾 */

	DWORD	GetSendDataSize		(void);							/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);							/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);					/* ���M�f�[�^�����荞�� */


protected:
	DWORD	GetNewID	(void);									/* �V����ID���擾 */


protected:
	PARRAYTALKEVENTINFO	m_paInfo;				/* ��b�C�x���g��� */
} CLibInfoTalkEvent, *PCLibInfoTalkEvent;

/* Copyright(C)URARA-works 2008 */
