/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_SYSTEM_RENEWINFO.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�V�X�e�����̍X�V) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoSystem;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_SYSTEM_RENEWINFO : public CPacketBase
{
public:
			CPacketADMIN_SYSTEM_RENEWINFO();						/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_SYSTEM_RENEWINFO();						/* �f�X�g���N�^ */

	void	Make	(CInfoSystem *pInfoSystem);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	CInfoSystem		*m_pInfoSystem;				/* �V�X�e����� */
} CPacketADMIN_SYSTEM_RENEWINFO, *PCPacketADMIN_SYSTEM_RENEWINFO;

/* Copyright(C)URARA-works 2008 */
