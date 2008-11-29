/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_SETMAPNAME.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v���ύX) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/24													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_SETMAPNAME : public CPacketBase
{
public:
			CPacketADMIN_MAP_SETMAPNAME();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_MAP_SETMAPNAME();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwMapID, DWORD dwBGMID, DWORD dwWeatherType, BOOL bEnableBattle, BOOL bRecovery, BYTE byLevel, LPCSTR pszMapName);			/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD		m_dwMapID,					/* �}�b�vID */
				m_dwBGMID,					/* BGMID */
				m_dwWeatherType;			/* �V�C��� */
	BOOL		m_bEnableBattle,			/* �퓬���� */
				m_bRecovery;				/* �C���񕜂��� */
	BYTE		m_byLevel;					/* ���邳���x�� */
	CmyString	m_strMapName;				/* �}�b�v�� */
} CPacketADMIN_MAP_SETMAPNAME, *PCPacketADMIN_MAP_SETMAPNAME;

/* Copyright(C)URARA-works 2008 */
