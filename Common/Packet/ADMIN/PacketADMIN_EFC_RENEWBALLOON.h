/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_EFC_RENEWBALLOON.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:���o�����̍X�V) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/30													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoEfcBalloon;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_EFC_RENEWBALLOON : public CPacketBase
{
public:
			CPacketADMIN_EFC_RENEWBALLOON();					/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_EFC_RENEWBALLOON();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwListID, CLibInfoEfcBalloon *pSrc);			/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	DWORD				m_dwListID;			/* ���ID */
	CLibInfoEfcBalloon	*m_pInfo;			/* ���o����� */
} CPacketADMIN_EFC_RENEWBALLOON, *PCPacketADMIN_EFC_RENEWBALLOON;

/* Copyright(C)URARA-works 2007 */
