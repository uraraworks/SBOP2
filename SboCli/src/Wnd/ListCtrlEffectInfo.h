/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:ListCtrlEffectInfo.h										 */
/* 内容			:エフェクト一覧リストコントロールクラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/11													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CLibInfoEffect;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CListCtrlEffectInfo : public CListCtrl
{
public:
			CListCtrlEffectInfo();						/* コンストラクタ */
	virtual ~CListCtrlEffectInfo();						/* デストラクタ */


	BOOL	Create		(CWnd *pParent, CMgrData *pMgrData);	/* 作成 */
	void	Renew		(void);									/* 一覧を更新 */


protected:


protected:
	CWnd			*m_pWndParent;			/* 親ウィンドウ */
	CMgrData		*m_pMgrData;			/* データ管理 */
	CLibInfoEffect	*m_pLibInfoEffect;		/* エフェクト情報 */



public:
	//{{AFX_VIRTUAL(CListCtrlEffectInfo)
	public:
	protected:
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CListCtrlEffectInfo)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CListCtrlEffectInfo, *PCListCtrlEffectInfo;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
