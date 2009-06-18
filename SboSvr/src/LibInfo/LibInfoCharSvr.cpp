/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoCharSvr.cpp											 */
/* ���e			:�L������񃉃C�u�����N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/21													 */
/* ========================================================================= */

#include "stdafx.h"
#include <math.h>
#include "SBOGlobal.h"
#include "UraraSockTCPSBO.h"
#include "Packet.h"
#include "Command.h"
#include "InfoMapParts.h"
#include "LibInfoMapBase.h"
#include "LibInfoItemType.h"
#include "LibInfoItem.h"
#include "LibInfoItemWeapon.h"
#include "LibInfoSystem.h"
#include "InfoSystem.h"
#include "InfoMapEvent.h"
#include "InfoCharATACKANIMESvr.h"
#include "InfoCharMOVEATACKSvr.h"
#include "InfoCharBATTLE1Svr.h"
#include "InfoCharBATTLE2Svr.h"
#include "InfoCharPUTNPC.h"
#include "InfoTextEffect.h"
#include "MainFrame.h"
#include "MgrData.h"
#include "LibInfoCharSvr.h"


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::CLibInfoCharSvr								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/01/21														 */
/* ========================================================================= */

CLibInfoCharSvr::CLibInfoCharSvr()
{
	m_pMainFrame		= NULL;
	m_pMgrData			= NULL;
	m_pLibInfoMap		= NULL;
	m_pLibInfoSkill		= NULL;
	m_pLibInfoItemType	= NULL;
	m_pLibInfoItem		= NULL;
	m_pSock				= NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::~CLibInfoCharSvr								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/01/21														 */
/* ========================================================================= */

CLibInfoCharSvr::~CLibInfoCharSvr()
{
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/01/21														 */
/* ========================================================================= */

void CLibInfoCharSvr::Create(CMgrData *pMgrData)
{
	CLibInfoCharBase::Create ();

	m_paInfoLogin = new ARRAYINFOCHARBASE;

	m_pMgrData				= pMgrData;
	m_pMainFrame			= m_pMgrData->GetMainFrame ();
	m_pLibInfoMap			= m_pMgrData->GetLibInfoMap ();
	m_pLibInfoSkill			= m_pMgrData->GetLibInfoSkill ();
	m_pLibInfoItemType		= m_pMgrData->GetLibInfoItemType ();
	m_pLibInfoItem			= m_pMgrData->GetLibInfoItem ();
	m_pLibInfoItemWeapon	= m_pMgrData->GetLibInfoItemWeapon ();
	m_pSock					= (PCUraraSockTCPSBO)m_pMgrData->GetSock ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

void CLibInfoCharSvr::Destroy(void)
{
	CLibInfoCharBase::Destroy ();

	SAFE_DELETE (m_paInfoLogin);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::Proc											 */
/* ���e		:����															 */
/* �߂�l	:TRUE:�������� FALS:�������Ă��Ȃ�								 */
/* ���t		:2007/01/21														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::Proc(void)
{
	BOOL bRet, bResult;
	int i, nCount;

	bRet	= FALSE;
	bResult	= FALSE;

	nCount = GetCountLogIn ();
	/* �폜����邩������Ȃ��̂Ō�납�珈�� */
	for (i = nCount - 1; i >= 0; i --) {
		bResult |= ProcLocal (i);
	}
	if (bResult == FALSE) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetCountLogIn									 */
/* ���e		:���O�C�����L���������擾										 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

int CLibInfoCharSvr::GetCountLogIn(void)
{
	return m_paInfoLogin->GetSize ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetCountOnline								 */
/* ���e		:�I�����C������PC�����擾										 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

int CLibInfoCharSvr::GetCountOnline(DWORD dwMapID/*0*/)
{
	int i, nCount, nRet;
	PCInfoCharSvr pInfoChar;

	nRet = 0;

	if (m_paInfoLogin == NULL) {
		goto Exit;
	}

	nCount = m_paInfoLogin->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoChar = (PCInfoCharSvr)m_paInfoLogin->GetAt (i);
		if (pInfoChar->IsNPC ()) {
			continue;
		}
		if (dwMapID != 0) {
			if (pInfoChar->m_dwMapID != dwMapID) {
				continue;
			}
		}
		nRet ++;
	}

Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::LogIn											 */
/* ���e		:���O�C��														 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

void CLibInfoCharSvr::LogIn(
	DWORD dwCharID,			/* [in] �L����ID */
	DWORD dwSessionID,		/* [in] �ڑ���Z�b�V����ID */
	DWORD dwAccountID)		/* [in] �A�J�E���gID */
{
	PCInfoCharBase pChar, pCharTmp;

	pChar = (PCInfoCharBase)GetPtr (dwCharID);
	if (pChar == NULL) {
		return;
	}
	pCharTmp = GetPtrLogIn (dwCharID);
	if (pCharTmp) {
		return;
	}
	pChar->m_dwSessionID = dwSessionID;
	pChar->m_dwAccountID = dwAccountID;
	pChar->SetMoveState (CHARMOVESTATE_STAND);
	pChar->m_strSpeak.Empty ();
	pChar->m_ptCharBack.x = pChar->m_nMapX;
	pChar->m_ptCharBack.y = pChar->m_nMapY;
	m_paInfoLogin->Add (pChar);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::LogOut										 */
/* ���e		:���O�A�E�g														 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

void CLibInfoCharSvr::LogOut(DWORD dwCharID)
{
	BOOL bResult;
	int i, nCount;
	PCInfoCharSvr pCharTmp, pCharParent;
	CPacketCHAR_STATE Packet;

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pCharTmp = (PCInfoCharSvr)m_paInfoLogin->GetAt (i);
		if (pCharTmp->m_dwCharID == dwCharID) {
			break;
		}
	}
	if (i >= nCount) {
		/* ������Ȃ����� */
		return;
	}

	/* �t���čs���̂ƕt���ė��Ă���̂������������2��Ă� */
	Tail (pCharTmp, NULL, FALSE);
	Tail (pCharTmp, NULL, FALSE);
	pCharTmp->m_dwSessionID = 0;
	pCharTmp->m_dwAccountID = 0;
	pCharTmp->m_strSpeak.Empty ();

	/* ����̃L�����ɒʒm */
	Packet.Make (pCharTmp->m_dwCharID, CHARMOVESTATE_DELETE);
	m_pMainFrame->SendToScreenChar (pCharTmp, &Packet);

	pCharParent = (PCInfoCharSvr)GetPtrLogIn (pCharTmp->m_dwParentCharID);

	bResult = pCharTmp->IsNPC ();
	if (bResult) {
		if (pCharParent) {
			if (pCharParent->m_nMoveType == CHARMOVETYPE_PUTNPC) {
				pCharParent->DecPutCount ();
			}
		}

		bResult = pCharTmp->IsLogoutDelete ();
		/* ���O�A�E�g���ɍ폜�H */
		if (bResult) {
			Delete (pCharTmp->m_dwCharID);
		}

	} else {
		pCharTmp->SetMoveState (CHARMOVESTATE_STAND);
		pCharTmp->SetProcState (CHARPROCSTATEID_NORMAL);
	}
	m_paInfoLogin->RemoveAt (i);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::Revice										 */
/* ���e		:�f�[�^�̕␳													 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

void CLibInfoCharSvr::Revice(void)
{
	int i, j, nCount, nCount2, nNo;
	PCInfoCharSvr pInfoCharTmp, pInfoChar;
	PCInfoItem pInfoItem;
	ARRAYINFOCHARBASE aInfoLogin;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)GetPtr (i);

		/* �ړ���ʂɉ����ăN���X���X�V */
		pInfoChar = (PCInfoCharSvr)GetNew (pInfoCharTmp->m_nMoveType);
		pInfoChar->Copy (pInfoCharTmp);

		nNo = GetNoLogIn (pInfoChar->m_dwCharID);
		if (nNo >= 0) {
			m_paInfoLogin->SetAt (nNo, pInfoChar);
		}
		SAFE_DELETE (pInfoCharTmp);
		m_paInfo->SetAt (i, pInfoChar);

		if (pInfoChar->m_nMoveType != CHARMOVETYPE_PC) {
			pInfoChar->m_bNPC = TRUE;
			nNo = GetNoLogIn (pInfoChar->m_dwCharID);
			if (nNo < 0) {
				m_paInfoLogin->Add (pInfoChar);
			}
		}
		/* �s���A�C�e��ID�̐��� */
		nCount2 = pInfoChar->m_adwItemID.GetSize ();
		for (j = nCount2 - 1; j >= 0; j --) {
			pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (pInfoChar->m_adwItemID[j]);
			if (pInfoItem == NULL) {
				pInfoChar->m_adwItemID.RemoveAt (j);
			}
		}
//Todo:�}�b�v����̂͂ݏo���`�F�b�N
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::Move											 */
/* ���e		:1���i�߂�														 */
/* ���t		:2007/09/02														 */
/* ========================================================================= */

void CLibInfoCharSvr::Move(
	DWORD dwCharID,		/* [in] �L����ID */
	int nDirection,		/* [in] ���� */
	BOOL bTurn)			/* [in] TRUE:�Ԃ��鎞�ɔ��]���� */
{
	BOOL bResult, bEvent;
	int nTurnDirection;
	PCInfoCharSvr pInfoChar;
	POINT ptFront;

	pInfoChar = (PCInfoCharSvr)GetPtr (dwCharID);
	if (pInfoChar == NULL) {
		return;
	}

	bEvent = pInfoChar->IsNPC ();

	/* �Ԃ���H */
	bResult = !IsBlockChar (pInfoChar, nDirection);
	if (bResult) {
		/* �i�߂�H */
		bResult = IsMove (pInfoChar, nDirection, bEvent);
	}
	if (bResult == FALSE) {
		if (bTurn == FALSE) {
			return;
		}
		/* �t�����ɐi�߂邩�`�F�b�N */
		nTurnDirection = GetTurnDirection (nDirection);
		bResult = IsMove (pInfoChar, nTurnDirection, bEvent);
		if (bResult == FALSE) {
			/* ���]�ł��Ȃ� */
			return;
		}
		nDirection = nTurnDirection;
	}
	pInfoChar->GetFrontPos (ptFront, nDirection, TRUE);

	pInfoChar->SetDirection (nDirection);
	pInfoChar->SetPos (ptFront.x, ptFront.y);
	pInfoChar->m_bChgPos		= TRUE;
	pInfoChar->m_bChgUpdatePos	= TRUE;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::Tail											 */
/* ���e		:�t���čs���ݒ�E����											 */
/* �߂�l	:�t���čs������̃L����ID										 */
/* ���t		:2007/09/05														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::Tail(
	PCInfoCharBase pChar,			/* [in] �ΏۂƂȂ�L���� */
	PCInfoCharBase pCharTarget,		/* [in] �t���čs������(NULL�ŉ���) */
	BOOL bTail)						/* [in] TRUE:���Ă��� FALSE:���� */
{
	BOOL bResult;
	DWORD dwRet, dwSessionID;
	POINT ptFrontPos;
	PCInfoCharBase pCharTmp;
	PCInfoMapBase pInfoMap;
	CPacketCHAR_RES_TAIL Packet;
	SIZE size;

	dwRet = 0;
	dwSessionID = 0;

	if (pChar->m_dwFrontCharID) {
		pCharTarget = NULL;
	}

	/* �����H */
	if (bTail == FALSE) {
		if (pChar->m_dwFrontCharID) {
			pCharTmp = (PCInfoCharBase)GetPtrLogIn (pChar->m_dwFrontCharID);
			if (pCharTmp) {
				pCharTmp->m_dwTailCharID = 0;
			}
			Packet.Make (pChar->m_dwCharID, 0, RES_TAIL_RESULT_PARGE);
			m_pMainFrame->SendToClient (pChar->m_dwSessionID, &Packet);

			Packet.Make (pChar->m_dwCharID, pCharTmp->m_dwCharID, RES_TAIL_RESULT_PARGE);
			m_pMainFrame->SendToClient (pCharTmp->m_dwSessionID, &Packet);

			pChar->m_dwFrontCharID = 0;

		/* �t���ė��Ă���L���������� */
		} else {
			if (pChar->m_dwTailCharID) {
				pCharTmp = (PCInfoCharBase)GetPtrLogIn (pChar->m_dwTailCharID);
				if (pCharTmp) {
					pCharTmp->m_dwFrontCharID = 0;
					Packet.Make (pCharTmp->m_dwCharID, 0, RES_TAIL_RESULT_PARGE);
					m_pMainFrame->SendToClient (pCharTmp->m_dwSessionID, &Packet);
				}
				pChar->m_dwTailCharID = 0;

				Packet.Make (pChar->m_dwCharID, 0, RES_TAIL_RESULT_TAILPARGE);
				m_pMainFrame->SendToClient (pChar->m_dwSessionID, &Packet);
			}
		}
		goto Exit;
	}

	/*****************
		�t���čs��
	*****************/

	if (pCharTarget == NULL) {
		goto Exit;
	}
	/* ���ɕt���čs���Ă���H */
	if (pChar->m_dwFrontCharID) {
		goto Exit;
	}
	if ((pCharTarget->m_nMoveState == CHARMOVESTATE_SWOON) || (pChar->m_nMoveState == CHARMOVESTATE_SWOON)) {
		Packet.Make (pChar->m_dwCharID, pCharTarget->m_dwCharID, RES_TAIL_RESULT_NG_SWOON);
		m_pMainFrame->SendToClient (pChar->m_dwSessionID, &Packet);
		goto Exit;
	}
	/* �퓬���[�h�H */
	bResult = pCharTarget->IsStateBattle ();
	if (bResult) {
		Packet.Make (pChar->m_dwCharID, pCharTarget->m_dwCharID, RES_TAIL_RESULT_NG_STATE);
		m_pMainFrame->SendToClient (pChar->m_dwSessionID, &Packet);
		goto Exit;
	}
	bResult = pCharTarget->IsLogin ();
	/* ���O�C�����Ă��Ȃ��H */
	if (bResult == FALSE) {
		goto Exit;
	}
	/* NPC�H */
	if (pCharTarget->m_bNPC) {
		goto Exit;
	}
	/* �}�b�v���Ⴄ�H */
	if (pCharTarget->m_dwMapID != pChar->m_dwMapID) {
		goto Exit;
	}
	/* �}�b�v�C�x���g�̏�ɂ���H */
	bResult = CheckMapEvent ((PCInfoCharSvr)pCharTarget, TRUE);
	if (bResult) {
		Packet.Make (pChar->m_dwCharID, pCharTarget->m_dwCharID, RES_TAIL_RESULT_NG_PLACE, 0);
		m_pMainFrame->SendToClient (pChar->m_dwSessionID, &Packet);
		goto Exit;
	}

	pChar->GetFrontPos (ptFrontPos, -1, TRUE);
	pChar->GetCharSize (size);
	/* �ڂ̑O�ɂ��Ȃ��H */
	if (pCharTarget->IsHitCharPos (ptFrontPos.x, ptFrontPos.y, &size) == FALSE) {
		goto Exit;
	}
	/* ���Ɉ��������Ă���H */
	if (pCharTarget->m_dwTailCharID) {
		Packet.Make (pChar->m_dwCharID, pCharTarget->m_dwCharID, RES_TAIL_RESULT_NG_TAIL, pCharTarget->m_dwTailCharID);
		m_pMainFrame->SendToClient (pChar->m_dwSessionID, &Packet);
		goto Exit;
	}
	/* ���Ɏ����Ɍq�����Ă��邩�`�F�b�N */
	if (pCharTarget->m_dwFrontCharID) {
		bResult		= FALSE;
		pCharTmp	= pCharTarget;
		while (1) {
			pCharTmp = (PCInfoCharBase)GetPtrLogIn (pCharTmp->m_dwFrontCharID);
			if (pCharTmp == NULL) {
				break;
			}
			if (pCharTmp->m_dwCharID == pChar->m_dwCharID) {
				bResult = TRUE;
				break;
			}
		}
		if (bResult) {
			goto Exit;
		}
	}
	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}

	bResult = IsMove (pChar, pChar->m_nDirection);
	if (bResult == FALSE) {
		goto Exit;
	}

	/* ���ꂼ��̃L����ID��ݒ� */
	pChar->m_dwFrontCharID		= pCharTarget->m_dwCharID;
	pCharTarget->m_dwTailCharID	= pChar->m_dwCharID;
	Packet.Make (pChar->m_dwCharID, pCharTarget->m_dwCharID, RES_TAIL_RESULT_OK);
	m_pMainFrame->SendToClient (pChar->		 m_dwSessionID, &Packet);
	m_pMainFrame->SendToClient (pCharTarget->m_dwSessionID, &Packet);

	dwRet = pCharTarget->m_dwCharID;
Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::Atack											 */
/* ���e		:�U��															 */
/* ���t		:2007/09/11														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::Atack(PCInfoCharSvr pChar)
{
	int i, nCount;
	BOOL bRet, bHitQuit;
	DWORD dwCharID;
	ARRAYDWORD adwCharID;
	PCInfoCharMOVEATACKSvr pInfoCharTmp;

	bRet = FALSE;

	if (pChar == NULL) {
		return FALSE;
	}

	if (pChar->m_nDirection >= 4) {
		/* �΂ߕ����ɂ͍U�����肵�Ȃ� */
		goto Exit;
	}
	bHitQuit = TRUE;
	switch (pChar->m_nMoveType) {
	case CHARMOVETYPE_MOVEATACK:		/* �ړ����čU�� */
		pInfoCharTmp = (PCInfoCharMOVEATACKSvr)pChar;
		bHitQuit = pInfoCharTmp->m_bHitQuit;
		break;
	}

	if (bHitQuit) {
		dwCharID = GetFrontCharIDTarget (pChar->m_dwCharID, -1, 1);
		/* ����O�ɂ��Ȃ��H */
		if (dwCharID == 0) {
			goto Exit;
		}
		bRet = AtackImple (pChar, dwCharID);

	} else {
		GetFrontCharIDTarget (pChar->m_dwCharID, -1, 1, &adwCharID);
		nCount = adwCharID.GetSize ();
		for (i = 0; i < nCount; i ++) {
			bRet |= AtackImple (pChar, adwCharID[i]);
		}
	}

Exit:
	pChar->SetMoveState (CHARMOVESTATE_BATTLE);

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::Atack											 */
/* ���e		:�U��������														 */
/* ���t		:2009/05/13														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::AtackImple(PCInfoCharSvr pChar, DWORD dwCharID)
{
	int nTmp, nReserveChgEfect;
	BOOL bRet, bResult, bChargeAtack, bCritical;
	DWORD dwPoint;
	PCInfoCharSvr pCharTarget, pCharTmp, pCharParent;
	CPacketCHAR_TEXTEFFECT PacketCHAR_TEXTEFFECT;

	bRet = FALSE;
	pCharTmp = NULL;

	pCharTarget = (PCInfoCharSvr)GetPtrLogIn (dwCharID);
	if (pCharTarget == NULL) {
		goto Exit;
	}
	switch (pChar->m_nAtackTarget) {
	case ATACKTARGETTYPE_NONE:			/* ���ݒ� */
		if (IsNPC (pChar) && IsNPC (pCharTarget)) {
			goto Exit;
		}
		break;
	case ATACKTARGETTYPE_PC:			/* �v���C���[ */
		if (IsNPC (pCharTarget)) {
			goto Exit;
		}
		break;
	case ATACKTARGETTYPE_NPC:			/* NPC */
		if (IsNPC (pCharTarget) == FALSE) {
			goto Exit;
		}
		break;
	case ATACKTARGETTYPE_ALL:			/* �S�� */
		break;
	}

	bResult = pChar->IsEnableAtack ();
	if (bResult == FALSE) {
		goto Exit;
	}
	bResult = pCharTarget->IsAtackTarget ();
	if (bResult == FALSE) {
		goto Exit;
	}
	pCharParent = pCharTmp = pChar;
	if (pChar->m_dwParentCharID) {
		pCharParent = (PCInfoCharSvr)GetPtrLogIn (pChar->m_dwParentCharID);
		if (pChar->m_bParentInfo) {
			/* �e������ꍇ�͐e�̏����g�p���� */
			pCharTmp = pCharParent;
		}
	}

	bChargeAtack = pChar->m_bChargeAtack;
	pChar->ProcAtack ();

	bResult = IsHitAtack (pCharTmp, pCharTarget);
	/* �U�����s�H */
	if (bResult == FALSE) {
		/* �����G�t�F�N�g��ʒm */
		PacketCHAR_TEXTEFFECT.Make (pCharTarget->m_dwCharID, (DWORD)0, 5, TEXTEFFECTNUMSIZE_SMALL);
		m_pMainFrame->SendToScreenChar (pCharTarget, &PacketCHAR_TEXTEFFECT);
		bRet = TRUE;
		goto Exit;
	}

	bResult = pCharTarget->ProcHit (pCharParent);
	if (bResult == FALSE) {
		bRet = TRUE;
		goto Exit;
	}

	nReserveChgEfect = 0;
	bCritical = FALSE;
	nTmp = pCharTmp->m_wPCriticalAverage;
	if (nTmp == 0) {
		/* ���w�莞��5%�ɂ��Ă��� */
		nTmp = 5;
	}
	if (genrand () % 100 >= 100 - nTmp) {
		bCritical = TRUE;
	}

	dwPoint = GetAtackDamage (pCharTmp, pCharTarget);
	if (bChargeAtack) {
		/* ���ߍU���͈З�2�{ */
		dwPoint *= 2;
	}
	if (bCritical) {
		/* �N���e�B�J���͈З�2�{ */
		dwPoint *= 2;
	}

	nReserveChgEfect = GetAtackEffectID (pCharTmp, bCritical);

	Damage (pChar, pCharTarget, dwPoint, nReserveChgEfect, bCritical);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::Equip											 */
/* ���e		:����															 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::Equip(CInfoCharSvr *pChar, DWORD dwItemID)
{
	BOOL bRet, bResult;
	DWORD dwItemType, dwEquipItemID, dwEquipItemIDBack;
	PCInfoItem pInfoItem;
	PCInfoItemTypeBase pInfoItemType;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;

	bRet = FALSE;
	dwEquipItemID		= dwItemID;
	dwEquipItemIDBack	= 0;

	bResult = pChar->HaveItem (dwItemID);
	if (bResult == FALSE) {
		goto Exit;
	}
	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		goto Exit;
	}
	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (pInfoItem->m_dwItemTypeID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}

	dwItemType = m_pLibInfoItem->GetItemType (pInfoItem->m_dwItemID);
	switch (dwItemType) {
	case ITEMTYPEID_CLOTH:		/* �� */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDCloth;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDCloth, pInfoItem->m_dwItemID);
		pInfoItemType->SetGrpID (&pChar->m_wGrpIDCloth, &pChar->m_wGrpIDSP);
		pChar->m_dwEquipItemIDCloth = dwItemID;
		pChar->m_bChgGrp = TRUE;
		break;
	case ITEMTYPEID_ACCE:		/* �A�N�Z�T�� */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDAcce1;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDAcce1, pInfoItem->m_dwItemID);
		pInfoItemType->SetGrpID (&pChar->m_wGrpIDAcce, NULL);
		pChar->m_dwEquipItemIDAcce1 = dwItemID;
		pChar->m_bChgGrp = TRUE;
		break;
	case ITEMTYPEID_ARMS:		/* ������ */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDArmsRight;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDArmsRight, pInfoItem->m_dwItemID);
		pInfoItemType->SetGrpID (&pChar->m_wGrpIDArmsMain, &pChar->m_wGrpIDArmsSub);
		pChar->m_dwEquipItemIDArmsRight = dwItemID;
		pChar->m_bChgGrp = TRUE;
		break;
	case ITEMTYPEID_SHIELD:		/* �� */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDArmsLeft;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDArmsLeft, pInfoItem->m_dwItemID);
		pInfoItemType->SetGrpID (&pChar->m_wGrpIDArmsLeftMain, &pChar->m_wGrpIDArmsLeftSub);
		pChar->m_dwEquipItemIDArmsLeft = dwItemID;
		pChar->m_bChgGrp = TRUE;
		break;
	default:
		goto Exit;
	}

	if (pChar->m_dwSessionID == 0) {
		bRet = TRUE;
		goto Exit;
	}

	/* �ύX���ꂽ�A�C�e������ʒm */
	if (dwEquipItemID) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwEquipItemID);
		if (pInfoItem) {
			PacketITEM_RES_ITEMINFO.Make (pInfoItem);
			m_pMainFrame->SendToScreenChar (pChar, &PacketITEM_RES_ITEMINFO);
		}
	}
	if (dwEquipItemIDBack) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwEquipItemIDBack);
		if (pInfoItem) {
			PacketITEM_RES_ITEMINFO.Make (pInfoItem);
			m_pMainFrame->SendToScreenChar (pChar, &PacketITEM_RES_ITEMINFO);
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::UnEquip										 */
/* ���e		:��������														 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::UnEquip(CInfoCharSvr *pChar, DWORD dwItemID)
{
	BOOL bRet, bResult;
	DWORD dwItemType, dwEquipItemID, dwEquipItemIDBack;
	PCInfoItem pInfoItem;
	PCInfoItemTypeBase pInfoItemType;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;

	bRet = FALSE;
	dwEquipItemID		= dwItemID;
	dwEquipItemIDBack	= 0;

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		goto Exit;
	}
	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (pInfoItem->m_dwItemTypeID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}
	bResult = pChar->IsItemAdd ();
	/* �O�����A�C�e�������ĂȂ��H */
	if (bResult == FALSE) {
		SendFormatMsg (pChar->m_dwSessionID, FORMATMSGID_NOUNEQUIP_ITEMFULL, 0, 0, FORMATMSGTYPE_NOLOG);
		goto Exit;
	}

	dwItemType = m_pLibInfoItem->GetItemType (pInfoItem->m_dwItemID);
	switch (dwItemType) {
	case ITEMTYPEID_CLOTH:		/* �� */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDCloth;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDCloth, 0);
		/* �����摜�ɖ߂� */
		pChar->m_wGrpIDCloth		= pChar->m_wGrpIDInitCloth;
		pChar->m_wGrpIDSP			= pChar->m_wGrpIDInitSP;
		pChar->m_dwEquipItemIDCloth	= 0;
		pChar->m_bChgGrp			= TRUE;
		break;
	case ITEMTYPEID_ACCE:		/* �A�N�Z�T�� */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDAcce1;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDAcce1, 0);
		/* �����摜�ɖ߂� */
		pChar->m_wGrpIDAcce			= 0;
		pChar->m_dwEquipItemIDAcce1	= 0;
		pChar->m_bChgGrp			= TRUE;
		break;
	case ITEMTYPEID_ARMS:		/* ������ */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDArmsRight;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDArmsRight, 0);
		/* �����摜�ɖ߂� */
		pChar->m_wGrpIDArmsMain			= 0;
		pChar->m_wGrpIDArmsSub			= 0;
		pChar->m_dwEquipItemIDArmsRight	= 0;
		pChar->m_bChgGrp = TRUE;
		break;
	case ITEMTYPEID_SHIELD:		/* �� */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDArmsLeft;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDArmsLeft, 0);
		/* �����摜�ɖ߂� */
		pChar->m_wGrpIDArmsLeftMain		= 0;
		pChar->m_wGrpIDArmsLeftSub		= 0;
		pChar->m_dwEquipItemIDArmsLeft	= 0;
		pChar->m_bChgGrp = TRUE;
		break;
	default:
		goto Exit;
	}

	if (pChar->m_dwSessionID == 0) {
		bRet = TRUE;
		goto Exit;
	}

	/* �ύX���ꂽ�A�C�e������ʒm */
	if (dwEquipItemID) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwEquipItemID);
		if (pInfoItem) {
			PacketITEM_RES_ITEMINFO.Make (pInfoItem);
			m_pMainFrame->SendToClient (pChar->m_dwSessionID, &PacketITEM_RES_ITEMINFO);
		}
	}
	if (dwEquipItemIDBack) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwEquipItemIDBack);
		if (pInfoItem) {
			PacketITEM_RES_ITEMINFO.Make (pInfoItem);
			m_pMainFrame->SendToClient (pChar->m_dwSessionID, &PacketITEM_RES_ITEMINFO);
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::UseItem										 */
/* ���e		:�A�C�e���g�p													 */
/* ���t		:2008/07/05														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::UseItem(CInfoCharSvr *pChar, DWORD dwItemID)
{
	int nResult;
	BOOL bRet, bResult;
	DWORD dwTypeID;
	CPacketCHAR_ITEMINFO PacketCHAR_ITEMINFO;
	CPacketMAP_FORMATMSG PacketMsg;

	bRet = FALSE;
	if (pChar == NULL) {
		goto Exit;
	}
	bResult = pChar->HaveItem (dwItemID);
	if (bResult == FALSE) {
		goto Exit;
	}
	/* �g�p�ł���A�C�e�������� */
	bResult = m_pLibInfoItem->IsUseItem (dwItemID);
	if (bResult == FALSE) {
		SendFormatMsg (pChar->m_dwSessionID, FORMATMSGID_NOUSE, dwItemID, 0, FORMATMSGTYPE_NOLOG);
		goto Exit;
	}

	dwTypeID = m_pLibInfoItem->GetItemType (dwItemID);
	switch (dwTypeID) {
	case ITEMTYPEID_HP:		/* HP���� */
		nResult = UseItemProcHP (pChar, dwItemID);
		break;
	case ITEMTYPEID_LIGHT:	/* ���� */
		nResult = UseItemProcLIGHT (pChar, dwItemID);
		break;
	}
	switch (nResult) {
	case 0:		/* �g��Ȃ����� */
		SendFormatMsg (pChar->m_dwSessionID, FORMATMSGID_NOUSE, dwItemID, 0, FORMATMSGTYPE_NOLOG);
		break;
	case 1:		/* �g�����̂ō폜���� */
		pChar->DeleteItem (dwItemID);
		m_pLibInfoItem->DeleteItem (dwItemID, pChar);
		m_pLibInfoItem->Delete (dwItemID);
		PacketCHAR_ITEMINFO.Make (pChar->m_dwCharID, &pChar->m_adwItemID);
		m_pMainFrame->SendToClient (pChar->m_dwSessionID, &PacketCHAR_ITEMINFO);
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::DragItem										 */
/* ���e		:�A�C�e���ʒu�ύX												 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

void CLibInfoCharSvr::DragItem(CInfoCharSvr *pChar, DWORD dwItemID, POINT ptNewPos)
{
	int i, nCount;
	PCInfoItem pInfoItem, pInfoItemTmp, pInfoItemSrc;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;

	if (pChar == NULL) {
		return;
	}
	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	pInfoItemSrc = NULL;
	nCount = pChar->m_adwItemID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoItemTmp = (PCInfoItem)m_pLibInfoItem->GetPtr (pChar->m_adwItemID[i]);
		if (pInfoItemTmp == NULL) {
			continue;
		}
		if (!((pInfoItemTmp->m_ptBackPack.x == ptNewPos.x) && (pInfoItemTmp->m_ptBackPack.y == ptNewPos.y))) {
			continue;
		}
		/* ����ւ���A�C�e���𔭌� */
		pInfoItemSrc = pInfoItemTmp;
		break;
	}
	if (pInfoItemSrc) {
		pInfoItemSrc->m_ptBackPack.x = pInfoItem->m_ptBackPack.x;
		pInfoItemSrc->m_ptBackPack.y = pInfoItem->m_ptBackPack.y;
	}
	pInfoItem->m_ptBackPack.x = ptNewPos.x;
	pInfoItem->m_ptBackPack.y = ptNewPos.y;
	PacketITEM_RES_ITEMINFO.Make (pInfoItem);
	m_pSock->SendTo (pChar->m_dwSessionID, &PacketITEM_RES_ITEMINFO);
	if (pInfoItemSrc) {
		PacketITEM_RES_ITEMINFO.Make (pInfoItemSrc);
		m_pSock->SendTo (pChar->m_dwSessionID, &PacketITEM_RES_ITEMINFO);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::RenewGrpID									 */
/* ���e		:�����摜ID���X�V												 */
/* ���t		:2007/12/22														 */
/* ========================================================================= */

void CLibInfoCharSvr::RenewGrpID(DWORD dwCharID)
{
	int i, nCount;
	PCInfoCharBase pInfoChar;
	PCInfoItemTypeBase pInfoItemType;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (dwCharID == 0) {
			pInfoChar = (PCInfoCharBase)GetPtr (i);
		} else {
			pInfoChar = (PCInfoCharBase)GetPtr (dwCharID);
			if (pInfoChar == NULL) {
				break;
			}
			i = nCount;
		}

		/* �� */
		pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDCloth);
		if (pInfoItemType) {
			pInfoItemType->SetGrpID (&pInfoChar->m_wGrpIDCloth, &pInfoChar->m_wGrpIDSP);
		}
		/* �A�N�Z�T�� */
		pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDAcce1);
		if (pInfoItemType) {
			pInfoItemType->SetGrpID (&pInfoChar->m_wGrpIDAcce, NULL);
		}
		/* ������ */
		pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDArmsRight);
		if (pInfoItemType) {
			pInfoItemType->SetGrpID (&pInfoChar->m_wGrpIDArmsMain, &pInfoChar->m_wGrpIDArmsSub);
		}
		/* �� */
		pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDArmsLeft);
		if (pInfoItemType) {
			pInfoItemType->SetGrpID (&pInfoChar->m_wGrpIDArmsLeftMain, &pInfoChar->m_wGrpIDArmsLeftSub);
		}
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetPlaceName									 */
/* ���e		:�ł��l���W�܂��Ă���}�b�v�����擾								 */
/* ���t		:2008/05/23														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::GetPlaceName(CmyString &strDst)
{
	int i, nCount, nNo;
	PCInfoCharSvr pInfoChar;
	PCInfoMapBase pInfoMap;
	ARRAYINT anMapLoginCount;

	strDst.Empty ();

	nCount = m_pLibInfoMap->GetCount ();
	if (nCount == 0) {
		return 0;
	}
	for (i = 0; i < nCount; i ++) {
		anMapLoginCount.Add (0);
	}

	/* �e�}�b�v�̐l�����W�v */
	nCount = m_paInfoLogin->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoChar = (PCInfoCharSvr)m_paInfoLogin->GetAt (i);
		if (pInfoChar->IsNPC ()) {
			continue;
		}
		if ((int)pInfoChar->m_dwMapID - 1 >= anMapLoginCount.GetSize ()) {
			continue;
		}
		anMapLoginCount[pInfoChar->m_dwMapID - 1] ++;
	}

	nNo = 0;
	nCount = m_pLibInfoMap->GetCount ();
	for (i = 1; i < nCount; i ++) {
		if (anMapLoginCount[i] > anMapLoginCount[nNo]) {
			nNo = i;
		}
	}
	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (nNo);
	strDst = pInfoMap->m_strMapName;

	return pInfoMap->m_dwMapID;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::MoveMapIn										 */
/* ���e		:�}�b�v���ړ�													 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

void CLibInfoCharSvr::MoveMapIn(CInfoCharSvr *pInfoChar)
{
	PBYTE pTmp;
	DWORD dwSessionID;
	CLibInfoCharSvr LibInfoCharTmp;
	CPacketCHAR_CHARINFO PacketCHAR_CHARINFO;
	CPacketCHAR_RES_CHARINFO PacketCHAR_RES_CHARINFO;
	CPacketCHAR_PARA1 PacketCHAR_PARA1;
	CPacketMAP_PARA1 PacketMAP_PARA1;

	dwSessionID = pInfoChar->m_dwSessionID;
	if (dwSessionID == 0) {
		return;
	}

	/* ����̃L�������𑗐M */
	LibInfoCharTmp.Create (m_pMgrData);
	pTmp = GetDataScreen (pInfoChar);
	LibInfoCharTmp.SetSendData (pTmp);
	PacketCHAR_CHARINFO.Make (&LibInfoCharTmp);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_CHARINFO);

	/* ����̃L�����ɒʒm */
	PacketCHAR_RES_CHARINFO.Make (pInfoChar);
	m_pMainFrame->SendToScreenChar (pInfoChar, &PacketCHAR_RES_CHARINFO);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_RES_CHARINFO);

	pInfoChar->m_bStateFadeInOut = FALSE;
	PacketMAP_PARA1.Make (SBOCOMMANDID_SUB_MAP_FADEINOUT, pInfoChar->m_dwMapID, 0);
	m_pSock->SendTo (dwSessionID, &PacketMAP_PARA1);

	PacketCHAR_PARA1.Make (SBOCOMMANDID_SUB_CHAR_RES_CHECKMAPEVENT, pInfoChar->m_dwCharID, 0);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_PARA1);

	SAFE_DELETE_ARRAY (pTmp);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::MoveMapOut									 */
/* ���e		:�}�b�v�O�ړ�													 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

void CLibInfoCharSvr::MoveMapOut(CInfoCharSvr *pInfoChar)
{
	PCInfoMapBase pInfoMap;
	CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;
	CPacketMAP_RES_MAPINFO PacketRES_MAPINFO;
	CPacketITEM_ITEMINFO PacketITEM_ITEMINFO;

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}
	if (pInfoMap->IsEnableBattle () == FALSE) {
		if (pInfoChar->IsStateBattle ()) {
			pInfoChar->SetMoveState (CHARMOVESTATE_STAND);
		}
	}

	PacketRES_CHARINFO.Make (pInfoChar);
	m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketRES_CHARINFO);

	PacketRES_MAPINFO.Make (pInfoMap);
	m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketRES_MAPINFO);
	PacketITEM_ITEMINFO.Make (m_pLibInfoItem);
	m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketITEM_ITEMINFO);

	MoveMapIn (pInfoChar);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::SetInitStatus									 */
/* ���e		:�X�e�[�^�X�����l�ݒ�											 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

void CLibInfoCharSvr::SetInitStatus(CInfoCharSvr *pInfoChar, BOOL bInitPos/*FALSE*/)
{
	PCInfoSystem pInfoSystem;
	PSTSYSTEM_INITCHARSTATUS pInitCharStatus;

	pInfoSystem		= (PCInfoSystem)m_pMgrData->GetLibInfoSystem ()->GetPtr ();
	pInitCharStatus	= pInfoSystem->m_pInitCharStatus;

	pInfoChar->m_wStamina			= pInitCharStatus->wStamina;			/* �X�^�~�i */
	pInfoChar->m_wPower				= pInitCharStatus->wPower;				/* �r�� */
	pInfoChar->m_wStrength			= pInitCharStatus->wStrength;			/* �̗� */
	pInfoChar->m_wMagic				= pInitCharStatus->wMagic;				/* ���� */
	pInfoChar->m_wSkillful			= pInitCharStatus->wSkillful;			/* ��p */
	pInfoChar->m_wAbillityAT		= pInitCharStatus->wAbillityAT;			/* �U���Z�\ */
	pInfoChar->m_wAbillityDF		= pInitCharStatus->wAbillityDF;			/* �h��Z�\ */
	pInfoChar->m_wPAtack			= pInitCharStatus->wPAtack;				/* �U���� */
	pInfoChar->m_wPDefense			= pInitCharStatus->wPDefense;			/* �h��� */
	pInfoChar->m_wPMagic			= pInitCharStatus->wPMagic;				/* ���@�� */
	pInfoChar->m_wPMagicDefense		= pInitCharStatus->wPMagicDefense;		/* ���@�h��� */
	pInfoChar->m_wPHitAverage		= pInitCharStatus->wPHitAverage;		/* ������ */
	pInfoChar->m_wPAvoidAverage		= pInitCharStatus->wPAvoidAverage;		/* ��� */
	pInfoChar->m_wPCriticalAverage	= pInitCharStatus->wPCriticalAverage;	/* �N���e�B�J���� */
	pInfoChar->m_wAttrFire			= pInitCharStatus->wAttrFire;			/* ����[��] */
	pInfoChar->m_wAttrWind			= pInitCharStatus->wAttrWind;			/* ����[��] */
	pInfoChar->m_wAttrWater			= pInitCharStatus->wAttrWater;			/* ����[��] */
	pInfoChar->m_wAttrEarth			= pInitCharStatus->wAttrEarth;			/* ����[�y] */
	pInfoChar->m_wAttrLight			= pInitCharStatus->wAttrLight;			/* ����[��] */
	pInfoChar->m_wAttrDark			= pInitCharStatus->wAttrDark;			/* ����[��] */
	pInfoChar->m_dwMaxHP			= pInitCharStatus->dwMaxHP;				/* �ő�HP */
	pInfoChar->m_dwMaxSP			= pInitCharStatus->dwMaxSP;				/* �ő�SP */

	if (bInitPos) {
		pInfoChar->m_dwMapID		= pInitCharStatus->dwInitPosMapID;		/* �}�b�vID */
		pInfoChar->m_nMapX			= pInitCharStatus->ptInitPos.x;			/* X���W */
		pInfoChar->m_nMapY			= pInitCharStatus->ptInitPos.y;			/* Y���W */
	}

	pInfoChar->m_dwHP = min (pInfoChar->m_dwHP, pInfoChar->m_dwMaxHP);
	pInfoChar->m_dwSP = min (pInfoChar->m_dwSP, pInfoChar->m_dwMaxSP);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::AddNPC										 */
/* ���e		:NPC�̒ǉ�														 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

PCInfoCharBase CLibInfoCharSvr::AddNPC(CInfoCharBase *pInfoChar)
{
	PCInfoCharSvr pInfoCharTmp;

	pInfoCharTmp = (PCInfoCharSvr)GetNew (pInfoChar->m_nMoveType);
	pInfoCharTmp->Copy (pInfoChar);

	/* �����ݒ� */
	pInfoCharTmp->m_dwCharID	= 0;
	pInfoCharTmp->m_bNPC		= TRUE;
	pInfoCharTmp->m_bChgPos		= TRUE;
	pInfoCharTmp->m_nMoveType	= pInfoChar->m_nMoveType;

	Add (pInfoCharTmp);
	LogIn (pInfoCharTmp->m_dwCharID, 0, 0);

	return (PCInfoCharBase)pInfoCharTmp;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2007/01/21														 */
/* ========================================================================= */

PCInfoBase CLibInfoCharSvr::GetNew(int nType)
{
	PCInfoBase pNew;

	switch (nType) {
	case CHARMOVETYPE_ATACKANIME:	pNew = (PCInfoBase)new CInfoCharATACKANIMESvr;	break;	/* �U���󂯂�ƃA�j���[�V���� */
	case CHARMOVETYPE_MOVEATACK:	pNew = (PCInfoBase)new CInfoCharMOVEATACKSvr;	break;	/* �ړ����čU�� */
	case CHARMOVETYPE_BATTLE1:		pNew = (PCInfoBase)new CInfoCharBATTLE1Svr;		break;	/* �퓬1 */
	case CHARMOVETYPE_BATTLE2:		pNew = (PCInfoBase)new CInfoCharBATTLE2Svr;		break;	/* �퓬2 */
	case CHARMOVETYPE_PUTNPC:		pNew = (PCInfoBase)new CInfoCharPUTNPC;			break;	/* NPC���� */
	default:						pNew = (PCInfoBase)new CInfoCharSvr;			break;
	}

	return pNew;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::SetPtr										 */
/* ���e		:�L���������X�V												 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

void CLibInfoCharSvr::SetPtr(DWORD dwCharID, PCInfoCharBase pChar)
{
	int i, nCount;
	PCInfoCharBase pTmp;

	/* ��Ƀ��O�C������������������ */
	nCount = m_paInfoLogin->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pTmp = (PCInfoCharBase)m_paInfoLogin->GetAt (i);
		if (pTmp->m_dwCharID == dwCharID) {
			m_paInfoLogin->SetAt (i, pChar);
			break;
		}
	}

	CLibInfoCharBase::SetPtr (dwCharID, pChar);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetPtrSessionID								 */
/* ���e		:�L���������擾(�Z�b�V����ID����)								 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

PCInfoCharBase CLibInfoCharSvr::GetPtrSessionID(DWORD dwSessionID)
{
	int i, nCount;
	PCInfoCharBase pRet, pInfoTmp;

	pRet = NULL;

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfoLogin->GetAt (i);
		if (pInfoTmp->m_dwSessionID == dwSessionID) {
			pRet = pInfoTmp;
			break;
		}
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetPtrLogIn									 */
/* ���e		:���O�C�����L���������擾										 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

PCInfoCharBase CLibInfoCharSvr::GetPtrLogIn(int nNo)
{
	int nCount;
	PCInfoCharBase pRet;

	pRet = NULL;

	nCount = GetCountLogIn ();
	if (nNo >= nCount) {
		goto Exit;
	}
	pRet = m_paInfoLogin->GetAt (nNo);

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetPtrLogIn									 */
/* ���e		:���O�C�����L���������擾										 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

PCInfoCharBase CLibInfoCharSvr::GetPtrLogIn(DWORD dwCharID)
{
	int i, nCount;
	PCInfoCharBase pRet, pCharTmp;

	pRet = NULL;
	if (dwCharID == 0) {
		goto Exit;
	}

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pCharTmp = m_paInfoLogin->GetAt (i);
		if (pCharTmp->m_dwCharID == dwCharID) {
			pRet = pCharTmp;
			break;
		}
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetPtrParent									 */
/* ���e		:�e�L���������擾												 */
/* ���t		:2008/07/19														 */
/* ========================================================================= */

PCInfoCharBase CLibInfoCharSvr::GetPtrParent(CInfoCharSvr *pChar)
{
	PCInfoCharBase pRet;

	if (pChar->m_dwParentCharID == 0) {
		goto Exit;
	}

	pRet = GetPtrLogIn (pChar->m_dwParentCharID);
Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetNoLogIn									 */
/* ���e		:���O�C�����L�����̔z��ԍ����擾								 */
/* ���t		:2007/09/02														 */
/* ========================================================================= */

int CLibInfoCharSvr::GetNoLogIn(DWORD dwCharID)
{
	int i, nCount, nRet;
	PCInfoCharBase pInfoCharTmp;

	nRet = -1;

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfoLogin->GetAt (i);
		if (pInfoCharTmp->m_dwCharID == dwCharID) {
			nRet = i;
			break;
		}
	}

	return nRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetScreenCharID								 */
/* ���e		:�w��L�����̉�ʓ��ɂ���L����ID���擾							 */
/* ���t		:2007/02/12														 */
/* ========================================================================= */

void CLibInfoCharSvr::GetScreenCharID(
	CInfoCharSvr *pInfoChar,		/* [in] ��ƂȂ�L������� */
	ARRAYDWORD &aDst)				/* [in] �o�͐� */
{
	int i, nCount;
	PCInfoCharSvr pInfoCharTmp;

	aDst.RemoveAll ();

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)GetPtrLogIn (i);
		/* �ʃ}�b�v�H */
		if (pInfoCharTmp->m_dwMapID != pInfoChar->m_dwMapID) {
			continue;
		}
		/* ��ʊO�H */
		if (!((abs (pInfoCharTmp->m_nMapX - pInfoChar->m_nMapX) < DRAW_PARTS_X * 2) &&
			(abs (pInfoCharTmp->m_nMapY - pInfoChar->m_nMapY) < DRAW_PARTS_Y))) {
			continue;
		}
		aDst.Add (pInfoCharTmp->m_dwCharID);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetScreenCharIDLineOut						 */
/* ���e		:�w��L�����̉�ʒ[�ɂ���L����ID���擾							 */
/* ���t		:2007/02/13														 */
/* ========================================================================= */

void CLibInfoCharSvr::GetScreenCharIDLineOut(
	CInfoCharSvr *pInfoChar,		/* [in] ��ƂȂ�L������� */
	ARRAYDWORD &aDst)				/* [in] �o�͐� */
{
	int i, nCount, x, y,
		aPosX[] = {0, 0, DRAW_PARTS_X * 2 * -1, DRAW_PARTS_X * 2},
		aPosY[] = {DRAW_PARTS_Y * 2 * -1, DRAW_PARTS_Y * 2, 0, 0};
	PCInfoCharSvr pInfoCharTmp;

	aDst.RemoveAll ();

	x = pInfoChar->m_nMapX + aPosX[pInfoChar->m_nDirection];
	y = pInfoChar->m_nMapY + aPosY[pInfoChar->m_nDirection];

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)GetPtrLogIn (i);
		/* �ʃ}�b�v�H */
		if (pInfoCharTmp->m_dwMapID != pInfoChar->m_dwMapID) {
			continue;
		}
		switch (pInfoChar->m_nDirection) {
		case 0:
		case 1:
			/* ��ʒ[�H */
			if (abs (pInfoCharTmp->m_nMapY - y) > 2) {
				continue;
			}
			break;
		case 2:
		case 3:
			/* ��ʒ[�H */
			if (abs (pInfoCharTmp->m_nMapX - x) > 2) {
				continue;
			}
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			/* ��ʒ[�H */
			if (abs (pInfoCharTmp->m_nMapY - y) > 2) {
				continue;
			}
			/* ��ʒ[�H */
			if (abs (pInfoCharTmp->m_nMapX - x) > 2) {
				continue;
			}
			break;
		}
		aDst.Add (pInfoCharTmp->m_dwCharID);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetAreaCharInfo								 */
/* ���e		:�w��͈͂ɂԂ���L���������擾								 */
/* ���t		:2009/01/28														 */
/* ========================================================================= */

void CLibInfoCharSvr::GetAreaCharInfo(
	DWORD dwMapID,				/* [in] �Ώۃ}�b�vID */
	RECT *prcSrc,				/* [in] �Ώ۔͈� */
	ARRAYINFOCHARSVR &aDst)		/* [in] �o�͐� */
{
	int i, nCount;
	RECT rcTmp;
	PCInfoCharSvr pInfoCharTmp;

	aDst.RemoveAll ();

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)GetPtrLogIn (i);
		/* �ʃ}�b�v�H */
		if (pInfoCharTmp->m_dwMapID != dwMapID) {
			continue;
		}
		pInfoCharTmp->GetPosRect (rcTmp);
		/* �Ԃ���Ȃ��H */
		if (!((prcSrc->left <= rcTmp.right) &&
			(prcSrc->right >= rcTmp.left) &&
			(prcSrc->top <= rcTmp.bottom) &&
			(prcSrc->bottom >= rcTmp.top))) {
			continue;
		}
		aDst.Add (pInfoCharTmp);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetTailCharInfo								 */
/* ���e		:���Ă��Ă���L���������擾									 */
/* ���t		:2008/06/29														 */
/* ========================================================================= */

void CLibInfoCharSvr::GetTailCharInfo(
	CInfoCharSvr *pInfoChar,	/* [in] ��ƂȂ�L������� */
	ARRAYINFOCHARSVR &aDst)		/* [in] �o�͐� */
{
	PCInfoCharSvr pInfoCharTmp;

	aDst.RemoveAll ();

	pInfoCharTmp = pInfoChar;
	while (1) {
		if (pInfoCharTmp->m_dwTailCharID == 0) {
			break;
		}
		pInfoCharTmp = (PCInfoCharSvr)GetPtrLogIn (pInfoCharTmp->m_dwTailCharID);
		if (pInfoCharTmp == NULL) {
			break;
		}
		aDst.Add (pInfoCharTmp);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::IsMove										 */
/* ���e		:�w������ɐi�߂邩�`�F�b�N										 */
/* ���t		:2008/05/01														 */
/* �߂�l	:TRUE:�i�߂�													 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::IsMove(
	PCInfoCharBase pInfoChar,		/* [in] �L������� */
	int &nDirection,				/* [in] �ړ�������� */
	BOOL bEvent)/*FALSE*/			/* [in] TRUE:�}�b�v�C�x���g�ɂԂ��� */
{
	int x, y, nDirectionTmp;
	BOOL bRet, bResult;
	RECT rcMap;
	POINT ptFront, ptBack;
	PCInfoMapBase pInfoMap;
	PCInfoMapEventBase pInfoMapEventBase;

	bRet = FALSE;
	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}

	ptBack.x = pInfoChar->m_nMapX;
	ptBack.y = pInfoChar->m_nMapY;

	switch (nDirection) {
	case 4:
		nDirectionTmp = 0;
		bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
		if (bRet == FALSE) {
			nDirection = 3;
		} else {
			pInfoChar->GetFrontPos (ptFront, nDirectionTmp, TRUE);
			pInfoChar->m_nMapX = ptFront.x;
			pInfoChar->m_nMapY = ptFront.y;
			nDirectionTmp = 3;
			bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
			pInfoChar->m_nMapX = ptBack.x;
			pInfoChar->m_nMapY = ptBack.y;
			if (bRet) {
				return TRUE;
			}
			nDirection = 0;
			return TRUE;
		}
		break;
	case 5:
		nDirectionTmp = 1;
		bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
		if (bRet == FALSE) {
			nDirection = 3;
		} else {
			pInfoChar->GetFrontPos (ptFront, nDirectionTmp, TRUE);
			pInfoChar->m_nMapX = ptFront.x;
			pInfoChar->m_nMapY = ptFront.y;
			nDirectionTmp = 3;
			bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
			pInfoChar->m_nMapX = ptBack.x;
			pInfoChar->m_nMapY = ptBack.y;
			if (bRet) {
				return TRUE;
			}
			nDirection = 1;
			return TRUE;
		}
		break;
	case 6:
		nDirectionTmp = 1;
		bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
		if (bRet == FALSE) {
			nDirection = 2;
		} else {
			pInfoChar->GetFrontPos (ptFront, nDirectionTmp, TRUE);
			pInfoChar->m_nMapX = ptFront.x;
			pInfoChar->m_nMapY = ptFront.y;
			nDirectionTmp = 2;
			bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
			pInfoChar->m_nMapX = ptBack.x;
			pInfoChar->m_nMapY = ptBack.y;
			if (bRet) {
				return TRUE;
			}
			nDirection = 1;
			return TRUE;
		}
		break;
	case 7:
		nDirectionTmp = 0;
		bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
		if (bRet == FALSE) {
			nDirection = 2;
		} else {
			pInfoChar->GetFrontPos (ptFront, nDirectionTmp, TRUE);
			pInfoChar->m_nMapX = ptFront.x;
			pInfoChar->m_nMapY = ptFront.y;
			nDirectionTmp = 2;
			bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
			pInfoChar->m_nMapX = ptBack.x;
			pInfoChar->m_nMapY = ptBack.y;
			if (bRet) {
				return TRUE;
			}
			nDirection = 0;
			return TRUE;
		}
		break;
	}

	/* �E�o�\���`�F�b�N */
	bResult = FALSE;
	pInfoChar->GetFrontMapPosRect (rcMap, nDirection);
	for (y = rcMap.top; y <= rcMap.bottom; y ++) {
		for (x = rcMap.left; x <= rcMap.right; x ++) {
			bResult |= !pInfoMap->IsMoveOut (x, y, nDirection);
		}
	}
	bResult = !bResult;
	if (bResult == FALSE) {
		goto Exit;
	}

	/* �i�߂邩�`�F�b�N */
	bResult = FALSE;
	pInfoChar->GetFrontMapPosRect (rcMap, nDirection);
	for (y = rcMap.top; y <= rcMap.bottom; y ++) {
		for (x = rcMap.left; x <= rcMap.right; x ++) {
			bResult |= !pInfoMap->IsMove (x, y, nDirection);
			if (bEvent) {
				pInfoMapEventBase = pInfoMap->GetEvent (x, y);
				if (pInfoMapEventBase) {
					bResult = TRUE;
					break;
				}
			}
		}
	}
	bResult = !bResult;
	if (bResult == FALSE) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::IsNPC											 */
/* ���e		:NPC������														 */
/* ���t		:2008/07/25														 */
/* �߂�l	:TRUE:NPC														 */
/* ���l		:�e�L����������΁A���̐e�̑����Ŕ��肷��						 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::IsNPC(CInfoCharSvr *pInfoChar)
{
	BOOL bRet;
	CInfoCharSvr *pInfoCharParent;

	bRet = FALSE;

	pInfoCharParent = (PCInfoCharSvr)GetPtrLogIn (pInfoChar->m_dwParentCharID);
	if (pInfoCharParent) {
		pInfoChar = pInfoCharParent;
	}
	bRet = pInfoChar->IsNPC ();

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetFrontCharID								 */
/* ���e		:����O�̃L����ID���擾											 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::GetFrontCharID(DWORD dwCharID, int nDirection)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoCharBase pInfoCharSrc, pInfoCharTmp;
	POINT ptFront;

	dwRet = 0;

	pInfoCharSrc = (PCInfoCharBase)GetPtr (dwCharID);
	if (pInfoCharSrc == NULL) {
		goto Exit;
	}
	pInfoCharSrc->GetFrontPos (ptFront, nDirection, TRUE);

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfoLogin->GetAt (i);
		if (pInfoCharSrc == pInfoCharTmp) {
			continue;
		}
		if (pInfoCharSrc->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
		}
		if (pInfoCharTmp->IsHitCharPos (ptFront.x, ptFront.y) == FALSE) {
			continue;
		}
		if ((pInfoCharSrc->m_nMapX == pInfoCharTmp->m_nMapX) && (pInfoCharSrc->m_nMapY == pInfoCharTmp->m_nMapY)) {
			continue;
		}
		dwRet = pInfoCharTmp->m_dwCharID;
		break;
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetFrontCharIDPush							 */
/* ���e		:����O�̉�����L����ID���擾									 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::GetFrontCharIDPush(DWORD dwCharID, int nDirection)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoCharBase pInfoCharSrc, pInfoCharTmp;
	POINT ptFront;
	SIZE size;

	dwRet = 0;

	pInfoCharSrc = (PCInfoCharBase)GetPtr (dwCharID);
	if (pInfoCharSrc == NULL) {
		goto Exit;
	}
	pInfoCharSrc->GetFrontPos (ptFront, nDirection, FALSE);
	pInfoCharSrc->GetCharSize (size);

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfoLogin->GetAt (i);
		if (pInfoCharSrc == pInfoCharTmp) {
			continue;
		}
		if (pInfoCharTmp->m_bPush == FALSE) {
			continue;
		}
		if (pInfoCharSrc->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
		}
		if ((pInfoCharSrc->m_nMapX == pInfoCharTmp->m_nMapX) && (pInfoCharSrc->m_nMapY == pInfoCharTmp->m_nMapY)) {
			continue;
		}
		if (pInfoCharTmp->IsHitCharPos (ptFront.x, ptFront.y, &size) == FALSE) {
			continue;
		}
		dwRet = pInfoCharTmp->m_dwCharID;
		break;
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetFrontCharIDTarget							 */
/* ���e		:����O�̍U���ΏۃL����ID���擾									 */
/* ���t		:2008/07/26														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::GetFrontCharIDTarget(
	DWORD dwCharID,					/* [in] �U������L����ID */
	int nDirection/*-1*/,			/* [in] ���� */
	int nXType/*0*/,				/* [in] 1:�c�����̎��ɁA���L�������ꂽ��Ԃ͔��肵�Ȃ� */
	PARRAYDWORD padwCharID)/*NULL*/	/* [out] �ΏۃL����ID��S�Ď擾 */
{
	int i, nCount;
	DWORD dwRet;
	RECT rcFrontRect, rcTmp;
	PCInfoCharSvr pInfoCharSrc, pInfoCharTmp;

	dwRet = 0;

	pInfoCharSrc = (PCInfoCharSvr)GetPtr (dwCharID);
	if (pInfoCharSrc == NULL) {
		goto Exit;
	}
	if (nDirection == -1) {
		nDirection = pInfoCharSrc->m_nDirection;
	}
	pInfoCharSrc->GetPosRectOnce (rcFrontRect, TRUE);

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)m_paInfoLogin->GetAt (i);
		if (pInfoCharSrc == pInfoCharTmp) {
			continue;
		}
		if (pInfoCharTmp->m_dwCharID == pInfoCharSrc->m_dwParentCharID) {
			continue;
		}
		if (pInfoCharTmp->m_dwParentCharID != 0) {
			if (pInfoCharTmp->m_dwParentCharID == pInfoCharSrc->m_dwParentCharID) {
				continue;
			}
		}
		if (pInfoCharTmp->IsAtackTarget () == FALSE) {
			continue;
		}

		pInfoCharTmp->GetPosRect (rcTmp);
		if (!((rcFrontRect.left <= rcTmp.right) && (rcTmp.left <= rcFrontRect.right) &&
			(rcFrontRect.top <= rcTmp.bottom) && (rcTmp.top <= rcFrontRect.bottom))) {
			continue;
		}

		dwRet = pInfoCharTmp->m_dwCharID;
		if (padwCharID == NULL) {
			break;
		}
		padwCharID->Add (dwRet);
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetNearCharID									 */
/* ���e		:�߂��ɂ���G�L����ID���擾										 */
/* ���t		:2009/07/17														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::GetNearCharID(
	DWORD dwCharID,			/* [in] �L����ID */
	SIZE &sizedistance)		/* [in] �Ώۋ��� */
{
	int i, nCount, nMinDistance, nTmp;
	DWORD dwRet;
	RECT rcSrc, rcSearch, rcTmp;
	PCInfoCharSvr pInfoCharSrc, pInfoCharTmp;

	dwRet = 0;

	pInfoCharSrc = (PCInfoCharSvr)GetPtr (dwCharID);
	if (pInfoCharSrc == NULL) {
		goto Exit;
	}
	pInfoCharSrc->GetPosRect (rcSrc);
	rcSearch = rcSrc;
	rcSearch.left	-= sizedistance.cx;
	rcSearch.right  += sizedistance.cx;
	rcSearch.top    -= sizedistance.cy;
	rcSearch.bottom += sizedistance.cy;
	nMinDistance = -1;

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)m_paInfoLogin->GetAt (i);
		if (pInfoCharSrc == pInfoCharTmp) {
			continue;
		}
		if (pInfoCharTmp->m_dwMapID != pInfoCharSrc->m_dwMapID) {
			continue;
		}
		if (pInfoCharTmp->m_dwCharID == pInfoCharSrc->m_dwParentCharID) {
			continue;
		}
		if (pInfoCharTmp->m_dwParentCharID != 0) {
			if (pInfoCharTmp->m_dwParentCharID == pInfoCharSrc->m_dwParentCharID) {
				continue;
			}
		}
		if (pInfoCharTmp->IsAtackTarget () == FALSE) {
			continue;
		}
		if (IsNPC (pInfoCharSrc) && IsNPC (pInfoCharTmp)) {
			continue;
		}

		pInfoCharTmp->GetPosRect (rcTmp);
		if (!((rcSearch.left <= rcTmp.right) && (rcTmp.left <= rcSearch.right) &&
			(rcSearch.top <= rcTmp.bottom) && (rcTmp.top <= rcSearch.bottom))) {
			continue;
		}
		nTmp = abs (rcSrc.left - rcTmp.left) + abs (rcSrc.top - rcTmp.top);
		if (nMinDistance < 0) {
			nMinDistance = nTmp;

		} else {
			if (nMinDistance <= nTmp) {
				continue;
			}
			nMinDistance = nTmp;
		}

		dwRet = pInfoCharTmp->m_dwCharID;
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::SetPos										 */
/* ���e		:�L�����̍��W��ݒ�												 */
/* ���t		:2008/06/29														 */
/* ========================================================================= */

void CLibInfoCharSvr::SetPos(
	CInfoCharSvr *pInfoChar,		/* [in] �ΏۃL������� */
	DWORD dwMapID,					/* [in] �}�b�vID */
	int x,							/* [in] X���W */
	int y,							/* [in] Y���W */
	BOOL bTail)						/* [in] TRUE:���Ă��Ă���L�������ړ� FALSE:�؂藣�� */
{
	int i, nCount, nResult;
	PCInfoCharSvr pInfoCharTmp, pInfoCharFront;
	ARRAYINFOCHARSVR apInfoChar;

	pInfoChar->SetPos (x, y);

	if (bTail) {
		/* ���Ă��Ă���L�����ꗗ���擾 */
		GetTailCharInfo (pInfoChar, apInfoChar);

		nCount = apInfoChar.GetSize ();
		for (i = nCount - 1; i >= 0; i --) {
			pInfoCharTmp	= apInfoChar[i];
			pInfoCharFront	= (PCInfoCharSvr)GetPtrLogIn (pInfoCharTmp->m_dwFrontCharID);
			if (pInfoCharFront == NULL) {
				continue;
			}
			nResult = pInfoCharTmp->GetDirection (pInfoCharFront->m_ptCharBack.x, pInfoCharFront->m_ptCharBack.y);
			if (nResult != -1) {
				pInfoCharTmp->SetDirection (nResult);
			}
			pInfoCharTmp->SetPos (pInfoCharFront->m_ptCharBack.x, pInfoCharFront->m_ptCharBack.y);
			pInfoCharTmp->m_bChgPos			= TRUE;
			pInfoCharTmp->m_bChgUpdatePos	= TRUE;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::RenewItemGrp									 */
/* ���e		:�������̃A�C�e���摜���X�V										 */
/* ���t		:2008/07/09														 */
/* ========================================================================= */

void CLibInfoCharSvr::RenewItemGrp(
	DWORD dwTypeID)		/* [in] ���ID */
{
	BOOL bRenew;
	int i, nCount;
	DWORD dwTypeIDTmp;
	PCInfoItemTypeBase pInfoItemType;
	PCInfoCharSvr pChar;

	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (dwTypeID);
	if (pInfoItemType == NULL) {
		return;
	}

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharSvr)GetPtr (i);

		bRenew = FALSE;
		switch (pInfoItemType->m_dwItemTypeID) {
		case ITEMTYPEID_ARMS:		/* ������ */
			dwTypeIDTmp = m_pLibInfoItem->GetItemTypeID (pChar->m_dwEquipItemIDArmsRight);
			if (dwTypeID != dwTypeIDTmp) {
				break;
			}
			pInfoItemType->SetGrpID (&pChar->m_wGrpIDArmsMain, &pChar->m_wGrpIDArmsSub);
			bRenew = TRUE;
			break;
		}
		if (bRenew == FALSE) {
			continue;
		}
		pChar->m_bChgGrp = TRUE;
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::ProcAtack										 */
/* ���e		:�U������														 */
/* ���t		:2008/07/16														 */
/* ========================================================================= */

void CLibInfoCharSvr::ProcAtack(CInfoCharSvr *pInfoChar)
{
	ProcLocalStateBATTLEATACK (pInfoChar);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::ProcLocal										 */
/* ���e		:����															 */
/* ���t		:2007/09/13														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::ProcLocal(int nNo)
{
	BOOL bRet;
	PCInfoCharSvr pInfoChar;

	bRet = FALSE;

	pInfoChar = (PCInfoCharSvr)GetPtrLogIn (nNo);
	if (pInfoChar == NULL) {
		goto Exit;
	}

	pInfoChar->TimerProc (timeGetTime ());

	bRet |= ProcLocalFlgCheck (pInfoChar);
	bRet |= ProcLocalState (pInfoChar);

	if (pInfoChar->m_nMoveState == CHARMOVESTATE_DELETE) {
		LogOut (pInfoChar->m_dwCharID);
		goto Exit;
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::SendSystemMsg									 */
/* ���e		:�V�X�e�����b�Z�[�W�𑗐M										 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CLibInfoCharSvr::SendSystemMsg(
	DWORD dwSessionID,							/* [in] �Z�b�V����ID */
	LPCSTR pszMsg,								/* [in] �\�����b�Z�[�W */
	int nMsgType,	/*=SYSTEMMSGTYPE_NOLOG*/	/* [in] ���b�Z�[�W��� */
	COLORREF clMsg,	/*=RGB (255, 255, 255)*/	/* [in] �\���F */
	BOOL bSound)	/*=TRUE*/					/* [in] �\�����鎞�ɉ����Ȃ炷������ */
{
	CPacketMAP_SYSTEMMSG PacketMsg;

	if (dwSessionID == 0) {
		return;
	}

	PacketMsg.Make (pszMsg, clMsg, bSound, nMsgType);
	m_pSock->SendTo (dwSessionID, &PacketMsg);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::SendFormatMsg									 */
/* ���e		:�t�H�[�}�b�g���b�Z�[�W�𑗐M									 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CLibInfoCharSvr::SendFormatMsg(
	DWORD dwSessionID,							/* [in] �Z�b�V����ID */
	DWORD dwMsgID,								/* [in] ���b�Z�[�WID */
	DWORD dwPara1,	/*=0*/						/* [in] �p�����[�^1 */
	DWORD dwPara2,	/*=0*/						/* [in] �p�����[�^2 */
	int nMsgType,	/*=FORMATMSGTYPE_BATTLE*/	/* [in] ���b�Z�[�W��� */
	COLORREF clMsg,	/*=RGB (255, 255, 255)*/	/* [in] �\���F */
	BOOL bSound)	/*=FALSE*/					/* [in] �\�����鎞�ɉ����Ȃ炷������ */
{
	CPacketMAP_FORMATMSG PacketMsg;

	if (dwSessionID == 0) {
		return;
	}

	PacketMsg.Make (dwMsgID, dwPara1, dwPara2, clMsg, bSound, nMsgType);
	m_pSock->SendTo (dwSessionID, &PacketMsg);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::ProcLocalFlgCheck								 */
/* ���e		:�t���O�`�F�b�N����												 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::ProcLocalFlgCheck(CInfoCharSvr *pInfoChar)
{
	BOOL bRet, bResult;

	bRet = FALSE;

	/* ���O�C�����Ă��Ȃ��H */
	if (pInfoChar->IsLogin () == FALSE) {
		goto Exit;

	/* �A�C�e���h���b�v�H */
	} else if (pInfoChar->m_bDropItem) {
		DropItem (pInfoChar);
		pInfoChar->m_bDropItem = FALSE;

	/* �ړ������H */
	} else if (pInfoChar->m_bChgPos) {
		ProcChgPos (pInfoChar);

	/* �}�b�v�ړ������H */
	} else if (pInfoChar->m_bChgMap) {
		ProcChgMap (pInfoChar);

	/* �ړ���Ԃ��ς�����H */
	} else if (pInfoChar->m_bChgMoveState) {
		CPacketCHAR_STATE Packet;

		Packet.Make (pInfoChar->m_dwCharID, pInfoChar->m_nMoveState);
		m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);
		pInfoChar->m_bChgMoveState = FALSE;

	/* �s����Ԃ��ς�����H */
	} else if (pInfoChar->m_bChgProcState) {
		CPacketCHAR_PARA1 Packet;

		Packet.Make (SBOCOMMANDID_SUB_CHAR_PROCSTATE, pInfoChar->m_dwCharID, pInfoChar->m_nProcState);
		m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &Packet);
		pInfoChar->m_bChgProcState = FALSE;

	/* �G�t�F�N�g�ύX�\�񂠂�H */
	} else if (pInfoChar->m_nReserveChgEfect != 0) {
		CPacketCHAR_SET_EFFECT Packet;

		Packet.Make (pInfoChar->m_dwCharID, pInfoChar->m_nReserveChgEfect);
		m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);
		pInfoChar->m_nReserveChgEfect = 0;

	/* �������e���ς�����H */
	} else if (pInfoChar->m_bChgSpeak) {
		CPacketCHAR_CHAT Packet;

		if (pInfoChar->m_strSpeak.IsEmpty () == FALSE) {
			Packet.Make (pInfoChar->m_dwCharID, pInfoChar->m_strSpeak);
			m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);
		}
		pInfoChar->m_bChgSpeak = FALSE;

	/* �L������񂪕ς�����H */
	} else if (pInfoChar->m_bChgInfo) {
		CPacketCHAR_RES_CHARINFO Packet;

		Packet.Make (pInfoChar, pInfoChar->m_bChgScreenPos);
		m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);
		pInfoChar->m_bChgInfo = FALSE;
		pInfoChar->m_bChgScreenPos = FALSE;

	/* �L������񂪕ς�����H */
	} else if (pInfoChar->m_bChgPosRenew) {
		ProcChgPosRenew (pInfoChar);

	/* �摜���ύX���ꂽ�H */
	} else if (pInfoChar->m_bChgGrp) {
		CPacketCHAR_GRP PacketGRP;
		CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;

		PacketGRP.Make (pInfoChar);
		m_pMainFrame->SendToScreenChar (pInfoChar, &PacketGRP);
		PacketRES_CHARINFO.Make (pInfoChar);
		m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketRES_CHARINFO);
		pInfoChar->m_bChgGrp = FALSE;

	/* ���o�����ύX���ꂽ�H */
	} else if (pInfoChar->m_bChgEfcBalloon) {
		CPacketCHAR_SET_EFCBALLOON Packet;

		Packet.Make (pInfoChar->m_dwCharID, pInfoChar->m_dwEfcBalloonID);
		m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);
		pInfoChar->m_dwEfcBalloonID = 0;
		pInfoChar->m_bChgEfcBalloon = FALSE;

	/* ���[�V�������ύX���ꂽ�H */
	} else if (pInfoChar->m_bChgMotion) {
		CPacketCHAR_PARA1 Packet;

		Packet.Make (SBOCOMMANDID_SUB_CHAR_SET_MOTION, pInfoChar->m_dwCharID, pInfoChar->m_dwMotionID);
		m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);
		pInfoChar->m_dwMotionID = 0;
		pInfoChar->m_bChgMotion = FALSE;

	/* �X�e�[�^�X���ύX���ꂽ�H */
	} else if (pInfoChar->m_bChgStatus) {
		CPacketCHAR_STATUS Packet;

		pInfoChar->m_bChgStatus = FALSE;
		Packet.Make (pInfoChar);
		m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);

	/* �ނꂽ�H */
	} else if (pInfoChar->m_bChgFishingHit) {
		DWORD dwItemID, dwItemTypeID;
		POINT ptPos;
		PCInfoItem pInfoItem;
		CPacketCHAR_ITEMINFO PacketCHAR_ITEMINFO;
		CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;

		bResult = pInfoChar->IsItemAdd ();
		if (bResult) {
			ptPos.x = ptPos.y = 0;
//Todo:
			dwItemTypeID = 9 + (genrand () % 2);
//			dwItemTypeID = (genrand () % 30) + 1;
//			if (dwItemTypeID == 5) {
//				dwItemTypeID = 16;
//			}
			dwItemID = m_pLibInfoItem->MakeItem (0, &ptPos, dwItemTypeID);
			if (dwItemID != 0) {
				m_pLibInfoItem->AddItem (pInfoChar->m_dwCharID, dwItemID, &pInfoChar->m_adwItemID);
				pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);

				PacketCHAR_ITEMINFO.Make (pInfoChar->m_dwCharID, &pInfoChar->m_adwItemID);
				m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketCHAR_ITEMINFO);
				PacketITEM_RES_ITEMINFO.Make (pInfoItem);
				m_pMainFrame->SendToMapChar (pInfoChar->m_dwMapID, &PacketITEM_RES_ITEMINFO);
			}
		}
		pInfoChar->m_bChgFishingHit = FALSE;

	/* �w������̈ړ� */
	} else if (pInfoChar->m_bChgMoveCount) {
		pInfoChar->m_bChgMoveCount = FALSE;
		Move (pInfoChar->m_dwCharID, pInfoChar->m_nDirection, FALSE);

	/* NPC���� */
	} else if (pInfoChar->m_bChgPutNpc) {
		PutNpc (pInfoChar);

	/* �߂��̃L�����Ƀ^�[�Q�b�g�ύX */
	} else if (pInfoChar->m_bChgTargetChar) {
		TargetChar (pInfoChar);	

	/* �}�b�v�C�x���g�`�F�b�N�҂� */
	} else if (pInfoChar->m_bWaitCheckMapEvent) {
		CPacketCHAR_PARA1 Packet;

		bResult = CheckMapEvent (pInfoChar);
		pInfoChar->m_bWaitCheckMapEvent = FALSE;
		if (bResult) {
			Packet.Make (SBOCOMMANDID_SUB_CHAR_RES_CHECKMAPEVENT, pInfoChar->m_dwCharID, 0);
			m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &Packet);
		}

	/* �}�b�v���ړ� */
	} else if (pInfoChar->m_bProcMoveMapIn) {
		MoveMapIn (pInfoChar);
		pInfoChar->m_bProcMoveMapIn = FALSE;

	/* �}�b�v�O�ړ� */
	} else if (pInfoChar->m_bProcMoveMapOut) {
		MoveMapOut (pInfoChar);
		pInfoChar->m_bProcMoveMapOut = FALSE;

	/* �L�^�ʒu�ֈړ� */
	} else if (pInfoChar->m_bProcMoveMarkPos) {
		CharProcMoveMarkPos (pInfoChar);
		pInfoChar->m_bProcMoveMarkPos = FALSE;

	/* �C�� */
	} else if (pInfoChar->m_bProcSwoon) {
		CharProcSWOON (pInfoChar);
		pInfoChar->m_bProcSwoon = FALSE;

	/* �U�� */
	} else if (pInfoChar->m_bAtack) {
		CharProcAtack (pInfoChar);
		pInfoChar->m_bAtack = FALSE;

	/* �^�[�Q�b�g���W�X�V */
	} else if (pInfoChar->m_bRenewTargetPos) {
		PCInfoCharSvr pCharTarget;

		pCharTarget = (PCInfoCharSvr)GetPtrLogIn (pInfoChar->m_dwTargetCharID);
		pInfoChar->SetTarget (pCharTarget);
		pInfoChar->m_bRenewTargetPos = FALSE;

	/* �ړ���ʕύX�\�� */
	} else if (pInfoChar->m_nReserveChgMoveState != 0) {
		pInfoChar->SetMoveState (pInfoChar->m_nReserveChgMoveState);
		pInfoChar->m_nReserveChgMoveState = 0;

	} else {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::ProcLocalState								 */
/* ���e		:��Ԃɉ���������												 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::ProcLocalState(CInfoCharSvr *pInfoChar)
{
	BOOL bRet, bResult;

	bRet = FALSE;

	if (pInfoChar->IsNPC () == FALSE) {
		goto Exit;
	}
	switch (pInfoChar->m_nMoveState) {
	case CHARMOVESTATE_BATTLEATACK:		/* �퓬�U���� */
		bResult = ProcLocalStateBATTLEATACK (pInfoChar);
		if (bResult == FALSE) {
			goto Exit;
		}
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::ProcChgPos									 */
/* ���e		:�ړ�����														 */
/* ���t		:2007/02/12														 */
/* ========================================================================= */

void CLibInfoCharSvr::ProcChgPos(CInfoCharSvr *pInfoChar)
{
	CPacketCHAR_MOVEPOS Packet;
	CPacketCHAR_CHARID PacketCHAR_CHARID;
	ARRAYDWORD adwCharID;

	/* ����̃L�����ɍ��W��ʒm */
	Packet.Make (
			pInfoChar->m_dwMapID,
			pInfoChar->m_dwCharID,
			pInfoChar->m_nDirection,
			pInfoChar->m_nMapX,
			pInfoChar->m_nMapY,
			pInfoChar->m_bChgUpdatePos);
	m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);

	/* �ړ���Ɍ�����͈͂̃L����ID��ʒm */
	GetScreenCharID (pInfoChar, adwCharID);
	PacketCHAR_CHARID.Make (&adwCharID);
	m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketCHAR_CHARID);

	pInfoChar->m_bChgPos		= FALSE;
	pInfoChar->m_bChgUpdatePos	= FALSE;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::ProcChgMap									 */
/* ���e		:�}�b�v�ړ�����													 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

void CLibInfoCharSvr::ProcChgMap(CInfoCharSvr *pInfoChar)
{
	CPacketMAP_PARA1 PacketMAP_PARA1;

	pInfoChar->m_bChgMap = FALSE;

	pInfoChar->AddProcInfo (CHARPROCID_MAPMOVEOUT, 2000, 0);

	PacketMAP_PARA1.Make (SBOCOMMANDID_SUB_MAP_FADEINOUT, pInfoChar->m_dwMapID, 1);
	m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketMAP_PARA1);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::ProcChgPosRenew								 */
/* ���e		:���W�ύX����													 */
/* ���t		:2007/09/16														 */
/* ========================================================================= */

void CLibInfoCharSvr::ProcChgPosRenew(CInfoCharSvr *pInfoChar)
{
	CPacketCHAR_MOVEPOS Packet;
	CPacketCHAR_CHARID PacketCHAR_CHARID;
	ARRAYDWORD adwCharID;

	/* ����̃L�����ɍ��W��ʒm */
	Packet.Make (
			pInfoChar->m_dwMapID,
			pInfoChar->m_dwCharID,
			pInfoChar->m_nDirection,
			pInfoChar->m_nMapX,
			pInfoChar->m_nMapY,
			pInfoChar->m_bChgUpdatePos);
	m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);

	/* �ړ���Ɍ�����͈͂̃L����ID��ʒm */
	GetScreenCharID (pInfoChar, adwCharID);
	PacketCHAR_CHARID.Make (&adwCharID);
	m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketCHAR_CHARID);

	pInfoChar->m_bChgPosRenew = FALSE;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::ProcLocalStateBATTLEATACK						 */
/* ���e		:��Ԃɉ���������(�퓬�U����)									 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::ProcLocalStateBATTLEATACK(CInfoCharSvr *pInfoChar)
{
	BOOL bRet, bResult;
	POINT ptFront;
	int nType;

	bRet = FALSE;

	if (pInfoChar->m_nDirection >= 4) {
		/* �΂ߕ����ɂ͍U�����肵�Ȃ� */
		goto Exit;
	}

	nType = pInfoChar->GetAtackType ();
	switch (nType) {
	case WEAPONTYPE_SWING:			/* �U�� */
	case WEAPONTYPE_POKE:			/* �˂� */
		bResult = Atack (pInfoChar);
		if (bResult == FALSE) {
			goto Exit;
		}
		break;
	case WEAPONTYPE_BOW:			/* �| */
		{
			PCInfoMapBase pInfoMap;
			PCInfoItemTypeBase pInfoItemType;
			CInfoCharMOVEATACKSvr InfoCharTmp, *pInfoCharTmp;

			pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDArmsRight);
			if (pInfoItemType == NULL) {
				goto Exit;
			}
			pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);

			/* �i�߂邩�`�F�b�N */
			bResult = IsMove (pInfoChar, pInfoChar->m_nDirection);
			if (bResult == FALSE) {
				goto Exit;
			}

			pInfoChar->GetFrontPos (ptFront, pInfoChar->m_nDirection, TRUE);
			InfoCharTmp.m_dwParentCharID	= pInfoChar->m_dwCharID;
			InfoCharTmp.m_dwMapID			= pInfoChar->m_dwMapID;
			InfoCharTmp.m_nDirection		= pInfoChar->m_nDirection;
			InfoCharTmp.m_bChargeAtack		= pInfoChar->m_bChargeAtack;
			InfoCharTmp.m_nMapX				= ptFront.x;
			InfoCharTmp.m_nMapY				= ptFront.y;
			InfoCharTmp.m_dwHP				= 1;
			InfoCharTmp.m_nMoveType			= CHARMOVETYPE_MOVEATACK;
			InfoCharTmp.m_wGrpIDNPC			= 50011;
			InfoCharTmp.m_ptViewCharPos.y	= 16;
			InfoCharTmp.m_dwMoveWait		= pInfoItemType->m_dwMoveWait;
			InfoCharTmp.m_dwMoveCount		= pInfoItemType->m_dwMoveCount;

			pInfoCharTmp = (PCInfoCharMOVEATACKSvr)AddNPC (&InfoCharTmp);
			pInfoCharTmp->SetMap (pInfoMap);
			pInfoCharTmp->SetLibInfoChar (this);
		}
		break;
	}

	bRet = TRUE;
Exit:
	pInfoChar->SetMoveState (CHARMOVESTATE_BATTLE);
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::CharProcMoveMarkPos							 */
/* ���e		:�L��������(�L�^�ʒu�ֈړ�)										 */
/* ���t		:2008/12/02														 */
/* ========================================================================= */

void CLibInfoCharSvr::CharProcMoveMarkPos(CInfoCharSvr *pInfoChar)
{
//Todo:�L�^�ʒu���������Ȃ̂ŏ����ʒu�֖߂�
	PCInfoSystem pInfoSystem;
	PSTSYSTEM_INITCHARSTATUS pInitCharStatus;

	pInfoSystem		= (PCInfoSystem)m_pMgrData->GetLibInfoSystem ()->GetPtr ();
	pInitCharStatus	= pInfoSystem->m_pInitCharStatus;

	/* �����ʒu�ɓ]�� */
	pInfoChar->m_dwMapID = pInitCharStatus->dwInitPosMapID;		/* �}�b�vID */
	pInfoChar->SetPos (pInitCharStatus->ptInitPos.x, pInitCharStatus->ptInitPos.y, TRUE);
	pInfoChar->SetDirection (1);
	pInfoChar->AddProcInfo (CHARPROCID_MAPMOVEOUT, 2000, 0);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::CharProcSWOON									 */
/* ���e		:�L��������(�C��)												 */
/* ���t		:2008/06/29														 */
/* ========================================================================= */

void CLibInfoCharSvr::CharProcSWOON(CInfoCharSvr *pInfoChar)
{
	CPacketCHAR_STATUS PacketCHAR_STATUS;

	pInfoChar->m_dwHP = pInfoChar->m_dwMaxHP;
	pInfoChar->SetMoveState (CHARMOVESTATE_STAND);

	PacketCHAR_STATUS.Make (pInfoChar);
	m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketCHAR_STATUS);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::CharProcAtack									 */
/* ���e		:�L��������(�U��)												 */
/* ���t		:2008/07/24														 */
/* ========================================================================= */

void CLibInfoCharSvr::CharProcAtack(CInfoCharSvr *pInfoChar)
{
	BOOL bResult;
	DWORD dwCharID;
	PCInfoCharSvr pCharTarget;
	CPacketCHAR_STATE PacketCHAR_STATE;

	dwCharID = GetFrontCharIDTarget (pInfoChar->m_dwCharID);
	/* ����O�ɂ��Ȃ��H */
	if (dwCharID == 0) {
		goto Exit;
	}
	pCharTarget = (PCInfoCharSvr)GetPtrLogIn (dwCharID);
	if (pCharTarget == NULL) {
		goto Exit;
	}
	if (pInfoChar->IsNPC () && pCharTarget->IsNPC ()) {
		goto Exit;
	}
	bResult = pCharTarget->IsAtackTarget ();
	if (bResult == FALSE) {
		goto Exit;
	}

	pInfoChar->SetMoveState (CHARMOVESTATE_BATTLEATACK);
	PacketCHAR_STATE.Make (pInfoChar->m_dwCharID, CHARMOVESTATE_BATTLEATACK);
	m_pMainFrame->SendToScreenChar (pInfoChar, &PacketCHAR_STATE);

Exit:
	return;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::PutNpc										 */
/* ���e		:NPC����														 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CLibInfoCharSvr::PutNpc(CInfoCharSvr *pInfoChar)
{
	int cx, cy;
	BOOL bResult;
	PCInfoMapBase pInfoMap;
	PCInfoCharSvr pInfoCharTmp, pInfoCharAdd;
	POINT ptPos;
	RECT rcPut;

	pInfoCharTmp = NULL;

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}

	/* ���W��ݒ� */
	rcPut.left		= pInfoChar->m_nMapX - pInfoChar->m_ptPutArea.x;
	rcPut.left		= max (rcPut.left, 0);
	rcPut.right		= pInfoChar->m_nMapX + pInfoChar->m_ptPutArea.x;
	rcPut.right		= min (rcPut.right, (pInfoMap->m_sizeMap.cx - 1) * 2);
	rcPut.top		= pInfoChar->m_nMapY - pInfoChar->m_ptPutArea.y;
	rcPut.top		= max (rcPut.top, 0);
	rcPut.bottom	= pInfoChar->m_nMapY + pInfoChar->m_ptPutArea.y;
	rcPut.bottom	= min (rcPut.bottom, (pInfoMap->m_sizeMap.cy - 1) * 2);

	cx = rcPut.right - rcPut.left + 1;
	cy = rcPut.bottom - rcPut.top + 1;
	ptPos.x = (genrand () % cx) + rcPut.left;
	ptPos.y = (genrand () % cy) + rcPut.top;

	pInfoCharTmp = (PCInfoCharSvr)GetNew (pInfoChar->m_nPutMoveType);
	pInfoCharTmp->Copy (pInfoChar);
	pInfoCharTmp->GetItem ()->RemoveAll ();			/* �A�C�e�����̓R�s�[���Ȃ� */
	pInfoCharTmp->m_dwCharID		= 0;
	pInfoCharTmp->m_dwParentCharID	= pInfoChar->m_dwCharID;
	pInfoCharTmp->m_nMoveType		= pInfoChar->m_nPutMoveType;
	pInfoCharTmp->m_nMapX			= ptPos.x;
	pInfoCharTmp->m_nMapY			= ptPos.y;
	pInfoCharTmp->m_nDirection		= 1;

	bResult = IsMove (pInfoCharTmp, pInfoCharTmp->m_nDirection);
	if (bResult == FALSE) {
		goto Exit;
	}

	pInfoCharAdd = (PCInfoCharSvr)AddNPC (pInfoCharTmp);
	pInfoCharAdd->SetMap (pInfoMap);
	pInfoCharAdd->SetLibInfoChar (this);

	pInfoChar->IncPutCount ();
	pInfoChar->m_bChgPutNpc = FALSE;
Exit:
	SAFE_DELETE (pInfoCharTmp);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::TargetChar									 */
/* ���e		:�߂��̃L�����Ƀ^�[�Q�b�g�ύX									 */
/* ���t		:2009/07/17														 */
/* ========================================================================= */

void CLibInfoCharSvr::TargetChar(CInfoCharSvr *pInfoChar)
{
	DWORD dwTargetCharID;
	PCInfoCharSvr pCharTarget;

	dwTargetCharID = GetNearCharID (pInfoChar->m_dwCharID, pInfoChar->m_sizeSearchDistance);
	if (dwTargetCharID == 0) {
		return;
	}
	pCharTarget = (PCInfoCharSvr)GetPtrLogIn (dwTargetCharID);
	if (pCharTarget) {
		pInfoChar->SetTarget (pCharTarget);
	}
	pInfoChar->m_bChgTargetChar = FALSE;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::DropItem										 */
/* ���e		:�A�C�e���h���b�v												 */
/* ���t		:2008/07/19														 */
/* ========================================================================= */

void CLibInfoCharSvr::DropItem(CInfoCharSvr *pInfoChar)
{
	int nItemCount;
	DWORD dwItemID;
	POINT ptPos;
	PARRAYDWORD paItem;
	PCInfoItem pInfoItem;
	PCInfoCharSvr pInfoCharParent;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;
	CPacketMAP_FORMATMSG PacketMsg;

	if (genrand () % 100 < 100 - pInfoChar->m_nDropItemAverage) {
		return;
	}

	paItem		= pInfoChar->GetItem ();
	nItemCount	= paItem->GetSize ();
	/* �A�C�e���������Ă��Ȃ���ΐe�̏����i��ΏۂƂ��� */
	if (nItemCount == 0) {
		pInfoCharParent = (PCInfoCharSvr)GetPtrParent (pInfoChar);
		if (pInfoCharParent == NULL) {
			return;
		}
		paItem		= pInfoCharParent->GetItem ();
		nItemCount	= paItem->GetSize ();
		if (nItemCount == 0) {
			return;
		}
	}

	/* �����i����1�I�� */
	dwItemID = paItem->GetAt (genrand () % nItemCount);

	ptPos.x = pInfoChar->m_nMapX;
	ptPos.y = pInfoChar->m_nMapY;

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}
	dwItemID = m_pLibInfoItem->MakeItem (pInfoChar->m_dwMapID, &ptPos, pInfoItem->m_dwItemTypeID);
	if (dwItemID == 0) {
		return;
	}
	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}
	PacketITEM_RES_ITEMINFO.Make (pInfoItem);
	m_pMainFrame->SendToMapChar (pInfoChar->m_dwMapID, &PacketITEM_RES_ITEMINFO);

	PacketMsg.Make (FORMATMSGID_DROPITEM, pInfoChar->m_dwCharID, dwItemID, RGB (255, 255, 255), FALSE, FORMATMSGTYPE_BATTLE);
	m_pMainFrame->SendToScreenChar (pInfoChar, &PacketMsg);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetTargetCharID								 */
/* ���e		:�ΏۃL����ID���擾												 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

void CLibInfoCharSvr::GetTargetCharID(CInfoCharSvr *pInfoChar, int nTarget, int nArea, ARRAYDWORD &adstCharID)
{
	int i, nCount;
	PCInfoCharSvr pInfoCharTmp;

	adstCharID.RemoveAll ();

	nCount = GetCountLogIn ();

	switch (nArea) {
	case CHARAREA_SELF:			/* ���� */
		adstCharID.Add (pInfoChar->m_dwCharID);
		break;
	case CHARAREA_FRONT:		/* 1���O */
	case CHARAREA_AROUND:		/* ���� */
		break;
	case CHARAREA_SCREEN:		/* ��ʓ� */
		for (i = 0; i < nCount; i ++) {
			pInfoCharTmp = (PCInfoCharSvr)GetPtrLogIn (i);
			if (pInfoCharTmp->m_dwMapID != pInfoChar->m_dwMapID) {
				continue;
			}
			if (pInfoCharTmp->IsAtackTarget () == FALSE) {
				continue;
			}
			/* ��ʊO�H */
			if (!(
				(abs (pInfoCharTmp->m_nMapX - pInfoChar->m_nMapX) < DRAW_PARTS_X * 2 + 2) &&
				(abs (pInfoCharTmp->m_nMapY - pInfoChar->m_nMapY) < DRAW_PARTS_Y * 2 + 2))) {
				continue;
			}
			if (nTarget == CHARTARGET_ALLY) {
				if (pInfoCharTmp->IsNPC ()) {
					continue;
				}
			} else {
				if (pInfoCharTmp->IsNPC () == FALSE) {
					continue;
				}
			}
			adstCharID.Add (pInfoCharTmp->m_dwCharID);
		}
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::IsHitAtack									 */
/* ���e		:�U�����q�b�g���邩����											 */
/* ���t		:2008/09/20														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::IsHitAtack(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget)
{
	BOOL bRet;
	int nAverage;

	bRet = FALSE;

	nAverage = (int)pInfoChar->m_wPHitAverage;
	if (nAverage == 0) {
		/* �������Ɖ�𗦂��O�������ꍇ�͏]���ʂ蓖����Ƃ��Ă��� */
		nAverage = 100;
	}
	nAverage -= (int)pCharTarget->m_wPAvoidAverage;
	nAverage  = max (nAverage, 0);
	nAverage  = min (nAverage, 100);

	if (genrand () % 100 >= 100 - nAverage) {
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::Damage										 */
/* ���e		:�_���[�W����													 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

void CLibInfoCharSvr::Damage(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget, DWORD dwPoint, int nEffectID, BOOL bCritical)
{
	BOOL bDefense, bResult;
	int nReserveChgEfect, anDirection[] = {1, 0, 3, 2, 6, 7, 4, 5};
	DWORD dwStartTime, dwPointTmp;
	COLORREF clMsg;
	PCInfoCharSvr pCharTmp;
	PCInfoMapBase pInfoMap;
	CPacketMAP_FORMATMSG PacketMsg;
	CPacketCHAR_STATUS PacketCHAR_STATUS;
	CPacketCHAR_STATE PacketCHAR_STATE;
	CPacketCHAR_TEXTEFFECT PacketCHAR_TEXTEFFECT;
	CPacketCHAR_SET_EFFECT PacketCHAR_SET_EFFECT;

	clMsg = RGB (255, 255, 255);

	pCharTmp = pInfoChar;
	if (pInfoChar->m_dwParentCharID) {
		/* �e������ꍇ�͐e�̏����g�p���� */
		pCharTmp = (PCInfoCharSvr)GetPtrLogIn (pInfoChar->m_dwParentCharID);
	}

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pCharTarget->m_dwMapID);

	bDefense = (pCharTarget->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) ? TRUE : FALSE;
	if (bDefense) {
		if (pCharTarget->m_nDirection != anDirection[pInfoChar->m_nDirection]) {
			/* ���ʂłȂ����͖h�䖳�� */
			bDefense = FALSE;
		}
	}

	nReserveChgEfect = nEffectID;
	if (bDefense) {
		/* �h�䒆�̓_���[�W���� */
		dwPoint /= 2;
		nReserveChgEfect = 3;
		/* �h�䐬�����[�V�����Đ� */
		pCharTarget->SetMotion (CHARMOTIONLISTID_DEFENSE_SUCCESS_UP);

	} else {
		/* �_���[�W�ʒm�Ƃ��ď�Ԃ͕ς����ɃN���C�A���g�֒ʒm */
		PacketCHAR_STATE.Make (pCharTarget->m_dwCharID, CHARMOVESTATE_DAMAGE);
		m_pMainFrame->SendToScreenChar (pCharTarget, &PacketCHAR_STATE);
		if (pCharTarget->IsLogoutDelete ()) {
			/* �_���[�W���[�V�����Đ� */
			pCharTarget->SetMotion (CHARMOTIONLISTID_DAMAGE_UP);
		}
	}
	/* �G�t�F�N�g�Đ� */
	if (nReserveChgEfect) {
		PacketCHAR_SET_EFFECT.Make (pCharTarget->m_dwCharID, nReserveChgEfect);
		m_pMainFrame->SendToScreenChar (pCharTarget, &PacketCHAR_SET_EFFECT);
	}

	dwPointTmp = min (dwPoint, pCharTarget->m_dwHP);
	pCharTarget->m_dwHP -= dwPointTmp;

	SendFormatMsg (pCharTarget->m_dwSessionID, FORMATMSGID_DAMAGE1, pCharTmp->m_dwCharID, dwPoint);
	SendFormatMsg (pCharTmp->m_dwSessionID, FORMATMSGID_DAMAGE2, pCharTarget->m_dwCharID, dwPoint);

	PacketCHAR_STATUS.Make (pCharTarget);
	m_pMainFrame->SendToClient (pCharTarget->m_dwSessionID, &PacketCHAR_STATUS);

	/* �����G�t�F�N�g��ʒm */
	PacketCHAR_TEXTEFFECT.Make (
			pCharTarget->m_dwCharID,
			dwPoint,
			(bCritical) ? 2 : 1,
			(bCritical) ? TEXTEFFECTNUMSIZE_LARGE : TEXTEFFECTNUMSIZE_MEDIUM);
	m_pMainFrame->SendToScreenChar (pCharTarget, &PacketCHAR_TEXTEFFECT);

	/* �C��H */
	if (pCharTarget->m_dwHP == 0) {
		pCharTarget->SetMoveState (CHARMOVESTATE_SWOON);
//Todo:�b��
		dwStartTime = 5000;
		bResult		= TRUE;
		if (pCharTarget->IsLogoutDelete ()) {
			/* ���S�G�t�F�N�g�Đ� */
			pCharTarget->SetMotion (CHARMOTIONLISTID_DEAD_UP);
			dwStartTime = 2500;
		} else {
			/* �t���čs���̂ƕt���ė��Ă���̂������������2��Ă� */
			Tail (pCharTarget, NULL, FALSE);
			Tail (pCharTarget, NULL, FALSE);
			pCharTarget->m_nReserveChgEfect = 2;
			if (pInfoMap->m_bRecovery == FALSE) {
				bResult = FALSE;
			}
		}
		if (bResult) {
			pCharTarget->AddProcInfo (CHARPROCID_SWOON, dwStartTime, 0);
		}

		SendFormatMsg (pCharTarget->m_dwSessionID, FORMATMSGID_SWOON3, pCharTmp->m_dwCharID);
		SendFormatMsg (pCharTmp->m_dwSessionID, FORMATMSGID_SWOON2, pCharTarget->m_dwCharID);

		PacketMsg.Make (FORMATMSGID_SWOON1, pCharTmp->m_dwCharID, pCharTarget->m_dwCharID, clMsg, FALSE, FORMATMSGTYPE_BATTLE);
		m_pMainFrame->SendToScreenChar (pCharTmp, &PacketMsg);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetAtackDamage								 */
/* ���e		:�U���_���[�W���擾												 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::GetAtackDamage(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget)
{
	int nHitAverage, nAT, nDF, nPoint;
	DWORD dwRet;
	PCInfoItemTypeBase pInfoItemType;

	dwRet = pInfoChar->GetDamage ();
	if (dwRet != 0) {
		return dwRet;
	}
	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDArmsRight);

//Todo:
	nHitAverage = pInfoChar->m_wPHitAverage;
	nHitAverage -= pCharTarget->m_wPAvoidAverage;
	nHitAverage = 100;

	nAT = pInfoChar->m_wPAtack;
	nDF = pInfoChar->m_wPDefense;

	/* ���푕�����H */
	if (pInfoItemType) {
		nAT += pInfoItemType->m_dwValue;
	}

	/* ���ڍU���̃_���[�W��{ �U���� - ( ����� / 2 ) } �}16�� */
	nPoint = nAT - (nDF / 2);

	/* �{�|�̕␳ */
	if (genrand () % 2) {
		nPoint += (nPoint * ((genrand () % 16) + 1) / 100);
	} else {
		nPoint -= (nPoint * ((genrand () % 16) + 1) / 100);
	}
	nPoint = max (nPoint, 0);

	/* �m�[�_���[�W�H */
	if (nPoint == 0) {
		if ((genrand () % 100) > 66) {
			nPoint = 1;
		}
	}

	dwRet = nPoint;
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::GetAtackEffectID								 */
/* ���e		:�����ɉ������U�����̃G�t�F�N�gID���擾							 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::GetAtackEffectID(CInfoCharSvr *pInfoChar, BOOL bCritical)
{
	int nCount;
	DWORD dwRet;
	PCInfoItemTypeBase pInfoItemType;
	PCInfoItemWeapon pInfoItemWeapon;
	PARRAYDWORD padwEffectID;

	dwRet = pInfoChar->GetHitEffectID ();
	if (dwRet != 0) {
		return dwRet;
	}
//Todo:�����l
	dwRet = 1;

	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDArmsRight);
	if (pInfoItemType == NULL) {
		goto Exit;
	}
	pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr (pInfoItemType->m_dwWeaponInfoID);
	if (pInfoItemWeapon == NULL) {
		goto Exit;
	}

	padwEffectID = &pInfoItemWeapon->m_adwEffectIDAtack;
	if (bCritical) {
		padwEffectID = &pInfoItemWeapon->m_adwEffectIDCritical;
	}
	nCount = padwEffectID->GetSize ();
	if (nCount <= 0) {
		goto Exit;
	}
	dwRet = padwEffectID->GetAt (genrand () % nCount);

Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::UseItemProcHP									 */
/* ���e		:�A�C�e���g�p����(HP����)										 */
/* ���t		:2008/07/05														 */
/* ========================================================================= */

int CLibInfoCharSvr::UseItemProcHP(CInfoCharSvr *pInfoChar, DWORD dwItemID)
{
	int i, nCount, nRet, nTmp, nPoint;
	PCInfoItem pInfoItem;
	PCInfoItemTypeBase pInfoItemType;
	PCInfoCharSvr pCharTarget;
	CmyString strMsg;
	ARRAYDWORD aCharID;
	CPacketCHAR_STATUS PacketCHAR_STATUS;
	CPacketADMIN_PLAYSOUND PacketPLAYSOUND;
	CPacketCHAR_TEXTEFFECT PacketCHAR_TEXTEFFECT;

	nRet = 0;

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		goto Exit;
	}
	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (pInfoItem->m_dwItemTypeID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}

	nTmp = (int)pInfoItemType->m_dwValue2;
	if (nTmp > 0) {
		if (pInfoChar->m_dwMaxHP == pInfoChar->m_dwHP) {
			goto Exit;
		}
	}
	GetTargetCharID (pInfoChar, pInfoItemType->m_byTarget, pInfoItemType->m_byArea, aCharID);

	nCount = aCharID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		nTmp = (int)pInfoItemType->m_dwValue2;
		if (nTmp == 0) {
			nTmp = 1;
		}
		nPoint = genrand () % abs (nTmp);
		if (nTmp < 0) {
			nPoint *= -1;
		}
		nPoint += (int)pInfoItemType->m_dwValue;

		pCharTarget = (PCInfoCharSvr)GetPtrLogIn (aCharID[i]);
		if (pCharTarget == NULL) {
			continue;
		}
		/* �� */
		if (nPoint >= 0) {
			nPoint = min ((DWORD)nPoint, pCharTarget->m_dwMaxHP - pCharTarget->m_dwHP);

			strMsg.Format ("HP�� %d �񕜂��܂���", nPoint);

			pCharTarget->m_dwHP += nPoint;

			/* �����G�t�F�N�g��ʒm */
			PacketCHAR_TEXTEFFECT.Make (pCharTarget->m_dwCharID, nPoint, 3);
			m_pMainFrame->SendToScreenChar (pCharTarget, &PacketCHAR_TEXTEFFECT);

			if (pCharTarget->m_dwSessionID) {
				SendSystemMsg (pCharTarget->m_dwSessionID, (LPCSTR)strMsg, SYSTEMMSGTYPE_NOLOG);

				PacketCHAR_STATUS.Make (pCharTarget);
				m_pSock->SendTo (pCharTarget->m_dwSessionID, &PacketCHAR_STATUS);
			}
			/* �G�t�F�N�g�̐ݒ� */
			if (pInfoItemType->m_dwUseEffectID != 0) {
				pCharTarget->m_nReserveChgEfect = (int)pInfoItemType->m_dwUseEffectID;
			}

		/* �_���[�W */
		} else {
			if (pInfoChar == pCharTarget) {
				continue;
			}
			Damage (pInfoChar, pCharTarget, abs (nPoint), (int)pInfoItemType->m_dwUseEffectID, FALSE);
		}
	}

	nRet = -1;
	/* �����Ȃ邩���� */
	if (pInfoItemType->m_byDelAverage != 0) {
		if (genrand () % 100 >= 100 - pInfoItemType->m_byDelAverage) {
			nRet = 1;
		}
	}
	/* ���ʉ��̍Đ� */
	if (pInfoItemType->m_dwUseSoundID != 0) {
		PacketPLAYSOUND.Make (pInfoItemType->m_dwUseSoundID);
		m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketPLAYSOUND);
	}

Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::UseItemProcLIGHT								 */
/* ���e		:�A�C�e���g�p����(����)											 */
/* ���t		:2008/10/19														 */
/* ========================================================================= */

int CLibInfoCharSvr::UseItemProcLIGHT(CInfoCharSvr *pInfoChar, DWORD dwItemID)
{
	int nRet;
	PCInfoItem pInfoItem;
	PCInfoItemTypeBase pInfoItemType;
	CPacketADMIN_PLAYSOUND PacketPLAYSOUND;
	CmyString strMsg;

	nRet = 0;

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		goto Exit;
	}
	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (pInfoItem->m_dwItemTypeID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}

	pInfoChar->m_nLightLevel = (int)pInfoItemType->m_dwValue;
	pInfoChar->m_dwLightTime = timeGetTime () + pInfoItemType->m_dwValue2;
	pInfoChar->m_bChgStatus = TRUE;

	strMsg.Format ("%s ���g���܂���", (LPCSTR)pInfoItem->m_strName);
	SendSystemMsg (pInfoChar->m_dwSessionID, (LPCSTR)strMsg, SYSTEMMSGTYPE_NOLOG);

	nRet = -1;
	/* �����Ȃ邩���� */
	if (pInfoItemType->m_byDelAverage != 0) {
		if (genrand () % 100 >= 100 - pInfoItemType->m_byDelAverage) {
			nRet = 1;
		}
	}
	/* ���ʉ��̍Đ� */
	if (pInfoItemType->m_dwUseSoundID != 0) {
		PacketPLAYSOUND.Make (pInfoItemType->m_dwUseSoundID);
		m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketPLAYSOUND);
	}

Exit:
	return nRet;
}

/* Copyright(C)URARA-works 2007 */
