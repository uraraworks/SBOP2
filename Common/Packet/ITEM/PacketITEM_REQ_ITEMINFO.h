/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_REQ_ITEMINFO.h									 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e�����v��) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/16													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketITEM_REQ_ITEMINFO : public CPacketBase
{
public:
			CPacketITEM_REQ_ITEMINFO();					/* �R���X�g���N�^ */
	virtual ~CPacketITEM_REQ_ITEMINFO();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwItemID);							/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD m_dwItemID;				/* �A�C�e��ID */
} CPacketITEM_REQ_ITEMINFO, *PCPacketITEM_REQ_ITEMINFO;

/* Copyright(C)URARA-works 2007 */
