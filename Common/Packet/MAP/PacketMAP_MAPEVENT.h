/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_MAPEVENT.h										 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�C�x���g���ʒm) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/25													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMapEvent;
class CInfoMapEventBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_MAPEVENT : public CPacketBase
{
public:
			CPacketMAP_MAPEVENT();					/* �R���X�g���N�^ */
	virtual ~CPacketMAP_MAPEVENT();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwMapID, CLibInfoMapEvent *pLibInfo);	/* �p�P�b�g���쐬 */
	void	Make	(DWORD dwMapID, CInfoMapEventBase *pInfo);		/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwMapID;						/* �}�b�vID */
	CLibInfoMapEvent	*m_pLibInfo;		/* �}�b�v�C�x���g��� */
} CPacketMAP_MAPEVENT, *PCPacketMAP_MAPEVENT;

/* Copyright(C)URARA-works 2008 */
