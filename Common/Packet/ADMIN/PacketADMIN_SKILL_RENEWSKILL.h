/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_SKILL_RENEWSKILL.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�X�L�����ʒm) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoSkillBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_SKILL_RENEWSKILL : public CPacketBase
{
public:
			CPacketADMIN_SKILL_RENEWSKILL();			/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_SKILL_RENEWSKILL();			/* �f�X�g���N�^ */

	void	Make	(CInfoSkillBase *pInfo);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CInfoSkillBase	*m_pInfo;				/* �X�L����� */
} CPacketADMIN_SKILL_RENEWSKILL, *PCPacketADMIN_SKILL_RENEWSKILL;

/* Copyright(C)URARA-works 2008 */
