/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	crc.h														 */
/* ���e�F		CRC�v�Z�N���X ��`�t�@�C��									 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/03/29													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */
class CCRC
{
public:
			CCRC();										/* �R���X�g���N�^ */
	virtual ~CCRC();									/* �f�X�g���N�^ */

	DWORD Update	(DWORD dwSrc, PBYTE pBuf, DWORD dwSize);	/* CRC���X�V */
	DWORD GetCRC	(PBYTE pBuf, DWORD dwSize);					/* CRC���擾 */


private:
	void MakeTbl(void);									/* �v�Z�p�e�[�u�����쐬 */


private:
	PDWORD		m_padwCRC;								/* �v�Z�p�e�[�u�� */
};

/* Copyright(C)URARA-works 2005 */
