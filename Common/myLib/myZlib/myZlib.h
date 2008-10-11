/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	myZlib.h													 */
/* ���e�F		zlib�������ׂ̃N���X ��`�t�@�C��							 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/02/02													 */
/* ========================================================================= */

#ifndef MYZLIB_H
#define MYZLIB_H

#include "zlib.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CmyZlib
{
public:
			CmyZlib();										/* �R���X�g���N�^ */
	virtual ~CmyZlib();										/* �f�X�g���N�^ */

	/* ���k */
	BOOL	DeflateInit	(int nLevel = Z_DEFAULT_COMPRESSION);							/* ������ */
	DWORD	Deflate		(LPBYTE pSrc, DWORD dwSrcSize, LPBYTE pDst, DWORD dwDstSize);	/* ���k���� */
	void	DeflateEnd	(void);															/* �㏈�� */

	/* �� */
	BOOL	InflateInit	(void);															/* ������ */
	DWORD	Inflate		(LPBYTE pSrc, DWORD dwSrcSize, LPBYTE pDst, DWORD dwDstSize);	/* �𓀏��� */
	void	InflateEnd	(void);															/* �㏈�� */


private:
	z_stream		*m_pStream;									/* ���k�E�𓀗p�\���� */
};

#endif
/* Copyright(C)URARA-works 2005 */
