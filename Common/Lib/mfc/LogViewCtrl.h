#ifndef	__LOGVIEWCTRL_H__
#define	__LOGVIEWCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 表示設定
struct LOGVIEWCTRLSETTING {
	TCHAR		szFontText[LF_FACESIZE];	// フォント名(本文)
	TCHAR		szFontHeader[LF_FACESIZE];	// フォント名(ヘッダ)
	int			nFontPoint;					// フォントサイズ(ポイント)
	COLORREF	crText;						// テキスト文字色
	COLORREF	crBack;						// テキスト背景色
	COLORREF	crSelectText;				// 選択文字色
	COLORREF	crSelectBack;				// 選択背景色
	COLORREF	crLinkText;					// リンク文字色
	COLORREF	crLinkBack;					// リンク背景色
	BOOL		bCancelSelectAfterCopy;		// コピー処理後、選択解除するか
	int			nWheelDelta;				// マウスホイールでの移動量
	int			nLinkNotifyType;			// リンク通知種別
};

// 行情報
struct LINEINFO {
	SYSTEMTIME	stDateTime;					// 日時
	CString		strText;					// 行テキスト
	COLORREF	crFore;						// 前景色
	COLORREF	crBack;						// 背景色
	struct TOKEN {
		int		nStart;						// 単語開始位置
		int		nLen;						// 単語の長さ(バイト)
		int		nType;						// 単語の種類
	};
	CArray<TOKEN, TOKEN&>	aTokens;		// 単語情報

	void operator=(const LINEINFO& li) {
		stDateTime = li.stDateTime;
		strText = li.strText;
		crFore = li.crFore;
		crBack = li.crBack;
		aTokens = li.aTokens;
	}
};

// 通知情報
struct NMLVCLINK {
	NMHDR	hdr;
	LPCTSTR	pszLink;
};

// テキスト保持限界
#define	LVC_TEXT_LIMIT_INFINITE			0	// 無制限(メモリ依存)
#define	LVC_TEXT_LIMIT_LINE				1	// 行数制限
#define	LVC_TEXT_LIMIT_SIZE				2	// サイズ制限

// リンク通知種別
#define	LVC_LINK_NOTIFY_NONE			0	// リンク通知なし
#define	LVC_LINK_NOTIFY_DOUBLE_CLICK	1	// ダブルクリックで通知
#define	LVC_LINK_NOTIFY_TRIPLE_CLICK	2	// トリプルクリックで通知

// 通知コード
#define	LVCN_LINK						100	// リンク通知コード

/////////////////////////////////////////////////////////////////////////////
// CLogViewCtrl ウィンドウ

class CLogViewCtrl : public CWnd
{
public:
	CLogViewCtrl();
	virtual ~CLogViewCtrl();

	DECLARE_DYNCREATE(CLogViewCtrl)

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CLogViewCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:

	BOOL	AddLine(LPCTSTR pszLine, COLORREF crFore = 0xffffffff, COLORREF crBack = 0xffffffff, BOOL bRedraw = TRUE);
	void	Redraw();
	void	Clear();
	void	SetSetting(const LOGVIEWCTRLSETTING* pSetting);
	void	GetSetting(LOGVIEWCTRLSETTING* pSetting);
	void	SetTextLimit(int nLimit, DWORD dwSize);

protected:
	CArray<LINEINFO, LINEINFO&>	m_aTexts;	// 行情報
	int		m_nLimitMode;					// テキスト限界
	DWORD	m_dwLimitSize;					// テキスト限界値
	DWORD	m_dwTextSize;					// 現在のテキストのサイズ
	CSize	m_sizeChar;						// 半角1文字の幅と高さ
	CSize	m_sizeView;						// 表示できるサイズ
	CSize	m_sizeHeader;					// ヘッダの表示サイズ
	CFont	m_fontText;						// フォント
	CFont	m_fontHeader;					// フォント
	int		m_nSelect;						// 選択状態(0:選択なし/1:文字選択/2:行選択)
	int		m_nSelectCharStart;				// 文字選択開始位置
	int		m_nSelectCharEnd;				// 文字選択終了位置
	int		m_nSelectLineStart;				// 行選択開始位置
	int		m_nSelectLineEnd;				// 行選択終了位置
	CMenu	m_menuPopup;					// コンテキストメニュー
	DWORD	m_dwTripleClick;				// トリプルクリック判定用
	CPoint	m_ptTripleClick;				// トリプルクリック判定用

	LOGVIEWCTRLSETTING	m_stSetting;		// 設定

	void	RecalcParam(const SIZE* psizeClient = NULL);
	int		GetTextHeight(CDC* pDC, int nIndex);
	void	LimitText();
	BOOL	GetSelectPos(CPoint point, int* pnLine, int* pnChar);
	void	UpdateSelectParam(int nLine, int nChar);
	int		FindToken(int nLine, int nChar);

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CLogViewCtrl)
	afx_msg HRESULT OnNcHitTest(CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	afx_msg void OnMenuCopy();
	afx_msg void OnMenuAllSelect();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif
