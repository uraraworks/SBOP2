/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_RES_TAIL.h										 */
/* ���e			:�R�}���h(�L�����n:�t���čs������) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

enum {
	RES_TAIL_RESULT_NONE = 0,
	RES_TAIL_RESULT_OK,							/* ���Ă��� */
	RES_TAIL_RESULT_PARGE,						/* ���Ă������� */
	RES_TAIL_RESULT_TAILPARGE,					/* ��Ă������� */
	RES_TAIL_RESULT_NG_TAIL,					/* ���ɂ��Ă����Ă��� */
	RES_TAIL_RESULT_NG_STATE,					/* ���Ă����Ȃ���� */
	RES_TAIL_RESULT_NG_SWOON,					/* �C�⒆�Ȃ̂ł��Ă����Ȃ� */
	RES_TAIL_RESULT_NG_PLACE,					/* ���Ă����Ȃ��ꏊ */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_RES_TAIL : public CPacketBase
{
public:
			CPacketCHAR_RES_TAIL();									/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_RES_TAIL();								/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, DWORD dwTargetCharID, int nResult, DWORD dwPara=0);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);										/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID,						/* �L����ID */
			m_dwTargetCharID,				/* �t���čs������̃L����ID */
			m_dwPara;						/* �p�����[�^ */
	int		m_nResult;						/* ���� */
} CPacketCHAR_RES_TAIL, *PCPacketCHAR_RES_TAIL;

/* Copyright(C)URARA-works 2007 */
