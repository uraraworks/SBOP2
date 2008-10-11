/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_REQ_PLAYSOUND.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:���ʉ��̍Đ��v��) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/02													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_REQ_PLAYSOUND : public CPacketBase
{
public:
			CPacketADMIN_REQ_PLAYSOUND();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_REQ_PLAYSOUND();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwSoundID);							/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD m_dwSoundID;					/* ���ʉ�ID */
} CPacketADMIN_REQ_PLAYSOUND, *PCPacketADMIN_REQ_PLAYSOUND;

/* Copyright(C)URARA-works 2008 */
