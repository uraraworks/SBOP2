/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_SETPARTS.h									 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�p�[�c�z�u) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/30													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_SETPARTS : public CPacketBase
{
public:
			CPacketADMIN_MAP_SETPARTS();					/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_MAP_SETPARTS();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwMapID, int x, int y, DWORD dwPartsID, BOOL bPile);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	BOOL	m_bPile;				/* TRUE:�d�ˍ��킹�p�}�b�v */
	POINT	m_ptPos;				/* ���W */
	DWORD	m_dwMapID,				/* �}�b�vID */
			m_dwPartsID;			/* �p�[�cID */
} CPacketADMIN_MAP_SETPARTS, *PCPacketADMIN_MAP_SETPARTS;

/* Copyright(C)URARA-works 2007 */
