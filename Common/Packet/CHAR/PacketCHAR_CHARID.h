/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_CHARID.h										 */
/* ���e			:�R�}���h(�L�����n:�L����ID�ʒm) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/02/12													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_CHARID : public CPacketBase
{
public:
			CPacketCHAR_CHARID();						/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_CHARID();						/* �f�X�g���N�^ */

	void	Make	(PARRAYDWORD paSrc);						/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	ARRAYDWORD m_adwCharID;				/* �L����ID */
} CPacketCHAR_CHARID, *PCPacketCHAR_CHARID;

/* Copyright(C)URARA-works 2006 */
