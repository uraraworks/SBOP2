/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_FORMATMSG.h										 */
/* ���e			:�R�}���h(�}�b�v�n:�t�H�[�}�b�g���b�Z�[�W�ʒm) ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* ���b�Z�[�W��� */
enum {
	FORMATMSGTYPE_DEFAULT = 0,		/* �ʏ� */
	FORMATMSGTYPE_NOLOG,			/* ���O�Ɏc���Ȃ� */
	FORMATMSGTYPE_BATTLE,			/* �퓬�֘A */
};

/* ���b�Z�[�WID */
enum {
	FORMATMSGID_NONE = 0,
	FORMATMSGID_DAMAGE1,			/* %s���� %d �̃_���[�W */
	FORMATMSGID_DAMAGE2,			/* %s�� %d �̃_���[�W */
	FORMATMSGID_SWOON1,				/* %s��%s��|���� */
	FORMATMSGID_SWOON2,				/* %s��|���� */
	FORMATMSGID_SWOON3,				/* %s�ɓ|���ꂽ */
	FORMATMSGID_NOUSE,				/* %s�͌��ʂ�����܂���ł��� */
	FORMATMSGID_NOUNEQUIP_ITEMFULL,	/* �������������ς��ő������O���܂��� */
	FORMATMSGID_DROPITEM,			/* %s��%s�𗎂Ƃ��� */
};

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_FORMATMSG : public CPacketBase
{
public:
			CPacketMAP_FORMATMSG();						/* �R���X�g���N�^ */
	virtual ~CPacketMAP_FORMATMSG();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwMsgID, DWORD dwPara1 = 0, DWORD dwPara2 = 0, COLORREF clMsg = 0, BOOL bSound = TRUE, int nMsgType = FORMATMSGTYPE_DEFAULT);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);											/* �p�P�b�g��ݒ� */


public:
	int			m_nMsgType;			/* ���b�Z�[�W��� */
	BOOL		m_bSound;			/* �\�����鎞�ɉ����Ȃ炷������ */
	DWORD		m_dwMsgID,			/* ���b�Z�[�WID */
				m_dwPara1,			/* �p�����[�^1 */
				m_dwPara2;			/* �p�����[�^2 */
	COLORREF	m_clMsg;			/* �\���F */
} CPacketMAP_FORMATMSG, *PCPacketMAP_FORMATMSG;

/* Copyright(C)URARA-works 2008 */
