/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_DELETEITEMINFO.h								 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e�����폜) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketITEM_DELETEITEMINFO : public CPacketBase
{
public:
			CPacketITEM_DELETEITEMINFO();					/* �R���X�g���N�^ */
	virtual ~CPacketITEM_DELETEITEMINFO();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwItemID);								/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwItemID;			/* �A�C�e��ID */
} CPacketITEM_DELETEITEMINFO, *PCPacketITEM_DELETEITEMINFO;

/* Copyright(C)URARA-works 2007 */
