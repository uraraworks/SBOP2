/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemWeaponNew.h									 */
/* 内容			:アイテム武器情報設定ダイアログクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoItemWeapon;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminItemWeaponNew : public CDlgAdminBase
{
public:
			CDlgAdminItemWeaponNew(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminItemWeaponNew();							/* デストラクタ */

	void	Set		(CInfoItemWeapon *pSrc);							/* 編集内容を設定 */
	void	Get		(CInfoItemWeapon *&pDst);							/* 編集内容を取得 */


protected:
	void	SetMotionType		(DWORD dwMotionType);					/* 使用可能な攻撃モーションを設定 */
	DWORD	GetMotionType		(void);									/* 使用可能な攻撃モーションを取得 */
	void	SetMotionTypeStand	(DWORD dwMotionID);						/* 戦闘モード中の立ちモーションを設定 */
	DWORD	GetMotionTypeStand	(void);									/* 戦闘モード中の立ちモーションを取得 */
	void	SetMotionTypeWalk	(DWORD dwMotionID);						/* 戦闘モード中のすり足モーションを設定 */
	DWORD	GetMotionTypeWalk	(void);									/* 戦闘モード中のすり足モーションを取得 */
	void	RenewList			(CListBox *pList, ARRAYDWORD *pSrc);	/* リストを更新 */


protected:
	DWORD		m_dwMotionType,			/* 使用可能な攻撃モーション */
				m_dwMotionTypeStand,	/* 戦闘モード中の立ちモーション */
				m_dwMotionTypeWalk;		/* 戦闘モード中のすり足モーション */
	ARRAYDWORD	m_adwEffectIDAtack,		/* 通常攻撃時のエフェクトID */
				m_adwEffectIDCritical;	/* クリティカル時のエフェクトID */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminItemWeaponNew)
	enum { IDD = IDD_ITEMWEAPONINFO_NEW };
	CComboBox	m_ctlStandMotion;
	CComboBox	m_ctlWalkMotion;
	BOOL	m_bSwing;
	BOOL	m_bPoke;
	BOOL	m_bBow;
	BOOL	m_bBlow;
	BOOL	m_bFishing;
	CString	m_strName;
	CListBox m_ctlListNormal;
	CListBox m_ctlListCritical;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemWeaponNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemWeaponNew)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddNormal();
	afx_msg void OnBnClickedDelNormal();
	afx_msg void OnBnClickedAddCritical();
	afx_msg void OnBnClickedDelCritical();
} CDlgAdminItemWeaponNew, *PCDlgAdminItemWeaponNew;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
