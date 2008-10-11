/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEMWEAPON_RENEW.h								 */
/* ���e			:�R�}���h(�A�C�e���n:������X�V) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItemWeapon;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_ITEMWEAPON_RENEW : public CPacketBase
{
public:
			CPacketADMIN_ITEMWEAPON_RENEW();			/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_ITEMWEAPON_RENEW();			/* �f�X�g���N�^ */

	void	Make	(CInfoItemWeapon *pInfo);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CInfoItemWeapon	*m_pInfo;					/* ������ */
} CPacketADMIN_ITEMWEAPON_RENEW, *PCPacketADMIN_ITEMWEAPON_RENEW;

/* Copyright(C)URARA-works 2007 */
