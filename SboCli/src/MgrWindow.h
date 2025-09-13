/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MgrWindow.h												 */
/* 内容			:ウィンドウマネージャクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/03													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;
class CMgrKeyInput;
class CInfoTalkEvent;

/* ========================================================================= */
/* 定数の定義																 */
/* ========================================================================= */

/* ウィンドウID */
enum {
	WINDOWTYPE_NONE = 0,
	WINDOWTYPE_LOGIN,						/* ログイン */
	WINDOWTYPE_MSG,							/* メッセージ表示 */
	WINDOWTYPE_LOGINMENU,					/* メニュー */
	WINDOWTYPE_CHARNAME,					/* キャラ名入力 */
	WINDOWTYPE_SEX,							/* 性別 */
	WINDOWTYPE_MAKECHAR,					/* キャラ作成 */
	WINDOWTYPE_ACCOUNTINFO,					/* アカウント情報 */
	WINDOWTYPE_STATUS,						/* 強さ */
	WINDOWTYPE_CHAT,						/* チャット入力 */
	WINDOWTYPE_FAMILYTYPE,					/* 種族選択 */
	WINDOWTYPE_HAIRTYPE,					/* 髪型選択 */
	WINDOWTYPE_HAIRCOLOR,					/* 髪色選択 */
	WINDOWTYPE_EYECOLOR,					/* 目色選択 */
	WINDOWTYPE_STYLESELECT,					/* 容姿選択 */
	WINDOWTYPE_NAMEINPUT,					/* 名前入力 */
	WINDOWTYPE_SYSTEMMENU,					/* システムメニュー */
	WINDOWTYPE_SETCOLOR,					/* 名前と発言色の設定 */
	WINDOWTYPE_SETBGMVOLUME,				/* BGM音量の設定 */
	WINDOWTYPE_SETSOUNDVOLUME,				/* 効果音音量の設定 */
	WINDOWTYPE_SETDRAWMODE,					/* 表示内容の設定 */
	WINDOWTYPE_ITEMMENU,					/* アイテムメニュー */
	WINDOWTYPE_ITEMMENU_SELECT,				/* アイテムどうするかメニュー */
	WINDOWTYPE_OPTION,						/* オプションメニュー */
	WINDOWTYPE_OPTION_VIEWSET,				/* オプション-表示設定メニュー */
	WINDOWTYPE_OPTION_VOLUMESET,			/* オプション-音量設定メニュー */
	WINDOWTYPE_OPTION_INPUTSET,				/* オプション-入力設定メニュー */
	WINDOWTYPE_OPTION_TASKBAR,				/* オプションメニュー(発言時にチカチカ) */
	WINDOWTYPE_OPTION_INPUTSET_SETDEVICE,	/* オプション-入力設定-入力デバイスの設定 */
	WINDOWTYPE_OPTION_ACTIONSET,			/* オプション-動作設定メニュー */
	WINDOWTYPE_OPTION_ACTIONSET_SLEEPTIMER,	/* オプション-動作設定-おひるねタイマーの設定 */
	WINDOWTYPE_COMMANDMENU,					/* コマンドメニュー */
	WINDOWTYPE_CHAR_STATUS,					/* キャラ-ステータス */
	WINDOWTYPE_CHAR_STATUS2,				/* キャラ-ステータス2 */
	WINDOWTYPE_CHAR_STATUS3,				/* キャラ-ステータス3 */
	WINDOWTYPE_CHAR_STATUS4,				/* キャラ-ステータス4 */
	WINDOWTYPE_TEXTMSG,						/* テキストメッセージ */
	WINDOWTYPE_SWOON,						/* 気絶メニュー */
	WINDOWTYPE_SKILLMENU,					/* スキルメニュー */
	WINDOWTYPE_PLACEINFORMATION,			/* 場所情報 */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CMgrWindow
{
public:
			CMgrWindow();								/* コンストラクタ */
	virtual ~CMgrWindow();								/* デストラクタ */

	void		Create		(CMgrData *pMgrData);				/* 作成 */
	void		Destroy		(void);								/* 破棄 */

	void		Update		(void);								/* 全てのウィンドウを更新 */
	void		Draw		(CImg32 *pDst);						/* 描画 */
	BOOL		TimerProc	(void);								/* 時間処理 */
	void		KeyProc		(BYTE byEvent, BOOL bDown);			/* キー処理 */
	void		DeleteAll	(void);								/* 全てのウィンドウを破棄 */
	void		Delete		(int nID);							/* 指定IDのウィンドウを削除 */
	CWindowBase	*GetWindow	(int nID);							/* 指定IDのウィンドウを取得 */
	BOOL		IsKeyInput	(void);								/* キー処理の必要があるか判定 */

	/* ウィンドウ作成 */
	void MakeWindowLOGIN			(void);						/* ログイン情報 */
	void MakeWindowMSG				(LPCSTR pszMsg, DWORD dwTime = 0, int nType = 0);	/* メッセージ表示 */
	void MakeWindowLOGINMENU		(void);						/* メニュー */
	void MakeWindowCHARNAME			(void);						/* キャラ名入力 */
	void MakeWindowSEX				(void);						/* 性別 */
	void MakeWindowACCOUNTINFO		(void);						/* アカウント情報 */
	void MakeWindowSTATUS			(BOOL bShow = TRUE);		/* 強さ */
	void MakeWindowCHAT				(void);						/* チャット入力 */
	void MakeWindowFAMILYTYPE		(void);						/* 種族選択 */
	void MakeWindowHAIRTYPE			(void);						/* 髪型選択 */
	void MakeWindowHAIRCOLOR		(void);						/* 髪色選択 */
	void MakeWindowEYECOLOR			(void);						/* 目色選択 */
	void MakeWindowSTYLESELECT		(void);						/* 容姿選択 */
	void MakeWindowNAMEINPUT		(void);						/* 名前入力 */
	void MakeWindowSYSTEMMENU		(void);						/* システムメニュー */
	void MakeWindowSETCOLOR			(void);						/* 名前と発言色の設定 */
	void MakeWindowSETBGMVOLUME		(void);						/* BGM音量の設定 */
	void MakeWindowSETSOUNDVOLUME	(void);						/* 効果音音量の設定 */
	void MakeWindowSETDRAWMODE		(void);						/* 表示内容の設定 */
	void MakeWindowITEMMENU			(void);						/* アイテムメニュー */
	void MakeWindowITEMMENU_SELECT	(int nPos, DWORD dwItemID);	/* アイテムどうするかメニュー */
	void MakeWindowOPTION			(void);						/* オプション */
	void MakeWindowOPTION_VIEWSET	(void);						/* オプション-表示設定 */
	void MakeWindowOPTION_VOLUMESET	(void);						/* オプション-音量設定 */
	void MakeWindowOPTION_INPUTSET	(void);						/* オプション-入力設定 */
	void MakeWindowOPTION_TASKBAR	(void);						/* オプション(発言時にタスクバーチカチカ) */
	void MakeWindowOPTION_INPUTSET_SETDEVICE	(void);			/* オプション-入力設定-入力デバイスの設定 */
	void MakeWindowOPTION_ACTIONSET	(void);						/* オプション-動作設定 */
	void MakeWindowOPTION_ACTIONSET_SLEEPTIMER	(void);			/* オプション-動作設定-おひるねタイマーの設定 */
	void MakeWindowCOMMANDMENU		(void);						/* コマンドメニュー */
	void MakeWindowCHAR_STATUS		(void);						/* キャラ-ステータス */
	void MakeWindowCHAR_STATUS4		(void);						/* キャラ-ステータス4 */
	void MakeWindowTEXTMSG			(LPCSTR pszTitle, LPCSTR pszName, LPCSTR pszMsg);			/* テキストメッセージ */
	void MakeWindowTEXTMSG			(LPCSTR pszTitle, LPCSTR pszName, CInfoTalkEvent *pInfo);	/* テキストメッセージ */
	void MakeWindowSWOON			(void);						/* 気絶メニュー */
	void MakeWindowSKILLMENU		(int nType = -1);			/* スキルメニュー */
	void MakeWindowPLACEINFORMATION	(void);						/* 場所情報 */


private:
	void SetActive				(void);								/* アクティブなウィンドウの設定 */


private:
	BOOL				m_bDraw,						/* 描画フラグ */
						m_bKeyInput;					/* キー処理の必要があるか */
	PARRAYWINDOWBASE	m_paWindow;						/* ウィンドウ情報 */
	CMgrData			*m_pMgrData;					/* データ管理 */
	CMgrGrpData			*m_pMgrGrpData;					/* グラフィックデータ管理 */
	CMgrKeyInput		*m_pMgrKeyInput;				/* キー入力管理 */
} CMgrWindow, *PCMgrWindow;

/* Copyright(C)URARA-works 2006 */
