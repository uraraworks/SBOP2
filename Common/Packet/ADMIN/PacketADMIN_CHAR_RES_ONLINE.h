/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_RES_ONLINE.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�I�����C�����L�����ꗗ����) ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/01													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"
#include "LibInfoCharBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_RES_ONLINE : public CPacketBase
{
public:
			CPacketADMIN_CHAR_RES_ONLINE();			/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_CHAR_RES_ONLINE();		/* �f�X�g���N�^ */

	void	Make	(PCLibInfoCharBase pInfo);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	PCLibInfoCharBase	m_pLibInfo;				/* �L������� */
} CPacketADMIN_CHAR_RES_ONLINE, *PCPacketADMIN_CHAR_RES_ONLINE;

/* Copyright(C)URARA-works 2008 */
