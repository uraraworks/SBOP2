/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_RENEWSTATUS.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�X�e�[�^�X���X�V) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/12													 */
/* ========================================================================= */

#pragma once

#include "PacketCHAR_STATUS.h"

class CInfoCharBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_RENEWSTATUS : public CPacketCHAR_STATUS
{
public:
			CPacketADMIN_CHAR_RENEWSTATUS();			/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_CHAR_RENEWSTATUS();			/* �f�X�g���N�^ */
} CPacketADMIN_CHAR_RENEWSTATUS, *PCPacketADMIN_CHAR_RENEWSTATUS;

/* Copyright(C)URARA-works 2008 */
