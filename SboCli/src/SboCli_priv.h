/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:SboCli_priv.h												 */
/* 内容			:クライアント用定義ファイル									 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2005/05/04													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

#define MAPPARTSSIZE	(32)										/* マップパーツサイズ */
#define SCRSIZEX		(MAPPARTSSIZE * DRAW_PARTS_X)				/* マップ表示サイズ(横) */
#define SCRSIZEY		(MAPPARTSSIZE * DRAW_PARTS_Y)				/* マップ表示サイズ(縦) */
#define SCROLLSIZE		(16)										/* スクロール単位 */
#define MAXCOUNT_CHARGRPCOUNTONEFILE	(16)						/* キャラ画像の1ファイルに登録されているキャラ数 */

/* ウィンドウメッセージ */
enum {
	WM_INITEND = WM_APP,								/* 初期化完了 */
	WM_WNDCLOSE,										/* ウィンドウが閉じた */
	WM_WINDOWMSG,										/* ウィンドウからの通知 */
	WM_MGRDRAW,											/* 描画管理からの通知 */
	WM_MAINFRAME,										/* メインフレームへの通知 */
	WM_RECVCOMMAND,										/* コマンド受信 */
	WM_ADMINMSG,										/* 管理者ウィンドウ関連の通知 */
};

/* ウィンドウID */
enum {
	WINDOWID_SELECTMAPPARTSGRP = 1,						/* マップパーツ画像選択 */
	WINDOWID_SELECTMAPSHADOWGRP,						/* マップ影画像選択 */
};

/* リソースID */
enum {
	IDC_SAVEPASSWORD = 1000,							/* パスワードを記録するチェック */
	IDC_CONNECT,										/* 接続ボタン */
};

/* 描画マネージャからの通知 */
enum {
	MGRDRAWMSG_NONE = 0,
	MGRDRAWMSG_START_FADEIN,							/* フェードイン開始 */
	MGRDRAWMSG_START_FADEOUT,							/* フェードアウト開始 */
	MGRDRAWMSG_END_FADEIN,								/* フェードイン完了 */
	MGRDRAWMSG_END_FADEOUT,								/* フェードアウト完了 */
};

/* メインフレームへの通知 */
enum {
	MAINFRAMEMSG_NONE = 0,
	MAINFRAMEMSG_CHGSTATE,								/* 状態変更 */
	MAINFRAMEMSG_CONNECT,								/* 接続 */
	MAINFRAMEMSG_DISCONNECT,							/* 切断 */
	MAINFRAMEMSG_CHARNAME,								/* キャラ名入力完了 */
	MAINFRAMEMSG_RENEWACCOUNTINFO,						/* アカウント情報更新 */
	MAINFRAMEMSG_RES_MAKECHAR,							/* キャラ作成応答 */
	MAINFRAMEMSG_RENEWCHARINFO,							/* キャラ情報更新 */
	MAINFRAMEMSG_RENEWCHARCHAT,							/* 発言内容更新 */
	MAINFRAMEMSG_RENEWCHARCOUNT,						/* キャラ数更新 */
	MAINFRAMEMSG_RENEWMAPINFO,							/* マップ情報更新 */
	MAINFRAMEMSG_RENEWONLINECOUNT,						/* オンライン数更新 */
	MAINFRAMEMSG_RENEWSYSTEMMSG,						/* システムメッセージ更新 */
	MAINFRAMEMSG_RENEWADMINLEVEL,						/* 管理者権限変更 */
	MAINFRAMEMSG_RENEWITEMTYPEINFO,						/* アイテム種別情報更新 */
	MAINFRAMEMSG_RENEWITEMINFO,							/* アイテム情報更新 */
	MAINFRAMEMSG_RENEWITEMWEAPONINFO,					/* 武器情報更新 */
	MAINFRAMEMSG_RENEWMOTION,							/* モーション情報更新 */
	MAINFRAMEMSG_RENEWMOTIONTYPE,						/* モーション種別情報更新 */
	MAINFRAMEMSG_RENEWBALLOON,							/* 噴出し情報更新 */
	MAINFRAMEMSG_RENEWEFFECT,							/* エフェクト情報更新 */
	MAINFRAMEMSG_RENEWSTATUS,							/* ステータス情報更新 */
	MAINFRAMEMSG_RENEWSYSTEMINFO,						/* システム情報更新 */
	MAINFRAMEMSG_RENEWVIEWSET,							/* 表示設定更新 */
	MAINFRAMEMSG_RENEWSKILLINFO,						/* スキル情報更新 */
	MAINFRAMEMSG_RENEWTALKEVENT,						/* 会話イベント情報更新 */
	MAINFRAMEMSG_DAMAGE,								/* ダメージを受けた */
};

/* 管理者ウィンドウ関連の通知 */
enum {
	ADMINMSG_NONE = 0,
	ADMINMSG_NOTIFYTYPE_LBUTTONDOWN,					/* 左クリック通知 */
	ADMINMSG_NOTIFYTYPE_RBUTTONDOWN,					/* 右クリック通知 */
	ADMINMSG_NOTIFYTYPE_RBUTTONDBLCLK,					/* 右ダブルクリック通知 */
	ADMINMSG_RENEWMAPINFO,								/* マップ情報更新 */
	ADMINMSG_RENEWMAPOBJECT,							/* マップオブジェクト情報更新 */
	ADMINMSG_RENEWMAPPARTS,								/* マップパーツ情報更新 */
	ADMINMSG_RENEWMAPSHADOW,							/* マップ影情報更新 */
	ADMINMSG_RENEWTALKEVENT,							/* 会話イベント情報更新 */
	ADMINMSG_RENEWDISABLE,								/* 拒否情報更新 */
	ADMINMSG_ADMINLEVEL,								/* 管理者レベル通知 */
	ADMINMSG_SELECT_ANIME,								/* アニメーションコマ選択 */
	ADMINMSG_ACCOUNTINFO,								/* アカウント情報取得 */
	ADMINMSG_CHAR_ONLINE,								/* オンライン中キャラ一覧更新 */
};

/* 管理者ウィンドウへの通知種別 */
enum {
	ADMINNOTIFYTYPE_NONE = 0,
	ADMINNOTIFYTYPE_CHARID,								/* キャラID */
	ADMINNOTIFYTYPE_ACCOUNTID,							/* アカウントID */
	ADMINNOTIFYTYPE_MAPEDIT,							/* マップ編集(通知は無し) */
	ADMINNOTIFYTYPE_MAPSHADOWEDIT,						/* マップ影編集(通知は無し) */
	ADMINNOTIFYTYPE_ITEMID,								/* アイテムID */
	ADMINNOTIFYTYPE_POS,								/* マップ座標 */
	ADMINNOTIFYTYPE_CHARPOS,							/* キャラ座標 */
};

/* フェード状態 */
enum {
	FADESTATE_NONE = 0,
	FADESTATE_FADEIN,									/* フェードイン中 */
	FADESTATE_FADEOUT,									/* フェードアウト中 */
};

/* レイヤーID */
enum {
	LAYERTYPE_NONE = 0,
	LAYERTYPE_LOGO,										/* ロゴ */
	LAYERTYPE_TITLE,									/* タイトル */
	LAYERTYPE_PRODUCT,									/* プロダクト */
	LAYERTYPE_LOGINMENU,								/* ログインメニュー */
	LAYERTYPE_CHARSELECT,								/* キャラ選択 */
	LAYERTYPE_MAP,										/* マップ */
	LAYERTYPE_INFO,										/* お知らせ */
	LAYERTYPE_CLOUD,									/* 雲 */
	LAYERTYPE_SYSTEMMSG,								/* システムメッセージ */
};

/* ゲーム状態 */
enum {
	GAMESTATE_NONE = 0,
	GAMESTATE_LOGO,										/* URARA-worksロゴ */
	GAMESTATE_TITLE,									/* タイトル画面 */
	GAMESTATE_LOGIN,									/* ログイン画面 */
	GAMESTATE_DISCONNECT,								/* 切断 */
	GAMESTATE_INFO,										/* お知らせ画面 */
	GAMESTATE_LOGINMENU,								/* メニュー画面 */
	GAMESTATE_MAP,										/* マップ画面 */
};

/* 切断理由 */
enum {
	DISCONNECTID_CONNECT = 0,							/* 接続失敗 */
	DISCONNECTID_VERSION,								/* バージョン不一致 */
	DISCONNECTID_USER,									/* 満員 */
	DISCONNECTID_DISABLE,								/* 禁止 */
	DISCONNECTID_PASSWORD,								/* パスワード不一致 */
	DISCONNECTID_LOGIN,									/* ログイン済み */
	DISCONNECTID_MAC,									/* 作成済み */
};

/* ========================================================================= */
/* 構造体定義																 */
/* ========================================================================= */

/* システムメッセージ構造体 */
typedef struct _STSYSTEMMSG {
	BOOL		bAddLog;			/* ログへ追加する？ */
	CmyString	strMsg;				/* メッセージ */
	COLORREF	clMsg;				/* 描画色 */
} STSYSTEMMSG, *PSTSYSTEMMSG;

/* 数値と文字列ポインタ構造体 */
typedef struct _STINTLPCSTR {
	int			nValue;				/* 数値 */
	LPCSTR		pszText;			/* 文字列ポインタ */
} STINTLPCSTR, *PSTINTLPCSTR;

/* Copyright(C)URARA-works 2006 */
