/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_PARA2.h										 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�ėp[�p�����[�^2��]) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/25													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_PARA2 : public CPacketBase
{
public:
			CPacketADMIN_PARA2();							/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_PARA2();							/* �f�X�g���N�^ */

	void	Make	(BYTE byCmdSub, DWORD dwPara1=0, DWORD dwPara=0);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwPara1,						/* �p�����[�^1 */
			m_dwPara2;						/* �p�����[�^2 */
} CPacketADMIN_PARA2, *PCPacketADMIN_PARA2;

/* Copyright(C)URARA-works 2008 */
