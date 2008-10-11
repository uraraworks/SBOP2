/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_PUSH.h										 */
/* ���e			:�R�}���h(�L�����n:�����v��) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/02													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_PUSH : public CPacketBase
{
public:
			CPacketCHAR_REQ_PUSH();						/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_REQ_PUSH();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, int nDirection, int nPushCount = 1);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID;				/* �L����ID */
	int		m_nDirection,			/* ���� */
			m_nPushCount;			/* �����Đi�ސ� */
} CPacketCHAR_REQ_PUSH, *PCPacketCHAR_REQ_PUSH;

/* Copyright(C)URARA-works 2007 */
