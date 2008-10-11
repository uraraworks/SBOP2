/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEMTYPE_ADD.h									 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�C�e����ʏ��ǉ�) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/01													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItemTypeBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_ITEMTYPE_ADD : public CPacketBase
{
public:
			CPacketADMIN_ITEMTYPE_ADD();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_ITEMTYPE_ADD();				/* �f�X�g���N�^ */

	void	Make	(CInfoItemTypeBase *pInfo);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CInfoItemTypeBase	*m_pInfoItem;			/* �ǉ�����A�C�e����ʏ�� */
} CPacketADMIN_ITEMTYPE_ADD, *PCPacketADMIN_ITEMTYPE_ADD;

/* Copyright(C)URARA-works 2007 */
