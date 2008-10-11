/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_USEITEM.h									 */
/* ���e			:�R�}���h(�L�����n:�A�C�e�����g���v��) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_USEITEM : public CPacketBase
{
public:
			CPacketCHAR_REQ_USEITEM();					/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_REQ_USEITEM();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, DWORD dwItemID);			/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID,					/* �L����ID */
			m_dwItemID;					/* �A�C�e��ID */
} CPacketCHAR_REQ_USEITEM, *PCPacketCHAR_REQ_USEITEM;

/* Copyright(C)URARA-works 2008 */
