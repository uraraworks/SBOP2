/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:WindowBase.h												 */
/* 内容			:ウィンドウ基底クラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/03													 */
/* ========================================================================= */

#pragma once
#include <vector>

class CImg32;
class CWindowBase;
class CMgrData;
class CMgrDraw;
class CMgrSound;
class CMgrGrpData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowBase
{
public:
			CWindowBase();										/* コンストラクタ */
	virtual ~CWindowBase();										/* デストラクタ */

	virtual void	Create		(CMgrData *pMgrData);					/* 作成 */
	virtual void	OnWindowMsg	(int nType, DWORD dwPara);				/* メッセージハンドラ(WM_WINDOWMSG) */
	virtual void	Update		(void);									/* 更新 */
	virtual void	Draw		(CImg32 *pDst);							/* 描画 */
	virtual BOOL	TimerProc	(void);									/* 時間処理 */
	virtual void	KeyProc		(BYTE byCode, BOOL bDown);				/* キー処理 */
			BOOL	IsDelete	(void) { return m_bDelete;	}			/* 削除判定 */
			BOOL	IsInput		(void) { return m_bInput;	}			/* キー入力を行うか判定 */
			BOOL	IsActive	(void) { return m_bActive;	}			/* アクティブか判定 */
			BOOL	IsShow		(void) { return m_bShow;	}			/* 表示するか判定 */
			int		GetID		(void) { return m_nID;		}			/* ウィンドウIDを取得 */
	virtual void	SetShow		(BOOL bShow);							/* 表示するか設定 */
	virtual void	SetActive	(BOOL bActive) { m_bActive = bActive; }	/* アクティブか設定 */
	virtual void	SetInput	(BOOL bInput) { m_bInput = bInput; }	/* キー入力を行うか設定 */
			void	Redraw		(void);									/* 再描画 */
			void	SetPos		(int nPos);								/* 位置の設定 */
			void	AddChild	(CWindowBase *pChild);					/* 子ウィンドウを追加 */
			void	DeleteChild	(CWindowBase *pChild);					/* 子ウィンドウを削除 */


protected:
	virtual BOOL OnUp		(void);										/* キーハンドラ(↑) */
	virtual BOOL OnDown		(void);										/* キーハンドラ(↓) */
	virtual BOOL OnLeft		(void);										/* キーハンドラ(←) */
	virtual BOOL OnRight	(void);										/* キーハンドラ(→) */
	virtual BOOL OnB		(BOOL bDown);								/* キーハンドラ(B) */
	virtual BOOL OnF		(BOOL bDown);								/* キーハンドラ(F) */
	virtual BOOL OnI		(BOOL bDown);								/* キーハンドラ(I) */
	virtual BOOL OnJ		(BOOL bDown);								/* キーハンドラ(J) */
	virtual BOOL OnK		(BOOL bDown);								/* キーハンドラ(K) */
	virtual BOOL OnL		(BOOL bDown);								/* キーハンドラ(L) */
	virtual BOOL OnS		(BOOL bDown);								/* キーハンドラ(S) */
	virtual BOOL OnX		(BOOL bDown);								/* キーハンドラ(X) */
	virtual BOOL OnZ		(BOOL bDown);								/* キーハンドラ(Z) */
	virtual BOOL OnEscape	(BOOL bDown);								/* キーハンドラ(Escape) */
	virtual BOOL OnSpace	(BOOL bDown);								/* キーハンドラ(Space) */
	void TextOut2			(HDC hDC, int x, int y, LPCSTR pStr, COLORREF Color, BOOL bDraw = FALSE, COLORREF ColorFrame = RGB(10, 10, 10));	/* 黒縁取りで文字描画 */
	void TextOut3			(HDC hDC, int x, int y, int cx, int cy, LPCSTR pStr, COLORREF Color);		/* 黒縁取りで文字描画(右詰) */
	void TextOut4			(HDC hDC, int x, int y, LPCSTR pStr, COLORREF ColorFrame, COLORREF Color = RGB (255, 255, 255));	/* 黒縁取りで文字描画 */
	void DrawFrame			(int nType = 0);							/* フレームを描画 */
	void DrawFrame			(int x, int y, int cx, int cy, int nType, BOOL bRightErace=FALSE);	/* フレームを描画 */
	void DrawFrame2			(int x, int y, int cx, int cy, int nType, CImg32 *pDst=NULL, int nBottom=0);	/* フレームを描画 */
	void DrawFrame3			(int x, int y, int cx, int cy, int nType);	/* フレームを描画 */
	void DrawCursor			(int x, int y);								/* カーソルを描画 */
	void DrawInputFrame1	(int x, int y, int cx, int cy, int nType);	/* 入力欄用フレームを描画1 */
	void DrawIconFrame		(int x, int y);								/* アイコン用フレームを描画 */


public:
	HWND			m_hWndMain;						/* メインウィンドウハンドル */
	DWORD			m_dwLastTimerProc,				/* 前回の時間処理時間 */
					m_dwLastTimeCursor,				/* 前回のカーソル処理時間 */
					m_dwLastTimeKey;				/* 前回のキー処理時間 */
	BOOL			m_bShow,						/* 表示する */
					m_bDelete,						/* 削除フラグ */
					m_bInput,						/* キー入力を行う */
					m_bActive;						/* アクティブなウィンドウ */
	int				m_nCursorAnime,					/* カーソルアニメーションコマ番号 */
					m_nCountKeyRepeate;				/* キーリピート回数 */

	int				m_nID;							/* ウィンドウID */
	POINT			m_ptViewPos;					/* 表示座標 */
	SIZE			m_sizeWindow;					/* ウィンドウサイズ */
	int				m_nPos,							/* 位置 */
					m_nPosMax;						/* 位置の最大値 */
	DWORD			m_dwTimeDrawStart;				/* 描画した時間 */
	HFONT			m_hFont,						/* 描画に使うフォント */
					m_hFont12,						/* 描画に使うフォント(12ドット) */
					m_hFont12Bold,					/* 描画に使うフォント(12ドット) */
					m_hFont14,						/* 描画に使うフォント(14ドット) */
					m_hFont16,						/* 描画に使うフォント(16ドット) */
					m_hFont16Normal;				/* 描画に使うフォント(16ドット) */

	CWindowBase		*m_pParent;						/* 親ウィンドウ */
	CImg32			*m_pDib,						/* ビットマップ */
					*m_pDibSystem;					/* システム画像 */
	CMgrData		*m_pMgrData;					/* データ管理 */
	CMgrDraw		*m_pMgrDraw;					/* 描画管理 */
	CMgrSound		*m_pMgrSound;					/* サウンド管理 */
	CMgrGrpData		*m_pMgrGrpData;					/* グラフィックデータ管理 */
	std::vector<CWindowBase *>	m_apChild;	/* 子ウィンドウ */
} CWindowBase, *PCWindowBase;
typedef std::vector<PCWindowBase>	  ARRAYWINDOWBASE;
typedef std::vector<PCWindowBase>	*PARRAYWINDOWBASE;

/* Copyright(C)URARA-works 2006 */
