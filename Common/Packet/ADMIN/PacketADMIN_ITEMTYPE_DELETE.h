/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEMTYPE_DELETE.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�C�e����ʏ��폜) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/01													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_ITEMTYPE_DELETE : public CPacketBase
{
public:
			CPacketADMIN_ITEMTYPE_DELETE();					/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_ITEMTYPE_DELETE();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwTypeID);								/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwTypeID;			/* ���ID */
} CPacketADMIN_ITEMTYPE_DELETE, *PCPacketADMIN_ITEMTYPE_DELETE;

/* Copyright(C)URARA-works 2007 */
