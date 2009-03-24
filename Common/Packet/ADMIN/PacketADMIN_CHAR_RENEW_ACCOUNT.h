/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_RENEW_ACCOUNT.h							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�J�E���g���X�V) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/07													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoAccount;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_RENEW_ACCOUNT : public CPacketBase
{
public:
			CPacketADMIN_CHAR_RENEW_ACCOUNT();			/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_CHAR_RENEW_ACCOUNT();			/* �f�X�g���N�^ */

	void	Make	(CInfoAccount *pInfoAccount, BOOL bDisable=FALSE, LPCSTR pszMacAddress=NULL);/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	BOOL		m_bDisable;					/* ���O�C������ */
	DWORD		m_dwAccountID;				/* �A�J�E���gID */
	CmyString	m_strAccount,				/* �A�J�E���g */
				m_strPassword,				/* �p�X���[�h */
				m_strMacAddress;			/* ����MAC�A�h���X */
} CPacketADMIN_CHAR_RENEW_ACCOUNT, *PCPacketADMIN_CHAR_RENEW_ACCOUNT;

/* Copyright(C)URARA-works 2008 */
