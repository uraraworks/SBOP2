/// @file UraraSockTCP.h
/// @brief Generic TCP communication class definition
/// @author URARA-works
/// @date 2003/10/28
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#ifdef _WIN32
#define DLLURARASOCKTCP_API __declspec(dllexport)
#else
#define DLLURARASOCKTCP_API
#endif

// Constant definitions

#ifndef SAFE_DELETE
	#define SAFE_DELETE(p)			{ if(p) { delete (p);	(p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);	(p)=NULL; } }
#endif

#define URARASOCK_SENDALL		0x00000000	// Send to all

// Window messages (base value + following values)
enum {
	WM_URARASOCK_HOST = 0,		// Start listening
	WM_URARASOCK_ADDCLIENT,		// Client connected
	WM_URARASOCK_DECCLIENT,		// Client disconnected
	WM_URARASOCK_CONNECT,		// Connected to server
	WM_URARASOCK_DISCONNECT,	// Disconnected from server
	WM_URARASOCK_RECV,			// Data received
	WM_URARASOCK_SEND,			// Data sent
	WM_URARASOCK_MAX
};

// Send priority
enum URARASOCK_SENDPRIORITY {
	URARASOCK_SENDPRIORITY_HIGH = 0,	// High
	URARASOCK_SENDPRIORITY_MIDDLE,		// Normal
	URARASOCK_SENDPRIORITY_LOW,			// Low
};

typedef void (*PFURARASOCKNOTIFY)(void *pUserData, UINT uMsgOffset, WPARAM wParam, LPARAM lParam);


// Class declaration

class CUraraSockTCP
{
public:
	virtual void	DeleteRecvData(PBYTE pData)												= 0;	// Delete received data
	virtual void	Destroy(void)															= 0;	// Cleanup
	virtual BOOL	Host(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount)		= 0;	// Start listening
	virtual BOOL	Connect(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr)	= 0;	// Connect to server
	virtual void	DeleteClient(DWORD dwID)												= 0;	// Disconnect client
	virtual void	SendCancel(DWORD dwID = 0)												= 0;	// Cancel send
	virtual void	Send(PBYTE pData, DWORD dwSize, BYTE byPriority = URARASOCK_SENDPRIORITY_MIDDLE)					= 0;	// Send data to server
	virtual void	SendTo(DWORD dwID, PBYTE pData, DWORD dwSize, BYTE byPriority = URARASOCK_SENDPRIORITY_MIDDLE)	= 0;	// Send data to client
	virtual DWORD	GetThrowghPutSend(DWORD dwID)											= 0;	// Get send throughput
	virtual DWORD	GetThrowghPutRecv(DWORD dwID)											= 0;	// Get recv throughput
	virtual DWORD	GetQueCount(DWORD dwID)													= 0;	// Get queue count
	virtual DWORD	GetIPAddress(DWORD dwID)												= 0;	// Get IP address
};

#ifdef _WIN32
extern "C"{
DLLURARASOCKTCP_API CUraraSockTCP *GetUraraSockTCP(void);			// Get class pointer
DLLURARASOCKTCP_API void ReleaseUraraSockTCP(CUraraSockTCP *&pSrc);	// Release class pointer
}
typedef CUraraSockTCP *(*PFGETURARASOCKTCP)(void);
typedef void (*PFRELEASEURARASOCKTCP)(CUraraSockTCP *&pSrc);
#else
// 非Windows環境向け実装
#ifndef __EMSCRIPTEN__
// 非Windows / 非Emscripten: スタブ実装（何もしない）
class CUraraSockTCPStub : public CUraraSockTCP
{
public:
	virtual void	DeleteRecvData(PBYTE) override {}
	virtual void	Destroy(void) override {}
	virtual BOOL	Host(HWND, DWORD, DWORD, WORD, DWORD) override { return FALSE; }
	virtual BOOL	Connect(HWND, DWORD, DWORD, WORD, LPCSTR) override { return FALSE; }
	virtual void	DeleteClient(DWORD) override {}
	virtual void	SendCancel(DWORD = 0) override {}
	virtual void	Send(PBYTE, DWORD, BYTE = URARASOCK_SENDPRIORITY_MIDDLE) override {}
	virtual void	SendTo(DWORD, PBYTE, DWORD, BYTE = URARASOCK_SENDPRIORITY_MIDDLE) override {}
	virtual DWORD	GetThrowghPutSend(DWORD) override { return 0; }
	virtual DWORD	GetThrowghPutRecv(DWORD) override { return 0; }
	virtual DWORD	GetQueCount(DWORD) override { return 0; }
	virtual DWORD	GetIPAddress(DWORD) override { return 0; }
};
inline CUraraSockTCP *GetUraraSockTCP(void) { return new CUraraSockTCPStub(); }
#endif // !__EMSCRIPTEN__

inline void ReleaseUraraSockTCP(CUraraSockTCP *&pSrc) { if (pSrc) { delete pSrc; pSrc = NULL; } }
typedef CUraraSockTCP *(*PFGETURARASOCKTCP)(void);
typedef void (*PFRELEASEURARASOCKTCP)(CUraraSockTCP *&pSrc);
#endif // _WIN32

// Emscripten環境では GetUraraSockTCP() は UraraSockTCPWebSocket.h 末尾で定義される
// （CUraraSockTCPWebSocket クラスの定義後に inline 実装する必要があるため）
#if defined(__EMSCRIPTEN__) && !defined(URARASOCKTCP_WEBSOCKET_INCLUDED)
#include "UraraSockTCPWebSocket.h"
#endif // __EMSCRIPTEN__ && !URARASOCKTCP_WEBSOCKET_INCLUDED
