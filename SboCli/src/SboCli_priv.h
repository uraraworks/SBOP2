/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:SboCli_priv.h												 */
/* ���e			:�N���C�A���g�p��`�t�@�C��									 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/05/04													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

#define MAPPARTSSIZE	(32)										/* �}�b�v�p�[�c�T�C�Y */
#define SCRSIZEX		(MAPPARTSSIZE * DRAW_PARTS_X)				/* �}�b�v�\���T�C�Y(��) */
#define SCRSIZEY		(MAPPARTSSIZE * DRAW_PARTS_Y)				/* �}�b�v�\���T�C�Y(�c) */
#define SCROLLSIZE		(16)										/* �X�N���[���P�� */
#define MAXCOUNT_CHARGRPCOUNTONEFILE	(16)						/* �L�����摜��1�t�@�C���ɓo�^����Ă���L������ */

/* �E�B���h�E���b�Z�[�W */
enum {
	WM_INITEND = WM_APP,								/* ���������� */
	WM_WNDCLOSE,										/* �E�B���h�E������ */
	WM_WINDOWMSG,										/* �E�B���h�E����̒ʒm */
	WM_MGRDRAW,											/* �`��Ǘ�����̒ʒm */
	WM_MAINFRAME,										/* ���C���t���[���ւ̒ʒm */
	WM_RECVCOMMAND,										/* �R�}���h��M */
	WM_ADMINMSG,										/* �Ǘ��҃E�B���h�E�֘A�̒ʒm */
};

/* �E�B���h�EID */
enum {
	WINDOWID_SELECTMAPPARTSGRP = 1,						/* �}�b�v�p�[�c�摜�I�� */
	WINDOWID_SELECTMAPSHADOWGRP,						/* �}�b�v�e�摜�I�� */
};

/* ���\�[�XID */
enum {
	IDC_SAVEPASSWORD = 1000,							/* �p�X���[�h���L�^����`�F�b�N */
	IDC_CONNECT,										/* �ڑ��{�^�� */
};

/* �`��}�l�[�W������̒ʒm */
enum {
	MGRDRAWMSG_NONE = 0,
	MGRDRAWMSG_START_FADEIN,							/* �t�F�[�h�C���J�n */
	MGRDRAWMSG_START_FADEOUT,							/* �t�F�[�h�A�E�g�J�n */
	MGRDRAWMSG_END_FADEIN,								/* �t�F�[�h�C������ */
	MGRDRAWMSG_END_FADEOUT,								/* �t�F�[�h�A�E�g���� */
};

/* ���C���t���[���ւ̒ʒm */
enum {
	MAINFRAMEMSG_NONE = 0,
	MAINFRAMEMSG_CHGSTATE,								/* ��ԕύX */
	MAINFRAMEMSG_CONNECT,								/* �ڑ� */
	MAINFRAMEMSG_DISCONNECT,							/* �ؒf */
	MAINFRAMEMSG_CHARNAME,								/* �L���������͊��� */
	MAINFRAMEMSG_RENEWACCOUNTINFO,						/* �A�J�E���g���X�V */
	MAINFRAMEMSG_RES_MAKECHAR,							/* �L�����쐬���� */
	MAINFRAMEMSG_RENEWCHARINFO,							/* �L�������X�V */
	MAINFRAMEMSG_RENEWCHARCHAT,							/* �������e�X�V */
	MAINFRAMEMSG_RENEWCHARCOUNT,						/* �L�������X�V */
	MAINFRAMEMSG_RENEWMAPINFO,							/* �}�b�v���X�V */
	MAINFRAMEMSG_RENEWONLINECOUNT,						/* �I�����C�����X�V */
	MAINFRAMEMSG_RENEWSYSTEMMSG,						/* �V�X�e�����b�Z�[�W�X�V */
	MAINFRAMEMSG_RENEWADMINLEVEL,						/* �Ǘ��Ҍ����ύX */
	MAINFRAMEMSG_RENEWITEMTYPEINFO,						/* �A�C�e����ʏ��X�V */
	MAINFRAMEMSG_RENEWITEMINFO,							/* �A�C�e�����X�V */
	MAINFRAMEMSG_RENEWITEMWEAPONINFO,					/* ������X�V */
	MAINFRAMEMSG_RENEWMOTION,							/* ���[�V�������X�V */
	MAINFRAMEMSG_RENEWMOTIONTYPE,						/* ���[�V������ʏ��X�V */
	MAINFRAMEMSG_RENEWBALLOON,							/* ���o�����X�V */
	MAINFRAMEMSG_RENEWEFFECT,							/* �G�t�F�N�g���X�V */
	MAINFRAMEMSG_RENEWSTATUS,							/* �X�e�[�^�X���X�V */
	MAINFRAMEMSG_RENEWSYSTEMINFO,						/* �V�X�e�����X�V */
	MAINFRAMEMSG_RENEWVIEWSET,							/* �\���ݒ�X�V */
	MAINFRAMEMSG_RENEWSKILLINFO,						/* �X�L�����X�V */
	MAINFRAMEMSG_RENEWTALKEVENT,						/* ��b�C�x���g���X�V */
	MAINFRAMEMSG_DAMAGE,								/* �_���[�W���󂯂� */
};

/* �Ǘ��҃E�B���h�E�֘A�̒ʒm */
enum {
	ADMINMSG_NONE = 0,
	ADMINMSG_NOTIFYTYPE_LBUTTONDOWN,					/* ���N���b�N�ʒm */
	ADMINMSG_NOTIFYTYPE_RBUTTONDOWN,					/* �E�N���b�N�ʒm */
	ADMINMSG_NOTIFYTYPE_RBUTTONDBLCLK,					/* �E�_�u���N���b�N�ʒm */
	ADMINMSG_RENEWMAPINFO,								/* �}�b�v���X�V */
	ADMINMSG_RENEWMAPOBJECT,							/* �}�b�v�I�u�W�F�N�g���X�V */
	ADMINMSG_RENEWMAPPARTS,								/* �}�b�v�p�[�c���X�V */
	ADMINMSG_RENEWMAPSHADOW,							/* �}�b�v�e���X�V */
	ADMINMSG_RENEWTALKEVENT,							/* ��b�C�x���g���X�V */
	ADMINMSG_ADMINLEVEL,								/* �Ǘ��҃��x���ʒm */
	ADMINMSG_SELECT_ANIME,								/* �A�j���[�V�����R�}�I�� */
	ADMINMSG_ACCOUNTINFO,								/* �A�J�E���g���擾 */
	ADMINMSG_CHAR_ONLINE,								/* �I�����C�����L�����ꗗ�X�V */
};

/* �Ǘ��҃E�B���h�E�ւ̒ʒm��� */
enum {
	ADMINNOTIFYTYPE_NONE = 0,
	ADMINNOTIFYTYPE_CHARID,								/* �L����ID */
	ADMINNOTIFYTYPE_ACCOUNTID,							/* �A�J�E���gID */
	ADMINNOTIFYTYPE_MAPEDIT,							/* �}�b�v�ҏW(�ʒm�͖���) */
	ADMINNOTIFYTYPE_MAPSHADOWEDIT,						/* �}�b�v�e�ҏW(�ʒm�͖���) */
	ADMINNOTIFYTYPE_ITEMID,								/* �A�C�e��ID */
	ADMINNOTIFYTYPE_POS,								/* �}�b�v���W */
	ADMINNOTIFYTYPE_CHARPOS,							/* �L�������W */
};

/* �t�F�[�h��� */
enum {
	FADESTATE_NONE = 0,
	FADESTATE_FADEIN,									/* �t�F�[�h�C���� */
	FADESTATE_FADEOUT,									/* �t�F�[�h�A�E�g�� */
};

/* ���C���[ID */
enum {
	LAYERTYPE_NONE = 0,
	LAYERTYPE_LOGO,										/* ���S */
	LAYERTYPE_TITLE,									/* �^�C�g�� */
	LAYERTYPE_PRODUCT,									/* �v���_�N�g */
	LAYERTYPE_LOGINMENU,								/* ���O�C�����j���[ */
	LAYERTYPE_CHARSELECT,								/* �L�����I�� */
	LAYERTYPE_MAP,										/* �}�b�v */
	LAYERTYPE_INFO,										/* ���m�点 */
	LAYERTYPE_CLOUD,									/* �_ */
	LAYERTYPE_SYSTEMMSG,								/* �V�X�e�����b�Z�[�W */
};

/* �Q�[����� */
enum {
	GAMESTATE_NONE = 0,
	GAMESTATE_LOGO,										/* URARA-works���S */
	GAMESTATE_TITLE,									/* �^�C�g����� */
	GAMESTATE_LOGIN,									/* ���O�C����� */
	GAMESTATE_DISCONNECT,								/* �ؒf */
	GAMESTATE_INFO,										/* ���m�点��� */
	GAMESTATE_LOGINMENU,								/* ���j���[��� */
	GAMESTATE_MAP,										/* �}�b�v��� */
};

/* �ؒf���R */
enum {
	DISCONNECTID_CONNECT = 0,							/* �ڑ����s */
	DISCONNECTID_VERSION,								/* �o�[�W�����s��v */
	DISCONNECTID_USER,									/* ���� */
	DISCONNECTID_DISABLE,								/* �֎~ */
	DISCONNECTID_PASSWORD,								/* �p�X���[�h�s��v */
	DISCONNECTID_LOGIN,									/* ���O�C���ς� */
	DISCONNECTID_MAC,									/* �쐬�ς� */
};

/* ========================================================================= */
/* �\���̒�`																 */
/* ========================================================================= */

/* �V�X�e�����b�Z�[�W�\���� */
typedef struct _STSYSTEMMSG {
	BOOL		bAddLog;			/* ���O�֒ǉ�����H */
	CmyString	strMsg;				/* ���b�Z�[�W */
	COLORREF	clMsg;				/* �`��F */
} STSYSTEMMSG, *PSTSYSTEMMSG;

/* ���l�ƕ�����|�C���^�\���� */
typedef struct _STINTLPCSTR {
	int			nValue;				/* ���l */
	LPCSTR		pszText;			/* ������|�C���^ */
} STINTLPCSTR, *PSTINTLPCSTR;

/* Copyright(C)URARA-works 2006 */
