/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMSGCMD_CHGCLOTH.h									 */
/* ���e			:�R�}���h(���b�Z�[�W�R�}���h�n:�����ύX) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/02													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMSGCMD_CHGCLOTH : public CPacketBase
{
public:
			CPacketMSGCMD_CHGCLOTH();					/* �R���X�g���N�^ */
	virtual ~CPacketMSGCMD_CHGCLOTH();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, int nType);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID;				/* �L����ID */
	int		m_nType;				/* ��� */
} CPacketMSGCMD_CHGCLOTH, *PCPacketMSGCMD_CHGCLOTH;

/* Copyright(C)URARA-works 2007 */
