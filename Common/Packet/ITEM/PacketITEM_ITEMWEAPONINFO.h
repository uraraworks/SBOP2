/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_ITEMWEAPONINFO.h								 */
/* ���e			:�R�}���h(�A�C�e���n:������ʒm) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"
#include "LibInfoItemWeapon.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketITEM_ITEMWEAPONINFO : public CPacketBase
{
public:
			CPacketITEM_ITEMWEAPONINFO();			/* �R���X�g���N�^ */
	virtual ~CPacketITEM_ITEMWEAPONINFO();			/* �f�X�g���N�^ */

	void	Make	(PCLibInfoItemWeapon pInfo);			/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	PCLibInfoItemWeapon	m_pLibInfo;				/* ������ */
} CPacketITEM_ITEMWEAPONINFO, *PCPacketITEM_ITEMWEAPONINFO;

/* Copyright(C)URARA-works 2008 */
