/// @file DlgAdminItemTypeNew.cpp
/// @brief アイテム種別新規追加ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/30
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "resource.h"
#include "LibInfoItemType.h"
#include "LibInfoEffect.h"
#include "WndSelectItemGrp.h"
#include "MgrGrpData.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "LibSboSoundLoader.h"
#include "DlgAdminItemTypeNewARMS.h"
#include "DlgAdminItemTypeNewHP.h"
#include "DlgAdminItemTypeNewLIGHT.h"
#include "DlgAdminItemTypeNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminItemTypeNew::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemTypeNew)
	DDX_Control(pDX, IDC_TYPEGRPIDMAIN, m_ctlTypeGrpIDMain);
	DDX_Control(pDX, IDC_DROPSOUNDID, m_ctlDropSoundID);
	DDX_Control(pDX, IDC_USESOUNDID, m_ctlUseSoundID);
	DDX_Control(pDX, IDC_USEEFFECTID, m_ctlUseEffectID);
	DDX_Control(pDX, IDC_TYPE, m_ctlType);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_GRPID, m_nGrpID);
	DDX_Text(pDX, IDC_DELAVERAGE, m_nDelAverage);
	DDX_Check(pDX, IDC_PUTON, m_bPutOn);
	DDX_Text(pDX, IDC_GRPIDSUB, m_nGrpIDSub);
	DDX_Text(pDX, IDC_ICONGRPID, m_nIconGrpID);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminItemTypeNew, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminItemTypeNew)
	ON_BN_CLICKED(IDC_SELECT_ITEMGRP, OnSelectItemgrp)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	ON_BN_CLICKED(IDC_TYPEINFO, OnTypeinfo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WNDCLOSE, OnWndClose)
END_MESSAGE_MAP()


CDlgAdminItemTypeNew::CDlgAdminItemTypeNew(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminItemTypeNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminItemTypeNew)
	m_strName = _T("");
	m_nDelAverage = 0;
	m_nGrpID = 0;
	m_bPutOn = FALSE;
	m_nGrpIDSub = 0;
	m_nIconGrpID = 0;
	//}}AFX_DATA_INIT

	m_dwSelectItemTypeID	= 0;
	m_dwSelectSoundID	= 0;
	m_dwUseEffectID	= 0;
	m_dwUseSoundID	= 0;
	m_dwSelectGrpIDMain	= 0;
	m_bModeModify	= FALSE;
	m_ppWndNotify	= NULL;
	m_pMgrGrpData	= NULL;
	m_pWndSelectItemGrp	= NULL;
	m_pLibInfoItemType	= NULL;
	m_pInfoItemType	= NULL;
}


CDlgAdminItemTypeNew::~CDlgAdminItemTypeNew()
{
	SAFE_DELETE(m_pInfoItemType);
}


void CDlgAdminItemTypeNew::Init(CMgrData *pMgrData, CWnd **pWndNotify, BOOL bPosSet)
{
	CDlgAdminBase::Init(pMgrData);

	m_ppWndNotify	= pWndNotify;
	m_pLibInfoItemType	= m_pMgrData->GetLibInfoItemType();
}


void CDlgAdminItemTypeNew::Get(CInfoItemTypeBase *&pDst)
{
	PCInfoItemTypeBase pDstTmp;

	pDstTmp = (PCInfoItemTypeBase)m_pLibInfoItemType->GetNew(m_dwSelectItemTypeID);
	if (m_pInfoItemType) {
		pDstTmp->Copy(m_pInfoItemType);
	}
	pDstTmp->m_wGrpIDSub	= m_nGrpIDSub;
	pDstTmp->m_byDelAverage	= (BYTE)m_nDelAverage;
	pDstTmp->m_dwGrpID	= m_nGrpID;
	pDstTmp->m_dwIconGrpID	= m_nIconGrpID;
	pDstTmp->m_wGrpIDMain	= (WORD)m_dwSelectGrpIDMain;
	pDstTmp->m_dwItemTypeID	= m_dwSelectItemTypeID;
	pDstTmp->m_dwDropSoundID	= m_dwSelectSoundID;
	pDstTmp->m_dwUseEffectID	= m_dwUseEffectID;
	pDstTmp->m_dwUseSoundID	= m_dwUseSoundID;
	pDstTmp->m_strName	= m_strName;
	pDstTmp->m_dwGrpID	= m_nGrpID;
	pDstTmp->m_bPutOn	= m_bPutOn;
	SAFE_DELETE(pDst);
	pDst = pDstTmp;
}


void CDlgAdminItemTypeNew::SetModify(CInfoItemTypeBase *pInfoItemType)
{
	m_nGrpIDSub	= pInfoItemType->m_wGrpIDSub;
	m_nIconGrpID	= pInfoItemType->m_dwIconGrpID;
	m_dwSelectGrpIDMain	= pInfoItemType->m_wGrpIDMain;
	m_dwSelectItemTypeID	= pInfoItemType->m_dwItemTypeID;
	m_dwSelectSoundID	= pInfoItemType->m_dwDropSoundID;
	m_dwUseEffectID	= pInfoItemType->m_dwUseEffectID;
	m_dwUseSoundID	= pInfoItemType->m_dwUseSoundID;
	m_strName	= (LPCTSTR)pInfoItemType->m_strName;
	m_nDelAverage	= pInfoItemType->m_byDelAverage;
	m_nGrpID	= pInfoItemType->m_dwGrpID;
	m_bPutOn	= pInfoItemType->m_bPutOn;

	SAFE_DELETE(m_pInfoItemType);
	m_pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetNew(pInfoItemType->m_dwItemTypeID);
	m_pInfoItemType->Copy(pInfoItemType);

	m_bModeModify = TRUE;
}


BOOL CDlgAdminItemTypeNew::OnInitDialog()
{
	int i, nNo, nNoUse, nNoEffect, nCount;
	DWORD dwSoundID;
	LPCSTR pszTmp;
	PCInfoEffect pInfoEffect;
	CLibSboSoundLoader LibSboSoundLoader;
	PCLibInfoEffect pLibInfoEffect;

	CDlgAdminBase::OnInitDialog();

	if (m_ppWndNotify) {
		*m_ppWndNotify = this;
	}
	m_pMgrGrpData = m_pMgrData->GetMgrGrpData();
	m_pMgrGrpData->ReadItemTmp();

	if (m_bModeModify) {
		SetWindowText(_T("アイテム種別情報の編集"));
	}

	// 種別
	for (i = ITEMTYPEID_NONE; i < ITEMTYPEID_MAX; i ++) {
		pszTmp = m_pLibInfoItemType->GetTypeName((DWORD)i);
		m_ctlType.InsertString(i, Utf8ToTString(pszTmp));
		m_ctlType.SetItemData(i, (DWORD)i);
	}

	m_ctlDropSoundID.InsertString(0, _T("無し"));
	m_ctlUseSoundID.InsertString(0, _T("無し"));
	nNo = nNoUse = 0;

	// 効果音ID
	nCount = LibSboSoundLoader.GetSoundCount();
	for (i = 0; i < nCount; i ++) {
		dwSoundID	= LibSboSoundLoader.GetSoundID(i);
		pszTmp	= LibSboSoundLoader.GetSoundName(dwSoundID);
		m_ctlDropSoundID.InsertString(i + 1, Utf8ToTString(pszTmp));
		m_ctlDropSoundID.SetItemData(i + 1, dwSoundID);
		if (dwSoundID == m_dwSelectSoundID) {
			nNo = i + 1;
		}
		m_ctlUseSoundID.InsertString(i + 1, Utf8ToTString(pszTmp));
		m_ctlUseSoundID.SetItemData(i + 1, dwSoundID);
		if (dwSoundID == m_dwUseSoundID) {
			nNoUse = i + 1;
		}
	}

	// エフェクトID
	nNoEffect = 0;
	m_ctlUseEffectID.InsertString(0, _T("無し"));
	pLibInfoEffect = m_pMgrData->GetLibInfoEffect();
	nCount = pLibInfoEffect->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoEffect = (PCInfoEffect)pLibInfoEffect->GetPtr(i);
		m_ctlUseEffectID.InsertString(i + 1, Utf8ToTString((LPCSTR)pInfoEffect->m_strName));
		m_ctlUseEffectID.SetItemData(i + 1, pInfoEffect->m_dwEffectID);
		if (pInfoEffect->m_dwEffectID == m_dwUseEffectID) {
			nNoEffect = i + 1;
		}
	}

	// 装備メイン
	SetGrpIDMain(m_dwSelectItemTypeID);

	m_ctlType.	SetCurSel(m_dwSelectItemTypeID);
	m_ctlDropSoundID.	SetCurSel(nNo);
	m_ctlUseSoundID.	SetCurSel(nNoUse);
	m_ctlTypeGrpIDMain.	SetCurSel(m_dwSelectGrpIDMain);
	m_ctlUseEffectID.	SetCurSel(nNoEffect);

	return TRUE;
}


void CDlgAdminItemTypeNew::PostNcDestroy()
{
	if (m_pMgrGrpData) {
		m_pMgrGrpData->DeleteItemTmp();
	}

	CDialog::PostNcDestroy();
}


void CDlgAdminItemTypeNew::OnSelectItemgrp()
{
	m_pWndSelectItemGrp = new CWndSelectItemGrp;
	m_pWndSelectItemGrp->Create(this, m_pMgrData);
	m_pWndSelectItemGrp->ShowWindow(SW_SHOW);
}


LRESULT CDlgAdminItemTypeNew::OnWndClose(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case WINDOWID_SELECTMAPPARTSGRP: // アイテム画像選択
		if (lParam < 0) {
			break;
		}
		UpdateData();
		m_nGrpID = lParam;
		UpdateData(FALSE);
		break;
	}

	return -1;
}


void CDlgAdminItemTypeNew::OnSelchangeType()
{
	int nNo;
	DWORD dwTmp;
	CInfoItemTypeBase *pInfoItemType;

	dwTmp	= 0;
	nNo	= m_ctlType.GetCurSel();

	if (nNo >= 0) {
		dwTmp = m_ctlType.GetItemData(nNo);
	}

	m_dwSelectItemTypeID = dwTmp;
	SetGrpIDMain(m_dwSelectItemTypeID);

	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetNew(m_dwSelectItemTypeID);
	pInfoItemType->Copy(m_pInfoItemType);
	SAFE_DELETE(m_pInfoItemType);
	m_pInfoItemType = pInfoItemType;
}


void CDlgAdminItemTypeNew::OnPlay()
{
	int nNo;
	DWORD dwSoundID;
	PCMgrSound pMgrSound;

	nNo = m_ctlDropSoundID.GetCurSel();
	if (nNo < 0) {
		goto Exit;
	}

	pMgrSound = m_pMgrData->GetMgrSound();
	dwSoundID = m_ctlDropSoundID.GetItemData(nNo);
	if (dwSoundID == 0) {
		goto Exit;
	}
	pMgrSound->PlaySound(dwSoundID);

Exit:
	m_ctlDropSoundID.SetFocus();
}


void CDlgAdminItemTypeNew::OnTypeinfo()
{
	int nResult;

	if (m_pInfoItemType == NULL) {
		return;
	}

	switch (m_pInfoItemType->m_dwItemTypeID) {
	case ITEMTYPEID_ARMS: // 持ち物
		{
			CDlgAdminItemTypeNewARMS Dlg(this);

			Dlg.Init(m_pMgrData);
			Dlg.Set(m_pInfoItemType);
			nResult = Dlg.DoModal();
			if (nResult != IDOK) {
				break;
			}
			Dlg.Get(m_pInfoItemType);
		}
		break;
	case ITEMTYPEID_HP: // HP増減
		{
			CDlgAdminItemTypeNewHP Dlg(this);

			Dlg.Init(m_pMgrData);
			Dlg.Set(m_pInfoItemType);
			nResult = Dlg.DoModal();
			if (nResult != IDOK) {
				break;
			}
			Dlg.Get(m_pInfoItemType);
		}
		break;
	case ITEMTYPEID_LIGHT: // 灯り
		{
			CDlgAdminItemTypeNewLIGHT Dlg(this);

			Dlg.Init(m_pMgrData);
			Dlg.Set(m_pInfoItemType);
			nResult = Dlg.DoModal();
			if (nResult != IDOK) {
				break;
			}
			Dlg.Get(m_pInfoItemType);
		}
		break;
	}
}


void CDlgAdminItemTypeNew::OnOK()
{
	int nNo;

	UpdateData();

	nNo = m_ctlType.GetCurSel();
	m_dwSelectItemTypeID	= m_ctlType.GetItemData(nNo);
	nNo = m_ctlDropSoundID.GetCurSel();
	m_dwSelectSoundID	= m_ctlDropSoundID.GetItemData(nNo);
	nNo = m_ctlUseSoundID.GetCurSel();
	m_dwUseSoundID	= m_ctlUseSoundID.GetItemData(nNo);
	nNo = m_ctlTypeGrpIDMain.GetCurSel();
	m_dwSelectGrpIDMain	= m_ctlTypeGrpIDMain.GetItemData(nNo);
	nNo = m_ctlUseEffectID.GetCurSel();
	m_dwUseEffectID	= m_ctlUseEffectID.GetItemData(nNo);

	CDlgAdminBase::EndDialog(IDOK);
}


void CDlgAdminItemTypeNew::SetGrpIDMain(DWORD dwItemTypeID)
{
	int i, nCount;
	LPCSTR pszTmp;

	m_ctlTypeGrpIDMain.ResetContent();

	nCount = m_pLibInfoItemType->GetGrpIDMainCount(dwItemTypeID);
	// 装備メイン
	for (i = 0; i < nCount; i ++) {
		pszTmp = m_pLibInfoItemType->GetGrpIDMainName(dwItemTypeID, (WORD)i);
		m_ctlTypeGrpIDMain.InsertString(i, Utf8ToTString(pszTmp));
		m_ctlTypeGrpIDMain.SetItemData(i, (DWORD)i);
	}
	m_ctlTypeGrpIDMain.SetCurSel(0);
}