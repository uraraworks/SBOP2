/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:Command.h													 */
/* ���e			:�R�}���h��`�t�@�C��										 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �R�}���hID(���C��) */
enum {
	SBOCOMMANDID_MAIN_NONE = 0,
	SBOCOMMANDID_MAIN_VERSION,					/* �o�[�W�����n */
	SBOCOMMANDID_MAIN_CONNECT,					/* �ڑ��n */
	SBOCOMMANDID_MAIN_ACCOUNT,					/* �A�J�E���g�n */
	SBOCOMMANDID_MAIN_CHAR,						/* �L�����n */
	SBOCOMMANDID_MAIN_MAP,						/* �}�b�v�n */
	SBOCOMMANDID_MAIN_ADMIN,					/* �Ǘ��Ҍn */
	SBOCOMMANDID_MAIN_MSGCMD,					/* ���b�Z�[�W�R�}���h�n */
	SBOCOMMANDID_MAIN_ITEM,						/* �A�C�e���n */
	SBOCOMMANDID_MAIN_EFFECT,					/* �G�t�F�N�g�n */
	SBOCOMMANDID_MAIN_BATTLE,					/* �퓬�n */
	SBOCOMMANDID_MAIN_SYSTEM,					/* �V�X�e���n */

	SBOCOMMANDID_MAIN_MAX
};

/* �R�}���hID(�o�[�W�����n) */
enum {
	SBOCOMMANDID_SUB_VERSION_NONE = 0,
	SBOCOMMANDID_SUB_VERSION_REQ_VERSIONCHECK,	/* �o�[�W�����`�F�b�N�v�� */
	SBOCOMMANDID_SUB_VERSION_RES_VERSIONCHECK,	/* �o�[�W�����`�F�b�N���� */
	SBOCOMMANDID_SUB_VERSION_REQ_FILELISTCHECK,	/* �t�@�C�����X�g�`�F�b�N�v�� */
	SBOCOMMANDID_SUB_VERSION_RES_FILELISTCHECK,	/* �t�@�C�����X�g�`�F�b�N���� */
	SBOCOMMANDID_SUB_VERSION_REQ_FILELIST,		/* �t�@�C�����X�g�v�� */
	SBOCOMMANDID_SUB_VERSION_RES_FILELIST,		/* �t�@�C�����X�g���� */
	SBOCOMMANDID_SUB_VERSION_REQ_FILE,			/* �t�@�C���v�� */
	SBOCOMMANDID_SUB_VERSION_RES_FILE,			/* �t�@�C������ */
	SBOCOMMANDID_SUB_VERSION_MAX
};

/* �R�}���hID(�ڑ��n) */
enum {
	SBOCOMMANDID_SUB_CONNECT_NONE = 0,
	SBOCOMMANDID_SUB_CONNECT_REQ_LOGIN,			/* ���O�C���v�� */
	SBOCOMMANDID_SUB_CONNECT_RES_LOGIN,			/* ���O�C������ */
	SBOCOMMANDID_SUB_CONNECT_REQ_PLAY,			/* �Q�[���J�n�v�� */
	SBOCOMMANDID_SUB_CONNECT_RES_PLAY,			/* �Q�[���J�n���� */
	SBOCOMMANDID_SUB_CONNECT_KEEPALIVE,			/* �����m�F�ʒm */
};

/* �R�}���hID(�A�J�E���g�n) */
enum {
	SBOCOMMANDID_SUB_ACCOUNT_NONE = 0,
	SBOCOMMANDID_SUB_ACCOUNT_REQ_ACCOUNTINFO,	/* �A�J�E���g���v�� */
	SBOCOMMANDID_SUB_ACCOUNT_RES_ACCOUNTINFO,	/* �A�J�E���g��񉞓� */
	SBOCOMMANDID_SUB_ACCOUNT_REQ_MAKECHAR,		/* �L�����쐬�v�� */
	SBOCOMMANDID_SUB_ACCOUNT_RES_MAKECHAR,		/* �L�����쐬���� */
};

/* �R�}���hID(�L�����n) */
enum {
	SBOCOMMANDID_SUB_CHAR_NONE = 0,
	SBOCOMMANDID_SUB_CHAR_REQ_CHARINFO,			/* �L�������v�� */
	SBOCOMMANDID_SUB_CHAR_RES_CHARINFO,			/* �L������񉞓� */
	SBOCOMMANDID_SUB_CHAR_CHARINFO,				/* �L�������ʒm */
	SBOCOMMANDID_SUB_CHAR_MOVEPOS,				/* �ړ��ʒm */
	SBOCOMMANDID_SUB_CHAR_STATE,				/* ��Ԓʒm */
	SBOCOMMANDID_SUB_CHAR_PROCSTATE,			/* �s����Ԓʒm */
	SBOCOMMANDID_SUB_CHAR_REQ_CHAT,				/* �`���b�g�v�� */
	SBOCOMMANDID_SUB_CHAR_CHAT,					/* �`���b�g�ʒm */
	SBOCOMMANDID_SUB_CHAR_CHARID,				/* �L����ID�ʒm */
	SBOCOMMANDID_SUB_CHAR_REQ_CHARINFO2,		/* �L�������v��(����) */
	SBOCOMMANDID_SUB_CHAR_REQ_PUTGET,			/* �A�C�e�����E���u���v�� */
	SBOCOMMANDID_SUB_CHAR_RES_PUTGET,			/* �A�C�e�����E���u������ */
	SBOCOMMANDID_SUB_CHAR_REQ_USEITEM,			/* �A�C�e�����g���v�� */
	SBOCOMMANDID_SUB_CHAR_SET_EFFECT,			/* �G�t�F�N�g���w�� */
	SBOCOMMANDID_SUB_CHAR_ITEMINFO,				/* �A�C�e�����ʒm */
	SBOCOMMANDID_SUB_CHAR_REQ_PUSH,				/* �����v�� */
	SBOCOMMANDID_SUB_CHAR_REQ_TAIL,				/* �t���čs���v�� */
	SBOCOMMANDID_SUB_CHAR_RES_TAIL,				/* �t���čs������ */
	SBOCOMMANDID_SUB_CHAR_REQ_MODIFY_PARAM,		/* �p�����[�^�̕ύX�v�� */
	SBOCOMMANDID_SUB_CHAR_MODIFY_PARAM,			/* �p�����[�^�̕ύX�ʒm */
	SBOCOMMANDID_SUB_CHAR_REQ_EQUIP,			/* �����v�� */
	SBOCOMMANDID_SUB_CHAR_GRP,					/* �摜�ʒm */
	SBOCOMMANDID_SUB_CHAR_MOTION,				/* ���[�V�������ʒm */
	SBOCOMMANDID_SUB_CHAR_MOTIONTYPE,			/* ���[�V������ʏ��ʒm */
	SBOCOMMANDID_SUB_CHAR_SET_EFCBALLOON,		/* ���o�����w�� */
	SBOCOMMANDID_SUB_CHAR_PROC_FISHING,			/* �ނ�v�� */
	SBOCOMMANDID_SUB_CHAR_SET_MOTION,			/* ���[�V�������w�� */
	SBOCOMMANDID_SUB_CHAR_REQ_CHECKMAPEVENT,	/* �}�b�v�C�x���g�`�F�b�N�v�� */
	SBOCOMMANDID_SUB_CHAR_RES_CHECKMAPEVENT,	/* �}�b�v�C�x���g�`�F�b�N���� */
	SBOCOMMANDID_SUB_CHAR_STATUS,				/* �X�e�[�^�X���ʒm */
	SBOCOMMANDID_SUB_CHAR_TEXTEFFECT,			/* �����G�t�F�N�g�ʒm */
	SBOCOMMANDID_SUB_CHAR_STATE_CHARGE,			/* ���ߏ�Ԓʒm */
};

/* �R�}���hID(�}�b�v�n) */
enum {
	SBOCOMMANDID_SUB_MAP_NONE = 0,
	SBOCOMMANDID_SUB_MAP_REQ_MAPINFO,			/* �}�b�v���v�� */
	SBOCOMMANDID_SUB_MAP_RES_MAPINFO,			/* �}�b�v��񉞓� */
	SBOCOMMANDID_SUB_MAP_ONLINE,				/* �I�����C�����ʒm */
	SBOCOMMANDID_SUB_MAP_SYSTEMMSG,				/* �V�X�e�����b�Z�[�W�ʒm */
	SBOCOMMANDID_SUB_MAP_FORMATMSG,				/* �t�H�[�}�b�g���b�Z�[�W�ʒm */
	SBOCOMMANDID_SUB_MAP_MAPPARTS,				/* �}�b�v�p�[�c���ʒm */
	SBOCOMMANDID_SUB_MAP_SETPARTS,				/* �}�b�v�p�[�c�z�u */
	SBOCOMMANDID_SUB_MAP_RENEWMAPSIZE,			/* �}�b�v�T�C�Y�X�V */
	SBOCOMMANDID_SUB_MAP_DELETEPARTS,			/* �}�b�v�p�[�c�폜 */
	SBOCOMMANDID_SUB_MAP_MAPSHADOW,				/* �}�b�v�e���ʒm */
	SBOCOMMANDID_SUB_MAP_SETMAPSHADOW,			/* �}�b�v�e�z�u */
	SBOCOMMANDID_SUB_MAP_DELETEMAPSHADOW,		/* �}�b�v�e�폜 */
	SBOCOMMANDID_SUB_MAP_MAPNAME,				/* �}�b�v���ʒm */
	SBOCOMMANDID_SUB_MAP_MAPEVENT,				/* �}�b�v�C�x���g���ʒm */
	SBOCOMMANDID_SUB_MAP_DELETEEVENT,			/* �}�b�v�C�x���g���폜 */
	SBOCOMMANDID_SUB_MAP_FADEINOUT,				/* �t�F�[�h�C��/�A�E�g�ʒm */
};

/* �R�}���hID(�Ǘ��Ҍn) */
enum {
	SBOCOMMANDID_SUB_ADMIN_NONE = 0,
	SBOCOMMANDID_SUB_ADMIN_CHARINFO,			/* �L�������X�V */
	SBOCOMMANDID_SUB_ADMIN_MAKEADMINWINDOW,		/* �Ǘ��҃E�B���h�E�쐬�ʒm */
	SBOCOMMANDID_SUB_ADMIN_RENEWMAPPARTS,		/* �}�b�v�p�[�c�X�V */
	SBOCOMMANDID_SUB_ADMIN_MAP_SETPARTS,		/* �}�b�v�p�[�c�z�u */
	SBOCOMMANDID_SUB_ADMIN_MAP_RENEWMAPSIZE,	/* �}�b�v�T�C�Y�X�V */
	SBOCOMMANDID_SUB_ADMIN_MAP_DELETEPARTS,		/* �}�b�v�p�[�c�폜 */
	SBOCOMMANDID_SUB_ADMIN_MAP_COPYPARTS,		/* �}�b�v�p�[�c�R�s�[ */
	SBOCOMMANDID_SUB_ADMIN_MAP_SETMAPNAME,		/* �}�b�v���ύX */
	SBOCOMMANDID_SUB_ADMIN_RENEWMAPSHADOW,		/* �}�b�v�e�X�V */
	SBOCOMMANDID_SUB_ADMIN_MAP_SETMAPSHADOW,	/* �}�b�v�e�z�u */
	SBOCOMMANDID_SUB_ADMIN_MAP_DELETEMAPSHADOW,	/* �}�b�v�e�폜 */
	SBOCOMMANDID_SUB_ADMIN_MAP_RENEWEVENT,		/* �}�b�v�C�x���g���X�V */
	SBOCOMMANDID_SUB_ADMIN_MAP_DELETEEVENT,		/* �}�b�v�C�x���g���폜 */
	SBOCOMMANDID_SUB_ADMIN_MAP_ADD,				/* �}�b�v�ǉ� */
	SBOCOMMANDID_SUB_ADMIN_REQ_ADMINLEVEL,		/* �Ǘ��Ҍ������x���v�� */
	SBOCOMMANDID_SUB_ADMIN_RES_ADMINLEVEL,		/* �Ǘ��Ҍ������x������ */
	SBOCOMMANDID_SUB_ADMIN_RENEWADMINLEVEL,		/* �Ǘ��Ҍ������x���X�V */
	SBOCOMMANDID_SUB_ADMIN_SERVER_SAVEINFO,		/* �T�[�o�[���ۑ� */
	SBOCOMMANDID_SUB_ADMIN_ITEM_ADD,			/* �A�C�e�����ǉ� */
	SBOCOMMANDID_SUB_ADMIN_ITEM_COPY,			/* �A�C�e�����R�s�[ */
	SBOCOMMANDID_SUB_ADMIN_ITEM_DELETE,			/* �A�C�e�����폜 */
	SBOCOMMANDID_SUB_ADMIN_ITEMTYPE_ADD,		/* �A�C�e����ʏ��ǉ� */
	SBOCOMMANDID_SUB_ADMIN_ITEMTYPE_COPY,		/* �A�C�e����ʏ��R�s�[ */
	SBOCOMMANDID_SUB_ADMIN_ITEMTYPE_DELETE,		/* �A�C�e����ʏ��폜 */
	SBOCOMMANDID_SUB_ADMIN_ITEMWEAPON_ADD,		/* ������ǉ� */
	SBOCOMMANDID_SUB_ADMIN_ITEMWEAPON_COPY,		/* ������R�s�[ */
	SBOCOMMANDID_SUB_ADMIN_ITEMWEAPON_RENEW,	/* ������X�V */
	SBOCOMMANDID_SUB_ADMIN_ITEMWEAPON_DELETE,	/* ������폜 */
	SBOCOMMANDID_SUB_ADMIN_CHAR_ADDNPC,			/* NPC�̒ǉ� */
	SBOCOMMANDID_SUB_ADMIN_CHAR_MODIFYITEM,		/* �����A�C�e���̕ύX */
	SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWMOTION,	/* �L�������[�V�������̍X�V */
	SBOCOMMANDID_SUB_ADMIN_CHAR_ADDMOTIONTYPE,	/* �L�������[�V������ʏ��̒ǉ� */
	SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWMOTIONTYPE,/* �L�������[�V������ʏ��̍X�V */
	SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWSTATUS,	/* �X�e�[�^�X���X�V */
	SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_STATUS,		/* �X�e�[�^�X���v�� */
	SBOCOMMANDID_SUB_ADMIN_EFC_RENEWBALLOON,	/* ���o�����̍X�V */
	SBOCOMMANDID_SUB_ADMIN_EFC_RENEWEFFECT,		/* �G�t�F�N�g���̍X�V */
	SBOCOMMANDID_SUB_ADMIN_REQ_PLAYSOUND,		/* ���ʉ��̍Đ��v�� */
	SBOCOMMANDID_SUB_ADMIN_PLAYSOUND,			/* ���ʉ��̍Đ� */
	SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_ACCOUNT,	/* �A�J�E���g���v�� */
	SBOCOMMANDID_SUB_ADMIN_CHAR_RES_ACCOUNT,	/* �A�J�E���g��񉞓� */
	SBOCOMMANDID_SUB_ADMIN_CHAR_RENEW_ACCOUNT,	/* �A�J�E���g���X�V */
	SBOCOMMANDID_SUB_ADMIN_RENEW_CLIENTVERSION,	/* �N���C�A���g�o�[�W�����X�V */
	SBOCOMMANDID_SUB_ADMIN_SYSTEM_REQ_INFO,		/* �V�X�e�����v�� */
	SBOCOMMANDID_SUB_ADMIN_SYSTEM_RENEWINFO,	/* �V�X�e�����̍X�V */
};

/* �R�}���hID(���b�Z�[�W�R�}���h�n) */
enum {
	SBOCOMMANDID_SUB_MSGCMD_NONE = 0,
	SBOCOMMANDID_SUB_MSGCMD_CHGFACE,			/* �\��ύX */
	SBOCOMMANDID_SUB_MSGCMD_CHGHAIR,			/* ���ύX */
	SBOCOMMANDID_SUB_MSGCMD_CHGCLOTH,			/* �����ύX */
	SBOCOMMANDID_SUB_MSGCMD_CHGACCE,			/* �A�N�Z�T���ύX */
	SBOCOMMANDID_SUB_MSGCMD_CHGCOLOR,			/* �F�ύX */
	SBOCOMMANDID_SUB_MSGCMD_CHGARMS,			/* �������ύX */
	SBOCOMMANDID_SUB_MSGCMD_CHGSHIELD,			/* ���ύX */
	SBOCOMMANDID_SUB_MSGCMD_MAKEITEM,			/* �A�C�e���쐬 */
	SBOCOMMANDID_SUB_MSGCMD_BALLOON,			/* ���o�� */
	SBOCOMMANDID_SUB_MSGCMD_DICE,				/* �T�C�R�� */
	SBOCOMMANDID_SUB_MSGCMD_RND,				/* �����_�� */
	SBOCOMMANDID_SUB_MSGCMD_EFFECT,				/* �G�t�F�N�g */
};

/* �R�}���hID(�A�C�e���n) */
enum {
	SBOCOMMANDID_SUB_ITEM_NONE = 0,
	SBOCOMMANDID_SUB_ITEM_REQ_ITEMINFO,			/* �A�C�e�����v�� */
	SBOCOMMANDID_SUB_ITEM_RES_ITEMINFO,			/* �A�C�e����񉞓� */
	SBOCOMMANDID_SUB_ITEM_ITEMINFO,				/* �A�C�e�����ʒm */
	SBOCOMMANDID_SUB_ITEM_DELETEITEMINFO,		/* �A�C�e�����폜 */
	SBOCOMMANDID_SUB_ITEM_RENEWITEMINFO,		/* �A�C�e�����X�V */
	SBOCOMMANDID_SUB_ITEM_REQ_ITEMTYPEINFO,		/* �A�C�e����ʏ��v�� */
	SBOCOMMANDID_SUB_ITEM_RES_ITEMTYPEINFO,		/* �A�C�e����ʏ�񉞓� */
	SBOCOMMANDID_SUB_ITEM_ITEMTYPEINFO,			/* �A�C�e������ʒʒm */
	SBOCOMMANDID_SUB_ITEM_DELETEITEMTYPEINFO,	/* �A�C�e������ʍ폜 */
	SBOCOMMANDID_SUB_ITEM_RENEWITEMTYPEINFO,	/* �A�C�e������ʍX�V */
	SBOCOMMANDID_SUB_ITEM_REQ_ITEMWEAPONINFO,	/* ������v�� */
	SBOCOMMANDID_SUB_ITEM_RES_ITEMWEAPONINFO,	/* �����񉞓� */
	SBOCOMMANDID_SUB_ITEM_ITEMWEAPONINFO,		/* ������ʒm */
};

/* �R�}���hID(�G�t�F�N�g�n) */
enum {
	SBOCOMMANDID_SUB_EFFECT_NONE = 0,
	SBOCOMMANDID_SUB_EFFECT_REQ_EFFECTINFO,		/* �G�t�F�N�g���v�� */
	SBOCOMMANDID_SUB_EFFECT_RES_EFFECTINFO,		/* �G�t�F�N�g��񉞓� */
	SBOCOMMANDID_SUB_EFFECT_EFFECTINFO,			/* �G�t�F�N�g���ʒm */
	SBOCOMMANDID_SUB_EFFECT_DELETEEFFECTINFO,	/* �G�t�F�N�g���폜 */
	SBOCOMMANDID_SUB_EFFECT_REQ_BALLOONINFO,	/* ���o�����v�� */
	SBOCOMMANDID_SUB_EFFECT_BALLOONINFO,		/* ���o�����ʒm */
	SBOCOMMANDID_SUB_EFFECT_DELETEBALLOONINFO,	/* ���o�����폜 */
};

/* �R�}���hID(�퓬�n) */
enum {
	SBOCOMMANDID_SUB_BATTLE_NONE = 0,
	SBOCOMMANDID_SUB_BATTLE_REQ_ATACK,			/* �U���v�� */
};

/* �R�}���hID(�V�X�e���n) */
enum {
	SBOCOMMANDID_SUB_SYSTEM_NONE = 0,
	SBOCOMMANDID_SUB_SYSTEM_INFO,				/* �V�X�e�����ʒm */
};

/* Copyright(C)URARA-works 2006 */
