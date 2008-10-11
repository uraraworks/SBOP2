/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_TAIL.h										 */
/* ���e			:�R�}���h(�L�����n:�t���čs���v��) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_TAIL : public CPacketBase
{
public:
			CPacketCHAR_REQ_TAIL();						/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_REQ_TAIL();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, DWORD dwTargetCharID, BOOL bTail);		/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);										/* �p�P�b�g��ݒ� */


public:
	BOOL	m_bTail;						/* ���Ă����������� */
	DWORD	m_dwCharID,						/* �L����ID */
			m_dwTargetCharID;				/* �t���čs������̃L����ID */
} CPacketCHAR_REQ_TAIL, *PCPacketCHAR_REQ_TAIL;

/* Copyright(C)URARA-works 2007 */
