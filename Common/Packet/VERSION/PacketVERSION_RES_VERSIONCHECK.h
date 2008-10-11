/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_RES_VERSIONCHECK.h							 */
/* ���e			:�R�}���h(�o�[�W�����n:�o�[�W�����`�F�b�N����) ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �萔�̒�`																 */
/* ========================================================================= */

enum {
	VERSIONCHECKRES_NONE = 0,
	VERSIONCHECKRES_OK,							/* ��薳�� */
	VERSIONCHECKRES_NG_VERSION,					/* �o�[�W�����s��v */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketVERSION_RES_VERSIONCHECK : public CPacketBase
{
public:
			CPacketVERSION_RES_VERSIONCHECK();			/* �R���X�g���N�^ */
	virtual ~CPacketVERSION_RES_VERSIONCHECK();			/* �f�X�g���N�^ */

	void	Make	(int nResult);								/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	int		m_nResult;					/* ���� */
} CPacketVERSION_RES_VERSIONCHECK, *PCPacketVERSION_RES_VERSIONCHECK;

/* Copyright(C)URARA-works 2006 */
