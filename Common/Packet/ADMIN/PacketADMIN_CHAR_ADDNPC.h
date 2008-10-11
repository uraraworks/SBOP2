/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_ADDNPC.h									 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:NPC�̒ǉ�) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/01													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_ADDNPC : public CPacketBase
{
public:
			CPacketADMIN_CHAR_ADDNPC();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_CHAR_ADDNPC();			/* �f�X�g���N�^ */

	void	Make	(CInfoCharBase *pInfo);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	CInfoCharBase	*m_pInfoChar;				/* �L������� */
} CPacketADMIN_CHAR_ADDNPC, *PCPacketADMIN_CHAR_ADDNPC;

/* Copyright(C)URARA-works 2007 */
