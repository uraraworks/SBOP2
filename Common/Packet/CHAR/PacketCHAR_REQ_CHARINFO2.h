/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_CHARINFO2.h									 */
/* ���e			:�R�}���h(�L�����n:�L�������v��(����)) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/02/13													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_CHARINFO2 : public CPacketBase
{
public:
			CPacketCHAR_REQ_CHARINFO2();				/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_REQ_CHARINFO2();				/* �f�X�g���N�^ */

	void	Make	(PARRAYDWORD paSrc);						/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	ARRAYDWORD m_adwCharID;				/* �L����ID */
} CPacketCHAR_REQ_CHARINFO2, *PCPacketCHAR_REQ_CHARINFO2;

/* Copyright(C)URARA-works 2007 */
