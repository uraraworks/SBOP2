/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_ADDMOTIONTYPE.h							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�L�������[�V������ʏ��̒ǉ�) ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/09													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_ADDMOTIONTYPE : public CPacketBase
{
public:
			CPacketADMIN_CHAR_ADDMOTIONTYPE();			/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_CHAR_ADDMOTIONTYPE();			/* �f�X�g���N�^ */

	void	Make	(void);										/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
} CPacketADMIN_CHAR_ADDMOTIONTYPE, *PCPacketADMIN_CHAR_ADDMOTIONTYPE;

/* Copyright(C)URARA-works 2008 */
