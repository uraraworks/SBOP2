/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_DELETEITEMTYPEINFO.h							 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e����ʏ��폜) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketITEM_DELETEITEMTYPEINFO : public CPacketBase
{
public:
			CPacketITEM_DELETEITEMTYPEINFO();				/* �R���X�g���N�^ */
	virtual ~CPacketITEM_DELETEITEMTYPEINFO();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwItemID);								/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwTypeID;			/* ���ID */
} CPacketITEM_DELETEITEMTYPEINFO, *PCPacketITEM_DELETEITEMTYPEINFO;

/* Copyright(C)URARA-works 2007 */
