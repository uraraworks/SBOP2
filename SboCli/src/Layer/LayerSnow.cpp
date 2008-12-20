/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LayerSnow.cpp												 */
/* ���e			:���C���[�`��N���X(��) �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/14													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrLayer.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "InfoMapBase.h"
#include "LayerMap.h"
#include "LayerSnow.h"


/* ========================================================================= */
/* �֐���	:CLayerSnow::CLayerSnow											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/14														 */
/* ========================================================================= */

CLayerSnow::CLayerSnow()
{
	m_nID = WEATHERTYPE_SNOW;

	m_dwLastProc	= 0;
	m_pLayerMap		= NULL;
}


/* ========================================================================= */
/* �֐���	:CLayerSnow::~CLayerSnow										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/14														 */
/* ========================================================================= */

CLayerSnow::~CLayerSnow()
{
	DeleteSnowInfoAll ();
}


/* ========================================================================= */
/* �֐���	:CLayerSnow::Create												 */
/* ���e		:�쐬															 */
/* ���t		:2008/12/14														 */
/* ========================================================================= */

void CLayerSnow::Create(
	CMgrData	*pMgrData)		/* [in] �f�[�^�Ǘ� */
{
	PCMgrLayer pMgrLayer;

	CLayerBase::Create (pMgrData);

	pMgrLayer	= pMgrData->GetMgrLayer ();
	m_pLayerMap = (PCLayerMap)pMgrLayer->Get (LAYERTYPE_MAP);

	RenewSnowInfo (200);
}


/* ========================================================================= */
/* �֐���	:CLayerSnow::Draw												 */
/* ���e		:�`��															 */
/* ���t		:2008/12/14														 */
/* ========================================================================= */

void CLayerSnow::Draw(PCImg32 pDst)
{
	PSTLAYERSNOW_SNOWINFO pInfo;
	int i, x, y, xx, yy, nMoveX, nMoveY, nPosX, nPosY, nCount;
	int aMoveX[] = {1, 1, 1, -1, -1, -1, 1, 1}, aMoveY[] = {1, -1, 1, 1, 1, -1, -1, 1},
		aPosX[] = {0, 0, 1, -1, -1, -1, 1, 1}, aPosY[] = {1, -1, 0, 0, 1, -1, -1, 1};

	nMoveX	= 0;
	nMoveY	= 0;
	nPosX	= 0;
	nPosY	= 0;

	if (m_pLayerMap) {
		nMoveX	= m_pLayerMap->m_nMoveX;
		nMoveY	= m_pLayerMap->m_nMoveY;
		nPosX	= m_pLayerMap->m_nViewX;
		nPosY	= m_pLayerMap->m_nViewY;

		if (nMoveX || nMoveY) {
			/* �ړ����̍��W��␳ */
			nMoveX	*= aMoveX[m_pLayerMap->m_byDirection];
			nMoveY	*= aMoveY[m_pLayerMap->m_byDirection];
			nPosX	+= aPosX[m_pLayerMap->m_byDirection];
			nPosY	+= aPosY[m_pLayerMap->m_byDirection];
		}
	}
	x = nPosX % (DRAW_PARTS_X * 2);
	y = nPosY % (DRAW_PARTS_Y * 2);

	nCount = m_aSnowInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aSnowInfo[i];

		xx = (pInfo->x + (SCRSIZEX - x * 16)) % SCRSIZEX;
		yy = (pInfo->y + (SCRSIZEY - y * 16)) % SCRSIZEY;
		pDst->Circle (32 + xx + nMoveX, 32 + yy + nMoveY, pInfo->nSize, RGB (255, 255, 255));
	}
}


/* ========================================================================= */
/* �֐���	:CLayerSnow::TimerProc											 */
/* ���e		:���ԏ���														 */
/* ���t		:2008/12/14														 */
/* ========================================================================= */

BOOL CLayerSnow::TimerProc(void)
{
	BOOL bRet;
	int i, nCount;
	DWORD dwTime;
	PSTLAYERSNOW_SNOWINFO pInfo;

	bRet	= FALSE;
	dwTime	= timeGetTime ();

	if (dwTime - m_dwLastProc < 25) {
		goto Exit;
	}

	nCount = m_aSnowInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aSnowInfo[i];
		if (pInfo->dwStartWait != 0) {
			if (dwTime - pInfo->dwLastProc < pInfo->dwStartWait) {
				continue;
			}
			pInfo->dwStartWait = 0;
		}
		if (dwTime - pInfo->dwLastProc < pInfo->dwWait) {
			continue;
		}
		pInfo->dwLastProc = dwTime;
		pInfo->y ++;
		if (pInfo->y >= pInfo->nEndY) {
			pInfo->y = pInfo->nStartY;
		}
		bRet = TRUE;
	}
	m_dwLastProc = dwTime;

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLayerSnow::RenewSnowInfo										 */
/* ���e		:������X�V													 */
/* ���t		:2008/12/14														 */
/* ========================================================================= */
void CLayerSnow::RenewSnowInfo(int nCount)
{
	int i, nTmp;
	PSTLAYERSNOW_SNOWINFO pInfo;

	DeleteSnowInfoAll ();

	for (i = 0; i < nCount; i ++) {
		pInfo = new STLAYERSNOW_SNOWINFO;

		nTmp = (genrand () % 3);
		pInfo->nSize		= 3 - nTmp;
		pInfo->nStartY		= genrand () % SCRSIZEY;
		pInfo->nEndY		= pInfo->nStartY + (SCRSIZEY - (genrand () % (SCRSIZEY / 3 * (nTmp + 1)))) + 32;
		pInfo->x			= (genrand () % SCRSIZEX) + 32;
		pInfo->y			= pInfo->nStartY;
		pInfo->dwStartWait	= 0;
		pInfo->dwWait		= 50 + nTmp * 25;
		pInfo->dwLastProc	= timeGetTime ();

		m_aSnowInfo.Add (pInfo);
	}
}


/* ========================================================================= */
/* �֐���	:CLayerSnow::DeleteSnowInfoAll									 */
/* ���e		:�����S�č폜												 */
/* ���t		:2008/12/14														 */
/* ========================================================================= */
void CLayerSnow::DeleteSnowInfoAll(void)
{
	int i, nCount;
	PSTLAYERSNOW_SNOWINFO pInfo;

	nCount = m_aSnowInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aSnowInfo[i];
		SAFE_DELETE (pInfo);
	}
	m_aSnowInfo.RemoveAll ();
}

/* Copyright(C)URARA-works 2008 */
