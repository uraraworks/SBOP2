/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMSGCMD_CHGARMS.h										 */
/* ���e			:�R�}���h(���b�Z�[�W�R�}���h�n:�������ύX) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/21													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMSGCMD_CHGARMS : public CPacketBase
{
public:
			CPacketMSGCMD_CHGARMS();					/* �R���X�g���N�^ */
	virtual ~CPacketMSGCMD_CHGARMS();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, int nType);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID;				/* �L����ID */
	int		m_nType;				/* ��� */
} CPacketMSGCMD_CHGARMS, *PCPacketMSGCMD_CHGARMS;

/* Copyright(C)URARA-works 2007 */
