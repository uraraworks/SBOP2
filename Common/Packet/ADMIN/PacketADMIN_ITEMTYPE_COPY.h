/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEMTYPE_COPY.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�C�e����ʏ��R�s�[) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/01													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_ITEMTYPE_COPY : public CPacketBase
{
public:
			CPacketADMIN_ITEMTYPE_COPY();					/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_ITEMTYPE_COPY();					/* �f�X�g���N�^ */

	void	Make	(DWORD m_dwTypeID);								/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwTypeID;			/* ���ID */
} CPacketADMIN_ITEMTYPE_COPY, *PCPacketADMIN_ITEMTYPE_COPY;

/* Copyright(C)URARA-works 2007 */
