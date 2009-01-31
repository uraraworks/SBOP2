/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:SBOGlobal.h												 */
/* 内容			:プロジェクト依存ではない共通処理 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/06													 */
/* ========================================================================= */

#pragma once

class CmyString;

extern void sgenrand(unsigned long);								/* 乱数初期化 */
extern long genrand();												/* 乱数取得 */

extern PBYTE	ZeroNew				(DWORD dwSize);												/* 指定サイズのメモリを確保して0クリア */
extern void		CopyMemoryRenew		(PVOID pDst, CONST PVOID pSrc, DWORD dwSize, PBYTE &pPos);	/* メモリコピーしてポインタを進める */
extern void		strcpyRenew			(LPSTR pszDst, LPCSTR pszSrc, PBYTE &pPos);					/* 文字列コピーしてポインタを進める */
extern void		StoreRenew			(CmyString &strDst, LPCSTR pszSrc, PBYTE &pPos);			/* 文字列コピーしてポインタを進める */
extern void		GetModuleFilePath	(LPSTR pszDst, DWORD dwSize);								/* モジュールパスを取得 */
extern BOOL		AllCreateDirectory	(LPCSTR, LPSECURITY_ATTRIBUTES pSAtt = NULL, BOOL bResult = TRUE);	/* 指定されたパスのフォルダを作成 */
extern void		TrimViewString		(CmyString &strDst, LPCSTR pszSrc);							/* 表示できない文字を削除 */
extern BOOL		IsInRect			(RECT *pSrc, RECT *pTarget);								/* 指定矩形が完全に内側にあるか判定 */
extern BOOL		IsHitRect			(RECT *pSrc1, RECT *pSrc2);									/* 矩形同士の当たり判定 */

/* Copyright(C)URARA-works 2006 */
