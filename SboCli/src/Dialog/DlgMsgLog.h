/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名：	DlgMsgLog.h													 */
/* 内容：		メッセージログダイアログクラス 定義ファイル					 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2005/09/25													 */
/* ========================================================================= */

#pragma once

#include "resource.h"
#include "LayoutHelper.h"
#include "LogViewCtrl.h"

class CMgrData;
class CTextOutput;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgMsgLog : public CDialog, CLayoutHelper
{
public:
			CDlgMsgLog(CWnd* pParent = NULL);			/* コンストラクタ */
	virtual ~CDlgMsgLog();								/* デストラクタ */

	BOOL Create (HWND hWndParent, CMgrData *pMgrData);			/* 作成 */
	void Add	(LPCSTR pszLog, COLORREF cl);					/* 追加 */



protected:
	void	MakeLogFile	(void);									/* 現在時刻でログファイルを作成 */
	static LRESULT CALLBACK ChatWndProc		(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* チャット入力欄プロシージャ */


protected:
	HWND			m_hWndChat;				/* チャット入力欄 */
	WNDPROC			m_OrgWndProcChat;		/* チャット入力欄の元ウィンドウプロシージャ */
	BOOL			m_bPushEnter;			/* Enterキー押下状態 */
	CTime			m_timeMakeLog;			/* ログ作成年月日 */
	CMgrData		*m_pMgrData;			/* データ管理 */
	CTextOutput		*m_pLog;				/* ログファイル */



public:
	void OnOK () {}
	void OnCancel () {}
	//{{AFX_DATA(CDlgMsgLog)
	enum { IDD = IDD_MSGLOG };
	CLogViewCtrl m_wndLogViewCtrl;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgMsgLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgMsgLog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnLink(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTopMost();
	afx_msg void OnBnClickedHideMainframe();
	BOOL m_bTopMost;
	BOOL m_bHideMainFrame;
	CString m_strChat;
} CDlgMsgLog, *PCDlgMsgLog;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2005 */
