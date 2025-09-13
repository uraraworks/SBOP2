/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapParts.h											 */
/* 内容			:マップパーツ編集ダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/25													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "StaticMapParts.h"

class CMgrGrpData;
class CImg32;
class CLibInfoMapParts;
class CUraraSockTCPSBO;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CDlgAdminMapParts : public CDlgAdminBase
{
public:
			CDlgAdminMapParts(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminMapParts();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);						/* 初期化 */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* メッセージハンドラ(WM_ADMINMSG) */


protected:
	void	RenewMessage		(LPCSTR pszMsg);	/* メッセージ欄を更新 */
	void	RenewPartsInfo		(void);				/* パーツ情報を更新 */
	void	MakePartsImage		(void);				/* パーツ一覧画像を作成 */
	DWORD	GetSelectMapPartsID	(void);				/* 選択中のマップパーツIDを取得 */


protected:
	int		m_nSelectType,						/* 設定項目 */
			m_nState;							/* 状態 */
	DWORD	m_dwSelectPartsID;					/* 選択中のマップパーツID */
	CPoint	m_ptCursor,							/* カーソルのあるパーツ位置 */
			m_ptMoveSrc,						/* 移動元の位置 */
			m_ptMoveDst;						/* 移動先の位置 */

	CMgrGrpData			*m_pMgrGrpData;			/* グラフィックデータ管理 */
	CImg32				*m_pImgParts;			/* パーツ一覧画像 */
	CLibInfoMapParts	*m_pLibInfoMapParts;	/* 編集中のマップパーツ情報 */
	CUraraSockTCPSBO	*m_pSock;				/* 通信マネージャ */


public:
	//{{AFX_DATA(CDlgAdminMapParts)
	enum { IDD = IDD_MAP_PARTS };
	CStaticMapParts	m_ctlMapParts;
	CScrollBar	m_ctlScroll;
	CComboBox	m_cbType;
	int		m_nTypeSelect;
	CString	m_strID;
	CString	m_strMsg;
	CString	m_strAttr;
	CString	m_strAttr2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapParts)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapParts)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeType();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	afx_msg LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bPile;
	afx_msg void OnBnClickedPile();
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
