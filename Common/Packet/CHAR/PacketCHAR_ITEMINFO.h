/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_ITEMINFO.h										 */
/* ���e			:�R�}���h(�L�����n:�A�C�e�����ʒm) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_ITEMINFO : public CPacketBase
{
public:
			CPacketCHAR_ITEMINFO();						/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_ITEMINFO();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, PARRAYDWORD paSrc);		/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD		m_dwCharID;					/* �L����ID */
	ARRAYDWORD	m_adwItemID;				/* �A�C�e��ID */
} CPacketCHAR_ITEMINFO, *PCPacketCHAR_ITEMINFO;

/* Copyright(C)URARA-works 2007 */
