/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_EFC_RENEWEFFECT.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�G�t�F�N�g���̍X�V) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/06													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoEffect;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_EFC_RENEWEFFECT : public CPacketBase
{
public:
			CPacketADMIN_EFC_RENEWEFFECT();						/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_EFC_RENEWEFFECT();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwEffectID, CInfoEffect *pSrc);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	DWORD		m_dwEffectID;		/* �G�t�F�N�gID */
	CInfoEffect	*m_pInfo;			/* �G�t�F�N�g��� */
} CPacketADMIN_EFC_RENEWEFFECT, *PCPacketADMIN_EFC_RENEWEFFECT;

/* Copyright(C)URARA-works 2008 */
