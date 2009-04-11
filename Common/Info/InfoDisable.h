/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:InfoDisable.h												 */
/* ���e			:���ۏ��N���X ��`�t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/04/08													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoDisable : public CInfoBase
{
public:
			CInfoDisable();									/* �R���X�g���N�^ */
	virtual ~CInfoDisable();								/* �f�X�g���N�^ */

	int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	void	Copy				(CInfoDisable *pSrc);					/* �R�s�[ */


public:
	/* �ۑ�����f�[�^ */
	DWORD		m_dwDisableID;								/* ���ۏ��ID */
	CmyString	m_strMacAddress;							/* MAC�A�h���X */
} CInfoDisable, *PCInfoDisable;
typedef CmyArray<PCInfoDisable, PCInfoDisable>	   ARRAYDISABLEINFO;
typedef CmyArray<PCInfoDisable, PCInfoDisable>	 *PARRAYDISABLEINFO;

/* Copyright(C)URARA-works 2009 */
