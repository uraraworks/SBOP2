/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:LayerMap.h													 */
/* 内容			:レイヤー描画クラス(マップ) 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
/* ========================================================================= */

#pragma once

#include "LayerCloud.h"

class CInfoCharCli;
class CLibInfoItem;
class CLibInfoMapParts;
class CLibInfoMapShadow;
class CLayerCloud;
class CLayerMisty;
class CLayerSnow;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLayerMap : public CLayerBase
{
public:
			CLayerMap();											/* コンストラクタ */
	virtual ~CLayerMap();											/* デストラクタ */

	void	Create				(CMgrData *pMgrData);						/* 作成 */
	void	Draw				(CImg32 *pDst);								/* 描画 */
	BOOL	TimerProc			(void);										/* 時間処理 */
	int		IsScrollPos			(int x, int y, int nDirection);				/* スクロールする場所か判定 */
	BOOL	IsScrollArea		(int x, int y, int &nDirection);			/* スクロールできるか判定 */
	BOOL	IsScroll			(void);										/* スクロール中か判定 */
	BOOL	IsInScreen			(int x, int y);								/* 指定座標が画面内にいるかチェック */
	BYTE	Scroll				(BYTE byDirection, BOOL bNoCheck = FALSE);	/* スクロール */
	void	SetCenterPos		(int x, int y);								/* 指定座標が画面中央になるように設定 */
	void	SetScrollWait		(int nMoveWait);							/* スクロール移動待ち時間を設定 */
	void	SetScrollMode		(BOOL bScroll, int nViewIcon = -1);			/* スクロールするか設定 */
	void	SetSystemIconMode	(int nMode);								/* システムアイコンモードを設定 */
	void	GetViewMapPos		(int &nDstX, int &nDstY);					/* 視点をマップ座標で取得 */
	void	RenewLevel			(void);										/* 明度レベル画像を更新 */
	void	RenewMapName		(LPCSTR pszMapName);						/* マップ名表示用画像を更新 */


private:
	BOOL	TimerProcScroll		(void);											/* 時間処理(スクロール) */
	BOOL	TimerProcSystemIcon	(void);											/* 時間処理(システムアイコン) */
	BOOL	TimerProcMapName	(void);											/* 時間処理(マップ名表示) */
	void	DrawPartsBase		(CImg32 *pDst, int nDrawY = -99);				/* 描画(土台) */
	void	DrawMapPile			(CImg32 *pDst, int nDrawY = -99);				/* 描画(マップ重ね合わせ) */
	void	GetDrawMapPos		(POINT *ptPos, int &nDstX, int &nDstY);			/* マップ座標の描画位置を所得 */
	void	GetDrawMovePos		(POINT &ptMove, POINT &ptPos);					/* スクロール中の描画位置を所得 */
	void	DrawPartsPile		(CImg32 *pDst, int nDrawY = -99);				/* 描画(重ね合わせ) */
	void	DrawShadow			(CImg32 *pDst, int nDrawY = -99);				/* 描画(マップ影) */
	void	DrawMapObject		(CImg32 *pDst, int nDrawY = -99);				/* 描画(マップオブジェクト) */
	void	DrawItem			(CImg32 *pDst, int nType, int nDrawY = -99);	/* 描画(アイテム) */
	void	GetDrawPos			(CInfoCharCli *pChar, int &nDstX, int &nDstY);	/* 描画位置を取得 */
	void	DrawChar			(CImg32 *pDst, int nDrawY = -99);				/* 描画(キャラ) */
	void	DrawCharText		(CImg32 *pDst, int nDrawY = -99);				/* 描画(キャラ関連のテキスト) */
	void	DrawSystemIcon		(CImg32 *pDst);									/* 描画(システムアイコン) */
	void	DrawGauge			(CImg32 *pDst);									/* 描画(ゲージ類) */
	void	DrawMapName			(CImg32 *pDst);									/* 描画(マップ名) */


public:
	HFONT		m_hFont32;					/* 描画に使うフォント(32ドット) */
	BOOL		m_bScroll;					/* スクロールする判定 */
	BYTE		m_byDirection;				/* スクロール向き */
	int			m_nViewIcon,				/* 視点アイコン */
				m_nMoveWait,				/* スクロール移動待ち時間 */
				m_nViewX,					/* 視点(横) */
				m_nViewY,					/* 視点(縦) */
				m_nMoveX,					/* スクロール中(横) */
				m_nMoveY,					/* スクロール中(縦) */
				m_nSystemIconMode,			/* システムアイコン表示モード */
				m_nSyatemIconOffset,		/* システムアイコン表示オフセット */
				m_nLevelMapName;			/* マップ名表示用透過レベル */
	DWORD		m_dwLastTimeScroll,			/* 前回のスクロール処理時間 */
				m_dwLastTimeSystemIconMode,	/* 前回のシステムアイコン表示モード処理時間 */
				m_dwLastTimeMapName,		/* 前回のマップ名表示処理時間 */
				m_dwMoveWaitOnce;			/* 一度だけの移動待ち時間 */

	CImg32		*m_pDibLevel,					/* 明度レベル画像 */
				*m_pDibLevelTmp,				/* 灯り描画用テンポラリ */
				*m_pDibMapName;					/* マップ名表示用画像 */
	CLibInfoItem		*m_pLibInfoItem;		/* アイテム情報ライブラリ */
	CLibInfoMapParts	*m_pLibInfoMapParts;	/* マップパーツ情報 */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;	/* マップ影情報 */
	CLayerCloud			*m_pLayerCould;			/* 雲レイヤー */
	CLayerMisty			*m_pLayerMisty;			/* 霧レイヤー */
	CLayerSnow			*m_pLayerSnow;			/* 雪レイヤー */
} CLayerMap, *PCLayerMap;

/* Copyright(C)URARA-works 2006 */
