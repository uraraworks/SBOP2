/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketCONNECT_REQ_LOGIN.h									 */
/* ���e			:�R�}���h(�ڑ��n:���O�C���v��) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCONNECT_REQ_LOGIN : public CPacketBase
{
public:
			CPacketCONNECT_REQ_LOGIN();					/* �R���X�g���N�^ */
	virtual ~CPacketCONNECT_REQ_LOGIN();				/* �f�X�g���N�^ */

	void	Make	(LPCSTR pszAccount, LPCSTR pszPassword, PBYTE pbyMacAddr);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	BYTE		m_byMacAddr[6];					/* MAC�A�h���X */
	CmyString	m_strAccount,					/* �A�J�E���g */
				m_strPassword;					/* �p�X���[�h */
} CPacketCONNECT_REQ_LOGIN, *PCPacketCONNECT_REQ_LOGIN;

/* Copyright(C)URARA-works 2006 */
