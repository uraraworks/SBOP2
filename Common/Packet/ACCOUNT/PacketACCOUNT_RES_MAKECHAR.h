/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketACCOUNT_RES_MAKECHAR.h								 */
/* ���e			:�R�}���h(�A�J�E���g�n:�L�����쐬����) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/12/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �萔�̒�`																 */
/* ========================================================================= */

enum {
	MAKECHARRES_NONE = 0,
	MAKECHARRES_OK,								/* ��薳�� */
	MAKECHARRES_NG_USE,							/* �g�p�ς� */
	MAKECHARRES_NG_SPACE,						/* �󔒂��g�p����Ă��� */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketACCOUNT_RES_MAKECHAR : public CPacketBase
{
public:
			CPacketACCOUNT_RES_MAKECHAR();				/* �R���X�g���N�^ */
	virtual ~CPacketACCOUNT_RES_MAKECHAR();				/* �f�X�g���N�^ */

	void	Make	(int nResult, DWORD dwCharID);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	int		m_nResult;					/* ���� */
	DWORD	m_dwCharID;					/* �L����ID */
} CPacketACCOUNT_RES_MAKECHAR, *PCPacketACCOUNT_RES_MAKECHAR;

/* Copyright(C)URARA-works 2006 */
