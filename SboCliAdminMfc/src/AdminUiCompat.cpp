#include "StdAfx.h"

#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
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

// Compatibility shims for Step4-B staged migration.
// MgrDraw/MgrLayer/MgrWindow/MgrData/InfoCharCli are excluded from this DLL project
// because they pull in the entire game-client dependency tree.
// Only the symbols actually referenced by migrated Admin UI code are shimmed here.
// Note: CLibInfoItem::GetTypeName shim removed (real impl in LibInfoItem.cpp).

// --- CLibInfoCharCli ---
CInfoBase* CLibInfoCharCli::GetPtr(DWORD dwCharID)
{
	UNREFERENCED_PARAMETER(dwCharID);
	return NULL;
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
	UNREFERENCED_PARAMETER(pMgrData);
}

void CMgrDraw::DrawMapParts(CImg32 *pDst, int x, int y, DWORD dwPartsID,
	int nMode, BOOL bSingleSize, BOOL bUseColorKey, BYTE byLevel)
{
	UNREFERENCED_PARAMETER(pDst);         UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);            UNREFERENCED_PARAMETER(dwPartsID);
	UNREFERENCED_PARAMETER(nMode);        UNREFERENCED_PARAMETER(bSingleSize);
	UNREFERENCED_PARAMETER(bUseColorKey); UNREFERENCED_PARAMETER(byLevel);
}

void CMgrDraw::DrawMapParts(CImg32 *pDst, int x, int y, CInfoMapParts *pInfoMapParts,
	int nMode, BOOL bSingleSize, BOOL bUseColorKey, BOOL bLock, BYTE byLevel)
{
	UNREFERENCED_PARAMETER(pDst);          UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);             UNREFERENCED_PARAMETER(pInfoMapParts);
	UNREFERENCED_PARAMETER(nMode);         UNREFERENCED_PARAMETER(bSingleSize);
	UNREFERENCED_PARAMETER(bUseColorKey);  UNREFERENCED_PARAMETER(bLock);
	UNREFERENCED_PARAMETER(byLevel);
}

void CMgrDraw::DrawMapShadow(CImg32 *pDst, int x, int y, DWORD dwShadowID,
	BOOL bSingleSize, BOOL bUseColorKey, BOOL bLock)
{
	UNREFERENCED_PARAMETER(pDst);         UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);            UNREFERENCED_PARAMETER(dwShadowID);
	UNREFERENCED_PARAMETER(bSingleSize);  UNREFERENCED_PARAMETER(bUseColorKey);
	UNREFERENCED_PARAMETER(bLock);
}

void CMgrDraw::DrawIcon(CImg32 *pDst, int x, int y, int nIndex)
{
	UNREFERENCED_PARAMETER(pDst); UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);    UNREFERENCED_PARAMETER(nIndex);
}

// --- CMgrData shim ---
// ctor/dtor and non-inline methods needed by excluded DlgAdmin*.cpp.
CMgrData::CMgrData()  {}
CMgrData::~CMgrData() {}

DWORD  CMgrData::GetMapID(void)                     { return 0;  }
LPCSTR CMgrData::GetMotionName(DWORD dwMotionID)    { UNREFERENCED_PARAMETER(dwMotionID); return ""; }
LPCSTR CMgrData::GetMotionProcName(DWORD dwProcID)  { UNREFERENCED_PARAMETER(dwProcID);   return ""; }
void   CMgrData::Destroy(void)                      {}
LPCSTR CMgrData::GetFamilyTypeName(WORD wFamilyTypeID)  { UNREFERENCED_PARAMETER(wFamilyTypeID); return ""; }
LPCSTR CMgrData::GetMapEventName(int nMapEventType)     { UNREFERENCED_PARAMETER(nMapEventType);  return ""; }
CInfoTalkEvent* CMgrData::GetInfoTalkEvent(void)        { return NULL; }

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
