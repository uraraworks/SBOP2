/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_GRP.h											 */
/* ���e			:�R�}���h(�L�����n:�摜�ʒm) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_GRP : public CPacketBase
{
public:
			CPacketCHAR_GRP();							/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_GRP();							/* �f�X�g���N�^ */

	void	Make		(CInfoCharBase *pInfo);					/* �p�P�b�g���쐬 */
	PBYTE	Set			(PBYTE pPacket);						/* �p�P�b�g��ݒ� */
	void	SetParam	(CInfoCharBase *pInfo);					/* �w��L�������փp�����[�^�𔽉f */


public:
	DWORD	m_dwCharID,					/* �L����ID */
			m_dwEquipItemIDCloth,		/* �����A�C�e��ID(��) */
			m_dwEquipItemIDAcce1,		/* �����A�C�e��ID(�A�N�Z�T��) */
			m_dwEquipItemIDArmsRight,	/* �����A�C�e��ID(������) */
			m_dwEquipItemIDArmsLeft;	/* �����A�C�e��ID:���� */
	WORD	m_wGrpIDNPC,				/* �摜ID(NPC) */
			m_wGrpIDCloth,				/* �摜ID(��) */
			m_wGrpIDEye,				/* �摜ID(��) */
			m_wGrpIDEyeColor,			/* �摜ID(�ڐF) */
			m_wGrpIDHairType,			/* �摜ID(��) */
			m_wGrpIDHairColor,			/* �摜ID(���F) */
			m_wGrpIDSP,					/* �摜ID(���ꕞ) */
			m_wGrpIDAcce,				/* �摜ID(�A�N�Z�T��) */
			m_wGrpIDArmsMain,			/* �摜ID(������:���C��) */
			m_wGrpIDArmsSub,			/* �摜ID(������:�T�u) */
			m_wGrpIDArmsLeftMain,		/* �摜ID(��:���C��) */
			m_wGrpIDArmsLeftSub,		/* �摜ID(��:�T�u) */
			m_wGrpIDTmpMain,			/* �摜ID(�ꎞ��:���C��) */
			m_wGrpIDTmpSub;				/* �摜ID(�ꎞ��:�T�u) */
} CPacketCHAR_GRP, *PCPacketCHAR_GRP;

/* Copyright(C)URARA-works 2007 */
