/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEM_ADD.h										 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�C�e�����ǉ�) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/16													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItem;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_ITEM_ADD : public CPacketBase
{
public:
			CPacketADMIN_ITEM_ADD();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_ITEM_ADD();				/* �f�X�g���N�^ */

	void	Make	(CInfoItem *pInfo);						/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	CInfoItem	*m_pInfoItem;				/* �ǉ�����A�C�e����� */
} CPacketADMIN_ITEM_ADD, *PCPacketADMIN_ITEM_ADD;

/* Copyright(C)URARA-works 2007 */
