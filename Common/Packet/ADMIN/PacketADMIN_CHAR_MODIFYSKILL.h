/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_MODIFYSKILL.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�����X�L���̕ύX) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/01/18													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

enum {
	CHARMODIFYSKILLTYPE_NONE = 0,
	CHARMODIFYSKILLTYPE_ADD,						/* �ǉ� */
	CHARMODIFYSKILLTYPE_DELETE,						/* �폜 */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_MODIFYSKILL : public CPacketBase
{
public:
			CPacketADMIN_CHAR_MODIFYSKILL();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_CHAR_MODIFYSKILL();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, DWORD dwSkillID, int nType);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD		m_dwCharID,					/* �L����ID */
				m_dwSkillID;				/* �X�L��ID */
	int			m_nType;					/* ��� */
} CPacketADMIN_CHAR_MODIFYSKILL, *PCPacketADMIN_CHAR_MODIFYSKILL;

/* Copyright(C)URARA-works 2009 */
