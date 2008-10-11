/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_EQUIP.h										 */
/* ���e			:�R�}���h(�L�����n:�����v��) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_EQUIP : public CPacketBase
{
public:
			CPacketCHAR_REQ_EQUIP();								/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_REQ_EQUIP();								/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, DWORD dwItemID, int nUnsetType = -1);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);										/* �p�P�b�g��ݒ� */


public:
	int		m_nUnsetType;			/* �������������� */
	DWORD	m_dwCharID,				/* �L����ID */
			m_dwItemID;				/* ��������A�C�e��ID */
} CPacketCHAR_REQ_EQUIP, *PCPacketCHAR_REQ_EQUIP;

/* Copyright(C)URARA-works 2007 */
