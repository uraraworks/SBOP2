/// @file MgrDraw.cpp
/// @brief 描画マネージャクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>
#endif
#include "SboCli_priv.h"	// SCRSIZEX / SCRSIZEY
#include "LibInfoMapParts.h"
#include "LibInfoMapShadow.h"
#include "LibInfoMapObject.h"
#include "LibInfoItem.h"
#include "LibInfoMotionType.h"
#include "InfoMotion.h"
#include "InfoTextEffect.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrLayer.h"
#include "MgrWindow.h"
#include "MainFrame.h"
#include "myString.h"
#include "MgrDraw.h"
#include <vector>

#if defined(__EMSCRIPTEN__)
EM_JS(void, SBOP2_PresentToCanvas, (int pRgba, int width, int height), {
	const canvas = Module['canvas'];
	Module.sbop2PresentCount = (Module.sbop2PresentCount || 0) + 1;
	Module.sbop2PresentSize = width + 'x' + height;
	Module.sbop2Stage = 7;
	if (!canvas) {
		return;
	}
	if (canvas.width !== width) {
		canvas.width = width;
	}
	if (canvas.height !== height) {
		canvas.height = height;
	}
	let ctx = Module['sbop2Ctx2d'];
	if (!ctx) {
		ctx = canvas.getContext('2d', { willReadFrequently: true });
		Module['sbop2Ctx2d'] = ctx;
	}
	if (!ctx) {
		return;
	}
	ctx.imageSmoothingEnabled = false;

	let imageData = Module['sbop2ImageData'];
	if (!imageData || imageData.width !== width || imageData.height !== height) {
		imageData = ctx.createImageData(width, height);
		Module['sbop2ImageData'] = imageData;
	}

	const base = pRgba >>> 0;
	Module.sbop2PresentPtr = String(base);
	const src = HEAPU8.subarray(base, base + width * height * 4);
	const dst = imageData.data;
	const sampleIndex = 0;
	Module.sbop2PresentSample =
		src[sampleIndex + 0] + '/' +
		src[sampleIndex + 1] + '/' +
		src[sampleIndex + 2] + '/' +
		src[sampleIndex + 3];
	dst.set(src);
	ctx.putImageData(imageData, 0, 0);

	const rectQueue = Module.sbop2RectQueue || [];
	if (rectQueue.length > 0) {
		ctx.save();
		ctx.imageSmoothingEnabled = false;
		for (const item of rectQueue) {
			if (item.hasFill) {
				ctx.fillStyle = 'rgba(' + item.fillR + ',' + item.fillG + ',' + item.fillB + ',' + (item.fillA / 255) + ')';
				ctx.fillRect(item.x, item.y, item.w, item.h);
			}
			if (item.hasStroke) {
				ctx.lineWidth = item.lineWidth;
				ctx.strokeStyle = 'rgba(' + item.strokeR + ',' + item.strokeG + ',' + item.strokeB + ',' + (item.strokeA / 255) + ')';
				ctx.strokeRect(item.x + 0.5, item.y + 0.5, Math.max(0, item.w - 1), Math.max(0, item.h - 1));
			}
		}
		ctx.restore();
	}
	Module.sbop2RectQueue = [];

	const textQueue = Module.sbop2TextQueue || [];
	if (textQueue.length > 0) {
		ctx.save();
		ctx.textBaseline = 'top';
		ctx.imageSmoothingEnabled = false;
		for (const item of textQueue) {
			ctx.fontKerning = 'none';
			ctx.textAlign = 'left';
			ctx.direction = 'ltr';
			ctx.textRendering = 'geometricPrecision';
			ctx.font =
				(item.bold ? "bold " : "") +
				String(item.size) +
				"px 'Meiryo UI','Meiryo','Yu Gothic UI','Yu Gothic','MS PGothic','MS Gothic','Noto Sans JP',sans-serif";
			if (item.outline) {
				ctx.fillStyle = 'rgb(' + item.frameR + ',' + item.frameG + ',' + item.frameB + ')';
				ctx.fillText(item.text, item.x - 1, item.y);
				ctx.fillText(item.text, item.x + 1, item.y);
				ctx.fillText(item.text, item.x, item.y - 1);
				ctx.fillText(item.text, item.x, item.y + 1);
			}
			ctx.fillStyle = 'rgb(' + item.r + ',' + item.g + ',' + item.b + ')';
			ctx.fillText(item.text, item.x, item.y);
		}
		ctx.restore();
	}
	Module.sbop2TextQueue = [];
});
EM_JS(void, SBOP2_DebugCanvasRgbaHead, (int r, int g, int b, int a), {
	Module.sbop2CppRgbaHead = r + '/' + g + '/' + b + '/' + a;
});
#endif

static void SBOP2_LogSurfaceSample(const char *pszLabel, CImg32 *pImg, int x, int y)
{
#if defined(__EMSCRIPTEN__)
	if ((pszLabel == NULL) || (pImg == NULL) || (pImg->GetBits() == NULL)) {
		return;
	}

	const int nWidth = pImg->Width();
	const int nHeight = pImg->Height();
	if ((nWidth <= 0) || (nHeight <= 0)) {
		return;
	}

	x = max(0, min(nWidth - 1, x));
	y = max(0, min(nHeight - 1, y));

	const BYTE *pBits = pImg->GetBits();
	const int nIndex = ((nHeight - 1 - y) * nWidth + x) * 4;
	SDL_Log("%s sample=%u/%u/%u/%u", pszLabel,
		(unsigned)pBits[nIndex + 0],
		(unsigned)pBits[nIndex + 1],
		(unsigned)pBits[nIndex + 2],
		(unsigned)pBits[nIndex + 3]);
#else
	(void)pszLabel;
	(void)pImg;
	(void)x;
	(void)y;
#endif
}


CMgrDraw::CMgrDraw()
{
	m_byLevel		= 0;
	m_byFadeState	= FADESTATE_NONE;
	m_dwLastFade	= 0;

	m_pMgrData				= NULL;
	m_pMgrGrpData			= NULL;
	m_pMgrLayer				= NULL;
	m_pMgrWindow			= NULL;
	m_pLibInfoMapParts		= NULL;
	m_pLibInfoMapShadow		= NULL;
	m_pLibInfoItem			= NULL;
	m_pLibInfoMotionType	= NULL;
	m_pBackTexture			= NULL;		// SDLテクスチャ（Lazy初期化）

	m_pDibBack = new CImg32;
	m_pDibBack->Create(SCRSIZEX + 64, SCRSIZEY + 64);
	m_pDibTmp = new CImg32;
	m_pDibTmp->Create((SCRSIZEX + 64) * 2, (SCRSIZEY + 64) * 2);
	m_pDibTmp->SetColorKey(RGB(255, 0, 255));
}


CMgrDraw::~CMgrDraw()
{
	if (m_pBackTexture != NULL) {
		SDL_DestroyTexture(m_pBackTexture);
		m_pBackTexture = NULL;
	}
	SAFE_DELETE(m_pDibBack);
	SAFE_DELETE(m_pDibTmp);
}


void CMgrDraw::Create(
	CMgrData	*pMgrData)		// [in] データ管理
{
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData();
	m_pMgrLayer		= m_pMgrData->GetMgrLayer();
	m_pMgrWindow	= m_pMgrData->GetMgrWindow();

	m_pLibInfoMapParts		= m_pMgrData->GetLibInfoMapParts();
	m_pLibInfoMapShadow		= m_pMgrData->GetLibInfoMapShadow();
	m_pLibInfoItem			= m_pMgrData->GetLibInfoItem();
	m_pLibInfoMotionType	= m_pMgrData->GetLibInfoMotionType();
}


void CMgrDraw::Destroy(void)
{
}


void CMgrDraw::Draw(SDL_Renderer *pRenderer)
{
	m_pMgrLayer->	Draw(m_pDibBack);		// レイヤー
#if defined(__EMSCRIPTEN__)
	static BOOL s_bLoggedLayer = FALSE;
	if (s_bLoggedLayer == FALSE) {
		SBOP2_LogSurfaceSample("MgrDraw layer", m_pDibBack, 32, 32);
		s_bLoggedLayer = TRUE;
	}
#endif
	m_pMgrWindow->	Draw(m_pDibBack);		// ウィンドウ
#if defined(__EMSCRIPTEN__)
	static BOOL s_bLoggedWindow = FALSE;
	if (s_bLoggedWindow == FALSE) {
		SBOP2_LogSurfaceSample("MgrDraw window", m_pDibBack, 32, 32);
		s_bLoggedWindow = TRUE;
	}
#endif

	// 明度指定あり？
	if (m_byLevel < 100 && m_byLevel > 0) {
		m_pDibBack->ChgLevel(32, 32, SCRSIZEX, SCRSIZEY, (m_byLevel == 1) ? 0 : m_byLevel);
	}

#if defined(__EMSCRIPTEN__)
	{
		static std::vector<unsigned char> aCanvasRgba;
		const int nWidth = SCRSIZEX;
		const int nHeight = SCRSIZEY;
		const int nFullWidth = m_pDibBack->Width();
		const int nFullHeight = m_pDibBack->Height();
		const BYTE *pSrc = m_pDibBack->GetBits();

		aCanvasRgba.resize((size_t)nWidth * (size_t)nHeight * 4);
		for (int y = 0; y < nHeight; ++y) {
			const BYTE *pSrcRow = pSrc + ((size_t)(nFullHeight - 1 - (32 + y)) * (size_t)nFullWidth + 32) * 4;
			unsigned char *pDstRow = &aCanvasRgba[(size_t)y * (size_t)nWidth * 4];
			for (int x = 0; x < nWidth; ++x) {
				pDstRow[x * 4 + 0] = pSrcRow[x * 4 + 2];
				pDstRow[x * 4 + 1] = pSrcRow[x * 4 + 1];
				pDstRow[x * 4 + 2] = pSrcRow[x * 4 + 0];
				pDstRow[x * 4 + 3] = 255;
			}
		}

		if (aCanvasRgba.size() >= 4) {
			SBOP2_DebugCanvasRgbaHead(
				(int)aCanvasRgba[0],
				(int)aCanvasRgba[1],
				(int)aCanvasRgba[2],
				(int)aCanvasRgba[3]);
		}
		SBOP2_PresentToCanvas((int)(intptr_t)aCanvasRgba.data(), nWidth, nHeight);
	}
	return;
#endif

	if (pRenderer == NULL) {
		return;
	}

	// バックバッファを SDL_Texture 経由で描画
	// CImg32 は CreateDIBSection (biHeight > 0) のため
	// メモリ上の先頭行が画像の最下行（ボトムアップ DIB）
	// SDL_RenderCopyEx の SDL_FLIP_VERTICAL で垂直フリップして正しい向きに表示
	{
		LPBYTE		pBits;
		int			nW, nStride;
		SDL_Rect	stDst;

		pBits	= m_pDibBack->GetBits();
		nW		= m_pDibBack->Width();
		nStride	= nW * 4;					// 1行あたりのバイト数（32bit BGRA）

		// Lazy 初期化: テクスチャを初回のみ作成
		if (m_pBackTexture == NULL) {
			// SDL_PIXELFORMAT_ARGB8888: CImg32(BI_RGB 32bit) のメモリ配置 [B][G][R][0x00] に対応
			// リトルエンディアンでは ARGB8888 のバイト順が [B][G][R][A] となり一致する
			m_pBackTexture = SDL_CreateTexture(
								pRenderer,
								SDL_PIXELFORMAT_ARGB8888,
								SDL_TEXTUREACCESS_STREAMING,
								SCRSIZEX,
								SCRSIZEY);
			if (m_pBackTexture != NULL) {
				// CImg32 はアルファ値を常に正しく管理していないため、
				// ブラウザ側ではテクスチャを不透明・非ブレンド扱いに固定する。
				SDL_SetTextureBlendMode(m_pBackTexture, SDL_BLENDMODE_NONE);
				SDL_SetTextureAlphaMod(m_pBackTexture, 255);
			}
		}
		if (m_pBackTexture == NULL) {
			return;
		}

		// ボトムアップ DIB の (x=32, y=32) から SCRSIZEX × SCRSIZEY 行を転送
		// ボトムアップ DIBでは y=0 がメモリ先頭（画像の最下行）
		// 下から 32行目（描画開始オフセット）: pBits + 32 * nStride + 32 * 4
		SDL_UpdateTexture(m_pBackTexture, NULL,
							pBits + 32 * nStride + 32 * 4,
							nStride);

		// SDL_FLIP_VERTICAL で垂直フリップ → ボトムアップ DIB を正しい向きに
		stDst.x = 0;
		stDst.y = 0;
		stDst.w = SCRSIZEX;
		stDst.h = SCRSIZEY;
		SDL_RenderCopyEx(pRenderer, m_pBackTexture, NULL, &stDst,
							0.0, NULL, SDL_FLIP_VERTICAL);
#if defined(__EMSCRIPTEN__)
		{
			SDL_Rect stDebugRect;

			stDebugRect.x = 0;
			stDebugRect.y = 0;
			stDebugRect.w = 32;
			stDebugRect.h = 32;
			SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_NONE);
			SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
			SDL_RenderFillRect(pRenderer, &stDebugRect);
		}
#endif
	}
}


void CMgrDraw::DrawChar(
	CImg32 *pDst,					// [in] 描画先
	int x,							// [in] 描画位置(X)
	int y,							// [in] 描画位置(Y)
	CInfoCharCli *pInfoChar,		// [in] キャラ情報
	BOOL bLock)/*TRUE*/				// [in] テンポラリをロックする
{
	WORD wGrpIDMain, wGrpIDSub, wGrpIDMainBase, wGrpIdNPC, wGrpID;
	int cx, cy, cxChar, cyChar, nTmp, nDirection, nEyeID, i, nCount, nFadeLevel;
	POINT ptTmp;
	PCInfoMotion pInfoMotion;
	PCImg32 pSrc, pDibTmp;

	LockDibTmp();
	pDibTmp = GetDibTmp();
	pInfoMotion = pInfoChar->GetMotionInfo();
	if (pInfoMotion == NULL) {
		goto Exit;
	}

	wGrpID			= 0;
	wGrpIDMainBase	= pInfoMotion->m_wGrpIDMainBase;
	wGrpIdNPC		= pInfoChar->m_wGrpIDNPC;
	if (wGrpIdNPC != 0) {
		if (wGrpIdNPC >= 50000) {
			wGrpIdNPC -= 50000;
			if (wGrpIDMainBase == GRPIDMAIN_2X2_CHAR) {
				wGrpIDMainBase = GRPIDMAIN_2X2_NPC;
			} else {
				wGrpIDMainBase = GRPIDMAIN_NPC;
				if (pInfoChar->m_bMotionDirection == FALSE) {
					wGrpID = wGrpIdNPC;
				}
			}
		} else {
			wGrpIDMainBase = GRPIDMAIN_2X2_NPC;
		}
	}

	nDirection = pInfoChar->GetDrawDirection(-1);
	cx = cy = m_pMgrGrpData->GetGrpSize(wGrpIDMainBase);
	cxChar = cyChar = cx;

	if (pInfoChar->m_wGrpIDNPC == (WORD)-1) {
		goto ExitEffect;
	}

//Todo;
	nEyeID = 0;
	if (pInfoChar->IsDamage()) {
		nEyeID = 1;
	}
	if (pInfoChar->m_nMoveState == CHARMOVESTATE_SWOON) {
		nEyeID = 2;
	}
	if (pInfoChar->m_nMoveState == CHARMOVESTATE_SLEEPTIMER) {
		nEyeID = 3;
	}

	nFadeLevel = pInfoChar->m_nFadeLevel;
	if (pInfoChar->m_nMoveType == CHARMOVETYPE_PUTNPC) {
		if (m_pMgrData->GetAdminLevel() == ADMINLEVEL_NONE) {
			return;
		}
		if (nFadeLevel == 0) {
			nFadeLevel = 50;
		}
	}

	// キャラ影の描画
	switch (wGrpIDMainBase) {
	case GRPIDMAIN_2X2_CHAR:
		m_pMgrGrpData->GetGrpPos(wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase - 1, ptTmp);
		pSrc = m_pMgrGrpData->GetDib2x2CharShadow(0);
		pDibTmp->BltFrom256(0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y);
		pDibTmp->BltStretchNearest(0, cyChar * 2, cxChar * 2, cyChar * 2, pDibTmp, 0, 0, cxChar, cyChar);
		pDst->BltAlpha(
			x + pInfoMotion->m_ptDrawPosPile0.x,
			y + pInfoMotion->m_ptDrawPosPile0.y,
			cxChar * 2, cyChar * 2, pDibTmp, 0, cyChar * 2, 75, TRUE);
		break;

	case GRPIDMAIN_2X2_NPC:
		m_pMgrGrpData->GetGrpPos(wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase - 1, ptTmp);
		nTmp = 0;
		if (pInfoChar->m_nSex == SEX_FEMALE) {
			nTmp += 32 * 4;
		}
		ptTmp.y += nTmp;
		pSrc = m_pMgrGrpData->GetDib2x2NPCShadow(wGrpIdNPC);
		pDibTmp->BltFrom256(0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y);
		pDibTmp->BltStretchNearest(0, cyChar * 2, cxChar * 2, cyChar * 2, pDibTmp, 0, 0, cxChar, cyChar);
		pDst->BltAlpha(
			x + pInfoMotion->m_ptDrawPosPile0.x,
			y + pInfoMotion->m_ptDrawPosPile0.y,
			cxChar * 2, cyChar * 2, pDibTmp, 0, cyChar * 2, 75, TRUE);
		break;
	}

	nCount = pInfoMotion->m_anDrawList.size();
	for (i = 0; i < nCount; i++) {
		switch (pInfoMotion->m_anDrawList[i]) {
		case 0:
			// 2x2サイズ以外？
			if (wGrpIDMainBase != GRPIDMAIN_2X2_CHAR) {
				nTmp = 0;
				if (wGrpIDMainBase == GRPIDMAIN_2X2_NPC) {
					if (pInfoChar->m_nSex == SEX_FEMALE) {
						nTmp += 32 * 4;
					}
				}
				m_pMgrGrpData->GetGrpPos(wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase - 1, ptTmp, wGrpID);
				ptTmp.y += nTmp;
				if (wGrpIDMainBase == GRPIDMAIN_CHAR || ((wGrpIDMainBase == GRPIDMAIN_NPC) && (pInfoChar->m_bMotionDirection))) {
					pSrc = pInfoChar->m_pDibChar;
					pDibTmp->Blt(0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y);
				} else {
					pSrc = m_pMgrGrpData->GetDib(wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase - 1, wGrpIdNPC);
					pDibTmp->BltFrom256(0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y);
				}

				if (pInfoMotion->m_bRedrawHand) {
					pSrc = m_pMgrGrpData->GetDib(wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase, pInfoChar->m_wFamilyID);
					if (pSrc) {
						pDibTmp->Blt(10, 9,  2, 4, pSrc, ptTmp.x + 10,  9, TRUE);
						pDibTmp->Blt(9,  10, 4, 2, pSrc, ptTmp.x +  9, 10, TRUE);
					}
				}
				pDibTmp->BltStretchNearest(0, cyChar * 2, cxChar * 2, cyChar * 2, pDibTmp, 0, 0, cxChar, cyChar);
				pDst->BltAlpha(
					x + pInfoMotion->m_ptDrawPosPile0.x,
					y + pInfoMotion->m_ptDrawPosPile0.y,
					cxChar * 2, cyChar * 2, pDibTmp, 0, cyChar * 2, nFadeLevel, TRUE);

			// 2x2サイズ
			} else {
				m_pMgrGrpData->GetGrpPos(wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase - 1, ptTmp);
				nTmp = 0;
				if (pInfoChar->m_nSex == SEX_FEMALE) {
					nTmp += 32 * 4;
				}
				pSrc = m_pMgrGrpData->GetDib(wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase - 1, pInfoChar->m_wFamilyID);
				// 体
				pDibTmp->BltFrom256(0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y + nTmp);
				if ((pInfoChar->m_wGrpIDSP == 0) && (pInfoChar->m_wGrpIDTmpMain == 0)) {
					wGrpIDMain = GRPIDMAIN_2X2_CLOTH;
					// 服リソースは IDP_2X2_CLOTH_00 (i=0) 始まりで配列にロードされるため
					// m_wGrpIDCloth=1 → 配列[1]=CLOTH_01 と直接対応する。補正不要。
					wGrpIDSub  = pInfoChar->m_wGrpIDCloth;
				} else {
					wGrpIDMain = GRPIDMAIN_2X2_SPCLOTH;
					// 特殊服リソースは IDP_2X2_SP_CLOTH_01 (i=0) 始まりで配列にロードされるため
					// m_wGrpIDSP=1 → 配列[0]=SP_CLOTH_01 と対応させるには -1 補正が必要
					wGrpIDSub  = (pInfoChar->m_wGrpIDSP > 0) ? pInfoChar->m_wGrpIDSP - 1 : 0;
					if (pInfoChar->m_wGrpIDTmpMain != 0) {
						wGrpIDMain = pInfoChar->m_wGrpIDTmpMain;
						wGrpIDSub  = pInfoChar->m_wGrpIDTmpSub;
					}
				}
				pSrc = m_pMgrGrpData->GetDib(wGrpIDMain, 0, wGrpIDSub);
				// 服
				pDibTmp->BltFrom256(0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y + nTmp, TRUE);
				// m_wGrpIDHairType は 1-based (1〜N) だが GetDib2x2Hair は 0-based 配列インデックスを使うため -1 する
				pSrc = m_pMgrGrpData->GetDib(GRPIDMAIN_2X2_HAIR, 0,
					pInfoChar->m_wGrpIDHairType > 0 ? pInfoChar->m_wGrpIDHairType - 1 : 0);
				// 髪
				pDibTmp->BltFrom256(0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y + nTmp, TRUE);
				if (nDirection != 0) {
					// 目
					pSrc = m_pMgrGrpData->GetDib(GRPIDMAIN_2X2_EYE, 0, nEyeID);
					pDibTmp->BltFrom256(0, 0, cxChar, cyChar, pSrc, ptTmp.x - 32 * 4, ptTmp.y, TRUE);
				}
				pDibTmp->BltStretchNearest(0, cyChar * 2, cxChar * 2, cyChar * 2, pDibTmp, 0, 0, cxChar, cyChar);
				pDst->BltAlpha(
					x + pInfoMotion->m_ptDrawPosPile0.x,
					y + pInfoMotion->m_ptDrawPosPile0.y,
					cxChar * 2, cyChar * 2, pDibTmp, 0, cyChar * 2, nFadeLevel, TRUE);
			}
			break;
		case 1:		// 重ね画像１
			wGrpIDMain = pInfoMotion->m_wGrpIDMainPile1;
			wGrpIDSub  = pInfoMotion->m_wGrpIDSubPile1;
			if (wGrpIDSub == 0) {
				break;
			}
			pInfoChar->GetGrpID(wGrpIDMain, wGrpIDSub);
			pSrc = m_pMgrGrpData->GetDib(wGrpIDMain, wGrpIDSub - 1, 0);
			if (pSrc == NULL) {
				break;
			}
			cx = cy = m_pMgrGrpData->GetGrpSize(wGrpIDMain);
			nTmp = 0;
			if (cx > cxChar) {
				nTmp = (cx - cxChar) * -1;
			}
			m_pMgrGrpData->GetGrpPos(wGrpIDMain, wGrpIDSub - 1, ptTmp);
			pDibTmp->BltFrom256(0, 0, cx, cy, pSrc, ptTmp.x, ptTmp.y);

			pDibTmp->BltStretchNearest(0, cy * 2, cx * 2, cy * 2, pDibTmp, 0, 0, cx, cy);
			pDst->BltAlpha(
				x + nTmp + pInfoMotion->m_ptDrawPosPile1.x,
				y + nTmp + pInfoMotion->m_ptDrawPosPile1.y,
				cx * 2, cy * 2, pDibTmp, 0, cy * 2, pInfoMotion->m_byLevel1, TRUE);
			break;
		case 2:		// 重ね画像２
			wGrpIDMain = pInfoMotion->m_wGrpIDMainPile2;
			wGrpIDSub  = pInfoMotion->m_wGrpIDSubPile2;
			if (wGrpIDSub == 0) {
				break;
			}
			pInfoChar->GetGrpID(wGrpIDMain, wGrpIDSub);
			pSrc = m_pMgrGrpData->GetDib(wGrpIDMain, wGrpIDSub - 1, 0);
			if (pSrc == NULL) {
				break;
			}
			cx = cy = m_pMgrGrpData->GetGrpSize(wGrpIDMain);
			nTmp = 0;
			if (cx > cxChar) {
				nTmp = (cx - cxChar) * -1;
			}
			m_pMgrGrpData->GetGrpPos(wGrpIDMain, wGrpIDSub - 1, ptTmp);
			pDibTmp->BltFrom256(0, 0, cx, cy, pSrc, ptTmp.x, ptTmp.y);

			pDibTmp->BltStretchNearest(0, cy * 2, cx * 2, cy * 2, pDibTmp, 0, 0, cx, cy);
			pDst->BltAlpha(
				x + nTmp + pInfoMotion->m_ptDrawPosPile2.x,
				y + nTmp + pInfoMotion->m_ptDrawPosPile2.y,
				cx * 2, cy * 2, pDibTmp, 0, cy * 2, pInfoMotion->m_byLevel2, TRUE);
			break;
		case 3:		// 重ね画像３
			wGrpIDMain = pInfoMotion->m_wGrpIDMainPile3;
			wGrpIDSub  = pInfoMotion->m_wGrpIDSubPile3;
			if (wGrpIDSub == 0) {
				break;
			}
			pInfoChar->GetGrpID(wGrpIDMain, wGrpIDSub);
			pSrc = m_pMgrGrpData->GetDib(wGrpIDMain, wGrpIDSub - 1, 0);
			if (pSrc == NULL) {
				break;
			}
			cx = cy = m_pMgrGrpData->GetGrpSize(wGrpIDMain);
			nTmp = 0;
			if (cx > cxChar) {
				nTmp = (cx - cxChar) * -1;
			}
			m_pMgrGrpData->GetGrpPos(wGrpIDMain, wGrpIDSub - 1, ptTmp);
			pDibTmp->BltFrom256(0, 0, cx, cy, pSrc, ptTmp.x, ptTmp.y);

			pDibTmp->BltStretchNearest(0, cy * 2, cx * 2, cy * 2, pDibTmp, 0, 0, cx, cy);
			pDst->BltAlpha(
				x + nTmp + pInfoMotion->m_ptDrawPosPile3.x,
				y + nTmp + pInfoMotion->m_ptDrawPosPile3.y,
				cx * 2, cy * 2, pDibTmp, 0, cy * 2, pInfoMotion->m_byLevel3, TRUE);
			break;
		}
	}

ExitEffect:
	// エフェクト
	pSrc = pInfoChar->GetEfcImg();
	if (pSrc) {
		pInfoChar->GetEfcGrpPos(ptTmp);
		cx = cy = pInfoChar->GetEfcGrpSize();
		nTmp = 0;
		if (cx > cxChar) {
			nTmp = (cx - cxChar) * -1;
		}
		pDibTmp->BltFrom256(0, 0, cx, cy, pSrc, ptTmp.x, ptTmp.y);
		pDibTmp->BltStretchNearest(0, cy * 2, cx * 2, cy * 2, pDibTmp, 0, 0, cx, cy);
		pDst->BltAlpha(x + nTmp, y + nTmp, cx * 2, cy * 2, pDibTmp, 0, cy * 2, pInfoChar->GetEfcLevel(), TRUE);
	}

Exit:
	ReleaseDibTmp();
	UnLockDibTmp();
}


void CMgrDraw::DrawChar(
	CImg32 *pDst,					// [in] 描画先
	int x,							// [in] 描画位置(X)
	int y,							// [in] 描画位置(Y)
	BYTE byDirection,				// [in] 向き
	BYTE byAnimeNo,					// [in] アニメーション番号
	BYTE byLevel,					// [in] 透明度
	CInfoCharCli *pInfoChar)		// [in] キャラ情報
{
	BYTE byAnimeNoTmp;
	int yy, cx, cy;
	PCImg32 pSrc, pDibTmp;

	yy		= 0;
	pSrc	= pInfoChar->m_pDibChar;
	if (pSrc == NULL) {
		return;
	}

	pDibTmp = GetDibTmp();
	cx = 16;
	cy = 16;

	byAnimeNoTmp = byAnimeNo / 2;
	if ((byAnimeNo % 2) != 0) {
		byAnimeNoTmp = 2;
	}

	pDibTmp->Blt(0, 0, cx, cy, pSrc, (byDirection * 4 + byAnimeNoTmp) * cx, yy);

	pDibTmp->BltStretchNearest(0, cy * 2, cx * 2, cy * 2, pDibTmp, 0, 0, cx, cy);

	pDst->BltAlpha(x, y, cx * 2, cy * 2, pDibTmp, 0, cy * 2, byLevel, TRUE);
	ReleaseDibTmp();
}


void CMgrDraw::DrawMapParts(
	CImg32 *pDst,				// [in] 描画先
	int x,						// [in] 描画位置(X)
	int y,						// [in] 描画位置(Y)
	DWORD dwPartsID,			// [in] マップパーツID
	int nMode,					// [in] 描画モード(0:下地のみ 1:重ね合わせのみ 2:全て 3:背景ごと重なるなら全て)
	BOOL bSingleSize,			// [in] 等倍指定
	BOOL bUseColorKey,/*TRUE*/	// [in] TRUE:重ね合わせする
	BYTE byLevel)/*0*/			// [in] 透過レベル
{
	PCInfoMapParts pInfoMapParts;

	pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr(dwPartsID);
	DrawMapParts(pDst, x, y, pInfoMapParts, nMode, bSingleSize, bUseColorKey, TRUE, byLevel);
}


void CMgrDraw::DrawMapParts(
	CImg32 *pDst,						// [in] 描画先
	int x,								// [in] 描画位置(X)
	int y,								// [in] 描画位置(Y)
	CInfoMapParts *pInfoMapParts,		// [in] マップパーツ情報
	int nMode,							// [in] 描画モード(0:下地のみ 1:重ね合わせのみ 2:全て 3:背景ごと重なるなら全て)
	BOOL bSingleSize,					// [in] 等倍指定
	BOOL bUseColorKey,/*TRUE*/			// [in] TRUE:重ね合わせする
	BOOL bLock,/*TRUE*/					// [in] テンポラリをロックする
	BYTE byLevel)/*0*/					// [in] 透過レベル
{
	int nSize, nSizeDst;
	WORD wGrpIDBase, wGrpIDPile;
	PCImg32 pImg, pDibTmp;
	PCInfoAnime pAnime;

	nSize = 16;
	nSizeDst = 32;
	if (bSingleSize) {
		nSizeDst = 16;
	}

	pDibTmp = GetDibTmp();
	if (pInfoMapParts == NULL) {
		pDst->FillRect(x, y, nSizeDst, nSizeDst, RGB(0, 0, 0));
		goto Exit;
	}

	wGrpIDBase = pInfoMapParts->m_wGrpIDBase;
	wGrpIDPile = pInfoMapParts->m_wGrpIDPile;
	if (pInfoMapParts->GetAnimeCount() > 0) {
		pAnime = pInfoMapParts->m_paAnimeInfo->at(pInfoMapParts->m_byAnimeNo);
		wGrpIDBase = pAnime->m_wGrpIDBase;
		wGrpIDPile = pAnime->m_wGrpIDPile;
	}
	if (nMode == 0) {
		wGrpIDPile = 0;
	}
	if (nMode == 3) {
		// 背景ごと重なる
		if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_PILEBACK) {
			nMode = 2;
		// 重なる
		} else if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_PILE) {
			nMode = 1;
		} else {
			goto Exit;
		}
	}
	if ((nMode == 1) && (wGrpIDPile == 0)) {
		goto Exit;
	}

	if (nMode != 1) {
		pImg = m_pMgrGrpData->GetDibMapParts(wGrpIDBase / 1024);
		pDibTmp->BltFrom256(0, 0, nSize, nSize, pImg, ((wGrpIDBase % 1024) % 32) * nSize, ((wGrpIDBase % 1024) / 32) * nSize);
	} else {
		pDibTmp->FillRect(0, 0, nSize, nSize, RGB(255, 0, 255));
	}
	if (wGrpIDPile > 0) {
		pImg = m_pMgrGrpData->GetDibMapParts(wGrpIDPile / 1024);
		pDibTmp->BltFrom256(0, 0, nSize+1, nSize, pImg, ((wGrpIDPile % 1024) % 32) * nSize, ((wGrpIDPile % 1024) / 32) * nSize, TRUE);
	}

	pDibTmp->BltStretchNearest(0, nSizeDst, nSizeDst, nSizeDst, pDibTmp, 0, 0, nSize, nSize);

	if (byLevel == 0) {
		pDst->Blt(x, y, nSizeDst, nSizeDst, pDibTmp, 0, nSizeDst, bUseColorKey);
	} else {
		pDst->BltAlpha(x, y, nSizeDst, nSizeDst, pDibTmp, 0, nSizeDst, byLevel, bUseColorKey);
	}

Exit:
	ReleaseDibTmp();
}


void CMgrDraw::DrawMapShadow(
	CImg32 *pDst,				// [in] 描画先
	int x,						// [in] 描画位置(X)
	int y,						// [in] 描画位置(Y)
	DWORD dwShadowID,			// [in] マップ影ID
	BOOL bSingleSize,			// [in] 等倍指定
	BOOL bUseColorKey,/*TRUE*/	// [in] TRUE:重ね合わせする
	BOOL bLock)/*TRUE*/			// [in] テンポラリをロックする
{
	int nSize, nSizeDst;
	WORD wGrpIDBase;
	PCImg32 pImg, pDibTmp;
	PCInfoMapShadow pInfoMapShadow;
	PCInfoAnime pAnime;

	pDibTmp = GetDibTmp();
	nSize = 16;
	nSizeDst = 32;
	if (bSingleSize) {
		nSizeDst = 16;
	}

	pInfoMapShadow = (PCInfoMapShadow)m_pLibInfoMapShadow->GetPtr(dwShadowID);
	if (pInfoMapShadow == NULL) {
		pDst->FillRect(x, y, nSizeDst, nSizeDst, RGB(0, 0, 0));
		goto Exit;
	}

	wGrpIDBase = pInfoMapShadow->m_wGrpID;
	if (pInfoMapShadow->GetAnimeCount() > 0) {
		pAnime = pInfoMapShadow->m_paAnimeInfo->at(pInfoMapShadow->m_byAnimeNo);
		wGrpIDBase = pAnime->m_wGrpIDBase;
	}
	if (bSingleSize) {
		pImg = m_pMgrGrpData->GetDibMapShadowTmp(wGrpIDBase / 1024);
		pDibTmp->BltFrom256(0, 0, nSize, nSize, pImg, ((wGrpIDBase % 1024) % 32) * nSize, ((wGrpIDBase % 1024) / 32) * nSize);
	} else {
		pImg = m_pMgrGrpData->GetDibMapShadow(wGrpIDBase / 1024);
		pDibTmp->BltFrom256(0, 0, nSize, nSize, pImg, ((wGrpIDBase % 1024) % 32) * nSize, ((wGrpIDBase % 1024) / 32) * nSize);
	}

	pDibTmp->BltStretchNearest(0, nSizeDst, nSizeDst, nSizeDst, pDibTmp, 0, 0, nSize, nSize);

	if (bSingleSize) {
		pDst->Blt(x, y, nSizeDst, nSizeDst, pDibTmp, 0, nSizeDst, bUseColorKey);
	} else {
		pDst->BltAlpha(x, y, nSizeDst, nSizeDst, pDibTmp, 0, nSizeDst, 75, TRUE);
	}

Exit:
	ReleaseDibTmp();
}


void CMgrDraw::DrawMapObject(
	CImg32 *pDst,				// [in] 描画先
	int x,						// [in] 描画位置(X)
	int y,						// [in] 描画位置(Y)
	DWORD dwObjectID,			// [in] オブジェクトID
	BOOL bUseColorKey,/*TRUE*/	// [in] TRUE:重ね合わせする
	BOOL bLock)/*TRUE*/			// [in] テンポラリをロックする
{
	int xx, yy;
	SIZE sizeSrc, sizeDst;
	WORD wGrpID;
	PCImg32 pImg, pDibTmp;
	PCInfoMapObject pInfo;
	PSTMAPOBJECTANIMEINFO pAnimeInfo;
	PCLibInfoMapObject pLibInfoMapObject;

	pDibTmp = GetDibTmp();

	pLibInfoMapObject = m_pMgrData->GetLibInfoMapObject();
	pInfo = (PCInfoMapObject)pLibInfoMapObject->GetPtr(dwObjectID);
	if (pInfo == NULL) {
		goto Exit;
	}
	pAnimeInfo = pInfo->GetAnimePtr(0);
	if (pAnimeInfo == NULL) {
		goto Exit;
	}

	sizeSrc = pInfo->m_sizeGrp;
	sizeSrc.cx *= 16;
	sizeSrc.cy *= 16;
	sizeDst = sizeSrc;
	sizeDst.cx *= 2;
	sizeDst.cy *= 2;

	for (yy = 0; yy < pInfo->m_sizeGrp.cy; yy++) {
		for (xx = 0; xx < pInfo->m_sizeGrp.cx; xx++) {
			wGrpID = pAnimeInfo->pwGrpID[yy * pInfo->m_sizeGrp.cx + xx];
			pImg = m_pMgrGrpData->GetDibMapParts(wGrpID / 1024);
			if (pImg == NULL) {
				continue;
			}
			pDibTmp->BltFrom256(xx * 16, yy * 16, 16, 16, pImg, ((wGrpID % 1024) % 32) * 16, ((wGrpID % 1024) / 32) * 16);
		}
	}

	pDibTmp->BltStretchNearest(0, sizeDst.cy, sizeDst.cx, sizeDst.cy, pDibTmp, 0, 0, sizeSrc.cx, sizeSrc.cy);

	pDst->Blt(x, y - (sizeDst.cy - 32), sizeDst.cx, sizeDst.cy, pDibTmp, 0, sizeDst.cy, bUseColorKey);

Exit:
	ReleaseDibTmp();
}


void CMgrDraw::DrawItem(
	CImg32 *pDst,		// [in] 描画先
	int x,				// [in] 描画位置(X)
	int y,				// [in] 描画位置(Y)
	DWORD dwItemID,		// [in] アイテムID
	BYTE byLevel)/*0*/	// [in] 透明度
{
	PCInfoItem pInfoItem;

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr(dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	DrawItem(pDst, x, y, pInfoItem, byLevel);
}


void CMgrDraw::DrawItem(
	CImg32 *pDst,				// [in] 描画先
	int x,						// [in] 描画位置(X)
	int y,						// [in] 描画位置(Y)
	CInfoItem *pInfoItem,		// [in] アイテム情報
	BYTE byLevel,/*0*/			// [in] 透明度
	BOOL bLock)/*TRUE*/			// [in] テンポラリをロックする
{
	PCImg32 pImg, pDibTmp;

	pDibTmp = GetDibTmp();
	if (pInfoItem == NULL) {
		goto Exit;
	}
//Todo:
	pImg = m_pMgrGrpData->GetDibItem(pInfoItem->m_dwGrpID % 1024);
	if (pImg == NULL) {
		goto Exit;
	}
	pDibTmp->BltFrom256(0, 0, 16, 16, pImg, (pInfoItem->m_dwGrpID % 32) * 16, (pInfoItem->m_dwGrpID / 32) * 16);

	pDibTmp->BltStretchNearest(0, 32, 32, 32, pDibTmp, 0, 0, 16, 16);

	if (byLevel == 0) {
		pDst->Blt(x, y, 32, 32, pDibTmp, 0, 32, TRUE);
	} else {
		pDst->BltAlpha(x, y, 32, 32, pDibTmp, 0, 32, byLevel, TRUE);
	}

Exit:
	ReleaseDibTmp();
}


void CMgrDraw::DrawBalloon(
	CImg32 *pDst,			// [in] 描画先
	int x,					// [in] 描画位置(X)
	int y,					// [in] 描画位置(Y)
	DWORD dwBalloonID)		// [in] 噴出しID
{
	int cx, cy;
	PCImg32 pImg, pDibTmp;
	POINT ptTmp;

	pDibTmp = GetDibTmp();
	if (dwBalloonID == 0) {
		goto Exit;
	}
	dwBalloonID--;
//Todo:
	pImg = m_pMgrGrpData->GetDib(GRPIDMAIN_EFCBALLOON, (WORD)dwBalloonID, 0);
	if (pImg == NULL) {
		goto Exit;
	}
	cx = cy = m_pMgrGrpData->GetGrpSize(GRPIDMAIN_EFCBALLOON);
	m_pMgrGrpData->GetGrpPos(GRPIDMAIN_EFCBALLOON, (WORD)dwBalloonID, ptTmp);
	pDibTmp->BltFrom256(0, 0, 16, 16, pImg, ptTmp.x, ptTmp.y);

	pDibTmp->BltStretchNearest(0, cy * 2, cx * 2, cy * 2, pDibTmp, 0, 0, cx, cy);

	pDst->Blt(x, y, cx * 2, cy * 2, pDibTmp, 0, cy * 2, TRUE);

Exit:
	ReleaseDibTmp();
}


void CMgrDraw::DrawTextEffect(
	CImg32 *pDst,							// [in] 描画先
	int x,									// [in] 描画位置(X)
	int y,									// [in] 描画位置(Y)
	CInfoTextEffect *pInfoTextEffect)		// [in] 文字エフェクト情報
{
	BOOL bResult;
	BYTE byLevel;
	int cx, cy, i, nCount;
	PCImg32 pImg, pDibTmp;
	POINT ptSrc, ptTmp;

	pDibTmp = GetDibTmp();
	if (pInfoTextEffect == NULL) {
		goto Exit;
	}

	cx = 10;
	cy = 12;
	pImg = m_pMgrGrpData->GetDibNumS();
	if (pInfoTextEffect->m_nSize == TEXTEFFECTNUMSIZE_MEDIUM) {
		pImg = m_pMgrGrpData->GetDibNumM();
		cx = 14;
		cy = 17;
	}
	if (pInfoTextEffect->m_nSize == TEXTEFFECTNUMSIZE_LARGE) {
		pImg = m_pMgrGrpData->GetDibNumL();
		cx = 28;
		cy = 34;
	}
	if (pImg == NULL) {
		goto Exit;
	}

	ptSrc.y = pInfoTextEffect->m_nColor * cy;

	nCount = pInfoTextEffect->GetGrpCount();
	pDibTmp->FillRect(0, 0, nCount * cx, cy * 2 + 20, RGB(255, 0, 255));
	for (i = 0; i < nCount; i++) {
		bResult = pInfoTextEffect->GetDrawPos(i, ptTmp, byLevel);
		if (bResult == FALSE) {
			continue;
		}
		ptSrc.x = pInfoTextEffect->GetGrpNo(i) * cx;
		pDibTmp->BltFrom256(cx * i + ptTmp.x, cy + ptTmp.y + 20, cx, cy, pImg, ptSrc.x, ptSrc.y);
	}

	if (byLevel == 0) {
		pDst->Blt(x - (cx * nCount / 2), y - cy, (cx * nCount), cy * 2 + 20, pDibTmp, 0, 0, TRUE);
	} else {
		pDst->BltAlpha(x - (cx * nCount / 2), y - cy, (cx * nCount), cy * 2 + 20, pDibTmp, 0, 0, byLevel, TRUE);
	}

Exit:
	ReleaseDibTmp();
}


void CMgrDraw::DrawMapHitMark(
	CImg32 *pDst,		// [in] 描画先
	int x,				// [in] 描画位置(X)
	int y,				// [in] 描画位置(Y)
	int nNo)			// [in] 種別
{
	CImg32 *pDibSystem;

	pDibSystem = m_pMgrGrpData->GetDibSystem();
	pDst->Blt(x, y, 32, 32, pDibSystem, nNo * 32, 544, TRUE);
}


void CMgrDraw::DrawCursor(
	CImg32 *pDst,		// [in] 描画先
	int x,				// [in] 描画位置(X)
	int y,				// [in] 描画位置(Y)
	int nType)			// [in] 種別
{
	POINT apt[] = { 48, 0, 72, 0, 48, 24, 72, 24 };
	CImg32 *pDibSystem;

	if (nType > 3) {
		return;
	}

	pDibSystem = m_pMgrGrpData->GetDibSystem();
	pDst->BltFrom256(x, y, 24, 24, pDibSystem, apt[nType].x, apt[nType].y, TRUE);
}


void CMgrDraw::DrawSkillMenu(
	CImg32 *pDst,		// [in] 描画先
	int x,				// [in] 描画位置(X)
	int y)				// [in] 描画位置(Y)
{
	CImg32 *pDibSystem = m_pMgrGrpData->GetDibSystem();

	pDst->BltFrom256(x, y, 192, 192, pDibSystem, 368, 432, TRUE);
}


void CMgrDraw::DrawSkillType(
	CImg32 *pDst,		// [in] 描画先
	int x,				// [in] 描画位置(X)
	int y,				// [in] 描画位置(Y)
	int nType,			// [in] 種別
	BOOL bActive)		// [in] TRUE:選択 FALSE:非選択
{
	int yy;
	CImg32 *pDibSystem = m_pMgrGrpData->GetDibSystem();

	yy		= (bActive) ? 320 : 376;
	nType	= min(nType, 2);

	pDst->BltFrom256(x, y, 52, 56, pDibSystem, 368 + (nType * 52), yy, TRUE);
}


void CMgrDraw::DrawIcon(
	CImg32 *pDst,		// [in] 描画先
	int x,				// [in] 描画位置(X)
	int y,				// [in] 描画位置(Y)
	int nIndex)			// [in] アイコン番号
{
	int cx, cy;
	PCImg32 pImg, pDibTmp;

	cx = cy = 16;
	pDibTmp	= GetDibTmp();
	pImg	= m_pMgrGrpData->GetDibIcon();
	pDibTmp->BltFrom256(0, 0, 16, 16, pImg, nIndex % 20 * cx, nIndex / 20 * cy);

	pDibTmp->BltStretchNearest(0, cy * 2, cx * 2, cy * 2, pDibTmp, 0, 0, cx, cy);

	pDst->Blt(x, y, cx * 2, cy * 2, pDibTmp, 0, cy * 2, TRUE);
	ReleaseDibTmp();
}


BOOL CMgrDraw::TimerProc(void)
{
	BOOL bRet;
	DWORD dwTime;

	dwTime = SDL_GetTicks();

	bRet = Fade();

	return bRet;
}


BOOL CMgrDraw::Fade(void)
{
	int nTmp;
	BOOL bRet;
	DWORD dwTime;
	WPARAM wParam;

	bRet = FALSE;
	if (m_byFadeState == FADESTATE_NONE) {
		goto Exit;
	}

	dwTime = SDL_GetTicks();

	if (m_dwLastFade == 0) {
		m_dwLastFade = dwTime;
	}

	wParam		= 0;
	nTmp		= (dwTime - m_dwLastFade) / 4;
	if (nTmp == 0) {
		goto Exit;
	}

	switch (m_byFadeState) {
	case FADESTATE_FADEIN:
		nTmp = min(100, nTmp + m_byLevel);
		if (nTmp == 100) {
			wParam = (WPARAM)MGRDRAWMSG_END_FADEIN;
		}
		break;
	case FADESTATE_FADEOUT:
		nTmp = max(1, m_byLevel - nTmp);
		if (nTmp == 1) {
			wParam = (WPARAM)MGRDRAWMSG_END_FADEOUT;
		}
		break;
	}

	m_byLevel = (BYTE)nTmp;
	m_dwLastFade = dwTime;

	if (wParam != 0) {
		m_pMgrData->PostMgrDrawMessage((int)wParam, 0);
		SetFadeState(FADESTATE_NONE);
	}

	bRet = TRUE;
Exit:
	return bRet;
}


CImg32 *CMgrDraw::GetDibTmp(void)
{
	m_CritDataLock.Enter();
	return m_pDibTmp;
}


void CMgrDraw::ReleaseDibTmp(void)
{
	m_CritDataLock.Leave();
}


BOOL CMgrDraw::IsFade(void)
{
	BOOL bRet;

	bRet = FALSE;
	if (m_byFadeState != FADESTATE_NONE) {
		bRet = TRUE;
	}
	if (m_dwLastFade) {
		bRet = TRUE;
	}
	if (m_byLevel < 100 && m_byLevel > 0) {
		bRet = TRUE;
	}

	return bRet;
}


void CMgrDraw::SetLevel(BYTE byLevel)
{
	m_byLevel = byLevel;
}


void CMgrDraw::SetFadeState(BYTE byFadeState)
{
	m_byFadeState	= byFadeState;
	m_dwLastFade	= 0;

	switch (m_byFadeState) {
	case FADESTATE_FADEIN:
		if (m_byLevel == 0) {
			SetLevel(1);
		}
		m_pMgrData->PostMgrDrawMessage(MGRDRAWMSG_START_FADEIN, 0);
		break;
	case FADESTATE_FADEOUT:
		if (m_byLevel == 0) {
			SetLevel(100);
		}
		m_pMgrData->PostMgrDrawMessage(MGRDRAWMSG_START_FADEOUT, 0);
		break;
	}
}


void CMgrDraw::Clear(void)
{
	m_pDibBack->Clear();
}


void CMgrDraw::SaveScreenShot(void)
{
	int cx, cy;
	TCHAR szName[MAX_PATH];
	TCHAR szTmp[128];
	TCHAR szTmp2[20];
	SYSTEMTIME sysTime;
	CImg32 ImgTmp;

	// ファイル名の作成
	BuildModuleRelativePath(szName, _countof(szName), _T("ss\\Sbo"));

	GetLocalTime(&sysTime);
	_stprintf_s(szTmp2, _countof(szTmp2), _T("%04d"), sysTime.wYear);
	_stprintf_s(szTmp, _countof(szTmp), _T("%s%02d%02d%02d%02d%02d"),
			&szTmp2[2],
			sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	_tcscat_s(szName, _countof(szName), szTmp);
	_tcscat_s(szName, _countof(szName), _T(".png"));

	std::string szNameUtf8 = TStringToUtf8Std(szName);

	cx = m_pDibBack->Width();
	cy = m_pDibBack->Height();
	ImgTmp.CreateWithoutGdi(SCRSIZEX, SCRSIZEY);
	ImgTmp.Blt(0, 0, SCRSIZEX, SCRSIZEY, m_pDibBack, (cx - SCRSIZEX) / 2, (cy - SCRSIZEY) / 2);
	m_pMgrGrpData->Write(szNameUtf8.c_str(), &ImgTmp);
}


void CMgrDraw::LockDibTmp(void)
{
	GetDibTmp();
}


void CMgrDraw::UnLockDibTmp(void)
{
//	m_pDibTmp->Unlock();
	ReleaseDibTmp();
}
