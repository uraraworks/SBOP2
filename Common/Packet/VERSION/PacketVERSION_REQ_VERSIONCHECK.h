/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_REQ_VERSIONCHECK.h							 */
/* ���e			:�R�}���h(�o�[�W�����n:�o�[�W�����`�F�b�N�v��) ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketVERSION_REQ_VERSIONCHECK : public CPacketBase
{
public:
			CPacketVERSION_REQ_VERSIONCHECK();			/* �R���X�g���N�^ */
	virtual ~CPacketVERSION_REQ_VERSIONCHECK();			/* �f�X�g���N�^ */

	void	Make	(DWORD dwVersion);							/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwVersion;						/* �o�[�W�����R�[�h */
} CPacketVERSION_REQ_VERSIONCHECK, *PCPacketVERSION_REQ_VERSIONCHECK;

/* Copyright(C)URARA-works 2006 */
