/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrLayer.h													 */
/* ���e			:���C���[�}�l�[�W���N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
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
/* �֐���	:CMgrLayer::CMgrLayer											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

CMgrLayer::CMgrLayer()
{
	m_pMgrData		= NULL;
	m_pMgrGrpData	= NULL;

	m_paLayer		= new ARRAYLAYERBASE;
}


/* ========================================================================= */
/* �֐���	:CMgrLayer::~CMgrLayer											 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

CMgrLayer::~CMgrLayer()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CMgrLayer::Create												 */
/* ���e		:�쐬															 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrLayer::Create(
	CMgrData	*pMgrData)		/* [in] �f�[�^�Ǘ� */
{
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData ();
}


/* ========================================================================= */
/* �֐���	:CMgrLayer::Destroy												 */
/* ���e		:�j��															 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrLayer::Destroy(void)
{
	DeleteAll ();
}


/* ========================================================================= */
/* �֐���	:CMgrLayer::Draw												 */
/* ���e		:�`��															 */
/* ���t		:2006/09/24														 */
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
/* �֐���	:CMgrLayer::TimerProc											 */
/* ���e		:���ԏ���														 */
/* ���t		:2006/09/24														 */
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
/* �֐���	:CMgrLayer::Delete												 */
/* ���e		:�w�背�C���[��j��												 */
/* ���t		:2006/09/24														 */
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
/* �֐���	:CMgrLayer::DeleteAll											 */
/* ���e		:�S�Ẵ��C���[��j��											 */
/* ���t		:2006/09/24														 */
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
/* �֐���	:CMgrLayer::Get													 */
/* ���e		:�w��ID�̃��C���[���擾											 */
/* ���t		:2006/09/24														 */
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
/* �֐���	:CMgrLayer::MakeLOGO											 */
/* ���e		:���C���[�쐬(���S)												 */
/* ���t		:2005/05/18														 */
/* ========================================================================= */

void CMgrLayer::MakeLOGO(void)
{
	PCLayerLogo pNew;

	pNew = new CLayerLogo;
	pNew->Create (m_pMgrData);
	m_paLayer->Add (pNew);
}


/* ========================================================================= */
/* �֐���	:CMgrLayer::MakeTITLE											 */
/* ���e		:���C���[�쐬(�^�C�g��)											 */
/* ���t		:2007/03/26														 */
/* ========================================================================= */

void CMgrLayer::MakeTITLE(void)
{
	PCLayerTitle pNew;

	pNew = new CLayerTitle;
	pNew->Create (m_pMgrData);
	m_paLayer->Add (pNew);
}


/* ========================================================================= */
/* �֐���	:CMgrLayer::MakeINFO											 */
/* ���e		:���C���[�쐬(���m�点)											 */
/* ���t		:2005/08/16														 */
/* ========================================================================= */

void CMgrLayer::MakeINFO(void)
{
	PCLayerInfo pNew;

	pNew = new CLayerInfo;
	pNew->Create (m_pMgrData);
	m_paLayer->Add (pNew);
}


/* ========================================================================= */
/* �֐���	:CMgrLayer::MakeLOGINMENU										 */
/* ���e		:���C���[�쐬(���O�C�����j���[)									 */
/* ���t		:2007/04/15														 */
/* ========================================================================= */

void CMgrLayer::MakeLOGINMENU(void)
{
	PCLayerLoginMenu pNew;

	pNew = new CLayerLoginMenu;
	pNew->Create (m_pMgrData);
	m_paLayer->Add (pNew);
}


/* ========================================================================= */
/* �֐���	:CMgrLayer::MakeCHARSELECT										 */
/* ���e		:���C���[�쐬(�L�����I��)										 */
/* ���t		:2005/07/04														 */
/* ========================================================================= */

void CMgrLayer::MakeCHARSELECT(DWORD dwAccountID)
{
	PCLayerCharSelect pNew;

	pNew = new CLayerCharSelect;
	pNew->Create (m_pMgrData, dwAccountID);
	m_paLayer->Add (pNew);
}


/* ========================================================================= */
/* �֐���	:CMgrLayer::MakeMAP												 */
/* ���e		:���C���[�쐬(�}�b�v)											 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrLayer::MakeMAP(void)
{
	PCLayerMap pNew;

	pNew = new CLayerMap;
	pNew->Create (m_pMgrData);
	m_paLayer->Add (pNew);
}


/* ========================================================================= */
/* �֐���	:CMgrLayer::MakeCLOUD											 */
/* ���e		:���C���[�쐬(�_)												 */
/* ���t		:2007/04/19														 */
/* ========================================================================= */

void CMgrLayer::MakeCLOUD(void)
{
	PCLayerCloud pNew;

	pNew = new CLayerCloud;
	pNew->Create (m_pMgrData);
	m_paLayer->Add (pNew);
}


/* ========================================================================= */
/* �֐���	:CMgrLayer::MakeSYSTEMMSG										 */
/* ���e		:���C���[�쐬(�V�X�e�����b�Z�[�W)								 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

void CMgrLayer::MakeSYSTEMMSG(void)
{
	PCLayerSystemMsg pNew;

	pNew = new CLayerSystemMsg;
	pNew->Create (m_pMgrData);
	m_paLayer->Add (pNew);
}

/* Copyright(C)URARA-works 2006 */
