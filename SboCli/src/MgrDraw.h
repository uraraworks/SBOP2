/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MgrDraw.h													 */
/* 内容			:描画マネージャクラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
/* ========================================================================= */

#pragma once

class CImg32;
class CMgrData;
class CMgrGrpData;
class CMgrLayer;
class CMgrWindow;
class CInfoCharCli;
class CInfoItem;
class CInfoMapParts;
class CLibInfoMapParts;
class CLibInfoMapShadow;
class CLibInfoItem;
class CInfoTextEffect;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CMgrDraw
{
public:
			CMgrDraw();									/* コンストラクタ */
	virtual ~CMgrDraw();								/* デストラクタ */

	void	Create			(CMgrData *pMgrData);				/* 作成 */
	void	Destroy			(void);								/* 破棄 */

	void	Draw			(HDC hDC);							/* 描画 */
	void	DrawChar		(CImg32 *pDst, int x, int y, CInfoCharCli *pInfoChar, BOOL bLock = TRUE);	/* キャラを描画 */
	void	DrawChar		(CImg32 *pDst, int x, int y, BYTE byDirection, BYTE byAnimeNo, BYTE byLevel, CInfoCharCli *pInfoChar);	/* キャラを描画 */

	void	DrawMapParts	(CImg32 *pDst, int x, int y, DWORD dwPartsID, int nMode, BOOL bSingleSize = FALSE, BOOL bUseColorKey = TRUE, BYTE byLevel = 0);			/* マップパーツを描画 */
	void	DrawMapParts	(CImg32 *pDst, int x, int y, CInfoMapParts *pInfoMapParts, int nMode, BOOL bSingleSize = FALSE, BOOL bUseColorKey = TRUE, BOOL bLock = TRUE, BYTE byLevel = 0);		/* マップパーツを描画 */
	void	DrawMapShadow	(CImg32 *pDst, int x, int y, DWORD dwShadowID, BOOL bSingleSize = FALSE, BOOL bUseColorKey = TRUE, BOOL bLock = TRUE);	/* マップ影を描画 */
	void	DrawMapObject	(CImg32 *pDst, int x, int y, DWORD dwObjectID, BOOL bUseColorKey = TRUE, BOOL bLock = TRUE);			/* マップオブジェクトを描画 */
	void	DrawItem		(CImg32 *pDst, int x, int y, DWORD dwItemID, BYTE byLevel = 0);			/* アイテムを描画 */
	void	DrawItem		(CImg32 *pDst, int x, int y, CInfoItem *pInfoItem, BYTE byLevel = 0, BOOL bLock = TRUE);	/* アイテムを描画 */
	void	DrawBalloon		(CImg32 *pDst, int x, int y, DWORD dwBalloonID);						/* 噴出しを描画 */
	void	DrawTextEffect	(CImg32 *pDst, int x, int y, CInfoTextEffect *pInfoTextEffect);			/* 文字エフェクトを描画 */
	void	DrawMapHitMark	(CImg32 *pDst, int x, int y, int nNo);									/* マップ当たり判定マークを描画 */
	void	DrawCursor		(CImg32 *pDst, int x, int y, int nType);								/* 指カーソルを描画 */

	BOOL	TimerProc		(void);								/* 時間処理 */
	BOOL	IsFade			(void);								/* フェードイン/アウト中か判定 */
	void	SetLevel		(BYTE byLevel);						/* 明度設定 */
	void	SetFadeState	(BYTE byFadeState);					/* フェード状態設定 */
	void	Clear			(void);								/* 背景クリア */
	void	SaveScreenShot	(void);								/* スクリーンショットの保存 */
	void	LockDibTmp		(void);								/* 描画テンポラリをロック */
	void	UnLockDibTmp	(void);								/* 描画テンポラリのロックを解除 */


private:
	BOOL	Fade			(void);						/* フェード処理 */
	CImg32	*GetDibTmp		(void);						/* 描画テンポラリ取得 */
	void	ReleaseDibTmp	(void);						/* 描画テンポラリ解放 */


private:
	BYTE				m_byLevel;						/* 明度 */
	BYTE				m_byFadeState;					/* フェード状態 */
	DWORD				m_dwLastFade;					/* 前回のフェード処理 */

	CmySection			m_CritDataLock;					/* データロック用のクリティカルセクション */
	CImg32				*m_pDibBack,					/* バックバッファ */
						*m_pDibTmp;						/* 描画用テンポラリ */
	CMgrData			*m_pMgrData;					/* データマネージャ */
	CMgrGrpData			*m_pMgrGrpData;					/* グラフィックデータマネージャ */
	CMgrLayer			*m_pMgrLayer;					/* レイヤーマネージャ */
	CMgrWindow			*m_pMgrWindow;					/* ウィンドウマネージャ */
	CLibInfoMapParts	*m_pLibInfoMapParts;			/* マップパーツ情報 */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;			/* マップ影情報 */
	CLibInfoItem		*m_pLibInfoItem;				/* アイテム情報 */
	CLibInfoMotionType	*m_pLibInfoMotionType;			/* モーション種別情報ライブラリ */
} CMgrDraw, *PCMgrDraw;

/* Copyright(C)URARA-works 2006 */
