/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_RES_FILE.h									 */
/* ���e			:�R�}���h(�o�[�W�����n:�t�@�C������) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/01/20													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketVERSION_RES_FILE : public CPacketBase
{
public:
			CPacketVERSION_RES_FILE();											/* �R���X�g���N�^ */
	virtual ~CPacketVERSION_RES_FILE();											/* �f�X�g���N�^ */

	void	Make	(DWORD dwOffset, DWORD dwDataSize, LPCSTR pszFileName, PBYTE pFileData);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);															/* �p�P�b�g��ݒ� */


public:
	DWORD		m_dwOffset,						/* �J�n�I�t�Z�b�g */
				m_dwSize;						/* �T�C�Y */
	CmyString	m_strFileName;					/* ���΃p�X�t���t�@�C���� */
	PBYTE		m_pFileData;					/* �t�@�C���f�[�^ */
} CPacketVERSION_RES_FILE, *PCPacketVERSION_RES_FILE;

/* Copyright(C)URARA-works 2008 */
