/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketEFFECT_DELETEEFFECTINFO.h							 */
/* ���e			:�R�}���h(�G�t�F�N�g�n:�G�t�F�N�g���폜) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketEFFECT_DELETEEFFECTINFO : public CPacketBase
{
public:
			CPacketEFFECT_DELETEEFFECTINFO();				/* �R���X�g���N�^ */
	virtual ~CPacketEFFECT_DELETEEFFECTINFO();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwEffectID);								/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwEffectID;			/* �G�t�F�N�gID */
} CPacketEFFECT_DELETEEFFECTINFO, *PCPacketEFFECT_DELETEEFFECTINFO;

/* Copyright(C)URARA-works 2007 */
