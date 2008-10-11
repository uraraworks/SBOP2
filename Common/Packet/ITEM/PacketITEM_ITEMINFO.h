/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_ITEMINFO.h										 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e�����ʒm) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"
#include "LibInfo/LibInfoItem.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketITEM_ITEMINFO : public CPacketBase
{
public:
			CPacketITEM_ITEMINFO();					/* �R���X�g���N�^ */
	virtual ~CPacketITEM_ITEMINFO();				/* �f�X�g���N�^ */

	void	Make	(PCLibInfoItem pInfo);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	PCLibInfoItem	m_pLibInfo;				/* �A�C�e����� */
} CPacketITEM_ITEMINFO, *PCPacketITEM_ITEMINFO;

/* Copyright(C)URARA-works 2007 */
