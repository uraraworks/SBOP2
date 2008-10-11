/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:CryptUtil.h												 */
/* ���e			:�Í������N���X ��`�t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/02													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */
class CCryptUtil
{
public:
			CCryptUtil();											/* �R���X�g���N�^ */
	virtual ~CCryptUtil();											/* �f�X�g���N�^ */

	void	Crypt		(PBYTE pSrc, PBYTE pDst, DWORD dwSize, int nKeyNo);	/* �Í��� */
	void	UnCrypt		(PBYTE pSrc, PBYTE pDst, DWORD dwSize, int nKeyNo);	/* ������ */
	void	CryptStr	(LPCSTR pszSrc, LPSTR pszDst, int nKeyNo);			/* �Í���(������) */
	void	UnCryptStr	(LPCSTR pszSrc, LPSTR pszDst, int nKeyNo);			/* ������(������) */


protected:
	void	Init	(void);				/* ������ */


protected:
	PBYTE	m_pCryptData;				/* �Í��p�f�[�^ */
};

/* Copyright(C)URARA-works 2007 */
