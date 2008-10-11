/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_DELETEMAPSHADOW.h								 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�e�폜) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_DELETEMAPSHADOW : public CPacketBase
{
public:
			CPacketMAP_DELETEMAPSHADOW();					/* �R���X�g���N�^ */
	virtual ~CPacketMAP_DELETEMAPSHADOW();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwShadowID);								/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwShadowID;			/* �}�b�v�eID */
} CPacketMAP_DELETEMAPSHADOW, *PCPacketMAP_DELETEMAPSHADOW;

/* Copyright(C)URARA-works 2007 */
