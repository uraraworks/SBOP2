/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEM_COPY.h									 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�C�e�����R�s�[) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/22													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_ITEM_COPY : public CPacketBase
{
public:
			CPacketADMIN_ITEM_COPY();						/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_ITEM_COPY();						/* �f�X�g���N�^ */

	void	Make	(DWORD dwItemID);								/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwItemID;			/* �A�C�e��ID */
} CPacketADMIN_ITEM_COPY, *PCPacketADMIN_ITEM_COPY;

/* Copyright(C)URARA-works 2007 */
