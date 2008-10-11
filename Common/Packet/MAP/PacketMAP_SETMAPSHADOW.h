/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_SETMAPSHADOW.h									 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�e�z�u) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_SETMAPSHADOW : public CPacketBase
{
public:
			CPacketMAP_SETMAPSHADOW();						/* �R���X�g���N�^ */
	virtual ~CPacketMAP_SETMAPSHADOW();						/* �f�X�g���N�^ */

	void	Make	(DWORD dwMapID, int x, int y, DWORD dwShadowID);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	POINT	m_ptPos;				/* ���W */
	DWORD	m_dwMapID,				/* �}�b�vID */
			m_dwShadowID;			/* �}�b�v�eID */
} CPacketMAP_SETMAPSHADOW, *PCPacketMAP_SETMAPSHADOW;

/* Copyright(C)URARA-works 2007 */
