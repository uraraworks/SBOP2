/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapEventLIGHT.h										 */
/* ���e			:�C�x���g���(����)�N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/07													 */
/* ========================================================================= */

#pragma once

#include "InfoMapEventBase.h"


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoMapEventLIGHT : public CInfoMapEventBase
{
public:
			CInfoMapEventLIGHT();							/* �R���X�g���N�^ */
	virtual ~CInfoMapEventLIGHT();							/* �f�X�g���N�^ */

	int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	void	Copy				(CInfoMapEventBase *pSrc);			/* �R�s�[ */


public:
	BOOL		m_bLightOn;		/* ��������� */
	DWORD		m_dwTime;		/* �������� */
} CInfoMapEventLIGHT, *PCInfoMapEventLIGHT;

/* Copyright(C)URARA-works 2008 */
