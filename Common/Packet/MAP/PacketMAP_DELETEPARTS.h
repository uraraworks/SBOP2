/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_DELETEPARTS.h									 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�p�[�c�폜) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_DELETEPARTS : public CPacketBase
{
public:
			CPacketMAP_DELETEPARTS();						/* �R���X�g���N�^ */
	virtual ~CPacketMAP_DELETEPARTS();						/* �f�X�g���N�^ */

	void	Make	(DWORD dwPartsID);								/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwPartsID;			/* �p�[�cID */
} CPacketMAP_DELETEPARTS, *PCPacketMAP_DELETEPARTS;

/* Copyright(C)URARA-works 2007 */
