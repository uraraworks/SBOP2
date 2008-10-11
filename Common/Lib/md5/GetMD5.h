/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:GetMD5.h													 */
/* ���e			:MD5�v�Z�N���X ��`�t�@�C��									 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/04/06													 */
/* ========================================================================= */

#pragma once

extern "C"{
#include "md5.h"
}

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CGetMD5
{
public:
			CGetMD5();								/* �R���X�g���N�^ */
	virtual ~CGetMD5();								/* �f�X�g���N�^ */

	void Init		(void);									/* ������ */
	void Update		(PBYTE pSrc, DWORD dwSize);				/* �X�V */
	void GetBin		(PBYTE pDst);							/* �擾(�o�C�i��) */
	void GetStr		(LPSTR pszDst);							/* �擾(������) */


private:
	MD5_CTX		m_stContext;								/* �o�b�t�@ */
};

/* Copyright(C)URARA-works 2005 */
