/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:UraraSockTCP.h												 */
/* 内容			:汎用TCP通信クラス 定義ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2003/10/28													 */
/* ========================================================================= */

#pragma once

#define DLLURARASOCKTCP_API __declspec(dllexport)

/* ========================================================================= */
/* 定数の定義																 */
/* ========================================================================= */

#ifndef SAFE_DELETE
	#define SAFE_DELETE(p)			{ if(p) { delete (p);	(p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);	(p)=NULL; } }
#endif

#define URARASOCK_SENDALL		0x00000000						/* 全員に送信 */

/* ウィンドウメッセージ(設定した基準値＋以下の値) */
enum {
	WM_URARASOCK_HOST = 0,										/* 待ち受け開始 */
	WM_URARASOCK_ADDCLIENT,										/* クライアントが接続した */
	WM_URARASOCK_DECCLIENT,										/* クライアントが切断した */
	WM_URARASOCK_CONNECT,										/* サーバーに接続した */
	WM_URARASOCK_DISCONNECT,									/* サーバーと切断した */
	WM_URARASOCK_RECV,											/* データ受信 */
	WM_URARASOCK_SEND,											/* データ送信 */
	WM_URARASOCK_MAX
};

/* 送信優先順位 */
enum URARASOCK_SENDPRIORITY {
	URARASOCK_SENDPRIORITY_HIGH = 0,							/* 高い */
	URARASOCK_SENDPRIORITY_MIDDLE,								/* 普通 */
	URARASOCK_SENDPRIORITY_LOW,									/* 低い */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CUraraSockTCP
{
public:
	virtual void	DeleteRecvData		(PBYTE pData)									= 0;	/* 受信データを削除 */
	virtual void	Destroy				(void)											= 0;	/* 後始末 */
	virtual BOOL	Host				(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount)	= 0;	/* 接続待ち開始 */
	virtual BOOL	Connect				(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr)	= 0;	/* サーバーへ接続 */
	virtual void	DeleteClient		(DWORD dwID)									= 0;	/* クライアントを切断 */
	virtual void	SendCancel			(DWORD dwID = 0)								= 0;	/* 送信キャンセル */
	virtual void	Send				(PBYTE pData, DWORD dwSize, BYTE byPriority = URARASOCK_SENDPRIORITY_MIDDLE)				= 0;	/* サーバーへデータ送信 */
	virtual void	SendTo				(DWORD dwID, PBYTE pData, DWORD dwSize, BYTE byPriority = URARASOCK_SENDPRIORITY_MIDDLE)	= 0;	/* クライアントへデータ送信 */
	virtual DWORD	GetThrowghPutSend	(DWORD dwID)									= 0;	/* リンク・スループット量を取得 */
	virtual DWORD	GetThrowghPutRecv	(DWORD dwID)									= 0;	/* リンク・スループット量を取得 */
	virtual DWORD	GetQueCount			(DWORD dwID)									= 0;	/* キュー数を取得 */
	virtual DWORD	GetIPAddress		(DWORD dwID)									= 0;	/* IPアドレスを取得 */
};

extern "C"{
DLLURARASOCKTCP_API CUraraSockTCP *GetUraraSockTCP(void);			/* クラスのポインタを取得 */
DLLURARASOCKTCP_API void ReleaseUraraSockTCP(CUraraSockTCP *&pSrc);	/* クラスのポインタを解放 */
}
typedef CUraraSockTCP *(*PFGETURARASOCKTCP)(void);
typedef void (*PFRELEASEURARASOCKTCP)(CUraraSockTCP *&pSrc);

/* Copyright(C)URARA-works 2008 */
