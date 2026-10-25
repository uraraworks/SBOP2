/// @file MgrDraw.h
/// @brief 描画マネージャクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006
/// @note Phase 3: Draw() を HDC → SDL_Renderer* に変更
///       CImg32 の内部描画ロジックはそのまま維持

#pragma once

#include <unordered_map>

// SDL_Renderer / SDL_Texture の前方宣言（SDL2 の typedef に合わせる）
// SboCliAdminMfc は SDL2 に依存しないため SDL.h はインクルード不可
// MgrDraw.cpp 側で SDL.h をインクルードして SDL API を使用する
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture  SDL_Texture;

class CImg32;
class CMgrData;
class CMgrGrpData;
class CMgrLayer;
class CMgrWindow;
class CInfoCharCli;
class CInfoItem;
class CInfoMapObject;
class CInfoMapParts;
class CLibInfoMapParts;
class CLibInfoMapShadow;
class CLibInfoItem;
class CInfoTextEffect;

// クラス宣言

typedef class CMgrDraw
{
public:
			CMgrDraw();								// コンストラクタ
	virtual ~CMgrDraw();							// デストラクタ

	void	Create(CMgrData *pMgrData);				// 作成
	void	Destroy(void);							// 破棄

	void	Draw(SDL_Renderer *pRenderer);			// 描画（SDL_RenderCopy経由）
	CImg32	*GetDibWorld(void);						// ワールド描画バッファ(DRAW_SCALE倍解像度)取得
	int		GetDrawScale(CImg32 *pDst);				// 描画先の倍率(ワールドバッファなら DRAW_SCALE、それ以外は1)
	void	BeginUiOverlay(void);					// ワールド描画完了通知。UIバッファをワールドの縮小で初期化し、以降のUI描画を重ね合わせ対象にする
	void	DrawChar(CImg32 *pDst, int x, int y, CInfoCharCli *pInfoChar, BOOL bLock = TRUE);	// キャラを描画
	void	DrawChar(CImg32 *pDst, int x, int y, BYTE byDirection, BYTE byAnimeNo, BYTE byLevel, CInfoCharCli *pInfoChar);	// キャラを描画

	void	DrawMapParts(CImg32 *pDst, int x, int y, DWORD dwPartsID, int nMode, BOOL bSingleSize = FALSE, BOOL bUseColorKey = TRUE, BYTE byLevel = 0);			// マップパーツを描画
	void	DrawMapParts(CImg32 *pDst, int x, int y, CInfoMapParts *pInfoMapParts, int nMode, BOOL bSingleSize = FALSE, BOOL bUseColorKey = TRUE, BOOL bLock = TRUE, BYTE byLevel = 0);		// マップパーツを描画
	void	DrawMapShadow(CImg32 *pDst, int x, int y, DWORD dwShadowID, BOOL bSingleSize = FALSE, BOOL bUseColorKey = TRUE, BOOL bLock = TRUE);	// マップ影を描画
	void	DrawMapObjectShadow(CImg32 *pDst, int x, int y, DWORD dwObjectID);	// 一枚絵オブジェクトの影を描画(影画像があるときだけ)
	BOOL	DrawMapObjectIllust(CImg32 *pDst, int x, int y, CInfoMapObject *pInfo, BOOL bShadow);	// 一枚絵(RGBA)を描画。描けたらTRUE
	void	DrawMapObject(CImg32 *pDst, int x, int y, DWORD dwObjectID, BOOL bUseColorKey = TRUE, BOOL bLock = TRUE);			// マップオブジェクトを描画
	void	DrawItem(CImg32 *pDst, int x, int y, DWORD dwItemID, BYTE byLevel = 0);			// アイテムを描画
	void	DrawItem(CImg32 *pDst, int x, int y, CInfoItem *pInfoItem, BYTE byLevel = 0, BOOL bLock = TRUE);	// アイテムを描画
	void	DrawBalloon(CImg32 *pDst, int x, int y, DWORD dwBalloonID);						// 噴出しを描画
	void	DrawTextEffect(CImg32 *pDst, int x, int y, CInfoTextEffect *pInfoTextEffect);	// 文字エフェクトを描画
	void	DrawMapHitMark(CImg32 *pDst, int x, int y, int nNo);							// マップ当たり判定マークを描画
	void	DrawCursor(CImg32 *pDst, int x, int y, int nType);								// 指カーソルを描画
	void	DrawSkillMenu(CImg32 *pDst, int x, int y);										// スキルメニューを描画
	void	DrawSkillType(CImg32 *pDst, int x, int y, int nType, BOOL bActive);				// スキル種別タブを描画
	void	DrawIcon(CImg32 *pDst, int x, int y, int nIndex);								// アイコンを描画

	BOOL	TimerProc(void);						// 時間処理
	BOOL	IsFade(void);							// フェードイン/アウト中か判定
	void	SetLevel(BYTE byLevel);					// 明度設定
	void	SetFadeState(BYTE byFadeState);			// フェード状態設定
	void	Clear(void);							// 背景クリア
	void	SaveScreenShot(void);					// スクリーンショットの保存
	void	LockDibTmp(void);						// 描画テンポラリをロック
	void	UnLockDibTmp(void);						// 描画テンポラリのロックを解除


private:
	BOOL	Fade(void);								// フェード処理
	CImg32	*GetDibTmp(void);						// 描画テンポラリ取得
	void	ReleaseDibTmp(void);					// 描画テンポラリ解放


private:
	BYTE				m_byLevel;					// 明度
	BOOL				m_bWorldDrawn;				// このフレームでワールドを描いたか
	BYTE				m_byFadeState;				// フェード状態
	DWORD				m_dwLastFade;				// 前回のフェード処理

	CmySection			m_CritDataLock;				// データロック用のクリティカルセクション
	// 描画先の座標規約: DrawXxx(pDst, x, y, ...) の x,y は常に「論理(UI)座標」(余白32px込み)で渡す。
	// pDst がワールドバッファのとき DrawXxx 内部で DRAW_SCALE 倍して描く(サイズも倍率込みで拡大する)。
	// 拡大済みタイル/オブジェクト画像のキャッシュ(ワールド描画のみ)。
	// 16px素材を毎フレーム 作業域へ展開→拡大→転送 するのは4倍解像度では重いので、拡大結果を使い回す。
	struct STScaledCache {
		CImg32	*pImg;			// 拡大済み画像(カラーキーはマゼンタ)
		BOOL	bHasKey;		// カラーキー画素を含むか(含まなければキー判定なしで高速転送できる)
	};
	STScaledCache	*FindScaledCache(unsigned long long ullKey);							// キャッシュ検索
	STScaledCache	*StoreScaledCache(unsigned long long ullKey, CImg32 *pSrc, int sx, int sy, int cx, int cy);	// 作業域の矩形を複製して登録
	void	ClearScaledCache(void);					// キャッシュ全破棄
	std::unordered_map<unsigned long long, STScaledCache>	m_mapScaledCache;
	size_t	m_nScaledCacheBytes;
	void	UpscaleUiToWorld(void);					// UIバッファ全体をワールドへ最近傍で拡大(ワールド非描画時)
	void	CompositeUiOverlay(void);				// ワールド上へUIバッファの変更分だけを拡大して重ねる

private:
	CImg32				*m_pDibWorld,				// ワールド描画バッファ(最終出力)
						*m_pDibBack,				// UI用バッファ(論理解像度)
						*m_pDibTmp;					// 描画用テンポラリ
	SDL_Texture			*m_pBackTexture;			// バックバッファ用SDLテクスチャ（Lazy初期化）
	CMgrData			*m_pMgrData;				// データマネージャ
	CMgrGrpData			*m_pMgrGrpData;				// グラフィックデータマネージャ
	CMgrLayer			*m_pMgrLayer;				// レイヤーマネージャ
	CMgrWindow			*m_pMgrWindow;				// ウィンドウマネージャ
	CLibInfoMapParts	*m_pLibInfoMapParts;		// マップパーツ情報
	CLibInfoMapShadow	*m_pLibInfoMapShadow;		// マップ影情報
	CLibInfoItem		*m_pLibInfoItem;			// アイテム情報
	CLibInfoMotionType	*m_pLibInfoMotionType;		// モーション種別情報ライブラリ
} CMgrDraw, *PCMgrDraw;
