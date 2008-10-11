/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_SETPARTS.h										 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�p�[�c�z�u) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/30													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_SETPARTS : public CPacketBase
{
public:
			CPacketMAP_SETPARTS();							/* �R���X�g���N�^ */
	virtual ~CPacketMAP_SETPARTS();							/* �f�X�g���N�^ */

	void	Make	(DWORD dwMapID, int x, int y, DWORD dwPartsID);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	POINT	m_ptPos;				/* ���W */
	DWORD	m_dwMapID,				/* �}�b�vID */
			m_dwPartsID;			/* �p�[�cID */
} CPacketMAP_SETPARTS, *PCPacketMAP_SETPARTS;

/* Copyright(C)URARA-works 2007 */
