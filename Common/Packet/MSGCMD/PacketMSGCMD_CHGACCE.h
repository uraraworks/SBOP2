/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMSGCMD_CHGACCE.h										 */
/* ���e			:�R�}���h(���b�Z�[�W�R�}���h�n:�A�N�Z�T���ύX) ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMSGCMD_CHGACCE : public CPacketBase
{
public:
			CPacketMSGCMD_CHGACCE();					/* �R���X�g���N�^ */
	virtual ~CPacketMSGCMD_CHGACCE();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, int nType);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID;				/* �L����ID */
	int		m_nType;				/* ��� */
} CPacketMSGCMD_CHGACCE, *PCPacketMSGCMD_CHGACCE;

/* Copyright(C)URARA-works 2007 */
