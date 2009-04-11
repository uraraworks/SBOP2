/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_DISABLE_RENEWINFO.h							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:���ۏ��̍X�V) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/04/11													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoDisable;
class CLibInfoDisable;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_DISABLE_RENEWINFO : public CPacketBase
{
public:
			CPacketADMIN_DISABLE_RENEWINFO();						/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_DISABLE_RENEWINFO();						/* �f�X�g���N�^ */

	void	Make	(CLibInfoDisable *pLibInfoDisable);		/* �p�P�b�g���쐬 */
	void	Make	(CInfoDisable *pInfoDisable);			/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);						/* �p�P�b�g��ݒ� */


public:
	CLibInfoDisable		*m_pLibInfoDisable;				/* ���ۏ�� */
} CPacketADMIN_DISABLE_RENEWINFO, *PCPacketADMIN_DISABLE_RENEWINFO;

/* Copyright(C)URARA-works 2009 */
