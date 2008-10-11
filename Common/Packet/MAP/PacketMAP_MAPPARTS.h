/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_MAPPARTS.h										 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�p�[�c���ʒm) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/30													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMapParts;
class CInfoMapParts;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_MAPPARTS : public CPacketBase
{
public:
			CPacketMAP_MAPPARTS();					/* �R���X�g���N�^ */
	virtual ~CPacketMAP_MAPPARTS();					/* �f�X�g���N�^ */

	void	Make	(CLibInfoMapParts *pLibInfo);			/* �p�P�b�g���쐬 */
	void	Make	(CInfoMapParts *pInfo);					/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	CLibInfoMapParts	*m_pLibInfoMapParts;		/* �}�b�v�p�[�c��� */
} CPacketMAP_MAPPARTS, *PCPacketMAP_MAPPARTS;

/* Copyright(C)URARA-works 2007 */
