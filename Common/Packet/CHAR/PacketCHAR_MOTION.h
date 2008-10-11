/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_MOTION.h										 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:���[�V�������ʒm) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/11/23													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMotion;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_MOTION : public CPacketBase
{
public:
			CPacketCHAR_MOTION();							/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_MOTION();							/* �f�X�g���N�^ */

	void	Make	(DWORD dwMotionTypeID, DWORD dwMotionListID, CLibInfoMotion *pSrc);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD			m_dwMotionTypeID,	/* ���[�V�������ID */
					m_dwMotionListID;	/* ���[�V�������X�gID */
	CLibInfoMotion	*m_pInfo;			/* ���[�V������� */
} CPacketCHAR_MOTION, *PCPacketCHAR_MOTION;

/* Copyright(C)URARA-works 2007 */
