/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_RES_MAPINFO.h									 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v��񉞓�) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoMapBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_RES_MAPINFO : public CPacketBase
{
public:
			CPacketMAP_RES_MAPINFO();					/* �R���X�g���N�^ */
	virtual ~CPacketMAP_RES_MAPINFO();					/* �f�X�g���N�^ */

	void	Make	(CInfoMapBase *pInfo);						/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CInfoMapBase	*m_pInfo;					/* �}�b�v��� */
} CPacketMAP_RES_MAPINFO, *PCPacketMAP_RES_MAPINFO;

/* Copyright(C)URARA-works 2007 */
