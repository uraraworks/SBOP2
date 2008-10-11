/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_RES_PUTGET.h									 */
/* ���e			:�R�}���h(�L�����n:�A�C�e�����E���u������) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

enum {
	RESULTID_CHAR_RES_PUTGET_NONE = 0,
	RESULTID_CHAR_RES_PUTGET_PUT,				/* �u���� */
	RESULTID_CHAR_RES_PUTGET_GET,				/* �擾���� */
	RESULTID_CHAR_RES_PUTGET_DELETE,			/* �폜���� */
};

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_RES_PUTGET : public CPacketBase
{
public:
			CPacketCHAR_RES_PUTGET();					/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_RES_PUTGET();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwItemID, int nResult);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwItemID;					/* �A�C�e��ID */
	int		m_nResult;					/* ���� */
} CPacketCHAR_RES_PUTGET, *PCPacketCHAR_RES_PUTGET;

/* Copyright(C)URARA-works 2007 */
