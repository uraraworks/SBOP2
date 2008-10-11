/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_RENEWMAPSIZE.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�T�C�Y�X�V) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/01													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_RENEWMAPSIZE : public CPacketBase
{
public:
			CPacketADMIN_MAP_RENEWMAPSIZE();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_MAP_RENEWMAPSIZE();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwMapID, int nDirection, int nSize);		/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwMapID;				/* �}�b�vID */
	int		m_nDirection,			/* ����������� */
			m_nSize;				/* �������鋗�� */
} CPacketADMIN_MAP_RENEWMAPSIZE, *PCPacketADMIN_MAP_RENEWMAPSIZE;

/* Copyright(C)URARA-works 2007 */
