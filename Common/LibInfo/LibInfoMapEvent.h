/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMapEvent.h											 */
/* ���e			:�}�b�v�C�x���g��񃉃C�u�����N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/22													 */
/* ========================================================================= */

#pragma once

#include "InfoMapEventBase.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoMapEvent : public CLibInfoBase
{
public:
			CLibInfoMapEvent();							/* �R���X�g���N�^ */
	virtual ~CLibInfoMapEvent();						/* �f�X�g���N�^ */

	void Create			(void);									/* �쐬 */
	void Destroy		(void);									/* �j�� */

	PCInfoBase GetNew	(int nType);							/* �V�K�f�[�^���擾 */

	int		GetCount		(void);								/* �f�[�^�����擾 */
	void	Add				(PCInfoBase pInfo);					/* �ǉ� */
	void	Delete			(int nNo);							/* �폜 */
	void	Delete			(DWORD dwMapEventID);				/* �폜 */
	void	DeleteAll		(void);								/* �S�č폜 */
	void	Merge			(CLibInfoMapEvent *pSrc);			/* ��荞�� */

	CInfoMapEventBase	*Renew	(DWORD dwMapEventID, CInfoMapEventBase *pSrc);	/* �X�V */
	PCInfoBase			GetPtr	(int nNo);										/* �}�b�v�C�x���g�����擾 */
	PCInfoBase			GetPtr	(DWORD dwMapEventID);							/* �}�b�v�C�x���g�����擾 */

	virtual DWORD	GetDataSize		(void);						/* �f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetWriteData	(PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	virtual DWORD	ReadElementData	(PBYTE pSrc);				/* �w��v�f�f�[�^��ǂݍ��� */

	virtual DWORD	GetSendDataSize	(void);						/* ���M�f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetSendData		(void);						/* ���M�f�[�^���擾 */
	virtual PBYTE	SetSendData		(PBYTE pSrc);				/* ���M�f�[�^�����荞�� */


protected:
	DWORD	GetNewID	(void);									/* �V�����A�C�e��ID���擾 */


protected:
	DWORD	m_dwNewIDTmp;						/* �V�KID�쐬�p */
	PARRAYMAPEVENTBASEINFO	m_paInfo;			/* �}�b�v�C�x���g��� */
} CLibInfoMapEvent, *PCLibInfoMapEvent;

/* Copyright(C)URARA-works 2008 */
