/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketEFFECT_REQ_BALLOONINFO.h								 */
/* ���e			:�R�}���h(�G�t�F�N�g�n:���o�����v��) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/24													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketEFFECT_REQ_BALLOONINFO : public CPacketBase
{
public:
			CPacketEFFECT_REQ_BALLOONINFO();				/* �R���X�g���N�^ */
	virtual ~CPacketEFFECT_REQ_BALLOONINFO();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwBalloonID);						/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD m_dwBalloonID;					/* ���o��ID */
} CPacketEFFECT_REQ_BALLOONINFO, *PCPacketEFFECT_REQ_BALLOONINFO;

/* Copyright(C)URARA-works 2007 */
