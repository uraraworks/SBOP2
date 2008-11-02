/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_RENEWMAPOBJECT.h							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�I�u�W�F�N�g�X�V) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/01													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoMapObject;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_RENEWMAPOBJECT : public CPacketBase
{
public:
			CPacketADMIN_MAP_RENEWMAPOBJECT();		/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_MAP_RENEWMAPOBJECT();		/* �f�X�g���N�^ */

	void	Make	(CInfoMapObject *pInfo);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	CInfoMapObject	*m_pInfoMapObject;		/* �}�b�v�I�u�W�F�N�g��� */
} CPacketADMIN_MAP_RENEWMAPOBJECT, *PCPacketADMIN_MAP_RENEWMAPOBJECT;

/* Copyright(C)URARA-works 2008 */
