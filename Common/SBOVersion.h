/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SBOVersion.h												 */
/* 内容			:バージョン定義ファイル										 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/07													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

#define VERTEXT					"0.039.1"							/* バージョン */
#define VERSIONVAL				(0x00003901)						/* バージョン番号 */
#define DLLVER					"1"									/* DLLバージョン */
#define GRPDATADLLVER			"4"									/* グラフィックデータDLLバージョン */

/* ※注意[FILEVERSION]・[PRODUCTVERSION]を変更する必要があります */

/****************************************************************************
	定数の定義
****************************************************************************/

/* ファイルフラグ定数 */
#ifdef _DEBUG
#define FILEFLAGS_VALUE		(0x1L)
#else
#define	FILEFLAGS_VALUE		(0x0L)
#endif

/* ファイルタイプ定数 */
#if(defined(_USRDLL) || defined(_WINDLL))
#define	FILETYPE_VALUE		(0x2L)
#else
#define	FILETYPE_VALUE		(0x1L)
#endif

/****************************************************************************
	バージョンリソース定義マクロ
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
				VALUE "Comments", "スクラップブックオンライン\0"										\
				VALUE "CompanyName", "URARA-WORKS\0"													\
				VALUE "FileDescription", "スクラップブックオンライン\0"									\
				VALUE "FileVersion", VERTEXT "\0"														\
				VALUE "InternalName", " \0"																\
				VALUE "LegalCopyright", "Copyright (C)2003-2009 URARA-WORKS. All rights reserved.\0"	\
				VALUE "LegalTrademarks", "スクラップブックオンライン\0"									\
				VALUE "OriginalFilename", OriginalFilename "\0"											\
				VALUE "PrivateBuild", " \0"																\
				VALUE "ProductName", "スクラップブックオンライン\0"										\
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
