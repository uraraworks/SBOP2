/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_RENEWMAPSHADOW.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�e�X�V) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/06													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoMapShadow;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_RENEWMAPSHADOW : public CPacketBase
{
public:
			CPacketADMIN_RENEWMAPSHADOW();			/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_RENEWMAPSHADOW();			/* �f�X�g���N�^ */

	void	Make	(CInfoMapShadow *pInfo);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	CInfoMapShadow	*m_pInfoMapShadow;		/* �}�b�v�e��� */
} CPacketADMIN_RENEWMAPSHADOW, *PCPacketADMIN_RENEWMAPSHADOW;

/* Copyright(C)URARA-works 2007 */
