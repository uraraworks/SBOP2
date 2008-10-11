/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_RENEWMOTION.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�L�������[�V�������̍X�V) ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/11/23													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMotion;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_RENEWMOTION : public CPacketBase
{
public:
			CPacketADMIN_CHAR_RENEWMOTION();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_CHAR_RENEWMOTION();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwMotionTypeID, DWORD dwMotionListID, CLibInfoMotion *pSrc);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD			m_dwMotionTypeID,	/* ���[�V�������ID */
					m_dwMotionListID;	/* ���[�V�������X�gID */
	CLibInfoMotion	*m_pInfo;			/* ���[�V������� */
} CPacketADMIN_CHAR_RENEWMOTION, *PCPacketADMIN_CHAR_RENEWMOTION;

/* Copyright(C)URARA-works 2007 */
