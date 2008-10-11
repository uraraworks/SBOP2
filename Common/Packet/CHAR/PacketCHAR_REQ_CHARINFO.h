/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_CHARINFO.h									 */
/* ���e			:�R�}���h(�L�����n:�L�������v��) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/12/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_CHARINFO : public CPacketBase
{
public:
			CPacketCHAR_REQ_CHARINFO();					/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_REQ_CHARINFO();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID);							/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD m_dwCharID;						/* �L����ID */
} CPacketCHAR_REQ_CHARINFO, *PCPacketCHAR_REQ_CHARINFO;

/* Copyright(C)URARA-works 2006 */
