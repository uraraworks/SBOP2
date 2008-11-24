/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapEventMAPMOVE.h										 */
/* ���e			:�C�x���g���(�}�b�v�Ԉړ�)�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/27													 */
/* ========================================================================= */

#pragma once

#include "InfoMapEventBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoMapEventMAPMOVE : public CInfoMapEventBase
{
public:
			CInfoMapEventMAPMOVE();							/* �R���X�g���N�^ */
	virtual ~CInfoMapEventMAPMOVE();						/* �f�X�g���N�^ */

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
	DWORD		m_dwMapID;			/* �ړ���}�b�vID */
	POINT		m_ptDst;			/* �ړ��� */
	int			m_nDirection;		/* �ړ���̌��� */
} CInfoMapEventMAPMOVE, *PCInfoMapEventMAPMOVE;

/* Copyright(C)URARA-works 2008 */
