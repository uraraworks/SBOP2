/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名：	Img32.h														 */
/* 内容：		32bitイメージクラス 定義ファイル							 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2005/01/24													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CImg32
{
public:
			CImg32(void);										/* コンストラクタ */
	virtual ~CImg32();											/* デストラクタ */

	BOOL	Create				(int cx, int cy);										/* ビットマップを作成 */
	BOOL	CreateWithoutGdi	(int cx, int cy, LPBYTE pBits = NULL);					/* GDI非互換で作成 */
	BOOL	CreateWithoutGdi256	(int cx, int cy);										/* GDI非互換で作成(256色) */
	void	Destroy				(void);													/* 破棄 */

	void	Clear				(void);													/* クリア */
	void	FillRect			(int x, int y, int cx, int cy, COLORREF clFill);		/* 矩形塗りつぶし */
	void	XorRect				(int x, int y, int cx, int cy);							/* 矩形XOR */
	void	Rectangle			(int x, int y, int cx, int cy, COLORREF clDraw);		/* 矩形 */
	void	Circle				(int x, int y, int r, COLORREF clDraw);					/* 円 */
	void	CircleGradation		(int x, int y, int r, int rr, COLORREF clDraw);			/* 円(グラデーション) */
	void	ChgLevel			(int x, int y, int cx, int cy, BYTE byLevel);			/* 明度を変更 */
	void	ChgColor			(int x, int y, int cx, int cy, COLORREF clChg);			/* 色素を変更 */
	void	SetLevel			(CImg32 *pSrc);											/* 指定画像のR値を明度として反映 */

	void	Blt					(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BOOL bColorKey = FALSE);						/* 矩形転送 */
	void	BltLevel			(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BYTE byLevel, BOOL bColorKey = FALSE);		/* 明度を変えながら矩形転送 */
	void	BltAlpha			(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BYTE byLevel, BOOL bColorKey = FALSE);		/* 混ぜ合わせ矩形転送 */
	void	BltAlpha2			(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BYTE byLevelAlpha, BOOL bColorKey = FALSE);	/* カラーキーのみ混ぜ合わせ矩形転送 */
	void	BltPlus				(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, int nMax, BOOL bColorKey = FALSE);			/* 矩形加算転送 */
	void	BltTurnRightAndLeft	(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BOOL bColorKey = FALSE);						/* 左右反転して矩形転送 */
	void	BltTurnUpAndDown(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BOOL bColorKey = FALSE);							/* 上下反転して矩形転送 */	

	HDC		Lock				(void);									/* 画像をロック */
	void	Unlock				(void);									/* ロック解除 */

	HBITMAP	GetSafeHandle		(void);									/* ビットマップハンドルを取得 */
	int		Width				(void);									/* 幅を取得 */
	int		Height				(void);									/* 高さを取得 */
	int		GetImageSize		(void);									/* データサイズを取得 */
	LPBYTE	GetBits				(void);									/* ビットデータのアドレスを取得 */
	void	SetColorKey			(COLORREF clKey);						/* カラーキーを設定 */
	COLORREF GetColorKey		(void);									/* カラーキーを取得 */

	/* 256色画像用 */
	BOOL	IsMode256			(void);											/* 256色画像か判定 */
	void	SetColorKeyNo		(BYTE byColorKeyNo);							/* カラーキーを設定 */
	BYTE	GetColorKeyNo		(void);											/* カラーキーを取得 */
	void	SetPallet			(BYTE byNo, BYTE r, BYTE g, BYTE b);			/* パレットを設定 */
	PBYTE	GetPallet			(BYTE byNo);									/* パレットを取得 */
	void	FillRect256			(int x, int y, int cx, int cy, BYTE byPallet);	/* 矩形塗りつぶし */
	void	BltFrom256			(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BOOL bColorKey = FALSE);
	void	BltAlphaFrom256		(int dx, int dy, int cx, int cy, CImg32 *pSrc, int sx, int sy, BYTE byLevel, BOOL bColorKey = FALSE);		/* 混ぜ合わせ矩形転送 */


protected:
	void	InitData			(void);									/* データを初期化 */
	DWORD	ColorrefToDword		(COLORREF cl);							/* 色素をDWORD値に変換 */
	BYTE	PercentToHex		(BYTE byPercent);						/* パーセントを255MAXの16進数に変換 */
	void	ClipPos				(int &dx, int &dy, int &cx, int &cy, CImg32 *pSrc, int &sx, int &sy);	/* 座標をクリッピング */


protected:
	HBITMAP			m_hBmp;								/* ビットマップハンドル */
	HBITMAP			m_hBmpBack;							/* ビットマップハンドル保存用 */
	BITMAPINFO		m_bmpInfo;							/* ビットマップ情報 */
	LPBYTE			m_pBits;							/* ビットデータ */
	HDC				m_hDC;								/* デバイスコンテキストハンドル */
	DWORD			m_dwColorKey;						/* カラーキー */

	BYTE			m_byColorKey;						/* カラーキー番号 */
	PBYTE			m_pPallet;							/* パレット */
} CImg32, *PCImg32;

/* Copyright(C)URARA-works 2005 */
