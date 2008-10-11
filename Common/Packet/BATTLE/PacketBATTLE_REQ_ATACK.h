/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketBATTLE_REQ_ATACK.h									 */
/* ���e			:�R�}���h(�퓬�n:�U���v��) ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketBATTLE_REQ_ATACK : public CPacketBase
{
public:
			CPacketBATTLE_REQ_ATACK();					/* �R���X�g���N�^ */
	virtual ~CPacketBATTLE_REQ_ATACK();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, DWORD dwTargetCharID);		/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID,						/* �L����ID */
			m_dwTargetCharID;				/* �U����L����ID */
} CPacketBATTLE_REQ_ATACK, *PCPacketBATTLE_REQ_ATACK;

/* Copyright(C)URARA-works 2007 */
