/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketCONNECT_RES_LOGIN.h									 */
/* ���e			:�R�}���h(�ڑ��n:���O�C������) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �萔�̒�`																 */
/* ========================================================================= */

enum {
	LOGINRES_NONE = 0,
	LOGINRES_OK,							/* ��薳�� */
	LOGINRES_NG_PASSWORD,					/* �p�X���[�h�s��v */
	LOGINRES_NG_LOGIN,						/* ���O�C���ς� */
	LOGINRES_NG_MAC,						/* �쐬�ς� */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCONNECT_RES_LOGIN : public CPacketBase
{
public:
			CPacketCONNECT_RES_LOGIN();					/* �R���X�g���N�^ */
	virtual ~CPacketCONNECT_RES_LOGIN();				/* �f�X�g���N�^ */

	void	Make	(int nResult, DWORD dwAccountID);			/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	int		m_nResult;					/* ���� */
	DWORD	m_dwAccountID;				/* �A�J�E���gID */
} CPacketCONNECT_RES_LOGIN, *PCPacketCONNECT_RES_LOGIN;

/* Copyright(C)URARA-works 2006 */
