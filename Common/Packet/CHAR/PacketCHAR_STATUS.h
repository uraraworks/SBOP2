/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_STATUS.h										 */
/* ���e			:�R�}���h(�L�����n:�X�e�[�^�X���ʒm) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/29													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_STATUS : public CPacketBase
{
public:
			CPacketCHAR_STATUS();						/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_STATUS();						/* �f�X�g���N�^ */

	void	Make		(CInfoCharBase *pInfo);					/* �p�P�b�g���쐬 */
	PBYTE	Set			(PBYTE pPacket);						/* �p�P�b�g��ݒ� */
	void	SetParam	(CInfoCharBase *pInfo);					/* �w��L�������փp�����[�^�𔽉f */


public:
	BYTE	m_byCmdMain,				/* �R�}���hID���C�� */
			m_byCmdSub;					/* �R�}���hID�T�u */
	WORD	m_wAtackGauge,				/* �A�^�b�N�Q�[�W */
			m_wDefenseGauge,			/* �f�B�t�F���X�Q�[�W */
			m_wLevel,					/* ���x�� */
			m_wStamina,					/* �X�^�~�i */
			m_wPower,					/* �r�� */
			m_wStrength,				/* �̗� */
			m_wMagic,					/* ���� */
			m_wSkillful,				/* ��p */
			m_wAbillityAT,				/* �U���Z�\ */
			m_wAbillityDF,				/* �h��Z�\ */
			m_wPAtack,					/* �U���� */
			m_wPDefense,				/* �h��� */
			m_wPMagic,					/* ���@�� */
			m_wPMagicDefense,			/* ���@�h��� */
			m_wPHitAverage,				/* ������ */
			m_wPAvoidAverage,			/* ��� */
			m_wPCriticalAverage,		/* �N���e�B�J���� */
			m_wAttrFire,				/* ����[��] */
			m_wAttrWind,				/* ����[��] */
			m_wAttrWater,				/* ����[��] */
			m_wAttrEarth,				/* ����[�y] */
			m_wAttrLight,				/* ����[��] */
			m_wAttrDark;				/* ����[��] */
	DWORD	m_dwCharID,					/* �L����ID */
			m_dwMoveWait,				/* �ړ��҂����� */
			m_dwMoveWaitBattle,			/* �퓬���ړ��҂����� */
			m_dwExp,					/* �o���l */
			m_dwHP,						/* HP */
			m_dwMaxHP,					/* �ő�HP */
			m_dwSP,						/* SP */
			m_dwMaxSP,					/* �ő�SP */
			m_dwLightTime;				/* ����p������ */
	int		m_nDropItemAverage,			/* �A�C�e���h���b�v�� */
			m_nMoveAverage,				/* �ړ��m�� */
			m_nMoveAverageBattle,		/* �퓬���ړ��m�� */
			m_nLightLevel;				/* ���背�x�� */
} CPacketCHAR_STATUS, *PCPacketCHAR_STATUS;

/* Copyright(C)URARA-works 2008 */
