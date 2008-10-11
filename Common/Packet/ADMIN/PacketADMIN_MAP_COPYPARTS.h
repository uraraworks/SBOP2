/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_COPYPARTS.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�p�[�c�R�s�[) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/14													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_COPYPARTS : public CPacketBase
{
public:
			CPacketADMIN_MAP_COPYPARTS();					/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_MAP_COPYPARTS();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwSrcPartsID, POINT ptViewPos);			/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwSrcPartsID;			/* �R�s�[���p�[�cID */
	POINT	m_ptViewPos;			/* �\���ʒu */
} CPacketADMIN_MAP_COPYPARTS, *PCPacketADMIN_MAP_COPYPARTS;

/* Copyright(C)URARA-works 2007 */
