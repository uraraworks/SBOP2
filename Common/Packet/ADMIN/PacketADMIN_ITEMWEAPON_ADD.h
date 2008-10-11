/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEMWEAPON_ADD.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:������ǉ�) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItemWeapon;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_ITEMWEAPON_ADD : public CPacketBase
{
public:
			CPacketADMIN_ITEMWEAPON_ADD();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_ITEMWEAPON_ADD();				/* �f�X�g���N�^ */

	void	Make	(CInfoItemWeapon *pInfo);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CInfoItemWeapon		*m_pInfo;			/* �ǉ����镐���� */
} CPacketADMIN_ITEMWEAPON_ADD, *PCPacketADMIN_ITEMWEAPON_ADD;

/* Copyright(C)URARA-works 2008 */
