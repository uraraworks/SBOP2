/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:SBOGlobal.h												 */
/* ���e			:�v���W�F�N�g�ˑ��ł͂Ȃ����ʏ��� ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/06													 */
/* ========================================================================= */

#pragma once

class CmyString;

extern void sgenrand(unsigned long);								/* ���������� */
extern long genrand();												/* �����擾 */

extern PBYTE	ZeroNew				(DWORD dwSize);												/* �w��T�C�Y�̃��������m�ۂ���0�N���A */
extern void		CopyMemoryRenew		(PVOID pDst, CONST PVOID pSrc, DWORD dwSize, PBYTE &pPos);	/* �������R�s�[���ă|�C���^��i�߂� */
extern void		strcpyRenew			(LPSTR pszDst, LPCSTR pszSrc, PBYTE &pPos);					/* ������R�s�[���ă|�C���^��i�߂� */
extern void		StoreRenew			(CmyString &strDst, LPCSTR pszSrc, PBYTE &pPos);			/* ������R�s�[���ă|�C���^��i�߂� */
extern void		GetModuleFilePath	(LPSTR pszDst, DWORD dwSize);								/* ���W���[���p�X���擾 */
extern BOOL		AllCreateDirectory	(LPCSTR, LPSECURITY_ATTRIBUTES pSAtt = NULL, BOOL bResult = TRUE);	/* �w�肳�ꂽ�p�X�̃t�H���_���쐬 */
extern void		TrimViewString		(CmyString &strDst, LPCSTR pszSrc);							/* �\���ł��Ȃ��������폜 */
extern BOOL		IsInRect			(RECT *pSrc, RECT *pTarget);								/* �w���`�����S�ɓ����ɂ��邩���� */
extern BOOL		IsHitRect			(RECT *pSrc1, RECT *pSrc2);									/* ��`���m�̓����蔻�� */

/* Copyright(C)URARA-works 2006 */
