/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_RENEW_TALKEVENT.h							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:��b�C�x���g���X�V) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/27													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoTalkEvent;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_RENEW_TALKEVENT : public CPacketBase
{
public:
			CPacketADMIN_CHAR_RENEW_TALKEVENT();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_CHAR_RENEW_TALKEVENT();				/* �f�X�g���N�^ */

	void	Make	(CInfoTalkEvent *pInfo, DWORD dwParam);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	DWORD			m_dwParam;				/* �p�����[�^ */
	CInfoTalkEvent	*m_pInfo;				/* ��b�C�x���g��� */
} CPacketADMIN_CHAR_RENEW_TALKEVENT, *PCPacketADMIN_CHAR_RENEW_TALKEVENT;

/* Copyright(C)URARA-works 2008 */
