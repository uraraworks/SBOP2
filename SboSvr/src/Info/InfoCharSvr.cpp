/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharSvr.cpp											 */
/* ���e			:�L�������T�[�o�[�N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/14													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMotion.h"
#include "InfoCharSvr.h"


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::CInfoCharSvr										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/01/14														 */
/* ========================================================================= */

CInfoCharSvr::CInfoCharSvr()
{
	m_nReserveChgEfect		= 0;
	m_nReserveChgMoveState	= 0;
	m_nMoveCount			= 0;
	m_bChgPos				= FALSE;
	m_bChgMap				= FALSE;
	m_bChgUpdatePos			= FALSE;
	m_bChgSpeak				= FALSE;
	m_bChgInfo				= FALSE;
	m_bChgMoveState			= FALSE;
	m_bChgProcState			= FALSE;
	m_bChgScreenPos			= FALSE;
	m_bChgGrp				= FALSE;
	m_bChgEfcBalloon		= FALSE;
	m_bChgMotion			= FALSE;
	m_bChgStatus			= FALSE;
	m_bChgFishingHit		= FALSE;
	m_bChgMoveCount			= FALSE;
	m_bChgPutNpc			= FALSE;
	m_bWaitCheckMapEvent	= FALSE;
	m_bDropItem				= FALSE;
	m_bAtack				= FALSE;
	m_bRenewTargetPos		= FALSE;
	m_bProcMoveMapIn		= FALSE;
	m_bProcMoveMapOut		= FALSE;
	m_bProcMoveMarkPos		= FALSE;
	m_bProcSwoon			= FALSE;
	m_bProcInvincible		= FALSE;
	m_bStateFadeInOut		= FALSE;
	m_bStatusInvincible		= FALSE;
	m_pInfoMap				= NULL;
	m_dwChgWait				= 0;
	m_dwLastTimeChg			= 0;
	m_dwEfcBalloonID		= 0;
	m_dwMotionID			= 0;
	m_dwMoveCount			= 0;

	m_pLibInfoCharSvr		= NULL;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::~CInfoCharSvr									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/01/14														 */
/* ========================================================================= */

CInfoCharSvr::~CInfoCharSvr()
{
	DeleteAllProcInfo ();
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::SetSpeak											 */
/* ���e		:�������e���X�V													 */
/* ���t		:2007/02/24														 */
/* ========================================================================= */

void CInfoCharSvr::SetSpeak(LPCSTR pszSpeak)
{
	CInfoCharBase::SetSpeak (pszSpeak);

	if (m_strSpeak.IsEmpty () == FALSE) {
		m_bChgSpeak = TRUE;
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::SetMoveState										 */
/* ���e		:�ړ���Ԃ�ύX													 */
/* ���t		:2007/04/20														 */
/* ========================================================================= */

void CInfoCharSvr::SetMoveState(int nMoveState)
{
	if (m_nMoveState == CHARMOVESTATE_DELETE) {
		/* �폜�͎������Ȃ��悤�ɂ��� */
		return;
	}
	if (nMoveState == m_nMoveState) {
		return;
	}

	m_bChgMoveState = TRUE;
	switch (nMoveState) {
	case CHARMOVESTATE_BATTLEATACK_WAIT:		/* �퓬�U����̑҂����� */
		m_dwChgWait = 1000;
		m_dwLastTimeChg = timeGetTime ();
		m_bChgMoveState = FALSE;
		break;
	case CHARMOVESTATE_BATTLE:
		/* ���ߍU������ */
		m_bChargeAtack = FALSE;
		break;
	case CHARMOVESTATE_SWOON:					/* �C�� */
		/* ���ߍU������ */
		m_bChargeAtack = FALSE;
		/* �s������S�č폜 */
		DeleteAllProcInfo ();
		SetMotion (-1);
		SetProcState (CHARPROCSTATEID_NORMAL);
		break;
	}

	CInfoCharBase::SetMoveState (nMoveState);
}


/* ========================================================================= */
/* �֐���	:CInfoCharBase::SetProcState									 */
/* ���e		:�s����Ԃ�ύX													 */
/* ���t		:2008/06/11														 */
/* ========================================================================= */

void CInfoCharSvr::SetProcState(int nProcState)
{
	m_bChgProcState = TRUE;

	CInfoCharBase::SetProcState (nProcState);
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::SetMap											 */
/* ���e		:�}�b�v����ݒ�												 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

void CInfoCharSvr::SetMap(CInfoMapBase *pInfoMap)
{
	m_pInfoMap = pInfoMap;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::SetLibInfoChar									 */
/* ���e		:�L������񃉃C�u������ݒ�										 */
/* ���t		:2009/01/17														 */
/* ========================================================================= */

void CInfoCharSvr::SetLibInfoChar(CLibInfoCharSvr *pLibInfoChar)
{
	m_pLibInfoCharSvr = pLibInfoChar;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::SetEfcBalloon									 */
/* ���e		:���o����ݒ�													 */
/* ���t		:2008/01/03														 */
/* ========================================================================= */

void CInfoCharSvr::SetEfcBalloon(DWORD dwEfcBalloonID)
{
	m_dwEfcBalloonID = dwEfcBalloonID;
	m_bChgEfcBalloon = TRUE;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::SetMotion										 */
/* ���e		:���[�V������ݒ�												 */
/* ���t		:2008/01/03														 */
/* ========================================================================= */

void CInfoCharSvr::SetMotion(DWORD dwMotionID)
{
	m_dwMotionID = dwMotionID;
	m_bChgMotion = TRUE;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::IsEnableBattle									 */
/* ���e		:�퓬��ԂɑJ�ڂł��邩����										 */
/* ���t		:2008/07/08														 */
/* ========================================================================= */

BOOL CInfoCharSvr::IsEnableBattle(void)
{
	BOOL bRet;

	bRet = FALSE;

	/* ���Ă����Ă���H */
	if (m_dwFrontCharID) {
		goto Exit;
	}
	/* �C�⒆�H */
	if (m_nMoveState == CHARMOVESTATE_SWOON) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::IsEnableMove										 */
/* ���e		:�ړ��ł����Ԃ�����											 */
/* ���t		:2008/07/28														 */
/* �߂�l	:TRUE:�ړ���													 */
/* ========================================================================= */

BOOL CInfoCharSvr::IsEnableMove(void)
{
	BOOL bRet;

	bRet = CInfoCharBase::IsEnableMove ();
	if (bRet == FALSE) {
		goto Exit;
	}
	if (m_bStateFadeInOut) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::CopyAll											 */
/* ���e		:�h����̏����S�ăR�s�[										 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

void CInfoCharSvr::CopyAll(CInfoCharSvr *pSrc)
{
	CInfoCharBase::Copy (pSrc);

	m_abyMark.Copy (&pSrc->m_abyMark);
	m_bNPC				= pSrc->m_bNPC;
	m_dwSessionID		= pSrc->m_dwSessionID;
	m_dwAccountID		= pSrc->m_dwAccountID;
	m_dwLastTimeSpeak	= pSrc->m_dwLastTimeSpeak;
	m_dwTailCharID		= pSrc->m_dwTailCharID;
	m_dwFrontCharID		= pSrc->m_dwFrontCharID;
	m_nReserveChgEfect	= pSrc->m_nReserveChgEfect;
	m_bChgPos			= pSrc->m_bChgPos;
	m_bChgUpdatePos		= pSrc->m_bChgUpdatePos;
	m_bChgSpeak			= pSrc->m_bChgSpeak;
	m_bChgInfo			= pSrc->m_bChgInfo;
	m_bChgMoveState		= pSrc->m_bChgMoveState;
	m_bChgScreenPos		= pSrc->m_bChgScreenPos;
	m_bChgPosRenew		= pSrc->m_bChgPosRenew;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::Copy												 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

void CInfoCharSvr::Copy(CInfoCharBase *pSrc)
{
	PCInfoCharSvr pSrcTmp;

	pSrcTmp = (PCInfoCharSvr)pSrc;
	CInfoCharBase::Copy (pSrc);

	m_dwMoveCount	= pSrcTmp->m_dwMoveCount;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::ProcAtack										 */
/* ���e		:����(�U��������)												 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

void CInfoCharSvr::ProcAtack(void)
{
	/* ���ߍU������ */
	m_bChargeAtack = FALSE;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::ProcHit											 */
/* ���e		:����(�U�����󂯂���)											 */
/* ���t		:2007/09/17														 */
/* �߂�l	:TRUE:�ȍ~�̏����𑱍s����										 */
/* ========================================================================= */

BOOL CInfoCharSvr::ProcHit(CInfoCharSvr *pInfoChar)
{
	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::TimerProc										 */
/* ���e		:���ԏ���														 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

BOOL CInfoCharSvr::TimerProc(DWORD dwTime)
{
	BOOL bRet;

	bRet = CInfoCharBase::TimerProc (dwTime);

	if (m_nMoveState == CHARMOVESTATE_BATTLEATACK_WAIT) {
		if (m_dwChgWait) {
			if (dwTime - m_dwLastTimeChg > m_dwChgWait) {
				m_dwChgWait = 0;
				m_dwLastTimeChg = 0;
				SetMoveState (CHARMOVESTATE_BATTLEATACK);
			}
		}
	}
	bRet |= TimerProcMOVE (dwTime);

	Proc (dwTime);

	if (m_dwLightTime != 0) {
		if (dwTime > m_dwLightTime) {
			m_nLightLevel = 0;
			m_dwLightTime = 0;
			m_bChgStatus  = TRUE;
		}
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::TimerProcMOVE									 */
/* ���e		:���ԏ���(�ړ�)													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

BOOL CInfoCharSvr::TimerProcMOVE(DWORD dwTime)
{
	BOOL bRet;
	DWORD dwTimeTmp;

	bRet = FALSE;
	if (m_nMoveCount == 0) {
		goto Exit;
	}

	dwTimeTmp = dwTime - m_dwLastTimeMove;
//Todo:�b��
	if (dwTimeTmp < 150) {
		goto Exit;
	}
	m_bChgMoveCount = TRUE;
	m_nMoveCount --;
	m_dwLastTimeMove = dwTime;

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::IsAtackTarget									 */
/* ���e		:�U���ΏۂƂȂ邩����											 */
/* ���t		:2008/07/12														 */
/* �߂�l	:TRUE:�U����													 */
/* ========================================================================= */

BOOL CInfoCharSvr::IsAtackTarget(void)
{
	BOOL bRet;

	bRet = CInfoCharBase::IsAtackTarget ();
	if (bRet == FALSE) {
		goto Exit;
	}
	if (m_bStatusInvincible) {
		goto Exit;
	}
	switch (m_nMoveType) {
	case CHARMOVETYPE_STAND:		/* �ړ����Ȃ� */
	case CHARMOVETYPE_BALL:			/* �{�[�� */
	case CHARMOVETYPE_SCORE:		/* ���_ */
	case CHARMOVETYPE_PUTNPC:		/* NPC���� */
		bRet = FALSE;
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::GetHitEffectID									 */
/* ���e		:�q�b�g���ɑ���ɕ\������G�t�F�N�gID���擾						 */
/* ���t		:2009/01/12														 */
/* ========================================================================= */

DWORD CInfoCharSvr::GetHitEffectID(void)
{
	return 0;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::GetDamage										 */
/* ���e		:�_���[�W�l���擾												 */
/* ���t		:2009/01/12														 */
/* ========================================================================= */

DWORD CInfoCharSvr::GetDamage(void)
{
	return 0;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::Proc												 */
/* ���e		:�s������														 */
/* ���t		:2008/01/01														 */
/* ========================================================================= */

void CInfoCharSvr::Proc(DWORD dwTime)
{
	int i, nCount;
	BOOL bResult;
	DWORD dwTimeTmp;
	PCHARPROCINFO pInfo;

	nCount = m_apProcInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_apProcInfo[i];
		dwTimeTmp = dwTime - pInfo->dwProcSetTime;
		if (dwTimeTmp < pInfo->dwProcStartTime) {
			continue;
		}

		switch (pInfo->dwProcID) {
		case CHARPROCID_FISHING:		/* �ނ� */
			bResult = ProcFISHING (pInfo->dwPara);
			break;
		case CHARPROCID_FISHING_HIT:	/* �ނ�(�q�b�g) */
			bResult = ProcFISHING_HIT (pInfo->dwPara);
			break;
		case CHARPROCID_MAPMOVEIN:		/* �}�b�v���ړ� */
			bResult = ProcMAPMOVEIN (pInfo->dwPara);
			break;
		case CHARPROCID_MAPMOVEOUT:		/* �}�b�v�O�ړ� */
			bResult = ProcMAPMOVEOUT (pInfo->dwPara);
			break;
		case CHARPROCID_SWOON:			/* �C�� */
			bResult = ProcSWOON (pInfo->dwPara);
			break;
		case CHARPROCID_INVINCIBLE:		/* ���G */
			bResult = ProcINVINCIBLE (pInfo->dwPara);
			break;
		}
		if (bResult) {
			DeleteProcInfo (i);
			break;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::ProcFISHING										 */
/* ���e		:�s������(�ނ�)													 */
/* ���t		:2008/01/01														 */
/* ========================================================================= */

BOOL CInfoCharSvr::ProcFISHING(DWORD dwPara)
{
	int nState;

	nState = CHARMOVESTATE_STAND;
	if (IsStateBattle ()) {
		nState = CHARMOVESTATE_BATTLE;
	}

	/* �ނ�Ȃ����� */
	if ((genrand () % 100) < 60) {
//Todo:
		SetEfcBalloon (8);
		SetProcState (CHARPROCSTATEID_NORMAL);
		SetMoveState (nState);
	} else {
		SetMotion (CHARMOTIONLISTID_FISHING_HIT_UP);
		AddProcInfo (CHARPROCID_FISHING_HIT, 5000, 0);
	}

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::ProcFISHING_HIT									 */
/* ���e		:�s������(�ނ�(�q�b�g))											 */
/* ���t		:2008/01/02														 */
/* ========================================================================= */

BOOL CInfoCharSvr::ProcFISHING_HIT(DWORD dwPara)
{
	int nState;

	nState = CHARMOVESTATE_STAND;
	if (IsStateBattle ()) {
		nState = CHARMOVESTATE_BATTLE;
	}

	/* �ނ�Ȃ����� */
	if ((genrand () % 100) < 60) {
//Todo:
		SetEfcBalloon (8);

	} else {
		SetEfcBalloon (2);
		m_bChgFishingHit = TRUE;
	}
	SetMotion (-1);
	SetMoveState (nState);
	SetProcState (CHARPROCSTATEID_NORMAL);
	DeleteProcInfo (CHARPROCID_FISHING);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::ProcMAPMOVEIN									 */
/* ���e		:�s������(�}�b�v���ړ�)											 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

BOOL CInfoCharSvr::ProcMAPMOVEIN(DWORD dwPara)
{
	m_bProcMoveMapIn = TRUE;
	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::ProcMAPMOVEOUT									 */
/* ���e		:�s������(�}�b�v�O�ړ�)											 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

BOOL CInfoCharSvr::ProcMAPMOVEOUT(DWORD dwPara)
{
	m_bProcMoveMapOut = TRUE;
	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::ProcSWOON										 */
/* ���e		:�s������(�C��)													 */
/* ���t		:2008/06/29														 */
/* ========================================================================= */

BOOL CInfoCharSvr::ProcSWOON(DWORD dwPara)
{
	m_bProcSwoon = TRUE;
	m_nReserveChgEfect = -1;

	/* �L�^�ʒu�֖߂�H */
	if (dwPara != 0) {
		m_bProcMoveMarkPos = TRUE;
	}

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::ProcINVINCIBLE									 */
/* ���e		:�s������(���G)													 */
/* ���t		:2008/06/29														 */
/* ========================================================================= */

BOOL CInfoCharSvr::ProcINVINCIBLE(DWORD dwPara)
{
	m_bProcInvincible = TRUE;
	/* ���G���� */
	m_bStatusInvincible = FALSE;
	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::DeleteProcInfo									 */
/* ���e		:�s�������폜													 */
/* ���t		:2008/01/01														 */
/* ========================================================================= */

void CInfoCharSvr::DeleteProcInfo(int nNo)
{
	PCHARPROCINFO pInfo;

	if (nNo >= m_apProcInfo.GetSize ()) {
		return;
	}

	pInfo = m_apProcInfo[nNo];
	SAFE_DELETE (pInfo);
	m_apProcInfo.RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::DeleteProcInfo									 */
/* ���e		:�s�������폜													 */
/* ���t		:2008/01/01														 */
/* ========================================================================= */

void CInfoCharSvr::DeleteProcInfo(DWORD dwProcID)
{
	int i, nCount;
	PCHARPROCINFO pInfo;

	nCount = m_apProcInfo.GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_apProcInfo[i];
		if (pInfo->dwProcID != dwProcID) {
			continue;
		}
		DeleteProcInfo (i);
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::DeleteAllProcInfo								 */
/* ���e		:�s������S�č폜												 */
/* ���t		:2008/01/01														 */
/* ========================================================================= */

void CInfoCharSvr::DeleteAllProcInfo(void)
{
	int i, nCount;

	nCount = m_apProcInfo.GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		DeleteProcInfo (i);
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::AddProcInfo										 */
/* ���e		:�s������ǉ�													 */
/* ���t		:2008/01/01														 */
/* ========================================================================= */

void CInfoCharSvr::AddProcInfo(
	DWORD dwProcID,			/* [in] ����ID */
	DWORD dwStartTime,		/* [in] �J�n���� */
	DWORD dwPara)			/* [in] �p�����[�^ */
{
	PCHARPROCINFO pInfo;

	pInfo = new CHARPROCINFO;
	pInfo->dwProcID			= dwProcID;			/* �s��ID */
	pInfo->dwProcSetTime	= timeGetTime ();	/* �����ݒ莞�� */
	pInfo->dwProcStartTime	= dwStartTime;		/* �����J�n���� */
	pInfo->dwPara			= dwPara;			/* �p�����[�^ */

	m_apProcInfo.Add (pInfo);
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::IncPutCount										 */
/* ���e		:����NPC���𑝉�												 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */
void CInfoCharSvr::IncPutCount(void)
{
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::DecPutCount										 */
/* ���e		:����NPC��������												 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */
void CInfoCharSvr::DecPutCount(void)
{
}

/* Copyright(C)URARA-works 2007 */
