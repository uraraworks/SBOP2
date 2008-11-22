/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:LayerBase.h												 */
/* ���e			:���C���[�`����N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapBase.h"
#include "LibInfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "LayerBase.h"


/* ========================================================================= */
/* �֐���	:CLayerBase::CLayerBase											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

CLayerBase::CLayerBase()
{
	m_nID			= 0;
	m_pMgrData		= NULL;
	m_pMgrGrpData	= NULL;
	m_pMgrDraw		= NULL;
	m_pLibInfoChar	= NULL;
	m_pLibInfoMap	= NULL;
	m_pDibSystem	= NULL;

	m_pDib			= new CImg32;
	m_pDibBase		= new CImg32;

	m_hFont = CreateFont (12, 0, 0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "�l�r �S�V�b�N");
}


/* ========================================================================= */
/* �֐���	:CLayerBase::~CLayerBase										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

CLayerBase::~CLayerBase()
{
	if (m_hFont) {
		DeleteObject (m_hFont);
		m_hFont = NULL;
	}
	SAFE_DELETE (m_pDib);
	SAFE_DELETE (m_pDibBase);
}


/* ========================================================================= */
/* �֐���	:CLayerBase::Create												 */
/* ���e		:�쐬															 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CLayerBase::Create(
	CMgrData	*pMgrData)		/* [in] �f�[�^�Ǘ� */
{
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData ();
	m_pMgrDraw		= m_pMgrData->GetMgrDraw ();
	m_pDibSystem	= m_pMgrGrpData->GetDibSystem ();

	m_pLibInfoChar	= m_pMgrData->GetLibInfoChar ();
	m_pLibInfoMap	= m_pMgrData->GetLibInfoMap ();

	m_pDibBase->Create (16 * DRAW_PARTS_X, 16 * DRAW_PARTS_Y);
	m_pDibBase->SetColorKey (RGB (255, 0, 255));
}


/* ========================================================================= */
/* �֐���	:CLayerBase::Destroy											 */
/* ���e		:�j��															 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CLayerBase::Destroy(void)
{
}


/* ========================================================================= */
/* �֐���	:CLayerBase::Draw												 */
/* ���e		:�`��															 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CLayerBase::Draw(PCImg32 pDst)
{
}


/* ========================================================================= */
/* �֐���	:CLayerBase::TimerProc											 */
/* ���e		:���ԏ���														 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

BOOL CLayerBase::TimerProc(void)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:TextOut1														 */
/* ���e		:�����`��														 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CLayerBase::TextOut1(HDC hDC, int x, int y, LPCSTR pStr, COLORREF color)
{
	int nLen;

	nLen = strlen (pStr);
	SetTextColor (hDC, color);
	TextOut (hDC, x, y, pStr, nLen);
}


/* ========================================================================= */
/* �֐���	:TextOut2														 */
/* ���e		:����蕶���`��													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CLayerBase::TextOut2(HDC hDC, int x, int y, LPCSTR pStr, COLORREF color, COLORREF colorFrame)
{
	int nLen;

	nLen = strlen (pStr);
	SetTextColor (hDC, colorFrame);
	TextOut (hDC, x - 1, y, pStr, nLen);
	TextOut (hDC, x + 1, y, pStr, nLen);
	TextOut (hDC, x, y - 1, pStr, nLen);
	TextOut (hDC, x, y + 1, pStr, nLen);
	SetTextColor (hDC, color);
	TextOut (hDC, x, y, pStr, nLen);
}


/* ========================================================================= */
/* �֐���	:TextOut3														 */
/* ���e		:����蕶���`��													 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

void CLayerBase::TextOut3(HDC hDC, int x, int y, LPCSTR pStr, COLORREF color, COLORREF colorFrame)
{
	int nLen;

	nLen = strlen (pStr);
	SetTextColor (hDC, colorFrame);
	TextOut (hDC, x - 2, y, pStr, nLen);
	TextOut (hDC, x - 1, y, pStr, nLen);
	TextOut (hDC, x - 1, y - 2, pStr, nLen);
	TextOut (hDC, x - 2, y - 1, pStr, nLen);
	TextOut (hDC, x - 1, y - 1, pStr, nLen);
	TextOut (hDC, x - 2, y + 1, pStr, nLen);
	TextOut (hDC, x - 1, y + 1, pStr, nLen);
	TextOut (hDC, x - 1, y + 2, pStr, nLen);

	TextOut (hDC, x + 2, y, pStr, nLen);
	TextOut (hDC, x + 1, y, pStr, nLen);
	TextOut (hDC, x + 1, y - 2, pStr, nLen);
	TextOut (hDC, x + 2, y - 1, pStr, nLen);
	TextOut (hDC, x + 1, y - 1, pStr, nLen);
	TextOut (hDC, x + 2, y + 1, pStr, nLen);
	TextOut (hDC, x + 1, y + 1, pStr, nLen);
	TextOut (hDC, x + 1, y + 2, pStr, nLen);

	TextOut (hDC, x, y - 2, pStr, nLen);
	TextOut (hDC, x, y - 1, pStr, nLen);
	TextOut (hDC, x, y + 2, pStr, nLen);
	TextOut (hDC, x, y + 1, pStr, nLen);

	SetTextColor (hDC, color);
	TextOut (hDC, x, y, pStr, nLen);
}

/* Copyright(C)URARA-works 2005 */
