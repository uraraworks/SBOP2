/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_RENEWITEMTYPEINFO.h								 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e����ʏ��X�V) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoItemTypeBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketITEM_RENEWITEMTYPEINFO : public CPacketBase
{
public:
			CPacketITEM_RENEWITEMTYPEINFO();			/* �R���X�g���N�^ */
	virtual ~CPacketITEM_RENEWITEMTYPEINFO();			/* �f�X�g���N�^ */

	void	Make	(CInfoItemTypeBase *pInfo);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CInfoItemTypeBase	*m_pInfo;					/* �A�C�e����ʏ�� */
} CPacketITEM_RENEWITEMTYPEINFO, *PCPacketITEM_RENEWITEMTYPEINFO;

/* Copyright(C)URARA-works 2007 */
