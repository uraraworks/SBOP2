/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketACCOUNT_REQ_ACCOUNTINFO.h							 */
/* ���e			:�R�}���h(�A�J�E���g�n:�A�J�E���g���v��) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/06													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketACCOUNT_REQ_ACCOUNTINFO : public CPacketBase
{
public:
			CPacketACCOUNT_REQ_ACCOUNTINFO();			/* �R���X�g���N�^ */
	virtual ~CPacketACCOUNT_REQ_ACCOUNTINFO();			/* �f�X�g���N�^ */

	void	Make	(DWORD dwAccountID);						/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD m_dwAccountID;						/* �A�J�E���gID */
} CPacketACCOUNT_REQ_ACCOUNTINFO, *PCPacketACCOUNT_REQ_ACCOUNTINFO;

/* Copyright(C)URARA-works 2006 */
