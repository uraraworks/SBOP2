/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_REQ_MAPINFO.h									 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v���v��) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_REQ_MAPINFO : public CPacketBase
{
public:
			CPacketMAP_REQ_MAPINFO();					/* �R���X�g���N�^ */
	virtual ~CPacketMAP_REQ_MAPINFO();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwMapID);							/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD m_dwMapID;						/* �}�b�vID */
} CPacketMAP_REQ_MAPINFO, *PCPacketMAP_REQ_MAPINFO;

/* Copyright(C)URARA-works 2007 */
