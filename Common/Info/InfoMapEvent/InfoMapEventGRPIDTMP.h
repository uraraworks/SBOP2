/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapEventGRPIDTMP.h										 */
/* ���e			:�C�x���g���(�ꎞ�摜�ݒ�)�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/12													 */
/* ========================================================================= */

#pragma once

#include "InfoMapEventBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

enum {
	GRPIDTMPTYPE_OFF = 0,			/* ���� */
	GRPIDTMPTYPE_ON,				/* �ݒ� */
	GRPIDTMPTYPE_MAX
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoMapEventGRPIDTMP : public CInfoMapEventBase
{
public:
			CInfoMapEventGRPIDTMP();						/* �R���X�g���N�^ */
	virtual ~CInfoMapEventGRPIDTMP();						/* �f�X�g���N�^ */

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
	int			m_nSetType;		/* �ݒ��� */
	DWORD		m_dwIDMain,		/* �摜ID���C�� */
				m_dwIDSub;		/* �摜ID�T�u */
} CInfoMapEventGRPIDTMP, *PCInfoMapEventGRPIDTMP;

/* Copyright(C)URARA-works 2008 */
