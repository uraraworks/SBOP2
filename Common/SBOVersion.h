/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:SBOVersion.h												 */
/* ���e			:�o�[�W������`�t�@�C��										 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/07													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

#define VERTEXT					"0.039.1"							/* �o�[�W���� */
#define VERSIONVAL				(0x00003901)						/* �o�[�W�����ԍ� */
#define DLLVER					"1"									/* DLL�o�[�W���� */
#define GRPDATADLLVER			"4"									/* �O���t�B�b�N�f�[�^DLL�o�[�W���� */

/* ������[FILEVERSION]�E[PRODUCTVERSION]��ύX����K�v������܂� */

/****************************************************************************
	�萔�̒�`
****************************************************************************/

/* �t�@�C���t���O�萔 */
#ifdef _DEBUG
#define FILEFLAGS_VALUE		(0x1L)
#else
#define	FILEFLAGS_VALUE		(0x0L)
#endif

/* �t�@�C���^�C�v�萔 */
#if(defined(_USRDLL) || defined(_WINDLL))
#define	FILETYPE_VALUE		(0x2L)
#else
#define	FILETYPE_VALUE		(0x1L)
#endif

/****************************************************************************
	�o�[�W�������\�[�X��`�}�N��
****************************************************************************/

#define	SBO_VERSION(OriginalFilename)																	\
																										\
	VS_VERSION_INFO VERSIONINFO																			\
		FILEVERSION 2,0,0,0																				\
		PRODUCTVERSION 2,0,0,0																			\
		FILEFLAGSMASK 0x3fL																				\
		FILEFLAGS FILEFLAGS_VALUE																		\
		FILEOS 0x40004L																					\
		FILETYPE FILETYPE_VALUE																			\
		FILESUBTYPE 0x0L																				\
	BEGIN																								\
		BLOCK "StringFileInfo"																			\
		BEGIN																							\
			BLOCK "041104b0"																			\
			BEGIN																						\
				VALUE "Comments", "�X�N���b�v�u�b�N�I�����C��\0"										\
				VALUE "CompanyName", "URARA-WORKS\0"													\
				VALUE "FileDescription", "�X�N���b�v�u�b�N�I�����C��\0"									\
				VALUE "FileVersion", VERTEXT "\0"														\
				VALUE "InternalName", " \0"																\
				VALUE "LegalCopyright", "Copyright (C)2003-2009 URARA-WORKS. All rights reserved.\0"	\
				VALUE "LegalTrademarks", "�X�N���b�v�u�b�N�I�����C��\0"									\
				VALUE "OriginalFilename", OriginalFilename "\0"											\
				VALUE "PrivateBuild", " \0"																\
				VALUE "ProductName", "�X�N���b�v�u�b�N�I�����C��\0"										\
				VALUE "ProductVersion", VERTEXT "\0"													\
				VALUE "SpecialBuild", " \0"																\
			END																							\
		END																								\
		BLOCK "VarFileInfo"																				\
		BEGIN																							\
			VALUE "Translation", 0x411, 1200															\
		END																								\
	END

/* Copyright(C)URARA-works 2008 */
