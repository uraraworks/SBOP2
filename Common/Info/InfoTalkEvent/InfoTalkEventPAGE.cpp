/// @file InfoTalkEventPAGE.cpp
/// @brief 会話イベント情報(ページ切り替え)クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/29
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "InfoTalkEventPAGE.h"

// ヘッダ情報
static LPCSTR s_aszName[] = {
	"m_nPageChgCondition",	// ページ切り替え条件
	"m_nPageJump",	// ページ切り替え先
	NULL
};

CInfoTalkEventPAGE::CInfoTalkEventPAGE()
{
	m_nEventType = TALKEVENTTYPE_PAGE;

	m_nElementCountBase = m_nElementCount;
	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}

CInfoTalkEventPAGE::~CInfoTalkEventPAGE()
{
}

int CInfoTalkEventPAGE::GetElementCount(void)
{
	return m_nElementCountBase + m_nElementCount;
}

int CInfoTalkEventPAGE::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = CInfoTalkEventBase::GetElementNo(pszName);
	if (nRet >= 0) {
		goto Exit;
	}

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp(s_aszName[i], pszName) == 0) {
			nRet = m_nElementCountBase + i;
			break;
		}
	}

Exit:
	return nRet;
}

DWORD CInfoTalkEventPAGE::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoTalkEventBase::GetDataSize();
	dwRet += sizeof(m_nPageChgCondition);	// ページ切り替え条件
	dwRet += sizeof(m_nPageJump);	// ページ切り替え先

	return dwRet;
}

DWORD CInfoTalkEventPAGE::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;
	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::GetDataSizeNo(nNo);
	}

	switch (nNo - m_nElementCountBase) {
	case 0:	dwRet = sizeof(m_nPageChgCondition);	break;	// ページ切り替え条件
	case 1:	dwRet = sizeof(m_nPageJump);	break;	// ページ切り替え先
	}

	return dwRet;
}

LPCSTR CInfoTalkEventPAGE::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::GetName(nNo);
	}
	return s_aszName[nNo - m_nElementCountBase];
}

PBYTE CInfoTalkEventPAGE::GetWriteData(int nNo, PDWORD pdwSize)
{
	DWORD dwSize;
	PBYTE pRet, pSrc;

	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::GetWriteData(nNo, pdwSize);
	}

	pRet	= NULL;
	pSrc	= NULL;
	dwSize	= GetDataSizeNo(nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = ZeroNew(dwSize);

	nNo -= m_nElementCountBase;
	switch (nNo) {
	case 0:	pSrc = (PBYTE)&m_nPageChgCondition;	break;	// ページ切り替え条件
	case 1:	pSrc = (PBYTE)&m_nPageJump;	break;	// ページ切り替え先
	}

	if (pSrc) {
		CopyMemory(pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}

DWORD CInfoTalkEventPAGE::ReadElementData(
	PBYTE pSrc,	// [in] データの読み込み元
	int nNo)	// [in] 要素番号
{
	DWORD dwSize;
	PBYTE pDst;

	pDst	= NULL;
	dwSize	= 0;

	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::ReadElementData(pSrc, nNo);
	}

	nNo -= m_nElementCountBase;
	switch (nNo) {
	case 0:	pDst = (PBYTE)&m_nPageChgCondition;	dwSize = sizeof(m_nPageChgCondition);	break;	// ページ切り替え条件
	case 1:	pDst = (PBYTE)&m_nPageJump;	dwSize = sizeof(m_nPageJump);	break;	// ページ切り替え先
	}

	if (pDst) {
		CopyMemory(pDst, pSrc, dwSize);
	}

	return dwSize;
}

DWORD CInfoTalkEventPAGE::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoTalkEventBase::GetSendDataSize();
	dwRet += GetDataSizeNo(m_nElementCountBase + 0);	// ページ切り替え条件
	dwRet += GetDataSizeNo(m_nElementCountBase + 1);	// ページ切り替え先

	return dwRet;
}

PBYTE CInfoTalkEventPAGE::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	dwSize	 = GetSendDataSize();
	pData	 = ZeroNew(dwSize);
	pDataTmp = pData;

	pDataBase  = CInfoTalkEventBase::GetSendData();
	dwSizeBase = CInfoTalkEventBase::GetSendDataSize();
	CopyMemoryRenew(pDataTmp, pDataBase, dwSizeBase, pDataTmp);
	SAFE_DELETE_ARRAY(pDataBase);

	CopyMemoryRenew(pDataTmp, &m_nPageChgCondition,	sizeof(m_nPageChgCondition),	pDataTmp);	// ページ切り替え条件
	CopyMemoryRenew(pDataTmp, &m_nPageJump,	sizeof(m_nPageJump),	pDataTmp);	// ページ切り替え先

	return pData;
}

PBYTE CInfoTalkEventPAGE::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = CInfoTalkEventBase::SetSendData(pSrc);
	CopyMemoryRenew(&m_nPageChgCondition,	pDataTmp, sizeof(m_nPageChgCondition),	pDataTmp);	// ページ切り替え条件
	CopyMemoryRenew(&m_nPageJump,	pDataTmp, sizeof(m_nPageJump),	pDataTmp);	// ページ切り替え先

	pRet = pDataTmp;
	return pRet;
}

void CInfoTalkEventPAGE::Copy(CInfoTalkEventBase *pSrc)
{
	PCInfoTalkEventPAGE pSrcTmp;

	if (pSrc == NULL) {
		return;
	}
	pSrcTmp = (PCInfoTalkEventPAGE)pSrc;

	CInfoTalkEventBase::Copy(pSrcTmp);
	m_nPageChgCondition	= pSrcTmp->m_nPageChgCondition;	// ページ切り替え条件
	m_nPageJump	= pSrcTmp->m_nPageJump;	// ページ切り替え先
}

