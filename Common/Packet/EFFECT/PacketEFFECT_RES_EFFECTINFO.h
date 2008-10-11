/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketEFFECT_RES_EFFECTINFO.h								 */
/* ���e			:�R�}���h(�G�t�F�N�g�n:�G�t�F�N�g��񉞓�) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoEffect;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketEFFECT_RES_EFFECTINFO : public CPacketBase
{
public:
			CPacketEFFECT_RES_EFFECTINFO();				/* �R���X�g���N�^ */
	virtual ~CPacketEFFECT_RES_EFFECTINFO();			/* �f�X�g���N�^ */

	void	Make	(CInfoEffect *pInfo);						/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CInfoEffect	*m_pInfo;					/* �G�t�F�N�g��� */
} CPacketEFFECT_RES_EFFECTINFO, *PCPacketEFFECT_RES_EFFECTINFO;

/* Copyright(C)URARA-works 2007 */
