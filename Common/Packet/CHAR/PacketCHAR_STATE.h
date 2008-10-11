/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_STATE.h											 */
/* ���e			:�R�}���h(�L�����n:��Ԓʒm) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/24													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_STATE : public CPacketBase
{
public:
			CPacketCHAR_STATE();							/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_STATE();							/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, int nState);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID;						/* �L����ID */
	int		m_nState;						/* ��� */
} CPacketCHAR_STATE, *PCPacketCHAR_STATE;

/* Copyright(C)URARA-works 2007 */
