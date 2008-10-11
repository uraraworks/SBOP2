/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_RENEW_CLIENTVERSION.h							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�N���C�A���g�o�[�W�����X�V) ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/07													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoAccount;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_RENEW_CLIENTVERSION : public CPacketBase
{
public:
			CPacketADMIN_RENEW_CLIENTVERSION();			/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_RENEW_CLIENTVERSION();		/* �f�X�g���N�^ */

	void	Make	(LPCSTR pszClientVersion);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CmyString	m_strClientVersion;		/* �N���C�A���g�o�[�W���� */
} CPacketADMIN_RENEW_CLIENTVERSION, *PCPacketADMIN_RENEW_CLIENTVERSION;

/* Copyright(C)URARA-works 2008 */
