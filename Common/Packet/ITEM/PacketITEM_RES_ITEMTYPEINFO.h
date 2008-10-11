/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_RES_ITEMTYPEINFO.h								 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e����ʏ�񉞓�) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItemTypeBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketITEM_RES_ITEMTYPEINFO : public CPacketBase
{
public:
			CPacketITEM_RES_ITEMTYPEINFO();				/* �R���X�g���N�^ */
	virtual ~CPacketITEM_RES_ITEMTYPEINFO();			/* �f�X�g���N�^ */

	void	Make	(CInfoItemTypeBase *pInfo);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CInfoItemTypeBase	*m_pInfo;					/* �A�C�e����ʏ�� */
} CPacketITEM_RES_ITEMTYPEINFO, *PCPacketITEM_RES_ITEMTYPEINFO;

/* Copyright(C)URARA-works 2007 */
