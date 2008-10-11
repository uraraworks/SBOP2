/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LayerSystemMsg.cpp											 */
/* ���e			:���C���[�`��N���X(�V�X�e�����b�Z�[�W) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/30													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "LayerSystemMsg.h"


/* ========================================================================= */
/* �֐���	:CLayerSystemMsg::CLayerSystemMsg								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

CLayerSystemMsg::CLayerSystemMsg()
{
	m_nID				= LAYERTYPE_SYSTEMMSG;
	m_dwLastTimeProc	= 0;
}


/* ========================================================================= */
/* �֐���	:CLayerSystemMsg::~CLayerSystemMsg								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

CLayerSystemMsg::~CLayerSystemMsg()
{
	DeleteAllMsg ();
}


/* ========================================================================= */
/* �֐���	:CLayerSystemMsg::Draw											 */
/* ���e		:�`��															 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

void CLayerSystemMsg::Draw(PCImg32 pDst)
{
	int i, nCount;
	PSYSTEMMSGINFO pInfo;

	nCount = m_aSystemMsgInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aSystemMsgInfo[i];
		pDst->Blt (
				38, pInfo->nPosY,
				pInfo->pImg->Width (), pInfo->pImg->Height (),
				pInfo->pImg,
				0, 0, TRUE);
	}
}


/* ========================================================================= */
/* �֐���	:CLayerSystemMsg::TimerProc										 */
/* ���e		:���ԏ���														 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

BOOL CLayerSystemMsg::TimerProc(void)
{
	BOOL bRet;
	int i, nCount;
	DWORD dwTmp;
	PSYSTEMMSGINFO pInfo;

	bRet = FALSE;

	dwTmp = timeGetTime () - m_dwLastTimeProc;
	if (dwTmp < 50) {
		goto Exit;
	}
	m_dwLastTimeProc = timeGetTime ();

	nCount = m_aSystemMsgInfo.GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_aSystemMsgInfo[i];
		pInfo->nPosY --;
		if (pInfo->nPosY > SCRSIZEY - (SCRSIZEY / 3)) {
			continue;
		}
		DeleteMsg (i);
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLayerSystemMsg::AddMsg										 */
/* ���e		:�`��															 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

void CLayerSystemMsg::AddMsg(LPCSTR pszMsg, COLORREF cl)
{
	int i, nLen, nCount;
	HDC hDCTmp;
	HFONT hFontOld;
	PSYSTEMMSGINFO pInfo, pInfoTmp;

	nCount = m_aSystemMsgInfo.GetSize ();

	pInfo = new SYSTEMMSGINFO;
	pInfo->nPosY	= SCRSIZEY;
	pInfo->pImg		= new CImg32;

	if (nCount > 0) {
		pInfoTmp = m_aSystemMsgInfo[nCount - 1];
		/* �ǉ�����Ɗ����̃��b�Z�[�W�ɏd�Ȃ�H */
		if (pInfo->nPosY <= pInfoTmp->nPosY + 14) {
			for (i = 0; i < nCount; i ++) {
				pInfoTmp = m_aSystemMsgInfo[i];
				pInfoTmp->nPosY -= 14;
			}
		}
	}

	nLen = strlen (pszMsg);
	pInfo->pImg->Create (nLen * 14 + 1, 14);

	hDCTmp		= pInfo->pImg->Lock ();
	hFontOld	= (HFONT)SelectObject (hDCTmp, m_hFont);
	SetBkMode (hDCTmp, TRANSPARENT);
	TextOut2 (hDCTmp, 1, 1, pszMsg, cl);

	SelectObject (hDCTmp, hFontOld);
	pInfo->pImg->Unlock ();

	m_aSystemMsgInfo.Add (pInfo);
}


/* ========================================================================= */
/* �֐���	:CLayerSystemMsg::DeleteMsg										 */
/* ���e		:���b�Z�[�W���폜												 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

void CLayerSystemMsg::DeleteMsg(int nNo)
{
	PSYSTEMMSGINFO pInfo;

	pInfo = m_aSystemMsgInfo[nNo];
	SAFE_DELETE (pInfo->pImg);
	SAFE_DELETE (pInfo);
	m_aSystemMsgInfo.RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLayerSystemMsg::DeleteAllMsg									 */
/* ���e		:�S�Ẵ��b�Z�[�W���폜											 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

void CLayerSystemMsg::DeleteAllMsg(void)
{
	int i, nCount;

	nCount = m_aSystemMsgInfo.GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		DeleteMsg (i);
	}
}

/* Copyright(C)URARA-works 2007 */
