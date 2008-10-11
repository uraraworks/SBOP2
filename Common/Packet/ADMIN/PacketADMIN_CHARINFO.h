/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHARINFO.h										 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�L�������ʒm) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/03/18													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHARINFO : public CPacketBase
{
public:
			CPacketADMIN_CHARINFO();					/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_CHARINFO();					/* �f�X�g���N�^ */

	void	Make	(CInfoCharBase *pInfo, BOOL bChgScreenPos);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	BOOL			m_bChgScreenPos;			/* ��ʈʒu�ύX */
	CInfoCharBase	*m_pInfoChar;				/* �L������� */
} CPacketADMIN_CHARINFO, *PCPacketADMIN_CHARINFO;

/* Copyright(C)URARA-works 2007 */
