/// @file MainFrameWindow.cpp
/// @brief サーバーメインフレーム GUI(GDI/USER32)実装ファイル(Windows 専用)
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/04
/// @copyright Copyright(C)URARA-works 2006
///
/// MainFrame.cpp からウィンドウ表示専用の実装を切り出したファイル。
/// 対になる非Windows 版は MainFrameWindowNone.cpp(同じ関数の「何もしない版」)。
/// ビルド側(.vcxproj / 移植チェックスクリプト)でどちらか一方だけをコンパイル
/// 対象に選ぶ構成にしており、このファイル自体に #ifdef は書かない。

#include "StdAfx.h"
#include "resource.h"
#include "SBOVersion.h"
#include "LibInfoMapBase.h"
#include "LibInfoCharSvr.h"
#include "MgrData.h"
#include "MainFrame.h"
#include "../Platform/SvrPlatform.h"

// 定数定義

#define CLNAME "SboSvr"	// 登録クラス名

// 状態表示用フォントを作成する
//
// コンストラクタから呼ぶ。ウィンドウの有無に関わらず作成しておき、
// OnPaint() で使う(ヘッドレス時は OnPaint 自体が呼ばれないため未使用のまま残る)。

void CMainFrame::CreateStateFont(void)
{
	m_hFont = CreateFont(12, 0, 0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("ＭＳ ゴシック"));
}

// 状態表示用フォントを破棄する
//
// デストラクタから呼ぶ。

void CMainFrame::DestroyStateFont(void)
{
	if (m_hFont) {
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
}

// 状態表示の再描画を要求する
//
// ウィンドウがある場合のみ再描画する。TimerProcClock() の末尾から呼ばれる。

void CMainFrame::RefreshStateDisplay(void)
{
	if (m_hWnd) {
		InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

int CMainFrame::MainLoopWindow(HINSTANCE hInstance)
{
	TCHAR szBuf[256];
	MSG msg;
	TIMECAPS tc;
	WNDCLASS wc;

	wc.hInstance	= hInstance;
	wc.lpszClassName	= _T(CLNAME);
	wc.lpfnWndProc	= (WNDPROC)WndProcEntry;
	wc.style	= 0;
	wc.hIcon	= NULL;//LoadIcon (hInstance, (char *)IDI_MAINFRAME);
	wc.hCursor	= LoadCursor((HINSTANCE)NULL, IDC_ARROW);
	wc.lpszMenuName	= _T("IDR_MENU");
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);

	// ウィンドウのクラスを登録
	if (!RegisterClass(&wc)) {
		return FALSE;
	}

	// ウィンドウ作成
	wsprintf(szBuf, _T("%s Ver%s"), _T(WNDTITLE), _T(VERTEXT));
	m_hWnd = CreateWindow(
				_T(CLNAME),
				szBuf,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT,
				(int)(strlen(WNDTITLE) * 12), 150,
				NULL,
				NULL,
				hInstance,
				this);
	if (m_hWnd == NULL) {
		return FALSE;
	}

	ShowWindow(m_hWnd, SW_SHOW);

	timeGetDevCaps(&tc, sizeof (TIMECAPS));
	// マルチメディアタイマーのサービス精度を最大に
	timeBeginPeriod(tc.wPeriodMin);

	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		} else {
			TimerProc();
		}
	}

	timeEndPeriod(tc.wPeriodMin);
	UnregisterClass(_T(CLNAME), hInstance);

	// 終了メッセージによりプログラム終了
	return (int)msg.wParam;
}

LRESULT CALLBACK CMainFrame::WndProcEntry(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pThis;

	if (msg == WM_CREATE) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams));
	}

	// ユーザデータから this ポインタを取得し、処理を行う
	pThis = (CMainFrame *)GetWindowLong(hWnd, GWL_USERDATA);
	if (pThis) {
		return pThis->WndProc(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	HANDLE_MSG(hWnd, WM_CREATE,	OnCreate);
	HANDLE_MSG(hWnd, WM_CLOSE,	OnClose);
	HANDLE_MSG(hWnd, WM_DESTROY,	OnDestroy);
	HANDLE_MSG(hWnd, WM_PAINT,	OnPaint);
	HANDLE_MSG(hWnd, WM_COMMAND,	OnCommand);

	default:
		// 通信の通知は SetNotifySink() 経由でキューへ入るため、ここには来ない。
		// 修理しないメッセージはOSに返す
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

// ウィンドウ位置を復元
//
// ヘッドレス時は呼ばない。位置を持たないため読み込む意味が無い。

void CMainFrame::LoadWindowPos(HWND hWnd)
{
	RECT rc;
	std::string strIni = SboPlatform::GetIniFilePath();
	const char *pszIni = strIni.c_str();

	rc.left	= SboPlatform::GetIniInt(pszIni, "Pos", "MainLeft",	-1);
	rc.top	= SboPlatform::GetIniInt(pszIni, "Pos", "MainTop",	-1);
	rc.right	= SboPlatform::GetIniInt(pszIni, "Pos", "MainRight",	-1);
	rc.bottom	= SboPlatform::GetIniInt(pszIni, "Pos", "MainBottom",	-1);
	if (!((rc.left == -1) && (rc.top == -1))) {
		SetWindowPos(hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER);
	}
}

// ウィンドウ位置を保存
//
// ヘッドレス時は呼ばない。位置が無いのに書き込むと、次に
// ウィンドウ付きで起動したときの位置を壊してしまう。

void CMainFrame::SaveWindowPos(HWND hWnd)
{
	RECT rc;
	char szValue[32];

	if ((IsIconic(hWnd) != FALSE) || (IsWindowVisible(hWnd) == FALSE)) {
		return;
	}

	std::string strIni = SboPlatform::GetIniFilePath();
	const char *pszIni = strIni.c_str();
	GetWindowRect(hWnd, &rc);

	_snprintf_s(szValue, sizeof (szValue), _TRUNCATE, "%d", (int)rc.left);
	SboPlatform::SetIniString(pszIni, "Pos", "MainLeft", szValue);
	_snprintf_s(szValue, sizeof (szValue), _TRUNCATE, "%d", (int)rc.top);
	SboPlatform::SetIniString(pszIni, "Pos", "MainTop", szValue);
	_snprintf_s(szValue, sizeof (szValue), _TRUNCATE, "%d", (int)rc.right);
	SboPlatform::SetIniString(pszIni, "Pos", "MainRight", szValue);
	_snprintf_s(szValue, sizeof (szValue), _TRUNCATE, "%d", (int)rc.bottom);
	SboPlatform::SetIniString(pszIni, "Pos", "MainBottom", szValue);
}

BOOL CMainFrame::OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	m_hWnd	= hWnd;

	LoadWindowPos(hWnd);

	return InitServer();
}

void CMainFrame::OnClose(HWND hWnd)
{
	SaveWindowPos(hWnd);
	TermServer();

	DestroyWindow(hWnd);
}

void CMainFrame::OnDestroy(HWND hWnd)
{
	m_pMgrData->Save();

	PostQuitMessage(0);
}

// サーバー状態の表示項目を取得
//
// GDI にもウィンドウにも依存しない。ヘッドレス化した際は
// この結果をコンソールやログへ出せばよい。
//
// 戻り値は格納した項目数。

int CMainFrame::GetServerStateItem(
	SERVERSTATEITEM *paItem,	// [out] 項目の格納先
	int nMax)	// [in] 格納先の要素数
{
	int nCount;
	DWORD dwTime;

	if ((paItem == NULL) || (nMax <= 0)) {
		return 0;
	}

	nCount	= 0;
	dwTime	= SboPlatform::GetTickMs() - m_dwServerStartTime;

	if (nCount < nMax) {
		paItem[nCount].strLabel	= _T("サーバー稼動時間");
		paItem[nCount].strValue.Format(_T("%04d:%02d:%02d"),
				 dwTime / 3600000,
				 (dwTime % 3600000 - ((dwTime % 60000) / 1000)) / 60000,
				 (dwTime % 60000) / 1000);
		nCount ++;
	}
	if (nCount < nMax) {
		paItem[nCount].strLabel	= _T("接続数");
		paItem[nCount].strValue.Format(_T("%d"), m_pLibInfoChar->GetCountOnline());
		nCount ++;
	}
	if (nCount < nMax) {
		paItem[nCount].strLabel	= _T("処理キャラ数");
		paItem[nCount].strValue.Format(_T("%d"), m_pLibInfoChar->GetCount());
		nCount ++;
	}
	if (nCount < nMax) {
		paItem[nCount].strLabel	= _T("処理マップ数");
		paItem[nCount].strValue.Format(_T("%d"), m_pLibInfoMap->GetCount());
		nCount ++;
	}

	return nCount;
}

void CMainFrame::OnPaint(HWND hWnd)
{
	int i, nCount;
	HFONT hFontOld;
	HDC hDC;
	PAINTSTRUCT ps;
	SERVERSTATEITEM aItem[SERVERSTATEITEM_MAX];

	nCount	= GetServerStateItem(aItem, _countof(aItem));

	hDC	= BeginPaint(hWnd, &ps);

	SetBkMode(hDC, TRANSPARENT);
	hFontOld = (HFONT)SelectObject(hDC, m_hFont);

	for (i = 0; i < nCount; i ++) {
		SetTextColor(hDC, RGB(0, 255, 0));
		MyTextOut(hDC, 0, 12 * i, aItem[i].strLabel);

		SetTextColor(hDC, RGB(255, 255, 255));
		MyTextOut(hDC, 120, 12 * i, aItem[i].strValue);
	}

	SelectObject(hDC, hFontOld);

	EndPaint(hWnd, &ps);
}

void CMainFrame::OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify)
{
	switch (id) {
	case IDM_UPDATE_RENEW:	// アップデートファイル更新
		OnCommandUPDATE_RENEW();
		break;
	}
}

void CMainFrame::MyTextOut(HDC hDC, int x, int y, LPCTSTR pStr)
{
	if (pStr == NULL) {
		return;
	}

	int nLen = static_cast<int>(_tcslen(pStr));
	::TextOut(hDC, x, y, pStr, nLen);
}
