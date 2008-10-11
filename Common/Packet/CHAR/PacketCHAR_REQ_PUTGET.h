/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_PUTGET.h									 */
/* ���e			:�R�}���h(�L�����n:�A�C�e�����E���u���v��) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_PUTGET : public CPacketBase
{
public:
			CPacketCHAR_REQ_PUTGET();					/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_REQ_PUTGET();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, DWORD dwItemID);			/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID,					/* �L����ID */
			m_dwItemID;					/* �A�C�e��ID */
} CPacketCHAR_REQ_PUTGET, *PCPacketCHAR_REQ_PUTGET;

/* Copyright(C)URARA-works 2007 */
