/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_RENEWOBJECTDATA.h							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�I�u�W�F�N�g�z�u�f�[�^�ʒm) ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoMapObjectData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_MAP_RENEWOBJECTDATA : public CPacketBase
{
public:
			CPacketADMIN_MAP_RENEWOBJECTDATA();			/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_MAP_RENEWOBJECTDATA();		/* �f�X�g���N�^ */

	void	Make	(DWORD dwMapID, CInfoMapObjectData *pInfo);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwMapID;							/* �}�b�vID */
	CInfoMapObjectData	*m_pInfo;				/* �C�x���g��� */
} CPacketADMIN_MAP_RENEWOBJECTDATA, *PCPacketADMIN_MAP_RENEWOBJECTDATA;

/* Copyright(C)URARA-works 2008 */
