/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_TEXTEFFECT.h									 */
/* ���e			:�R�}���h(�L�����n:�����G�t�F�N�g�ʒm) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/13													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_TEXTEFFECT : public CPacketBase
{
public:
			CPacketCHAR_TEXTEFFECT();							/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_TEXTEFFECT();							/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, DWORD dwData, int nColor = 0, int nSize = 0);		/* �p�P�b�g���쐬 */
	void	Make	(DWORD dwCharID, LPCSTR pszData, int nColor = 0, int nSize = 0);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);									/* �p�P�b�g��ݒ� */


public:
	int			m_nColor,				/* �F */
				m_nSize;				/* �T�C�Y */
	DWORD		m_dwCharID,				/* �L����ID */
				m_dwData;				/* ���l�f�[�^ */
	CmyString	m_strData;				/* ������f�[�^ */
} CPacketCHAR_TEXTEFFECT, *PCPacketCHAR_TEXTEFFECT;

/* Copyright(C)URARA-works 2008 */
