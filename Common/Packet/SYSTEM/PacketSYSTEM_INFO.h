/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketSYSTEM_INFO.h										 */
/* ���e			:�R�}���h(�V�X�e���n:�V�X�e�����ʒm) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/09/29													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoSystem;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketSYSTEM_INFO : public CPacketBase
{
public:
			CPacketSYSTEM_INFO();						/* �R���X�g���N�^ */
	virtual ~CPacketSYSTEM_INFO();						/* �f�X�g���N�^ */

	void	Make	(CInfoSystem *pInfoSystem);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CInfoSystem		*m_pInfoSystem;				/* �V�X�e����� */
} CPacketSYSTEM_INFO, *PCPacketSYSTEM_INFO;

/* Copyright(C)URARA-works 2008 */
