/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_CHARINFO.h										 */
/* ���e			:�R�}���h(�L�����n:�L�������ʒm) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/27													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"
#include "LibInfoCharBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_CHARINFO : public CPacketBase
{
public:
			CPacketCHAR_CHARINFO();					/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_CHARINFO();				/* �f�X�g���N�^ */

	void	Make	(PCLibInfoCharBase pInfo);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	PCLibInfoCharBase	m_pLibInfo;				/* �L������� */
} CPacketCHAR_CHARINFO, *PCPacketCHAR_CHARINFO;

/* Copyright(C)URARA-works 2007 */
