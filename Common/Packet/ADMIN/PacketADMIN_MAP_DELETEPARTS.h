/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_DELETEPARTS.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�p�[�c�폜) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_DELETEPARTS : public CPacketBase
{
public:
			CPacketADMIN_MAP_DELETEPARTS();					/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_MAP_DELETEPARTS();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwPartsID);								/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwPartsID;			/* �p�[�cID */
} CPacketADMIN_MAP_DELETEPARTS, *PCPacketADMIN_MAP_DELETEPARTS;

/* Copyright(C)URARA-works 2007 */
