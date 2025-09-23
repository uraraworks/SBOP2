/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:GlobalDefine.h												 */
/* 内容			:共通定義ファイル											 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* マクロ定義																 */
/* ========================================================================= */

#define SAFE_DELETE(p)			{ if(p) { delete (p);		(p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);		(p)=NULL; } }
#define SAFE_DESTROYWND(p)		{ if(p) { DestroyWindow(p);	(p)=NULL; } }

typedef map<DWORD, PVOID>	MapIDPtr;

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

#define WNDTITLE				"Scrap Book Online Page.2"			/* ウィンドウタイトル */
#define DRAW_PARTS_X			(15)								/* マップ表示数(横) */
#define DRAW_PARTS_Y			(15)								/* マップ表示数(縦) */
#define MOVEWAIT				(11)								/* 移動周期 */
#define BATTLEMOVEWAIT			(15)								/* 移動周期(戦闘中) */

#define MAXLEN_ACCOUNT			(10)								/* 最大アカウント名長 */
#define MAXLEN_PASSWORD			(10)								/* 最大パスワード長 */
#define MAXLEN_CHARNAME			(15)								/* 最大キャラ名長 */
#define URARASOCK_PRECHECK		(0x56BB3E5E)						/* チェック用キーワード	 */
#define URARASOCK_MSGBASE		(WM_APP + 200)						/* 通信ライブラリ通知メッセージ基準 */

/* キャラグラフィックIDメイン */
enum {
	CHARGRPMAIN_NONE = 0,
	CHARGRPMAIN_3232,
};

/* キャラグラフィックIDサブ */
enum {
	CHARGRPSUB_NONE = 0,
	CHARGRPSUB_INIT_MALE,							/* 初期キャラ(男) */
	CHARGRPSUB_INIT_FEMALE,							/* 初期キャラ(女) */
};

/* 性別による種別 */
enum {
	SEX_ALL = 0,									/* 全員 */
	SEX_MALE,										/* 男性 */
	SEX_FEMALE,										/* 女性 */
	SEX_MAX
};

/* 種族 */
enum {
	FAMILYTYPE_NONE = 0,
	FAMILYTYPE_HUMAN,								/* ニンゲン */
	FAMILYTYPE_ELF,									/* エルフ */
	FAMILYTYPE_BST,									/* ジュウジン */
	FAMILYTYPE_DAEMON,								/* マゾク */
	FAMILYTYPE_MAX
};

/* 管理者レベル */
enum {
	ADMINLEVEL_NONE = 0,							/* 一般 */
	ADMINLEVEL_ALL									/* 全権限 */
};

/* マーク名 */
enum {
	MARKID_NONE = 0,
	MARKID_BEGINNER,								/* 初心者マーク */
	MARKID_ADMIN_URARA,								/* うららんマーク */
	MARKID_ADMIN_VELTINA,							/* べるちなマーク */
	MARKID_ADMIN,									/* 管理者マーク */
	MARKID_NAZO,									/* よくわからないマーク（ぇ */
	MARKID_PT_MEMBER,								/* ＰＴメンバ */
	MARKID_PT_LEADER,								/* ＰＴリーダー */
	MARKID_PT_RECRUIT,								/* PT募集中 */
	MARKID_SHOP,									/* 露店？ */
	MARKID_GUILD_LEADER,							/* ギルドリーダー */
	MARKID_GUILD_MEMBER,							/* ギルドメンバ */
	MARKID_WALLET,									/* 用途未定がまぐちサイフ */
};

/* 武器種別 */
enum {
	WEAPONTYPE_SWING = 0,							/* 振る */
	WEAPONTYPE_POKE,								/* 突く */
	WEAPONTYPE_BOW,									/* 弓 */
};

/* 装備の種類 */
enum {
	EQUIPTYPE_ACCE1 = 0,	/* アクセサリ1 */
	EQUIPTYPE_HEAD,			/* 頭 */
	EQUIPTYPE_ACCE2,		/* アクセサリ2 */
	EQUIPTYPE_ARMSRIGHT,	/* 右手 */
	EQUIPTYPE_CLOTH,		/* 服 */
	EQUIPTYPE_ARMSLEFT,		/* 左手 */
	EQUIPTYPE_MAX
};

/* 画像種別ID */
enum {
	GRPIDMAIN_NONE = 0,
	GRPIDMAIN_CHAR,			/* キャラ */
	GRPIDMAIN_WEAPON,		/* 武器 */
	GRPIDMAIN_WEAPON_BOW,	/* 弓 */
	GRPIDMAIN_EFFECT32,		/* エフェクト(32) */
	GRPIDMAIN_WEAPON_GLOVE,	/* 武器(打撃) */
	GRPIDMAIN_WEAPON_ETC,	/* 武器(その他) */
	GRPIDMAIN_NPC,			/* キャラ(NPC) */
	GRPIDMAIN_EFCBALLOON,	/* 噴出し */
	GRPIDMAIN_2X2_CHAR,		/* キャラ(2x2) */
	GRPIDMAIN_2X2_CLOTH,	/* 服(2x2) */
	GRPIDMAIN_2X2_EYE,		/* 目(2x2) */
	GRPIDMAIN_2X2_HAIR,		/* 髪(2x2) */
	GRPIDMAIN_2X2_SPCLOTH,	/* 特殊服(2x2) */
	GRPIDMAIN_2X2_SPHAIR,	/* 特殊髪(2x2) */
	GRPIDMAIN_2X2_ARMS,		/* 持ち物(2x2) */
	GRPIDMAIN_2X2_SHIELD,	/* 盾(2x2) */
	GRPIDMAIN_2X2_ARMSSP,	/* 特殊持ち物(2x2) */
	GRPIDMAIN_2X2_BOW,		/* 弓(2x2) */
	GRPIDMAIN_2X2_NPC,		/* NPC(2x2) */
	GRPIDMAIN_EFFECT64,		/* エフェクト(64) */
	GRPIDMAIN_ICON32,		/* アイコン(２倍表示) */
	GRPIDMAIN_MAX
};

/* 使用対象 */
enum {
	CHARTARGET_ALLY = 0,	/* 味方 */
	CHARTARGET_ENEMY,		/* 敵 */
};

/* 攻撃対象 */
enum {
	ATACKTARGETTYPE_NONE = 0,		/* 未設定 */
	ATACKTARGETTYPE_PC,				/* プレイヤー */
	ATACKTARGETTYPE_NPC,			/* NPC */
	ATACKTARGETTYPE_ALL,			/* 全て */
	ATACKTARGETTYPE_MAX
};

/* 使用範囲 */
enum {
	CHARAREA_SELF = 0,		/* 自分 */
	CHARAREA_FRONT,			/* 1歩前 */
	CHARAREA_AROUND,		/* 周囲 */
	CHARAREA_SCREEN,		/* 画面内 */
};


/* ========================================================================= */
/* 構造体定義																 */
/* ========================================================================= */


/* ========================================================================= */
/* 関数名	:FreeInfo														 */
/* 内容		:情報を開放														 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

template<class PARRAY, class PINFO>
void FreeInfo(PARRAY pArray, PINFO pInfo)
{
	int i, nCount;

	if (pArray) {
		nCount = pArray->size();
		for (i = nCount - 1; i >= 0; i --) {
			pInfo = pArray->at(i);
			SAFE_DELETE (pInfo);
		}
		pArray->clear();
	}
}

/* Copyright(C)URARA-works 2006 */
