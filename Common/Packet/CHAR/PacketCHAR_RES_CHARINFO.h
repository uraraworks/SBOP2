/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_RES_CHARINFO.h									 */
/* ���e			:�R�}���h(�L�����n:�L������񉞓�) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/12/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_RES_CHARINFO : public CPacketBase
{
public:
			CPacketCHAR_RES_CHARINFO();							/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_RES_CHARINFO();						/* �f�X�g���N�^ */

	void	Make	(CInfoCharBase *pInfo, BOOL bChgScreenPos = FALSE);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	BOOL			m_bChgScreenPos;			/* ��ʈʒu�ύX */
	CInfoCharBase	*m_pInfo;					/* �L������� */
} CPacketCHAR_RES_CHARINFO, *PCPacketCHAR_RES_CHARINFO;

/* Copyright(C)URARA-works 2006 */
