/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MgrLayer.h													 */
/* 内容			:レイヤーマネージャクラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "LayerLogo.h"
#include "LayerTitle.h"
#include "LayerInfo.h"
#include "LayerLoginMenu.h"
#include "LayerCharSelect.h"
#include "LayerMap.h"
#include "LayerCloud.h"
#include "LayerSystemMsg.h"
#include "MgrLayer.h"


/* ========================================================================= */
/* 関数名	:CMgrLayer::CMgrLayer											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

CMgrLayer::CMgrLayer()
{
	m_pMgrData		= NULL;
	m_pMgrGrpData	= NULL;

	m_paLayer		= new ARRAYLAYERBASE;
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::~CMgrLayer											 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

CMgrLayer::~CMgrLayer()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::Create												 */
/* 内容		:作成															 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CMgrLayer::Create(
	CMgrData	*pMgrData)		/* [in] データ管理 */
{
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData ();
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::Destroy												 */
/* 内容		:破棄															 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CMgrLayer::Destroy(void)
{
	DeleteAll ();
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::Draw												 */
/* 内容		:描画															 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CMgrLayer::Draw(PCImg32 pDst)
{
	int i, nCount;
	PCLayerBase pLayer;

	nCount = m_paLayer->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pLayer = m_paLayer->GetAt (i);
		pLayer->Draw (pDst);
	}
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::TimerProc											 */
/* 内容		:時間処理														 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

BOOL CMgrLayer::TimerProc(void)
{
	BOOL bRet;
	int i, nCount;
	PCLayerBase pLayer;

	bRet	= FALSE;
	nCount	= m_paLayer->GetSize ();

	for (i = 0; i < nCount; i ++) {
		pLayer = m_paLayer->GetAt (i);
		bRet |= pLayer->TimerProc ();
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::Delete												 */
/* 内容		:指定レイヤーを破棄												 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CMgrLayer::Delete(int nID)
{
	int i, nCount;
	PCLayerBase pLayer;

	nCount = m_paLayer->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pLayer = m_paLayer->GetAt (i);
		if (pLayer->GetID () == nID) {
			SAFE_DELETE (pLayer);
			m_paLayer->RemoveAt (i);
			break;
		}
	}
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::DeleteAll											 */
/* 内容		:全てのレイヤーを破棄											 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CMgrLayer::DeleteAll(void)
{
	int i, nCount;
	PCLayerBase pTmp;

	if (m_paLayer) {
		nCount = m_paLayer->GetSize ();
		for (i = 0; i < nCount; i ++) {
			pTmp = m_paLayer->GetAt (i);
			SAFE_DELETE (pTmp);
		}
		m_paLayer->RemoveAll ();
	}
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::Get													 */
/* 内容		:指定IDのレイヤーを取得											 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

CLayerBase *CMgrLayer::Get(int nID)
{
	int i, nCount;
	PCLayerBase pRet, pLayer;

	pRet	= NULL;
	nCount	= m_paLayer->GetSize ();

	for (i = 0; i < nCount; i ++) {
		pLayer = m_paLayer->GetAt (i);
		if (pLayer->GetID () == nID) {
			pRet = pLayer;
			break;
		}
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::MakeLOGO											 */
/* 内容		:レイヤー作成(ロゴ)												 */
/* 日付		:2005/05/18														 */
/* ========================================================================= */

void CMgrLayer::MakeLOGO(void)
{
	PCLayerLogo pNew;

	pNew = new CLayerLogo;
	pNew->Create (m_pMgrData);
	m_paLayer->Add (pNew);
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::MakeTITLE											 */
/* 内容		:レイヤー作成(タイトル)											 */
/* 日付		:2007/03/26														 */
/* ========================================================================= */

void CMgrLayer::MakeTITLE(void)
{
	PCLayerTitle pNew;

	pNew = new CLayerTitle;
	pNew->Create (m_pMgrData);
	m_paLayer->Add (pNew);
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::MakeINFO											 */
/* 内容		:レイヤー作成(お知らせ)											 */
/* 日付		:2005/08/16														 */
/* ========================================================================= */

void CMgrLayer::MakeINFO(void)
{
	PCLayerInfo pNew;

	pNew = new CLayerInfo;
	pNew->Create (m_pMgrData);
	m_paLayer->Add (pNew);
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::MakeLOGINMENU										 */
/* 内容		:レイヤー作成(ログインメニュー)									 */
/* 日付		:2007/04/15														 */
/* ========================================================================= */

void CMgrLayer::MakeLOGINMENU(void)
{
	PCLayerLoginMenu pNew;

	pNew = new CLayerLoginMenu;
	pNew->Create (m_pMgrData);
	m_paLayer->Add (pNew);
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::MakeCHARSELECT										 */
/* 内容		:レイヤー作成(キャラ選択)										 */
/* 日付		:2005/07/04														 */
/* ========================================================================= */

void CMgrLayer::MakeCHARSELECT(DWORD dwAccountID)
{
	PCLayerCharSelect pNew;

	pNew = new CLayerCharSelect;
	pNew->Create (m_pMgrData, dwAccountID);
	m_paLayer->Add (pNew);
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::MakeMAP												 */
/* 内容		:レイヤー作成(マップ)											 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CMgrLayer::MakeMAP(void)
{
	PCLayerMap pNew;

	pNew = new CLayerMap;
	pNew->Create (m_pMgrData);
	m_paLayer->Add (pNew);
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::MakeCLOUD											 */
/* 内容		:レイヤー作成(雲)												 */
/* 日付		:2007/04/19														 */
/* ========================================================================= */

void CMgrLayer::MakeCLOUD(void)
{
	PCLayerCloud pNew;

	pNew = new CLayerCloud;
	pNew->Create (m_pMgrData);
	m_paLayer->Add (pNew);
}


/* ========================================================================= */
/* 関数名	:CMgrLayer::MakeSYSTEMMSG										 */
/* 内容		:レイヤー作成(システムメッセージ)								 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

void CMgrLayer::MakeSYSTEMMSG(void)
{
	PCLayerSystemMsg pNew;

	pNew = new CLayerSystemMsg;
	pNew->Create (m_pMgrData);
	m_paLayer->Add (pNew);
}

/* Copyright(C)URARA-works 2006 */
