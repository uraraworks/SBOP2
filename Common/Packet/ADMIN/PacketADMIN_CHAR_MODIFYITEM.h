/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_MODIFYITEM.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�����A�C�e���̕ύX) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/24													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

enum {
	CHARMODIFYITEMTYPE_NONE = 0,
	CHARMODIFYITEMTYPE_ADD,							/* �ǉ� */
	CHARMODIFYITEMTYPE_DELETE,						/* �폜 */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_MODIFYITEM : public CPacketBase
{
public:
			CPacketADMIN_CHAR_MODIFYITEM();					/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_CHAR_MODIFYITEM();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, DWORD dwItemID, int nType);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD		m_dwCharID,					/* �L����ID */
				m_dwItemID;					/* �A�C�e��ID */
	int			m_nType;					/* ��� */
} CPacketADMIN_CHAR_MODIFYITEM, *PCPacketADMIN_CHAR_MODIFYITEM;

/* Copyright(C)URARA-works 2007 */
