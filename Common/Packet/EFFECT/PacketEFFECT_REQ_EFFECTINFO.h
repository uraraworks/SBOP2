/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketEFFECT_REQ_EFFECTINFO.h								 */
/* ���e			:�R�}���h(�G�t�F�N�g�n:�G�t�F�N�g���v��) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketEFFECT_REQ_EFFECTINFO : public CPacketBase
{
public:
			CPacketEFFECT_REQ_EFFECTINFO();				/* �R���X�g���N�^ */
	virtual ~CPacketEFFECT_REQ_EFFECTINFO();			/* �f�X�g���N�^ */

	void	Make	(DWORD dwEffect);							/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD m_dwEffectID;						/* �G�t�F�N�gID */
} CPacketEFFECT_REQ_EFFECTINFO, *PCPacketEFFECT_REQ_EFFECTINFO;

/* Copyright(C)URARA-works 2007 */
