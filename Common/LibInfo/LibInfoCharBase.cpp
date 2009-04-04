/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoCharBase.cpp										 */
/* ���e			:�L���������N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include <tchar.h>
#include <math.h>
#include "LibInfoCharBase.h"


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::CLibInfoCharBase								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

CLibInfoCharBase::CLibInfoCharBase()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::~CLibInfoCharBase							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

CLibInfoCharBase::~CLibInfoCharBase()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CLibInfoCharBase::Create(void)
{
	m_paInfo = new ARRAYINFOCHARBASE;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CLibInfoCharBase::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::Proc											 */
/* ���e		:����															 */
/* �߂�l	:TRUE:�������� FALS:�������Ă��Ȃ�								 */
/* ���t		:2007/01/21														 */
/* ========================================================================= */

BOOL CLibInfoCharBase::Proc(void)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::RenewSize									 */
/* ���e		:�}�b�v�T�C�Y�X�V												 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

void CLibInfoCharBase::RenewSize(DWORD dwMapID, int nDirection, int nSize)
{
	int i, nCount;
	PCInfoCharBase pInfoCharBase;

	/* �}�b�v�T�C�Y���L�����T�C�Y�ɕύX */
	nSize *= 2;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharBase = (PCInfoCharBase)GetPtr (i);

		if (pInfoCharBase->m_dwMapID != dwMapID) {
			continue;
		}

		switch (nDirection) {
		case 0:		/* �� */
			pInfoCharBase->m_nMapY += nSize;
			pInfoCharBase->m_nMapY = max (pInfoCharBase->m_nMapY, 0);
			break;
		case 1:		/* �� */
			break;
		case 2:		/* �� */
			pInfoCharBase->m_nMapX += nSize;
			pInfoCharBase->m_nMapX = max (pInfoCharBase->m_nMapX, 0);
			break;
		case 3:		/* �E */
			break;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2007/01/21														 */
/* ========================================================================= */

PCInfoBase CLibInfoCharBase::GetNew(void)
{
	return (PCInfoBase)GetNew (CHARMOVETYPE_PC);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoCharBase::GetNew(int nType)
{
	return (PCInfoBase)new CInfoCharBase;
}


/* ========================================================================= */
/* �֐���	:CLibInfoBase::GetSaveNo										 */
/* ���e		:�ۑ�������̃C���f�b�N�X���擾								 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CLibInfoCharBase::GetSaveNo(ARRAYINT &anDst)
{
	int i, nCount;
	PCInfoCharBase pChar;

	anDst.RemoveAll ();

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr (i);
		if (pChar->IsLogoutDelete ()) {
			continue;
		}

		anDst.Add (i);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetCount										 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

int CLibInfoCharBase::GetCount(void)
{
	int nRet;

	nRet = 0;

	if (m_paInfo == NULL) {
		goto Exit;
	}

	nRet = m_paInfo->GetSize ();
Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetCountScreen								 */
/* ���e		:�f�[�^�����擾(��ʓ��̃L����)									 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

int CLibInfoCharBase::GetCountScreen(PCInfoCharBase pCharBase)
{
	BOOL bResult;
	int nRet, i, nCount;
	PCInfoCharBase pChar;

	nRet = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr (i);

		bResult = IsScreenInside (pCharBase, pChar);
		if (bResult == FALSE) {
			continue;
		}
		nRet ++;
	}

	return nRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::Add											 */
/* ���e		:�ǉ�															 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CLibInfoCharBase::Add(PCInfoBase pInfo)
{
	Add ((PCInfoCharBase)pInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::Add											 */
/* ���e		:�ǉ�															 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

DWORD CLibInfoCharBase::Add(PCInfoCharBase pChar)
{
	if (pChar->m_dwCharID == 0) {
		pChar->m_dwCharID = GetNewID ();
	}

	m_paInfo->Add (pChar);

	return pChar->m_dwCharID;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::CopyAdd										 */
/* ���e		:�R�s�[���Ēǉ�													 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

void CLibInfoCharBase::CopyAdd(PCInfoCharBase pChar)
{
	PCInfoCharBase pCharTmp;

	pCharTmp = (PCInfoCharBase)GetNew (pChar->m_nMoveType);
	pCharTmp->Copy (pChar);

	m_paInfo->Add (pCharTmp);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CLibInfoCharBase::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoCharBase pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CLibInfoCharBase::Delete(
	DWORD dwCharID)		/* [in] �L����ID */
{
	int i, nCount, nNo;
	PCInfoCharBase pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwCharID != dwCharID) {
			continue;
		}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete (nNo);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::DeleteAll									 */
/* ���e		:�S�č폜														 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CLibInfoCharBase::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		Delete (i);
	}
	m_dwNewIDTmp = 0;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::SortY										 */
/* ���e		:Y���W���Ƀ\�[�g												 */
/* ���t		:2006/10/08														 */
/* ========================================================================= */

void CLibInfoCharBase::SortY(void)
{
	int i, j, nCount, nCountTmp, nNo;
	PCInfoCharBase pInfo, pInfoTmp;
	ARRAYINFOCHARBASE aTmp;
	ARRAYINT anNo;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		nCountTmp = anNo.GetSize ();
		nNo = 0;
		if (nCountTmp > 0) {
			for (j = 0; j < nCountTmp; j ++) {
				pInfoTmp = m_paInfo->GetAt (anNo[j]);
				if (pInfo->m_dwMapID < pInfoTmp->m_dwMapID) {
					break;
				}
				if (pInfo->m_dwMapID == pInfoTmp->m_dwMapID) {
					if (pInfo->m_nMapY < pInfoTmp->m_nMapY) {
						break;
					}
				}
				if (pInfo->m_dwMapID == pInfoTmp->m_dwMapID) {
					if (pInfo->m_nMapY == pInfoTmp->m_nMapY) {
						if (pInfo->m_nMapX < pInfoTmp->m_nMapX) {
							break;
						}
					}
				}
				if (pInfo->m_dwMapID == pInfoTmp->m_dwMapID) {
					if (pInfo->m_nMapY == pInfoTmp->m_nMapY) {
						if (pInfo->m_nMapX == pInfoTmp->m_nMapX) {
							if (pInfo->m_dwCharID < pInfoTmp->m_dwCharID) {
								break;
							}
						}
					}
				}
			}
			nNo = j;
		}
		anNo.InsertAt (nNo, i);
		aTmp.InsertAt (nNo, pInfo);
	}

	m_paInfo->Copy (&aTmp);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::IsBlockChar									 */
/* ���e		:����O�łԂ��邩�`�F�b�N										 */
/* �߂�l	:TRUE:�Ԃ���													 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

BOOL CLibInfoCharBase::IsBlockChar(PCInfoCharBase pChar, int nDirection)
{
	BOOL bRet, bResult;
	int i, nCount, nDirectionBack;
	PCInfoCharBase pInfoCharTmp;
	POINT ptFront;
	SIZE size, sizeTmp;

	bRet = FALSE;

	nDirectionBack = pChar->m_nDirection;
	pChar->m_nDirection = nDirection;
	pChar->GetFrontPos (ptFront, nDirection, TRUE);
	pChar->GetCharSize (size);

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->GetAt (i);
		bResult = pInfoCharTmp->IsLogin ();
		if (bResult == FALSE) {
			continue;
		}
		if (pChar == pInfoCharTmp) {
			continue;
		}
		if (pChar->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
		}
		GetDistance (sizeTmp, pChar, pInfoCharTmp, TRUE);
		if ((sizeTmp.cx < 0) || (sizeTmp.cx + sizeTmp.cy >= 1)) {
			continue;
		}
		if (pInfoCharTmp->m_bBlock == FALSE) {
			continue;
		}
		bRet = TRUE;
		break;
	}
	pChar->m_nDirection = nDirectionBack;

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::IsUseName									 */
/* ���e		:���O���g�p����Ă��邩�`�F�b�N									 */
/* ���t		:2006/12/03														 */
/* ========================================================================= */

BOOL CLibInfoCharBase::IsUseName(LPCSTR pszName)
{
	BOOL bRet;
	int i, nCount;
	PCInfoCharBase pInfo;

	bRet = FALSE;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (pInfo->m_strCharName == pszName) {
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::NameCheck									 */
/* ���e		:���O�Ɏg�p�ł��Ȃ����O�����邩�`�F�b�N							 */
/* �߂�l	:TRUE:��薳��													 */
/* ���t		:2007/02/22														 */
/* ========================================================================= */

BOOL CLibInfoCharBase::NameCheck(LPCSTR pszName)
{
	int i, nLen;
	BYTE byTmp;
	BOOL bRet;
	LPCSTR pszTmp;

	bRet = FALSE;

	if (pszName == NULL) {
		goto Exit;
	}
	nLen = strlen (pszName);
	if (nLen <= 0) {
		goto Exit;
	}
	pszTmp = _tcsstr (pszName, " ");
	if (pszTmp) {
		goto Exit;
	}
	pszTmp = _tcsstr (pszName, "�@");
	if (pszTmp) {
		goto Exit;
	}
	pszTmp = _tcsstr (pszName, "\t");
	if (pszTmp) {
		goto Exit;
	}

	for (i = 0; i < nLen; i ++) {
		byTmp = (BYTE)pszName[i];
		if (IsDBCSLeadByte (byTmp)) {
			i ++;
			continue;
		}
		if ((byTmp < 0x20) || ((byTmp >= 0x7F) && !((byTmp >= 0xA1) && (byTmp <= 0xDF)))) {
			goto Exit;
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetTurnDirection								 */
/* ���e		:�t�������擾													 */
/* ���t		:2007/09/02														 */
/* ========================================================================= */

int CLibInfoCharBase::GetTurnDirection(int nDirection)
{
	int anTurnDirection[] = {1, 0, 3, 2, 6, 7, 4, 5};

	return anTurnDirection[nDirection];
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetDistance									 */
/* ���e		:�L�������W�ŋ������擾											 */
/* ���t		:2009/01/17														 */
/* ========================================================================= */

void CLibInfoCharBase::GetDistance(SIZE &sizeDst, PCInfoCharBase pInfoCharSrc, PCInfoCharBase pInfoCharDst, BOOL bFrontPos/*FALSE*/)
{
	RECT rcSrc, rcDst;

	sizeDst.cx = sizeDst.cy = -1;
	if (pInfoCharSrc->m_dwMapID != pInfoCharDst->m_dwMapID) {
		return;
	}
	/* ��r���̍��W��`���擾 */
	pInfoCharSrc->GetPosRect (rcSrc, bFrontPos);
	/* ��r��̍��W��`���擾 */
	pInfoCharDst->GetPosRect (rcDst);

	sizeDst.cx = rcSrc.left - rcDst.right;
	if (pInfoCharSrc->m_nMapX < pInfoCharDst->m_nMapX) {
		sizeDst.cx = rcDst.left - rcSrc.right;

	} else if (pInfoCharSrc->m_nMapX == pInfoCharDst->m_nMapX) {
		sizeDst.cx = 0;
	}
	sizeDst.cx = max (sizeDst.cx, 0);

	sizeDst.cy = rcSrc.top - rcDst.bottom;
	if (pInfoCharSrc->m_nMapY < pInfoCharDst->m_nMapY) {
		sizeDst.cy = rcDst.top - rcSrc.bottom;

	} else if (pInfoCharSrc->m_nMapY == pInfoCharDst->m_nMapY) {
		sizeDst.cy = 0;
	}
	sizeDst.cy = max (sizeDst.cy, 0);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::IsScreenInside								 */
/* ���e		:��ʓ��ɂ��邩�`�F�b�N											 */
/* �߂�l	:TRUE:��ʓ� FALSE:��ʊO										 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

BOOL CLibInfoCharBase::IsScreenInside(
	PCInfoCharBase pCharBase,		/* [in] ��ƂȂ�L���� */
	PCInfoCharBase pCharTarget)		/* [in] �`�F�b�N�Ώۂ̃L���� */
{
	BOOL bRet, bResult;

	bRet = FALSE;

	/* ���O�C�����Ă��Ȃ��H */
	bResult = pCharTarget->IsLogin ();
	if (bResult == FALSE) {
		goto Exit;
	}
	/* �ʂ̃}�b�v�ɂ���H */
	if (pCharBase->m_dwMapID != pCharTarget->m_dwMapID) {
		goto Exit;
	}
	if (!(
		(abs (pCharBase->m_nMapX - pCharTarget->m_nMapX) < DRAW_PARTS_X * 2) &&
		(abs (pCharBase->m_nMapY - pCharTarget->m_nMapY) < DRAW_PARTS_Y * 2))) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetFrontCharID								 */
/* ���e		:����O�̃L����ID���擾											 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

DWORD CLibInfoCharBase::GetFrontCharID(DWORD dwCharID, int nDirection)
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
	pInfoCharSrc->GetFrontPos (ptFront, nDirection, TRUE);
	pInfoCharSrc->GetCharSize (size);

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->GetAt (i);
		if (pInfoCharTmp == pInfoCharSrc) {
			continue;
		}
		if (pInfoCharSrc->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
		}
		if (pInfoCharTmp->IsHitCharPos (ptFront.x, ptFront.y, &size) == FALSE) {
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
/* �֐���	:CLibInfoCharBase::GetFrontCharIDPush							 */
/* ���e		:����O�̉�����L����ID���擾									 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

DWORD CLibInfoCharBase::GetFrontCharIDPush(DWORD dwCharID, int nDirection)
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
	pInfoCharSrc->GetFrontPos (ptFront, nDirection, TRUE);
	pInfoCharSrc->GetCharSize (size);

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->GetAt (i);
		if (pInfoCharSrc == pInfoCharTmp) {
			continue;
		}
		if (pInfoCharTmp->m_bPush == FALSE) {
			continue;
		}
		if (pInfoCharTmp->m_nMoveType == CHARMOVETYPE_PUTNPC) {
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
/* �֐���	:CLibInfoCharBase::GetHitCharID									 */
/* ���e		:�w����W�ɓ�����L����ID���擾									 */
/* ���t		:2008/12/29														 */
/* ========================================================================= */

DWORD CLibInfoCharBase::GetHitCharID(DWORD dwCharIDBase, int x, int y)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoCharBase pInfoCharBase, pInfoCharTmp;
	SIZE size;

	dwRet = 0;

	pInfoCharBase = (PCInfoCharBase)GetPtr (dwCharIDBase);
	if (pInfoCharBase == NULL) {
		goto Exit;
	}
	pInfoCharBase->GetCharSize (size);

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->GetAt (i);
		if (pInfoCharBase == pInfoCharTmp) {
			continue;
		}
		if (pInfoCharTmp->m_nMoveType == CHARMOVETYPE_PUTNPC) {
			continue;
		}
		if (pInfoCharBase->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
		}
		if (pInfoCharTmp->IsHitCharPos (x, y, &size) == FALSE) {
			continue;
		}
		dwRet = pInfoCharTmp->m_dwCharID;
		break;
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::SetPtr										 */
/* ���e		:�L���������X�V												 */
/* ���t		:2007/09/02														 */
/* ========================================================================= */

void CLibInfoCharBase::SetPtr(DWORD dwCharID, PCInfoCharBase pChar)
{
	int i, nCount;
	PCInfoCharBase pTmp;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pTmp = (PCInfoCharBase)m_paInfo->GetAt (i);
		if (pTmp->m_dwCharID == dwCharID) {
			SAFE_DELETE (pTmp);
			m_paInfo->SetAt (i, pChar);
			break;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetPtr										 */
/* ���e		:�L���������擾												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoCharBase::GetPtr(int nNo)
{
	return (PCInfoBase)m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetPtr										 */
/* ���e		:�L���������擾												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoCharBase::GetPtr(
	DWORD dwCharID)		/* [in] �L����ID */
{
	int i, nCount;
	PCInfoCharBase pRet, pInfoTmp;

	pRet = NULL;

	if (dwCharID == 0) {
		return NULL;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwCharID != dwCharID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return (PCInfoBase)pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetPtrAccountID								 */
/* ���e		:�L���������擾												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoCharBase::GetPtrAccountID(
	DWORD dwAccountID)		/* [in] �A�J�E���gID */
{
	int i, nCount;
	PCInfoCharBase pRet, pInfoTmp;

	pRet = NULL;

	if (dwAccountID == 0) {
		return NULL;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwAccountID != dwAccountID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return (PCInfoBase)pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetPtrFront									 */
/* ���e		:�L���������擾												 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoCharBase::GetPtrFront(
	PCInfoCharBase pChar,		/* [in] ��ƂȂ�L������� */
	int nDirection)				/* [in] ���� */
{
	BOOL bResult;
	int i, j, nCount, nCount2;
	PCInfoCharBase pRet, pInfoCharTmp;
	CmyArray<POINT, POINT> aptFront;

	pRet = NULL;

	pChar->GetFrontPos (aptFront, nDirection);
	nCount2 = aptFront.GetSize ();

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->GetAt (i);
		bResult = pInfoCharTmp->IsLogin ();
		if (bResult == FALSE) {
			continue;
		}
		if (pChar->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
		}
		for (j = 0; j < nCount2; j ++) {
			if (pInfoCharTmp->IsHitCharPos (aptFront[j].x, aptFront[j].y)) {
				break;
			}
		}
		if (j >= nCount2) {
			continue;
		}
		pRet = pInfoCharTmp;
		break;
	}

	return (PCInfoBase)pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

DWORD CLibInfoCharBase::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoCharBase pChar;

	dwRet = dwSize = 0;

	/* �f�[�^�����̃T�C�Y */
	dwSize += sizeof (DWORD);

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr (i);

		dwSize += pChar->GetSendDataSize ();
	}

	dwRet = dwSize;
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

PBYTE CLibInfoCharBase::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoCharBase pChar;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	dwOffset	= 0;

	/* �f�[�^������������ */
	dwCount = (DWORD)GetCount ();
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* �L���������������� */
	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr (i);

		dwSizeTmp	= pChar->GetSendDataSize ();
		pDataTmp	= pChar->GetSendData ();
		CopyMemory (&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY (pDataTmp);
	}

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/01/27														 */
/* �߂�l	:����������̃A�h���X											 */
/* ========================================================================= */

PBYTE CLibInfoCharBase::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp;
	CInfoCharBase *pChar;

	pRet		= pSrc;
	pDataTmp	= pSrc;

	DeleteAll ();

	/* �f�[�^����ǂݍ��� */
	CopyMemory (&dwCount, pDataTmp, sizeof (dwCount));
	nCount		= (int)dwCount;
	pDataTmp	+= sizeof (dwCount);

	for (i = 0; i < nCount; i ++) {
		CInfoCharBase CharTmp;

		pDataTmp	= CharTmp.SetSendData (pDataTmp);
		pChar		= (PCInfoCharBase)GetNew (CharTmp.m_nMoveType);
		pChar->Copy (&CharTmp);
		Add (pChar);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetDataSizeScreen							 */
/* ���e		:�f�[�^�T�C�Y���擾(��ʓ��̃L����)								 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

DWORD CLibInfoCharBase::GetDataSizeScreen(PCInfoCharBase pCharBase)
{
	BOOL bResult;
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoCharBase pChar;

	dwRet = dwSize = 0;

	/* �f�[�^�����̃T�C�Y */
	dwSize += sizeof (DWORD);

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr (i);

		/* ��ʊO�H */
		bResult = IsScreenInside (pCharBase, pChar);
		if (bResult == FALSE) {
			continue;
		}
		dwSize += pChar->GetSendDataSize ();
	}

	dwRet = dwSize;
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetDataScreen								 */
/* ���e		:�f�[�^���擾(��ʓ��̃L����)									 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

PBYTE CLibInfoCharBase::GetDataScreen(PCInfoCharBase pCharBase)
{
	BOOL bResult;
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoCharBase pChar;

	pRet = NULL;

	dwSize		= GetDataSizeScreen (pCharBase);
	pData		= ZeroNew (dwSize);
	dwOffset	= 0;

	/* �f�[�^������������ */
	dwCount = (DWORD)GetCountScreen (pCharBase);
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* �L���������������� */
	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr (i);

		if (pCharBase->m_dwMapID != pChar->m_dwMapID) {
			continue;
		}
		/* ��ʊO�H */
		bResult = IsScreenInside (pCharBase, pChar);
		if (bResult == FALSE) {
			continue;
		}
		dwSizeTmp	= pChar->GetSendDataSize ();
		pDataTmp	= pChar->GetSendData ();
		CopyMemory (&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY (pDataTmp);
	}

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharBase::GetNewID										 */
/* ���e		:�V�����L����ID���擾											 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

DWORD CLibInfoCharBase::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoCharBase pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwCharID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2006 */
