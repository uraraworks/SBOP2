/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_PARA1.h											 */
/* ���e			:�R�}���h(�}�b�v�n:�ėp[�p�����[�^1��]) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/28													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_PARA1 : public CPacketBase
{
public:
			CPacketMAP_PARA1();								/* �R���X�g���N�^ */
	virtual ~CPacketMAP_PARA1();							/* �f�X�g���N�^ */

	void	Make	(BYTE byCmdSub, DWORD dwCharID, DWORD dwPara);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID,						/* �L����ID */
			m_dwPara;						/* �p�����[�^ */
} CPacketMAP_PARA1, *PCPacketMAP_PARA1;

/* Copyright(C)URARA-works 2006 */
