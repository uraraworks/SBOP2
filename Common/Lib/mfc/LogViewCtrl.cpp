#include "stdafx.h"
#include "LogViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CLASS_NAME			_T("LogViewCtrl")

#define	HEADER_COLUMN		11		// ヘッダ表示分の文字数

// 選択方法
#define	SELECT_TYPE_NONE	0		// 未選択
#define	SELECT_TYPE_CHAR	1		// 文字選択
#define	SELECT_TYPE_LINE	2		// 行選択

// トークン種別
#define	TOKEN_TYPE_NONE		0		// 不明
#define	TOKEN_TYPE_ALNUM	1		// 半角英数
#define	TOKEN_TYPE_ZENKAKU	2		// 全角
#define	TOKEN_TYPE_PUNCT	3		// 区切り
#define	TOKEN_TYPE_LINK		4		// リンク

// メニューコマンドID
#define	ID_MENU_COPY		100		// 「コピー」コマンド
#define	ID_MENU_ALLSELECT	101		// 「すべて選択」コマンド

class CCurrentDC : public CDC
{
public:
	CCurrentDC(CWnd* pWnd)
	{
		m_pWnd = pWnd;
		Attach(GetDC(pWnd->GetSafeHwnd()));
	}
	~CCurrentDC()
	{
		ReleaseDC(m_pWnd->GetSafeHwnd(), Detach());
	}
protected:
	CWnd*	m_pWnd;
};

class CCurrentGdiObject
{
public:
	CCurrentGdiObject(CDC* pDC, CGdiObject* pObject)
	{
		m_pDC = pDC;
		m_pPrev = pDC->SelectObject(pObject);
	}
	~CCurrentGdiObject()
	{
		m_pDC->SelectObject(m_pPrev);
	}
protected:
	CDC*	m_pDC;
	CGdiObject*	m_pPrev;
};

/////////////////////////////////////////////////////////////////////////////
// CLogViewCtrl

IMPLEMENT_DYNCREATE(CLogViewCtrl, CWnd)

CLogViewCtrl::CLogViewCtrl()
{
	// ユーザが変更可能な設定の初期化
	_tcscpy(m_stSetting.szFontText, _T("ＭＳ ゴシック"));
	_tcscpy(m_stSetting.szFontHeader, m_stSetting.szFontText);
	m_stSetting.nFontPoint = 100;
	m_stSetting.crText = GetSysColor(COLOR_WINDOWTEXT);
	m_stSetting.crBack = GetSysColor(COLOR_WINDOW);
	m_stSetting.crSelectText = m_stSetting.crBack;
	m_stSetting.crSelectBack = m_stSetting.crText;
	m_stSetting.crLinkText = GetSysColor(COLOR_HOTLIGHT);
	m_stSetting.crLinkBack = m_stSetting.crBack;
	m_stSetting.bCancelSelectAfterCopy = FALSE;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &m_stSetting.nWheelDelta, 0);
	m_stSetting.nLinkNotifyType = LVC_LINK_NOTIFY_NONE;

	// テキスト保持限界の初期化
	m_nLimitMode = LVC_TEXT_LIMIT_INFINITE;
	m_dwLimitSize = 0;
	m_dwTextSize = 0;
	// 色々初期化
	m_nSelect = SELECT_TYPE_NONE;
	m_dwTripleClick = 0;

	// ウィンドウクラスの登録
	WNDCLASS	wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = ::DefWindowProc;
	wc.hInstance = AfxGetInstanceHandle();
	wc.lpszClassName = CLASS_NAME;
	AfxRegisterClass(&wc);
}

CLogViewCtrl::~CLogViewCtrl()
{
}

// 行の追加
BOOL CLogViewCtrl::AddLine(LPCTSTR pszLine, COLORREF crFore, COLORREF crBack, BOOL bRedraw)
{
	LINEINFO	li;
	GetLocalTime(&li.stDateTime);
	li.strText = pszLine;
	li.crFore = (crFore == 0xffffffff) ? m_stSetting.crText : crFore;
	li.crBack = (crBack == 0xffffffff) ? m_stSetting.crBack : crBack;

	int	nType = TOKEN_TYPE_NONE;
	LPCTSTR	pszPrev = pszLine;
	for (LPCTSTR psz = pszLine; ; psz = CharNext(psz)) {
		int	nPrevType = nType;
		if (*psz != _T('\0')) {
			if ((nType == TOKEN_TYPE_LINK) && (_istprint(*psz) != 0) && (*psz != _T(' '))) {
				// リンク続行中
			} else if (_istalnum(*psz) != 0) {
				// 半角英数
				nType = TOKEN_TYPE_ALNUM;
			} else if (_istlead(*psz) != 0) {
				// 全角
				nType = TOKEN_TYPE_ZENKAKU;
			} else if (_istpunct(*psz) != 0) {
				// 区切り
				nType = TOKEN_TYPE_PUNCT;
			} else {
				// スペースとか
				nType = TOKEN_TYPE_NONE;
			}
		} else {
			nType = -1;
		}
		if (nType != nPrevType) {
			if ((nType == TOKEN_TYPE_ALNUM) && (nPrevType != TOKEN_TYPE_LINK)) {
				if ((_tcsncmp(psz, _T("http://"), 7) == 0) ||
					(_tcsncmp(psz, _T("https://"), 8) == 0) ||
					(_tcsncmp(psz, _T("ftp:"), 4) == 0)) {
					// リンク
					nType = TOKEN_TYPE_LINK;
				}
			}
			if (pszPrev != psz) {
				LINEINFO::TOKEN	t;
				t.nStart = pszPrev - pszLine;
				t.nLen = psz - pszPrev;
				t.nType = nPrevType;
				li.aTokens.Add(t);

				pszPrev = psz;
			}
			if (*psz == _T('\0')) {
				break;
			}
		}
	}

	m_aTexts.Add(li);
	m_dwTextSize += _tcslen(pszLine);

	// テキストの調整
	LimitText();
	// スクロールバーの調整
	SCROLLINFO info;
	info.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	GetScrollInfo(SB_VERT, &info);
	info.fMask |= SIF_DISABLENOSCROLL;
	info.nPos = (info.nPos == info.nMax) ? m_aTexts.GetUpperBound() : info.nPos;
	info.nMin = 0;
	info.nMax = m_aTexts.GetUpperBound();
	info.nPage = 1;
	SetScrollInfo(SB_VERT, &info, TRUE);

	if (bRedraw != FALSE) {
		Redraw();
	}

	return TRUE;
}

// 再描画
void CLogViewCtrl::Redraw()
{
	// データがないなら空欄表示
	if (m_aTexts.GetSize() == 0) {
		Invalidate(FALSE);
		return;
	}

	CWaitCursor	wait;

	m_fontText.DeleteObject();
	m_fontText.CreatePointFont(m_stSetting.nFontPoint, m_stSetting.szFontText);
	m_fontHeader.DeleteObject();
	m_fontHeader.CreatePointFont(m_stSetting.nFontPoint, m_stSetting.szFontHeader);

	RecalcParam();

	Invalidate(FALSE);
}

// クリア
void CLogViewCtrl::Clear()
{
	m_aTexts.RemoveAll();
	m_dwTextSize = 0;
	SCROLLINFO info;
	info.fMask = SIF_DISABLENOSCROLL | SIF_PAGE | SIF_RANGE | SIF_POS;
	info.nMin = 0;
	info.nMax = 0;
	info.nPage = 0;
	info.nPos = 0;
	SetScrollInfo(SB_VERT, &info, TRUE);
	m_nSelect = SELECT_TYPE_NONE;

	ReleaseCapture();

	Invalidate(FALSE);
}

// 設定変更
void CLogViewCtrl::SetSetting(const LOGVIEWCTRLSETTING* pSetting)
{
	// 設定変更
	m_stSetting = *pSetting;
	Redraw();
}

// 設定取得
void CLogViewCtrl::GetSetting(LOGVIEWCTRLSETTING* pSetting)
{
	*pSetting = m_stSetting;
}

// テキスト限界の設定
void CLogViewCtrl::SetTextLimit(int nLimit, DWORD dwSize)
{
	if (nLimit == LVC_TEXT_LIMIT_INFINITE) {
		m_nLimitMode = nLimit;
		m_dwLimitSize = 0;
	} else if ((nLimit == LVC_TEXT_LIMIT_LINE) || (nLimit == LVC_TEXT_LIMIT_SIZE)) {
		m_nLimitMode = nLimit;
		m_dwLimitSize = dwSize;
		// 調整
		LimitText();
		// スクロールバーの調整
		SCROLLINFO info;
		info.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
		GetScrollInfo(SB_VERT, &info);
		info.fMask |= SIF_DISABLENOSCROLL;
		info.nMin = 0;
		info.nMax = m_aTexts.GetUpperBound();
		info.nPage = 1;
		SetScrollInfo(SB_VERT, &info);
		// 再描画
		Redraw();
	}
}

// 描画パラメータ再計算
void CLogViewCtrl::RecalcParam(const SIZE* psizeClient)
{
	CCurrentDC	dc(this);

	// 1文字のサイズを取得(等幅フォントであることが前提)
	CCurrentGdiObject	cFont(&dc, &m_fontText);
	TEXTMETRIC	tm;
	dc.GetOutputTextMetrics(&tm);
	m_sizeChar.cx = tm.tmAveCharWidth;
	m_sizeChar.cy = tm.tmHeight;

	{	// ヘッダのサイズ
		CCurrentGdiObject	cHeader(&dc, &m_fontHeader);
		dc.GetOutputTextMetrics(&tm);
		m_sizeHeader.cx = tm.tmAveCharWidth * HEADER_COLUMN;
		m_sizeHeader.cy = tm.tmHeight;
	}

	// 表示できる行数と文字数の計算前処理
	if (psizeClient == NULL) {
		CRect	rc;
		GetClientRect(&rc);
		m_sizeView = rc.Size();
	} else {
		m_sizeView = *psizeClient;
	}
	m_sizeView.cx -= m_sizeHeader.cx;	// ヘッダ分を減らしておく
}

// テキストの高さ(pixel)計算
int CLogViewCtrl::GetTextHeight(CDC* pDC, int nIndex)
{
	if ((nIndex < 0) || (nIndex >= m_aTexts.GetSize())) {
		return 0;
	}

	int	cy = m_sizeChar.cy;
	int	cx = 0;
	for (LPCTSTR psz = m_aTexts[nIndex].strText; *psz != _T('\0'); psz = CharNext(psz)) {
		UINT	uChar = (_istlead(*psz) != 0) ? ((*reinterpret_cast<const WORD*>(psz) & 0x00ff) << 8) | (*reinterpret_cast<const WORD*>(psz) >> 8) : *reinterpret_cast<const BYTE*>(psz);
		int		nWidth;
		pDC->GetOutputCharWidth(uChar, uChar, &nWidth);
		cx += nWidth;
		if (cx > m_sizeView.cx) {
			cx -= m_sizeView.cx;
			cy += m_sizeChar.cy;
		}
	}

	return cy;
}

// テキスト限界調整
void CLogViewCtrl::LimitText()
{
	int	nDelCount = 0;
	if (m_nLimitMode == LVC_TEXT_LIMIT_LINE) {
		// 行数オーバーしている場合は先頭から削除する
		nDelCount = m_aTexts.GetSize() - m_dwLimitSize;
		if ((nDelCount > 0) && (nDelCount < m_aTexts.GetSize())) {
			m_aTexts.RemoveAt(0, nDelCount);
		}
	} else if (m_nLimitMode == LVC_TEXT_LIMIT_SIZE) {
		// サイズオーバーしている場合は先頭から削除する
		while ((m_aTexts.GetSize() > 0) && (m_dwTextSize > m_dwLimitSize)) {
			m_dwTextSize -= m_aTexts[0].strText.GetLength();
			m_aTexts.RemoveAt(0);
			++nDelCount;
		}
	}

	// 選択範囲の調整
	if (nDelCount > 0) {
		if (m_nSelect != SELECT_TYPE_NONE) {
			m_nSelectLineStart -= nDelCount;
			m_nSelectLineEnd -= nDelCount;
			if ((m_nSelectLineStart < 0) && (m_nSelectLineEnd < 0)) {
				m_nSelect = SELECT_TYPE_NONE;
			} else if (m_nSelectLineStart < 0) {
				if (m_nSelect == SELECT_TYPE_CHAR) {
					m_nSelect = SELECT_TYPE_NONE;
				} else {
					m_nSelectLineStart = 0;
				}
			} else if (m_nSelectLineEnd < 0) {
				m_nSelectLineEnd = 0;
			}
		}
	}
}

// クライアント座標から文字位置の計算
BOOL CLogViewCtrl::GetSelectPos(CPoint point, int* pnLine, int* pnChar)
{
	if (m_aTexts.GetSize() == 0) {
		// データがない
		return FALSE;
	}

	SCROLLINFO	info;
	info.fMask = SIF_RANGE | SIF_POS;
	GetScrollInfo(SB_VERT, &info);

	CRect	rcClient;
	GetClientRect(&rcClient);

	CCurrentDC	dc(this);
	CCurrentGdiObject	cFont(&dc, &m_fontText);

	// 行を特定する
	int	nLine = -1;
	CRect	rc = rcClient;
	CRect	rcItem;
	while ((rc.bottom > rc.top) && (info.nPos >= 0)) {
		rcItem.SetRect(rc.left, rc.bottom - GetTextHeight(&dc, info.nPos), rc.right, rc.bottom);
		rc.bottom = rcItem.top;
		if (rcItem.PtInRect(point) != FALSE) {
			nLine = info.nPos;
			break;
		}
		info.nPos--;
	}
	if (nLine < 0) {
		return FALSE;
	}

	*pnLine = nLine;

	if (pnChar == NULL) {
		return TRUE;
	}

	// 文字を特定する
	rcItem.left += m_sizeHeader.cx;
	if (rcItem.PtInRect(point) == FALSE) {
		// ヘッダ部分みたい
		*pnChar = -1;
		return TRUE;
	}
	int	y = rcItem.top + m_sizeChar.cy;
	int	x = 0;
	for (LPCTSTR psz = m_aTexts[nLine].strText; ; psz = CharNext(psz)) {
		int		nWidth;
		if (*psz != _T('\0')) {
			UINT	uChar = (_istlead(*psz) != 0) ? ((*reinterpret_cast<const WORD*>(psz) & 0x00ff) << 8) | (*reinterpret_cast<const WORD*>(psz) >> 8) : *reinterpret_cast<const BYTE*>(psz);
			dc.GetOutputCharWidth(uChar, uChar, &nWidth);
		} else {
			nWidth += m_sizeChar.cx;
		}
		x += nWidth;
		if (x > m_sizeView.cx) {
			x -= m_sizeView.cx;
			y += m_sizeChar.cy;
		}
		if (point.y < y) {
			if (point.x < (rcItem.left + x)) {
				*pnChar = psz - m_aTexts[nLine].strText;
				break;
			}
		}
		if (*psz == _T('\0')) {
			break;
		}
	}

	return TRUE;
}

// 単語の検索
int CLogViewCtrl::FindToken(int nLine, int nChar)
{
	int i;
	for (i = 0; i < m_aTexts[nLine].aTokens.GetSize(); ++i) {
		if (nChar < m_aTexts[nLine].aTokens[i].nStart) {
			return i - 1;
		}
	}
	if (i > 0) {
		if (nChar < (m_aTexts[nLine].aTokens[i - 1].nStart + m_aTexts[nLine].aTokens[i - 1].nLen)) {
			return i - 1;
		}
	}

	return -1;
}

// 選択範囲情報の更新
void CLogViewCtrl::UpdateSelectParam(int nLine, int nChar)
{
	if (m_nSelect != SELECT_TYPE_NONE) {
		// 選択範囲の変更
		if (nChar < 0) {
			// 行選択
			if (m_nSelect == SELECT_TYPE_LINE) {
				// 行選択→行選択
				m_nSelectLineEnd = nLine;
			} else {
				// 文字選択→行選択
				if (m_nSelectLineStart == nLine) {
					// 開始と同じ行
					m_nSelectLineEnd = nLine;
				} else {
					// 開始と違う行
					m_nSelectLineEnd = nLine;
				}
				// 行選択モードへ
				m_nSelect = SELECT_TYPE_LINE;
			}
		} else {
			// 文字選択
			if (m_nSelect == SELECT_TYPE_CHAR) {
				// 文字選択→文字選択
				if (m_nSelectLineStart == nLine) {
					// 開始と同じ行
					m_nSelectCharEnd = nChar;
				} else {
					// 開始と違う行
					m_nSelectLineEnd = nLine;
					// 行選択モードへ
					m_nSelect = SELECT_TYPE_LINE;
				}
			} else {
				// 行選択→文字選択
				if (m_nSelectLineStart == nLine) {
					// 開始と同じ行
					m_nSelectCharEnd = nChar;
					// 文字選択モードへ
					m_nSelect = SELECT_TYPE_CHAR;
				} else {
					// 開始と違う行
					m_nSelectLineEnd = nLine;
				}
			}
		}
		TRACE("選択変更 %d:%d,%d:%d,%d\n", m_nSelect, m_nSelectCharStart, m_nSelectCharEnd, m_nSelectLineStart, m_nSelectLineEnd);
	} else {
		// 選択開始
		if (nChar < 0) {
			// 行選択
			m_nSelect = SELECT_TYPE_LINE;
			m_nSelectCharStart = 0;
			m_nSelectCharEnd = 0;
			m_nSelectLineStart = nLine;
			m_nSelectLineEnd = nLine;
		} else {
			// 文字選択
			m_nSelect = SELECT_TYPE_CHAR;
			m_nSelectCharStart = nChar;
			m_nSelectCharEnd = nChar;
			m_nSelectLineStart = nLine;
			m_nSelectLineEnd = nLine;
		}
		TRACE("選択開始 %d:%d,%d:%d,%d\n", m_nSelect, m_nSelectCharStart, m_nSelectCharEnd, m_nSelectLineStart, m_nSelectLineEnd);
	}
}

// ウィンドウ作成前処理
BOOL CLogViewCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (CWnd::PreCreateWindow(cs) == FALSE) {
		return FALSE;
	}

	cs.style |= WS_VSCROLL;		// 縦スクロールを追加
	cs.style &= ~WS_HSCROLL;	// 横スクロールを削除
	cs.lpszClass = CLASS_NAME;

	return TRUE;
}

// ウィンドウサブクラス化前処理(ウィンドウ作成時にも呼ばれる)
void CLogViewCtrl::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();

	// フォントを作る
	m_fontText.CreatePointFont(m_stSetting.nFontPoint, m_stSetting.szFontText);
	m_fontHeader.CreatePointFont(m_stSetting.nFontPoint, m_stSetting.szFontHeader);

	// コンテキストメニューを作る
	if (m_menuPopup.CreatePopupMenu() != FALSE) {
		m_menuPopup.AppendMenu(MF_ENABLED | MF_STRING, ID_MENU_COPY, _T("コピー(&C)"));
		m_menuPopup.AppendMenu(MF_SEPARATOR);
		m_menuPopup.AppendMenu(MF_ENABLED | MF_STRING, ID_MENU_ALLSELECT, _T("すべて選択(&A)"));
	}

	SCROLLINFO info;
	info.fMask = SIF_DISABLENOSCROLL | SIF_PAGE | SIF_RANGE | SIF_POS;
	info.nMin = 0;
	info.nMax = 0;
	info.nPage = 0;
	info.nPos = 0;
	SetScrollInfo(SB_VERT, &info, TRUE);

	RecalcParam();
}


BEGIN_MESSAGE_MAP(CLogViewCtrl, CWnd)
	//{{AFX_MSG_MAP(CLogViewCtrl)
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_MENU_COPY, OnMenuCopy)
	ON_COMMAND(ID_MENU_ALLSELECT, OnMenuAllSelect)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLogViewCtrl メッセージ ハンドラ

HRESULT CLogViewCtrl::OnNcHitTest(CPoint point) 
{
	HRESULT	hHitTest = CWnd::OnNcHitTest(point);

	// 場所に応じてマウスカーソルを変更する
	switch (hHitTest) {
		case HTCLIENT:
			ScreenToClient(&point);
			if (point.x < m_sizeHeader.cx) {
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			} else {
				int	nLine;
				int	nChar;
				if ((m_stSetting.nLinkNotifyType != LVC_LINK_NOTIFY_NONE) && (GetSelectPos(point, &nLine, &nChar) != FALSE)) {
					int	nToken = FindToken(nLine, nChar);
					if ((nToken >= 0) && (m_aTexts[nLine].aTokens[nToken].nType == TOKEN_TYPE_LINK)) {
						SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
					} else {
						SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
					}
				} else {
					SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
				}
			}
			break;
		case HTHSCROLL:
		case HTVSCROLL:
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			break;
		default:
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			break;
	}

	return hHitTest;
}

void CLogViewCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (m_aTexts.GetSize() > 0) {
		// 描画パラメータの更新
		RecalcParam(&CSize(cx, cy));
	}
}

BOOL CLogViewCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// 背景は消去しない
	return FALSE;
}

void CLogViewCtrl::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	CRect	rcClient;
	GetClientRect(&rcClient);
	if (m_aTexts.GetSize() == 0) {
		// テキストを描画しないけど背景は描画しておく
		dc.FillSolidRect(&rcClient, m_stSetting.crBack);
		return;
	}

	dc.SetBkMode(OPAQUE);
	CCurrentGdiObject	cFont(&dc, &m_fontText);

	CPen	penLink(PS_SOLID, 1, m_stSetting.crLinkText);
	CCurrentGdiObject	cPen(&dc, &penLink);

	SCROLLINFO	info;
	info.fMask = SIF_RANGE | SIF_POS;
	GetScrollInfo(SB_VERT, &info);

	// 選択範囲の調整
	int	nSelectLineStart;
	int	nSelectLineEnd;
	if ((m_nSelect == SELECT_TYPE_LINE) && (m_nSelectLineStart > m_nSelectLineEnd)) {
		nSelectLineStart = m_nSelectLineEnd;
		nSelectLineEnd = m_nSelectLineStart;
	} else {
		nSelectLineStart = m_nSelectLineStart;
		nSelectLineEnd = m_nSelectLineEnd;
	}
	int	nSelectCharStart;
	int	nSelectCharEnd;
	if ((m_nSelect == SELECT_TYPE_CHAR) && (m_nSelectCharStart > m_nSelectCharEnd)) {
		nSelectCharStart = m_nSelectCharEnd;
		nSelectCharEnd = m_nSelectCharStart;
	} else {
		nSelectCharStart = m_nSelectCharStart;
		nSelectCharEnd = m_nSelectCharEnd;
	}

	CRect	rcItem(m_sizeHeader.cx, rcClient.bottom, m_sizeHeader.cx + m_sizeView.cx, rcClient.bottom);
	for (int i = info.nPos; (i >= 0) && (rcItem.bottom > 0); --i) {
		int	cy = GetTextHeight(&dc, i);
		rcItem.bottom = rcItem.top;
		rcItem.top -= cy;
		if (rcItem.bottom >= 0) {
			dc.SetTextColor(m_aTexts[i].crFore);
			dc.SetBkColor(m_aTexts[i].crBack);
			dc.FillSolidRect(0, rcItem.top, m_sizeHeader.cx, rcItem.Height(), m_aTexts[i].crBack);
			{	// ヘッダ描画
				CCurrentGdiObject	cHeader(&dc, &m_fontHeader);
				CString	strTime;
				strTime.Format(_T("[%02d:%02d:%02d]"), m_aTexts[i].stDateTime.wHour, m_aTexts[i].stDateTime.wMinute, m_aTexts[i].stDateTime.wSecond);
				dc.TextOut(0, rcItem.top, strTime);
			}
			// 背景塗り潰し用リージョン
			CRgn	rgnItem;
			rgnItem.CreateRectRgn(rcItem.left, rcItem.top, rcClient.right, rcItem.bottom);
			// テキスト描画
			int	nToken = 0;
			CPoint	ptDraw(rcItem.left, rcItem.top);
			for (LPCTSTR psz = m_aTexts[i].strText; *psz != _T('\0'); psz = CharNext(psz)) {
				int	nIndex = psz - (LPCTSTR)m_aTexts[i].strText;
				if (nIndex >= (m_aTexts[i].aTokens[nToken].nStart + m_aTexts[i].aTokens[nToken].nLen)) {
					++nToken;
				}
				if (((m_nSelect == SELECT_TYPE_CHAR) && (i == nSelectLineStart) && (nIndex >= nSelectCharStart) && (nIndex < nSelectCharEnd)) ||
					((m_nSelect == SELECT_TYPE_LINE) && (i >= nSelectLineStart) && (i <= nSelectLineEnd))) {
					// 選択
					dc.SetTextColor(m_stSetting.crSelectText);
					dc.SetBkColor(m_stSetting.crSelectBack);
				} else if ((m_stSetting.nLinkNotifyType != LVC_LINK_NOTIFY_NONE) && (m_aTexts[i].aTokens[nToken].nType == TOKEN_TYPE_LINK)) {
					// リンク
					dc.SetTextColor(m_stSetting.crLinkText);
					dc.SetBkColor(m_stSetting.crLinkBack);
				} else {
					// 通常
					dc.SetTextColor(m_aTexts[i].crFore);
					dc.SetBkColor(m_aTexts[i].crBack);
				}
				UINT	uChar;
				int		nChar;
				if (_istlead(*psz) != 0) {
					uChar = ((*reinterpret_cast<const WORD*>(psz) & 0x00ff) << 8) | (*reinterpret_cast<const WORD*>(psz) >> 8);
					nChar = 2;
				} else {
					uChar = *reinterpret_cast<const BYTE*>(psz);
					nChar = 1;
				}
				int		nWidth;
				dc.GetOutputCharWidth(uChar, uChar, &nWidth);
				if ((ptDraw.x + nWidth) > rcItem.right) {
					// 改行
					ptDraw.x = rcItem.left;
					ptDraw.y += m_sizeChar.cy;
				}
				dc.TextOut(ptDraw.x, ptDraw.y, psz, nChar);
				// リンクの場合の下線描画
				if ((m_stSetting.nLinkNotifyType != LVC_LINK_NOTIFY_NONE) && (m_aTexts[i].aTokens[nToken].nType == TOKEN_TYPE_LINK)) {
					dc.MoveTo(ptDraw.x, ptDraw.y + m_sizeChar.cy - 1);
					dc.LineTo(ptDraw.x + nWidth, ptDraw.y + m_sizeChar.cy - 1);
				}
				// 背景塗り潰しリージョンの更新
				CRgn	rgnChar;
				rgnChar.CreateRectRgn(ptDraw.x, ptDraw.y, ptDraw.x + nWidth, ptDraw.y + m_sizeChar.cy);
				rgnItem.CombineRgn(&rgnItem, &rgnChar, RGN_DIFF);
				// 描画位置の更新
				ptDraw.x += nWidth;
			}
			// 背景塗り潰し
			if ((m_nSelect == SELECT_TYPE_LINE) && ((i >= nSelectLineStart) && (i <= nSelectLineEnd))) {
				// 行選択
				CBrush	brBack(m_stSetting.crSelectBack);
				dc.FillRgn(&rgnItem, &brBack);
			} else {
				// 行選択以外
				CBrush	brBack(m_aTexts[i].crBack);
				dc.FillRgn(&rgnItem, &brBack);
			}
		}
	}
	if (rcItem.top > 0) {
		dc.FillSolidRect(0, 0, rcClient.right, rcItem.top, m_stSetting.crBack);
	}
}

void CLogViewCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	SCROLLINFO info;
	info.fMask = SIF_RANGE | SIF_POS;
	GetScrollInfo(SB_VERT, &info);

	switch (nSBCode) {
		case SB_TOP:
			info.nPos = info.nMin;
			break;
		case SB_PAGEUP:
			{
				CCurrentDC	dc(this);
				CCurrentGdiObject	cFont(&dc, &m_fontText);
				CRect	rc;
				GetClientRect(&rc);
				for ( ; info.nPos > 0; --info.nPos) {
					rc.bottom -= GetTextHeight(&dc, info.nPos);
					if (rc.bottom <= rc.top) {
						break;
					}
				}
			}
			break;
		case SB_LINEUP:
			--info.nPos;
			break;
		case SB_LINEDOWN:
			++info.nPos;
			break;
		case SB_PAGEDOWN:
			{
				CCurrentDC	dc(this);
				CCurrentGdiObject	cFont(&dc, &m_fontText);
				CRect	rc;
				GetClientRect(&rc);
				for ( ; info.nPos < info.nMax; ++info.nPos) {
					rc.top += GetTextHeight(&dc, info.nPos);
					if (rc.top >= rc.bottom) {
						break;
					}
				}
			}
			break;
		case SB_BOTTOM:
			info.nPos = info.nMax;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			info.nPos = nPos;
			break;
	}
	if (nSBCode != SB_ENDSCROLL) {
		info.fMask = SIF_DISABLENOSCROLL | SIF_POS;
		SetScrollInfo(SB_VERT, &info, TRUE);
		Invalidate(FALSE);
	}
}

void CLogViewCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ((nFlags & MK_SHIFT) != 0) {
		int	nLine;
		int	nChar;
		if (GetSelectPos(point, &nLine, &nChar) != FALSE) {
			UpdateSelectParam(nLine, nChar);
			SetCapture();
			Invalidate(FALSE);
		}
	} else {
		if (m_nSelect != SELECT_TYPE_NONE) {
			int	nToken;
			if ((m_nSelect == SELECT_TYPE_CHAR) &&
				(m_stSetting.nLinkNotifyType == LVC_LINK_NOTIFY_TRIPLE_CLICK) &&
				((GetTickCount() - m_dwTripleClick) <= GetDoubleClickTime()) &&
				(abs(m_ptTripleClick.x - point.x) <= GetSystemMetrics(SM_CXDOUBLECLK)) &&
				(abs(m_ptTripleClick.y - point.y) <= GetSystemMetrics(SM_CYDOUBLECLK)) &&
				((nToken = FindToken(m_nSelectLineStart, m_nSelectCharStart)) >= 0) &&
				(m_aTexts[m_nSelectLineStart].aTokens[nToken].nType == TOKEN_TYPE_LINK)) {
				// トリプルクリックされたので親へ通知
				CString	strSelect = m_aTexts[m_nSelectLineStart].strText.Mid(m_nSelectCharStart, m_nSelectCharEnd - m_nSelectCharStart);
				NMLVCLINK	nmLink;
				nmLink.hdr.hwndFrom = m_hWnd;
				nmLink.hdr.idFrom = GetDlgCtrlID();
				nmLink.hdr.code = LVCN_LINK;
				nmLink.pszLink = strSelect;
				GetParent()->SendMessage(WM_NOTIFY, nmLink.hdr.idFrom, reinterpret_cast<LPARAM>(&nmLink));
			} else {
				// 選択範囲の解除
				m_nSelect = SELECT_TYPE_NONE;
				Invalidate(FALSE);
				TRACE("選択解除\n");
			}
		}
	}
}

void CLogViewCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_aTexts.GetSize() == 0) {
		// 表示するデータがない場合は処理しない
		return;
	}

	ReleaseCapture();
}

void CLogViewCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int	nLine;
	int	nChar;
	if (GetSelectPos(point, &nLine, &nChar) != FALSE) {
		int	nToken = FindToken(nLine, nChar);
		if (nToken >= 0) {
			// 単語がダブルクリックされた
			m_nSelect = SELECT_TYPE_CHAR;
			m_nSelectLineStart = nLine;
			m_nSelectCharStart = m_aTexts[nLine].aTokens[nToken].nStart;
			m_nSelectCharEnd = m_nSelectCharStart + m_aTexts[nLine].aTokens[nToken].nLen;
			Invalidate(FALSE);

			if (m_aTexts[nLine].aTokens[nToken].nType == TOKEN_TYPE_LINK) {
				// リンクがダブルクリックされた
				if (m_stSetting.nLinkNotifyType == LVC_LINK_NOTIFY_DOUBLE_CLICK) {
					// 親へ通知
					NMLVCLINK	nmLink;
					nmLink.hdr.hwndFrom = m_hWnd;
					nmLink.hdr.idFrom = GetDlgCtrlID();
					nmLink.hdr.code = LVCN_LINK;
					nmLink.pszLink = m_aTexts[nLine].strText.Mid(m_aTexts[nLine].aTokens[nToken].nStart, m_aTexts[nLine].aTokens[nToken].nLen);
					GetParent()->SendMessage(WM_NOTIFY, nmLink.hdr.idFrom, reinterpret_cast<LPARAM>(&nmLink));
				} else {
					// トリプルクリック判定の準備
					m_dwTripleClick = GetTickCount();
					m_ptTripleClick = point;
				}
			}
		}
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

int CLogViewCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	int	nResult = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	if ((nResult == MA_NOACTIVATE) || (nResult == MA_NOACTIVATEANDEAT)) {
		return nResult;
	}

	SetFocus();

	return nResult;
}

void CLogViewCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_aTexts.GetSize() == 0) {
		// 表示するデータがない場合は処理しない
		return;
	}

	if ((nFlags & MK_LBUTTON) != 0) {
		if (m_nSelect != SELECT_TYPE_NONE) {
			CRect	rcClient;
			GetClientRect(&rcClient);
			if (point.y < rcClient.top) {
				OnVScroll(SB_LINEUP, 0, NULL);
			} else if (point.y > rcClient.bottom) {
				OnVScroll(SB_LINEDOWN, 0, NULL);
			}
			int	nLine;
			int	nChar;
			if (GetSelectPos(point, &nLine, &nChar) != FALSE) {
				UpdateSelectParam(nLine, nChar);
				Invalidate(FALSE);
			}
		} else {
			int	nLine;
			int	nChar;
			if (GetSelectPos(point, &nLine, &nChar) != FALSE) {
				UpdateSelectParam(nLine, nChar);
				SetCapture();
				Invalidate(FALSE);
			}
		}
	}
}

BOOL CLogViewCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	SCROLLINFO info;
	info.fMask = SIF_POS;
	GetScrollInfo(SB_VERT, &info);

	if (zDelta < 0) {
		// 下へ
		info.nPos += m_stSetting.nWheelDelta;
		info.fMask = SIF_DISABLENOSCROLL | SIF_POS;
		SetScrollInfo(SB_VERT, &info, TRUE);
		if ((m_nSelect != SELECT_TYPE_NONE) && ((nFlags & MK_LBUTTON) != 0)) {
			CPoint	ptCursor;
			GetCursorPos(&ptCursor);
			ScreenToClient(&ptCursor);
			int	nLine;
			int	nChar;
			if (GetSelectPos(ptCursor, &nLine, &nChar) != FALSE) {
				UpdateSelectParam(nLine, nChar);
			}
		}
		Invalidate();
	} else if (zDelta > 0) {
		// 上へ
		info.nPos -= m_stSetting.nWheelDelta;
		info.fMask = SIF_DISABLENOSCROLL | SIF_POS;
		SetScrollInfo(SB_VERT, &info, TRUE);
		if ((m_nSelect != SELECT_TYPE_NONE) && ((nFlags & MK_LBUTTON) != 0)) {
			CPoint	ptCursor;
			GetCursorPos(&ptCursor);
			ScreenToClient(&ptCursor);
			int	nLine;
			int	nChar;
			if (GetSelectPos(ptCursor, &nLine, &nChar) != FALSE) {
				UpdateSelectParam(nLine, nChar);
			}
		}
		Invalidate();
	}

	return TRUE;
}

void CLogViewCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (m_aTexts.GetSize() == 0) {
		// 表示するデータがない場合は処理しない
		return;
	}

	switch (nChar) {
		case VK_UP:
			OnVScroll(SB_LINEUP, 0, NULL);
			break;
		case VK_DOWN:
			OnVScroll(SB_LINEDOWN, 0, NULL);
			break;
		case VK_PRIOR:
			OnVScroll(SB_PAGEUP, 0, NULL);
			break;
		case VK_NEXT:
			OnVScroll(SB_PAGEDOWN, 0, NULL);
			break;
		case VK_HOME:
			OnVScroll(SB_TOP, 0, NULL);
			break;
		case VK_END:
			OnVScroll(SB_BOTTOM, 0, NULL);
			break;
		case _T('A'):
			if (GetKeyState(VK_CONTROL) < 0) {
				OnMenuAllSelect();
			}
			break;
		case _T('C'):
			if (GetKeyState(VK_CONTROL) < 0) {
				OnMenuCopy();
			}
			break;
		default:
			break;
	}
}

void CLogViewCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (m_aTexts.GetSize() > 0) {
		m_menuPopup.EnableMenuItem(ID_MENU_ALLSELECT, MF_BYCOMMAND | MF_ENABLED);
	} else {
		m_menuPopup.EnableMenuItem(ID_MENU_ALLSELECT, MF_BYCOMMAND | MF_GRAYED);
	}

	GetCursorPos(&point);
	m_menuPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CLogViewCtrl::OnMenuCopy()
{
	CWaitCursor	wc;

	HANDLE	hMem = NULL;
	if (m_nSelect == SELECT_TYPE_CHAR) {
		// 文字選択
		int	nSelectCharStart;
		int	nSelectCharEnd;
		if (m_nSelectCharStart <= m_nSelectCharEnd) {
			nSelectCharStart = m_nSelectCharStart;
			nSelectCharEnd = m_nSelectCharEnd;
		} else {
			nSelectCharStart = m_nSelectCharEnd;
			nSelectCharEnd = m_nSelectCharStart;
		}
		if (m_nSelectLineStart < m_aTexts.GetSize()) {
			CString	str = m_aTexts[m_nSelectLineStart].strText.Mid(nSelectCharStart, nSelectCharEnd - nSelectCharStart);
			hMem = GlobalAlloc(GHND, str.GetLength() + sizeof(_T('\0')));
			if (hMem != NULL) {
				_tcscpy(reinterpret_cast<LPTSTR>(GlobalLock(hMem)), str);
				GlobalUnlock(hMem);
			}
		}
	} else if (m_nSelect == SELECT_TYPE_LINE) {
		// 行選択
		int	nSelectLineStart;
		int	nSelectLineEnd;
		if (m_nSelectLineStart <= m_nSelectLineEnd) {
			nSelectLineStart = m_nSelectLineStart;
			nSelectLineEnd = m_nSelectLineEnd;
		} else {
			nSelectLineStart = m_nSelectLineEnd;
			nSelectLineEnd = m_nSelectLineStart;
		}
		hMem = GlobalAlloc(GHND, m_dwTextSize + (sizeof(_T('\n')) + HEADER_COLUMN) * m_aTexts.GetSize() + sizeof(TCHAR));
		if (hMem != NULL) {
			LPTSTR	psz = reinterpret_cast<LPTSTR>(GlobalLock(hMem));
			for (int i = nSelectLineStart; (i <= nSelectLineEnd) && (i < m_aTexts.GetSize()); ++i) {
				CString	str;
				str.Format(_T("[%02d:%02d:%02d] "), m_aTexts[i].stDateTime.wHour, m_aTexts[i].stDateTime.wMinute, m_aTexts[i].stDateTime.wSecond);
				_tcscat(psz, str);
				_tcscat(psz, m_aTexts[i].strText);
				_tcscat(psz, _T("\n"));
			}
			GlobalUnlock(hMem);
		}
	}

	if (hMem != NULL) {
		BOOL	bRet = FALSE;
		if (OpenClipboard() != FALSE) {
			if ((EmptyClipboard() != FALSE) &&
				(SetClipboardData(CF_TEXT, hMem) != NULL)) {
				bRet = TRUE;
			}
			CloseClipboard();
		}
		if (bRet == FALSE) {
			GlobalFree(hMem);
		}
	}

	if (m_stSetting.bCancelSelectAfterCopy != FALSE) {
		m_nSelect = SELECT_TYPE_NONE;
		Invalidate(FALSE);
		TRACE("選択解除\n");
	}
}

void CLogViewCtrl::OnMenuAllSelect()
{
	if (m_aTexts.GetSize() == 0) {
		// データがないので処理しない
		return;
	}

	m_nSelect = SELECT_TYPE_LINE;
	m_nSelectLineStart = 0;
	m_nSelectLineEnd = m_aTexts.GetSize() - 1;
	m_nSelectCharStart = 0;
	m_nSelectCharEnd = 0;

	Invalidate(FALSE);
}

UINT CLogViewCtrl::OnGetDlgCode() 
{
	return DLGC_WANTARROWS;
}
