/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_MODIFY_PARAM.h									 */
/* ���e			:�R�}���h(�L�����n:�p�����[�^�ύX�ʒm) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/09													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

enum {
	PARAMID_CHAR_MODIFY_NONE = 0,
	PARAMID_CHAR_MODIFY_ANIME,					/* �A�j���[�V�����ԍ��̕ύX */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_MODIFY_PARAM : public CPacketBase
{
public:
			CPacketCHAR_MODIFY_PARAM();					/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_MODIFY_PARAM();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, int nType, int nParam);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID;					/* �L����ID */
	int		m_nType,					/* ��� */
			m_nParam;					/* �p�����[�^ */
} CPacketCHAR_MODIFY_PARAM, *PCPacketCHAR_MODIFY_PARAM;

/* Copyright(C)URARA-works 2007 */
