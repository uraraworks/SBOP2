/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_MOVEPOS.h										 */
/* ���e			:�R�}���h(�L�����n:�ړ��ʒm) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/14													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_MOVEPOS : public CPacketBase
{
public:
			CPacketCHAR_MOVEPOS();							/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_MOVEPOS();							/* �f�X�g���N�^ */

	void	Make	(DWORD dwMapID, DWORD dwCharID, int nDirection, int x, int y, BOOL bUpdate);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	BOOL	m_bUpdate;					/* ���L�����̍��W���X�V���� */
	DWORD	m_dwMapID,					/* �}�b�vID */
			m_dwCharID;					/* �L����ID */
	int		m_nDirection;				/* ���� */
	POINT	m_pos;						/* ���W */
} CPacketCHAR_MOVEPOS, *PCPacketCHAR_MOVEPOS;

/* Copyright(C)URARA-works 2007 */
