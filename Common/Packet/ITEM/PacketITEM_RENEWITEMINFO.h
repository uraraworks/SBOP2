/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_RENEWITEMINFO.h									 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e�����X�V) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/19													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItem;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketITEM_RENEWITEMINFO : public CPacketBase
{
public:
			CPacketITEM_RENEWITEMINFO();				/* �R���X�g���N�^ */
	virtual ~CPacketITEM_RENEWITEMINFO();				/* �f�X�g���N�^ */

	void	Make	(CInfoItem *pInfo);							/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CInfoItem	*m_pInfo;					/* �A�C�e����� */
} CPacketITEM_RENEWITEMINFO, *PCPacketITEM_RENEWITEMINFO;

/* Copyright(C)URARA-works 2007 */
