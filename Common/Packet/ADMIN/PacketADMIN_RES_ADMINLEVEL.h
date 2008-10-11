/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_RES_ADMINLEVEL.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�Ǘ��Ҍ������x������) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_RES_ADMINLEVEL : public CPacketBase
{
public:
			CPacketADMIN_RES_ADMINLEVEL();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_RES_ADMINLEVEL();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwAccountID, int nLevel);			/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwAccountID;					/* �A�J�E���gID */
	int		m_nAdminLevel;					/* �Ǘ��҃��x�� */
} CPacketADMIN_RES_ADMINLEVEL, *PCPacketADMIN_RES_ADMINLEVEL;

/* Copyright(C)URARA-works 2006 */
