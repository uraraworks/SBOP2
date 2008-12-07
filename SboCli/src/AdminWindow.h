/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名：	AdminWindow.h												 */
/* 内容：		管理者ウィンドウメインフレームクラス 定義ファイル			 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2006/01/25													 */
/* ========================================================================= */

#pragma once

#include "myThread.h"
#include "LayoutHelper.h"

class CMgrData;
class CWndMap;
class CDlgAdminBase;

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* 画面ID */
enum {
	SCRIDADMIN_TOP = 0,
	SCRIDADMIN_MAP_INFO,					/* マップ情報の編集 */
	SCRIDADMIN_MAP_EVENT,					/* マップイベント情報の編集 */
	SCRIDADMIN_MAP_PARTS,					/* パーツの編集 */
	SCRIDADMIN_MAP_SHADOW,					/* 影の編集 */
	SCRIDADMIN_MAP_OBJECT,					/* マップオブジェクトの編集 */
	SCRIDADMIN_MAP_OBJECTDATA,				/* マップオブジェクトの配置 */
	SCRIDADMIN_CHAR_MODIFY,					/* 選択キャラ情報編集[基本情報] */
	SCRIDADMIN_CHAR_MODIFY_STATUS,			/* 選択キャラ情報編集[ステータス] */
	SCRIDADMIN_CHAR_MODIFY_GRP,				/* 選択キャラ情報編集[画像の設定] */
	SCRIDADMIN_CHAR_MODIFY_ITEM,			/* 選択キャラ情報編集[所持アイテムの設定] */
	SCRIDADMIN_CHAR_LIST,					/* 一覧 */
	SCRIDADMIN_CHAR_ADMIN,					/* 管理者権限の設定 */
	SCRIDADMIN_CHAR_ADDNPC,					/* NPCの追加 */
	SCRIDADMIN_CHAR_MOTION,					/* モーションの編集 */
	SCRIDADMIN_CHAR_ACCOUNTINFO,			/* アカウント情報の編集 */
	SCRIDADMIN_CHAR_SKILL,					/* スキルの編集 */
	SCRIDADMIN_ITEMTYPE_LIST,				/* アイテム種別一覧 */
	SCRIDADMIN_ITEM_LIST,					/* アイテム一覧 */
	SCRIDADMIN_ITEM_WEAPONLIST,				/* 武器情報一覧 */
	SCRIDADMIN_EFC_BALLOON_LIST,			/* 噴出し一覧 */
	SCRIDADMIN_EFC_EFFECT_LIST,				/* エフェクト一覧 */
	SCRIDADMIN_SERVER_INFO,					/* サーバー情報 */
	SCRIDADMIN_SYSTEM_SET_INITCHARSTATUS,	/* キャラステータス初期値の設定 */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CAdminWindow : public CWnd, public CLayoutHelper, public CmyThread
{
public:
			CAdminWindow();								/* コンストラクタ */
	virtual ~CAdminWindow();							/* デストラクタ */


	BOOL	Create		(HWND hWndParent, CMgrData *pMgrData);	/* 作成 */
	void	Destroy		(void);									/* 破棄 */

	void	Show		(void);									/* 表示 */
	void	Hide		(void);									/* 非表示 */
	void	ChgScreen	(int nScrID);							/* 画面切り替え */


protected:
	void	ThreadMain	(void);							/* メイン処理 */


protected:
	HANDLE		m_hInitEventWindow;				/* ウィンドウ作成完了イベント */
	HWND		m_hWnd,							/* ウィンドウハンドル */
				m_hWndParent;					/* 親ウィンドウハンドル */
	int			m_nScrID;						/* 画面ID */
	CWnd		*m_pWndParent;					/* 親ウィンドウ */
	CWndMap		*m_pWndMap;						/* マップウィンドウ */
	CMenu		m_Menu;							/* メニュー */

	CMgrData			*m_pMgrData;			/* データマネージャ */
	CLayoutHelper		*m_pLayoutHelper;		/* 配置補助 */
	CDlgAdminBase		*m_pDlgBase;			/* 表示中の画面 */



public:
	DECLARE_DYNAMIC(CAdminWindow)
	//{{AFX_VIRTUAL(CAdminWindow)
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CAdminWindow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMapInfo();
	afx_msg void OnMapEvent();
	afx_msg void OnMapParts();
	afx_msg void OnMapShadow();
	afx_msg void OnMapAdd();
	afx_msg void OnMapObject();
	afx_msg void OnMapObjectData();
	afx_msg void OnMapWindow();
	afx_msg void OnCharModify();
	afx_msg void OnCharModifyStatus();
	afx_msg void OnCharModifyGrp();
	afx_msg void OnCharModifyItem();
	afx_msg void OnCharList();
	afx_msg void OnCharAdmin();
	afx_msg void OnCharAddNPC();
	afx_msg void OnCharMotion();
	afx_msg void OnCharAccountInfo();
	afx_msg void OnCharSkill();
	afx_msg void OnItemTypeList();
	afx_msg void OnItemList();
	afx_msg void OnItemWeaponList();
	afx_msg void OnEfcBalloonList();
	afx_msg void OnEfcEffectList();
	afx_msg void OnServerInfo();
	afx_msg void OnDebugMoveNoBlock();
	afx_msg void OnDebugGridOff();
	afx_msg void OnDebugGrid();
	afx_msg void OnDebugGridHalf();
	afx_msg void OnSystemSetInitCharStatus();
	afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMainFrame(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	afx_msg void OnAdminJobInfo(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
} CAdminWindow, *PCAdminWindow;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2006 */
