/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_SET_EFCBALLOON.h								 */
/* ���e			:�R�}���h(�L�����n:���o�����w��) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_SET_EFCBALLOON : public CPacketBase
{
public:
			CPacketCHAR_SET_EFCBALLOON();					/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_SET_EFCBALLOON();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, DWORD dwBalloonID);			/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID,						/* �L����ID */
			m_dwBalloonID;					/* ���o��ID */
} CPacketCHAR_SET_EFCBALLOON, *PCPacketCHAR_SET_EFCBALLOON;

/* Copyright(C)URARA-works 2007 */
