#include "StdAfx.h"

/* ========================================================================= */
/* 関数プロトタイプ宣言
*/
/* ========================================================================= */

PBYTE   ZeroNew                 (DWORD dwSize);                                                                                /* 指定サイズのメモリを確保して0クリア */
void    CopyMemoryRenew (PVOID pDst, CONST PVOID pSrc, DWORD dwSize, PBYTE &pPos);      /* メモリコピーしてポインタを進める */
void    strcpyRenew             (LPSTR pszDst, LPCSTR pszSrc, PBYTE &pPos);                                     /* 文字列コピーしてポインタを進める */
void    StoreRenew              (CmyString &strDst, LPCSTR pszSrc, PBYTE &pPos);                        /* 文字列コピーしてポインタを進める */

/* ========================================================================= */
/* 関数名       :ZeroNew
*/
/* 内容         :指定サイズのメモリを確保して0クリア                                                     */
/* 日付         :2006/11/06
*/
/* ========================================================================= */

PBYTE ZeroNew(DWORD dwSize)
{
        PBYTE pRet;

        pRet = new BYTE[dwSize];
        ZeroMemory (pRet, dwSize);

        return pRet;
}


/* ========================================================================= */
/* 関数名       :CopyMemoryRenew
*/
/* 内容         :メモリコピーしてポインタを進める                                                                */
/* 日付         :2006/06/08
*/
/* ========================================================================= */

void CopyMemoryRenew(PVOID pDst, CONST PVOID pSrc, DWORD dwSize, PBYTE &pPos)
{
        CopyMemory (pDst, pSrc, dwSize);
        pPos += dwSize;
}


/* ========================================================================= */
/* 関数名       :strcpyRenew
*/
/* 内容         :文字列コピーしてポインタを進める                                                                */
/* 日付         :2006/06/08
*/
/* ========================================================================= */

void strcpyRenew(LPSTR pszDst, LPCSTR pszSrc, PBYTE &pPos)
{
        if ((pszSrc == NULL) || (strlen (pszSrc) <= 0)) {
                pPos ++;
                return;
        }
        strcpy (pszDst, pszSrc);
        pPos += (strlen (pszSrc) + 1);
}


/* ========================================================================= */
/* 関数名       :StoreRenew
*/
/* 内容         :文字列コピーしてポインタを進める                                                                */
/* 日付         :2006/11/05
*/
/* ========================================================================= */

void StoreRenew(CmyString &strDst, LPCSTR pszSrc, PBYTE &pPos)
{
        if ((pszSrc == NULL) || (strlen (pszSrc) <= 0)) {
                strDst.Empty ();
                pPos ++;
                return;
        }
        strDst = pszSrc;
        pPos += (strlen (pszSrc) + 1);
}


/* ========================================================================= */
/* 関数名       :GetModuleFilePath
*/
/* 内容         :モジュールパスを取得                                                                                    */
/* 日付         :2007/05/03
*/
/* ========================================================================= */

void GetModuleFilePath(
        LPTSTR pszDst,           /* [ou] パス格納先バッファ */
        DWORD dwSize)           /* [in] パス格納先バッファサイズ */
{
        if ((pszDst == NULL) || (dwSize == 0)) {
                return;
        }
        ZeroMemory (pszDst, sizeof (TCHAR) * dwSize);
        GetModuleFileName (NULL, pszDst, dwSize);       /* モジュール名を取得 */
        PathRemoveFileSpec (pszDst);                            /* ファイル名部分を消す */
        PathAddBackslash (pszDst);                                      /* 「\\」を追加 */
}


/* ========================================================================= */
/* 関数名       :AllCreateDirectory
*/
/* 内容         :指定されたパスのフォルダを作成                                                                  */
/* 日付         :2008/03/09
*/
/* ========================================================================= */

BOOL AllCreateDirectory(
        LPCTSTR                                 lpszDir,                        /* [in] ディレクトリ名                         */
        LPSECURITY_ATTRIBUTES   pSAtt,                          /* [in] (基本：NULL)セキュリティ識別子  */
        BOOL                                    bResult                         /* [in] (基本：TRUE)同名パスが存在する時*/
                                                                                                /*              TRUE FALSE どちらを返すか                */
        )
{
        UINT    nStrLen;
        TCHAR   szTempDir[MAX_PATH] = _T("");
        LPTSTR  posRoot;
        TCHAR   ch = _T('\0');


        /*****************************************
                初期チェック
        *****************************************/

        if (lpszDir == NULL) {
                return FALSE;
        }
        nStrLen = _tcslen (lpszDir);
        if ((nStrLen == 0) || (nStrLen >= MAX_PATH)){
                return FALSE;
        }


        /*****************************************
                とりあえず作成
        *****************************************/

        if (CreateDirectory (lpszDir, pSAtt)) {
                return TRUE;
        }

        if (GetLastError () == ERROR_ALREADY_EXISTS) {
                return bResult;
        }


        /*****************************************
                 作成開始位置を検出
        *****************************************/

        _tcscpy (szTempDir, lpszDir);
        szTempDir[MAX_PATH -1] = _T('\0');

        /* ネットワークフォルダを指定されている？ */
        if (_tcsncmp (szTempDir, _T("\\\\"), 2) == 0) {
                posRoot = &szTempDir[1];
                posRoot = _tcschr (posRoot + 1, _T('\\'));
                if (posRoot == NULL) {
                        return FALSE;
                }
                posRoot = _tcschr (posRoot + 1, _T('\\'));
                if (posRoot == NULL) {
                        return FALSE;
                }

        /* ローカルフォルダを指定されている */
        } else {
                posRoot = _tcschr (szTempDir, _T('\\'));
                if (posRoot == NULL) {
                        return FALSE;
                }
        }


        /********************************************
                ディレクトリを先頭から順に作成していく
        ********************************************/

        while (posRoot) {
                posRoot = _tcschr (posRoot + 1, _T('\\'));
                if (posRoot) {
                        ch = *posRoot;
                        /* 文字列を一旦切る（あとで元に戻す）。 */
                        *posRoot = _T('\0');
                }

                if  (!CreateDirectory(szTempDir, pSAtt)) {
                        if (ERROR_ALREADY_EXISTS != GetLastError()) {
                                return FALSE;
                        }
                }
                if (posRoot) {
                        /* 区切った位置を元に戻す */
                        *posRoot = ch;
                }
        }

        return TRUE;
}


/* ========================================================================= */
/* 関数名       :TrimViewString
*/
/* 内容         :表示できない文字を削除                                                                                  */
/* 日付         :2008/06/07
*/
/* ========================================================================= */

void TrimViewString(CmyString &strDst, LPCSTR pszSrc)
{
        if (pszSrc == NULL) {
                strDst.Empty ();
                return;
        }

        CmyString strTmp (pszSrc);
        TrimViewString (strDst, (LPCTSTR)strTmp);
}

void TrimViewString(CmyString &strDst, LPCTSTR pszSrc)
{
        if (pszSrc == NULL) {
                strDst.Empty ();
                return;
        }

        CString strFiltered;
        int nLen = _tcslen (pszSrc);
        for (int i = 0; i < nLen; ++i) {
                TCHAR ch = pszSrc[i];
                if (ch < _T(' ') && ch != _T('\t')) {
                        continue;
                }
                if (ch == 0x7F) {
                        continue;
                }
                strFiltered.AppendChar (ch);
        }
        strDst = (LPCTSTR)strFiltered;
}

/* Copyright(C)URARA-works 2006 */
