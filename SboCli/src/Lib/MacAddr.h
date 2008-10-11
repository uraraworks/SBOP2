/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	MacAddr.h													 */
/* ���e�F		MAC�A�h���X�擾�N���X ��`�t�@�C��							 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/04/12													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CMacAddr
{
public:
			CMacAddr();										/* �R���X�g���N�^ */
	virtual ~CMacAddr();									/* �f�X�g���N�^ */

	BOOL	Get				(PBYTE pDst, int nNo = 0);				/* MAC�A�h���X���擾 */
	BOOL	GetStr			(LPSTR pszDst, int nNo = 0);			/* MAC�A�h���X���擾(������) */
	int		GetCount		(void);									/* NIC�����擾 */
	BOOL	GetDeviceName	(LPSTR pszDst, int nNo = 0);			/* �f�o�C�X�����擾 */


protected:
	int		GetNICCount	(void);								/* NIC�����擾 */


protected:
	int		m_nCount;										/* NIC�� */
} CMacAddr, *PCMacAddr;

/* Copyright(C)URARA-works 2005 */
