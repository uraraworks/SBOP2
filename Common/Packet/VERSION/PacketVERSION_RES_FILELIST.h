/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_RES_FILELIST.h								 */
/* ���e			:�R�}���h(�o�[�W�����n:�t�@�C�����X�g����) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/03/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoFileList;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketVERSION_RES_FILELIST : public CPacketBase
{
public:
			CPacketVERSION_RES_FILELIST();				/* �R���X�g���N�^ */
	virtual ~CPacketVERSION_RES_FILELIST();				/* �f�X�g���N�^ */

	void	Make	(CInfoFileList *pSrc);						/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CInfoFileList	*m_pInfo;					/* �t�@�C�����X�g��� */
} CPacketVERSION_RES_FILELIST, *PCPacketVERSION_RES_FILELIST;

/* Copyright(C)URARA-works 2006 */
