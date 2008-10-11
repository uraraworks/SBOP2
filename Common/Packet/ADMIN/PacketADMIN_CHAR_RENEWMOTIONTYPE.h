/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_RENEWMOTIONTYPE.h							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�L�������[�V������ʏ��̍X�V) ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMotionType;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_RENEWMOTIONTYPE : public CPacketBase
{
public:
			CPacketADMIN_CHAR_RENEWMOTIONTYPE();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_CHAR_RENEWMOTIONTYPE();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwMotionTypeID, CLibInfoMotionType *pSrc);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	DWORD				m_dwMotionTypeID;	/* ���[�V�������ID */
	CLibInfoMotionType	*m_pInfo;			/* ���[�V������ʏ�� */
} CPacketADMIN_CHAR_RENEWMOTIONTYPE, *PCPacketADMIN_CHAR_RENEWMOTIONTYPE;

/* Copyright(C)URARA-works 2008 */
