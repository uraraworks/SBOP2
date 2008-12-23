/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharCli.cpp											 */
/* ���e			:�L�������N���C�A���g�N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoEffect.h"
#include "LibInfoEffect.h"
#include "LibInfoEfcBalloon.h"
#include "UraraSockTCPSBO.h"
#include "Packet.h"
#include "Command.h"
#include "Img32.h"
#include "MgrGrpData.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "InfoCharCli.h"

/* ========================================================================= */
/* �֐���	:CInfoCharCli::CInfoCharCli										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

CInfoCharCli::CInfoCharCli()
{
	m_bRedraw				= FALSE;
	m_bChgWait				= FALSE;
	m_bSkipMove				= TRUE;
	m_bWaitCheckMapEvent	= FALSE;
	m_bMotionDirection		= TRUE;
	m_bMotionInterrupt		= FALSE;
	m_bModeSleepTimer		= FALSE;
	m_nAnimeBack			= 0;
	m_nViewState			= INFOCHARCLI_VIEWSTATE_NONE;
	m_nFadeLevel			= 0;
	m_nMoveStateReserve		= 0;
	m_nMoveStateBack		= 0;
	m_dwMoveWaitOnce		= 0;
	m_dwLastTimeAnime		= 0;
	m_dwLastTimeViewState	= 0;
	m_dwBalloonID			= 0;
	m_dwBalloonGrpID		= 0;
	m_dwBalloonAnimeID		= 0;
	m_dwLastTimeBalloon		= 0;
	m_dwLastTimeDamage		= 0;
	m_pMgrData				= NULL;
	m_pDibChar				= NULL;
	m_pDibName				= NULL;
	m_pDibSpeak				= NULL;
	m_pInfoEffect			= NULL;
	m_pSock					= NULL;

	m_ptMove.x = m_ptMove.y = 0;

	m_hFont = CreateFont (12, 0, 0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "�l�r �S�V�b�N");
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::~CInfoCharCli									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

CInfoCharCli::~CInfoCharCli()
{
	DeleteAllTextEffect ();
	DeleteAllMovePosQue ();
	if (m_hFont) {
		DeleteObject (m_hFont);
		m_hFont = NULL;
	}
	SAFE_DELETE (m_pDibChar);
	SAFE_DELETE (m_pDibName);
	SAFE_DELETE (m_pDibSpeak);
	SAFE_DELETE (m_pInfoEffect);
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CInfoCharCli::Create(CMgrData *pMgrData)
{
	m_pMgrData	= pMgrData;
	m_pMgrSound	= m_pMgrData->GetMgrSound ();
	m_pSock		= m_pMgrData->GetUraraSockTCP ();
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::ChgDirection										 */
/* ���e		:�����ύX														 */
/* ���t		:2006/10/08														 */
/* ========================================================================= */

void CInfoCharCli::ChgDirection(int nDirection)
{
	switch (m_nMoveState) {
	case CHARMOVESTATE_STAND:			/* ���� */
	case CHARMOVESTATE_SIT:				/* ���蒆 */
	case CHARMOVESTATE_BATTLE:			/* �퓬�� */
	case CHARMOVESTATE_BATTLE_DEFENSE:	/* �h�䒆 */
		break;
	default:
		goto Exit;
	}
	m_nDirection = nDirection;

Exit:
	return;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::ChgMoveState										 */
/* ���e		:�ړ���ԕύX													 */
/* ���t		:2006/10/08														 */
/* ========================================================================= */

void CInfoCharCli::ChgMoveState(int nMoveState)
{
	BOOL bResult, bChange, bCheck = TRUE;
	int i, nAnime, nCount, nDirection, anInitMovePosX[] = {0, 0, 16, -16, -16, -16, 16, 16}, anInitMovePosY[] = {16, -16, 0, 0, 16, -16, -16, 16};
	PCInfoMotion pInfoMotion;

	if (nMoveState < 0) {
		return;
	}
	if (m_nMoveState == CHARMOVESTATE_DELETEREADY) {
		return;
	}

	bResult = IsStateMove ();
	nAnime	= -1;
	bChange	= (m_nMoveState != nMoveState) ? TRUE : FALSE;

	if (bResult == FALSE) {
		m_ptMove.x = m_ptMove.y = 0;
	}
	pInfoMotion = GetMotionInfo (&nCount);

	if (m_nMoveState == CHARMOVESTATE_SLEEPTIMER) {
		SetBalloonID (0);
	}

	nDirection = m_nDirection;
	if (m_nMoveDirection != -1) {
		nDirection = m_nMoveDirection;
	}

	switch (nMoveState) {
	case CHARMOVESTATE_STAND:		/* ���� */
	case CHARMOVESTATE_SIT:			/* ���蒆 */
	case CHARMOVESTATE_WALKANIME:	/* ������ */
		if (bResult) {
			AddMovePosQue (nMoveState, -1, -1, -1);
			return;
		}
		m_nAnimeBack = m_nAnime;
		nAnime = 0;
		m_nMoveDirection = -1;
		RenewBlockMapArea (0, 0, -1);
		/* ���ߍU������ */
		m_bChargeAtack = FALSE;
		bCheck = FALSE;
		break;
	case CHARMOVESTATE_MOVE:		/* �ړ��� */
	case CHARMOVESTATE_BATTLEMOVE:	/* �퓬�ړ��� */
		if (bChange) {
			nAnime = m_nAnimeBack;
		}
		m_ptMove.x = anInitMovePosX[nDirection];
		m_ptMove.y = anInitMovePosY[nDirection];
		bCheck = FALSE;
		break;
	case CHARMOVESTATE_BATTLE:		/* �퓬�� */
		if (m_nMoveState == CHARMOVESTATE_BATTLEATACK) {
			if (m_nMoveStateReserve == 0) {
				if (m_nAnime <= nCount - 1 && m_nProcState == 0) {
					/* ���[�V�����I����Ă���J�ڂ���悤�ɗ\�񂵂Ă��� */
					m_nMoveStateReserve = nMoveState;
					return;
				}
			}
		} else {
			m_nAnimeBack = m_nAnime;
		}
		nAnime = 0;
		RenewBlockMapArea (0, 0, -1);
		break;
	case CHARMOVESTATE_BATTLEATACK:		/* �퓬�U���� */
		if (m_nMoveState == nMoveState) {
			break;
		}
		if (m_bChargeAtack) {
			/* ���ߍU������ */
			m_bChargeAtack = FALSE;
			nAnime = 1;
		} else {
			nAnime = 0;
		}
		m_dwLastTimeAnime = timeGetTime ();
		break;
	case CHARMOVESTATE_ANIME:			/* �A�j���[�V���� */
		nAnime = 0;
		m_dwLastTimeAnime = timeGetTime ();
		break;
	case CHARMOVESTATE_SWOON:			/* �C�� */
		/* ���ߍU������ */
		m_bChargeAtack = FALSE;
		m_bModeSleepTimer = FALSE;
		nAnime = 0;
		break;
	case CHARMOVESTATE_BATTLE_DEFENSE:	/* �h�䒆 */
		nAnime = 0;
		if (bChange == FALSE) {
			bCheck = FALSE;
		}
		break;
	case CHARMOVESTATE_DELETEREADY:		/* �������� */
		if (bResult) {
			AddMovePosQue (nMoveState, -1, -1, -1);
			return;
		}
		break;
	default:
		bCheck = FALSE;
		break;
	}
	m_nMoveStateReserve = 0;

	m_nMoveStateBack = nMoveState;
	SetMoveState (nMoveState);

	/* ���[�V�����̍s���`�F�b�N����H */
	if (bCheck) {
		/* 1�R�}�ڈȊO����J�n�̏ꍇ�ɍs�����Ƃ肱�ڂ��̂Ń`�F�b�N */
		for (i = 0; i <= nAnime; i ++) {
			m_nAnime = i;
			RenewAnime (0, 0);
		}
	}
	if (nAnime != -1) {
		m_nAnime = nAnime;
	}

	if (IsStateMove () == FALSE) {
		DeleteAllMovePosQue ();
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::TimerProc										 */
/* ���e		:���ԏ���														 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CInfoCharCli::TimerProc(DWORD dwTime)
{
	int i, nCount;
	BOOL bRet, bResult, bResultTmp;
	PCInfoTextEffect pInfoTextEffect;

	bRet	= FALSE;
	bResult	= CInfoCharBase::TimerProc (dwTime);

	bResult |= RenewAnime			(dwTime);
	switch (m_nMoveState) {
	case CHARMOVESTATE_MOVE:		/* �ړ��� */
	case CHARMOVESTATE_BATTLEMOVE:	/* �퓬�ړ��� */
		bResult |= TimerProcMove	(dwTime);
		break;
	case CHARMOVESTATE_BATTLEATACK:	/* �퓬�U���� */
		bResult |= TimerProcAtack	(dwTime);
		break;
	}
	bResult |= TimerProcViewState	(dwTime);
	bResult |= TimerProcBalloon		(dwTime);
	bResult |= m_bRedraw;
	m_bRedraw = FALSE;

	nCount = GetTextEffectCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoTextEffect = GetTextEffect (i);
		bResult |= pInfoTextEffect->TimerProc (dwTime);
		if (pInfoTextEffect->m_bAnimeEnd) {
			SAFE_DELETE (pInfoTextEffect);
			m_aTextEffect.RemoveAt (i);
			i --;
			nCount --;
		}
	}

	if (m_pInfoEffect) {
		bResultTmp = m_pInfoEffect->TimerProc (dwTime);
		if (bResultTmp) {
			if ((m_pInfoEffect->m_byAnimeNo == 0) && (m_pInfoEffect->m_bLoopSound)) {
				if (m_pInfoEffect->m_dwSoundID) {
					m_pMgrSound->PlaySound (m_pInfoEffect->m_dwSoundID);
				}
			}
		}
		bResult |= bResultTmp;
		bResultTmp = m_pInfoEffect->IsAnimeEnd ();
		/* �A�j���[�V�����I���H */
		if (bResultTmp) {
			/* �G�t�F�N�g���� */
			SetEffectID (0);
		}
	}

	if (m_dwLastTimeDamage) {
		if (dwTime - m_dwLastTimeDamage > 500) {
			m_dwLastTimeDamage = 0;
		}
	}

	bRet |= bResult;
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::SetMoveState										 */
/* ���e		:�ړ���Ԃ�ύX													 */
/* ���t		:2007/02/26														 */
/* ========================================================================= */

void CInfoCharCli::SetMoveState(int nMoveState)
{
	switch (nMoveState) {
	case CHARMOVESTATE_DAMAGE:			/* �_���[�W */
		m_dwLastTimeDamage = timeGetTime ();
		/* ���Ԃ̍X�V�����ŏ�Ԃ͕ύX���Ȃ� */
		return;
	}

	m_nMoveStateBack = nMoveState;
	CInfoCharBase::SetMoveState (nMoveState);

	/* ���������H */
	if (m_nMoveState == CHARMOVESTATE_DELETEREADY) {
		/* �t�F�[�h�A�E�g���Ă�������悤�ɂ��� */
		SetViewState (INFOCHARCLI_VIEWSTATE_FADEOUT);
	}

}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::MakeCharGrp										 */
/* ���e		:�L�����摜���쐬												 */
/* ���t		:2007/03/25														 */
/* ========================================================================= */

void CInfoCharCli::MakeCharGrp(void)
{
	WORD wGrpIDNPC;
	int nTmp, x, y, nHairY, nClothY, nAcceX, nAcceY;
	PCMgrGrpData pMgrGrpData;
	PCImg32 pImgBody, pImgEar, pImgCloth, pImgHairD, pImgHairU, pImgEye, pImgAcce;

	/* �����p�̉摜��p�� */
	SAFE_DELETE (m_pDibChar);
	m_pDibChar = new CImg32;
	m_pDibChar->CreateWithoutGdi (m_nGrpSize * 16, m_nGrpSize);
	m_pDibChar->FillRect (0, 0, m_nGrpSize * 16, m_nGrpSize, RGB (255, 0, 255));
	m_pDibChar->SetColorKey (RGB (255, 0, 255));

	pImgAcce	= NULL;
	pMgrGrpData = m_pMgrData->GetMgrGrpData ();
	pImgBody	= pMgrGrpData->GetDibBody	(m_wFamilyID);
	pImgEar		= pMgrGrpData->GetDibEar	(m_wFamilyID);
	pImgCloth	= pMgrGrpData->GetDibCloth	(m_wGrpIDCloth / 32);
	pImgHairD	= pMgrGrpData->GetDibHairD	(m_wGrpIDHairColor - 1);
	pImgHairU	= pMgrGrpData->GetDibHairU	(m_wGrpIDHairColor - 1);
	pImgEye		= pMgrGrpData->GetDibEye	(m_wGrpIDEyeColor - 1);

//Todo:�b��
	m_ptViewCharPos.x = m_ptViewCharPos.y = 0;
	if (m_dwMotionTypeID != 1) {
		m_ptViewCharPos.x = 16;
		m_ptViewCharPos.y = 32;
	}

	nAcceX = nAcceY = 0;
	if (m_wGrpIDAcce) {
		pImgAcce	= pMgrGrpData->GetDibAcce (m_wGrpIDAcce / 32);
		nAcceY		= (m_wGrpIDAcce % 32) * 16;
	}

	x = y = nHairY = 0;
	if (m_nSex == SEX_FEMALE) {
		x = m_nGrpSize * 16;
	}
	wGrpIDNPC = m_wGrpIDNPC;
	if (wGrpIDNPC >= 50000) {
		wGrpIDNPC -= 50000;
	}
	nClothY = m_wGrpIDCloth % 32;
	if (m_wGrpIDSP > 0) {
		pImgCloth = pMgrGrpData->GetDibSP (0);
		nClothY = m_wGrpIDSP - 1;
		x = 0;
	} else if (wGrpIDNPC > 0) {
		pImgCloth = pMgrGrpData->GetDibNPC (0);
		nClothY = wGrpIDNPC;
		x = 0;
	}
	nClothY *= 16;

	y		= 16;
	nHairY	= m_wGrpIDHairType * m_nGrpSize;

	/* �e�����p�̉摜���쐬 */
	/* �� */
	if ((m_wGrpIDSP != 0) || (wGrpIDNPC != 0)) {
		m_pDibChar->BltFrom256 (0, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x, nClothY, TRUE);
	} else {
		m_pDibChar->BltFrom256 (0, 0, m_nGrpSize * 4, m_nGrpSize, pImgBody, x, 0, TRUE);
		m_pDibChar->BltFrom256 (0, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairD, x, nHairY, TRUE);
		m_pDibChar->BltFrom256 (0, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x, nClothY, TRUE);
		m_pDibChar->BltFrom256 (0, 0, m_nGrpSize * 4, m_nGrpSize, pImgEar, 0, 0, TRUE);
		m_pDibChar->BltFrom256 (0, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairU, x, nHairY, TRUE);
	}
	m_pDibChar->BltFrom256 (0, 0, m_nGrpSize * 4, m_nGrpSize, pImgAcce, nAcceX, nAcceY, TRUE);

	/* �� */
	nTmp = 4 * m_nGrpSize;
	if ((m_wGrpIDSP != 0) || (wGrpIDNPC != 0)) {
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x + nTmp, nClothY, TRUE);
	} else {
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgBody, x + nTmp, 0, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairD, x + nTmp, nHairY, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x + nTmp, nClothY, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgEar, nTmp, 0, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairU, x + nTmp, nHairY, TRUE);
	}
	if (wGrpIDNPC == 0) {
		m_pDibChar->BltFrom256 (nTmp +  0, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 0 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 16, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 0 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 32, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 0 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 48, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 1 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
	}
	m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgAcce, nAcceX + nTmp, nAcceY, TRUE);

	/* �� */
	nTmp = 8 * m_nGrpSize;
	if ((m_wGrpIDSP != 0) || (wGrpIDNPC != 0)) {
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x + nTmp, nClothY, TRUE);
	} else {
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgBody, x + nTmp, 0, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairD, x + nTmp, nHairY, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x + nTmp, nClothY, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgEar, nTmp, 0, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairU, x + nTmp, nHairY, TRUE);
	}
	if (wGrpIDNPC == 0) {
		m_pDibChar->BltFrom256 (nTmp +  0, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 2 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 16, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 2 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 32, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 2 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 48, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 3 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
	}
	m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgAcce, nAcceX + nTmp, nAcceY, TRUE);

	/* �E */
	nTmp = 12 * m_nGrpSize;
	if ((m_wGrpIDSP != 0) || (wGrpIDNPC != 0)) {
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x + nTmp, nClothY, TRUE);
	} else {
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgBody, x + nTmp, 0, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairD, x + nTmp, nHairY, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x + nTmp, nClothY, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgEar, nTmp, 0, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairU, x + nTmp, nHairY, TRUE);
	}
	if (wGrpIDNPC == 0) {
		m_pDibChar->BltFrom256 (nTmp +  0, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 4 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 16, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 4 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 32, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 4 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 48, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 5 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
	}
	m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgAcce, nAcceX + nTmp, nAcceY, TRUE);
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::SetName											 */
/* ���e		:�L���������X�V													 */
/* ���t		:2007/01/28														 */
/* ========================================================================= */

void CInfoCharCli::SetName(LPCSTR pszName)
{
	int i, nCount, nLen, x;
	HFONT hFontOld;
	HDC hDCTmp;
	PCImg32 pDibSystem;

	SAFE_DELETE (m_pDibName);
	CInfoCharBase::SetName (pszName);

	if (pszName == NULL) {
		return;
	}
	pDibSystem = m_pMgrData->GetMgrGrpData ()->GetDibSystem ();

	x		= 0;
	nCount	= m_abyMark.GetSize ();

	m_pDibName = new CImg32;
	m_pDibName->Create (strlen (pszName) * 6 + 2 + (nCount * 16), 16);
	m_pDibName->Clear ();

	hDCTmp = m_pDibName->Lock ();
	hFontOld = (HFONT)SelectObject (hDCTmp, m_hFont);
	SetBkMode (hDCTmp, TRANSPARENT);

	for (i = 0; i < nCount; i ++) {
		m_pDibName->BltFrom256 (x, 1, 16, 16, pDibSystem, 176 + (m_abyMark[i] - 1) * 16, 0, TRUE);
		x += 16;
	}

	nLen = strlen (pszName);
	SetTextColor (hDCTmp, RGB (10, 10, 10));
	TextOut (hDCTmp, x + 0, 2, pszName, nLen);
	TextOut (hDCTmp, x + 2, 2, pszName, nLen);
	TextOut (hDCTmp, x + 1, 1, pszName, nLen);
	TextOut (hDCTmp, x + 1, 3, pszName, nLen);
	SetTextColor (hDCTmp, m_clName);
	TextOut (hDCTmp, x + 1, 2, pszName, nLen);

	SelectObject (hDCTmp, hFontOld);
	m_pDibName->Unlock ();
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::SetSpeak											 */
/* ���e		:�������e���X�V													 */
/* ���t		:2007/02/04														 */
/* ========================================================================= */

void CInfoCharCli::SetSpeak(LPCSTR pszSpeak)
{
	char szTmp[32];
	BOOL bResult;
	int nPos, nLen, nLenTmp, nCount, nWidth, nHeight, nLine;
	HFONT hFontOld;
	HDC hDCTmp;
	CmyString strSpeak1, strSpeak2;

	SAFE_DELETE (m_pDibSpeak);
	CInfoCharBase::SetSpeak (pszSpeak);

	if (pszSpeak == NULL) {
		return;
	}
	nLen = strlen (pszSpeak);
	if (nLen <= 0) {
		return;
	}

	/* �摜�쐬���� */
	nPos	= 0;
	nLine	= 0;
	nWidth	= (nLen > 20) ? 20 : nLen;
	nHeight = nLen / 20 + 1;
	if (nLen % 20 == 0) {
		/* ���s���x�̕������̏ꍇ�͉摜�T�C�Y��1�s�����炷 */
		nHeight --;
		nHeight = max (nHeight, 1);
	}

	m_pDibSpeak = new CImg32;
	m_pDibSpeak->Create ((nWidth + 1) * 6 + 2, nHeight * 14);
	m_pDibSpeak->Clear ();

	hDCTmp = m_pDibSpeak->Lock ();
	hFontOld = (HFONT)SelectObject (hDCTmp, m_hFont);
	SetBkMode (hDCTmp, TRANSPARENT);

	/* �S�p10�����P�ʂ�1�s���摜���쐬 */
	while (1) {
		ZeroMemory (szTmp, sizeof (szTmp));
		/* 1�s���̕�������쐬 */
		for (nCount = 0; nCount < 20; nCount ++) {
			if (nPos + nCount >= nLen) {
				break;
			}
			szTmp[nCount] = pszSpeak[nPos + nCount];
			bResult = IsDBCSLeadByte (pszSpeak[nPos + nCount]);
			if (bResult) {
				/* �S�p�����̐擪�o�C�g�̎��͌���1�o�C�g���ǉ� */
				nCount ++;
				szTmp[nCount] = pszSpeak[nPos + nCount];
			}
		}
		nLenTmp = strlen (szTmp);
		nPos += nCount;
		if (nLenTmp > 0) {
			SetTextColor (hDCTmp, RGB (10, 10, 10));
			TextOut (hDCTmp, 0, 1 + nLine * 14, szTmp, nLenTmp);
			TextOut (hDCTmp, 2, 1 + nLine * 14, szTmp, nLenTmp);
			TextOut (hDCTmp, 1, 0 + nLine * 14, szTmp, nLenTmp);
			TextOut (hDCTmp, 1, 2 + nLine * 14, szTmp, nLenTmp);
			SetTextColor (hDCTmp, m_clSpeak);
			TextOut (hDCTmp, 1, 1 + nLine * 14, szTmp, nLenTmp);
			nLine ++;
		}

		if (nPos >= nLen) {
			break;
		}
	}

	SelectObject (hDCTmp, hFontOld);
	m_pDibSpeak->Unlock ();
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::SetSleepTimer									 */
/* ���e		:���₷�݃^�C�}�[��ݒ�											 */
/* ���t		:2008/07/21														 */
/* ========================================================================= */

void CInfoCharCli::SetSleepTimer(BOOL bSleepTimer)
{
	CPacketMSGCMD_PARA1 Packet;

	m_bModeSleepTimer = bSleepTimer;
	if (bSleepTimer && (m_dwBalloonID == 0)) {
//Todo:�b��
		Packet.Make (SBOCOMMANDID_SUB_MSGCMD_BALLOON, m_dwCharID, 33);
		m_pSock->Send (&Packet);
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::SetViewState										 */
/* ���e		:�\����Ԑݒ�													 */
/* ���t		:2007/02/26														 */
/* ========================================================================= */

void CInfoCharCli::SetViewState(int nViewState)
{
	m_dwLastTimeViewState	= timeGetTime ();
	m_nViewState			= nViewState;

	switch (m_nViewState) {
	case INFOCHARCLI_VIEWSTATE_NONE:
		m_dwLastTimeViewState	= 0;
		m_nFadeLevel			= 0;
		break;
	case INFOCHARCLI_VIEWSTATE_FADEIN:			/* �t�F�[�h�C�� */
		m_nFadeLevel = 99;
		break;
	case INFOCHARCLI_VIEWSTATE_FADEOUT:			/* �t�F�[�h�A�E�g */
		m_nFadeLevel = 1;
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::GetMotionInfo									 */
/* ���e		:���[�V���������擾											 */
/* ���t		:2007/11/24														 */
/* ========================================================================= */

CInfoMotion *CInfoCharCli::GetMotionInfo(int *pnCount)
{
	int nAnime, nCount, nDirection, nMoveState, anDirection[] = {0, 1, 2, 3, 0, 1, 1, 0};
	PCInfoMotion pRet;
	PARRAYMOTIONINFO paMotionInfo;

	pRet	= NULL;
	nCount	= 0;
	paMotionInfo = NULL;

	nAnime = m_nAnime;
	nDirection = anDirection[m_nDirection];
	if (m_bMotionDirection == FALSE) {
		nDirection = 0;
	}
	nMoveState = m_nMoveState;
	switch (nMoveState) {
	case CHARMOVESTATE_DELETE:		/* ���� */
	case CHARMOVESTATE_DELETEREADY:	/* �������� */
		nMoveState = m_nMoveStateBack;
		break;
	}

	/* ���荞�ݍĐ�����H */
	if (m_bMotionInterrupt) {
		paMotionInfo = &m_aMotion[CHARMOTIONID_INTERRUUPT][nDirection];

	} else {
		/* ���ߍU�����H */
		if (m_bChargeAtack) {
			switch (nMoveState) {
			case CHARMOVESTATE_BATTLEMOVE:	/* �퓬�ړ��� */
				break;
			default:
				/* ���ߒ��͍U�����[�V������1�R�}�ڂ�\������ */
				nAnime		= 0;
				nMoveState	= CHARMOVESTATE_BATTLEATACK;
				break;
			}
		}

		switch (nMoveState) {
		case CHARMOVESTATE_STAND:		/* ���� */
		case CHARMOVESTATE_DELETE:		/* ���� */
		case CHARMOVESTATE_DELETEREADY:	/* �������� */
			paMotionInfo = &m_aMotion[CHARMOTIONID_STAND][nDirection];
			break;
		case CHARMOVESTATE_SIT:			/* ���蒆 */
		case CHARMOVESTATE_SWOON:		/* �C�� */
		case CHARMOVESTATE_SLEEPTIMER:	/* ���₷�݃^�C�}�[�� */
			paMotionInfo = &m_aMotion[CHARMOTIONID_SIT][nDirection];
			break;
		case CHARMOVESTATE_MOVE:		/* �ړ��� */
		case CHARMOVESTATE_WALKANIME:	/* ������ */
			paMotionInfo = &m_aMotion[CHARMOTIONID_WALK][nDirection];
			break;
		case CHARMOVESTATE_BATTLE:		/* �퓬�� */
			paMotionInfo = &m_aMotion[CHARMOTIONID_BATTLESTAND][nDirection];
			break;
		case CHARMOVESTATE_BATTLEMOVE:	/* �퓬�ړ��� */
			paMotionInfo = &m_aMotion[CHARMOTIONID_BATTLEWALK][nDirection];
			break;
		case CHARMOVESTATE_BATTLEATACK:	/* �퓬�U���� */
			paMotionInfo = &m_aMotion[CHARMOTIONID_ATACK][nDirection];
			break;
		case CHARMOVESTATE_ANIME:		/* �A�j���[�V���� */
			paMotionInfo = &m_aMotion[CHARMOTIONID_ANIME][nDirection];
			break;
		case CHARMOVESTATE_BATTLE_DEFENSE:	/* �h�� */
			paMotionInfo = &m_aMotion[CHARMOTIONID_BATTLEDEFENSE][nDirection];
			break;
		}
	}
	if (paMotionInfo == NULL) {
		goto Exit;
	}

	nCount = paMotionInfo->GetSize ();
	if (nCount == 0) {
		goto Exit;
	}
	if (nAnime >= nCount) {
		nAnime = 0;
	}
	pRet = paMotionInfo->GetAt (nAnime);

Exit:
	if (pnCount) {
		*pnCount = nCount;
	}
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::IsChgWait										 */
/* ���e		:��ԕύX�҂�������												 */
/* ���t		:2007/07/24														 */
/* ========================================================================= */

BOOL CInfoCharCli::IsChgWait(void)
{
	BOOL bRet;

	bRet = TRUE;

	if (m_bChgWait) {
		goto Exit;
	}

	bRet = FALSE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::SetEffectID										 */
/* ���e		:�G�t�F�N�gID���w��												 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CInfoCharCli::SetEffectID(DWORD dwEffectID)
{
	PCLibInfoEffect pLibInfo;
	PCInfoEffect pInfo;

	pLibInfo	= m_pMgrData->GetLibInfoEffect ();
	pInfo		= (PCInfoEffect)pLibInfo->GetPtr (dwEffectID);

	SAFE_DELETE (m_pInfoEffect);
	if (pInfo == NULL) {
		goto Exit;
	}

	m_pInfoEffect = new CInfoEffect;
	m_pInfoEffect->Copy (pInfo);
	if (m_pInfoEffect->m_dwSoundID) {
		m_pMgrSound->PlaySound (m_pInfoEffect->m_dwSoundID);
	}

Exit:
	return;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::SetBalloonID										 */
/* ���e		:���o��ID���w��													 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

void CInfoCharCli::SetBalloonID(DWORD dwBalloonID)
{
	m_dwLastTimeBalloon = timeGetTime ();
	m_dwBalloonID		= dwBalloonID;
	m_dwBalloonAnimeID	= 0;
	m_dwBalloonGrpID	= 0;
	if (dwBalloonID == 0) {
		m_dwLastTimeBalloon = 0;
		m_dwBalloonID		= 0;
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::GetGrpID											 */
/* ���e		:�O���t�B�b�NID���擾											 */
/* ���t		:2007/12/01														 */
/* ���l		:���[�V��������ID���畐��摜�̏ꍇ��ID��ϊ�����				 */
/* ========================================================================= */

void CInfoCharCli::GetGrpID(WORD &wGrpIDMain, WORD &wGrpIDSub)
{
	int nCountX;
	BOOL bInit;
	WORD wGrpIDMainTmp, wGrpIDSubTmp;
	PCMgrGrpData pMgrGrpData;

	pMgrGrpData	= m_pMgrData->GetMgrGrpData ();
	bInit		= TRUE;

	switch (wGrpIDMain) {
	case GRPIDMAIN_WEAPON:			/* ���� */
	case GRPIDMAIN_WEAPON_BOW:		/* ����(�|) */
	case GRPIDMAIN_WEAPON_GLOVE:	/* ����(�Ō�) */
	case GRPIDMAIN_WEAPON_ETC:		/* ����(���̑�) */
	case GRPIDMAIN_2X2_ARMS:		/* ������(2x2) */
		if (m_dwEquipItemIDArmsRight == 0) {
			goto Exit;
		}
		break;
	case GRPIDMAIN_2X2_SHIELD:		/* ��(2x2) */
		if (m_dwEquipItemIDArmsLeft == 0) {
			goto Exit;
		}
		break;
	}

	wGrpIDMainTmp	= wGrpIDMain;
	wGrpIDSubTmp	= wGrpIDSub;

	switch (wGrpIDMainTmp) {
	case GRPIDMAIN_WEAPON:			/* ���� */
		wGrpIDMain = GRPIDMAIN_WEAPON;
		break;
	case GRPIDMAIN_WEAPON_BOW:		/* ����(�|) */
		wGrpIDMain = GRPIDMAIN_WEAPON_BOW;
		break;
	case GRPIDMAIN_WEAPON_GLOVE:	/* ����(�Ō�) */
		wGrpIDMain = GRPIDMAIN_WEAPON_GLOVE;
		break;
	case GRPIDMAIN_WEAPON_ETC:		/* ����(���̑�) */
		wGrpIDMain = GRPIDMAIN_WEAPON_ETC;
		break;
	case GRPIDMAIN_2X2_ARMS:		/* ������(2x2) */
	case GRPIDMAIN_2X2_SHIELD:		/* ��(2x2) */
		wGrpIDMain = wGrpIDMainTmp;
		break;
	default:
		break;
	}

	nCountX = pMgrGrpData->GetGrpCountX (wGrpIDMain);

	switch (wGrpIDMainTmp) {
	case GRPIDMAIN_WEAPON:			/* ���� */
	case GRPIDMAIN_WEAPON_BOW:		/* ����(�|) */
	case GRPIDMAIN_WEAPON_GLOVE:	/* ����(�Ō�) */
	case GRPIDMAIN_WEAPON_ETC:		/* ����(���̑�) */
	case GRPIDMAIN_2X2_ARMS:		/* ������(2x2) */
		if ((wGrpIDSub == 0) || (m_wGrpIDArmsSub == 0)) {
			break;
		}
		wGrpIDSub = (wGrpIDSubTmp / nCountX) * nCountX + (wGrpIDSubTmp % nCountX);
		wGrpIDSub += ((m_wGrpIDArmsSub - 1) * nCountX);
		bInit = FALSE;
		break;
	case GRPIDMAIN_2X2_SHIELD:		/* ��(2x2) */
		if ((wGrpIDSub == 0) || (m_wGrpIDArmsLeftSub == 0)) {
			break;
		}
		wGrpIDSub = (wGrpIDSubTmp / nCountX) * nCountX + (wGrpIDSubTmp % nCountX);
		wGrpIDSub += ((m_wGrpIDArmsLeftSub - 1) * nCountX);
		bInit = FALSE;
		break;
	default:
		bInit = FALSE;
		break;
	}

Exit:
	if (bInit) {
		wGrpIDMain = wGrpIDSub = 0;
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::InitMotionInfo									 */
/* ���e		:���[�V�������̏�����											 */
/* ���t		:2008/01/03														 */
/* ========================================================================= */

void CInfoCharCli::InitMotionInfo(DWORD dwMotionID)
{
	m_nAnime			= 0;
	m_bMotionInterrupt	= FALSE;
	m_dwLastTimeAnime	= timeGetTime ();

	if (dwMotionID == CHARMOTIONID_INTERRUUPT) {
		m_bMotionInterrupt = TRUE;
		RenewAnime (0, 0);
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::GetEfcGrpPos										 */
/* ���e		:�G�t�F�N�g�摜�̊J�n���W���擾									 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CInfoCharCli::GetEfcGrpPos(POINT &ptDst)
{
	int nSize, nCountX;
	WORD wGrpID;
	PCMgrGrpData pMgrGrpData;

	pMgrGrpData = m_pMgrData->GetMgrGrpData ();

	ptDst.x = ptDst.y = 0;
	nSize	= GetEfcGrpSize ();
	nCountX	= pMgrGrpData->GetGrpCountX (m_pInfoEffect->m_dwGrpIDMain);

	if (m_pInfoEffect == NULL) {
		goto Exit;
	}

//Todo:2���ڈȍ~�̑Ή�
	wGrpID = m_pInfoEffect->GetGrpID () - 1;
	ptDst.x = (wGrpID % nCountX) * nSize;
	ptDst.y = (wGrpID / nCountX) * nSize;

Exit:
	return;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::GetEfcImg										 */
/* ���e		:�`�挳�̃G�t�F�N�g�摜�C���[�W���擾							 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CImg32 *CInfoCharCli::GetEfcImg(void)
{
	PCImg32 pRet;

	pRet = NULL;
	if (m_pInfoEffect == NULL) {
		goto Exit;
	}
//Todo:2���ڈȍ~�̎w��
	pRet = m_pMgrData->GetMgrGrpData ()->GetDib (m_pInfoEffect->m_dwGrpIDMain, 0, 0);
Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::GetEfcLevel										 */
/* ���e		:�G�t�F�N�g�摜�̓��߃��x�����擾								 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

int CInfoCharCli::GetEfcLevel(void)
{
	int nRet;

	nRet = 0;
	if (m_pInfoEffect == NULL) {
		goto Exit;
	}
	nRet = m_pInfoEffect->GetLevel ();

Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::GetEfcGrpSize									 */
/* ���e		:�G�t�F�N�g�摜�̃T�C�Y���擾									 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

int CInfoCharCli::GetEfcGrpSize(void)
{
	int nRet;

	nRet = m_pMgrData->GetMgrGrpData ()->GetGrpSize (m_pInfoEffect->m_dwGrpIDMain);
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::GetEfcDrawPos									 */
/* ���e		:�G�t�F�N�g�摜�̕`��␳�����擾								 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CInfoCharCli::GetEfcDrawPos(POINT &ptDst)
{
	ptDst.x = -1 * (m_nGrpSize / 2);
	ptDst.y = -1 * (m_nGrpSize / 2);

	switch (m_nMoveState) {
	case CHARMOVESTATE_BATTLE:		/* �퓬�� */
	case CHARMOVESTATE_BATTLEATACK:	/* �퓬�U���� */
	case CHARMOVESTATE_BATTLEMOVE:	/* �퓬�ړ��� */
		ptDst.x = ptDst.y = 0;
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::GetBalloonGrpID									 */
/* ���e		:���o���摜ID���擾												 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

DWORD CInfoCharCli::GetBalloonGrpID(void)
{
	return m_dwBalloonGrpID;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::IsEnableMove										 */
/* ���e		:�ړ��ł����Ԃ�����											 */
/* ���t		:2008/06/28														 */
/* �߂�l	:TRUE:�ړ���													 */
/* ========================================================================= */

BOOL CInfoCharCli::IsEnableMove(void)
{
	BOOL bRet;

	bRet = CInfoCharBase::IsEnableMove ();
	if (bRet == FALSE) {
		goto Exit;
	}
	/* �}�b�v�C�x���g�`�F�b�N�҂��H */
	if (m_bWaitCheckMapEvent) {
		bRet = FALSE;
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::IsDamage											 */
/* ���e		:�_���[�W�󂯒�������											 */
/* ���t		:2008/06/29														 */
/* ========================================================================= */

BOOL CInfoCharCli::IsDamage(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_dwLastTimeDamage) {
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::GetTextEffectCount								 */
/* ���e		:�o�^�����擾													 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

int CInfoCharCli::GetTextEffectCount(void)
{
	return m_aTextEffect.GetSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::AddTextEffect									 */
/* ���e		:�����G�t�F�N�g��ǉ�											 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

void CInfoCharCli::AddTextEffect(DWORD dwData, int nColor, int nSize)
{
	PCInfoTextEffect pInfo;

	pInfo = new CInfoTextEffect;
//Todo;�w����@
	pInfo->Set (dwData, TEXTEFFECTTYPE_WAVE, nColor, nSize);

	m_aTextEffect.Add (pInfo);
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::AddTextEffect									 */
/* ���e		:�����G�t�F�N�g��ǉ�											 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

void CInfoCharCli::AddTextEffect(LPCSTR pszData, int nColor, int nSize)
{
	PCInfoTextEffect pInfo;

	pInfo = new CInfoTextEffect;
//Todo;�w����@
	pInfo->Set (pszData, TEXTEFFECTTYPE_NORMAL, nColor, nSize);

	m_aTextEffect.Add (pInfo);
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::DeleteAllTextEffect								 */
/* ���e		:�����G�t�F�N�g��S�č폜										 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

void CInfoCharCli::DeleteAllTextEffect(void)
{
	int i, nCount;
	PCInfoTextEffect pInfo;

	nCount = m_aTextEffect.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aTextEffect[i];
		SAFE_DELETE (pInfo);
	}
	m_aTextEffect.RemoveAll ();
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::GetTextEffectGrpCount							 */
/* ���e		:�摜�����擾													 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

int CInfoCharCli::GetTextEffectGrpCount(int nIndex)
{
	return m_aTextEffect[nIndex]->GetGrpCount ();
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::GetTextEffectGrpNo								 */
/* ���e		:�w��ԍ��̐��l���擾											 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

int CInfoCharCli::GetTextEffectGrpNo(int nIndex, int nNo)
{
	return m_aTextEffect[nIndex]->GetGrpNo (nNo);
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::GetTextEffectDrawPos								 */
/* ���e		:�w��ԍ��̕\���ʒu�␳�l���擾									 */
/* �߂�l	:TRUE:�`�悷�� FALSE:�`�悵�Ȃ�									 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

BOOL CInfoCharCli::GetTextEffectDrawPos(int nIndex, int nNo, POINT &ptDst, BYTE &byLevel)
{
	return m_aTextEffect[nIndex]->GetDrawPos (nNo, ptDst, byLevel);
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::GetTextEffectData								 */
/* ���e		:�w��ԍ��̕\���ʒu�␳�l���擾									 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

DWORD CInfoCharCli::GetTextEffectData(int nIndex)
{
	return m_aTextEffect[nIndex]->GetData ();
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::GetTextEffect									 */
/* ���e		:�����G�t�F�N�g���擾											 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

CInfoTextEffect *CInfoCharCli::GetTextEffect(int nIndex)
{
	return m_aTextEffect[nIndex];
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::AddMovePosQue									 */
/* ���e		:�ǉ�															 */
/* ���t		:2008/08/13														 */
/* ========================================================================= */

void CInfoCharCli::AddMovePosQue(int nState, int nDirection, int x, int y)
{
	PMOVEPOSQUE pInfo;

	if (m_nMoveState == CHARMOVESTATE_DELETEREADY) {
		return;
	}

	pInfo = new MOVEPOSQUE;
	pInfo->nState		= nState;
	pInfo->nDirection	= nDirection;
	pInfo->ptPos.x		= x;
	pInfo->ptPos.y		= y;

	m_apMovePosQue.Add (pInfo);
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::DeleteMovePosQue									 */
/* ���e		:�폜															 */
/* ���t		:2008/08/13														 */
/* ========================================================================= */

void CInfoCharCli::DeleteMovePosQue(int nNo)
{
	PMOVEPOSQUE pInfo;

	if (nNo >= m_apMovePosQue.GetSize ()) {
		return;
	}
	pInfo = m_apMovePosQue[nNo];
	SAFE_DELETE (pInfo);
	m_apMovePosQue.RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::DeleteAllMovePosQue								 */
/* ���e		:�S�č폜														 */
/* ���t		:2008/08/13														 */
/* ========================================================================= */

void CInfoCharCli::DeleteAllMovePosQue(void)
{
	int i, nCount;

	nCount = m_apMovePosQue.GetSize ();
	for (i = 0; i < nCount; i ++) {
		DeleteMovePosQue (0);
	}
	m_apMovePosQue.RemoveAll ();
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::RenewAnime										 */
/* ���e		:�A�j���[�V�����̍X�V											 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CInfoCharCli::RenewAnime(
	DWORD dwTime,		/* [in] ���� */
	int nAdd/*=1*/)		/* [in] ���̃p�^�[���ւ̑����� */
{
	BOOL bRet, bCheck;
	int nCount;
	DWORD dwTmp, dwWait;
	PCInfoMotion pInfoMotion;

	bRet = FALSE;

	pInfoMotion = GetMotionInfo (&nCount);
	if (pInfoMotion == NULL) {
		goto Exit;
	}
	if (pInfoMotion->m_byWait == 0) {
		/* �҂����Ԗ��ݒ�̏ꍇ�̓R�}�ړ����Ȃ� */
		goto Exit;
	}

	if (nAdd != 0) {
		dwTmp  = dwTime - m_dwLastTimeAnime;
		dwWait = (DWORD)pInfoMotion->m_byWait * 10;
		if (dwTmp < dwWait) {
			goto Exit;
		}
	}

	bCheck = FALSE;
	if (m_nAnime + nAdd >= nCount) {
		if (pInfoMotion->m_bLoop) {
			bCheck = TRUE;
			m_nAnime = 0;
			m_dwLastTimeAnime = dwTime;
		} else {
			switch (pInfoMotion->m_dwMotionListID) {
			case CHARMOTIONLISTID_DEAD_UP:			/* ���S(��) */
			case CHARMOTIONLISTID_DEAD_DOWN:		/* ���S(��) */
			case CHARMOTIONLISTID_DEAD_LEFT:		/* ���S(��) */
			case CHARMOTIONLISTID_DEAD_RIGHT:		/* ���S(�E) */
				m_dwLastTimeAnime = dwTime;
				/* ���S���[�V������͂��̂܂܂ɂ��Ă��� */
				break;
			default:
				if (m_bMotionInterrupt) {
					m_nAnime = 0;
				}
				m_bMotionInterrupt = FALSE;
				if (m_nMoveStateReserve) {
					/* �\�񂵂Ă�����Ԃ֑J�� */
					ChgMoveState (m_nMoveStateReserve);
				}
				break;
			}
		}

	} else {
		m_nAnime += nAdd;
		bCheck = TRUE;
		if (nAdd != 0) {
			m_dwLastTimeAnime = dwTime;
		}
	}
	if (bCheck || (nAdd == 0)) {
		pInfoMotion = GetMotionInfo (&nCount);
		if (pInfoMotion) {
			if (pInfoMotion->m_dwSoundID) {
				m_pMgrSound->PlaySound (pInfoMotion->m_dwSoundID);
			}
			if (pInfoMotion->m_dwProcID) {
				MotionProc (pInfoMotion->m_dwProcID);
			}
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::TimerProcMove									 */
/* ���e		:�ړ�����														 */
/* ���t		:2006/10/08														 */
/* ========================================================================= */

BOOL CInfoCharCli::TimerProcMove(DWORD dwTime)
{
	BOOL bRet;
	int nStandState, nDirection;
	DWORD dwTmp, dwWait;
	PMOVEPOSQUE pQue;

	bRet		= FALSE;
	dwWait		= GetMoveWait ();
	if (m_dwMoveWaitOnce != 0) {
		dwWait = m_dwMoveWaitOnce;
	}
	if (m_bSkipMove) {
		if (m_apMovePosQue.GetSize () > 0) {
			/* �L���[�ɐς܂�Ă���ꍇ�͑҂����Ԗ��� */
			dwWait = 1;
		}
	}
	nStandState	= CHARMOVESTATE_STAND;

	switch (m_nMoveState) {
	case CHARMOVESTATE_MOVE:		/* �ړ��� */
		break;
	case CHARMOVESTATE_BATTLEMOVE:	/* �퓬�ړ��� */
		nStandState = CHARMOVESTATE_BATTLE;
		break;
	default:
		goto Exit;
	}
	if (m_bChargeAtack) {
		dwWait *= 2;
	}

	nDirection = m_nDirection;
	if (m_nMoveDirection != -1) {
		nDirection = m_nMoveDirection;
	}

	dwTmp = dwTime - m_dwLastTimeMove;
	if (m_dwLastTimeMove == 0) {
		m_dwLastTimeMove = dwTime;
		goto Exit;
	}

	dwWait = max (dwWait, 1);
	dwTmp /= dwWait;

	switch (nDirection) {
	case 0:		/* �� */
		m_ptMove.y -= dwTmp;
		m_ptMove.y = max (0, m_ptMove.y);
		break;
	case 1:		/* �� */
		m_ptMove.y += dwTmp;
		m_ptMove.y = min (0, m_ptMove.y);
		break;
	case 2:		/* �� */
		m_ptMove.x -= dwTmp;
		m_ptMove.x = max (0, m_ptMove.x);
		break;
	case 3:		/* �E */
		m_ptMove.x += dwTmp;
		m_ptMove.x = min (0, m_ptMove.x);
		break;
	case 4:
		m_ptMove.y -= dwTmp;
		m_ptMove.y = max (0, m_ptMove.y);
		m_ptMove.x += dwTmp;
		m_ptMove.x = min (0, m_ptMove.x);
		break;
	case 5:
		m_ptMove.y += dwTmp;
		m_ptMove.y = min (0, m_ptMove.y);
		m_ptMove.x += dwTmp;
		m_ptMove.x = min (0, m_ptMove.x);
		break;
	case 6:
		m_ptMove.y += dwTmp;
		m_ptMove.y = min (0, m_ptMove.y);
		m_ptMove.x -= dwTmp;
		m_ptMove.x = max (0, m_ptMove.x);
		break;
	case 7:
		m_ptMove.y -= dwTmp;
		m_ptMove.y = max (0, m_ptMove.y);
		m_ptMove.x -= dwTmp;
		m_ptMove.x = max (0, m_ptMove.x);
		break;
	}

	if ((dwTmp > 0) && (!((m_ptMove.x == 0) && (m_ptMove.y == 0)))) {
		m_dwLastTimeMove += (dwTmp * dwWait);
	}
	if ((m_ptMove.x == 0) && (m_ptMove.y == 0)) {
		if (m_pMgrData->GetCharID () != m_dwCharID) {
			if (dwTime - m_dwLastTimeMove < 250) {
				if (m_apMovePosQue.GetSize () == 0) {
					goto Exit;
				}
			}
		}
		m_nAnimeBack = m_nAnime;
		m_nMoveState = 0;
		if (m_apMovePosQue.GetSize () > 0) {
			pQue = m_apMovePosQue[0];
			nStandState = pQue->nState;
			if (pQue->ptPos.x >= 0) {
				SetPos (pQue->ptPos.x, pQue->ptPos.y);
				SetDirection (pQue->nDirection);
				CInfoCharCli::ChgMoveState (nStandState);
				nStandState = -1;
			}
			DeleteMovePosQue (0);
		}
		m_dwMoveWaitOnce = 0;
		ChgMoveState (nStandState);
	}
	if (dwTmp == 0) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::TimerProcViewState								 */
/* ���e		:�\����ԏ���													 */
/* ���t		:2007/02/26														 */
/* ========================================================================= */

BOOL CInfoCharCli::TimerProcViewState(DWORD dwTime)
{
	BOOL bRet;
	DWORD dwTmp;

	bRet	= FALSE;
	dwTmp	= dwTime - m_dwLastTimeViewState;
	dwTmp	/= 10;

	if (dwTmp == 0) {
		goto Exit;
	}
	m_dwLastTimeViewState = dwTime;

	switch (m_nViewState) {
	case INFOCHARCLI_VIEWSTATE_NONE:
		goto Exit;
	case INFOCHARCLI_VIEWSTATE_FADEIN:			/* �t�F�[�h�C�� */
		m_nFadeLevel -= dwTmp;
		if (m_nFadeLevel < 0) {
			SetViewState (INFOCHARCLI_VIEWSTATE_NONE);
		}
		break;
	case INFOCHARCLI_VIEWSTATE_FADEOUT:			/* �t�F�[�h�A�E�g */
		m_nFadeLevel += dwTmp;
		if (m_nFadeLevel > 100) {
			m_nFadeLevel = 100;
			if ((m_pInfoEffect == NULL) || (m_pInfoEffect && m_pInfoEffect->m_bLoop)) {
				SetViewState (INFOCHARCLI_VIEWSTATE_NONE);
				/* ���������H */
				if (m_nMoveState == CHARMOVESTATE_DELETEREADY) {
					SetMoveState (CHARMOVESTATE_DELETE);
				} else {
					SetViewState (INFOCHARCLI_VIEWSTATE_FADEIN);
				}
			}
		}
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::TimerProcAtack									 */
/* ���e		:�U������														 */
/* ���t		:2007/07/12														 */
/* ========================================================================= */

BOOL CInfoCharCli::TimerProcAtack(DWORD dwTime)
{
	BOOL bRet;
	DWORD dwTmp, dwWait;
	int nCount;
	PCInfoMotion pInfoMotion;

	bRet = FALSE;
	pInfoMotion = GetMotionInfo (&nCount);
	if (pInfoMotion) {
		if (m_nAnime < nCount - 1) {
			goto Exit;
		}
		dwTmp  = dwTime - m_dwLastTimeAnime;
		dwWait = (DWORD)pInfoMotion->m_byWait * 10;
		/* �ŏI�R�}�̎��Ԃ�2�{�̊ԕ\�������H */
		if (dwTmp < dwWait * 2) {
			goto Exit;
		}
	}
	ChgMoveState (CHARMOVESTATE_BATTLE);
	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::TimerProcBalloon									 */
/* ���e		:���o������														 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

BOOL CInfoCharCli::TimerProcBalloon(DWORD dwTime)
{
	BOOL bRet;
	DWORD dwTimeTmp;
	PCInfoEfcBalloon pInfo;
	PCLibInfoEfcBalloon pLibInfo;

	bRet = FALSE;
	if (m_dwBalloonID == 0) {
		goto Exit;
	}

	pLibInfo = m_pMgrData->GetLibInfoEfcBalloon ();
	pInfo	 = pLibInfo->GetPtr (m_dwBalloonID, m_dwBalloonAnimeID);
	if (pInfo == NULL) {
		goto Exit;
	}

	/* ����H */
	if (m_dwBalloonGrpID == 0) {
		m_dwBalloonGrpID	= pInfo->m_dwGrpID;
		m_dwLastTimeBalloon	= dwTime;
		if (pInfo->m_dwSoundID != 0) {
			m_pMgrSound->PlaySound (pInfo->m_dwSoundID);
		}
		bRet = TRUE;
		goto Exit;
	}

	dwTimeTmp = m_dwLastTimeBalloon + (DWORD)pInfo->m_byWait * 10;
	if (dwTime < dwTimeTmp) {
		goto Exit;
	}
	m_dwBalloonAnimeID = pLibInfo->GetNextAnimeID (m_dwBalloonID, m_dwBalloonAnimeID);
	if (m_dwBalloonAnimeID == -1) {
		m_dwBalloonID		= 0;
		m_dwBalloonAnimeID	= 0;
		m_dwLastTimeBalloon = 0;
		m_dwBalloonGrpID	= 0;

	} else {
		pInfo = pLibInfo->GetPtr (m_dwBalloonID, m_dwBalloonAnimeID);
		if (pInfo == NULL) {
			goto Exit;
		}
		m_dwLastTimeBalloon = dwTime;
		m_dwBalloonGrpID	= pInfo->m_dwGrpID;
		if (pInfo->m_dwSoundID != 0) {
			m_pMgrSound->PlaySound (pInfo->m_dwSoundID);
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharCli::MotionProc										 */
/* ���e		:���[�V��������													 */
/* ���t		:2008/01/01														 */
/* ========================================================================= */

void CInfoCharCli::MotionProc(DWORD dwProcID)
{
	CPacketBATTLE_REQ_ATACK PacketBATTLE_REQ_ATACK;

	if (dwProcID == 0) {
		return;
	}
	if (m_pMgrData->GetCharID () != m_dwCharID) {
		return;
	}
	if (m_bChargeAtack) {
		return;
	}

	switch (dwProcID) {
	case CHARMOTIONPROCID_ATACK:			/* �U�� */
		PacketBATTLE_REQ_ATACK.Make (m_dwCharID, 0);
		m_pSock->Send (&PacketBATTLE_REQ_ATACK);
		break;
	case CHARMOTIONPROCID_FISHING:			/* �ނ� */
		{
			CPacketCHAR_PARA1 Packet;

			Packet.Make (SBOCOMMANDID_SUB_CHAR_PROC_FISHING, m_dwCharID, 0);
			m_pSock->Send (&Packet);
			SetProcState (CHARPROCSTATEID_FISHING);
		}
		break;
	}
}

/* Copyright(C)URARA-works 2006 */
