#pragma once

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */
namespace InfoItemType {

/* アイテム種別 */
enum {
	TYPEID_NONE = 0,				/* 効果無し */
	TYPEID_CLOTH,					/* 服 */
	TYPEID_ACCE,					/* アクセサリ */
	TYPEID_ARMS,					/* 持ち物 */
	TYPEID_SHIELD,					/* 盾 */
	TYPEID_HP,						/* HP増減 */
	TYPEID_LIGHT,					/* 灯り */
	TYPEID_MAX
};

/* 画像IDメイン(服) */
enum {
	GRPIDMAIN_CLOTH_NONE = 0,		/* 未設定 */
	GRPIDMAIN_CLOTH_SP,				/* 特殊服 */
	GRPIDMAIN_CLOTH_MAX
};

/* 画像IDメイン(持ち物) */
enum {
	GRPIDMAIN_ARMS_NONE = 0,		/* 未設定 */
	GRPIDMAIN_ARMS_BOW,				/* 弓 */
	GRPIDMAIN_WEAPON_GLOVE,			/* 武器(打撃) */
	GRPIDMAIN_WEAPON_ETC,			/* 武器(その他) */
	GRPIDMAIN_2X2_ARMS,				/* 持ち物(2x2) */
	GRPIDMAIN_2X2_SHIELD,			/* 盾(2x2) */
	GRPIDMAIN_2X2_ARMSSP,			/* 特殊持ち物(2x2) */
	GRPIDMAIN_2X2_BOW,				/* 弓(2x2) */
	GRPIDMAIN_ARMS_MAX
};

/* モーション種別 */
#define ARMS_MOTION_NONE		0x00000000
#define ARMS_MOTION_SWING		0x00000001		/* 振り */
#define ARMS_MOTION_POKE		0x00000002		/* 突き */
#define ARMS_MOTION_BOW			0x00000004		/* 弓 */
#define ARMS_MOTION_BLOW		0x00000008		/* 打撃 */
#define ARMS_MOTION_FISHING		0x00000010		/* 釣り */

}
