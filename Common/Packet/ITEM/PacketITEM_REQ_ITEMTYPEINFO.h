/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_REQ_ITEMTYPEINFO.h								 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e����ʏ��v��) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketITEM_REQ_ITEMTYPEINFO : public CPacketBase
{
public:
			CPacketITEM_REQ_ITEMTYPEINFO();				/* �R���X�g���N�^ */
	virtual ~CPacketITEM_REQ_ITEMTYPEINFO();			/* �f�X�g���N�^ */

	void	Make	(DWORD dwTypeID);							/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD m_dwTypeID;				/* ���ID */
} CPacketITEM_REQ_ITEMTYPEINFO, *PCPacketITEM_REQ_ITEMTYPEINFO;

/* Copyright(C)URARA-works 2007 */
