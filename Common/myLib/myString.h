/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:myString.h													 */
/* ���e			:������N���X ��`�t�@�C��									 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/05/25													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CmyString
{
public:
			CmyString();							/* �R���X�g���N�^ */
			CmyString(CmyString &strSrc);
	virtual ~CmyString();							/* �f�X�g���N�^ */

	void	Empty			(void);							/* ���������ɂ��� */
	BOOL	IsEmpty			(void);							/* �����񂪋󂩔��� */
	int		GetLength		(void);							/* �����񒷂��擾 */
	void	Format			(LPCSTR lpFormat, ...);			/* ����������ŏ����� */

	void	operator =		(CmyString &strSrc);			/* ������������� */
	void	operator =		(LPCSTR pszSrc);				/* ������������� */
	void	operator +=		(LPCSTR pszSrc);				/* �������ǉ� */
	BOOL	operator ==		(LPCSTR pszSrc);				/* ��������r */
	BOOL	operator !=		(LPCSTR pszSrc);				/* ��������r */
			operator LPCTSTR	();							/* �L���X�g */

	int		CompareNoCase	(LPCSTR pszSrc);				/* �������r(�啶����������ʖ���) */

protected:
	void	Renew	(LPCSTR pszSrc);						/* ��������X�V */


protected:
	int		m_nLength;							/* ������ */
	LPSTR	m_pszString;						/* ������f�[�^ */
} CmyString, *PCmyString;

/* Copyright(C)URARA-works 2006 */
