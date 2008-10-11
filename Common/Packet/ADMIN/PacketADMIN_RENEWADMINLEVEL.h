/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_RENEWADMINLEVEL.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�Ǘ��Ҍ������x���X�V) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_RENEWADMINLEVEL : public CPacketBase
{
public:
			CPacketADMIN_RENEWADMINLEVEL();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_RENEWADMINLEVEL();			/* �f�X�g���N�^ */

	void	Make	(DWORD dwAccountID, int nLevel);			/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwAccountID;					/* �A�J�E���gID */
	int		m_nAdminLevel;					/* �Ǘ��҃��x�� */
} CPacketADMIN_RENEWADMINLEVEL, *PCPacketADMIN_RENEWADMINLEVEL;

/* Copyright(C)URARA-works 2006 */
