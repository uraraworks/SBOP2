/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketACCOUNT_RES_ACCOUNTINFO.h							 */
/* ���e			:�R�}���h(�A�J�E���g�n:�A�J�E���g��񉞓�) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/07													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoAccount;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketACCOUNT_RES_ACCOUNTINFO : public CPacketBase
{
public:
			CPacketACCOUNT_RES_ACCOUNTINFO();			/* �R���X�g���N�^ */
	virtual ~CPacketACCOUNT_RES_ACCOUNTINFO();			/* �f�X�g���N�^ */

	void	Make	(CInfoAccount *pInfo);						/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CInfoAccount	*m_pInfo;					/* �A�J�E���g��� */
} CPacketACCOUNT_RES_ACCOUNTINFO, *PCPacketACCOUNT_RES_ACCOUNTINFO;

/* Copyright(C)URARA-works 2006 */
