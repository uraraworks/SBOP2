/// @file PacketMAP_SYSTEMMSG.cpp
/// @brief コマンド(マップ系:システムメッセージ通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/02/10
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_SYSTEMMSG.h"

CPacketMAP_SYSTEMMSG::CPacketMAP_SYSTEMMSG()
{
	m_nMsgType	= SYSTEMMSGTYPE_DEFAULT;
	m_bSound	= TRUE;
	m_clMsg	= 0;
}

CPacketMAP_SYSTEMMSG::~CPacketMAP_SYSTEMMSG()
{
}

void CPacketMAP_SYSTEMMSG::Make(
	LPCSTR pszMsg,	// [in] システムメッセージ
	COLORREF clMsg,	// =0*/	/* [in] 表示色
	BOOL bSound,	// =TRUE*/	/* [in] 表示する時に音をならすか判定
	int nMsgType)	// =SYSTEMMSGTYPE_DEFAULT*/	/* [in] メッセージ種別
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (clMsg)	+
			 sizeof (bSound)	+
			 sizeof (m_nMsgType)	+
			 (strlen(pszMsg) + 1);

	if (clMsg == 0) {
		clMsg = RGB(0, 200, 255);
	}

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_SYSTEMMSG;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	strcpyRenew((LPSTR)pDataTmp, pszMsg, pDataTmp);	// システムメッセージ
	CopyMemoryRenew(pDataTmp, &clMsg,	sizeof (clMsg),	pDataTmp);	// 表示色
	CopyMemoryRenew(pDataTmp, &bSound,	sizeof (bSound),	pDataTmp);	// 表示する時に音をならすか判定
	CopyMemoryRenew(pDataTmp, &nMsgType,	sizeof (nMsgType),	pDataTmp);	// メッセージ種別

	RenewPacket(pData, dwSize);
}

PBYTE CPacketMAP_SYSTEMMSG::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	StoreRenew(m_strMsg, (LPCSTR)pDataTmp, pDataTmp);	// システムメッセージ
	CopyMemoryRenew(&m_clMsg,	pDataTmp, sizeof (m_clMsg),	pDataTmp);	// 表示色
	CopyMemoryRenew(&m_bSound,	pDataTmp, sizeof (m_bSound),	pDataTmp);	// 表示する時に音をならすか判定
	CopyMemoryRenew(&m_nMsgType,	pDataTmp, sizeof (m_nMsgType),	pDataTmp);	// メッセージ種別

	pRet = pDataTmp;
	return pRet;
}
