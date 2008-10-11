/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketEFFECT_BALLOONINFO.h									 */
/* ���e			:�R�}���h(�G�t�F�N�g�n:���o����񉞓�) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/24													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoEfcBalloon;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketEFFECT_BALLOONINFO : public CPacketBase
{
public:
			CPacketEFFECT_BALLOONINFO();						/* �R���X�g���N�^ */
	virtual ~CPacketEFFECT_BALLOONINFO();						/* �f�X�g���N�^ */

	void	Make	(DWORD dwListID, CLibInfoEfcBalloon *pLibInfo);		/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	DWORD				m_dwListID;				/* ���ID */
	CLibInfoEfcBalloon	*m_pLibInfo;			/* ���o����� */
} CPacketEFFECT_BALLOONINFO, *PCPacketEFFECT_BALLOONINFO;

/* Copyright(C)URARA-works 2007 */
