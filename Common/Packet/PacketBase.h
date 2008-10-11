/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketBase.h												 */
/* ���e			:�R�}���h���N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/08/01													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �\���̐錾																 */
/* ========================================================================= */

typedef struct _PACKETBASE {
	BYTE	byCmdMain,					/* �R�}���hID(���C��) */
			byCmdSub;					/* �R�}���hID(�T�u) */
} PACKETBASE, *PPACKETBASE;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketBase
{
public:
			CPacketBase();								/* �R���X�g���N�^ */
	virtual ~CPacketBase();								/* �f�X�g���N�^ */

	DWORD	GetSize		(void);									/* �p�P�b�g�T�C�Y���擾 */
	PBYTE	Get			(void);									/* �p�P�b�g���擾 */
	PBYTE	Set			(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


protected:
	void	RenewPacket		(PBYTE pPacket, DWORD dwSize);		/* �p�P�b�g���X�V */


public:
	PBYTE	m_pPacket;							/* �p�P�b�g */
	DWORD	m_dwPacketSize;						/* �p�P�b�g�T�C�Y */
	BYTE	m_byCmdMain,						/* �R�}���hID(���C��) */
			m_byCmdSub;							/* �R�}���hID(�T�u) */
} CPacketBase, *PCPacketBase;

/* Copyright(C)URARA-works 2006 */
