/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_REQ_ITEMWEAPONINFO.h							 */
/* ���e			:�R�}���h(�A�C�e���n:������v��) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketITEM_REQ_ITEMWEAPONINFO : public CPacketBase
{
public:
			CPacketITEM_REQ_ITEMWEAPONINFO();			/* �R���X�g���N�^ */
	virtual ~CPacketITEM_REQ_ITEMWEAPONINFO();			/* �f�X�g���N�^ */

	void	Make	(DWORD dwWeaponInfoID);						/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD m_dwWeaponInfoID;			/* ������ID */
} CPacketITEM_REQ_ITEMWEAPONINFO, *PCPacketITEM_REQ_ITEMWEAPONINFO;

/* Copyright(C)URARA-works 2008 */
