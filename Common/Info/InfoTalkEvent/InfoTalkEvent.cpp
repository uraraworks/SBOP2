/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoTalkEvent.cpp											 */
/* 内容			:会話イベント情報クラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/16													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoTalkEventPAGE.h"
#include "InfoTalkEventMENU.h"
#include "InfoTalkEvent.h"

#include <limits>
#include <vector>

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_dwTalkEventID",	/* 会話イベントID */
	"m_apTalkEvent",	/* 会話イベント */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::CInfoTalkEvent									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/16														 */
/* ========================================================================= */

CInfoTalkEvent::CInfoTalkEvent()
{
	m_dwTalkEventID	= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::~CInfoTalkEvent								 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/16														 */
/* ========================================================================= */

CInfoTalkEvent::~CInfoTalkEvent()
{
	DeleteAllTalkEvent ();
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::GetElementNo									 */
/* 内容		:要素番号を取得													 */
/* 日付		:2005/07/05														 */
/* ========================================================================= */

int CInfoTalkEvent::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = -1;

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::GetDataSize									 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/12/16														 */
/* ========================================================================= */

DWORD CInfoTalkEvent::GetDataSize(void)
{
	int i;
	DWORD dwRet;

	dwRet = 0;
	for (i = 0; i < m_nElementCount; i ++) {
		dwRet += GetDataSizeNo (i);
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::GetDataSizeNo									 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2008/12/16														 */
/* ========================================================================= */

namespace {

class CSerializeBuffer
{
public:
	explicit CSerializeBuffer(std::vector<BYTE> *pBuffer)
		: m_pBuffer(pBuffer)
		, m_ullSize(0)
		, m_bFailed(false)
	{
	}

	void Append(const void *pSrc, size_t nSize)
	{
		if (m_bFailed) {
			return;
		}
		if ((pSrc == NULL) && (nSize > 0)) {
			m_bFailed = true;
			return;
		}
		if (nSize == 0) {
			return;
		}

		if (m_pBuffer != NULL) {
			size_t nOffset = m_pBuffer->size();
			m_pBuffer->resize(nOffset + nSize);
			CopyMemory(m_pBuffer->data() + nOffset, pSrc, nSize);
		}

		m_ullSize += nSize;
		const unsigned long long ullMax = (std::numeric_limits<DWORD>::max)();
		if (m_ullSize > ullMax) {
			m_bFailed = true;
		}
	}

	void AddSize(size_t nSize)
	{
		if (m_bFailed || nSize == 0) {
			return;
		}

		m_ullSize += nSize;
		const unsigned long long ullMax = (std::numeric_limits<DWORD>::max)();
		if (m_ullSize > ullMax) {
			m_bFailed = true;
		}
	}

	void Fail()
	{
		m_bFailed = true;
	}

	size_t GetSize() const
	{
		return static_cast<size_t>(m_ullSize);
	}

	bool HasFailed() const
	{
		return m_bFailed;
	}

	bool CanWrite() const
	{
		return (m_pBuffer != NULL);
	}

private:
	std::vector<BYTE> *m_pBuffer;
	unsigned long long m_ullSize;
	bool m_bFailed;
};

void AppendString(CSerializeBuffer &writer, LPCSTR pszString)
{
	if (pszString == NULL) {
		static const char szEmpty[1] = { '\0' };
		writer.Append(szEmpty, sizeof(szEmpty));
		return;
	}

	size_t nLen = strlen(pszString) + 1;
	writer.Append(pszString, nLen);
}

bool WriteBaseElementPayload(CInfoTalkEventBase *pInfo, int nIndex, CSerializeBuffer &writer)
{
        if (pInfo == NULL) {
                writer.Fail();
                return false;
        }

        switch (nIndex) {
        case 0:
                writer.Append(&pInfo->m_nEventType, sizeof(pInfo->m_nEventType));
                return !writer.HasFailed();
        case 1:
                writer.Append(&pInfo->m_nPage, sizeof(pInfo->m_nPage));
                return !writer.HasFailed();
        case 2:
                writer.Append(&pInfo->m_dwData, sizeof(pInfo->m_dwData));
                return !writer.HasFailed();
        case 3:
                AppendString(writer, (LPCSTR)pInfo->m_strText);
                return !writer.HasFailed();
        }

        writer.Fail();
        return false;
}

bool WritePageElementPayload(CInfoTalkEventPAGE *pPage, int nIndex, CSerializeBuffer &writer)
{
        if (pPage == NULL) {
                writer.Fail();
                return false;
        }

        switch (nIndex) {
        case 0:
                writer.Append(&pPage->m_nPageChgCondition, sizeof(pPage->m_nPageChgCondition));
                return !writer.HasFailed();
        case 1:
                writer.Append(&pPage->m_nPageJump, sizeof(pPage->m_nPageJump));
                return !writer.HasFailed();
        }

        writer.Fail();
        return false;
}

bool WriteMenuElementPayload(CInfoTalkEventMENU *pMenu, int nIndex, CSerializeBuffer &writer)
{
        if (pMenu == NULL) {
                writer.Fail();
                return false;
        }

        const int nCount = static_cast<int>(pMenu->m_aMenuInfo.size());
        switch (nIndex) {
        case 0: {
                writer.Append(&nCount, sizeof(nCount));
                return !writer.HasFailed();
        }
        case 1:
                for (int i = 0; i < nCount; i ++) {
                        PSTTALKEVENTMENUINFO pMenuInfo = pMenu->m_aMenuInfo[i];
                        int nPage = (pMenuInfo != NULL) ? pMenuInfo->nPage : 0;
                        writer.Append(&nPage, sizeof(nPage));
                        if (writer.HasFailed()) {
                                return false;
                        }
                }
                return true;
        case 2:
                for (int i = 0; i < nCount; i ++) {
                        PSTTALKEVENTMENUINFO pMenuInfo = pMenu->m_aMenuInfo[i];
                        LPCSTR pszName = (pMenuInfo != NULL) ? (LPCSTR)pMenuInfo->strName : NULL;
                        AppendString(writer, pszName);
                        if (writer.HasFailed()) {
                                return false;
                        }
                }
                return true;
        }

        writer.Fail();
        return false;
}

bool WriteTalkEventElementPayload(CInfoTalkEventBase *pInfo, int nIndex, CSerializeBuffer &writer)
{
        if (pInfo == NULL) {
                writer.Fail();
                return false;
        }

        if (nIndex < pInfo->m_nElementCountBase) {
                return WriteBaseElementPayload(pInfo, nIndex, writer);
        }

        int nDerivedIndex = nIndex - pInfo->m_nElementCountBase;
        switch (pInfo->m_nEventType) {
        case TALKEVENTTYPE_PAGE:
                return WritePageElementPayload(static_cast<CInfoTalkEventPAGE *>(pInfo), nDerivedIndex, writer);
        case TALKEVENTTYPE_MENU:
                return WriteMenuElementPayload(static_cast<CInfoTalkEventMENU *>(pInfo), nDerivedIndex, writer);
        default:
                writer.Fail();
                return false;
        }
}

bool SerializeTalkEventElement(CInfoTalkEventBase *pInfo, CSerializeBuffer &writer)
{
        if (pInfo == NULL) {
                writer.Append(NULL, 1);
		return false;
	}

	int nCount2 = pInfo->GetElementCount();
	writer.Append(&nCount2, sizeof(nCount2));
	if (writer.HasFailed()) {
		return false;
	}

        for (int j = 0; j < nCount2; j ++) {
                LPCSTR pszName = pInfo->GetName(j);
                AppendString(writer, pszName);
                if (writer.HasFailed()) {
                        return false;
                }

                const DWORD dwExpectedSize = pInfo->GetDataSizeNo(j);

                if (!writer.CanWrite()) {
                        writer.Append(&dwExpectedSize, sizeof(dwExpectedSize));
                        if (writer.HasFailed()) {
                                return false;
                        }

                        if (dwExpectedSize == 0) {
                                continue;
                        }

                        writer.AddSize(dwExpectedSize);
                        if (writer.HasFailed()) {
                                return false;
                        }
                        continue;
                }

                writer.Append(&dwExpectedSize, sizeof(dwExpectedSize));
                if (writer.HasFailed()) {
                        return false;
                }

                if (dwExpectedSize == 0) {
                        continue;
                }

                size_t nStart = writer.GetSize();
                if (!WriteTalkEventElementPayload(pInfo, j, writer)) {
                        return false;
                }

                size_t nWritten = writer.GetSize() - nStart;
                if (nWritten != dwExpectedSize) {
                        writer.Fail();
                        return false;
                }
        }

	return true;
}

bool SerializeTalkEvent(CInfoTalkEvent *pEvent, int nNo, CSerializeBuffer &writer)
{
	if (pEvent == NULL) {
		writer.Append(NULL, 1);
		return false;
	}

	switch (nNo) {
	case 0:
		writer.Append(&pEvent->m_dwTalkEventID, sizeof(pEvent->m_dwTalkEventID));
		return !writer.HasFailed();

	case 1: {
		int nCount = static_cast<int>(pEvent->m_apTalkEvent.size());
		writer.Append(&nCount, sizeof(nCount));
		if (writer.HasFailed()) {
			return false;
		}

		for (int i = 0; i < nCount; i ++) {
			CInfoTalkEventBase *pInfo = pEvent->m_apTalkEvent[i];
			if (!SerializeTalkEventElement(pInfo, writer)) {
				return false;
			}
		}
		return !writer.HasFailed();
	}
	}

	return false;
}

} /* anonymous namespace */

DWORD CInfoTalkEvent::GetDataSizeNo(int nNo)
{
	CSerializeBuffer writer(NULL);
	if (!SerializeTalkEvent(this, nNo, writer) || writer.HasFailed()) {
		return 0;
	}

	return static_cast<DWORD>(writer.GetSize());
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::GetName										 */
/* 内容		:要素名を取得													 */
/* 日付		:2005/07/05														 */
/* ========================================================================= */

LPCSTR CInfoTalkEvent::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::GetWriteData									 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/12/16														 */
/* ========================================================================= */

PBYTE CInfoTalkEvent::GetWriteData(int nNo, PDWORD pdwSize)
{
	std::vector<BYTE> buffer;

	CSerializeBuffer writer(&buffer);
	if (!SerializeTalkEvent(this, nNo, writer) || writer.HasFailed() || writer.GetSize() == 0) {
		*pdwSize = 0;
		return NULL;
	}

	*pdwSize = static_cast<DWORD>(writer.GetSize());
	PBYTE pRet = ZeroNew(*pdwSize);
	CopyMemory(pRet, buffer.data(), *pdwSize);
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::ReadElementData								 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2008/12/16														 */
/* ========================================================================= */

DWORD CInfoTalkEvent::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	int i, j, nNoTmp, nCount, nCount2, nType;
	PBYTE pDst, pSrcTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoTalkEventBase pInfo;
	CmyString strTmp;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = (PBYTE)&m_dwTalkEventID; dwSize = sizeof (m_dwTalkEventID); break;	/* 会話イベントID */
	case 1:				/* 会話イベント */
		pSrcTmp = pSrc;
		CopyMemoryRenew (&nCount, pSrcTmp, sizeof (nCount), pSrcTmp);				/* データ数 */
		for (i = 0; i < nCount; i ++) {
			CopyMemoryRenew (&nCount2, pSrcTmp, sizeof (nCount2), pSrcTmp);			/* 要素数 */

			for (j = 0; j < nCount2; j ++) {
				StoreRenew (strTmp, (LPCSTR)pSrcTmp, pSrcTmp);						/* 要素名 */
				CopyMemoryRenew (&dwSizeTmp, pSrcTmp, sizeof (dwSizeTmp), pSrcTmp);	/* データサイズ */
				if (j == 0) {
					/* 最初はイベント種別 */
					CopyMemoryRenew (&nType, pSrcTmp, sizeof (nType), pSrcTmp);		/* イベント種別 */
					pInfo = (PCInfoTalkEventBase)GetNew (nType);
				} else {
					nNoTmp = pInfo->GetElementNo ((LPCSTR)strTmp);
					if (nNoTmp >= 0) {
						dwSizeTmp = pInfo->ReadElementData (pSrcTmp, nNoTmp);
					}
					pSrcTmp += dwSizeTmp;
				}
			}
			AddTalkEvent (pInfo);
		}
		dwSize = (pSrcTmp - pSrc);
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/12/16														 */
/* ========================================================================= */

DWORD CInfoTalkEvent::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoTalkEventBase pInfo;

	dwRet = sizeof (m_dwTalkEventID);	/* 会話イベントID */

	/* データ数分のサイズ */
	dwRet += sizeof (DWORD);

	nCount = m_apTalkEvent.size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_apTalkEvent[i];

		dwRet += pInfo->GetSendDataSize ();
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/12/16														 */
/* ========================================================================= */

PBYTE CInfoTalkEvent::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp, pDataTmp2;
	DWORD dwSize, dwCount, dwSizeTmp;
	PCInfoTalkEventBase pInfo;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwTalkEventID, sizeof (m_dwTalkEventID), pDataTmp);	/* 会話イベントID */

	/* データ数を書き込み */
	dwCount = (DWORD)m_apTalkEvent.size();
	CopyMemoryRenew (pDataTmp, &dwCount, sizeof (dwCount), pDataTmp);

	/* イベント情報を書き込み */
	nCount = (int)dwCount;
	for (i = 0; i < nCount; i ++) {
		pInfo = m_apTalkEvent[i];

		dwSizeTmp	= pInfo->GetSendDataSize ();
		pDataTmp2	= pInfo->GetSendData ();
		CopyMemoryRenew (pDataTmp, pDataTmp2, dwSizeTmp, pDataTmp);	/* 会話イベント */

		SAFE_DELETE_ARRAY (pDataTmp2);
	}

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/12/16														 */
/* ========================================================================= */

PBYTE CInfoTalkEvent::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	PBYTE pDataTmp;
	DWORD dwCount;
	CInfoTalkEventBase InfoTmp, *pInfo;

	DeleteAllTalkEvent ();

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwTalkEventID, pDataTmp, sizeof (m_dwTalkEventID), pDataTmp);	/* 会話イベントID */

	/* データ数を読み込み */
	CopyMemoryRenew (&dwCount, pDataTmp, sizeof (dwCount), pDataTmp);
	nCount = (int)dwCount;

	for (i = 0; i < nCount; i ++) {
		InfoTmp.SetSendData (pDataTmp);
		pInfo = (PCInfoTalkEventBase)GetNew (InfoTmp.m_nEventType);
		pDataTmp = pInfo->SetSendData (pDataTmp);
		AddTalkEvent (pInfo);
	}

	return pDataTmp;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::Copy											 */
/* 内容		:コピー															 */
/* 日付		:2008/12/16														 */
/* ========================================================================= */

void CInfoTalkEvent::Copy(CInfoTalkEvent *pSrc)
{
	PBYTE pTmp;

	if (pSrc == NULL) {
		return;
	}

	DeleteAllTalkEvent ();
	pTmp = pSrc->GetSendData ();
	SetSendData (pTmp);

	SAFE_DELETE (pTmp);
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::GetNew											 */
/* 内容		:新しい会話イベントを取得										 */
/* 日付		:2008/12/17														 */
/* ========================================================================= */

PCInfoTalkEventBase CInfoTalkEvent::GetNew(int nType)
{
	PCInfoTalkEventBase pRet;

	pRet = NULL;

	switch (nType) {
	case TALKEVENTTYPE_PAGE:			/* ページ切り替え */
		pRet = new CInfoTalkEventPAGE;
		break;
	case TALKEVENTTYPE_MSG:				/* メッセージ表示 */
		pRet = new CInfoTalkEventBase;
		break;
	case TALKEVENTTYPE_MENU:			/* 項目選択 */
		pRet = new CInfoTalkEventMENU;
		break;
	default:
		pRet = new CInfoTalkEventBase;
		break;
	}
	pRet->m_nEventType = nType;

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::GetPtr											 */
/* 内容		:会話イベントを取得												 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

PCInfoTalkEventBase CInfoTalkEvent::GetPtr(int nPage, int nNo)
{
	int i, nCount, nNoCount;
	PCInfoTalkEventBase pInfo, pRet;

	pRet	 = NULL;
	nNoCount = 0;
	nCount   = m_apTalkEvent.size();

	for (i = 0; i < nCount; i ++) {
		pInfo = m_apTalkEvent[i];
		if (pInfo->m_nPage != nPage) {
			continue;
		}
		if (nNo != nNoCount) {
			nNoCount ++;
			continue;
		}
		pRet = pInfo;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::SetPtr											 */
/* 内容		:会話イベントを差し替え											 */
/* 日付		:2008/12/17														 */
/* ========================================================================= */

void CInfoTalkEvent::SetPtr(int nPage, int nNo, CInfoTalkEventBase *pInfo)
{
	int i, nCount, nNoCount;
	PCInfoTalkEventBase pInfoTmp;

	nNoCount = 0;
	nCount   = m_apTalkEvent.size();
        for (i = 0; i < nCount; i ++) {
                pInfoTmp = m_apTalkEvent[i];
                if (pInfoTmp->m_nPage != nPage) {
                        continue;
                }
                if (nNo != nNoCount) {
                        nNoCount ++;
                        continue;
                }
                if (pInfoTmp == pInfo) {
                        /*
                         * 編集ダイアログなどから同一ポインタが返ってきた場合は内容のみ更新済みとみなし、
                         * delete せずにそのまま保持する。
                         */
                        return;
                }

                DeleteTalkEvent (pInfoTmp);
                m_apTalkEvent[i] = pInfo;
                break;
        }
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::GetEventArray									 */
/* 内容		:指定ページ番号のイベント一覧を取得								 */
/* 日付		:2008/12/17														 */
/* ========================================================================= */

void CInfoTalkEvent::GetEventArray(int nPage, ARRAYTALKEVENTBASEINFO &aDst)
{
	int i, nCount;
	PCInfoTalkEventBase pInfo;

	aDst.clear();

	nCount = m_apTalkEvent.size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_apTalkEvent[i];
		if (pInfo->m_nPage != nPage) {
			continue;
		}
		aDst.push_back (pInfo);
	}
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::GetTalkEventCount								 */
/* 内容		:会話イベントページ数を取得										 */
/* 日付		:2008/12/29														 */
/* ========================================================================= */

int CInfoTalkEvent::GetTalkEventCount(int nPage)
{
	ARRAYTALKEVENTBASEINFO aInfoTmp;

	GetEventArray (nPage, aInfoTmp);

	return aInfoTmp.size();
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::GetPageCount									 */
/* 内容		:会話イベントページ数を取得										 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

int CInfoTalkEvent::GetPageCount(void)
{
	int nRet, i, nCount;
	PCInfoTalkEventBase pInfo;

	nRet = 0;

	nCount = m_apTalkEvent.size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_apTalkEvent[i];
		nRet = max (nRet, pInfo->m_nPage + 1);
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::AddTalkEvent									 */
/* 内容		:会話イベントを追加												 */
/* 日付		:2008/12/17														 */
/* ========================================================================= */

void CInfoTalkEvent::AddTalkEvent(CInfoTalkEventBase *pInfo)
{
	m_apTalkEvent.push_back (pInfo);
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::UpTalkEvent									 */
/* 内容		:指定イベントを1つ上に移動										 */
/* 日付		:2008/12/29														 */
/* ========================================================================= */

void CInfoTalkEvent::UpTalkEvent(int nPage, int nNo)
{
	int i, nNoFront, nCount, nNoCount;
	PCInfoTalkEventBase pInfo;

	if (nNo <= 0) {
		return;
	}

	nNoFront = 0;
	nNoCount = 0;
	nCount   = m_apTalkEvent.size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_apTalkEvent[i];
		if (pInfo->m_nPage != nPage) {
			continue;
		}
		if (nNo != nNoCount) {
			if (nNoCount == nNo - 1) {
				nNoFront = i;
			}
			nNoCount ++;
			continue;
		}
		m_apTalkEvent[i] = m_apTalkEvent[nNoFront];
		m_apTalkEvent[nNoFront] = pInfo;
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::DownTalkEvent									 */
/* 内容		:指定イベントを1つ下に移動										 */
/* 日付		:2008/12/29														 */
/* ========================================================================= */

void CInfoTalkEvent::DownTalkEvent(int nPage, int nNo)
{
	int i, nNoBack, nCount, nNoCount;
	PCInfoTalkEventBase pInfo;

	nCount = GetTalkEventCount (nPage);
	if (nNo >= nCount - 1) {
		return;
	}

	nNoBack = 0;
	nNoCount = nCount - 1;
	nCount   = m_apTalkEvent.size();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_apTalkEvent[i];
		if (pInfo->m_nPage != nPage) {
			continue;
		}
		if (nNo != nNoCount) {
			if (nNoCount == nNo + 1) {
				nNoBack = i;
			}
			nNoCount --;
			continue;
		}
		m_apTalkEvent[i] = m_apTalkEvent[nNoBack];
		m_apTalkEvent[nNoBack] = pInfo;
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::DeleteTalkEvent								 */
/* 内容		:会話イベントを削除												 */
/* 日付		:2008/12/17														 */
/* ========================================================================= */

void CInfoTalkEvent::DeleteTalkEvent(int nNo)
{
	PCInfoTalkEventBase pInfo;

	if ((nNo < 0) || (nNo >= static_cast<int>(m_apTalkEvent.size()))) {
		return;
	}

	pInfo = m_apTalkEvent[nNo];
	SAFE_DELETE (pInfo);
	m_apTalkEvent.erase (m_apTalkEvent.begin () + nNo);
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::DeleteTalkEvent								 */
/* 内容		:会話イベントを削除												 */
/* 日付		:2008/12/17														 */
/* ========================================================================= */

void CInfoTalkEvent::DeleteTalkEvent(int nPage, int nNo)
{
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::DeleteTalkEvent								 */
/* 内容		:会話イベントを削除												 */
/* 日付		:2008/12/17														 */
/* ========================================================================= */

void CInfoTalkEvent::DeleteTalkEvent(CInfoTalkEventBase *pInfo)
{
	SAFE_DELETE (pInfo);
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEvent::DeleteAllTalkEvent								 */
/* 内容		:会話イベントを全て削除											 */
/* 日付		:2008/12/20														 */
/* ========================================================================= */

void CInfoTalkEvent::DeleteAllTalkEvent(void)
{
	int i, nCount;

	nCount = m_apTalkEvent.size();
	for (i = nCount - 1; i >= 0; i --) {
		DeleteTalkEvent (i);
	}
	m_apTalkEvent.clear();
}

/* Copyright(C)URARA-works 2008 */
