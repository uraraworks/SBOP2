/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapObjectData.h										 */
/* ���e			:�}�b�v�I�u�W�F�N�g�z�u�f�[�^�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/03													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoMapObjectData : public CInfoBase
{
public:
			CInfoMapObjectData();									/* �R���X�g���N�^ */
	virtual ~CInfoMapObjectData();									/* �f�X�g���N�^ */

	virtual void	RenewSize			(int nDirection, int nSize);		/* �T�C�Y�X�V */
	virtual int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	virtual DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	virtual LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	virtual DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	virtual void	Copy				(CInfoMapObjectData *pSrc);			/* �R�s�[ */


public:
	DWORD		m_dwDataID,						/* �z�u�f�[�^ID */
				m_dwObjectID;					/* �I�u�W�F�N�gID */
	POINT		m_ptPos;						/* ���W */
} CInfoMapObjectData, *PCInfoMapObjectData;
typedef CmyArray<PCInfoMapObjectData, PCInfoMapObjectData>	  ARRAYMAPOBJECTDATAINFO;
typedef CmyArray<PCInfoMapObjectData, PCInfoMapObjectData>	*PARRAYMAPOBJECTDATAINFO;

/* Copyright(C)URARA-works 2008 */
