/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_MAPOBJECT.h										 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�I�u�W�F�N�g���ʒm) ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/01													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMapObject;
class CInfoMapObject;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_MAPOBJECT : public CPacketBase
{
public:
			CPacketMAP_MAPOBJECT();					/* �R���X�g���N�^ */
	virtual ~CPacketMAP_MAPOBJECT();				/* �f�X�g���N�^ */

	void	Make	(CLibInfoMapObject *pLibInfo);			/* �p�P�b�g���쐬 */
	void	Make	(CInfoMapObject *pInfo);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	CLibInfoMapObject	*m_pLibInfoMapObject;		/* �}�b�v�I�u�W�F�N�g��� */
} CPacketMAP_MAPOBJECT, *PCPacketMAP_MAPOBJECT;

/* Copyright(C)URARA-works 2008 */
