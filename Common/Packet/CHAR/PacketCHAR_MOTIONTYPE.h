/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_MOTIONTYPE.h									 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:���[�V������ʏ��ʒm) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMotionType;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_MOTIONTYPE : public CPacketBase
{
public:
			CPacketCHAR_MOTIONTYPE();							/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_MOTIONTYPE();							/* �f�X�g���N�^ */

	void	Make	(DWORD dwMotionTypeID, CLibInfoMotionType *pSrc);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	DWORD				m_dwMotionTypeID;	/* ���[�V�������ID */
	CLibInfoMotionType	*m_pInfo;			/* ���[�V������ʏ�� */
} CPacketCHAR_MOTIONTYPE, *PCPacketCHAR_MOTIONTYPE;

/* Copyright(C)URARA-works 2008 */
