/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:SboLaunch_priv.h											 */
/* ���e			:SBO�����`���[�p��`�t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/01/27													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �Q�[����� */
enum {
	GAMESTATE_NONE = 0,
	GAMESTATE_CONNECT,							/* �T�[�o�[�֐ڑ� */
	GAMESTATE_CONNECTMISS,						/* �T�[�o�[�֐ڑ����s */
	GAMESTATE_DISCONNECT,						/* �T�[�o�[�Ɛؒf */
	GAMESTATE_GETFILELIST,						/* �t�@�C�����X�g�擾 */
	GAMESTATE_CHECKFILELIST,					/* �t�@�C�����X�g�`�F�b�N */
	GAMESTATE_GETFILE,							/* �t�@�C���擾 */
	GAMESTATE_MAKEFILELIST,						/* �t�@�C�����X�g�o�� */
	GAMESTATE_RUNUPDATE,						/* �A�b�v�f�[�^�N�� */
	GAMESTATE_RUNSBOCLI,						/* �N���C�A���g�N�� */
	GAMESTATE_QUIT,								/* �I�� */
};

/* Copyright(C)URARA-works 2006 */
