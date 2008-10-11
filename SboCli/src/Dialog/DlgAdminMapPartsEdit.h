/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapPartsEdit.h										 */
/* 内容			:マップパーツ編集ダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/14													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CMgrData;
class CWndMapPartsAnimeList;
class CWndMapPartsGrp;
class CInfoMapParts;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CDlgAdminMapPartsEdit : public CDlgAdminBase
{
public:
			CDlgAdminMapPartsEdit(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual	~CDlgAdminMapPartsEdit();						/* デストラクタ */

	int DoModal (CMgrData *pMgrData);								/* モーダルダイアログ表示 */


protected:
	void	SetData				(void);					/* データを画面に反映 */
	void	GetData				(void);					/* 画面からデータを取得 */
	void	SetPartsType		(DWORD dwType);			/* パーツ種別に応じてチェックボックスの状態を設定 */
	DWORD	GetPartsType		(void);					/* チェックボックスの状態に応じてパーツ種別を取得 */
	void	OnSelectAnime		(void);					/* コマ選択処理 */
	void	RenewButtonState	(void);					/* ボタン状態の設定 */


public:
	int						m_nSelectType,				/* どちらの画像を選択しているか */
							m_nAnimeNo;					/* アニメーション中の番号 */
	WORD					m_wPartsBase,				/* 下地 */
							m_wPartsPile;				/* 重ね合わせ */
	DWORD					m_dwTimeLastAnime;			/* 最後にアニメーションした時間 */
	SIZE					m_sizeWindow;				/* 表示開始時のウィンドウサイズ */
	CMgrData				*m_pMgrData;				/* データ管理 */
	CMgrGrpData				*m_pMgrGrpData;				/* グラフィックデータ管理 */
	CWndMapPartsAnimeList	*m_pWndMapPartsAnimeList;	/* マップパーツアニメーション一覧ウィンドウ */
	CInfoMapParts			*m_pInfoMapParts;			/* マップパーツ情報 */
	CWndMapPartsGrp			*m_pWndMapPartsGrp;			/* マップパーツ画像一覧 */



public:
	//{{AFX_DATA(CDlgAdminMapPartsEdit)
	enum { IDD = IDD_PARTS_EDIT };
	CSliderCtrl	m_Slider;
	CString	m_strCount;
	CString	m_strNow;
	CComboBox	m_cbGrpNo;
	int		m_nGrpNo;
	int		m_nLevel;
	int		m_nViewTime;
	BOOL	m_bTypeBlock;
	BOOL	m_bTypePileBack;
	BOOL	m_bTypePile;
	BOOL	m_bFishing;
	BOOL	m_bBlockDown;
	BOOL	m_bBlockLeft;
	BOOL	m_bBlockRight;
	BOOL	m_bBlockUp;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapPartsEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapPartsEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnChangeLevel();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	afx_msg void OnChangeViewtime();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnSelchangeGrpNo();
	//}}AFX_MSG
	afx_msg LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
