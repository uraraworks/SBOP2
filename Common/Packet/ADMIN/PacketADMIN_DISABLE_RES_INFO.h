/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_DISABLE_RES_INFO.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:���ۏ�񉞓�) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/04/11													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoDisable;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_DISABLE_RES_INFO : public CPacketBase
{
public:
			CPacketADMIN_DISABLE_RES_INFO();						/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_DISABLE_RES_INFO();						/* �f�X�g���N�^ */

	void	Make	(CLibInfoDisable *pLibInfoDisable);		/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	CLibInfoDisable		*m_pLibInfoDisable;				/* ���ۏ�� */
} CPacketADMIN_DISABLE_RES_INFO, *PCPacketADMIN_DISABLE_RES_INFO;

/* Copyright(C)URARA-works 2009 */
