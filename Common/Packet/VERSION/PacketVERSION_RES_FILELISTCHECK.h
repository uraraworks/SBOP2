/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_RES_FILELISTCHECK.h							 */
/* ���e			:�R�}���h(�o�[�W�����n:�t�@�C�����X�g�`�F�b�N����) ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/28													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �萔�̒�`																 */
/* ========================================================================= */

enum {
	FILELISTCHECKRES_NONE = 0,
	FILELISTCHECKRES_OK,					/* ��薳�� */
	FILELISTCHECKRES_NG,					/* �s��v */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketVERSION_RES_FILELISTCHECK : public CPacketBase
{
public:
			CPacketVERSION_RES_FILELISTCHECK();			/* �R���X�g���N�^ */
	virtual ~CPacketVERSION_RES_FILELISTCHECK();		/* �f�X�g���N�^ */

	void	Make	(int nResult);								/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	int		m_nResult;					/* ���� */
} CPacketVERSION_RES_FILELISTCHECK, *PCPacketVERSION_RES_FILELISTCHECK;

/* Copyright(C)URARA-works 2007 */
