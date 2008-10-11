/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoFileList.h												 */
/* ���e			:�t�@�C�����X�g���N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/03/02													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �\���̒�`																 */
/* ========================================================================= */

/* �t�@�C�����X�g��� */
typedef struct _FILELISTINFO {
	CmyString	strMD5,					/* MD5�n�b�V�� */
				strFileName;			/* �t�@�C���� */
	DWORD		dwFileSize;				/* �t�@�C���T�C�Y */
} FILELISTINFO, *PFILELISTINFO;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoFileList
{
public:
			CInfoFileList();										/* �R���X�g���N�^ */
	virtual ~CInfoFileList();										/* �f�X�g���N�^ */

	void			Add			(LPCSTR pszHash, LPCSTR pszFileName, DWORD dwFileSize);	/* �ǉ� */
	int				GetCount	(void);										/* �f�[�^�����擾 */
	PFILELISTINFO	GetPtr		(int nNo);									/* �t�@�C�����X�g�����擾 */
	void			DeleteAll	(void);										/* �S�č폜 */

	void	Copy				(CInfoFileList *pSrc);						/* �R�s�[ */
	DWORD	GetSendDataSize		(void);										/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);										/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);								/* ���M�f�[�^�����荞�� */


protected:
	PFILELISTINFO	GetNew		(void);					/* �V�K�f�[�^�擾 */


protected:
	CmyArray<PFILELISTINFO, PFILELISTINFO>	m_aFileListInfo;		/* �t�@�C�����X�g��� */
} CInfoFileList, *PCInfoFileList;

/* Copyright(C)URARA-works 2008 */
