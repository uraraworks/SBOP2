/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEM_DELETE.h									 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�C�e�����폜) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/19													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_ITEM_DELETE : public CPacketBase
{
public:
			CPacketADMIN_ITEM_DELETE();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_ITEM_DELETE();			/* �f�X�g���N�^ */

	void	Make	(DWORD dwItemID);						/* �p�P�b�g���쐬 */
	void	Make	(PARRAYDWORD padwItemID);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	ARRAYDWORD	m_adwItemID;	/* �A�C�e��ID */
} CPacketADMIN_ITEM_DELETE, *PCPacketADMIN_ITEM_DELETE;

/* Copyright(C)URARA-works 2007 */
