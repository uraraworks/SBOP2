/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoTalkEvent.cpp											 */
/* ���e			:��b�C�x���g���N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/16													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoTalkEventMENU.h"
#include "InfoTalkEvent.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_dwTalkEventID",	/* ��b�C�x���gID */
	"m_apTalkEvent",	/* ��b�C�x���g */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::CInfoTalkEvent									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/16														 */
/* ========================================================================= */

CInfoTalkEvent::CInfoTalkEvent()
{
	m_dwTalkEventID	= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::~CInfoTalkEvent								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/16														 */
/* ========================================================================= */

CInfoTalkEvent::~CInfoTalkEvent()
{
	DeleteAllTalkEvent ();
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::GetElementNo									 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2005/07/05														 */
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
/* �֐���	:CInfoTalkEvent::GetDataSize									 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/12/16														 */
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
/* �֐���	:CInfoTalkEvent::GetDataSizeNo									 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/12/16														 */
/* ========================================================================= */

DWORD CInfoTalkEvent::GetDataSizeNo(int nNo)
{
	int i, j, nCount, nCount2;
	DWORD dwRet;
	PCInfoTalkEventBase pInfo;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof (m_dwTalkEventID);	break;	/* ��b�C�x���gID */
	case 1:		/* ��b�C�x���g */
		dwRet += sizeof (int);				/* �f�[�^�� */
		nCount = m_apTalkEvent.GetSize ();
		for (i = 0; i < nCount; i ++) {
			pInfo = m_apTalkEvent[i];
			dwRet += sizeof (int);								/* �v�f�� */
			nCount2 = pInfo->GetElementCount ();
			for (j = 0; j < nCount2; j ++) {
				dwRet += (strlen (pInfo->GetName (j)) + 1);		/* �v�f�� */
				dwRet += sizeof (DWORD);						/* �f�[�^�T�C�Y */
				dwRet += pInfo->GetDataSizeNo (j);				/* �f�[�^ */
			}
		}
		break;
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::GetName										 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2005/07/05														 */
/* ========================================================================= */

LPCSTR CInfoTalkEvent::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::GetWriteData									 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/12/16														 */
/* ========================================================================= */

PBYTE CInfoTalkEvent::GetWriteData(int nNo, PDWORD pdwSize)
{
	int i, j, nCount, nCount2;
	PBYTE pRet, pSrc, pRetTmp, pTmp;
	DWORD dwSize, dwTmp;
	PCInfoTalkEventBase pInfo;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = ZeroNew (dwSize);
	pRetTmp = pRet;

	switch (nNo) {
	case 0:	pSrc = (PBYTE)&m_dwTalkEventID;			break;	/* ��b�C�x���gID */
	case 1:			/* ��b�C�x���g */
		nCount = m_apTalkEvent.GetSize ();
		CopyMemoryRenew (pRetTmp, &nCount, sizeof (nCount), pRetTmp);		/* �f�[�^�� */
		for (i = 0; i < nCount; i ++) {
			pInfo = m_apTalkEvent[i];
			nCount2 = pInfo->GetElementCount ();
			CopyMemoryRenew (pRetTmp, &nCount2, sizeof (nCount2), pRetTmp);	/* �v�f�� */

			for (j = 0; j < nCount2; j ++) {
				strcpyRenew ((LPSTR)pRetTmp, pInfo->GetName (j), pRetTmp); 	/* �v�f�� */
				dwTmp = pInfo->GetDataSizeNo (j);
				CopyMemoryRenew (pRetTmp, &dwTmp, sizeof (dwTmp), pRetTmp);	/* �f�[�^�T�C�Y */
				pTmp = pInfo->GetWriteData (j, &dwTmp);
				CopyMemoryRenew (pRetTmp, pTmp, dwTmp, pRetTmp);			/* �f�[�^ */
				SAFE_DELETE_ARRAY (pTmp);
			}
		}
		break;
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::ReadElementData								 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/12/16														 */
/* ========================================================================= */

DWORD CInfoTalkEvent::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	int i, j, nNoTmp, nCount, nCount2, nType;
	PBYTE pDst, pSrcTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoTalkEventBase pInfo;
	CmyString strTmp;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = (PBYTE)&m_dwTalkEventID; dwSize = sizeof (m_dwTalkEventID); break;	/* ��b�C�x���gID */
	case 1:				/* ��b�C�x���g */
		pSrcTmp = pSrc;
		CopyMemoryRenew (&nCount, pSrcTmp, sizeof (nCount), pSrcTmp);				/* �f�[�^�� */
		for (i = 0; i < nCount; i ++) {
			CopyMemoryRenew (&nCount2, pSrcTmp, sizeof (nCount2), pSrcTmp);			/* �v�f�� */

			for (j = 0; j < nCount2; j ++) {
				StoreRenew (strTmp, (LPCSTR)pSrcTmp, pSrcTmp);						/* �v�f�� */
				CopyMemoryRenew (&dwSizeTmp, pSrcTmp, sizeof (dwSizeTmp), pSrcTmp);	/* �f�[�^�T�C�Y */
				if (j == 0) {
					/* �ŏ��̓C�x���g��� */
					CopyMemoryRenew (&nType, pSrcTmp, sizeof (nType), pSrcTmp);		/* �C�x���g��� */
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
/* �֐���	:CInfoTalkEvent::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/12/16														 */
/* ========================================================================= */

DWORD CInfoTalkEvent::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoTalkEventBase pInfo;

	dwRet = sizeof (m_dwTalkEventID);	/* ��b�C�x���gID */

	/* �f�[�^�����̃T�C�Y */
	dwRet += sizeof (DWORD);

	nCount = m_apTalkEvent.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_apTalkEvent[i];

		dwRet += pInfo->GetSendDataSize ();
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/12/16														 */
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

	CopyMemoryRenew (pDataTmp, &m_dwTalkEventID, sizeof (m_dwTalkEventID), pDataTmp);	/* ��b�C�x���gID */

	/* �f�[�^������������ */
	dwCount = (DWORD)m_apTalkEvent.GetSize ();
	CopyMemoryRenew (pDataTmp, &dwCount, sizeof (dwCount), pDataTmp);

	/* �C�x���g������������ */
	nCount = (int)dwCount;
	for (i = 0; i < nCount; i ++) {
		pInfo = m_apTalkEvent[i];

		dwSizeTmp	= pInfo->GetSendDataSize ();
		pDataTmp2	= pInfo->GetSendData ();
		CopyMemoryRenew (pDataTmp, pDataTmp2, dwSizeTmp, pDataTmp);	/* ��b�C�x���g */

		SAFE_DELETE_ARRAY (pDataTmp2);
	}

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/12/16														 */
/* ========================================================================= */

PBYTE CInfoTalkEvent::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	PBYTE pDataTmp;
	DWORD dwCount;
	CInfoTalkEventBase InfoTmp, *pInfo;

	DeleteAllTalkEvent ();

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwTalkEventID, pDataTmp, sizeof (m_dwTalkEventID), pDataTmp);	/* ��b�C�x���gID */

	/* �f�[�^����ǂݍ��� */
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
/* �֐���	:CInfoTalkEvent::Copy											 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/12/16														 */
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
/* �֐���	:CInfoTalkEvent::GetNew											 */
/* ���e		:�V������b�C�x���g���擾										 */
/* ���t		:2008/12/17														 */
/* ========================================================================= */

PCInfoTalkEventBase CInfoTalkEvent::GetNew(int nType)
{
	PCInfoTalkEventBase pRet;

	pRet = NULL;

	switch (nType) {
	case TALKEVENTTYPE_MSG:				/* ���b�Z�[�W�\�� */
		pRet = new CInfoTalkEventBase;
		break;
	case TALKEVENTTYPE_MENU:			/* ���ڑI�� */
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
/* �֐���	:CInfoTalkEvent::GetPtr											 */
/* ���e		:��b�C�x���g���擾												 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

PCInfoTalkEventBase CInfoTalkEvent::GetPtr(int nPage, int nNo)
{
	int i, nCount, nNoCount;
	PCInfoTalkEventBase pInfo, pRet;

	pRet	 = NULL;
	nNoCount = 0;
	nCount   = m_apTalkEvent.GetSize ();

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
/* �֐���	:CInfoTalkEvent::SetPtr											 */
/* ���e		:��b�C�x���g�������ւ�											 */
/* ���t		:2008/12/17														 */
/* ========================================================================= */

void CInfoTalkEvent::SetPtr(int nPage, int nNo, CInfoTalkEventBase *pInfo)
{
	int i, nCount, nNoCount;
	PCInfoTalkEventBase pInfoTmp;

	nNoCount = 0;
	nCount   = m_apTalkEvent.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_apTalkEvent[i];
		if (pInfoTmp->m_nPage != nPage) {
			continue;
		}
		if (nNo != nNoCount) {
			nNoCount ++;
			continue;
		}
		DeleteTalkEvent (pInfoTmp);
		m_apTalkEvent.SetAt (i, pInfo);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::GetEventArray									 */
/* ���e		:�w��y�[�W�ԍ��̃C�x���g�ꗗ���擾								 */
/* ���t		:2008/12/17														 */
/* ========================================================================= */

void CInfoTalkEvent::GetEventArray(int nPage, ARRAYTALKEVENTBASEINFO &aDst)
{
	int i, nCount;
	PCInfoTalkEventBase pInfo;

	aDst.RemoveAll ();

	nCount = m_apTalkEvent.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_apTalkEvent[i];
		if (pInfo->m_nPage != nPage) {
			continue;
		}
		aDst.Add (pInfo);
	}
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::GetPageCount									 */
/* ���e		:��b�C�x���g�y�[�W�����擾										 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

int CInfoTalkEvent::GetPageCount(void)
{
	int nRet, i, nCount;
	PCInfoTalkEventBase pInfo;

	nRet = 0;

	nCount = m_apTalkEvent.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_apTalkEvent[i];
		nRet = max (nRet, pInfo->m_nPage + 1);
	}

	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::AddTalkEvent									 */
/* ���e		:��b�C�x���g��ǉ�												 */
/* ���t		:2008/12/17														 */
/* ========================================================================= */

void CInfoTalkEvent::AddTalkEvent(CInfoTalkEventBase *pInfo)
{
	m_apTalkEvent.Add (pInfo);
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::DeleteTalkEvent								 */
/* ���e		:��b�C�x���g���폜												 */
/* ���t		:2008/12/17														 */
/* ========================================================================= */

void CInfoTalkEvent::DeleteTalkEvent(int nNo)
{
	PCInfoTalkEventBase pInfo;

	if (nNo >= m_apTalkEvent.GetSize ()) {
		return;
	}

	pInfo = m_apTalkEvent[nNo];
	SAFE_DELETE (pInfo);
	m_apTalkEvent.RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::DeleteTalkEvent								 */
/* ���e		:��b�C�x���g���폜												 */
/* ���t		:2008/12/17														 */
/* ========================================================================= */

void CInfoTalkEvent::DeleteTalkEvent(int nPage, int nNo)
{
	int i, nCount, nNoCount;
	PCInfoTalkEventBase pInfo;

	nNoCount = 0;
	nCount   = m_apTalkEvent.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_apTalkEvent[i];
		if (pInfo->m_nPage != nPage) {
			continue;
		}
		if (nNo != nNoCount) {
			nNoCount ++;
			continue;
		}
		DeleteTalkEvent (i);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::DeleteTalkEvent								 */
/* ���e		:��b�C�x���g���폜												 */
/* ���t		:2008/12/17														 */
/* ========================================================================= */

void CInfoTalkEvent::DeleteTalkEvent(CInfoTalkEventBase *pInfo)
{
	SAFE_DELETE (pInfo);
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEvent::DeleteAllTalkEvent								 */
/* ���e		:��b�C�x���g��S�č폜											 */
/* ���t		:2008/12/20														 */
/* ========================================================================= */

void CInfoTalkEvent::DeleteAllTalkEvent(void)
{
	int i, nCount;

	nCount = m_apTalkEvent.GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		DeleteTalkEvent (i);
	}
	m_apTalkEvent.RemoveAll ();
}

/* Copyright(C)URARA-works 2008 */
