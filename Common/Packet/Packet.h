/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:Packet.h													 */
/* ���e			:�p�P�b�g�N���X��`�t�@�C��									 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "VERSION/PacketVERSION_REQ_VERSIONCHECK.h"		/* �o�[�W�����n:�o�[�W�����`�F�b�N�v�� */
#include "VERSION/PacketVERSION_RES_VERSIONCHECK.h"		/* 				�o�[�W�����`�F�b�N���� */
#include "VERSION/PacketVERSION_REQ_FILELISTCHECK.h"	/* 				�t�@�C�����X�g�`�F�b�N�v�� */
#include "VERSION/PacketVERSION_RES_FILELISTCHECK.h"	/* 				�t�@�C�����X�g�`�F�b�N���� */
#include "VERSION/PacketVERSION_REQ_FILELIST.h"			/* 				�t�@�C�����X�g�v�� */
#include "VERSION/PacketVERSION_RES_FILELIST.h"			/* 				�t�@�C�����X�g���� */
#include "VERSION/PacketVERSION_REQ_FILE.h"				/*				�t�@�C���v�� */
#include "VERSION/PacketVERSION_RES_FILE.h"				/*				�t�@�C������ */
#include "CONNECT/PacketCONNECT_REQ_LOGIN.h"			/* �ڑ��n:	���O�C���v�� */
#include "CONNECT/PacketCONNECT_RES_LOGIN.h"			/* 			���O�C������ */
#include "CONNECT/PacketCONNECT_REQ_PLAY.h"				/* 			�Q�[���J�n�v�� */
#include "CONNECT/PacketCONNECT_RES_PLAY.h"				/* 			�Q�[���J�n���� */
#include "CONNECT/PacketCONNECT_KEEPALIVE.h"			/* 			�����m�F�ʒm */
#include "ACCOUNT/PacketACCOUNT_REQ_ACCOUNTINFO.h"		/* �A�J�E���g�n:�A�J�E���g���v�� */
#include "ACCOUNT/PacketACCOUNT_RES_ACCOUNTINFO.h"		/* 				�A�J�E���g��񉞓� */
#include "ACCOUNT/PacketACCOUNT_REQ_MAKECHAR.h"			/* 				�L�����쐬�v�� */
#include "ACCOUNT/PacketACCOUNT_RES_MAKECHAR.h"			/* 				�L�����쐬���� */
#include "CHAR/PacketCHAR_REQ_CHARINFO.h"				/* �L�����n:�L�������v�� */
#include "CHAR/PacketCHAR_RES_CHARINFO.h"				/* 			�L������񉞓� */
#include "CHAR/PacketCHAR_MOVEPOS.h"					/* 			�ړ��ʒm */
#include "CHAR/PacketCHAR_STATE.h"						/* 			��Ԓʒm */
#include "CHAR/PacketCHAR_CHARINFO.h"					/* 			�L�������ʒm */
#include "CHAR/PacketCHAR_REQ_CHAT.h"					/* 			�`���b�g�v�� */
#include "CHAR/PacketCHAR_CHAT.h"						/* 			�`���b�g�ʒm */
#include "CHAR/PacketCHAR_CHARID.h"						/* 			�L����ID�ʒm */
#include "CHAR/PacketCHAR_REQ_CHARINFO2.h"				/* 			�L�������v��(����) */
#include "CHAR/PacketCHAR_REQ_PUTGET.h"					/* 			�A�C�e�����E���u���v�� */
#include "CHAR/PacketCHAR_RES_PUTGET.h"					/* 			�A�C�e�����E���u������ */
#include "CHAR/PacketCHAR_REQ_USEITEM.h"				/* 			�A�C�e�����g���v�� */
#include "CHAR/PacketCHAR_REQ_DRAGITEM.h"				/* 			�A�C�e���ʒu�ύX�v�� */
#include "CHAR/PacketCHAR_SET_EFFECT.h"					/* 			�G�t�F�N�g�w�� */
#include "CHAR/PacketCHAR_ITEMINFO.h"					/* 			�A�C�e�����ʒm */
#include "CHAR/PacketCHAR_REQ_PUSH.h"					/* 			�����v�� */
#include "CHAR/PacketCHAR_REQ_TAIL.h"					/* 			�t���čs���v�� */
#include "CHAR/PacketCHAR_RES_TAIL.h"					/* 			�t���čs������ */
#include "CHAR/PacketCHAR_REQ_MODIFY_PARAM.h"			/* 			�p�����[�^�ύX�v�� */
#include "CHAR/PacketCHAR_MODIFY_PARAM.h"				/* 			�p�����[�^�ύX�ʒm */
#include "CHAR/PacketCHAR_REQ_EQUIP.h"					/* 			�����v�� */
#include "CHAR/PacketCHAR_GRP.h"						/* 			�摜�ʒm */
#include "CHAR/PacketCHAR_MOTION.h"						/* 			���[�V�������ʒm */
#include "CHAR/PacketCHAR_MOTIONTYPE.h"					/* 			���[�V������ʏ��ʒm */
#include "CHAR/PacketCHAR_SET_EFCBALLOON.h"				/* 			���o���w�� */
#include "CHAR/PacketCHAR_PARA1.h"						/* 			�ėp[�p�����[�^1��] */
#include "CHAR/PacketCHAR_STATUS.h"						/* 			�X�e�[�^�X���ʒm */
#include "CHAR/PacketCHAR_TEXTEFFECT.h"					/* 			�����G�t�F�N�g�ʒm */
#include "CHAR/PacketCHAR_RES_TALKEVENT.h"				/* 			��b�C�x���g��񉞓� */
#include "MAP/PacketMAP_REQ_MAPINFO.h"					/* �}�b�v�n:�}�b�v���v�� */
#include "MAP/PacketMAP_RES_MAPINFO.h"					/* 			�}�b�v��񉞓� */
#include "MAP/PacketMAP_ONLINE.h"						/* 			�I�����C�����ʒm */
#include "MAP/PacketMAP_SYSTEMMSG.h"					/* 			�V�X�e�����b�Z�[�W�ʒm */
#include "MAP/PacketMAP_FORMATMSG.h"					/* 			�t�H�[�}�b�g���b�Z�[�W�ʒm */
#include "MAP/PacketMAP_MAPOBJECT.h"					/* 			�}�b�v�I�u�W�F�N�g���ʒm */
#include "MAP/PacketMAP_MAPOBJECTDATA.h"				/* 			�}�b�v�I�u�W�F�N�g�z�u�f�[�^�ʒm */
#include "MAP/PacketMAP_MAPPARTS.h"						/* 			�}�b�v�p�[�c���ʒm */
#include "MAP/PacketMAP_SETPARTS.h"						/* 			�}�b�v�p�[�c�z�u */
#include "MAP/PacketMAP_RENEWMAPSIZE.h"					/* 			�}�b�v�T�C�Y�X�V */
#include "MAP/PacketMAP_DELETEPARTS.h"					/* 			�}�b�v�p�[�c�폜 */
#include "MAP/PacketMAP_MAPSHADOW.h"					/* 			�}�b�v�e���ʒm */
#include "MAP/PacketMAP_SETMAPSHADOW.h"					/* 			�}�b�v�e�z�u */
#include "MAP/PacketMAP_DELETEMAPSHADOW.h"				/* 			�}�b�v�e�폜 */
#include "MAP/PacketMAP_MAPNAME.h"						/* 			�}�b�v���ʒm */
#include "MAP/PacketMAP_MAPEVENT.h"						/* 			�}�b�v�C�x���g���ʒm */
#include "MAP/PacketMAP_PARA1.h"						/* 			�ėp[�p�����[�^1��] */
#include "ADMIN/PacketADMIN_CHARINFO.h"					/* �Ǘ��Ҍn:�L�������ʒm */
#include "ADMIN/PacketADMIN_MAP_RENEWMAPOBJECT.h"		/* 			�}�b�v�I�u�W�F�N�g�X�V */
#include "ADMIN/PacketADMIN_MAP_RENEWOBJECTDATA.h"		/* 			�}�b�v�I�u�W�F�N�g�z�u�f�[�^�X�V */
#include "ADMIN/PacketADMIN_RENEWMAPPARTS.h"			/* 			�}�b�v�p�[�c�X�V */
#include "ADMIN/PacketADMIN_MAP_SETPARTS.h"				/* 			�}�b�v�p�[�c�z�u */
#include "ADMIN/PacketADMIN_MAP_RENEWMAPSIZE.h"			/* 			�}�b�v�T�C�Y�X�V */
#include "ADMIN/PacketADMIN_MAP_DELETEPARTS.h"			/* 			�}�b�v�p�[�c�폜 */
#include "ADMIN/PacketADMIN_MAP_COPYPARTS.h"			/* 			�}�b�v�p�[�c�R�s�[ */
#include "ADMIN/PacketADMIN_MAP_SETMAPNAME.h"			/* 			�}�b�v���ύX */
#include "ADMIN/PacketADMIN_RENEWMAPSHADOW.h"			/* 			�}�b�v�e�X�V */
#include "ADMIN/PacketADMIN_MAP_SETMAPSHADOW.h"			/* 			�}�b�v�e�z�u */
#include "ADMIN/PacketADMIN_MAP_DELETEMAPSHADOW.h"		/* 			�}�b�v�e�폜 */
#include "ADMIN/PacketADMIN_MAP_EVENT.h"				/*			�}�b�v�C�x���g���X�V */
#include "ADMIN/PacketADMIN_SKILL_RENEWSKILL.h"			/*			�X�L�����X�V */
#include "ADMIN/PacketADMIN_REQ_ADMINLEVEL.h"			/* 			�Ǘ��Ҍ������x���v�� */
#include "ADMIN/PacketADMIN_RES_ADMINLEVEL.h"			/* 			�Ǘ��Ҍ������x������ */
#include "ADMIN/PacketADMIN_RENEWADMINLEVEL.h"			/* 			�Ǘ��Ҍ������x���X�V */
#include "ADMIN/PacketADMIN_SERVER_SAVEINFO.h"			/* 			�T�[�o�[���ۑ� */
#include "ADMIN/PacketADMIN_ITEM_ADD.h"					/* 			�A�C�e�����ǉ� */
#include "ADMIN/PacketADMIN_ITEM_COPY.h"				/* 			�A�C�e�����R�s�[ */
#include "ADMIN/PacketADMIN_ITEM_DELETE.h"				/* 			�A�C�e�����폜 */
#include "ADMIN/PacketADMIN_ITEMTYPE_ADD.h"				/* 			�A�C�e����ʏ��ǉ� */
#include "ADMIN/PacketADMIN_ITEMTYPE_COPY.h"			/* 			�A�C�e����ʏ��R�s�[ */
#include "ADMIN/PacketADMIN_ITEMTYPE_DELETE.h"			/* 			�A�C�e����ʏ��폜 */
#include "ADMIN/PacketADMIN_ITEMWEAPON_ADD.h"			/* 			������ǉ� */
#include "ADMIN/PacketADMIN_ITEMWEAPON_RENEW.h"			/* 			������X�V */
#include "ADMIN/PacketADMIN_CHAR_ADDNPC.h"				/* 			NPC�̒ǉ� */
#include "ADMIN/PacketADMIN_CHAR_MODIFYITEM.h"			/* 			�����A�C�e���̕ύX */
#include "ADMIN/PacketADMIN_CHAR_RENEWMOTION.h"			/* 			�L�������[�V�������̍X�V */
#include "ADMIN/PacketADMIN_CHAR_ADDMOTIONTYPE.h"		/* 			�L�������[�V������ʏ��̒ǉ� */
#include "ADMIN/PacketADMIN_CHAR_RENEWMOTIONTYPE.h"		/* 			�L�������[�V������ʏ��̍X�V */
#include "ADMIN/PacketADMIN_CHAR_RENEWSTATUS.h"			/* 			�X�e�[�^�X���X�V */
#include "ADMIN/PacketADMIN_CHAR_RES_ONLINE.h"			/* 			�I�����C�����L�����ꗗ���� */
#include "ADMIN/PacketADMIN_EFC_RENEWBALLOON.h"			/* 			���o�����̍X�V */
#include "ADMIN/PacketADMIN_EFC_RENEWEFFECT.h"			/* 			�G�t�F�N�g���̍X�V */
#include "ADMIN/PacketADMIN_REQ_PLAYSOUND.h"			/* 			���ʉ��̍Đ��v�� */
#include "ADMIN/PacketADMIN_PLAYSOUND.h"				/* 			���ʉ��̍Đ� */
#include "ADMIN/PacketADMIN_CHAR_REQ_ACCOUNT.h"			/*			�A�J�E���g���v�� */
#include "ADMIN/PacketADMIN_CHAR_RES_ACCOUNT.h"			/*			�A�J�E���g��񉞓� */
#include "ADMIN/PacketADMIN_CHAR_RENEW_ACCOUNT.h"		/*			�A�J�E���g���X�V */
#include "ADMIN/PacketADMIN_CHAR_RENEW_TALKEVENT.h"		/* 			��b�C�x���g���X�V */
#include "ADMIN/PacketADMIN_RENEW_CLIENTVERSION.h"		/*			�N���C�A���g�o�[�W�����X�V */
#include "ADMIN/PacketADMIN_PARA2.h"					/* 			�ėp[�p�����[�^2��] */
#include "ADMIN/PacketADMIN_SYSTEM_RENEWINFO.h"			/*			�V�X�e�����̍X�V */
#include "MSGCMD/PacketMSGCMD_CHGFACE.h"				/* ���b�Z�[�W�R�}���h�n:�\��ύX */
#include "MSGCMD/PacketMSGCMD_CHGCLOTH.h"				/*						�����ύX */
#include "MSGCMD/PacketMSGCMD_CHGACCE.h"				/*						�A�N�Z�T���ύX */
#include "MSGCMD/PacketMSGCMD_CHGCOLOR.h"				/*						�F�ύX */
#include "MSGCMD/PacketMSGCMD_CHGARMS.h"				/*						�������ύX */
#include "MSGCMD/PacketMSGCMD_MAKEITEM.h"				/*						�A�C�e���쐬 */
#include "MSGCMD/PacketMSGCMD_PARA1.h"					/*						�ėp[�p�����[�^1��] */
#include "ITEM/PacketITEM_REQ_ITEMINFO.h"				/* �A�C�e���n:	�A�C�e�����v�� */
#include "ITEM/PacketITEM_RES_ITEMINFO.h"				/* 				�A�C�e����񉞓� */
#include "ITEM/PacketITEM_ITEMINFO.h"					/* 				�A�C�e�����ʒm */
#include "ITEM/PacketITEM_DELETEITEMINFO.h"				/* 				�A�C�e�����폜 */
#include "ITEM/PacketITEM_RENEWITEMINFO.h"				/* 				�A�C�e�����X�V */
#include "ITEM/PacketITEM_REQ_ITEMTYPEINFO.h"			/* 				�A�C�e����ʏ��v�� */
#include "ITEM/PacketITEM_RES_ITEMTYPEINFO.h"			/* 				�A�C�e����ʏ�񉞓� */
#include "ITEM/PacketITEM_ITEMTYPEINFO.h"				/* 				�A�C�e����ʏ��ʒm */
#include "ITEM/PacketITEM_DELETEITEMTYPEINFO.h"			/* 				�A�C�e����ʏ��폜 */
#include "ITEM/PacketITEM_RENEWITEMTYPEINFO.h"			/* 				�A�C�e����ʏ��X�V */
#include "ITEM/PacketITEM_REQ_ITEMWEAPONINFO.h"			/* 				������v�� */
#include "ITEM/PacketITEM_RES_ITEMWEAPONINFO.h"			/* 				�����񉞓� */
#include "ITEM/PacketITEM_ITEMWEAPONINFO.h"				/* 				������ʒm */
#include "EFFECT/PacketEFFECT_REQ_EFFECTINFO.h"			/* �G�t�F�N�g�n:�G�t�F�N�g���v�� */
#include "EFFECT/PacketEFFECT_RES_EFFECTINFO.h"			/* 				�G�t�F�N�g��񉞓� */
#include "EFFECT/PacketEFFECT_EFFECTINFO.h"				/* 				�G�t�F�N�g���ʒm */
#include "EFFECT/PacketEFFECT_DELETEEFFECTINFO.h"		/* 				�G�t�F�N�g���폜 */
#include "EFFECT/PacketEFFECT_REQ_BALLOONINFO.h"		/* 				���o�����v�� */
#include "EFFECT/PacketEFFECT_BALLOONINFO.h"			/* 				���o�����ʒm */
#include "EFFECT/PacketEFFECT_DELETEBALLOONINFO.h"		/* 				���o�����폜 */
#include "BATTLE/PacketBATTLE_REQ_ATACK.h"				/* �퓬�n:	�U���v�� */
#include "SYSTEM/PacketSYSTEM_INFO.h"					/* �V�X�e���n:	�V�X�e�����ʒm */
#include "SKILL/PacketSKILL_PARA2.h"					/* �X�L���n:	�ėp[�p�����[�^2��] */
#include "SKILL/PacketSKILL_SKILLINFO.h"				/* 				�X�L�����ʒm */

/* Copyright(C)URARA-works 2006 */
