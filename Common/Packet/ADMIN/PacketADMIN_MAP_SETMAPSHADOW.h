/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_SETMAPSHADOW.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�e�z�u) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/07													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_SETMAPSHADOW : public CPacketBase
{
public:
			CPacketADMIN_MAP_SETMAPSHADOW();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_MAP_SETMAPSHADOW();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwMapID, int x, int y, DWORD dwShadowID);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	POINT	m_ptPos;				/* ���W */
	DWORD	m_dwMapID,				/* �}�b�vID */
			m_dwShadowID;			/* �}�b�v�eID */
} CPacketADMIN_MAP_SETMAPSHADOW, *PCPacketADMIN_MAP_SETMAPSHADOW;

/* Copyright(C)URARA-works 2007 */
