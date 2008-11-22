/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_DRAGITEM.h									 */
/* ���e			:�R�}���h(�L�����n:�A�C�e���ʒu�ύX�v��) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/22													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_DRAGITEM : public CPacketBase
{
public:
			CPacketCHAR_REQ_DRAGITEM();							/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_REQ_DRAGITEM();						/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, DWORD dwItemID, POINT ptNewPos);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID,					/* �L����ID */
			m_dwItemID;					/* �A�C�e��ID */
	POINT	m_ptNewPos;					/* �V�����ʒu */
} CPacketCHAR_REQ_DRAGITEM, *PCPacketCHAR_REQ_DRAGITEM;

/* Copyright(C)URARA-works 2008 */
