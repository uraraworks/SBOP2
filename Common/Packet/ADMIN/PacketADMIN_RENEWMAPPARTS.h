/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_RENEWMAPPARTS.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�p�[�c�X�V) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/29													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoMapParts;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_RENEWMAPPARTS : public CPacketBase
{
public:
			CPacketADMIN_RENEWMAPPARTS();			/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_RENEWMAPPARTS();			/* �f�X�g���N�^ */

	void	Make	(CInfoMapParts *pInfo);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	CInfoMapParts	*m_pInfoMapParts;		/* �}�b�v�p�[�c��� */
} CPacketADMIN_RENEWMAPPARTS, *PCPacketADMIN_RENEWMAPPARTS;

/* Copyright(C)URARA-works 2007 */
