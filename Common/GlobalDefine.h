/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:GlobalDefine.h												 */
/* ���e			:���ʒ�`�t�@�C��											 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �}�N����`																 */
/* ========================================================================= */

#define SAFE_DELETE(p)			{ if(p) { delete (p);		(p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);		(p)=NULL; } }
#define SAFE_DESTROYWND(p)		{ if(p) { DestroyWindow(p);	(p)=NULL; } }

typedef map<DWORD, PVOID>	MapIDPtr;

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

#define WNDTITLE				"Scrap Book Online Page.2"			/* �E�B���h�E�^�C�g�� */
#define DRAW_PARTS_X			(15)								/* �}�b�v�\����(��) */
#define DRAW_PARTS_Y			(15)								/* �}�b�v�\����(�c) */
#define MOVEWAIT				(11)								/* �ړ����� */
#define BATTLEMOVEWAIT			(15)								/* �ړ�����(�퓬��) */

#define MAXLEN_ACCOUNT			(10)								/* �ő�A�J�E���g���� */
#define MAXLEN_PASSWORD			(10)								/* �ő�p�X���[�h�� */
#define MAXLEN_CHARNAME			(15)								/* �ő�L�������� */
#define URARASOCK_PRECHECK		(0x56BB3E5E)						/* �`�F�b�N�p�L�[���[�h	 */
#define URARASOCK_MSGBASE		(WM_APP + 200)						/* �ʐM���C�u�����ʒm���b�Z�[�W� */

/* �L�����O���t�B�b�NID���C�� */
enum {
	CHARGRPMAIN_NONE = 0,
	CHARGRPMAIN_3232,
};

/* �L�����O���t�B�b�NID�T�u */
enum {
	CHARGRPSUB_NONE = 0,
	CHARGRPSUB_INIT_MALE,							/* �����L����(�j) */
	CHARGRPSUB_INIT_FEMALE,							/* �����L����(��) */
};

/* ���ʂɂ���� */
enum {
	SEX_ALL = 0,									/* �S�� */
	SEX_MALE,										/* �j�� */
	SEX_FEMALE,										/* ���� */
	SEX_MAX
};

/* �푰 */
enum {
	FAMILYTYPE_NONE = 0,
	FAMILYTYPE_HUMAN,								/* �j���Q�� */
	FAMILYTYPE_ELF,									/* �G���t */
	FAMILYTYPE_BST,									/* �W���E�W�� */
	FAMILYTYPE_DAEMON,								/* �}�]�N */
	FAMILYTYPE_MAX
};

/* �Ǘ��҃��x�� */
enum {
	ADMINLEVEL_NONE = 0,							/* ��� */
	ADMINLEVEL_ALL									/* �S���� */
};

/* �}�[�N�� */
enum {
	MARKID_NONE = 0,
	MARKID_BEGINNER,								/* ���S�҃}�[�N */
	MARKID_ADMIN_URARA,								/* ������}�[�N */
	MARKID_ADMIN_VELTINA,							/* �ׂ邿�ȃ}�[�N */
	MARKID_ADMIN,									/* �Ǘ��҃}�[�N */
	MARKID_NAZO,									/* �悭�킩��Ȃ��}�[�N�i�� */
	MARKID_PT_MEMBER,								/* �o�s�����o */
	MARKID_PT_LEADER,								/* �o�s���[�_�[ */
	MARKID_PT_RECRUIT,								/* PT��W�� */
	MARKID_SHOP,									/* �I�X�H */
	MARKID_GUILD_LEADER,							/* �M���h���[�_�[ */
	MARKID_GUILD_MEMBER,							/* �M���h�����o */
	MARKID_WALLET,									/* �p�r���肪�܂����T�C�t */
};

/* ������ */
enum {
	WEAPONTYPE_SWING = 0,							/* �U�� */
	WEAPONTYPE_POKE,								/* �˂� */
	WEAPONTYPE_BOW,									/* �| */
};

/* �����̎�� */
enum {
	EQUIPTYPE_ACCE1 = 0,	/* �A�N�Z�T��1 */
	EQUIPTYPE_HEAD,			/* �� */
	EQUIPTYPE_ACCE2,		/* �A�N�Z�T��2 */
	EQUIPTYPE_ARMSRIGHT,	/* �E�� */
	EQUIPTYPE_CLOTH,		/* �� */
	EQUIPTYPE_ARMSLEFT,		/* ���� */
	EQUIPTYPE_MAX
};

/* �摜���ID */
enum {
	GRPIDMAIN_NONE = 0,
	GRPIDMAIN_CHAR,			/* �L���� */
	GRPIDMAIN_WEAPON,		/* ���� */
	GRPIDMAIN_WEAPON_BOW,	/* �| */
	GRPIDMAIN_EFFECT32,		/* �G�t�F�N�g(32) */
	GRPIDMAIN_WEAPON_GLOVE,	/* ����(�Ō�) */
	GRPIDMAIN_WEAPON_ETC,	/* ����(���̑�) */
	GRPIDMAIN_NPC,			/* �L����(NPC) */
	GRPIDMAIN_EFCBALLOON,	/* ���o�� */
	GRPIDMAIN_2X2_CHAR,		/* �L����(2x2) */
	GRPIDMAIN_2X2_CLOTH,	/* ��(2x2) */
	GRPIDMAIN_2X2_EYE,		/* ��(2x2) */
	GRPIDMAIN_2X2_HAIR,		/* ��(2x2) */
	GRPIDMAIN_2X2_SPCLOTH,	/* ���ꕞ(2x2) */
	GRPIDMAIN_2X2_SPHAIR,	/* ���ꔯ(2x2) */
	GRPIDMAIN_2X2_ARMS,		/* ������(2x2) */
	GRPIDMAIN_2X2_SHIELD,	/* ��(2x2) */
	GRPIDMAIN_2X2_ARMSSP,	/* ���ꎝ����(2x2) */
	GRPIDMAIN_2X2_BOW,		/* �|(2x2) */
	GRPIDMAIN_2X2_NPC,		/* NPC(2x2) */
	GRPIDMAIN_EFFECT64,		/* �G�t�F�N�g(64) */
	GRPIDMAIN_MAX
};

/* �g�p�Ώ� */
enum {
	CHARTARGET_ALLY = 0,	/* ���� */
	CHARTARGET_ENEMY,		/* �G */
};

/* �U���Ώ� */
enum {
	ATACKTARGETTYPE_NONE = 0,		/* ���ݒ� */
	ATACKTARGETTYPE_PC,				/* �v���C���[ */
	ATACKTARGETTYPE_NPC,			/* NPC */
	ATACKTARGETTYPE_ALL,			/* �S�� */
	ATACKTARGETTYPE_MAX
};

/* �g�p�͈� */
enum {
	CHARAREA_SELF = 0,		/* ���� */
	CHARAREA_FRONT,			/* 1���O */
	CHARAREA_AROUND,		/* ���� */
	CHARAREA_SCREEN,		/* ��ʓ� */
};


/* ========================================================================= */
/* �\���̒�`																 */
/* ========================================================================= */


/* ========================================================================= */
/* �֐���	:FreeInfo														 */
/* ���e		:�����J��														 */
/* ���t		:2005/05/06														 */
/* ========================================================================= */

template<class PARRAY, class PINFO>
void FreeInfo(PARRAY pArray, PINFO pInfo)
{
	int i, nCount;

	if (pArray) {
		nCount = pArray->GetSize ();
		for (i = nCount - 1; i >= 0; i --) {
			pInfo = pArray->GetAt (i);
			SAFE_DELETE (pInfo);
		}
		pArray->RemoveAll ();
	}
}

/* Copyright(C)URARA-works 2006 */
