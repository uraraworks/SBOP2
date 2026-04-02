/// @file LayerLogo.cpp
/// @brief レイヤー描画クラス(ロゴ) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/05/17
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>
#endif
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "LayerLogo.h"

#if defined(__EMSCRIPTEN__)
EM_JS(void, SBOP2_DebugMarkLogoDraw, (), {
	Module.sbop2LogoDrawCount = (Module.sbop2LogoDrawCount || 0) + 1;
});
EM_JS(void, SBOP2_DebugMarkLogoPixels, (const unsigned char *pBits, int width, int height), {
	if (!pBits || width <= 0 || height <= 0) {
		return;
	}
	const base = pBits >>> 0;
	const stride = width * 4;
	function sample(x, y) {
		const iy = (height - 1 - y);
		const idx = base + iy * stride + x * 4;
		return HEAPU8[idx + 0] + '/' + HEAPU8[idx + 1] + '/' + HEAPU8[idx + 2] + '/' + HEAPU8[idx + 3];
	}
	Module.sbop2LogoTopLeft = sample(32, 32);
	Module.sbop2LogoCenter = sample(width / 2, height / 2);
});
#endif


CLayerLogo::CLayerLogo()
{
	m_nID = LAYERTYPE_LOGO;
	m_pDibLogo = NULL;
}


CLayerLogo::~CLayerLogo()
{
	SAFE_DELETE(m_pDibLogo);
}


void CLayerLogo::Create(
	CMgrData *pMgrData) // [in] データ管理
{
	CLayerBase::Create(pMgrData);

	m_pDibLogo = m_pMgrGrpData->GetDibTmpLogo();
}


void CLayerLogo::Draw(PCImg32 pDst)
{
	int cx, cy;

#if defined(__EMSCRIPTEN__)
	SBOP2_DebugMarkLogoDraw();
#endif

	cx = m_pDibBase->Width();
	cy = m_pDibBase->Height();
	m_pDibBase->FillRect(0, 0, cx, cy, RGB(255, 255, 255));
	m_pDibBase->BltFrom256(cx / 2 - 104, cy / 2 - 40, 208, 80, m_pDibLogo, 0, 0, TRUE);

	pDst->BltStretchNearest(
		32,
		32,
		pDst->Width() - 64,
		pDst->Height() - 64,
		m_pDibBase,
		0,
		0,
		m_pDibBase->Width(),
		m_pDibBase->Height());

#if defined(__EMSCRIPTEN__)
	SBOP2_DebugMarkLogoPixels(pDst->GetBits(), pDst->Width(), pDst->Height());
#endif
}
