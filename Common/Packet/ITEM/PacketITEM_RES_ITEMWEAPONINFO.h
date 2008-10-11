/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_RES_ITEMWEAPONINFO.h							 */
/* ���e			:�R�}���h(�A�C�e���n:�����񉞓�) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItemWeapon;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketITEM_RES_ITEMWEAPONINFO : public CPacketBase
{
public:
			CPacketITEM_RES_ITEMWEAPONINFO();			/* �R���X�g���N�^ */
	virtual ~CPacketITEM_RES_ITEMWEAPONINFO();			/* �f�X�g���N�^ */

	void	Make	(CInfoItemWeapon *pInfo);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CInfoItemWeapon	*m_pInfo;					/* ������ */
} CPacketITEM_RES_ITEMWEAPONINFO, *PCPacketITEM_RES_ITEMWEAPONINFO;

/* Copyright(C)URARA-works 2008 */
