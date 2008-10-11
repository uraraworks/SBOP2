/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_RES_ITEMINFO.h									 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e����񉞓�) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItem;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketITEM_RES_ITEMINFO : public CPacketBase
{
public:
			CPacketITEM_RES_ITEMINFO();					/* �R���X�g���N�^ */
	virtual ~CPacketITEM_RES_ITEMINFO();				/* �f�X�g���N�^ */

	void	Make	(CInfoItem *pInfo);							/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CInfoItem	*m_pInfo;					/* �A�C�e����� */
} CPacketITEM_RES_ITEMINFO, *PCPacketITEM_RES_ITEMINFO;

/* Copyright(C)URARA-works 2007 */
