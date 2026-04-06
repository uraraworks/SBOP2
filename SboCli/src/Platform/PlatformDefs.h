/// @file PlatformDefs.h
/// @brief 非Windows環境向け 型定義・構造体・定数・マクロ
/// @date 2026/04/07

#pragma once

#if !defined(_WIN32)

// -----------------------------------------------------------------------
// 基本型 typedef
// -----------------------------------------------------------------------
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
typedef void *HBRUSH;

// -----------------------------------------------------------------------
// 定数マクロ
// -----------------------------------------------------------------------
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
#define ERROR_SUCCESS 0L

// WM_* メッセージ定数
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

// VK_* 仮想キーコード定数
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

// メモリ操作マクロ
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

// -----------------------------------------------------------------------
// 構造体定義
// -----------------------------------------------------------------------
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

// ウィンドウプロシージャ型
typedef LRESULT (CALLBACK *WNDPROC)(HWND, UINT, WPARAM, LPARAM);

#endif // !_WIN32
