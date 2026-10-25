/// @file MgrDraw.cpp
/// @brief 描画マネージャクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>
// wasm SIMD が有効な場合は wasm_simd128.h をインクルード
#ifdef __wasm_simd128__
#include <wasm_simd128.h>
#endif
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
#include <set>
#include <string>
#include <vector>

#if defined(__EMSCRIPTEN__)
// SDLApp.cpp で定義した描画区間計測用グローバル変数
extern DWORD g_dwDrawLayerMs;
extern DWORD g_dwDrawSwizzleMs;
extern DWORD g_dwDrawPresentMs;
#endif

#if defined(_DEBUG)
static void LogMissingMapPartsAtlasOnce(
	CMgrGrpData *pMgrGrpData,
	PCInfoMapParts pInfoMapParts,
	WORD wGrpID,
	LPCSTR pszKind)
{
	static std::set<unsigned long long> s_setLogged;
	const int nAtlasNo = static_cast<int>(wGrpID / 1024);
	const int nLoadedAtlasCount = pMgrGrpData ? (pMgrGrpData->GetMapPartsCount() / 1024) : 0;
	const DWORD dwPartsID = pInfoMapParts ? pInfoMapParts->m_dwPartsID : 0;
	const unsigned long long ullKey =
		(static_cast<unsigned long long>(dwPartsID) << 32) |
		static_cast<unsigned long long>(wGrpID);

	if (s_setLogged.insert(ullKey).second == false) {
		return;
	}

	SboDbgLog(
		"[DrawMapParts] %s 用画像不足 partsID=%u grpID=%u atlas=%d loadedAtlas=%d animeNo=%u animeCount=%d partsType=0x%08X",
		pszKind,
		dwPartsID,
		static_cast<unsigned int>(wGrpID),
		nAtlasNo,
		nLoadedAtlasCount,
		pInfoMapParts ? static_cast<unsigned int>(pInfoMapParts->m_byAnimeNo) : 0,
		pInfoMapParts ? pInfoMapParts->GetAnimeCount() : 0,
		pInfoMapParts ? static_cast<unsigned int>(pInfoMapParts->m_dwPartsType) : 0);
}

static void LogMissingMapShadowAtlasOnce(
	CMgrGrpData *pMgrGrpData,
	PCInfoMapShadow pInfoMapShadow,
	WORD wGrpID)
{
	static std::set<unsigned long long> s_setLogged;
	const int nAtlasNo = static_cast<int>(wGrpID / 1024);
	const int nLoadedAtlasCount = pMgrGrpData ? (pMgrGrpData->GetMapShadowCount() / 1024) : 0;
	const DWORD dwShadowID = pInfoMapShadow ? pInfoMapShadow->m_dwShadowID : 0;
	const unsigned long long ullKey =
		(static_cast<unsigned long long>(dwShadowID) << 32) |
		static_cast<unsigned long long>(wGrpID);

	if (s_setLogged.insert(ullKey).second == false) {
		return;
	}

	SboDbgLog(
		"[DrawMapShadow] 画像不足 shadowID=%u grpID=%u atlas=%d loadedAtlas=%d animeNo=%u animeCount=%d",
		dwShadowID,
		static_cast<unsigned int>(wGrpID),
		nAtlasNo,
		nLoadedAtlasCount,
		pInfoMapShadow ? static_cast<unsigned int>(pInfoMapShadow->m_byAnimeNo) : 0,
		pInfoMapShadow ? pInfoMapShadow->GetAnimeCount() : 0);
}
#endif

#if defined(__EMSCRIPTEN__)
EM_JS(void, SBOP2_PresentToCanvas, (int pRgba, int width, int height, int uiScale), {
	// shell.html の pump fallback に「描画が生きている」ことを知らせる
	// （このカウンタの未更新で fallback が誤動作し OnFrame が二重駆動されていた）
	Module.sbop2OnDrawCount = (Module.sbop2OnDrawCount || 0) + 1;
	const canvas = Module['canvas'];
	if (!canvas) {
		return;
	}
	// キャンバスサイズが異なる場合のみ変更（初回のみ走る想定）
	if (canvas.width !== width) {
		canvas.width = width;
	}
	if (canvas.height !== height) {
		canvas.height = height;
	}
	let ctx = Module['sbop2Ctx2d'];
	if (!ctx) {
		ctx = canvas.getContext('2d', { willReadFrequently: true });
		if (!ctx) {
			return;
		}
		// imageSmoothingEnabled は 2D コンテキスト生成直後に一度だけ設定
		ctx.imageSmoothingEnabled = false;
		Module['sbop2Ctx2d'] = ctx;
	}

	// ImageData キャッシュ: 毎フレームの new ImageData / new Uint8ClampedArray 生成を回避する
	// ALLOW_MEMORY_GROWTH=1 で Wasm memory grow が起きると HEAPU8.buffer が新しい
	// ArrayBuffer に差し替わるため、前回の buffer 参照と比較して検出する
	const base = pRgba >>> 0;
	const wh = width * height;
	let imageData = Module.sbop2ImageData;
	if (!imageData
		|| Module.sbop2HeapBuffer !== HEAPU8.buffer
		|| Module.sbop2ImageDataBase !== base
		|| Module.sbop2ImageDataWH !== wh) {
		// buffer 差し替え or アドレス/サイズ変化 → キャッシュを再生成
		imageData = new ImageData(
			new Uint8ClampedArray(HEAPU8.buffer, base, wh * 4),
			width,
			height
		);
		Module.sbop2ImageData     = imageData;
		Module.sbop2HeapBuffer    = HEAPU8.buffer;
		Module.sbop2ImageDataBase = base;
		Module.sbop2ImageDataWH   = wh;
	}
	ctx.putImageData(imageData, 0, 0);

	const rectQueue = Module.sbop2RectQueue || [];
	if (rectQueue.length > 0) {
		ctx.save();
		// rect/text キューは論理(UI)座標。canvas は uiScale 倍の解像度なので変換で合わせる
		ctx.setTransform(uiScale, 0, 0, uiScale, 0, 0);
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
		ctx.setTransform(uiScale, 0, 0, uiScale, 0, 0);
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
#endif


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

	m_bWorldDrawn			= FALSE;
	m_nScaledCacheBytes		= 0;
	m_nGroundMapX			= 0;
	m_nGroundMapY			= 0;

	m_pDibBack = new CImg32;
	m_pDibBack->Create(SCRSIZEX + UI_MARGIN * 2, SCRSIZEY + UI_MARGIN * 2);
	m_pDibWorld = new CImg32;
	m_pDibWorld->Create(WORLD_BUFSIZEX, WORLD_BUFSIZEY);
	m_pDibTmp = new CImg32;
	m_pDibTmp->Create((SCRSIZEX + 64) * 2 * DRAW_SCALE, (SCRSIZEY + 64) * 2 * DRAW_SCALE);
	m_pDibTmp->SetColorKey(RGB(255, 0, 255));
}


CMgrDraw::~CMgrDraw()
{
	if (m_pBackTexture != NULL) {
		SDL_DestroyTexture(m_pBackTexture);
		m_pBackTexture = NULL;
	}
	ClearScaledCache();
	SAFE_DELETE(m_pDibBack);
	SAFE_DELETE(m_pDibWorld);
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
#if defined(__EMSCRIPTEN__)
	// 区間計測: レイヤー合成開始
	DWORD dwT0 = SDL_GetTicks();
#endif

	// UI(論理解像度)バッファへレイヤー→ウィンドウの順に描く。
	// ワールド(CLayerMap)は m_pDibWorld(DRAW_SCALE倍)へ直接描き、描き終えた時点で BeginUiOverlay を呼んで
	// UIバッファをワールドの縮小で初期化する(半透明ウィンドウ等が背景と正しく混ざるように)。
	m_bWorldDrawn = FALSE;
	m_pMgrLayer->	Draw(m_pDibBack);		// レイヤー
	m_pMgrWindow->	Draw(m_pDibBack);		// ウィンドウ
	if (m_bWorldDrawn) {
		CompositeUiOverlay();				// UIバッファの変更分だけワールドへ拡大して重ねる
	} else {
		UpscaleUiToWorld();					// ワールド非描画(タイトル等): UI全体を拡大して出力
	}

	// 明度指定あり？
	if (m_byLevel < 100 && m_byLevel > 0) {
		m_pDibWorld->ChgLevel(WORLD_MARGIN, WORLD_MARGIN, WORLD_SCRSIZEX, WORLD_SCRSIZEY, (m_byLevel == 1) ? 0 : m_byLevel);
	}

#if defined(__EMSCRIPTEN__)
	{
		// 区間計測: レイヤー合成終了
		DWORD dwT1 = SDL_GetTicks();
		g_dwDrawLayerMs = dwT1 - dwT0;

		static std::vector<unsigned char> aCanvasRgba;
		const int nWidth = WORLD_SCRSIZEX;
		const int nHeight = WORLD_SCRSIZEY;
		const int nFullWidth = m_pDibWorld->Width();
		const int nFullHeight = m_pDibWorld->Height();
		const BYTE *pSrc = m_pDibWorld->GetBits();

		aCanvasRgba.resize((size_t)nWidth * (size_t)nHeight * 4);
		for (int y = 0; y < nHeight; ++y) {
			const uint32_t *pSrcRow = reinterpret_cast<const uint32_t *>(
				pSrc + ((size_t)(nFullHeight - 1 - (WORLD_MARGIN + y)) * (size_t)nFullWidth + WORLD_MARGIN) * 4);
			uint32_t *pDstRow = reinterpret_cast<uint32_t *>(
				&aCanvasRgba[(size_t)y * (size_t)nWidth * 4]);
#if defined(__wasm_simd128__)
			// SIMD 版: 一度に 4 ピクセル（16 バイト）処理
			// 入力: B G R A B G R A B G R A B G R A  (バイトインデックス 0-15)
			// 出力: R G B A R G B A R G B A R G B A
			// shuffle index [2,1,0,3, 6,5,4,7, 10,9,8,11, 14,13,12,15] で B↔R swap
			const v128_t alpha_mask = wasm_i32x4_splat((int)0xFF000000u);
			int xSimd = 0;
			for (; xSimd <= nWidth - 4; xSimd += 4) {
				v128_t px = wasm_v128_load(pSrcRow + xSimd);
				v128_t swapped = wasm_i8x16_shuffle(px, px,
					2,1,0,3,  6,5,4,7,  10,9,8,11,  14,13,12,15);
				// A チャンネルを 0xFF に固定（元の A 値を上書き）
				v128_t out = wasm_v128_or(swapped, alpha_mask);
				wasm_v128_store(pDstRow + xSimd, out);
			}
			// 端数ピクセル（SCRSIZEX が 4 の倍数でない場合のフォールバック）
			for (int x = xSimd; x < nWidth; ++x) {
				uint32_t px = pSrcRow[x];
				pDstRow[x] = (px & 0x0000FF00u)
				           | ((px & 0x00FF0000u) >> 16)
				           | ((px & 0x000000FFu) << 16)
				           | 0xFF000000u;
			}
#else
			// スカラー版フォールバック: BGRA → RGBA、A=0xFF 固定
			for (int x = 0; x < nWidth; ++x) {
				// BGRA → RGBA: R と B を入れ替え、A=0xFF 固定
				uint32_t px = pSrcRow[x];
				pDstRow[x] = (px & 0x0000FF00u)           // G そのまま
				           | ((px & 0x00FF0000u) >> 16)    // B → R
				           | ((px & 0x000000FFu) << 16)    // R → B
				           | 0xFF000000u;                  // A=255
			}
#endif
		}

		// 区間計測: BGRA→RGBA 変換終了
		DWORD dwT2 = SDL_GetTicks();
		g_dwDrawSwizzleMs = dwT2 - dwT1;

		SBOP2_PresentToCanvas((int)(intptr_t)aCanvasRgba.data(), nWidth, nHeight, DRAW_SCALE);

		// 区間計測: Present 終了
		DWORD dwT3 = SDL_GetTicks();
		g_dwDrawPresentMs = dwT3 - dwT2;
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

		pBits	= m_pDibWorld->GetBits();
		nW		= m_pDibWorld->Width();
		nStride	= nW * 4;					// 1行あたりのバイト数（32bit BGRA）

		// Lazy 初期化: テクスチャを初回のみ作成
		if (m_pBackTexture == NULL) {
			// SDL_PIXELFORMAT_ARGB8888: CImg32(BI_RGB 32bit) のメモリ配置 [B][G][R][0x00] に対応
			// リトルエンディアンでは ARGB8888 のバイト順が [B][G][R][A] となり一致する
			m_pBackTexture = SDL_CreateTexture(
								pRenderer,
								SDL_PIXELFORMAT_ARGB8888,
								SDL_TEXTUREACCESS_STREAMING,
								WORLD_SCRSIZEX,
								WORLD_SCRSIZEY);
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

		// ボトムアップ DIB の (x=WORLD_MARGIN, y=WORLD_MARGIN) から WORLD_SCRSIZEX × WORLD_SCRSIZEY 行を転送
		// ボトムアップ DIBでは y=0 がメモリ先頭（画像の最下行）
		// 下から WORLD_MARGIN 行目（描画開始オフセット）: pBits + WORLD_MARGIN * nStride + WORLD_MARGIN * 4
		SDL_UpdateTexture(m_pBackTexture, NULL,
							pBits + WORLD_MARGIN * nStride + WORLD_MARGIN * 4,
							nStride);

		// SDL_FLIP_VERTICAL で垂直フリップ → ボトムアップ DIB を正しい向きに
		stDst.x = 0;
		stDst.y = 0;
		stDst.w = WORLD_SCRSIZEX;
		stDst.h = WORLD_SCRSIZEY;
		SDL_RenderCopyEx(pRenderer, m_pBackTexture, NULL, &stDst,
							0.0, NULL, SDL_FLIP_VERTICAL);
	}
}


CMgrDraw::STScaledCache *CMgrDraw::FindScaledCache(unsigned long long ullKey)
{
	std::unordered_map<unsigned long long, STScaledCache>::iterator it = m_mapScaledCache.find(ullKey);
	if (it == m_mapScaledCache.end()) {
		return NULL;
	}
	return &it->second;
}


CMgrDraw::STScaledCache *CMgrDraw::StoreScaledCache(unsigned long long ullKey, CImg32 *pSrc, int sx, int sy, int cx, int cy)
{
	const size_t nBytes = (size_t)cx * (size_t)cy * 4;
	STScaledCache stNew;

	// 上限(48MB)を超えたら全破棄して作り直す(ポインタは呼び出し側で都度取り直す)
	if (m_nScaledCacheBytes + nBytes > (size_t)48 * 1024 * 1024) {
		ClearScaledCache();
	}

	stNew.pImg = new CImg32;
	stNew.pImg->CreateWithoutGdi(cx, cy);
	stNew.pImg->SetColorKey(RGB(255, 0, 255));
	stNew.pImg->Blt(0, 0, cx, cy, pSrc, sx, sy, FALSE);

	// カラーキー画素の有無を調べる
	stNew.bHasKey = FALSE;
	{
		const DWORD dwKey = stNew.pImg->GetColorKey();
		const DWORD *pBits = reinterpret_cast<const DWORD *>(stNew.pImg->GetBits());
		const size_t nCount = (size_t)cx * (size_t)cy;
		for (size_t i = 0; i < nCount; ++i) {
			if (pBits[i] == dwKey) {
				stNew.bHasKey = TRUE;
				break;
			}
		}
	}

	m_nScaledCacheBytes += nBytes;
	std::pair<std::unordered_map<unsigned long long, STScaledCache>::iterator, bool> res =
		m_mapScaledCache.insert(std::make_pair(ullKey, stNew));
	return &res.first->second;
}


void CMgrDraw::ClearScaledCache(void)
{
	std::unordered_map<unsigned long long, STScaledCache>::iterator it;
	for (it = m_mapScaledCache.begin(); it != m_mapScaledCache.end(); ++it) {
		delete it->second.pImg;
	}
	m_mapScaledCache.clear();
	m_nScaledCacheBytes = 0;
}


CImg32 *CMgrDraw::GetDibWorld(void)
{
	return m_pDibWorld;
}


int CMgrDraw::GetDrawScale(CImg32 *pDst)
{
	return (pDst == m_pDibWorld) ? DRAW_SCALE : 1;
}


void CMgrDraw::BeginUiOverlay(void)
{
	// UIバッファの可視領域を、ワールドの各 DRAW_SCALE x DRAW_SCALE ブロックの左上画素で初期化する。
	// ボトムアップ DIB: ワールドのメモリ行 R と UI のメモリ行 r は R = r * DRAW_SCALE + (DRAW_SCALE - 1) で対応
	// （ブロック内の画像上の左上画素）。列は C = c * DRAW_SCALE。
	const int nUiW = m_pDibBack->Width();
	const int nWorldW = m_pDibWorld->Width();
	BYTE *pUi = m_pDibBack->GetBits();
	const BYTE *pWorld = m_pDibWorld->GetBits();

	m_bWorldDrawn = TRUE;

	for (int r = UI_MARGIN; r < UI_MARGIN + SCRSIZEY; ++r) {
		uint32_t *pDstRow = reinterpret_cast<uint32_t *>(pUi) + (size_t)r * nUiW;
		const uint32_t *pSrcRow = reinterpret_cast<const uint32_t *>(pWorld) + (size_t)(r * DRAW_SCALE + (DRAW_SCALE - 1)) * nWorldW;
		for (int c = UI_MARGIN; c < UI_MARGIN + SCRSIZEX; ++c) {
			pDstRow[c] = pSrcRow[c * DRAW_SCALE];
		}
	}
}


void CMgrDraw::CompositeUiOverlay(void)
{
	// BeginUiOverlay で入れた値から変化したUI画素だけを、ワールドへ DRAW_SCALE 倍で書き込む
	const int nUiW = m_pDibBack->Width();
	const int nWorldW = m_pDibWorld->Width();
	const uint32_t *pUi = reinterpret_cast<const uint32_t *>(m_pDibBack->GetBits());
	uint32_t *pWorld = reinterpret_cast<uint32_t *>(m_pDibWorld->GetBits());

	for (int r = UI_MARGIN; r < UI_MARGIN + SCRSIZEY; ++r) {
		const uint32_t *pUiRow = pUi + (size_t)r * nUiW;
		uint32_t *pWorldBase = pWorld + (size_t)(r * DRAW_SCALE) * nWorldW;
		const uint32_t *pSample = pWorldBase + (size_t)(DRAW_SCALE - 1) * nWorldW;
		for (int c = UI_MARGIN; c < UI_MARGIN + SCRSIZEX; ++c) {
			const uint32_t dwPixel = pUiRow[c];
			if (dwPixel == pSample[c * DRAW_SCALE]) {
				continue;
			}
			for (int j = 0; j < DRAW_SCALE; ++j) {
				uint32_t *pOut = pWorldBase + (size_t)j * nWorldW + c * DRAW_SCALE;
				for (int i = 0; i < DRAW_SCALE; ++i) {
					pOut[i] = dwPixel;
				}
			}
		}
	}
}


void CMgrDraw::UpscaleUiToWorld(void)
{
	// UIバッファ全体(余白込み)を最近傍で DRAW_SCALE 倍にしてワールドへ書く。
	// UI 余白 = WORLD_MARGIN / DRAW_SCALE なので同じ添字の対応で拡大できる。
	const int nUiW = m_pDibBack->Width();
	const int nUiH = m_pDibBack->Height();
	const int nWorldW = m_pDibWorld->Width();
	const uint32_t *pUi = reinterpret_cast<const uint32_t *>(m_pDibBack->GetBits());
	uint32_t *pWorld = reinterpret_cast<uint32_t *>(m_pDibWorld->GetBits());

	for (int r = 0; r < nUiH; ++r) {
		const uint32_t *pUiRow = pUi + (size_t)r * nUiW;
		uint32_t *pOut0 = pWorld + (size_t)(r * DRAW_SCALE) * nWorldW;
		for (int c = 0; c < nUiW; ++c) {
			const uint32_t dwPixel = pUiRow[c];
			for (int i = 0; i < DRAW_SCALE; ++i) {
				pOut0[c * DRAW_SCALE + i] = dwPixel;
			}
		}
		for (int j = 1; j < DRAW_SCALE; ++j) {
			memcpy(pOut0 + (size_t)j * nWorldW, pOut0, (size_t)nWorldW * sizeof(uint32_t));
		}
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
	int cx, cy, cxChar, cyChar, nTmp, nDirection, nEyeID, i, nCount, nFadeLevel, sc, nSt, nIllustCol, nIllustRow;
	POINT ptTmp;
	PCInfoMotion pInfoMotion;
	PCImg32 pSrc, pDibTmp, pImgIllust, pImgShadow;

	// 描画先がワールドバッファなら DRAW_SCALE 倍で描く(x,y は論理座標で受け取る)
	sc = GetDrawScale(pDst);
	nSt = 2 * sc;
	x *= sc;
	y *= sc;

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

	// イラスト版キャラ: 960描画・不透明のときだけ、res/illust/char/ の一枚絵があれば2x2合成の代わりに使う
	pImgIllust = NULL;
	nIllustCol = nIllustRow = 0;
	if ((sc == 2) && (nFadeLevel == 0) &&
		((wGrpIDMainBase == GRPIDMAIN_2X2_NPC) || (wGrpIDMainBase == GRPIDMAIN_2X2_CHAR))) {
		char szKey[48];
		szKey[0] = '\0';
		if (wGrpIDMainBase == GRPIDMAIN_2X2_NPC) {
			snprintf(szKey, sizeof(szKey), "npc_%u", (unsigned)wGrpIdNPC);
		} else if (pInfoChar->m_wGrpIDTmpMain == 0) {
			if (pInfoChar->m_wGrpIDSP == 0) {
				snprintf(szKey, sizeof(szKey), "cloth_%u", (unsigned)pInfoChar->m_wGrpIDCloth);
			} else {
				snprintf(szKey, sizeof(szKey), "spcloth_%u", (unsigned)pInfoChar->m_wGrpIDSP);
			}
		}
		// NPC は1体ごとに固定の絵(npcchar_<キャラID>)を優先し、無ければ上のキーで探す
		// (m_bNPC はクライアントへ送られないので、NPC 画像を持つかで判断する)
		if (pInfoChar->m_wGrpIDNPC != 0) {
			char szKeyNpc[48];
			snprintf(szKeyNpc, sizeof(szKeyNpc), "npcchar_%lu", (unsigned long)pInfoChar->m_dwCharID);
			if (m_pMgrGrpData->GetDibIllustChar(szKeyNpc) != NULL) {
				strcpy(szKey, szKeyNpc);
			}
		}
		if (szKey[0] != '\0') {
#if defined(_DEBUG)
			static std::set<std::string> s_setIllustCharKey;
			if (s_setIllustCharKey.insert(szKey).second) {
				SDL_Log("illust char key: %s", szKey);
			}
#endif
			pImgIllust = m_pMgrGrpData->GetDibIllustChar(szKey);
			if ((pImgIllust != NULL) && ((pImgIllust->Width() < 512) || (pImgIllust->Height() < 512))) {
				pImgIllust = NULL;
			}
		}
		if (pImgIllust != NULL) {
			// 従来の frame(0-15: 上4/下4/左4/右4) = サブ番号の桁位置 → 行(下0,左1,右2,上3)・列(歩行4コマ)
			static const int s_anRow[4] = { 3, 0, 1, 2 };
			m_pMgrGrpData->GetGrpPos(wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase - 1, ptTmp);
			nTmp = (ptTmp.x / cxChar) & 15;
			nIllustRow = s_anRow[nTmp / 4];
			nIllustCol = nTmp % 4;
		}
	}

	// キャラ影の描画
	if (pImgIllust == NULL) switch (wGrpIDMainBase) {
	case GRPIDMAIN_2X2_CHAR:
		m_pMgrGrpData->GetGrpPos(wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase - 1, ptTmp);
		pSrc = m_pMgrGrpData->GetDib2x2CharShadow(0);
		pDibTmp->BltFrom256(0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y);
		pDibTmp->BltStretchNearest(0, cyChar * 2, cxChar * nSt, cyChar * nSt, pDibTmp, 0, 0, cxChar, cyChar);
		pDst->BltAlpha(
			x + pInfoMotion->m_ptDrawPosPile0.x * sc,
			y + pInfoMotion->m_ptDrawPosPile0.y * sc,
			cxChar * nSt, cyChar * nSt, pDibTmp, 0, cyChar * 2, 75, TRUE);
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
		pDibTmp->BltStretchNearest(0, cyChar * 2, cxChar * nSt, cyChar * nSt, pDibTmp, 0, 0, cxChar, cyChar);
		pDst->BltAlpha(
			x + pInfoMotion->m_ptDrawPosPile0.x * sc,
			y + pInfoMotion->m_ptDrawPosPile0.y * sc,
			cxChar * nSt, cyChar * nSt, pDibTmp, 0, cyChar * 2, 75, TRUE);
		break;
	}

	nCount = pInfoMotion->m_anDrawList.size();
	for (i = 0; i < nCount; i++) {
		switch (pInfoMotion->m_anDrawList[i]) {
		case 0:
			if (pImgIllust != NULL) {
				// 足元の影(res/illust/char/shadow.png、セルと同じ128px)。無ければ影なし
				pImgShadow = m_pMgrGrpData->GetDibIllustChar("shadow");
				if (pImgShadow != NULL) {
					pDst->BltSrcOver(
						x + pInfoMotion->m_ptDrawPosPile0.x * sc,
						y + pInfoMotion->m_ptDrawPosPile0.y * sc,
						pImgShadow->Width(), pImgShadow->Height(), pImgShadow, 0, 0);
				}
				pDst->BltSrcOver(
					x + pInfoMotion->m_ptDrawPosPile0.x * sc,
					y + pInfoMotion->m_ptDrawPosPile0.y * sc,
					cxChar * nSt, cyChar * nSt, pImgIllust, nIllustCol * cxChar * nSt, nIllustRow * cyChar * nSt);
				break;
			}
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
				// (1) 残像対策: 転写前に使用領域をカラーキー色でクリア（転写が空振りしても前キャラの残像が出ないようにする）
				pDibTmp->FillRect(0, 0,          cxChar,     cyChar,     RGB(255, 0, 255));
				pDibTmp->FillRect(0, cyChar * 2, cxChar * nSt, cyChar * nSt, RGB(255, 0, 255));
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
				pDibTmp->BltStretchNearest(0, cyChar * 2, cxChar * nSt, cyChar * nSt, pDibTmp, 0, 0, cxChar, cyChar);
				pDst->BltAlpha(
					x + pInfoMotion->m_ptDrawPosPile0.x * sc,
					y + pInfoMotion->m_ptDrawPosPile0.y * sc,
					cxChar * nSt, cyChar * nSt, pDibTmp, 0, cyChar * 2, nFadeLevel, TRUE);

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
				pDibTmp->BltStretchNearest(0, cyChar * 2, cxChar * nSt, cyChar * nSt, pDibTmp, 0, 0, cxChar, cyChar);
				pDst->BltAlpha(
					x + pInfoMotion->m_ptDrawPosPile0.x * sc,
					y + pInfoMotion->m_ptDrawPosPile0.y * sc,
					cxChar * nSt, cyChar * nSt, pDibTmp, 0, cyChar * 2, nFadeLevel, TRUE);
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

			pDibTmp->BltStretchNearest(0, cy * 2, cx * nSt, cy * nSt, pDibTmp, 0, 0, cx, cy);
			pDst->BltAlpha(
				x + nTmp * sc + pInfoMotion->m_ptDrawPosPile1.x * sc,
				y + nTmp * sc + pInfoMotion->m_ptDrawPosPile1.y * sc,
				cx * nSt, cy * nSt, pDibTmp, 0, cy * 2, pInfoMotion->m_byLevel1, TRUE);
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

			pDibTmp->BltStretchNearest(0, cy * 2, cx * nSt, cy * nSt, pDibTmp, 0, 0, cx, cy);
			pDst->BltAlpha(
				x + nTmp * sc + pInfoMotion->m_ptDrawPosPile2.x * sc,
				y + nTmp * sc + pInfoMotion->m_ptDrawPosPile2.y * sc,
				cx * nSt, cy * nSt, pDibTmp, 0, cy * 2, pInfoMotion->m_byLevel2, TRUE);
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

			pDibTmp->BltStretchNearest(0, cy * 2, cx * nSt, cy * nSt, pDibTmp, 0, 0, cx, cy);
			pDst->BltAlpha(
				x + nTmp * sc + pInfoMotion->m_ptDrawPosPile3.x * sc,
				y + nTmp * sc + pInfoMotion->m_ptDrawPosPile3.y * sc,
				cx * nSt, cy * nSt, pDibTmp, 0, cy * 2, pInfoMotion->m_byLevel3, TRUE);
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
		pDibTmp->BltStretchNearest(0, cy * 2, cx * nSt, cy * nSt, pDibTmp, 0, 0, cx, cy);
		pDst->BltAlpha(x + nTmp * sc, y + nTmp * sc, cx * nSt, cy * nSt, pDibTmp, 0, cy * 2, pInfoChar->GetEfcLevel(), TRUE);
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
	int yy, cx, cy, sc, nSt;
	PCImg32 pSrc, pDibTmp;

	yy		= 0;
	pSrc	= pInfoChar->m_pDibChar;
	if (pSrc == NULL) {
		return;
	}

	sc = GetDrawScale(pDst);
	nSt = 2 * sc;
	x *= sc;
	y *= sc;
	pDibTmp = GetDibTmp();
	cx = 16;
	cy = 16;

	byAnimeNoTmp = byAnimeNo / 2;
	if ((byAnimeNo % 2) != 0) {
		byAnimeNoTmp = 2;
	}

	pDibTmp->Blt(0, 0, cx, cy, pSrc, (byDirection * 4 + byAnimeNoTmp) * cx, yy);

	pDibTmp->BltStretchNearest(0, cy * 2, cx * nSt, cy * nSt, pDibTmp, 0, 0, cx, cy);

	pDst->BltAlpha(x, y, cx * nSt, cy * nSt, pDibTmp, 0, cy * 2, byLevel, TRUE);
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
	int nSize, nSizeDst, sc, nSrcY;
	BOOL bCache, bHasKey;
	unsigned long long ullKey;
	STScaledCache *pCache;
	WORD wGrpIDBase, wGrpIDPile, wGrpIDIllust;
	PCImg32 pImg, pDibTmp, pSrcImg, pImgIllust;
	PCInfoAnime pAnime;
	int nIllustSrcX, nIllustSrcY, nGroundNo, nGroundGX, nGroundGY;

	nIllustSrcX = nIllustSrcY = 0;
	nGroundNo = -1;
	nGroundGX = nGroundGY = 0;
	sc = GetDrawScale(pDst);
	x *= sc;
	y *= sc;
	nSize = 16;
	nSizeDst = 32 * sc;
	if (bSingleSize) {
		nSizeDst = 16 * sc;
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

	// イラスト版タイル番号 ILLUSTTILE_HIDE は「ワールド描画では描かない」(一枚絵オブジェクトに置き換えた重ねパーツ用)
	if ((sc == 2) && (bSingleSize == FALSE) && (pInfoMapParts->m_wGrpIDIllust == ILLUSTTILE_HIDE)) {
		goto Exit;
	}

	// イラスト版タイル: ワールド描画(sc=2・等倍指定なし)で、シートがあるときだけ使う
	wGrpIDIllust = 0;
	pImgIllust = NULL;
	if ((nMode != 1) && (sc == 2) && (bSingleSize == FALSE) && (pInfoMapParts->m_wGrpIDIllust >= ILLUSTTILE_GROUND_BASE) && (pInfoMapParts->m_wGrpIDIllust <= ILLUSTTILE_GROUND_MAX)) {
		// 大判の地面テクスチャ: マップ座標(mod テクスチャのマス数)で64pxを切り出す
		nGroundNo = pInfoMapParts->m_wGrpIDIllust - ILLUSTTILE_GROUND_BASE;
		pImgIllust = m_pMgrGrpData->GetDibIllustGround(nGroundNo);
		if (pImgIllust != NULL) {
			const int nCellsX = pImgIllust->Width() / nSizeDst;
			const int nCellsY = pImgIllust->Height() / nSizeDst;
			nGroundGX = ((m_nGroundMapX % nCellsX) + nCellsX) % nCellsX;
			nGroundGY = ((m_nGroundMapY % nCellsY) + nCellsY) % nCellsY;
			nIllustSrcX = nGroundGX * nSizeDst;
			nIllustSrcY = nGroundGY * nSizeDst;
			wGrpIDIllust = pInfoMapParts->m_wGrpIDIllust;
		} else {
			nGroundNo = -1;
		}
	} else if ((nMode != 1) && (sc == 2) && (bSingleSize == FALSE) && (pInfoMapParts->m_wGrpIDIllust != 0) && (pInfoMapParts->m_wGrpIDIllust != ILLUSTTILE_HIDE)) {
		pImgIllust = m_pMgrGrpData->GetDibIllustTile((pInfoMapParts->m_wGrpIDIllust - 1) / 256);
		if (pImgIllust != NULL) {
			wGrpIDIllust = pInfoMapParts->m_wGrpIDIllust;
			const int nCellTmp = (wGrpIDIllust - 1) % 256;
			nIllustSrcX = (nCellTmp % 16) * nSizeDst;
			nIllustSrcY = (nCellTmp / 16) * nSizeDst;
		}
	}

	// ワールド描画(拡大タイル)は拡大結果をキャッシュして使い回す
	bCache = ((sc > 1) && (bSingleSize == FALSE)) ? TRUE : FALSE;
	bHasKey = TRUE;
	pSrcImg = pDibTmp;
	nSrcY = nSizeDst;
	ullKey = 0;
	if (bCache) {
		ullKey = ((unsigned long long)((nMode == 1) ? 0 : wGrpIDBase))
			| ((unsigned long long)wGrpIDPile << 16)
			| ((unsigned long long)nMode << 32)
			| ((unsigned long long)sc << 40)
			| ((unsigned long long)wGrpIDIllust << 48);	// イラスト版を含めて別キャッシュにする
		if (nGroundNo >= 0) {
			// 大判の地面: テクスチャ番号と切り出し位置(mx mod 列数, my mod 行数)を含めて衝突させない
			ullKey = ((unsigned long long)((nMode == 1) ? 0 : wGrpIDBase))
				| ((unsigned long long)wGrpIDPile << 16)
				| ((unsigned long long)(nMode & 3) << 32)
				| ((unsigned long long)(sc & 3) << 34)
				| (1ULL << 36)
				| ((unsigned long long)(nGroundNo & 0xFF) << 37)
				| ((unsigned long long)(nGroundGX & 0x1FF) << 45)
				| ((unsigned long long)(nGroundGY & 0x1FF) << 54);
		}
		pCache = FindScaledCache(ullKey);
		if (pCache != NULL) {
			pSrcImg = pCache->pImg;
			nSrcY = 0;
			bHasKey = pCache->bHasKey;
			goto Blit;
		}
	}

	if (nMode != 1) {
		pImg = m_pMgrGrpData->GetDibMapParts(wGrpIDBase / 1024);
		if (pImg == NULL) {
#if defined(_DEBUG)
			LogMissingMapPartsAtlasOnce(m_pMgrGrpData, pInfoMapParts, wGrpIDBase, "base");
#endif
			goto Exit;
		}
		pDibTmp->BltFrom256(0, 0, nSize, nSize, pImg, ((wGrpIDBase % 1024) % 32) * nSize, ((wGrpIDBase % 1024) / 32) * nSize);
	} else {
		pDibTmp->FillRect(0, 0, nSize, nSize, RGB(255, 0, 255));
	}
	if ((wGrpIDPile > 0) && (wGrpIDIllust == 0)) {
		pImg = m_pMgrGrpData->GetDibMapParts(wGrpIDPile / 1024);
		if (pImg == NULL) {
#if defined(_DEBUG)
			LogMissingMapPartsAtlasOnce(m_pMgrGrpData, pInfoMapParts, wGrpIDPile, "pile");
#endif
			goto Exit;
		}
		pDibTmp->BltFrom256(0, 0, nSize+1, nSize, pImg, ((wGrpIDPile % 1024) % 32) * nSize, ((wGrpIDPile % 1024) / 32) * nSize, TRUE);
	}

	if (wGrpIDIllust != 0) {
		// 下地(16px)を拡大して敷き、その上にイラスト版(64px)を画素アルファで重ね、最後にpileを重ねる
		pDibTmp->BltStretchNearest(0, nSizeDst, nSizeDst, nSizeDst, pDibTmp, 0, 0, nSize, nSize);
		pDibTmp->BltSrcOver(0, nSizeDst, nSizeDst, nSizeDst, pImgIllust, nIllustSrcX, nIllustSrcY);
		if (wGrpIDPile > 0) {
			pDibTmp->FillRect(0, 0, nSize, nSize, RGB(255, 0, 255));
			pImg = m_pMgrGrpData->GetDibMapParts(wGrpIDPile / 1024);
			if (pImg != NULL) {
				pDibTmp->BltFrom256(0, 0, nSize+1, nSize, pImg, ((wGrpIDPile % 1024) % 32) * nSize, ((wGrpIDPile % 1024) / 32) * nSize, TRUE);
				pDibTmp->BltStretchNearest(0, nSizeDst, nSizeDst, nSizeDst, pDibTmp, 0, 0, nSize, nSize, TRUE);
			}
		}
	} else {
		pDibTmp->BltStretchNearest(0, nSizeDst, nSizeDst, nSizeDst, pDibTmp, 0, 0, nSize, nSize);
	}

	if (bCache) {
		pCache = StoreScaledCache(ullKey, pDibTmp, 0, nSizeDst, nSizeDst, nSizeDst);
		pSrcImg = pCache->pImg;
		nSrcY = 0;
		bHasKey = pCache->bHasKey;
	}

Blit:
	if (byLevel == 0) {
		pDst->Blt(x, y, nSizeDst, nSizeDst, pSrcImg, 0, nSrcY, (bUseColorKey && bHasKey) ? TRUE : FALSE);
	} else {
		pDst->BltAlpha(x, y, nSizeDst, nSizeDst, pSrcImg, 0, nSrcY, byLevel, (bUseColorKey && bHasKey) ? TRUE : FALSE);
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
	int nSize, nSizeDst, sc;
	WORD wGrpIDBase;
	PCImg32 pImg, pDibTmp;
	PCInfoMapShadow pInfoMapShadow;
	PCInfoAnime pAnime;

	pDibTmp = GetDibTmp();
	sc = GetDrawScale(pDst);
	x *= sc;
	y *= sc;
	nSize = 16;
	nSizeDst = 32 * sc;
	if (bSingleSize) {
		nSizeDst = 16 * sc;
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
		if (pImg == NULL) {
#if defined(_DEBUG)
			LogMissingMapShadowAtlasOnce(m_pMgrGrpData, pInfoMapShadow, wGrpIDBase);
#endif
			goto Exit;
		}
		pDibTmp->BltFrom256(0, 0, nSize, nSize, pImg, ((wGrpIDBase % 1024) % 32) * nSize, ((wGrpIDBase % 1024) / 32) * nSize);
	} else {
		pImg = m_pMgrGrpData->GetDibMapShadow(wGrpIDBase / 1024);
		if (pImg == NULL) {
#if defined(_DEBUG)
			LogMissingMapShadowAtlasOnce(m_pMgrGrpData, pInfoMapShadow, wGrpIDBase);
#endif
			goto Exit;
		}
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


/// 一枚絵オブジェクトを描く。
/// 基準位置: 一枚絵の「下端中央」を、オブジェクト格子(m_sizeGrp マス)の下端中央に合わせ、
/// (m_nIllustOffX, m_nIllustOffY)[960描画px]を足す。(x,y)は DrawMapObject と同じ論理座標
/// (格子の左端 / 最下行の上端。m_ptPos は格子の左下マス)。等倍(sc==2 のワールド描画)のときだけ有効。
/// @return 描いたらTRUE(一枚絵なし・画像なし・sc!=2 はFALSEで従来描画へ)
BOOL CMgrDraw::DrawMapObjectIllust(CImg32 *pDst, int x, int y, PCInfoMapObject pInfo, BOOL bShadow)
{
	int sc, nCenterX, nBottom;
	PCImg32 pImg;

	if ((pInfo == NULL) || (pInfo->m_strIllustFile.IsEmpty())) {
		return FALSE;
	}
	sc = GetDrawScale(pDst);
	if (sc != 2) {
		return FALSE;
	}
	pImg = m_pMgrGrpData->GetDibIllustObject((LPCSTR)pInfo->m_strIllustFile, bShadow);
	if (pImg == NULL) {
		return FALSE;
	}
	nCenterX = x * sc + pInfo->m_sizeGrp.cx * 32 * sc / 2;
	nBottom  = y * sc + 32 * sc;
	pDst->BltSrcOver(
		nCenterX - pImg->Width() / 2 + pInfo->m_nIllustOffX,
		nBottom - pImg->Height() + pInfo->m_nIllustOffY,
		pImg->Width(), pImg->Height(), pImg, 0, 0);
	return TRUE;
}


/// デカール(BIT_MAPOBJECT_GROUND)の本体を描く。位置は一枚絵と同じ基準
void CMgrDraw::DrawMapObjectDecal(CImg32 *pDst, int x, int y, DWORD dwObjectID)
{
	PCInfoMapObject pInfo;

	pInfo = (PCInfoMapObject)m_pMgrData->GetLibInfoMapObject()->GetPtr(dwObjectID);
	if (pInfo == NULL) {
		return;
	}
	DrawMapObjectIllust(pDst, x, y, pInfo, FALSE);
}


void CMgrDraw::DrawMapObjectShadow(CImg32 *pDst, int x, int y, DWORD dwObjectID)
{
	PCInfoMapObject pInfo;

	pInfo = (PCInfoMapObject)m_pMgrData->GetLibInfoMapObject()->GetPtr(dwObjectID);
	if (pInfo == NULL) {
		return;
	}
	DrawMapObjectIllust(pDst, x, y, pInfo, TRUE);
}


void CMgrDraw::DrawMapObject(
	CImg32 *pDst,				// [in] 描画先
	int x,						// [in] 描画位置(X)
	int y,						// [in] 描画位置(Y)
	DWORD dwObjectID,			// [in] オブジェクトID
	BOOL bUseColorKey,/*TRUE*/	// [in] TRUE:重ね合わせする
	BOOL bLock)/*TRUE*/			// [in] テンポラリをロックする
{
	int xx, yy, sc, nSrcY;
	BOOL bCache, bComplete, bHasKey;
	unsigned long long ullKey;
	STScaledCache *pCache;
	SIZE sizeSrc, sizeDst;
	WORD wGrpID;
	PCImg32 pImg, pDibTmp, pSrcImg;
	PCInfoMapObject pInfo;
	PSTMAPOBJECTANIMEINFO pAnimeInfo;
	PCLibInfoMapObject pLibInfoMapObject;

	sc = GetDrawScale(pDst);
	x *= sc;
	y *= sc;
	pDibTmp = GetDibTmp();

	pLibInfoMapObject = m_pMgrData->GetLibInfoMapObject();
	pInfo = (PCInfoMapObject)pLibInfoMapObject->GetPtr(dwObjectID);
	if (pInfo == NULL) {
		goto Exit;
	}
	// 一枚絵モード: 画像があれば16pxパーツ格子の代わりにこれを描く(無ければ従来描画へ)
	if (DrawMapObjectIllust(pDst, x / sc, y / sc, pInfo, FALSE)) {
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
	sizeDst.cx *= 2 * sc;
	sizeDst.cy *= 2 * sc;

	// ワールド描画は拡大結果をキャッシュ(キーはオブジェクトIDと構成グラフィックIDのハッシュ。定義編集にも追従)
	bCache = (sc > 1) ? TRUE : FALSE;
	bHasKey = TRUE;
	bComplete = TRUE;
	pSrcImg = pDibTmp;
	nSrcY = sizeDst.cy;
	ullKey = 0;
	if (bCache) {
		ullKey = 1469598103934665603ULL ^ (unsigned long long)dwObjectID;
		ullKey = (ullKey * 1099511628211ULL) ^ (unsigned long long)sc;
		for (yy = 0; yy < pInfo->m_sizeGrp.cx * pInfo->m_sizeGrp.cy; yy++) {
			ullKey = (ullKey * 1099511628211ULL) ^ (unsigned long long)pAnimeInfo->pwGrpID[yy];
		}
		ullKey = (ullKey * 1099511628211ULL) ^ ((unsigned long long)pInfo->m_sizeGrp.cx << 8) ^ (unsigned long long)pInfo->m_sizeGrp.cy;
		pCache = FindScaledCache(ullKey);
		if (pCache != NULL) {
			pSrcImg = pCache->pImg;
			nSrcY = 0;
			bHasKey = pCache->bHasKey;
			goto Blit;
		}
	}

	for (yy = 0; yy < pInfo->m_sizeGrp.cy; yy++) {
		for (xx = 0; xx < pInfo->m_sizeGrp.cx; xx++) {
			wGrpID = pAnimeInfo->pwGrpID[yy * pInfo->m_sizeGrp.cx + xx];
			pImg = m_pMgrGrpData->GetDibMapParts(wGrpID / 1024);
			if (pImg == NULL) {
				bComplete = FALSE;
				continue;
			}
			pDibTmp->BltFrom256(xx * 16, yy * 16, 16, 16, pImg, ((wGrpID % 1024) % 32) * 16, ((wGrpID % 1024) / 32) * 16);
		}
	}

	pDibTmp->BltStretchNearest(0, sizeDst.cy, sizeDst.cx, sizeDst.cy, pDibTmp, 0, 0, sizeSrc.cx, sizeSrc.cy);

	if (bCache && bComplete) {
		pCache = StoreScaledCache(ullKey, pDibTmp, 0, sizeDst.cy, sizeDst.cx, sizeDst.cy);
		pSrcImg = pCache->pImg;
		nSrcY = 0;
		bHasKey = pCache->bHasKey;
	}

Blit:
	pDst->Blt(x, y - (sizeDst.cy - 32 * sc), sizeDst.cx, sizeDst.cy, pSrcImg, 0, nSrcY, (bUseColorKey && bHasKey) ? TRUE : FALSE);

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
	int sc, nSz;
	PCImg32 pImg, pDibTmp;

	sc = GetDrawScale(pDst);
	nSz = 32 * sc;
	x *= sc;
	y *= sc;
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

	pDibTmp->BltStretchNearest(0, nSz, nSz, nSz, pDibTmp, 0, 0, 16, 16);

	if (byLevel == 0) {
		pDst->Blt(x, y, nSz, nSz, pDibTmp, 0, nSz, TRUE);
	} else {
		pDst->BltAlpha(x, y, nSz, nSz, pDibTmp, 0, nSz, byLevel, TRUE);
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
	int cx, cy, sc, nSt;
	PCImg32 pImg, pDibTmp;
	POINT ptTmp;

	sc = GetDrawScale(pDst);
	nSt = 2 * sc;
	x *= sc;
	y *= sc;
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

	pDibTmp->BltStretchNearest(0, cy * 2, cx * nSt, cy * nSt, pDibTmp, 0, 0, cx, cy);

	pDst->Blt(x, y, cx * nSt, cy * nSt, pDibTmp, 0, cy * 2, TRUE);

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
	int cx, cy, i, nCount, sc, nDx, nDy, nW, nH, nRowH;
	PCImg32 pImg, pDibTmp;
	POINT ptSrc, ptTmp;

	sc = GetDrawScale(pDst);
	x *= sc;
	y *= sc;
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

	// 数字列(論理サイズ)を組み立て済み。ワールド描画なら sc 倍に拡大して転送する
	nRowH = cy * 2 + 20;
	nDx = x - (cx * nCount / 2) * sc;
	nDy = y - cy * sc;
	nW = cx * nCount * sc;
	nH = nRowH * sc;
	if (sc > 1) {
		pDibTmp->BltStretchNearest(0, nRowH, nW, nH, pDibTmp, 0, 0, cx * nCount, nRowH);
	}
	if (byLevel == 0) {
		pDst->Blt(nDx, nDy, nW, nH, pDibTmp, 0, (sc > 1) ? nRowH : 0, TRUE);
	} else {
		pDst->BltAlpha(nDx, nDy, nW, nH, pDibTmp, 0, (sc > 1) ? nRowH : 0, byLevel, TRUE);
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
	m_pDibWorld->Clear();
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

	cx = m_pDibWorld->Width();
	cy = m_pDibWorld->Height();
	ImgTmp.CreateWithoutGdi(WORLD_SCRSIZEX, WORLD_SCRSIZEY);
	ImgTmp.Blt(0, 0, WORLD_SCRSIZEX, WORLD_SCRSIZEY, m_pDibWorld, (cx - WORLD_SCRSIZEX) / 2, (cy - WORLD_SCRSIZEY) / 2);
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
