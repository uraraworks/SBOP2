/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:Command.h													 */
/* 内容			:コマンド定義ファイル										 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* コマンドID(メイン) */
enum {
	SBOCOMMANDID_MAIN_NONE = 0,
	SBOCOMMANDID_MAIN_VERSION,					/* バージョン系 */
	SBOCOMMANDID_MAIN_CONNECT,					/* 接続系 */
	SBOCOMMANDID_MAIN_ACCOUNT,					/* アカウント系 */
	SBOCOMMANDID_MAIN_CHAR,						/* キャラ系 */
	SBOCOMMANDID_MAIN_MAP,						/* マップ系 */
	SBOCOMMANDID_MAIN_ADMIN,					/* 管理者系 */
	SBOCOMMANDID_MAIN_MSGCMD,					/* メッセージコマンド系 */
	SBOCOMMANDID_MAIN_ITEM,						/* アイテム系 */
	SBOCOMMANDID_MAIN_EFFECT,					/* エフェクト系 */
	SBOCOMMANDID_MAIN_BATTLE,					/* 戦闘系 */
	SBOCOMMANDID_MAIN_SYSTEM,					/* システム系 */
	SBOCOMMANDID_MAIN_SKILL,					/* スキル系 */

	SBOCOMMANDID_MAIN_MAX
};

/* コマンドID(バージョン系) */
enum {
	SBOCOMMANDID_SUB_VERSION_NONE = 0,
	SBOCOMMANDID_SUB_VERSION_REQ_VERSIONCHECK,	/* バージョンチェック要求 */
	SBOCOMMANDID_SUB_VERSION_RES_VERSIONCHECK,	/* バージョンチェック応答 */
	SBOCOMMANDID_SUB_VERSION_REQ_FILELISTCHECK,	/* ファイルリストチェック要求 */
	SBOCOMMANDID_SUB_VERSION_RES_FILELISTCHECK,	/* ファイルリストチェック応答 */
	SBOCOMMANDID_SUB_VERSION_REQ_FILELIST,		/* ファイルリスト要求 */
	SBOCOMMANDID_SUB_VERSION_RES_FILELIST,		/* ファイルリスト応答 */
	SBOCOMMANDID_SUB_VERSION_REQ_FILE,			/* ファイル要求 */
	SBOCOMMANDID_SUB_VERSION_RES_FILE,			/* ファイル応答 */
	SBOCOMMANDID_SUB_VERSION_MAX
};

/* コマンドID(接続系) */
enum {
	SBOCOMMANDID_SUB_CONNECT_NONE = 0,
	SBOCOMMANDID_SUB_CONNECT_REQ_LOGIN,			/* ログイン要求 */
	SBOCOMMANDID_SUB_CONNECT_RES_LOGIN,			/* ログイン応答 */
	SBOCOMMANDID_SUB_CONNECT_REQ_PLAY,			/* ゲーム開始要求 */
	SBOCOMMANDID_SUB_CONNECT_RES_PLAY,			/* ゲーム開始応答 */
	SBOCOMMANDID_SUB_CONNECT_KEEPALIVE,			/* 生存確認通知 */
};

/* コマンドID(アカウント系) */
enum {
	SBOCOMMANDID_SUB_ACCOUNT_NONE = 0,
	SBOCOMMANDID_SUB_ACCOUNT_REQ_ACCOUNTINFO,	/* アカウント情報要求 */
	SBOCOMMANDID_SUB_ACCOUNT_RES_ACCOUNTINFO,	/* アカウント情報応答 */
	SBOCOMMANDID_SUB_ACCOUNT_REQ_MAKECHAR,		/* キャラ作成要求 */
	SBOCOMMANDID_SUB_ACCOUNT_RES_MAKECHAR,		/* キャラ作成応答 */
};

/* コマンドID(キャラ系) */
enum {
	SBOCOMMANDID_SUB_CHAR_NONE = 0,
	SBOCOMMANDID_SUB_CHAR_REQ_CHARINFO,			/* キャラ情報要求 */
	SBOCOMMANDID_SUB_CHAR_RES_CHARINFO,			/* キャラ情報応答 */
	SBOCOMMANDID_SUB_CHAR_CHARINFO,				/* キャラ情報通知 */
	SBOCOMMANDID_SUB_CHAR_MOVEPOS,				/* 移動通知 */
	SBOCOMMANDID_SUB_CHAR_STATE,				/* 状態通知 */
	SBOCOMMANDID_SUB_CHAR_PROCSTATE,			/* 行動状態通知 */
	SBOCOMMANDID_SUB_CHAR_REQ_CHAT,				/* チャット要求 */
	SBOCOMMANDID_SUB_CHAR_CHAT,					/* チャット通知 */
	SBOCOMMANDID_SUB_CHAR_CHARID,				/* キャラID通知 */
	SBOCOMMANDID_SUB_CHAR_REQ_CHARINFO2,		/* キャラ情報要求(複数) */
	SBOCOMMANDID_SUB_CHAR_REQ_PUTGET,			/* アイテムを拾う置く要求 */
	SBOCOMMANDID_SUB_CHAR_RES_PUTGET,			/* アイテムを拾う置く応答 */
	SBOCOMMANDID_SUB_CHAR_REQ_USEITEM,			/* アイテムを使う要求 */
	SBOCOMMANDID_SUB_CHAR_REQ_DRAGITEM,			/* アイテム位置変更要求 */
	SBOCOMMANDID_SUB_CHAR_SET_EFFECT,			/* エフェクトを指定 */
	SBOCOMMANDID_SUB_CHAR_ITEMINFO,				/* アイテム情報通知 */
	SBOCOMMANDID_SUB_CHAR_REQ_PUSH,				/* 押す要求 */
	SBOCOMMANDID_SUB_CHAR_REQ_TAIL,				/* 付いて行く要求 */
	SBOCOMMANDID_SUB_CHAR_RES_TAIL,				/* 付いて行く応答 */
	SBOCOMMANDID_SUB_CHAR_REQ_MODIFY_PARAM,		/* パラメータの変更要求 */
	SBOCOMMANDID_SUB_CHAR_MODIFY_PARAM,			/* パラメータの変更通知 */
	SBOCOMMANDID_SUB_CHAR_REQ_EQUIP,			/* 装備要求 */
	SBOCOMMANDID_SUB_CHAR_GRP,					/* 画像通知 */
	SBOCOMMANDID_SUB_CHAR_MOTION,				/* モーション情報通知 */
	SBOCOMMANDID_SUB_CHAR_MOTIONTYPE,			/* モーション種別情報通知 */
	SBOCOMMANDID_SUB_CHAR_SET_EFCBALLOON,		/* 噴出しを指定 */
	SBOCOMMANDID_SUB_CHAR_PROC_FISHING,			/* 釣り要求 */
	SBOCOMMANDID_SUB_CHAR_SET_MOTION,			/* モーションを指定 */
	SBOCOMMANDID_SUB_CHAR_REQ_CHECKMAPEVENT,	/* マップイベントチェック要求 */
	SBOCOMMANDID_SUB_CHAR_RES_CHECKMAPEVENT,	/* マップイベントチェック応答 */
	SBOCOMMANDID_SUB_CHAR_STATUS,				/* ステータス情報通知 */
	SBOCOMMANDID_SUB_CHAR_TEXTEFFECT,			/* 文字エフェクト通知 */
	SBOCOMMANDID_SUB_CHAR_STATE_CHARGE,			/* 溜め状態通知 */
	SBOCOMMANDID_SUB_CHAR_REQ_RECOVERY,			/* 気絶後復活要求 */
	SBOCOMMANDID_SUB_CHAR_REQ_TALKEVENT,		/* 会話イベント情報要求 */
	SBOCOMMANDID_SUB_CHAR_RES_TALKEVENT,		/* 会話イベント情報応答 */
	SBOCOMMANDID_SUB_CHAR_REQ_ADDSKILL,			/* スキル追加要求 */
	SBOCOMMANDID_SUB_CHAR_SKILLINFO,			/* スキル情報通知 */
	SBOCOMMANDID_SUB_CHAR_REQ_USESKILL,			/* スキルを使う要求 */
};

/* コマンドID(マップ系) */
enum {
	SBOCOMMANDID_SUB_MAP_NONE = 0,
	SBOCOMMANDID_SUB_MAP_REQ_MAPINFO,			/* マップ情報要求 */
	SBOCOMMANDID_SUB_MAP_RES_MAPINFO,			/* マップ情報応答 */
	SBOCOMMANDID_SUB_MAP_ONLINE,				/* オンライン数通知 */
	SBOCOMMANDID_SUB_MAP_SYSTEMMSG,				/* システムメッセージ通知 */
	SBOCOMMANDID_SUB_MAP_FORMATMSG,				/* フォーマットメッセージ通知 */
	SBOCOMMANDID_SUB_MAP_MAPOBJECT,				/* マップオブジェクト情報通知 */
	SBOCOMMANDID_SUB_MAP_DELETEMAPOBJECT,		/* マップオブジェクト削除 */
	SBOCOMMANDID_SUB_MAP_MAPOBJECTDATA,			/* マップオブジェクト配置データ通知 */
	SBOCOMMANDID_SUB_MAP_DELETEMAPOBJECTDATA,	/* マップオブジェクト配置データ削除 */
	SBOCOMMANDID_SUB_MAP_MAPPARTS,				/* マップパーツ情報通知 */
	SBOCOMMANDID_SUB_MAP_SETPARTS,				/* マップパーツ配置 */
	SBOCOMMANDID_SUB_MAP_RENEWMAPSIZE,			/* マップサイズ更新 */
	SBOCOMMANDID_SUB_MAP_DELETEPARTS,			/* マップパーツ削除 */
	SBOCOMMANDID_SUB_MAP_MAPSHADOW,				/* マップ影情報通知 */
	SBOCOMMANDID_SUB_MAP_SETMAPSHADOW,			/* マップ影配置 */
	SBOCOMMANDID_SUB_MAP_DELETEMAPSHADOW,		/* マップ影削除 */
	SBOCOMMANDID_SUB_MAP_MAPNAME,				/* マップ名通知 */
	SBOCOMMANDID_SUB_MAP_MAPEVENT,				/* マップイベント情報通知 */
	SBOCOMMANDID_SUB_MAP_DELETEEVENT,			/* マップイベント情報削除 */
	SBOCOMMANDID_SUB_MAP_FADEINOUT,				/* フェードイン/アウト通知 */
};

/* コマンドID(管理者系) */
enum {
	SBOCOMMANDID_SUB_ADMIN_NONE = 0,
	SBOCOMMANDID_SUB_ADMIN_CHARINFO,				/* キャラ情報更新 */
	SBOCOMMANDID_SUB_ADMIN_DELETECHARINFO,			/* キャラ情報削除 */
	SBOCOMMANDID_SUB_ADMIN_MAKEADMINWINDOW,			/* 管理者ウィンドウ作成通知 */
	SBOCOMMANDID_SUB_ADMIN_MAP_RENEWMAPOBJECT,		/* マップオブジェクト更新 */
	SBOCOMMANDID_SUB_ADMIN_MAP_DELETEMAPOBJECT,		/* マップオブジェクト削除 */
	SBOCOMMANDID_SUB_ADMIN_MAP_RENEWOBJECTDATA,		/* マップオブジェクト配置データ更新 */
	SBOCOMMANDID_SUB_ADMIN_MAP_DELETEOBJECTDATA,	/* マップオブジェクト配置データ削除 */
	SBOCOMMANDID_SUB_ADMIN_RENEWMAPPARTS,			/* マップパーツ更新 */
	SBOCOMMANDID_SUB_ADMIN_MAP_SETPARTS,			/* マップパーツ配置 */
	SBOCOMMANDID_SUB_ADMIN_MAP_RENEWMAPSIZE,		/* マップサイズ更新 */
	SBOCOMMANDID_SUB_ADMIN_MAP_DELETEPARTS,			/* マップパーツ削除 */
	SBOCOMMANDID_SUB_ADMIN_MAP_COPYPARTS,			/* マップパーツコピー */
	SBOCOMMANDID_SUB_ADMIN_MAP_SETMAPNAME,			/* マップ名変更 */
	SBOCOMMANDID_SUB_ADMIN_RENEWMAPSHADOW,			/* マップ影更新 */
	SBOCOMMANDID_SUB_ADMIN_MAP_SETMAPSHADOW,		/* マップ影配置 */
	SBOCOMMANDID_SUB_ADMIN_MAP_DELETEMAPSHADOW,		/* マップ影削除 */
	SBOCOMMANDID_SUB_ADMIN_MAP_RENEWEVENT,			/* マップイベント情報更新 */
	SBOCOMMANDID_SUB_ADMIN_MAP_DELETEEVENT,			/* マップイベント情報削除 */
	SBOCOMMANDID_SUB_ADMIN_MAP_ADD,					/* マップ追加 */
	SBOCOMMANDID_SUB_ADMIN_REQ_ADMINLEVEL,			/* 管理者権限レベル要求 */
	SBOCOMMANDID_SUB_ADMIN_RES_ADMINLEVEL,			/* 管理者権限レベル応答 */
	SBOCOMMANDID_SUB_ADMIN_RENEWADMINLEVEL,			/* 管理者権限レベル更新 */
	SBOCOMMANDID_SUB_ADMIN_SERVER_SAVEINFO,			/* サーバー情報保存 */
	SBOCOMMANDID_SUB_ADMIN_ITEM_ADD,				/* アイテム情報追加 */
	SBOCOMMANDID_SUB_ADMIN_ITEM_COPY,				/* アイテム情報コピー */
	SBOCOMMANDID_SUB_ADMIN_ITEM_DELETE,				/* アイテム情報削除 */
	SBOCOMMANDID_SUB_ADMIN_ITEMTYPE_ADD,			/* アイテム種別情報追加 */
	SBOCOMMANDID_SUB_ADMIN_ITEMTYPE_COPY,			/* アイテム種別情報コピー */
	SBOCOMMANDID_SUB_ADMIN_ITEMTYPE_DELETE,			/* アイテム種別情報削除 */
	SBOCOMMANDID_SUB_ADMIN_ITEMWEAPON_ADD,			/* 武器情報追加 */
	SBOCOMMANDID_SUB_ADMIN_ITEMWEAPON_COPY,			/* 武器情報コピー */
	SBOCOMMANDID_SUB_ADMIN_ITEMWEAPON_RENEW,		/* 武器情報更新 */
	SBOCOMMANDID_SUB_ADMIN_ITEMWEAPON_DELETE,		/* 武器情報削除 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_ADDNPC,				/* NPCの追加 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_MODIFYITEM,			/* 所持アイテムの変更 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_MODIFYSKILL,		/* 所持スキルの変更 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWMOTION,		/* キャラモーション情報の更新 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_ADDMOTIONTYPE,		/* キャラモーション種別情報の追加 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWMOTIONTYPE,	/* キャラモーション種別情報の更新 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWSTATUS,		/* ステータス情報更新 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_STATUS,			/* ステータス情報要求 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_ONLINE,			/* オンライン中キャラ一覧要求 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_RES_ONLINE,			/* オンライン中キャラ一覧応答 */
	SBOCOMMANDID_SUB_ADMIN_EFC_RENEWBALLOON,		/* 噴出し情報の更新 */
	SBOCOMMANDID_SUB_ADMIN_EFC_RENEWEFFECT,			/* エフェクト情報の更新 */
	SBOCOMMANDID_SUB_ADMIN_REQ_PLAYSOUND,			/* 効果音の再生要求 */
	SBOCOMMANDID_SUB_ADMIN_PLAYSOUND,				/* 効果音の再生 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_ACCOUNT,		/* アカウント情報要求 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_RES_ACCOUNT,		/* アカウント情報応答 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_RENEW_ACCOUNT,		/* アカウント情報更新 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_RENEW_TALKEVENT,	/* 会話イベント情報更新 */
	SBOCOMMANDID_SUB_ADMIN_SKILL_RENEWSKILL,		/* スキル情報更新 */
	SBOCOMMANDID_SUB_ADMIN_RENEW_CLIENTVERSION,		/* クライアントバージョン更新 */
	SBOCOMMANDID_SUB_ADMIN_SYSTEM_REQ_INFO,			/* システム情報要求 */
	SBOCOMMANDID_SUB_ADMIN_SYSTEM_RENEWINFO,		/* システム情報の更新 */
	SBOCOMMANDID_SUB_ADMIN_ACCOUNT_REQ_ADD,			/* アカウントの追加要求 */
	SBOCOMMANDID_SUB_ADMIN_DISABLE_REQ_INFO,		/* 拒否情報要求 */
	SBOCOMMANDID_SUB_ADMIN_DISABLE_RES_INFO,		/* 拒否情報応答 */
	SBOCOMMANDID_SUB_ADMIN_DISABLE_REQ_DELETE,		/* 拒否情報の削除要求 */
	SBOCOMMANDID_SUB_ADMIN_DISABLE_RENEWINFO,		/* 拒否情報の更新 */
};

/* コマンドID(メッセージコマンド系) */
enum {
	SBOCOMMANDID_SUB_MSGCMD_NONE = 0,
	SBOCOMMANDID_SUB_MSGCMD_CHGFACE,			/* 表情変更 */
	SBOCOMMANDID_SUB_MSGCMD_CHGHAIR,			/* 髪変更 */
	SBOCOMMANDID_SUB_MSGCMD_CHGCLOTH,			/* 服装変更 */
	SBOCOMMANDID_SUB_MSGCMD_CHGACCE,			/* アクセサリ変更 */
	SBOCOMMANDID_SUB_MSGCMD_CHGCOLOR,			/* 色変更 */
	SBOCOMMANDID_SUB_MSGCMD_CHGARMS,			/* 持ち物変更 */
	SBOCOMMANDID_SUB_MSGCMD_CHGSHIELD,			/* 盾変更 */
	SBOCOMMANDID_SUB_MSGCMD_MAKEITEM,			/* アイテム作成 */
	SBOCOMMANDID_SUB_MSGCMD_BALLOON,			/* 噴出し */
	SBOCOMMANDID_SUB_MSGCMD_DICE,				/* サイコロ */
	SBOCOMMANDID_SUB_MSGCMD_RND,				/* ランダム */
	SBOCOMMANDID_SUB_MSGCMD_EFFECT,				/* エフェクト */
	SBOCOMMANDID_SUB_MSGCMD_WHERE,				/* 最も集まっている場所 */
};

/* コマンドID(アイテム系) */
enum {
	SBOCOMMANDID_SUB_ITEM_NONE = 0,
	SBOCOMMANDID_SUB_ITEM_REQ_ITEMINFO,			/* アイテム情報要求 */
	SBOCOMMANDID_SUB_ITEM_RES_ITEMINFO,			/* アイテム情報応答 */
	SBOCOMMANDID_SUB_ITEM_ITEMINFO,				/* アイテム情報通知 */
	SBOCOMMANDID_SUB_ITEM_DELETEITEMINFO,		/* アイテム情報削除 */
	SBOCOMMANDID_SUB_ITEM_RENEWITEMINFO,		/* アイテム情報更新 */
	SBOCOMMANDID_SUB_ITEM_REQ_ITEMTYPEINFO,		/* アイテム種別情報要求 */
	SBOCOMMANDID_SUB_ITEM_RES_ITEMTYPEINFO,		/* アイテム種別情報応答 */
	SBOCOMMANDID_SUB_ITEM_ITEMTYPEINFO,			/* アイテム情報種別通知 */
	SBOCOMMANDID_SUB_ITEM_DELETEITEMTYPEINFO,	/* アイテム情報種別削除 */
	SBOCOMMANDID_SUB_ITEM_RENEWITEMTYPEINFO,	/* アイテム情報種別更新 */
	SBOCOMMANDID_SUB_ITEM_REQ_ITEMWEAPONINFO,	/* 武器情報要求 */
	SBOCOMMANDID_SUB_ITEM_RES_ITEMWEAPONINFO,	/* 武器情報応答 */
	SBOCOMMANDID_SUB_ITEM_ITEMWEAPONINFO,		/* 武器情報通知 */
};

/* コマンドID(エフェクト系) */
enum {
	SBOCOMMANDID_SUB_EFFECT_NONE = 0,
	SBOCOMMANDID_SUB_EFFECT_REQ_EFFECTINFO,		/* エフェクト情報要求 */
	SBOCOMMANDID_SUB_EFFECT_RES_EFFECTINFO,		/* エフェクト情報応答 */
	SBOCOMMANDID_SUB_EFFECT_EFFECTINFO,			/* エフェクト情報通知 */
	SBOCOMMANDID_SUB_EFFECT_DELETEEFFECTINFO,	/* エフェクト情報削除 */
	SBOCOMMANDID_SUB_EFFECT_REQ_BALLOONINFO,	/* 噴出し情報要求 */
	SBOCOMMANDID_SUB_EFFECT_BALLOONINFO,		/* 噴出し情報通知 */
	SBOCOMMANDID_SUB_EFFECT_DELETEBALLOONINFO,	/* 噴出し情報削除 */
};

/* コマンドID(戦闘系) */
enum {
	SBOCOMMANDID_SUB_BATTLE_NONE = 0,
	SBOCOMMANDID_SUB_BATTLE_REQ_ATACK,			/* 攻撃要求 */
};

/* コマンドID(システム系) */
enum {
	SBOCOMMANDID_SUB_SYSTEM_NONE = 0,
	SBOCOMMANDID_SUB_SYSTEM_INFO,				/* システム情報通知 */
};

/* コマンドID(スキル系) */
enum {
	SBOCOMMANDID_SUB_SKILL_NONE = 0,
	SBOCOMMANDID_SUB_SKILL_REQ_SKILLINFO,		/* スキル情報要求 */
	SBOCOMMANDID_SUB_SKILL_SKILLINFO,			/* スキル情報通知 */
	SBOCOMMANDID_SUB_SKILL_DELETESKILLINFO,		/* スキル情報削除 */
};

/* Copyright(C)URARA-works 2006 */
