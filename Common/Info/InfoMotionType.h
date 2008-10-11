/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoMotionType.h											 */
/* ���e			:���[�V������ʏ��N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/26													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoMotionType : public CInfoBase
{
public:
			CInfoMotionType();								/* �R���X�g���N�^ */
	virtual ~CInfoMotionType();								/* �f�X�g���N�^ */

	int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	void	Copy				(CInfoMotionType *pSrc);			/* �R�s�[ */


public:
	/* �ۑ����Ȃ��f�[�^ */

	/* �ۑ�����f�[�^ */
	DWORD		m_dwMotionTypeID;				/* ���[�V�������ID */
	WORD		m_wGrpIDSub;					/* �v���r���[�p�O���t�B�b�NID�T�u */
	CmyString	m_strName;						/* ���[�V������ʖ� */
} CInfoMotionType, *PCInfoMotionType;
typedef CmyArray<PCInfoMotionType, PCInfoMotionType>	   ARRAYMOTIONTYPEINFO;
typedef CmyArray<PCInfoMotionType, PCInfoMotionType>	 *PARRAYMOTIONTYPEINFO;

/* Copyright(C)URARA-works 2008 */
