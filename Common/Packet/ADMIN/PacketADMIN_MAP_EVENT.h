/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_EVENT.h									 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�C�x���g���ʒm) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/25													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoMapEventBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_EVENT : public CPacketBase
{
public:
			CPacketADMIN_MAP_EVENT();					/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_MAP_EVENT();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwMapID, CInfoMapEventBase *pInfo);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwMapID;							/* �}�b�vID */
	CInfoMapEventBase	*m_pInfo;				/* �C�x���g��� */
} CPacketADMIN_MAP_EVENT, *PCPacketADMIN_MAP_EVENT;

/* Copyright(C)URARA-works 2008 */
