/// @file MgrLayer.cpp
/// @brief レイヤーマネージャクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

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


CMgrLayer::CMgrLayer()
{
	m_pMgrData		= NULL;
	m_pMgrGrpData	= NULL;

	m_paLayer		= new ARRAYLAYERBASE;
}


CMgrLayer::~CMgrLayer()
{
	Destroy();
}


void CMgrLayer::Create(
	CMgrData *pMgrData)		// [in] データ管理
{
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData();
}


void CMgrLayer::Destroy(void)
{
	DeleteAll();
}


void CMgrLayer::Draw(PCImg32 pDst)
{
	int i, nCount;
	PCLayerBase pLayer;

	nCount = m_paLayer->size();
	for (i = 0; i < nCount; i++) {
		pLayer = m_paLayer->at(i);
		pLayer->Draw(pDst);
	}
}


BOOL CMgrLayer::TimerProc(void)
{
	BOOL bRet;
	int i, nCount;
	PCLayerBase pLayer;

	bRet	= FALSE;
	nCount	= m_paLayer->size();

	for (i = 0; i < nCount; i++) {
		pLayer = m_paLayer->at(i);
		bRet |= pLayer->TimerProc();
	}

	return bRet;
}


void CMgrLayer::Delete(int nID)
{
	int i, nCount;
	PCLayerBase pLayer;

	nCount = m_paLayer->size();
	for (i = 0; i < nCount; i++) {
		pLayer = m_paLayer->at(i);
		if (pLayer->GetID() == nID) {
			SAFE_DELETE(pLayer);
			m_paLayer->erase(m_paLayer->begin() + i);
			break;
		}
	}
}


void CMgrLayer::DeleteAll(void)
{
	int i, nCount;
	PCLayerBase pTmp;

	if (m_paLayer) {
		nCount = m_paLayer->size();
		for (i = 0; i < nCount; i++) {
			pTmp = m_paLayer->at(i);
			SAFE_DELETE(pTmp);
		}
		m_paLayer->clear();
	}
}


CLayerBase *CMgrLayer::Get(int nID)
{
	int i, nCount;
	PCLayerBase pRet, pLayer;

	pRet	= NULL;
	nCount	= m_paLayer->size();

	for (i = 0; i < nCount; i++) {
		pLayer = m_paLayer->at(i);
		if (pLayer->GetID() == nID) {
			pRet = pLayer;
			break;
		}
	}

	return pRet;
}


void CMgrLayer::MakeLOGO(void)
{
	PCLayerLogo pNew;

	pNew = new CLayerLogo;
	pNew->Create(m_pMgrData);
	m_paLayer->Add(pNew);
}


void CMgrLayer::MakeTITLE(void)
{
	PCLayerTitle pNew;

	pNew = new CLayerTitle;
	pNew->Create(m_pMgrData);
	m_paLayer->Add(pNew);
}


void CMgrLayer::MakeINFO(void)
{
	PCLayerInfo pNew;

	pNew = new CLayerInfo;
	pNew->Create(m_pMgrData);
	m_paLayer->Add(pNew);
}


void CMgrLayer::MakeLOGINMENU(void)
{
	PCLayerLoginMenu pNew;

	pNew = new CLayerLoginMenu;
	pNew->Create(m_pMgrData);
	m_paLayer->Add(pNew);
}


void CMgrLayer::MakeCHARSELECT(DWORD dwAccountID)
{
	PCLayerCharSelect pNew;

	pNew = new CLayerCharSelect;
	pNew->Create(m_pMgrData, dwAccountID);
	m_paLayer->Add(pNew);
}


void CMgrLayer::MakeMAP(void)
{
	PCLayerMap pNew;

	pNew = new CLayerMap;
	pNew->Create(m_pMgrData);
	m_paLayer->Add(pNew);
}


void CMgrLayer::MakeCLOUD(void)
{
	PCLayerCloud pNew;

	pNew = new CLayerCloud;
	pNew->Create(m_pMgrData);
	m_paLayer->Add(pNew);
}


void CMgrLayer::MakeSYSTEMMSG(void)
{
	PCLayerSystemMsg pNew;

	pNew = new CLayerSystemMsg;
	pNew->Create(m_pMgrData);
	m_paLayer->Add(pNew);
}
