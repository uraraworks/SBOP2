/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketEFFECT_EFFECTINFO.h									 */
/* ���e			:�R�}���h(�G�t�F�N�g�n:�G�t�F�N�g���ʒm) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"
#include "LibInfo/LibInfoEffect.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketEFFECT_EFFECTINFO : public CPacketBase
{
public:
			CPacketEFFECT_EFFECTINFO();				/* �R���X�g���N�^ */
	virtual ~CPacketEFFECT_EFFECTINFO();			/* �f�X�g���N�^ */

	void	Make	(PCLibInfoEffect pInfo);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	PCLibInfoEffect	m_pLibInfo;				/* �G�t�F�N�g��� */
} CPacketEFFECT_EFFECTINFO, *PCPacketEFFECT_EFFECTINFO;

/* Copyright(C)URARA-works 2007 */
