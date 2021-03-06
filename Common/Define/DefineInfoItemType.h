#pragma once

/* ========================================================================= */
/* èè`																	 */
/* ========================================================================= */
namespace InfoItemType {

/* ACeíÊ */
enum {
	TYPEID_NONE = 0,				/* øÊ³µ */
	TYPEID_CLOTH,					/*  */
	TYPEID_ACCE,					/* ANZT */
	TYPEID_ARMS,					/* ¿¨ */
	TYPEID_SHIELD,					/*  */
	TYPEID_HP,						/* HP¸ */
	TYPEID_LIGHT,					/* è */
	TYPEID_MAX
};

/* æIDC() */
enum {
	GRPIDMAIN_CLOTH_NONE = 0,		/* ¢Ýè */
	GRPIDMAIN_CLOTH_SP,				/* Áê */
	GRPIDMAIN_CLOTH_MAX
};

/* æIDC(¿¨) */
enum {
	GRPIDMAIN_ARMS_NONE = 0,		/* ¢Ýè */
	GRPIDMAIN_ARMS_BOW,				/* | */
	GRPIDMAIN_WEAPON_GLOVE,			/* í(Å) */
	GRPIDMAIN_WEAPON_ETC,			/* í(»Ì¼) */
	GRPIDMAIN_2X2_ARMS,				/* ¿¨(2x2) */
	GRPIDMAIN_2X2_SHIELD,			/* (2x2) */
	GRPIDMAIN_2X2_ARMSSP,			/* Áê¿¨(2x2) */
	GRPIDMAIN_2X2_BOW,				/* |(2x2) */
	GRPIDMAIN_ARMS_MAX
};

/* [VíÊ */
#define ARMS_MOTION_NONE		0x00000000
#define ARMS_MOTION_SWING		0x00000001		/* Uè */
#define ARMS_MOTION_POKE		0x00000002		/* Ë« */
#define ARMS_MOTION_BOW			0x00000004		/* | */
#define ARMS_MOTION_BLOW		0x00000008		/* Å */
#define ARMS_MOTION_FISHING		0x00000010		/* Þè */

}
