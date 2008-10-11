/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_ITEMTYPEINFO.h									 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e����ʏ��ʒm) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"
#include "LibInfoItemType.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketITEM_ITEMTYPEINFO : public CPacketBase
{
public:
			CPacketITEM_ITEMTYPEINFO();				/* �R���X�g���N�^ */
	virtual ~CPacketITEM_ITEMTYPEINFO();			/* �f�X�g���N�^ */

	void	Make	(PCLibInfoItemType pInfo);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	PCLibInfoItemType	m_pLibInfo;				/* �A�C�e����ʏ�� */
} CPacketITEM_ITEMTYPEINFO, *PCPacketITEM_ITEMTYPEINFO;

/* Copyright(C)URARA-works 2007 */
