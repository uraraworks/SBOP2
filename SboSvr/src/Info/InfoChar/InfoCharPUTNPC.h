/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharPUTNPC.h											 */
/* ���e			:�L�������(NPC����)�T�[�o�[�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/12													 */
/* ========================================================================= */

#pragma once

#include "InfoCharSvr.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoCharPUTNPC : public CInfoCharSvr
{
public:
			CInfoCharPUTNPC();							/* �R���X�g���N�^ */
	virtual ~CInfoCharPUTNPC();							/* �f�X�g���N�^ */

	void SetMoveState	(int nMoveState);						/* �ړ���Ԃ�ύX */
	BOOL TimerProc		(DWORD dwTime);							/* ���ԏ��� */

	void IncPutCount	(void);									/* ����NPC���𑝉� */
	void DecPutCount	(void);									/* ����NPC�������� */


public:
	DWORD	m_dwLastTimePut;			/* �O��̔������� */
	int		m_nPutCount;				/* �������̐� */
} CInfoCharPUTNPC, *PCInfoCharPUTNPC;

/* Copyright(C)URARA-works 2008 */
