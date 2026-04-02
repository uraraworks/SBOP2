#include "StdAfx.h"
#if !defined(__EMSCRIPTEN__)
#include <tchar.h>
#endif
#include "myString.h"

// コンストラクタ

CmyString::CmyString()
{
	m_strString.Empty();
	m_strUtf8Cache.Empty();
	m_strAnsiCache.Empty();
	m_bUtf8Dirty = TRUE;
	m_bAnsiDirty = TRUE;
	m_uAnsiCodePage = CP_UTF8;
}

// コピーコンストラクタ

CmyString::CmyString(const CmyString &strSrc)
{
	m_strString     = strSrc.m_strString;
	m_strUtf8Cache  = strSrc.m_strUtf8Cache;
	m_strAnsiCache  = strSrc.m_strAnsiCache;
	m_bUtf8Dirty    = strSrc.m_bUtf8Dirty;
	m_bAnsiDirty    = strSrc.m_bAnsiDirty;
	m_uAnsiCodePage = strSrc.m_uAnsiCodePage;
}

CmyString::CmyString(LPCSTR szSrc)
{
	RenewUtf8(szSrc);
}

CmyString::CmyString(LPCTSTR szSrc)
{
	RenewWide(szSrc);
}

// デストラクタ

CmyString::~CmyString()
{
}

// 文字列を空にする

void CmyString::Empty(void)
{
	m_strString.Empty();
	m_strUtf8Cache.Empty();
	m_strAnsiCache.Empty();
	m_bUtf8Dirty = FALSE;
	m_bAnsiDirty = FALSE;
	m_uAnsiCodePage = CP_UTF8;
}

// 文字列が空か判定

BOOL CmyString::IsEmpty(void) const
{
	return m_strString.IsEmpty();
}

// 文字列長を取得

int CmyString::GetLength(void) const
{
	return m_strString.GetLength();
}

// 書式文字列で初期化

void CmyString::Format(LPCSTR lpFormat, ...)
{
	if (lpFormat == NULL) {
		Empty();
		return;
	}
	va_list argList;
	va_start(argList, lpFormat);
#ifdef _UNICODE
	CStringA strTmp;
	strTmp.FormatV(lpFormat, argList);
	RenewUtf8(strTmp);
#else
	CStringA strTmp;
	strTmp.FormatV(lpFormat, argList);
	m_strString = strTmp;
	m_bUtf8Dirty = TRUE;
	m_bAnsiDirty = TRUE;
#endif
	va_end(argList);
}

void CmyString::Format(LPCTSTR lpFormat, ...)
{
	if (lpFormat == NULL) {
		Empty();
		return;
	}
	va_list argList;
	va_start(argList, lpFormat);
	m_strString.FormatV(lpFormat, argList);
	va_end(argList);
	m_bUtf8Dirty = TRUE;
	m_bAnsiDirty = TRUE;
}

// 文字列を初期化(CmyString)

void CmyString::operator =(const CmyString &strSrc)
{
	if (this == &strSrc) {
		return;
	}
	m_strString     = strSrc.m_strString;
	m_strUtf8Cache  = strSrc.m_strUtf8Cache;
	m_strAnsiCache  = strSrc.m_strAnsiCache;
	m_bUtf8Dirty    = strSrc.m_bUtf8Dirty;
	m_bAnsiDirty    = strSrc.m_bAnsiDirty;
	m_uAnsiCodePage = strSrc.m_uAnsiCodePage;
}

// 文字列を初期化(LPCSTR/LPCTSTR)

void CmyString::operator =(LPCSTR pszSrc)
{
	RenewUtf8(pszSrc);
}

void CmyString::operator =(LPCTSTR pszSrc)
{
	RenewWide(pszSrc);
}

// 文字列を追加

void CmyString::operator +=(LPCSTR pszSrc)
{
#ifdef _UNICODE
	m_strString += Utf8ToTString(pszSrc);
#else
	if (pszSrc) {
		m_strString += pszSrc;
	}
#endif
	m_bUtf8Dirty = TRUE;
	m_bAnsiDirty = TRUE;
}

void CmyString::operator +=(LPCTSTR pszSrc)
{
	if (pszSrc) {
		m_strString += pszSrc;
		m_bUtf8Dirty = TRUE;
		m_bAnsiDirty = TRUE;
	}
}

// 文字列を比較(==)

BOOL CmyString::operator ==(LPCSTR pszSrc) const
{
#ifdef _UNICODE
	CString strTmp = Utf8ToTString(pszSrc);
	return (m_strString.Compare(strTmp) == 0);
#else
	if (pszSrc == NULL) {
		return m_strString.IsEmpty();
	}
	return (m_strString.Compare(pszSrc) == 0);
#endif
}

BOOL CmyString::operator ==(LPCTSTR pszSrc) const
{
	return (m_strString.Compare(pszSrc) == 0);
}

// 文字列を比較(!=)

BOOL CmyString::operator !=(LPCSTR pszSrc) const
{
	return !(*this == pszSrc);
}

BOOL CmyString::operator !=(LPCTSTR pszSrc) const
{
	return !(*this == pszSrc);
}

// キャスト

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
	return GetLegacyAnsiPointer();
#else
	return (LPCSTR)m_strString;
#endif
}

CmyString::operator LPCSTR()
{
#ifdef _UNICODE
	return GetLegacyAnsiPointer();
#else
	return (LPCSTR)m_strString;
#endif
}

LPCSTR CmyString::GetUtf8Pointer() const
{
#ifdef _UNICODE
	UpdateUtf8Cache();
	return (LPCSTR)m_strUtf8Cache;
#else
	return (LPCSTR)m_strString;
#endif
}

LPCSTR CmyString::GetAnsiPointer(UINT codePage) const
{
#ifdef _UNICODE
	UpdateAnsiCache(codePage);
	return (LPCSTR)m_strAnsiCache;
#else
	UNREFERENCED_PARAMETER(codePage);
	return (LPCSTR)m_strString;
#endif
}

LPCSTR CmyString::GetLegacyAnsiPointer() const
{
	return GetAnsiPointer(SBO_LEGACY_CODEPAGE);
}

int CmyString::GetStoreLength(UINT codePage) const
{
#ifdef _UNICODE
	UpdateAnsiCache(codePage);
	return m_strAnsiCache.GetLength();
#else
	UNREFERENCED_PARAMETER(codePage);
	return m_strString.GetLength();
#endif
}

int CmyString::GetLegacyStoreLength() const
{
	return GetStoreLength(SBO_LEGACY_CODEPAGE);
}

// 文字列比較(大文字小文字区別無し)

int CmyString::CompareNoCase(LPCSTR pszSrc) const
{
#ifdef _UNICODE
	CString strTmp = Utf8ToTString(pszSrc);
	return m_strString.CompareNoCase(strTmp);
#else
	if (pszSrc == NULL) {
		return m_strString.IsEmpty() ? 0 : 1;
	}
	return m_strString.CompareNoCase(pszSrc);
#endif
}

int CmyString::CompareNoCase(LPCTSTR pszSrc) const
{
	return m_strString.CompareNoCase(pszSrc);
}

// UTF-8文字列を更新

void CmyString::RenewUtf8(LPCSTR pszSrc)
{
#ifdef _UNICODE
	m_strString = Utf8ToTString(pszSrc);
#else
	if (pszSrc == NULL) {
		m_strString.Empty();
	} else {
		m_strString = pszSrc;
	}
#endif
	m_bUtf8Dirty = TRUE;
	m_bAnsiDirty = TRUE;
}

// Unicode文字列を更新

void CmyString::RenewWide(LPCTSTR pszSrc)
{
	if (pszSrc == NULL) {
		m_strString.Empty();
	} else {
		m_strString = pszSrc;
	}
	m_bUtf8Dirty = TRUE;
	m_bAnsiDirty = TRUE;
}

// UTF-8キャッシュを更新

void CmyString::UpdateUtf8Cache() const
{
#ifdef _UNICODE
	if (!m_bUtf8Dirty) {
		return;
	}
	m_strUtf8Cache = TStringToUtf8((LPCTSTR)m_strString);
	const_cast<CmyString *>(this)->m_bUtf8Dirty = FALSE;
#else
	UNREFERENCED_PARAMETER(this);
#endif
}

void CmyString::UpdateAnsiCache(UINT codePage) const
{
#ifdef _UNICODE
	if (!m_bAnsiDirty && (m_uAnsiCodePage == codePage)) {
		return;
	}
	m_strAnsiCache = TStringToAnsi((LPCTSTR)m_strString, codePage);
	const_cast<CmyString *>(this)->m_bAnsiDirty = FALSE;
	const_cast<CmyString *>(this)->m_uAnsiCodePage = codePage;
#else
	UNREFERENCED_PARAMETER(codePage);
	const_cast<CmyString *>(this)->m_bAnsiDirty = FALSE;
#endif
}
