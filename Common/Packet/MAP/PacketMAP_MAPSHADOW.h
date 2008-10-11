/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_MAPSHADOW.h										 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�e���ʒm) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/05													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMapShadow;
class CInfoMapShadow;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_MAPSHADOW : public CPacketBase
{
public:
			CPacketMAP_MAPSHADOW();					/* �R���X�g���N�^ */
	virtual ~CPacketMAP_MAPSHADOW();				/* �f�X�g���N�^ */

	void	Make	(CLibInfoMapShadow *pLibInfo);			/* �p�P�b�g���쐬 */
	void	Make	(CInfoMapShadow *pInfo);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	CLibInfoMapShadow	*m_pLibInfoMapShadow;		/* �}�b�v�e��� */
} CPacketMAP_MAPSHADOW, *PCPacketMAP_MAPSHADOW;

/* Copyright(C)URARA-works 2007 */
