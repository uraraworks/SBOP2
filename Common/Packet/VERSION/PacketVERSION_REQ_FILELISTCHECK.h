/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_REQ_FILELISTCHECK.h							 */
/* ���e			:�R�}���h(�o�[�W�����n:�t�@�C�����X�g�`�F�b�N�v��) ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/28													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketVERSION_REQ_FILELISTCHECK : public CPacketBase
{
public:
			CPacketVERSION_REQ_FILELISTCHECK();			/* �R���X�g���N�^ */
	virtual ~CPacketVERSION_REQ_FILELISTCHECK();		/* �f�X�g���N�^ */

	void	Make	(LPCSTR pszFileListHash);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CmyString	m_strFileListHash;				/* �t�@�C�����X�g�n�b�V�� */
} CPacketVERSION_REQ_FILELISTCHECK, *PCPacketVERSION_REQ_FILELISTCHECK;

/* Copyright(C)URARA-works 2006 */
