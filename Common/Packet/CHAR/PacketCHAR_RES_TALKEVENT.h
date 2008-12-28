/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_RES_TALKEVENT.h									 */
/* ���e			:�R�}���h(�L�����n:��b�C�x���g��񉞓�) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/27													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoTalkEvent;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_RES_TALKEVENT : public CPacketBase
{
public:
			CPacketCHAR_RES_TALKEVENT();						/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_RES_TALKEVENT();						/* �f�X�g���N�^ */

	void	Make	(CInfoTalkEvent *pInfo, DWORD dwParam);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	DWORD			m_dwParam;				/* �p�����[�^ */
	CInfoTalkEvent	*m_pInfo;				/* ��b�C�x���g��� */
} CPacketCHAR_RES_TALKEVENT, *PCPacketCHAR_RES_TALKEVENT;

/* Copyright(C)URARA-works 2008 */
