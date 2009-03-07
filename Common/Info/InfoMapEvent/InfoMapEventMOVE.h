/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapEventMOVE.h											 */
/* ���e			:�C�x���g���(�}�b�v���ړ�)�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/05													 */
/* ========================================================================= */

#pragma once

#include "InfoMapEventBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoMapEventMOVE : public CInfoMapEventBase
{
public:
			CInfoMapEventMOVE();							/* �R���X�g���N�^ */
	virtual ~CInfoMapEventMOVE();							/* �f�X�g���N�^ */

	void	RenewSize			(int nDirection, int nSize, SIZE *pSize);	/* �T�C�Y�X�V */
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
	POINT		m_ptDst;			/* �ړ��� */
	int			m_nDirection;		/* �ړ���̌��� */
} CInfoMapEventMOVE, *PCInfoMapEventMOVE;
typedef CmyArray<PCInfoMapEventMOVE, PCInfoMapEventMOVE>	  ARRAYMAPEVENTMAPINFO;
typedef CmyArray<PCInfoMapEventMOVE, PCInfoMapEventMOVE>	*PARRAYMAPEVENTMAPINFO;

/* Copyright(C)URARA-works 2007 */
