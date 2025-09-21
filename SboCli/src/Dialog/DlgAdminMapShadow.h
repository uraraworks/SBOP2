/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapShadow.h										 */
/* 内容			:マップ影編集ダイアログクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/06													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "StaticMapShadow.h"

class CMgrGrpData;
class CImg32;
class CLibInfoMapShadow;
class CUraraSockTCPSBO;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CDlgAdminMapShadow : public CDlgAdminBase
{
public:
			CDlgAdminMapShadow(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminMapShadow();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);						/* 初期化 */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* メッセージハンドラ(WM_ADMINMSG) */


protected:
	void	RenewMessage			(LPCSTR pszMsg);	/* メッセージ欄を更新 */
	void	MakeShadowImage			(void);				/* 影一覧画像を作成 */
	DWORD	GetSelectMapShadowID	(void);				/* 選択中の影IDを取得 */


protected:
	int		m_nSelectType,						/* 設定項目 */
			m_nState;							/* 状態 */
	DWORD	m_dwSelectShadowID;					/* 選択中の影ID */
	CPoint	m_ptCursor,							/* カーソルのある位置 */
			m_ptMoveSrc,						/* 移動元の位置 */
			m_ptMoveDst;						/* 移動先の位置 */

	CMgrGrpData			*m_pMgrGrpData;			/* グラフィックデータ管理 */
	CImg32				*m_pImgShadow;			/* 影一覧画像 */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;	/* 編集中のマップ影情報 */
	CUraraSockTCPSBO	*m_pSock;				/* 通信マネージャ */



public:
	//{{AFX_DATA(CDlgAdminMapShadow)
	enum { IDD = IDD_MAP_SHADOW };
	CStaticMapShadow	m_ctlMapShadow;
	CScrollBar	m_ctlScroll;
	CComboBox	m_cbType;
	int		m_nTypeSelect;
	CString	m_strID;
	CString	m_strMsg;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapShadow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapShadow)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeType();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	afx_msg LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
