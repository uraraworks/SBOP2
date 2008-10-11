/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharPUTNPC.cpp											 */
/* ���e			:�L�������(NPC����)�T�[�o�[�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoCharPUTNPC.h"


/* ========================================================================= */
/* �֐���	:CInfoCharPUTNPC::CInfoCharPUTNPC								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

CInfoCharPUTNPC::CInfoCharPUTNPC()
{
	m_nPutMoveType	= CHARMOVETYPE_BATTLE1;

	m_dwLastTimePut	= 0;
	m_nPutCount		= 0;
}


/* ========================================================================= */
/* �֐���	:CInfoCharPUTNPC::~CInfoCharPUTNPC								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

CInfoCharPUTNPC::~CInfoCharPUTNPC()
{
}


/* ========================================================================= */
/* �֐���	:CInfoCharPUTNPC::SetMoveState									 */
/* ���e		:�ړ���Ԃ�ύX													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CInfoCharPUTNPC::SetMoveState(int nMoveState)
{
	CInfoCharSvr::SetMoveState (nMoveState);
}


/* ========================================================================= */
/* �֐���	:CInfoCharPUTNPC::TimerProc										 */
/* ���e		:���ԏ���														 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

BOOL CInfoCharPUTNPC::TimerProc(DWORD dwTime)
{
	DWORD dwTmp;

	dwTmp = dwTime - m_dwLastTimePut;
	if (dwTmp >= m_dwPutCycle * 1000) {
		if (m_nPutCount < m_nMaxPutCount) {
			/* �����H */
			if ((genrand () % 100) > 100 - m_nPutAverage) {
				/* �����\�� */
				m_bChgPutNpc = TRUE;
			}
		}
		m_dwLastTimePut	= dwTime;
	}

	return CInfoCharSvr::TimerProc (dwTime);
}


/* ========================================================================= */
/* �֐���	:CInfoCharPUTNPC::IncPutCount									 */
/* ���e		:����NPC���𑝉�												 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */
void CInfoCharPUTNPC::IncPutCount(void)
{
	m_nPutCount ++;
}


/* ========================================================================= */
/* �֐���	:CInfoCharPUTNPC::DecPutCount									 */
/* ���e		:����NPC��������												 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */
void CInfoCharPUTNPC::DecPutCount(void)
{
	m_nPutCount --;
}

/* Copyright(C)URARA-works 2008 */
