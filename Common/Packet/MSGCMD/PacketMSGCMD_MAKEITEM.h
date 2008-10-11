/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMSGCMD_MAKEITEM.h									 */
/* ���e			:�R�}���h(���b�Z�[�W�R�}���h�n:�A�C�e���쐬) ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/20													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMSGCMD_MAKEITEM : public CPacketBase
{
public:
			CPacketMSGCMD_MAKEITEM();					/* �R���X�g���N�^ */
	virtual ~CPacketMSGCMD_MAKEITEM();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, DWORD dwItemTypeID);		/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID,				/* �L����ID */
			m_dwItemTypeID;			/* �A�C�e�����ID */
} CPacketMSGCMD_MAKEITEM, *PCPacketMSGCMD_MAKEITEM;

/* Copyright(C)URARA-works 2007 */
