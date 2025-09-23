#include "StdAfx.h"
#include <tchar.h>
#include "myString.h"

CString Utf8ToTString(LPCSTR pszSrc)
{
#ifdef _UNICODE
        CString strResult;
        if (pszSrc == NULL) {
                return strResult;
        }
        int nLen = MultiByteToWideChar (CP_UTF8, 0, pszSrc, -1, NULL, 0);
        if (nLen <= 0) {
                return strResult;
        }
        LPTSTR pszBuffer = strResult.GetBuffer (nLen);
        MultiByteToWideChar (CP_UTF8, 0, pszSrc, -1, pszBuffer, nLen);
        strResult.ReleaseBuffer ();
        return strResult;
#else
        CString strResult;
        if (pszSrc) {
                strResult = pszSrc;
        }
        return strResult;
#endif
}

CStringA TStringToUtf8(LPCTSTR pszSrc)
{
#ifdef _UNICODE
        CStringA strResult;
        if (pszSrc == NULL) {
                return strResult;
        }
        int nLen = WideCharToMultiByte (CP_UTF8, 0, pszSrc, -1, NULL, 0, NULL, NULL);
        if (nLen <= 0) {
                return strResult;
        }
        LPSTR pszBuffer = strResult.GetBuffer (nLen);
        WideCharToMultiByte (CP_UTF8, 0, pszSrc, -1, pszBuffer, nLen, NULL, NULL);
        strResult.ReleaseBuffer ();
        return strResult;
#else
        CStringA strResult;
        if (pszSrc) {
                strResult = pszSrc;
        }
        return strResult;
#endif
}

/* ========================================================================= */
/* 関数名       :CmyString::CmyString                                                                                    */
/* 内容         :コンストラクタ
*/
/* 日付         :2006/05/25
*/
/* ========================================================================= */

CmyString::CmyString()
{
        m_strString.Empty ();
        m_strUtf8Cache.Empty ();
        m_bUtf8Dirty = TRUE;
}


/* ========================================================================= */
/* 関数名       :CmyString::CmyString                                                                                    */
/* 内容         :コピーコンストラクタ                                                                                    */
/* 日付         :2007/02/11
*/
/* ========================================================================= */

CmyString::CmyString(const CmyString &strSrc)
{
        m_strString     = strSrc.m_strString;
        m_strUtf8Cache  = strSrc.m_strUtf8Cache;
        m_bUtf8Dirty    = strSrc.m_bUtf8Dirty;
}

CmyString::CmyString(LPCSTR szSrc)
{
        RenewUtf8 (szSrc);
}

CmyString::CmyString(LPCTSTR szSrc)
{
        RenewWide (szSrc);
}


/* ========================================================================= */
/* 関数名       :CmyString::~CmyString                                                                                   */
/* 内容         :デストラクタ
*/
/* 日付         :2006/05/25
*/
/* ========================================================================= */

CmyString::~CmyString()
{
}


/* ========================================================================= */
/* 関数名       :CmyString::Empty
*/
/* 内容         :文字列を空にする
*/
/* 日付         :2006/05/29
*/
/* ========================================================================= */

void CmyString::Empty(void)
{
        m_strString.Empty ();
        m_strUtf8Cache.Empty ();
        m_bUtf8Dirty = FALSE;
}


/* ========================================================================= */
/* 関数名       :CmyString::IsEmpty
*/
/* 内容         :文字列が空か判定
*/
/* 日付         :2006/05/29
*/
/* ========================================================================= */

BOOL CmyString::IsEmpty(void) const
{
        return m_strString.IsEmpty ();
}


/* ========================================================================= */
/* 関数名       :CmyString::GetLength                                                                                    */
/* 内容         :文字列長を取得
*/
/* 日付         :2006/05/30
*/
/* ========================================================================= */

int CmyString::GetLength(void) const
{
        return m_strString.GetLength ();
}


/* ========================================================================= */
/* 関数名       :CmyString::Format
*/
/* 内容         :書式文字列で初期化
*/
/* 日付         :2006/06/02
*/
/* ========================================================================= */

void CmyString::Format(LPCSTR lpFormat, ...)
{
        if (lpFormat == NULL) {
                Empty ();
                return;
        }
        va_list argList;
        va_start (argList, lpFormat);
#ifdef _UNICODE
        CStringA strTmp;
        strTmp.FormatV (lpFormat, argList);
        RenewUtf8 (strTmp);
#else
        CStringA strTmp;
        strTmp.FormatV (lpFormat, argList);
        m_strString = strTmp;
        m_bUtf8Dirty = TRUE;
#endif
        va_end (argList);
}

void CmyString::Format(LPCTSTR lpFormat, ...)
{
        if (lpFormat == NULL) {
                Empty ();
                return;
        }
        va_list argList;
        va_start (argList, lpFormat);
        m_strString.FormatV (lpFormat, argList);
        va_end (argList);
        m_bUtf8Dirty = TRUE;
}


/* ========================================================================= */
/* 関数名       :CmyString::operator =                                                                                   */
/* 内容         :文字列を初期
*/
/* 日付         :2006/05/25
*/
/* ========================================================================= */

void CmyString::operator =(const CmyString &strSrc)
{
        if (this == &strSrc) {
                return;
        }
        m_strString     = strSrc.m_strString;
        m_strUtf8Cache  = strSrc.m_strUtf8Cache;
        m_bUtf8Dirty    = strSrc.m_bUtf8Dirty;
}


/* ========================================================================= */
/* 関数名       :CmyString::operator =                                                                                   */
/* 内容         :文字列を初期化
*/
/* 日付         :2006/05/25
*/
/* ========================================================================= */

void CmyString::operator =(LPCSTR pszSrc)
{
        RenewUtf8 (pszSrc);
}

void CmyString::operator =(LPCTSTR pszSrc)
{
        RenewWide (pszSrc);
}


/* ========================================================================= */
/* 関数名       :CmyString::operator +=                                                                                  */
/* 内容         :文字列を追加
*/
/* 日付         :2006/05/25
*/
/* ========================================================================= */

void CmyString::operator +=(LPCSTR pszSrc)
{
#ifdef _UNICODE
        m_strString += Utf8ToTString (pszSrc);
#else
        if (pszSrc) {
                m_strString += pszSrc;
        }
#endif
        m_bUtf8Dirty = TRUE;
}

void CmyString::operator +=(LPCTSTR pszSrc)
{
        if (pszSrc) {
                m_strString += pszSrc;
                m_bUtf8Dirty = TRUE;
        }
}


/* ========================================================================= */
/* 関数名       :CmyString::operator ==                                                                                  */
/* 内容         :文字列を比較
*/
/* 日付         :2006/05/25
*/
/* ========================================================================= */

BOOL CmyString::operator ==(LPCSTR pszSrc) const
{
#ifdef _UNICODE
        CString strTmp = Utf8ToTString (pszSrc);
        return (m_strString.Compare (strTmp) == 0);
#else
        if (pszSrc == NULL) {
                return m_strString.IsEmpty ();
        }
        return (m_strString.Compare (pszSrc) == 0);
#endif
}

BOOL CmyString::operator ==(LPCTSTR pszSrc) const
{
        return (m_strString.Compare (pszSrc) == 0);
}


/* ========================================================================= */
/* 関数名       :CmyString::operator !=                                                                                  */
/* 内容         :文字列を比較
*/
/* 日付         :2006/05/25
*/
/* ========================================================================= */

BOOL CmyString::operator !=(LPCSTR pszSrc) const
{
        return !(*this == pszSrc);
}

BOOL CmyString::operator !=(LPCTSTR pszSrc) const
{
        return !(*this == pszSrc);
}


/* ========================================================================= */
/* 関数名       :CmyString::operator LPCTSTR                                                                             */
/* 内容         :キャスト
*/
/* 日付         :2006/05/25
*/
/* ========================================================================= */

CmyString::operator LPCTSTR() const
{
        return (LPCTSTR)m_strString;
}

CmyString::operator LPCTSTR()
{
        return (LPCTSTR)m_strString;
}

CmyString::operator LPCSTR() const
{
#ifdef _UNICODE
        UpdateUtf8Cache ();
        return (LPCSTR)m_strUtf8Cache;
#else
        return (LPCSTR)m_strString;
#endif
}

CmyString::operator LPCSTR()
{
#ifdef _UNICODE
        UpdateUtf8Cache ();
        return (LPCSTR)m_strUtf8Cache;
#else
        return (LPCSTR)m_strString;
#endif
}


/* ========================================================================= */
/* 関数名       :CmyString::CompareNoCase                                                                                */
/* 内容         :文字列比較(大文字小文字区別無し)
*/
/* 日付         :2006/09/26
*/
/* ========================================================================= */

int CmyString::CompareNoCase(LPCSTR pszSrc) const
{
#ifdef _UNICODE
        CString strTmp = Utf8ToTString (pszSrc);
        return m_strString.CompareNoCase (strTmp);
#else
        if (pszSrc == NULL) {
                return m_strString.IsEmpty () ? 0 : 1;
        }
        return m_strString.CompareNoCase (pszSrc);
#endif
}

int CmyString::CompareNoCase(LPCTSTR pszSrc) const
{
        return m_strString.CompareNoCase (pszSrc);
}


/* ========================================================================= */
/* 関数名       :CmyString::RenewUtf8
*/
/* 内容         :UTF-8文字列を更新
*/
/* ========================================================================= */

void CmyString::RenewUtf8(LPCSTR pszSrc)
{
#ifdef _UNICODE
        m_strString = Utf8ToTString (pszSrc);
#else
        if (pszSrc == NULL) {
                m_strString.Empty ();
        } else {
                m_strString = pszSrc;
        }
#endif
        m_bUtf8Dirty = TRUE;
}


/* ========================================================================= */
/* 関数名       :CmyString::RenewWide
*/
/* 内容         :Unicode文字列を更新
*/
/* ========================================================================= */

void CmyString::RenewWide(LPCTSTR pszSrc)
{
        if (pszSrc == NULL) {
                m_strString.Empty ();
        } else {
                m_strString = pszSrc;
        }
        m_bUtf8Dirty = TRUE;
}


/* ========================================================================= */
/* 関数名       :CmyString::UpdateUtf8Cache
*/
/* 内容         :UTF-8キャッシュを更新
*/
/* ========================================================================= */

void CmyString::UpdateUtf8Cache() const
{
#ifdef _UNICODE
        if (!m_bUtf8Dirty) {
                return;
        }
        m_strUtf8Cache = TStringToUtf8 ((LPCTSTR)m_strString);
        const_cast<CmyString *>(this)->m_bUtf8Dirty = FALSE;
#else
        UNREFERENCED_PARAMETER (this);
#endif
}

/* Copyright(C)URARA-works 2006 */
