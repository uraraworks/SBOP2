/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMSGCMD_PARA1.h										 */
/* ���e			:�R�}���h(���b�Z�[�W�R�}���h�n:�ėp[�p�����[�^1��]) ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMSGCMD_PARA1 : public CPacketBase
{
public:
			CPacketMSGCMD_PARA1();								/* �R���X�g���N�^ */
	virtual ~CPacketMSGCMD_PARA1();								/* �f�X�g���N�^ */

	void	Make	(BYTE byCmdSub, DWORD dwCharID, DWORD dwPara);		/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID,				/* �L����ID */
			m_dwPara;				/* �p�����[�^ */
} CPacketMSGCMD_PARA1, *PCPacketMSGCMD_PARA1;

/* Copyright(C)URARA-works 2007 */
