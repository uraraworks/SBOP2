/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_SYSTEMMSG.h										 */
/* ���e			:�R�}���h(�}�b�v�n:�V�X�e�����b�Z�[�W�ʒm) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/02/10													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* ���b�Z�[�W��� */
enum {
	SYSTEMMSGTYPE_DEFAULT = 0,		/* �ʏ� */
	SYSTEMMSGTYPE_NOLOG,			/* ���O�Ɏc���Ȃ� */
	SYSTEMMSGTYPE_BATTLE,			/* �퓬�֘A */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_SYSTEMMSG : public CPacketBase
{
public:
			CPacketMAP_SYSTEMMSG();						/* �R���X�g���N�^ */
	virtual ~CPacketMAP_SYSTEMMSG();					/* �f�X�g���N�^ */

	void	Make	(LPCSTR pszMsg, COLORREF clMsg = 0, BOOL bSound = TRUE, int nMsgType = SYSTEMMSGTYPE_DEFAULT);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);											/* �p�P�b�g��ݒ� */


public:
	int			m_nMsgType;			/* ���b�Z�[�W��� */
	BOOL		m_bSound;			/* �\�����鎞�ɉ����Ȃ炷������ */
	COLORREF	m_clMsg;			/* �\���F */
	CmyString	m_strMsg;			/* �V�X�e�����b�Z�[�W */
} CPacketMAP_SYSTEMMSG, *PCPacketMAP_SYSTEMMSG;

/* Copyright(C)URARA-works 2007 */
