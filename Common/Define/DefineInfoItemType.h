#pragma once

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */
namespace InfoItemType {

/* �A�C�e����� */
enum {
	TYPEID_NONE = 0,				/* ���ʖ��� */
	TYPEID_CLOTH,					/* �� */
	TYPEID_ACCE,					/* �A�N�Z�T�� */
	TYPEID_ARMS,					/* ������ */
	TYPEID_SHIELD,					/* �� */
	TYPEID_HP,						/* HP���� */
	TYPEID_LIGHT,					/* ���� */
	TYPEID_MAX
};

/* �摜ID���C��(��) */
enum {
	GRPIDMAIN_CLOTH_NONE = 0,		/* ���ݒ� */
	GRPIDMAIN_CLOTH_SP,				/* ���ꕞ */
	GRPIDMAIN_CLOTH_MAX
};

/* �摜ID���C��(������) */
enum {
	GRPIDMAIN_ARMS_NONE = 0,		/* ���ݒ� */
	GRPIDMAIN_ARMS_BOW,				/* �| */
	GRPIDMAIN_WEAPON_GLOVE,			/* ����(�Ō�) */
	GRPIDMAIN_WEAPON_ETC,			/* ����(���̑�) */
	GRPIDMAIN_2X2_ARMS,				/* ������(2x2) */
	GRPIDMAIN_2X2_SHIELD,			/* ��(2x2) */
	GRPIDMAIN_2X2_ARMSSP,			/* ���ꎝ����(2x2) */
	GRPIDMAIN_2X2_BOW,				/* �|(2x2) */
	GRPIDMAIN_ARMS_MAX
};

/* ���[�V������� */
#define ARMS_MOTION_NONE		0x00000000
#define ARMS_MOTION_SWING		0x00000001		/* �U�� */
#define ARMS_MOTION_POKE		0x00000002		/* �˂� */
#define ARMS_MOTION_BOW			0x00000004		/* �| */
#define ARMS_MOTION_BLOW		0x00000008		/* �Ō� */
#define ARMS_MOTION_FISHING		0x00000010		/* �ނ� */

}
