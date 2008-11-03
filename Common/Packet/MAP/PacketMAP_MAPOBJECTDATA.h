/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_MAPOBJECTDATA.h									 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�I�u�W�F�N�g�z�u�f�[�^�ʒm) ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMapObjectData;
class CInfoMapObjectData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_MAPOBJECTDATA : public CPacketBase
{
public:
			CPacketMAP_MAPOBJECTDATA();							/* �R���X�g���N�^ */
	virtual ~CPacketMAP_MAPOBJECTDATA();						/* �f�X�g���N�^ */

	void	Make	(DWORD dwMapID, CLibInfoMapObjectData *pLibInfo);	/* �p�P�b�g���쐬 */
	void	Make	(DWORD dwMapID, CInfoMapObjectData *pInfo);			/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	DWORD					m_dwMapID;			/* �}�b�vID */
	CLibInfoMapObjectData	*m_pLibInfo;		/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^ */
} CPacketMAP_MAPOBJECTDATA, *PCPacketMAP_MAPOBJECTDATA;

/* Copyright(C)URARA-works 2008 */
