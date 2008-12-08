/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketSKILL_SKILLINFO.h									 */
/* ���e			:�R�}���h(�X�L���n:�X�L�����ʒm) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/06													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"
#include "LibInfoSkill.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketSKILL_SKILLINFO : public CPacketBase
{
public:
			CPacketSKILL_SKILLINFO();				/* �R���X�g���N�^ */
	virtual ~CPacketSKILL_SKILLINFO();				/* �f�X�g���N�^ */

	void	Make	(PCLibInfoSkill pInfo);					/* �p�P�b�g���쐬 */
	void	Make	(PCInfoSkillBase pInfo);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	PCLibInfoSkill	m_pLibInfo;				/* �X�L����� */
} CPacketSKILL_SKILLINFO, *PCPacketSKILL_SKILLINFO;

/* Copyright(C)URARA-works 2008 */
