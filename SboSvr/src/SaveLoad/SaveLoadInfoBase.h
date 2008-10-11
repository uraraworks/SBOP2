/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:SaveLoadInfoBase.h											 */
/* ���e			:�f�[�^�ۑ��E�ǂݍ��݊��N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/04/26													 */
/* ========================================================================= */

#pragma once

#include "LibInfoBase.h"

class CInfoBase;

/* ========================================================================= */
/* �\���̒�`																 */
/* ========================================================================= */

/* �w�b�_��� */
typedef struct _HEADERINFO {
	char		szName[64];							/* �J������ */
	DWORD		dwOffset;							/* �I�t�Z�b�g */
	DWORD		dwDataSize;							/* �f�[�^�T�C�Y */
} HEADERINFO, *PHEADERINFO;

typedef CmyArray<PHEADERINFO, PHEADERINFO>	ARRAYHEADERINFO;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CSaveLoadInfoBase
{
public:
			CSaveLoadInfoBase();						/* �R���X�g���N�^ */
	virtual ~CSaveLoadInfoBase();						/* �f�X�g���N�^ */

	virtual void	WriteData			(void);					/* �f�[�^�̏������� */
	virtual BOOL	ReadData			(void);					/* �f�[�^�̓ǂݍ��� */

	virtual void	Save				(PCLibInfoBase pSrc);	/* �t�@�C���ɏ������� */
	virtual void	Load				(PCLibInfoBase pDst);	/* �t�@�C������ǂݍ��� */


protected:
	virtual void	SetHeaderInfo		(PCInfoBase pInfo);		/* �w�b�_����ݒ� */
	void			MakeWriteData		(void);					/* �ۑ��f�[�^�̍쐬 */
	void			ReadHeader			(void);					/* �w�b�_���̓ǂݍ��� */
	void			SetFileName			(LPCSTR pszName);		/* �t�@�C������ݒ� */
	void			AddHeaderInfo		(LPCSTR pszName);		/* �w�b�_����ǉ� */
	PHEADERINFO		GetHeaderInfo		(LPCSTR pszName);		/* �v�f������w�b�_�����擾 */
	void			DelAllHeaderInfo	(void);					/* �w�b�_�����폜 */

	void			CopyMemoryDataDst	(PBYTE &pDst, PBYTE pSrc, DWORD dwSize);			/* �R�s�[���ăR�s�[��|�C���^��i�߂� */
	void			CopyMemoryDataSrc	(PBYTE pDst, PBYTE &pSrc, DWORD dwSize);			/* �R�s�[���ăR�s�[���|�C���^��i�߂� */


protected:
	CmyString			m_strFileName;				/* �t�@�C���� */
	PBYTE				m_pData;					/* �ۑ��E�ǂݍ��݃f�[�^ */
	DWORD				m_dwDataSize;				/* �f�[�^�T�C�Y */
	DWORD				m_dwHeaderSize;				/* �w�b�_�T�C�Y */
	ARRAYHEADERINFO		*m_pHeaderInfo;				/* �w�b�_��� */

	PCLibInfoBase		m_pLibInfoBase;				/* ���N���X */
} CSaveLoadInfoBase, *PCSaveLoadInfoBase;

/* Copyright(C)URARA-works 2005 */
