/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_REQ_FILE.h									 */
/* ���e			:�R�}���h(�o�[�W�����n:�t�@�C���v��) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/01/20													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketVERSION_REQ_FILE : public CPacketBase
{
public:
			CPacketVERSION_REQ_FILE();								/* �R���X�g���N�^ */
	virtual ~CPacketVERSION_REQ_FILE();								/* �f�X�g���N�^ */

	void	Make	(DWORD dwOffset, DWORD dwReqSize, LPCSTR pszFileName);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);										/* �p�P�b�g��ݒ� */


public:
	DWORD		m_dwOffset,						/* �J�n�I�t�Z�b�g */
				m_dwReqSize;					/* �v���T�C�Y */
	CmyString	m_strFileName;					/* ���΃p�X�t���t�@�C���� */
} CPacketVERSION_REQ_FILE, *PCPacketVERSION_REQ_FILE;

/* Copyright(C)URARA-works 2008 */
