/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketACCOUNT_REQ_MAKECHAR.h								 */
/* ���e			:�R�}���h(�A�J�E���g�n:�L�����쐬�v��) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/14													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketACCOUNT_REQ_MAKECHAR : public CPacketBase
{
public:
			CPacketACCOUNT_REQ_MAKECHAR();					/* �R���X�g���N�^ */
	virtual ~CPacketACCOUNT_REQ_MAKECHAR();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwAccountID, CInfoCharBase *pInfoCharBase);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	DWORD			m_dwAccountID;				/* �A�J�E���gID */
	CInfoCharBase	*m_pInfoCharBase;			/* �L������� */
} CPacketACCOUNT_REQ_MAKECHAR, *PCPacketACCOUNT_REQ_MAKECHAR;

/* Copyright(C)URARA-works 2006 */
