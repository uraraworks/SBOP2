/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:TextInput.h												 */
/* ���e			:�e�L�X�g���̓N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/05/16													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CTextInput
{
public:
			CTextInput();								/* �R���X�g���N�^ */
	virtual ~CTextInput();								/* �f�X�g���N�^ */

	BOOL		Create			(LPCSTR pszFileName);			/* ������ */
	void		Destroy			(void);							/* �j�� */
	void		SetLine			(DWORD dwLine);					/* �ǂݍ��ލs��ݒ� */
	CmyString	ReadLine		(DWORD dwLine);					/* �w��s����1�s�ǂݍ��� */
	CmyString	ReadLine		(void);							/* ���݈ʒu����1�s�ǂݍ��� */
	DWORD		GetLineCount	(void);							/* �s�����擾 */


protected:
	BOOL		Open			(LPCSTR pszFileName);			/* �t�@�C�����J�� */
	void		Close			(void);							/* �t�@�C������� */
	CmyString	ReadProc		(void);							/* ���݈ʒu����̓ǂݍ��ݏ��� */
	BOOL		IsCSVCheck		(LPCSTR pszText);				/* CSV�`��������Ƃ��Đ����������� */


protected:
	DWORD		m_dwLine;				/* ���݂̓ǂݍ��݈ʒu */
	LPSTR		m_pszFileName;			/* �t�@�C���� */
	HANDLE		m_hFile;				/* �t�@�C���n���h�� */
};

/* Copyright(C)URARA-works 2006 */
