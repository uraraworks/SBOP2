/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_REQ_FILELIST.h								 */
/* ���e			:�R�}���h(�o�[�W�����n:�t�@�C�����X�g�v��) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/03/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketVERSION_REQ_FILELIST : public CPacketBase
{
public:
			CPacketVERSION_REQ_FILELIST();				/* �R���X�g���N�^ */
	virtual ~CPacketVERSION_REQ_FILELIST();				/* �f�X�g���N�^ */

	void	Make	(void);										/* �p�P�b�g���쐬 */
} CPacketVERSION_REQ_FILELIST, *PCPacketVERSION_REQ_FILELIST;

/* Copyright(C)URARA-works 2006 */
