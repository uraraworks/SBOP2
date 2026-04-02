/// @file BrowserCompat.h
/// @brief Emscripten 向けの最小 Windows 互換定義
/// @date 2026/04/01
/// @copyright Copyright(C)URARA-works 2026

#pragma once

#if defined(__EMSCRIPTEN__)

#include <SDL.h>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cerrno>
#include <cwctype>
#include <cwchar>
#include <ctime>
#include <cmath>
#include <codecvt>
#include <cstdio>
#include <algorithm>
#include <locale>
#include <map>
#include <sys/stat.h>
#include <string>
#include <vector>

using namespace std;

typedef int BOOL;
typedef unsigned char BYTE;
typedef std::int16_t SHORT;
typedef std::uint16_t WORD;
typedef std::uint32_t DWORD;
typedef std::int32_t LONG;
typedef int INT;
typedef std::uint32_t UINT;
typedef std::uintptr_t UINT_PTR;
typedef std::intptr_t LONG_PTR;
typedef std::intptr_t LPARAM;
typedef std::uintptr_t WPARAM;
typedef std::intptr_t LRESULT;
typedef void VOID;
typedef void *HANDLE;
typedef void *HINSTANCE;
typedef void *HWND;
typedef void *HDC;
typedef void *HFONT;
typedef void *HBITMAP;
typedef void *HGDIOBJ;
typedef void *HMODULE;
typedef void *FARPROC;
typedef void *HGLOBAL;
typedef void *HRSRC;
typedef void *PVOID;
typedef void *LPVOID;
typedef const void *LPCVOID;
typedef char *LPSTR;
typedef const char *LPCSTR;
typedef wchar_t *LPWSTR;
typedef const wchar_t *LPCWSTR;
typedef wchar_t WCHAR;
typedef wchar_t TCHAR;
typedef TCHAR *LPTSTR;
typedef const TCHAR *LPCTSTR;
typedef BYTE *LPBYTE;
typedef const BYTE *LPCBYTE;
typedef BYTE *PBYTE;
typedef WORD *PWORD;
typedef DWORD *LPDWORD;
typedef DWORD *PDWORD;
typedef std::size_t SIZE_T;
typedef int HRESULT;
typedef int errno_t;
typedef DWORD COLORREF;
typedef unsigned long ULONG;
typedef long long INT_PTR;
typedef unsigned long long ULONG_PTR;
typedef std::uint64_t DWORDLONG;
typedef void *HMENU;

#ifndef CONST
#define CONST const
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL 0
#endif

#define CALLBACK
#define WINAPI
#define APIENTRY
#define __stdcall
#define __cdecl
#define __declspec(x)
#define UNREFERENCED_PARAMETER(P) (void)(P)
#define MAX_PATH 260
#define _MAX_PATH 260
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)

#define CP_UTF8 65001
#define CP_ACP 0

#define S_OK ((HRESULT)0)
#define E_FAIL ((HRESULT)0x80004005L)
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)

#define COINIT_APARTMENTTHREADED 0x2
#define COINIT_DISABLE_OLE1DDE 0x4

#define MB_OK 0
#define SW_SHOW 5
#define SW_HIDE 0
#define PM_REMOVE 0x0001
#define QS_ALLINPUT 0
#define BI_RGB 0
#define DIB_RGB_COLORS 0
#define SRCCOPY 0x00CC0020
#define TRANSPARENT 1
#define DT_RIGHT 0x00000002
#define DT_NOCLIP 0x00000100
#define SHIFTJIS_CHARSET 128
#define OUT_DEFAULT_PRECIS 0
#define CLIP_DEFAULT_PRECIS 0
#define DEFAULT_QUALITY 0
#define DEFAULT_PITCH 0
#define FF_DONTCARE 0
#define FW_NORMAL 400
#define FW_ULTRABOLD 800
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define HWND_TOP ((HWND)0)
#define SWP_NOMOVE 0x0002
#define SWP_NOSIZE 0x0001
#define SWP_NOZORDER 0x0004
#define SWP_NOACTIVATE 0x0010
#define FLASHW_TRAY 0x00000002
#define ABS_AUTOHIDE 0x0000001
#define ABM_GETSTATE 0x00000004
#define ERROR_ALREADY_EXISTS 183L

#define WM_APP 0x8000
#define WM_ACTIVATE 0x0006
#define WM_KEYUP 0x0101
#define WM_KEYDOWN 0x0100
#define WM_SYSKEYDOWN 0x0104
#define WM_SYSKEYUP 0x0105
#define WM_MOUSEMOVE 0x0200
#define WM_LBUTTONDOWN 0x0201
#define WM_RBUTTONDOWN 0x0204
#define WM_RBUTTONDBLCLK 0x0206
#define WM_QUIT 0x0012
#define WM_CLOSE 0x0010
#define WM_PAINT 0x000F
#define WM_TIMER 0x0113
#define WM_COMMAND 0x0111

#define VK_RETURN 0x0D
#define VK_BACK 0x08
#define VK_ESCAPE 0x1B
#define VK_TAB 0x09
#define VK_UP 0x26
#define VK_DOWN 0x28
#define VK_LEFT 0x25
#define VK_RIGHT 0x27
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_SPACE 0x20
#define VK_SNAPSHOT 0x2C
#define VK_OEM_3 0xC0
#define BN_CLICKED 0

#define ZeroMemory(Destination, Length) memset((Destination), 0, (Length))
#define CopyMemory(Destination, Source, Length) memcpy((Destination), (Source), (Length))
#define MAKEWORD(a, b) ((WORD)(((BYTE)(((ULONG_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((ULONG_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b) ((LONG)(((WORD)(((ULONG_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((ULONG_PTR)(b)) & 0xffff))) << 16))
#define MAKEINTRESOURCE(i) ((LPCTSTR)((ULONG_PTR)((WORD)(i))))
#define _T(x) L##x
#define TEXT(x) _T(x)

#ifndef RGB
#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#endif

typedef struct tagPOINT {
	LONG x;
	LONG y;
} POINT, *LPPOINT;

typedef struct tagSIZE {
	LONG cx;
	LONG cy;
} SIZE, *LPSIZE;

typedef struct tagRECT {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT, *LPRECT;

typedef struct tagSYSTEMTIME {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} SYSTEMTIME, *LPSYSTEMTIME;

typedef struct _GUID {
	DWORD Data1;
	WORD Data2;
	WORD Data3;
	BYTE Data4[8];
} GUID;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO {
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO;

typedef struct tagPAINTSTRUCT {
	HDC hdc;
	BOOL fErase;
	RECT rcPaint;
	BOOL fRestore;
	BOOL fIncUpdate;
	BYTE rgbReserved[32];
} PAINTSTRUCT, *LPPAINTSTRUCT;

typedef struct tagMSG {
	HWND hwnd;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
	DWORD time;
	POINT pt;
} MSG, *LPMSG;

typedef struct tagFLASHWINFO {
	UINT cbSize;
	HWND hwnd;
	DWORD dwFlags;
	UINT uCount;
	DWORD dwTimeout;
} FLASHWINFO, *PFLASHWINFO;

typedef struct tagAPPBARDATA {
	DWORD cbSize;
	HWND hWnd;
	UINT uCallbackMessage;
	UINT uEdge;
	RECT rc;
	LPARAM lParam;
} APPBARDATA, *PAPPBARDATA;

typedef struct tagSECURITY_ATTRIBUTES {
	DWORD nLength;
	LPVOID lpSecurityDescriptor;
	BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef struct tagCREATESTRUCTW {
	LPVOID lpCreateParams;
	HINSTANCE hInstance;
	HMENU hMenu;
	HWND hwndParent;
	int cy;
	int cx;
	int y;
	int x;
	LONG style;
	LPCWSTR lpszName;
	LPCWSTR lpszClass;
	DWORD dwExStyle;
} CREATESTRUCTW, *LPCREATESTRUCT;

typedef struct tagCRITICAL_SECTION {
	SDL_mutex *pMutex;
} CRITICAL_SECTION, *LPCRITICAL_SECTION;

typedef LRESULT (CALLBACK *WNDPROC)(HWND, UINT, WPARAM, LPARAM);

template<typename TChar>
inline TChar BrowserCompatToLower(TChar ch);

template<>
inline char BrowserCompatToLower(char ch)
{
	return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}

template<>
inline wchar_t BrowserCompatToLower(wchar_t ch)
{
	return static_cast<wchar_t>(std::towlower(ch));
}

template<typename TChar>
class CStringTCompat
{
public:
	typedef std::basic_string<TChar> string_type;

	CStringTCompat()
		: m_data(), m_buffer()
	{
	}

	CStringTCompat(const TChar *pszSrc)
		: m_data((pszSrc != NULL) ? pszSrc : TChar()), m_buffer()
	{
	}

	CStringTCompat(const string_type &strSrc)
		: m_data(strSrc), m_buffer()
	{
	}

	CStringTCompat(const CStringTCompat &strSrc)
		: m_data(strSrc.m_data), m_buffer()
	{
	}

	CStringTCompat &operator =(const CStringTCompat &strSrc)
	{
		if (this != &strSrc) {
			m_data = strSrc.m_data;
			m_buffer.clear();
		}
		return *this;
	}

	CStringTCompat &operator =(const TChar *pszSrc)
	{
		m_data = (pszSrc != NULL) ? pszSrc : string_type();
		m_buffer.clear();
		return *this;
	}

	CStringTCompat &operator +=(const CStringTCompat &strSrc)
	{
		m_data += strSrc.m_data;
		return *this;
	}

	CStringTCompat &operator +=(const TChar *pszSrc)
	{
		if (pszSrc != NULL) {
			m_data += pszSrc;
		}
		return *this;
	}

	bool operator ==(const TChar *pszSrc) const
	{
		return Compare(pszSrc) == 0;
	}

	bool operator !=(const TChar *pszSrc) const
	{
		return !(*this == pszSrc);
	}

	operator const TChar *() const
	{
		return m_data.c_str();
	}

	operator TChar *()
	{
		return GetBuffer(GetLength());
	}

	const TChar *GetString() const
	{
		return m_data.c_str();
	}

	int GetLength() const
	{
		return static_cast<int>(m_data.length());
	}

	bool IsEmpty() const
	{
		return m_data.empty();
	}

	void Empty()
	{
		m_data.clear();
		m_buffer.clear();
	}

	TChar *GetBuffer(int nMinBufferLength)
	{
		if (nMinBufferLength < 0) {
			nMinBufferLength = GetLength();
		}
		const size_t nBufferLength = static_cast<size_t>(nMinBufferLength) + 1;
		m_buffer.assign(nBufferLength, 0);
		if (!m_data.empty()) {
			const size_t nCopyLength = std::min(m_data.length(), nBufferLength - 1);
			std::copy_n(m_data.c_str(), nCopyLength, m_buffer.begin());
			m_buffer[nCopyLength] = 0;
		}
		return m_buffer.data();
	}

	void ReleaseBuffer(int nNewLength = -1)
	{
		if (m_buffer.empty()) {
			return;
		}
		if (nNewLength < 0) {
			nNewLength = static_cast<int>(std::char_traits<TChar>::length(m_buffer.data()));
		}
		if (nNewLength < 0) {
			nNewLength = 0;
		}
		m_data.assign(m_buffer.data(), static_cast<size_t>(nNewLength));
		m_buffer.clear();
	}

	void AppendChar(TChar ch)
	{
		m_data.push_back(ch);
	}

	int Compare(const TChar *pszSrc) const
	{
		const string_type strSrc = (pszSrc != NULL) ? pszSrc : string_type();
		if (m_data == strSrc) {
			return 0;
		}
		return (m_data < strSrc) ? -1 : 1;
	}

	int Compare(const CStringTCompat &strSrc) const
	{
		return Compare(strSrc.GetString());
	}

	int CompareNoCase(const TChar *pszSrc) const
	{
		const string_type strSrc = (pszSrc != NULL) ? pszSrc : string_type();
		size_t nMin = std::min(m_data.length(), strSrc.length());
		for (size_t i = 0; i < nMin; ++i) {
			const TChar chLeft = BrowserCompatToLower(m_data[i]);
			const TChar chRight = BrowserCompatToLower(strSrc[i]);
			if (chLeft == chRight) {
				continue;
			}
			return (chLeft < chRight) ? -1 : 1;
		}
		if (m_data.length() == strSrc.length()) {
			return 0;
		}
		return (m_data.length() < strSrc.length()) ? -1 : 1;
	}

	int CompareNoCase(const CStringTCompat &strSrc) const
	{
		return CompareNoCase(strSrc.GetString());
	}

	void FormatV(const TChar *pszFormat, va_list args)
	{
		if (pszFormat == NULL) {
			Empty();
			return;
		}

		va_list argCopy;
		va_copy(argCopy, args);
		int nLength = BrowserCompatVsnprintf(NULL, 0, pszFormat, argCopy);
		va_end(argCopy);
		if (nLength < 0) {
			Empty();
			return;
		}

		std::vector<TChar> buffer(static_cast<size_t>(nLength) + 1, 0);
		BrowserCompatVsnprintf(buffer.data(), buffer.size(), pszFormat, args);
		m_data.assign(buffer.data(), static_cast<size_t>(nLength));
	}

	void Format(const TChar *pszFormat, ...)
	{
		va_list args;
		va_start(args, pszFormat);
		FormatV(pszFormat, args);
		va_end(args);
	}

	CStringTCompat Mid(int nFirst, int nCount = -1) const
	{
		if (nFirst < 0) {
			nFirst = 0;
		}
		if (static_cast<size_t>(nFirst) >= m_data.length()) {
			return CStringTCompat();
		}
		size_t nPos = static_cast<size_t>(nFirst);
		size_t nLength = m_data.length() - nPos;
		if (nCount >= 0) {
			nLength = std::min(nLength, static_cast<size_t>(nCount));
		}
		return CStringTCompat(m_data.substr(nPos, nLength));
	}

	CStringTCompat Left(int nCount) const
	{
		if (nCount <= 0) {
			return CStringTCompat();
		}
		return CStringTCompat(m_data.substr(0, std::min(m_data.length(), static_cast<size_t>(nCount))));
	}

	CStringTCompat Right(int nCount) const
	{
		if (nCount <= 0) {
			return CStringTCompat();
		}
		const size_t nLength = std::min(m_data.length(), static_cast<size_t>(nCount));
		return CStringTCompat(m_data.substr(m_data.length() - nLength, nLength));
	}

	int Find(TChar ch, int nStart = 0) const
	{
		if (nStart < 0) {
			nStart = 0;
		}
		size_t nPos = m_data.find(ch, static_cast<size_t>(nStart));
		return (nPos == string_type::npos) ? -1 : static_cast<int>(nPos);
	}

	int Find(const TChar *pszSub, int nStart = 0) const
	{
		if (pszSub == NULL) {
			return -1;
		}
		if (nStart < 0) {
			nStart = 0;
		}
		size_t nPos = m_data.find(pszSub, static_cast<size_t>(nStart));
		return (nPos == string_type::npos) ? -1 : static_cast<int>(nPos);
	}

	int ReverseFind(TChar ch) const
	{
		size_t nPos = m_data.rfind(ch);
		return (nPos == string_type::npos) ? -1 : static_cast<int>(nPos);
	}

	TChar operator [](int nIndex) const
	{
		return m_data[static_cast<size_t>(nIndex)];
	}

private:
	static int BrowserCompatVsnprintf(char *pszDst, size_t nDstCount, const char *pszFormat, va_list args)
	{
		va_list argCopy;
		va_copy(argCopy, args);
		int nRet = vsnprintf(pszDst, nDstCount, pszFormat, argCopy);
		va_end(argCopy);
		return nRet;
	}

	static int BrowserCompatVsnprintf(wchar_t *pszDst, size_t nDstCount, const wchar_t *pszFormat, va_list args)
	{
		va_list argCopy;
		va_copy(argCopy, args);
		int nRet = vswprintf(pszDst, nDstCount, pszFormat, argCopy);
		va_end(argCopy);
		return nRet;
	}

private:
	string_type m_data;
	std::vector<TChar> m_buffer;
};

typedef CStringTCompat<wchar_t> CString;
typedef CStringTCompat<char> CStringA;

inline int _tcslen(LPCTSTR pStr)
{
	return (pStr == NULL) ? 0 : (int)wcslen(pStr);
}

inline TCHAR *_tcscpy_s(LPTSTR pszDst, size_t nDstCount, LPCTSTR pszSrc)
{
	if ((pszDst == NULL) || (nDstCount == 0)) {
		return NULL;
	}
	if (pszSrc == NULL) {
		pszDst[0] = L'\0';
		return pszDst;
	}
	wcsncpy(pszDst, pszSrc, nDstCount - 1);
	pszDst[nDstCount - 1] = L'\0';
	return pszDst;
}

inline TCHAR *_tcscpy_s(LPTSTR pszDst, LPCTSTR pszSrc)
{
	if ((pszDst == NULL) || (pszSrc == NULL)) {
		return pszDst;
	}
	wcscpy(pszDst, pszSrc);
	return pszDst;
}

inline TCHAR *_tcscpy(LPTSTR pszDst, LPCTSTR pszSrc)
{
	if ((pszDst == NULL) || (pszSrc == NULL)) {
		return pszDst;
	}
	wcscpy(pszDst, pszSrc);
	return pszDst;
}

inline TCHAR *_tcscat_s(LPTSTR pszDst, size_t nDstCount, LPCTSTR pszSrc)
{
	if ((pszDst == NULL) || (nDstCount == 0) || (pszSrc == NULL)) {
		return pszDst;
	}
	size_t nLen = wcslen(pszDst);
	if (nLen >= nDstCount) {
		return pszDst;
	}
	wcsncat(pszDst, pszSrc, nDstCount - nLen - 1);
	pszDst[nDstCount - 1] = L'\0';
	return pszDst;
}

inline TCHAR *_tcscat_s(LPTSTR pszDst, LPCTSTR pszSrc)
{
	if ((pszDst == NULL) || (pszSrc == NULL)) {
		return pszDst;
	}
	wcscat(pszDst, pszSrc);
	return pszDst;
}

inline LPTSTR _tcsrchr(LPTSTR pszStr, TCHAR ch)
{
	return (pszStr == NULL) ? NULL : wcsrchr(pszStr, ch);
}

inline LPCTSTR _tcsrchr(LPCTSTR pszStr, TCHAR ch)
{
	return (pszStr == NULL) ? NULL : wcsrchr(pszStr, ch);
}

inline int _tcsncmp(LPCTSTR pszLeft, LPCTSTR pszRight, size_t nCount)
{
	if ((pszLeft == NULL) && (pszRight == NULL)) {
		return 0;
	}
	if (pszLeft == NULL) {
		return -1;
	}
	if (pszRight == NULL) {
		return 1;
	}
	return wcsncmp(pszLeft, pszRight, nCount);
}

inline LPTSTR _tcschr(LPTSTR pszStr, TCHAR ch)
{
	return (pszStr == NULL) ? NULL : wcschr(pszStr, ch);
}

inline LPCTSTR _tcschr(LPCTSTR pszStr, TCHAR ch)
{
	return (pszStr == NULL) ? NULL : wcschr(pszStr, ch);
}

inline int _stprintf_s(LPTSTR pszDst, size_t nDstCount, LPCTSTR pszFormat, ...)
{
	if ((pszDst == NULL) || (nDstCount == 0) || (pszFormat == NULL)) {
		return -1;
	}
	va_list args;
	va_start(args, pszFormat);
	int nRet = vswprintf(pszDst, nDstCount, pszFormat, args);
	va_end(args);
	if (nRet < 0) {
		pszDst[0] = L'\0';
	}
	return nRet;
}

inline int _stprintf_s(LPTSTR pszDst, LPCTSTR pszFormat, ...)
{
	if ((pszDst == NULL) || (pszFormat == NULL)) {
		return -1;
	}
	va_list args;
	va_start(args, pszFormat);
	int nRet = vswprintf(pszDst, 1024, pszFormat, args);
	va_end(args);
	if (nRet < 0) {
		pszDst[0] = L'\0';
	}
	return nRet;
}

inline int _vsntprintf_s(LPTSTR pszDst, size_t nDstCount, size_t, LPCTSTR pszFormat, va_list args)
{
	if ((pszDst == NULL) || (nDstCount == 0) || (pszFormat == NULL)) {
		return -1;
	}
	int nRet = vswprintf(pszDst, nDstCount, pszFormat, args);
	if (nRet < 0) {
		pszDst[0] = L'\0';
	}
	return nRet;
}

inline int _wtoi(LPCWSTR pszValue)
{
	return (pszValue == NULL) ? 0 : (int)wcstol(pszValue, NULL, 10);
}

#if !defined(_MSC_VER)
inline int sprintf_s(char *pszDst, size_t nDstCount, const char *pszFormat, ...)
{
	if ((pszDst == NULL) || (nDstCount == 0) || (pszFormat == NULL)) {
		return -1;
	}
	va_list args;
	va_start(args, pszFormat);
	int nRet = vsnprintf(pszDst, nDstCount, pszFormat, args);
	va_end(args);
	if (nRet < 0) {
		pszDst[0] = '\0';
	}
	return nRet;
}

inline int sprintf_s(char *pszDst, const char *pszFormat, ...)
{
	if ((pszDst == NULL) || (pszFormat == NULL)) {
		return -1;
	}
	va_list args;
	va_start(args, pszFormat);
	int nRet = vsnprintf(pszDst, 1024, pszFormat, args);
	va_end(args);
	if (nRet < 0) {
		pszDst[0] = '\0';
	}
	return nRet;
}
#endif

#if !defined(_MSC_VER)
inline char *strcpy_s(char *pszDst, size_t nDstCount, const char *pszSrc)
{
	if ((pszDst == NULL) || (nDstCount == 0)) {
		return NULL;
	}
	if (pszSrc == NULL) {
		pszDst[0] = '\0';
		return pszDst;
	}
	strncpy(pszDst, pszSrc, nDstCount - 1);
	pszDst[nDstCount - 1] = '\0';
	return pszDst;
}

inline char *strcpy_s(char *pszDst, const char *pszSrc)
{
	if ((pszDst == NULL) || (pszSrc == NULL)) {
		return pszDst;
	}
	strcpy(pszDst, pszSrc);
	return pszDst;
}

inline char *strcat_s(char *pszDst, size_t nDstCount, const char *pszSrc)
{
	if ((pszDst == NULL) || (pszSrc == NULL) || (nDstCount == 0)) {
		return pszDst;
	}
	const size_t nLength = strlen(pszDst);
	if (nLength >= nDstCount) {
		return pszDst;
	}
	strncat(pszDst, pszSrc, nDstCount - nLength - 1);
	pszDst[nDstCount - 1] = '\0';
	return pszDst;
}

inline char *strcat_s(char *pszDst, const char *pszSrc)
{
	if ((pszDst == NULL) || (pszSrc == NULL)) {
		return pszDst;
	}
	strcat(pszDst, pszSrc);
	return pszDst;
}
#endif

inline int _stricmp(const char *pszLeft, const char *pszRight)
{
	if ((pszLeft == NULL) && (pszRight == NULL)) {
		return 0;
	}
	if (pszLeft == NULL) {
		return -1;
	}
	if (pszRight == NULL) {
		return 1;
	}
	while ((*pszLeft != '\0') && (*pszRight != '\0')) {
		const char chLeft = static_cast<char>(std::tolower(static_cast<unsigned char>(*pszLeft)));
		const char chRight = static_cast<char>(std::tolower(static_cast<unsigned char>(*pszRight)));
		if (chLeft != chRight) {
			return (chLeft < chRight) ? -1 : 1;
		}
		++pszLeft;
		++pszRight;
	}
	if (*pszLeft == *pszRight) {
		return 0;
	}
	return (*pszLeft == '\0') ? -1 : 1;
}

inline int lstrlen(LPCTSTR pStr)
{
	return _tcslen(pStr);
}

inline int MessageBox(HWND, LPCTSTR, LPCTSTR, UINT)
{
	return 0;
}

inline void InitializeCriticalSection(LPCRITICAL_SECTION pSection)
{
	if (pSection == NULL) {
		return;
	}
	pSection->pMutex = SDL_CreateMutex();
}

inline void DeleteCriticalSection(LPCRITICAL_SECTION pSection)
{
	if ((pSection == NULL) || (pSection->pMutex == NULL)) {
		return;
	}
	SDL_DestroyMutex(pSection->pMutex);
	pSection->pMutex = NULL;
}

inline void EnterCriticalSection(LPCRITICAL_SECTION pSection)
{
	if ((pSection == NULL) || (pSection->pMutex == NULL)) {
		return;
	}
	SDL_LockMutex(pSection->pMutex);
}

inline void LeaveCriticalSection(LPCRITICAL_SECTION pSection)
{
	if ((pSection == NULL) || (pSection->pMutex == NULL)) {
		return;
	}
	SDL_UnlockMutex(pSection->pMutex);
}

inline void GetLocalTime(LPSYSTEMTIME pSystemTime)
{
	if (pSystemTime == NULL) {
		return;
	}

	std::time_t now = std::time(NULL);
	std::tm *pTm = std::localtime(&now);
	if (pTm == NULL) {
		ZeroMemory(pSystemTime, sizeof(SYSTEMTIME));
		return;
	}

	pSystemTime->wYear = (WORD)(pTm->tm_year + 1900);
	pSystemTime->wMonth = (WORD)(pTm->tm_mon + 1);
	pSystemTime->wDay = (WORD)pTm->tm_mday;
	pSystemTime->wDayOfWeek = (WORD)pTm->tm_wday;
	pSystemTime->wHour = (WORD)pTm->tm_hour;
	pSystemTime->wMinute = (WORD)pTm->tm_min;
	pSystemTime->wSecond = (WORD)pTm->tm_sec;
	pSystemTime->wMilliseconds = 0;
}

inline DWORD timeGetTime(void)
{
	return (DWORD)SDL_GetTicks();
}

inline DWORD GetTickCount(void)
{
	return timeGetTime();
}

inline DWORD GetModuleFileName(HINSTANCE, LPTSTR pszFileName, DWORD nSize)
{
	static const wchar_t kDummyPath[] = L"./sbocli.html";
	if ((pszFileName == NULL) || (nSize == 0)) {
		return 0;
	}
	wcsncpy(pszFileName, kDummyPath, nSize - 1);
	pszFileName[nSize - 1] = L'\0';
	return (DWORD)wcslen(pszFileName);
}

inline HINSTANCE GetModuleHandle(LPCTSTR)
{
	return NULL;
}

inline BOOL PathRemoveFileSpec(LPTSTR pszPath)
{
	if ((pszPath == NULL) || (pszPath[0] == L'\0')) {
		return FALSE;
	}
	LPTSTR pszLastSlash = _tcsrchr(pszPath, L'\\');
	if (pszLastSlash == NULL) {
		pszLastSlash = _tcsrchr(pszPath, L'/');
	}
	if (pszLastSlash == NULL) {
		pszPath[0] = L'\0';
		return FALSE;
	}
	*pszLastSlash = L'\0';
	return TRUE;
}

inline BOOL PathAddBackslash(LPTSTR pszPath)
{
	if (pszPath == NULL) {
		return FALSE;
	}
	const size_t nLength = wcslen(pszPath);
	if (nLength == 0) {
		return FALSE;
	}
	if ((pszPath[nLength - 1] == L'\\') || (pszPath[nLength - 1] == L'/')) {
		return TRUE;
	}
	pszPath[nLength] = L'\\';
	pszPath[nLength + 1] = L'\0';
	return TRUE;
}

inline DWORD GetCurrentDirectory(DWORD nSize, LPTSTR pszBuffer)
{
	return GetModuleFileName(NULL, pszBuffer, nSize);
}

inline BOOL SetCurrentDirectory(LPCTSTR)
{
	return TRUE;
}

inline BOOL CreateDirectory(LPCTSTR, LPVOID)
{
	return TRUE;
}

inline DWORD GetLastError(void)
{
	return 0;
}

inline UINT GetPrivateProfileInt(LPCTSTR, LPCTSTR, int nDefault, LPCTSTR)
{
	return (UINT)nDefault;
}

inline DWORD GetPrivateProfileString(LPCTSTR, LPCTSTR, LPCTSTR pszDefault, LPTSTR pszReturnedString, DWORD nSize, LPCTSTR)
{
	if ((pszReturnedString == NULL) || (nSize == 0)) {
		return 0;
	}
	_tcscpy_s(pszReturnedString, nSize, (pszDefault != NULL) ? pszDefault : _T(""));
	return (DWORD)_tcslen(pszReturnedString);
}

inline BOOL WritePrivateProfileString(LPCTSTR, LPCTSTR, LPCTSTR, LPCTSTR)
{
	return TRUE;
}

inline DWORD GetFileAttributes(LPCTSTR pszPath)
{
	if (pszPath == NULL) {
		return INVALID_FILE_ATTRIBUTES;
	}

	char szUtf8[MAX_PATH * 4];
	size_t nConverted = wcstombs(szUtf8, pszPath, sizeof(szUtf8) - 1);
	if (nConverted == (size_t)-1) {
		return INVALID_FILE_ATTRIBUTES;
	}
	szUtf8[nConverted] = '\0';
	for (size_t i = 0; szUtf8[i] != '\0'; ++i) {
		if (szUtf8[i] == '\\') {
			szUtf8[i] = '/';
		}
	}

	struct stat st;
	if (stat(szUtf8, &st) != 0) {
		return INVALID_FILE_ATTRIBUTES;
	}

#if defined(S_ISDIR)
	return S_ISDIR(st.st_mode) ? FILE_ATTRIBUTE_DIRECTORY : 0;
#else
	return ((st.st_mode & _S_IFDIR) != 0) ? FILE_ATTRIBUTE_DIRECTORY : 0;
#endif
}

inline BOOL PathFileExists(LPCTSTR pszPath)
{
	return (GetFileAttributes(pszPath) != INVALID_FILE_ATTRIBUTES) ? TRUE : FALSE;
}

inline HRESULT CoCreateGuid(GUID *pGuid)
{
	static DWORD s_dwCounter = 1;
	if (pGuid == NULL) {
		return E_FAIL;
	}

	ZeroMemory(pGuid, sizeof(GUID));
	pGuid->Data1 = s_dwCounter++;
	pGuid->Data2 = 0x5342;
	pGuid->Data3 = 0x4f50;
	pGuid->Data4[0] = 'W';
	pGuid->Data4[1] = 'E';
	pGuid->Data4[2] = 'B';
	return S_OK;
}

inline HRESULT CoInitializeEx(LPVOID, DWORD)
{
	return S_OK;
}

inline void CoUninitialize(void)
{
}

inline int StringFromGUID2(const GUID &guid, LPWSTR pszBuffer, int cchMax)
{
	if ((pszBuffer == NULL) || (cchMax <= 0)) {
		return 0;
	}
	int nRet = swprintf(
		pszBuffer,
		(size_t)cchMax,
		L"{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		guid.Data1,
		guid.Data2,
		guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	return (nRet < 0) ? 0 : (nRet + 1);
}

inline HRESULT CLSIDFromString(LPCWSTR, GUID *pGuid)
{
	if (pGuid == NULL) {
		return E_FAIL;
	}
	ZeroMemory(pGuid, sizeof(GUID));
	return S_OK;
}

inline HMODULE LoadLibrary(LPCTSTR)
{
	return NULL;
}

inline FARPROC GetProcAddress(HMODULE, LPCSTR)
{
	return NULL;
}

inline BOOL FreeLibrary(HMODULE)
{
	return TRUE;
}

inline HRSRC FindResource(HMODULE, LPCTSTR, LPCTSTR)
{
	return NULL;
}

inline DWORD SizeofResource(HMODULE, HRSRC)
{
	return 0;
}

inline HGLOBAL LoadResource(HMODULE, HRSRC)
{
	return NULL;
}

inline LPVOID LockResource(HGLOBAL)
{
	return NULL;
}

inline int LoadString(HINSTANCE, UINT, LPTSTR pszBuffer, int nMaxCount)
{
	if ((pszBuffer != NULL) && (nMaxCount > 0)) {
		pszBuffer[0] = L'\0';
	}
	return 0;
}

inline BOOL ShowWindow(HWND, int)
{
	return TRUE;
}

inline BOOL SetWindowPos(HWND, HWND, int, int, int, int, UINT)
{
	return TRUE;
}

inline BOOL GetWindowRect(HWND, LPRECT pRect)
{
	if (pRect == NULL) {
		return FALSE;
	}
	ZeroMemory(pRect, sizeof(RECT));
	return TRUE;
}

inline BOOL SetRect(LPRECT pRect, int left, int top, int right, int bottom)
{
	if (pRect == NULL) {
		return FALSE;
	}
	pRect->left = left;
	pRect->top = top;
	pRect->right = right;
	pRect->bottom = bottom;
	return TRUE;
}

inline BOOL IntersectRect(LPRECT pDst, const RECT *pSrc1, const RECT *pSrc2)
{
	if ((pDst == NULL) || (pSrc1 == NULL) || (pSrc2 == NULL)) {
		return FALSE;
	}
	pDst->left = std::max(pSrc1->left, pSrc2->left);
	pDst->top = std::max(pSrc1->top, pSrc2->top);
	pDst->right = std::min(pSrc1->right, pSrc2->right);
	pDst->bottom = std::min(pSrc1->bottom, pSrc2->bottom);
	if ((pDst->left >= pDst->right) || (pDst->top >= pDst->bottom)) {
		ZeroMemory(pDst, sizeof(RECT));
		return FALSE;
	}
	return TRUE;
}

inline BOOL IsWindowVisible(HWND)
{
	return FALSE;
}

inline HWND SetFocus(HWND hWnd)
{
	return hWnd;
}

inline HWND GetFocus(void)
{
	return NULL;
}

inline BOOL IsIconic(HWND)
{
	return FALSE;
}

inline LRESULT PostMessage(HWND, UINT, WPARAM, LPARAM)
{
	return 0;
}

inline LRESULT SendMessage(HWND, UINT, WPARAM, LPARAM)
{
	return 0;
}

inline LRESULT CallWindowProc(WNDPROC pWndProc, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (pWndProc == NULL) {
		return 0;
	}
	return pWndProc(hWnd, msg, wParam, lParam);
}

inline LRESULT DefWindowProc(HWND, UINT, WPARAM, LPARAM)
{
	return 0;
}

inline UINT_PTR SetTimer(HWND, UINT_PTR nIDEvent, UINT, void *)
{
	return nIDEvent;
}

inline BOOL KillTimer(HWND, UINT_PTR)
{
	return TRUE;
}

inline BOOL PeekMessage(LPMSG, HWND, UINT, UINT, UINT)
{
	return FALSE;
}

inline BOOL TranslateMessage(const MSG *)
{
	return TRUE;
}

inline LRESULT DispatchMessage(const MSG *)
{
	return 0;
}

inline void PostQuitMessage(int)
{
}

inline HDC BeginPaint(HWND, LPPAINTSTRUCT pPaintStruct)
{
	if (pPaintStruct != NULL) {
		ZeroMemory(pPaintStruct, sizeof(PAINTSTRUCT));
	}
	return NULL;
}

inline BOOL EndPaint(HWND, const PAINTSTRUCT *)
{
	return TRUE;
}

inline UINT SHAppBarMessage(DWORD, PAPPBARDATA)
{
	return 0;
}

inline BOOL FlashWindowEx(PFLASHWINFO)
{
	return TRUE;
}

inline HFONT CreateFont(int, int, int, int, int, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPCTSTR)
{
	return NULL;
}

inline HGDIOBJ SelectObject(HDC, HGDIOBJ hObject)
{
	return hObject;
}

inline BOOL DeleteObject(HGDIOBJ)
{
	return TRUE;
}

inline BOOL DeleteDC(HDC)
{
	return TRUE;
}

inline HDC CreateCompatibleDC(HDC)
{
	return NULL;
}

inline HDC GetDC(HWND)
{
	return NULL;
}

inline int ReleaseDC(HWND, HDC)
{
	return 0;
}

inline HBITMAP CreateDIBSection(HDC, const BITMAPINFO *pBmpInfo, UINT, void **ppBits, HANDLE, DWORD)
{
	if ((pBmpInfo == NULL) || (ppBits == NULL)) {
		return NULL;
	}

	const int cx = (int)pBmpInfo->bmiHeader.biWidth;
	const int cy = (int)abs(pBmpInfo->bmiHeader.biHeight);
	const size_t nSize = (size_t)max(cx, 0) * (size_t)max(cy, 0) * 4;
	if (nSize == 0) {
		*ppBits = NULL;
		return NULL;
	}

	BYTE *pNewBits = new BYTE[nSize];
	ZeroMemory(pNewBits, nSize);
	*ppBits = pNewBits;
	return (HBITMAP)pNewBits;
}

inline COLORREF SetTextColor(HDC, COLORREF color)
{
	return color;
}

inline int SetBkMode(HDC, int)
{
	return TRANSPARENT;
}

inline BOOL TextOut(HDC, int, int, LPCTSTR, int)
{
	return TRUE;
}

inline BOOL TextOutW(HDC, int, int, LPCWSTR, int)
{
	return TRUE;
}

inline int DrawText(HDC, LPCTSTR, int, LPRECT, UINT)
{
	return 0;
}

inline BOOL StretchBlt(HDC, int, int, int, int, HDC, int, int, int, int, DWORD)
{
	return TRUE;
}

inline std::string BrowserCompatWideToUtf8(LPCTSTR pszSrc)
{
	if (pszSrc == NULL) {
		return std::string();
	}
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(pszSrc);
}

inline errno_t _tfopen_s(FILE **ppFile, LPCTSTR pszFileName, LPCTSTR pszMode)
{
	if (ppFile == NULL) {
		return EINVAL;
	}
	*ppFile = NULL;
	if ((pszFileName == NULL) || (pszMode == NULL)) {
		return EINVAL;
	}
	std::string strFileName = BrowserCompatWideToUtf8(pszFileName);
	const std::string strMode = BrowserCompatWideToUtf8(pszMode);
	for (size_t i = 0; i < strFileName.size(); ++i) {
		if (strFileName[i] == '\\') {
			strFileName[i] = '/';
		}
	}
	*ppFile = fopen(strFileName.c_str(), strMode.c_str());
	return (*ppFile == NULL) ? errno : 0;
}

#define _ftprintf fwprintf

inline int MultiByteToWideChar(UINT, DWORD, LPCSTR pszSrc, int nSrcLen, LPWSTR pszDst, int nDstLen)
{
	if (pszSrc == NULL) {
		return 0;
	}

	std::string strSrc;
	if (nSrcLen < 0) {
		strSrc = pszSrc;
	} else {
		strSrc.assign(pszSrc, pszSrc + nSrcLen);
	}

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring strWide = converter.from_bytes(strSrc);
	int nRequired = (int)strWide.size() + ((nSrcLen < 0) ? 1 : 0);

	if ((pszDst == NULL) || (nDstLen <= 0)) {
		return nRequired;
	}

	int nCopy = min((int)strWide.size(), nDstLen - 1);
	wmemcpy(pszDst, strWide.c_str(), nCopy);
	if (nSrcLen < 0) {
		pszDst[nCopy] = L'\0';
		return nCopy + 1;
	}
	return nCopy;
}

inline int WideCharToMultiByte(UINT, DWORD, LPCWSTR pszSrc, int nSrcLen, LPSTR pszDst, int nDstLen, LPCSTR, BOOL *)
{
	if (pszSrc == NULL) {
		return 0;
	}

	std::wstring strSrc;
	if (nSrcLen < 0) {
		strSrc = pszSrc;
	} else {
		strSrc.assign(pszSrc, pszSrc + nSrcLen);
	}

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string strUtf8 = converter.to_bytes(strSrc);
	int nRequired = (int)strUtf8.size() + ((nSrcLen < 0) ? 1 : 0);

	if ((pszDst == NULL) || (nDstLen <= 0)) {
		return nRequired;
	}

	int nCopy = min((int)strUtf8.size(), nDstLen - 1);
	memcpy(pszDst, strUtf8.data(), (size_t)nCopy);
	if (nSrcLen < 0) {
		pszDst[nCopy] = '\0';
		return nCopy + 1;
	}
	return nCopy;
}

#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))

#endif
