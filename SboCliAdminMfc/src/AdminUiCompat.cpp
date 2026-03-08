#include "StdAfx.h"

#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "InfoAnime.h"
#include "InfoMapParts.h"
#include "InfoMapShadow.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapShadow.h"
#include "MgrLayer.h"   // CMgrLayer::Draw (referenced by MgrDraw.cpp)

// MgrDraw.h references CMgrGrpData and CLibInfoMotionType without forward-declaring them.
// Include their headers first so MgrDraw.h compiles cleanly.
#include "MgrGrpData.h"
#include "LibInfoMotionType.h"
#include "MgrDraw.h"
#include "DataSlot.h"   // CDataSlot (base of CMgrData)
#include "MgrData.h"
#include "MgrSound.h"
#include "MgrWindow.h"
#include "GlobalDefine.h"
#include "InfoMotion.h"
#include "InfoMapEventBase.h"

// Compatibility shims for Step4-B staged migration.
// MgrDraw/MgrLayer/MgrWindow/MgrData/InfoCharCli are excluded from this DLL project
// because they pull in the entire game-client dependency tree.
// Only the symbols actually referenced by migrated Admin UI code are shimmed here.
// Note: CLibInfoItem::GetTypeName shim removed (real impl in LibInfoItem.cpp).

// --- CLibInfoCharCli ---
CInfoBase* CLibInfoCharCli::GetPtr(DWORD dwCharID)
{
	return CLibInfoCharBase::GetPtr (dwCharID);
}

// --- CInfoCharCli shim ---
// DlgAdminCharModify* creates a temporary CInfoCharCli object.
// Empty implementation sufficient for Admin DLL side.
CInfoCharCli::CInfoCharCli()  {}
CInfoCharCli::~CInfoCharCli() {}

void CInfoCharCli::Create(CMgrData *pMgrData)
{
	UNREFERENCED_PARAMETER(pMgrData);
}

CImg32* CInfoCharCli::GetEfcImg(void)     { return NULL;  }
int     CInfoCharCli::GetEfcLevel(void)   { return 0;     }
int     CInfoCharCli::GetEfcGrpSize(void) { return 0;     }
BOOL    CInfoCharCli::IsDamage(void)      { return FALSE; }

// Additional virtual and non-virtual methods required by DlgAdminCharModify* and MgrDraw.
void CInfoCharCli::ChgMoveState(int nMoveState)   { UNREFERENCED_PARAMETER(nMoveState); }
BOOL CInfoCharCli::TimerProc(DWORD dwTime)         { UNREFERENCED_PARAMETER(dwTime); return FALSE; }
int CInfoCharCli::SetPos(int x, int y, BOOL bBack) { return CInfoCharBase::SetPos(x, y, bBack); }
void CInfoCharCli::SetMoveState(int nMoveState)    { UNREFERENCED_PARAMETER(nMoveState); }
void CInfoCharCli::SetName(LPCSTR pszName)         { UNREFERENCED_PARAMETER(pszName); }
void CInfoCharCli::SetSpeak(LPCSTR pszSpeak)       { UNREFERENCED_PARAMETER(pszSpeak); }
void CInfoCharCli::SetViewState(int nViewState)    { UNREFERENCED_PARAMETER(nViewState); }
CInfoMotion* CInfoCharCli::GetMotionInfo(int *pnCount) { UNREFERENCED_PARAMETER(pnCount); return NULL; }
BOOL CInfoCharCli::IsEnableMove(void)              { return FALSE; }
BOOL CInfoCharCli::RenewAnime(DWORD dwTime, int nAdd) { UNREFERENCED_PARAMETER(dwTime); UNREFERENCED_PARAMETER(nAdd); return FALSE; }
void CInfoCharCli::GetGrpID(WORD &wGrpIDMain, WORD &wGrpIDSub)
    { wGrpIDMain = 0; wGrpIDSub = 0; }
void CInfoCharCli::GetEfcGrpPos(POINT &ptDst) { ptDst.x = 0; ptDst.y = 0; }

// --- CMgrLayer shim ---
// MgrDraw.cpp calls CMgrLayer::Draw. MgrLayer.cpp excluded to avoid CLayer* chain.
CMgrLayer::CMgrLayer()  {}
CMgrLayer::~CMgrLayer() {}
void CMgrLayer::Draw(CImg32 *pDst) { UNREFERENCED_PARAMETER(pDst); }

// --- CMgrDraw shim ---
// WndMap.cpp calls CMgrDraw::Create and DrawMapParts.
// MgrDraw.cpp excluded to avoid pulling in CMgrWindow/CMgrLayer chains.
CMgrDraw::CMgrDraw()  {}
CMgrDraw::~CMgrDraw() {}

void CMgrDraw::Create(CMgrData *pMgrData)
{
	m_pMgrData = pMgrData;
	m_pMgrGrpData = (pMgrData != NULL) ? pMgrData->GetMgrGrpData () : NULL;
	m_pLibInfoMapParts = (pMgrData != NULL) ? pMgrData->GetLibInfoMapParts () : NULL;
	m_pLibInfoMapShadow = (pMgrData != NULL) ? pMgrData->GetLibInfoMapShadow () : NULL;
}

void CMgrDraw::DrawMapParts(CImg32 *pDst, int x, int y, DWORD dwPartsID,
	int nMode, BOOL bSingleSize, BOOL bUseColorKey, BYTE byLevel)
{
	CInfoMapParts *pInfoMapParts;

	if (m_pLibInfoMapParts == NULL) {
		return;
	}
	pInfoMapParts = (CInfoMapParts *)m_pLibInfoMapParts->GetPtr (dwPartsID);
	DrawMapParts (pDst, x, y, pInfoMapParts, nMode, bSingleSize, bUseColorKey, TRUE, byLevel);
}

void CMgrDraw::DrawMapParts(CImg32 *pDst, int x, int y, CInfoMapParts *pInfoMapParts,
	int nMode, BOOL bSingleSize, BOOL bUseColorKey, BOOL bLock, BYTE byLevel)
{
	int nSize, nSizeDst;
	WORD wGrpIDBase, wGrpIDPile;
	CImg32 ImgTmp;
	CImg32 *pImg;
	CInfoAnime *pAnime;
	HDC hDCBmp;

	UNREFERENCED_PARAMETER(bLock);

	if ((pDst == NULL) || (m_pMgrGrpData == NULL)) {
		return;
	}

	nSize = 16;
	nSizeDst = bSingleSize ? 16 : 32;
	ImgTmp.Create (nSizeDst, nSizeDst * 2);
	ImgTmp.SetColorKey (RGB (255, 0, 255));

	if (pInfoMapParts == NULL) {
		pDst->FillRect (x, y, nSizeDst, nSizeDst, RGB (0, 0, 0));
		return;
	}

	wGrpIDBase = pInfoMapParts->m_wGrpIDBase;
	wGrpIDPile = pInfoMapParts->m_wGrpIDPile;
	if (pInfoMapParts->GetAnimeCount () > 0) {
		pAnime = pInfoMapParts->m_paAnimeInfo->at (pInfoMapParts->m_byAnimeNo);
		wGrpIDBase = pAnime->m_wGrpIDBase;
		wGrpIDPile = pAnime->m_wGrpIDPile;
	}
	if (nMode == 0) {
		wGrpIDPile = 0;
	}
	if (nMode == 3) {
		if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_PILEBACK) {
			nMode = 2;
		} else if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_PILE) {
			nMode = 1;
		} else {
			return;
		}
	}
	if ((nMode == 1) && (wGrpIDPile == 0)) {
		return;
	}

	if (nMode != 1) {
		pImg = m_pMgrGrpData->GetDibMapParts (wGrpIDBase / 1024);
		if (pImg != NULL) {
			ImgTmp.BltFrom256 (0, 0, nSize, nSize, pImg,
				((wGrpIDBase % 1024) % 32) * nSize,
				((wGrpIDBase % 1024) / 32) * nSize);
		}
	} else {
		ImgTmp.FillRect (0, 0, nSize, nSize, RGB (255, 0, 255));
	}
	if (wGrpIDPile > 0) {
		pImg = m_pMgrGrpData->GetDibMapParts (wGrpIDPile / 1024);
		if (pImg != NULL) {
			ImgTmp.BltFrom256 (0, 0, nSize + 1, nSize, pImg,
				((wGrpIDPile % 1024) % 32) * nSize,
				((wGrpIDPile % 1024) / 32) * nSize,
				TRUE);
		}
	}

	hDCBmp = ImgTmp.Lock ();
	StretchBlt (hDCBmp, 0, nSizeDst, nSizeDst, nSizeDst, hDCBmp, 0, 0, nSize, nSize, SRCCOPY);
	ImgTmp.Unlock ();

	if (byLevel == 0) {
		pDst->Blt (x, y, nSizeDst, nSizeDst, &ImgTmp, 0, nSizeDst, bUseColorKey);
	} else {
		pDst->BltAlpha (x, y, nSizeDst, nSizeDst, &ImgTmp, 0, nSizeDst, byLevel, bUseColorKey);
	}
}

void CMgrDraw::DrawMapShadow(CImg32 *pDst, int x, int y, DWORD dwShadowID,
	BOOL bSingleSize, BOOL bUseColorKey, BOOL bLock)
{
	int nSize, nSizeDst;
	WORD wGrpIDBase;
	CImg32 ImgTmp;
	CImg32 *pImg;
	CInfoMapShadow *pInfoMapShadow;
	CInfoAnime *pAnime;
	HDC hDCBmp;

	UNREFERENCED_PARAMETER(bLock);

	if ((pDst == NULL) || (m_pMgrGrpData == NULL) || (m_pLibInfoMapShadow == NULL)) {
		return;
	}

	nSize = 16;
	nSizeDst = bSingleSize ? 16 : 32;
	ImgTmp.Create (nSizeDst, nSizeDst * 2);
	ImgTmp.SetColorKey (RGB (255, 0, 255));

	pInfoMapShadow = (CInfoMapShadow *)m_pLibInfoMapShadow->GetPtr (dwShadowID);
	if (pInfoMapShadow == NULL) {
		pDst->FillRect (x, y, nSizeDst, nSizeDst, RGB (0, 0, 0));
		return;
	}

	wGrpIDBase = pInfoMapShadow->m_wGrpID;
	if (pInfoMapShadow->GetAnimeCount () > 0) {
		pAnime = pInfoMapShadow->m_paAnimeInfo->at (pInfoMapShadow->m_byAnimeNo);
		wGrpIDBase = pAnime->m_wGrpIDBase;
	}

	pImg = bSingleSize
		? m_pMgrGrpData->GetDibMapShadowTmp (wGrpIDBase / 1024)
		: m_pMgrGrpData->GetDibMapShadow (wGrpIDBase / 1024);
	if (pImg == NULL) {
		return;
	}
	ImgTmp.BltFrom256 (0, 0, nSize, nSize, pImg,
		((wGrpIDBase % 1024) % 32) * nSize,
		((wGrpIDBase % 1024) / 32) * nSize);

	hDCBmp = ImgTmp.Lock ();
	StretchBlt (hDCBmp, 0, nSizeDst, nSizeDst, nSizeDst, hDCBmp, 0, 0, nSize, nSize, SRCCOPY);
	ImgTmp.Unlock ();

	if (bSingleSize) {
		pDst->Blt (x, y, nSizeDst, nSizeDst, &ImgTmp, 0, nSizeDst, bUseColorKey);
	} else {
		pDst->BltAlpha (x, y, nSizeDst, nSizeDst, &ImgTmp, 0, nSizeDst, 75, TRUE);
	}
}

void CMgrDraw::DrawIcon(CImg32 *pDst, int x, int y, int nIndex)
{
	int cx, cy;
	CImg32 ImgTmp;
	CImg32 *pImg;
	HDC hDCBmp;

	if ((pDst == NULL) || (m_pMgrGrpData == NULL)) {
		return;
	}

	cx = cy = 16;
	ImgTmp.Create (cx * 2, cy * 4);
	ImgTmp.SetColorKey (RGB (255, 0, 255));
	pImg = m_pMgrGrpData->GetDibIcon ();
	if (pImg == NULL) {
		return;
	}
	ImgTmp.BltFrom256 (0, 0, 16, 16, pImg, nIndex % 20 * cx, nIndex / 20 * cy);

	hDCBmp = ImgTmp.Lock ();
	StretchBlt (hDCBmp, 0, cy * 2, cx * 2, cy * 2, hDCBmp, 0, 0, cx, cy, SRCCOPY);
	ImgTmp.Unlock ();

	pDst->Blt (x, y, cx * 2, cy * 2, &ImgTmp, 0, cy * 2, TRUE);
}

// --- CMgrData shim ---
// ctor/dtor and non-inline methods needed by excluded DlgAdmin*.cpp.
CMgrData::CMgrData()  {}
CMgrData::~CMgrData() {}

DWORD CMgrData::GetMapID(void)
{
	CInfoCharCli *pPlayerChar;

	pPlayerChar = GetPlayerChar ();
	return pPlayerChar ? pPlayerChar->m_dwMapID : 0;
}

LPCSTR CMgrData::GetMotionName(DWORD dwMotionID)
{
	UNREFERENCED_PARAMETER(dwMotionID);
	return "";
}

LPCSTR CMgrData::GetMotionProcName(DWORD dwProcID)
{
	switch (dwProcID) {
	case CHARMOTIONPROCID_ATACK:	return "攻撃";
	case CHARMOTIONPROCID_FISHING:	return "釣り";
	default:						return "何もしない";
	}
}
void   CMgrData::Destroy(void)                      {}
LPCSTR CMgrData::GetFamilyTypeName(WORD wFamilyTypeID)
{
	switch (wFamilyTypeID) {
	case FAMILYTYPE_HUMAN:	return "ニンゲン";
	case FAMILYTYPE_ELF:	return "エルフ";
	case FAMILYTYPE_BST:	return "ジュウジン";
	case FAMILYTYPE_DAEMON:	return "マゾク";
	default:				return "";
	}
}

LPCSTR CMgrData::GetMapEventName(int nMapEventType)
{
	switch (nMapEventType) {
	case MAPEVENTTYPE_MOVE:			return "マップ内移動";
	case MAPEVENTTYPE_MAPMOVE:		return "マップ間移動";
	case MAPEVENTTYPE_TRASHBOX:		return "ゴミ箱";
	case MAPEVENTTYPE_INITSTATUS:	return "ステータス初期化";
	case MAPEVENTTYPE_GRPIDTMP:		return "一時画像設定";
	case MAPEVENTTYPE_LIGHT:		return "灯り";
	default:						return "未設定";
	}
}
/* GetInfoTalkEvent シムは削除。DlgAdminCharModify は Host API 経由で取得する */

// --- CMgrSound shim ---
// DlgAdminItemNew etc. call PlaySound for preview. Return silently.
void CMgrSound::PlaySound(DWORD dwSoundID)
{
	UNREFERENCED_PARAMETER(dwSoundID);
}

// --- CMgrWindow shim ---
// Referenced by MgrDraw before it was excluded. Keep for safety.
void CMgrWindow::Draw(CImg32 *pImg)
{
	UNREFERENCED_PARAMETER(pImg);
}
