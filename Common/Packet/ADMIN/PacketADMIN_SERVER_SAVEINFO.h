/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_SERVER_SAVEINFO.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�T�[�o�[���ۑ�) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_SERVER_SAVEINFO : public CPacketBase
{
public:
			CPacketADMIN_SERVER_SAVEINFO();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_SERVER_SAVEINFO();			/* �f�X�g���N�^ */

	void	Make	(void);										/* �p�P�b�g���쐬 */
} CPacketADMIN_SERVER_SAVEINFO, *PCPacketADMIN_SERVER_SAVEINFO;

/* Copyright(C)URARA-works 2006 */
