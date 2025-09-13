/* Copyright(C)URARA-works 2003-2005 */
/* ========================================================================= */
/* UraraSockTCPSBO.h														 */
/* 汎用TCP通信クラス														 */
/* 2003/10/28 作成開始														 */
/* ========================================================================= */

#pragma once

#include "UraraSockTCP.h"

class CPacketBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CUraraSockTCPSBO
{
public:
			CUraraSockTCPSBO(void);									/* コンストラクタ */
	virtual ~CUraraSockTCPSBO(void);								/* デストラクタ */

	void	DeleteRecvData		(PBYTE pData);										/*受信データを削除 */
	void	Destroy				(void);												/* 後始末 */
	BOOL	Host				(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount);		/* 接続待ち開始 */
	BOOL	Connect				(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr);	/* サーバーへ接続 */
	void	DeleteClient		(DWORD dwID);										/* クライアントを切断 */
	void	SendCancel			(DWORD dwID = 0);									/* 送信キャンセル */
	void	Send				(CPacketBase *pPacket);								/* サーバーへデータ送信 */
	void	SendTo				(DWORD dwID, CPacketBase *pPacket);					/* クライアントへデータ送信 */
	DWORD	GetThrowghPutSend	(DWORD dwID);										/* リンク・スループット量を取得 */
	DWORD	GetThrowghPutRecv	(DWORD dwID);										/* リンク・スループット量を取得 */
	DWORD	GetQueCount			(DWORD dwID);										/* キュー数を取得 */
	DWORD	GetIPAddress		(DWORD dwID);										/* IPアドレスを取得 */


protected:
	HMODULE			m_hDll;					/* 通信ライブラリDLL */
	CUraraSockTCP	*m_pSock;				/* 通信ライブラリ */
} CUraraSockTCPSBO, *PCUraraSockTCPSBO;

/* Copyright(C)URARA-works 2003-2005 */
