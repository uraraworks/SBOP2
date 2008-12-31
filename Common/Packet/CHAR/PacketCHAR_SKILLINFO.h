/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_SKILLINFO.h										 */
/* ���e			:�R�}���h(�L�����n:�X�L�����ʒm) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_SKILLINFO : public CPacketBase
{
public:
			CPacketCHAR_SKILLINFO();					/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_SKILLINFO();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, PARRAYDWORD paSrc);		/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD		m_dwCharID;					/* �L����ID */
	ARRAYDWORD	m_adwSkillID;				/* �X�L��ID */
} CPacketCHAR_SKILLINFO, *PCPacketCHAR_SKILLINFO;

/* Copyright(C)URARA-works 2008 */
