/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_REQ_ADMINLEVEL.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�Ǘ��Ҍ������x���v��) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_REQ_ADMINLEVEL : public CPacketBase
{
public:
			CPacketADMIN_REQ_ADMINLEVEL();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_REQ_ADMINLEVEL();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwAccountID);						/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD m_dwAccountID;					/* �A�J�E���gID */
} CPacketADMIN_REQ_ADMINLEVEL, *PCPacketADMIN_REQ_ADMINLEVEL;

/* Copyright(C)URARA-works 2006 */
