/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoTalkEventMENU.cpp										 */
/* ���e			:��b�C�x���g���(���ڑI��)�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/28													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoTalkEventMENU.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"nCount",			/* ���ڐ� */
	"nPage",			/* �W�����v��y�[�W�ԍ� */
	"strName",			/* ���ږ� */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::CInfoTalkEventMENU							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

CInfoTalkEventMENU::CInfoTalkEventMENU()
{
	m_nEventType = TALKEVENTTYPE_MENU;

	m_nElementCountBase = m_nElementCount;
	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::~CInfoTalkEventMENU						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

CInfoTalkEventMENU::~CInfoTalkEventMENU()
{
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::GetElementCount							 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

int CInfoTalkEventMENU::GetElementCount(void)
{
	return m_nElementCountBase + m_nElementCount;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::GetElementNo								 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2005/07/05														 */
/* ========================================================================= */

int CInfoTalkEventMENU::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = CInfoTalkEventBase::GetElementNo (pszName);
	if (nRet >= 0) {
		goto Exit;
	}

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = m_nElementCountBase + i;
			break;
		}
	}

Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::GetDataSize								 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

DWORD CInfoTalkEventMENU::GetDataSize(void)
{
	int i, nCount;
	DWORD dwRet;
	PSTTALKEVENTMENUINFO pInfo;

	nCount = m_aMenuInfo.GetSize ();

	dwRet = CInfoTalkEventBase::GetDataSize();
	dwRet += sizeof (int);				/* ���ڐ� */
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aMenuInfo[i];
		dwRet += sizeof (int);						/* �W�����v��y�[�W�ԍ� */
		dwRet += (pInfo->strName.GetLength () + 1);	/* ���ږ� */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::GetDataSizeNo								 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

DWORD CInfoTalkEventMENU::GetDataSizeNo(int nNo)
{
	int i, nCount;
	DWORD dwRet;
	PSTTALKEVENTMENUINFO pInfo;

	dwRet = 0;
	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::GetDataSizeNo (nNo);
	}

	nCount = m_aMenuInfo.GetSize ();

	switch (nNo - m_nElementCountBase) {
	case 0:			/* ���ڐ� */
		dwRet = sizeof (int);
		break;
	case 1:			/* �W�����v��y�[�W�ԍ� */
		dwRet = sizeof (int) * nCount;
		break;
	case 2:			/* ���ږ� */
		for (i = 0; i < nCount; i ++) {
			pInfo = m_aMenuInfo[i];
			dwRet += (pInfo->strName.GetLength () + 1);
		}
		break;
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::GetName									 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2005/07/05														 */
/* ========================================================================= */

LPCSTR CInfoTalkEventMENU::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::GetName (nNo);
	}
	return s_aszName[nNo - m_nElementCountBase];
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::GetWriteData								 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

PBYTE CInfoTalkEventMENU::GetWriteData(int nNo, PDWORD pdwSize)
{
	int i, nCount;
	DWORD dwSize;
	PBYTE pRet, pSrc, pDataTmp;
	PSTTALKEVENTMENUINFO pInfo;

	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::GetWriteData (nNo, pdwSize);
	}

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet	 = ZeroNew (dwSize);
	nCount	 = m_aMenuInfo.GetSize ();
	pDataTmp = pRet;

	nNo -= m_nElementCountBase;
	switch (nNo) {
	case 0:			/* ���ڐ� */
		pSrc = (PBYTE)&nCount;
		break;
	case 1:			/* �W�����v��y�[�W�ԍ� */
		for (i = 0; i < nCount; i ++) {
			pInfo = m_aMenuInfo[i];
			CopyMemoryRenew (pDataTmp, &pInfo->nPage, sizeof (pInfo->nPage), pDataTmp);
		}
		break;
	case 2:			/* ���ږ� */
		for (i = 0; i < nCount; i ++) {
			pInfo = m_aMenuInfo[i];
			strcpyRenew ((LPSTR)pDataTmp, pInfo->strName, pDataTmp);
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
/* �֐���	:CInfoTalkEventMENU::ReadElementData							 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

DWORD CInfoTalkEventMENU::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	int i, nCount;
	DWORD dwSize;
	PBYTE pDst, pDataTmp;
	PSTTALKEVENTMENUINFO pInfo;

	pDst	= NULL;
	dwSize	= 0;

	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::ReadElementData (pSrc, nNo);
	}

	pDataTmp = pSrc;
	nCount	 = m_aMenuInfo.GetSize ();

	nNo -= m_nElementCountBase;
	switch (nNo) {
	case 0:			/* ���ڐ� */
		DeleteAllMenuInfo ();
		nCount = 0;
		CopyMemoryRenew (&nCount, pDataTmp, sizeof (nCount), pDataTmp);
		for (i = 0; i < nCount; i ++) {
			AddMenuInfo (0, NULL);
		}
		dwSize = (pDataTmp - pSrc);
		break;
	case 1:			/* �W�����v��y�[�W�ԍ� */
		for (i = 0; i < nCount; i ++) {
			pInfo = m_aMenuInfo[i];
			CopyMemoryRenew (&pInfo->nPage, pDataTmp, sizeof (pInfo->nPage), pDataTmp);
		}
		dwSize = (pDataTmp - pSrc);
		break;
	case 2:			/* ���ږ� */
		for (i = 0; i < nCount; i ++) {
			pInfo = m_aMenuInfo[i];
			StoreRenew (pInfo->strName, (LPCSTR)pDataTmp, pDataTmp);
		}
		dwSize = (pDataTmp - pSrc);
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::GetSendDataSize							 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

DWORD CInfoTalkEventMENU::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoTalkEventBase::GetSendDataSize ();
	dwRet += GetDataSizeNo (m_nElementCountBase + 0);	/* ���ڐ� */
	dwRet += GetDataSizeNo (m_nElementCountBase + 1);	/* �W�����v��y�[�W�ԍ� */
	dwRet += GetDataSizeNo (m_nElementCountBase + 2);	/* ���ږ� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::GetSendData								 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

PBYTE CInfoTalkEventMENU::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;
	PSTTALKEVENTMENUINFO pInfo;

	dwSize	 = GetSendDataSize ();
	pData	 = ZeroNew (dwSize);
	pDataTmp = pData;

	pDataBase  = CInfoTalkEventBase::GetSendData ();
	dwSizeBase = CInfoTalkEventBase::GetSendDataSize ();
	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);
	SAFE_DELETE_ARRAY (pDataBase);

	nCount = m_aMenuInfo.GetSize ();
	CopyMemoryRenew (pDataTmp, &nCount, sizeof (nCount), pDataTmp);		/* ���ڐ� */

	for (i = 0; i < nCount; i ++) {
		pInfo = m_aMenuInfo[i];
		/* �W�����v��y�[�W�ԍ� */
		CopyMemoryRenew (pDataTmp, &pInfo->nPage, sizeof (pInfo->nPage), pDataTmp);
		/* ���ږ� */
		strcpyRenew ((LPSTR)pDataTmp, pInfo->strName, pDataTmp);
	}

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::SetSendData								 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

PBYTE CInfoTalkEventMENU::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	PBYTE pRet, pDataTmp;
	PSTTALKEVENTMENUINFO pInfo;

	pRet = pSrc;
	DeleteAllMenuInfo ();

	pDataTmp = CInfoTalkEventBase::SetSendData (pSrc);
	CopyMemoryRenew (&nCount, pDataTmp, sizeof (nCount), pDataTmp);		/* ���ڐ� */
	for (i = 0; i < nCount; i ++) {
		AddMenuInfo (0, NULL);
		pInfo = m_aMenuInfo[i];
		/* �W�����v��y�[�W�ԍ� */
		CopyMemoryRenew (&pInfo->nPage, pDataTmp, sizeof (pInfo->nPage), pDataTmp);
		/* ���ږ� */
		StoreRenew (pInfo->strName, (LPCSTR)pDataTmp, pDataTmp);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::Copy										 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

void CInfoTalkEventMENU::Copy(CInfoTalkEventBase *pSrc)
{
	int i, nCount;
	PSTTALKEVENTMENUINFO pInfo;
	PCInfoTalkEventMENU pSrcTmp;

	if (pSrc == NULL) {
		return;
	}
	pSrcTmp = (PCInfoTalkEventMENU)pSrc;
	DeleteAllMenuInfo ();

	CInfoTalkEventBase::Copy (pSrcTmp);

	nCount = pSrcTmp->GetMenuInfoCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = pSrcTmp->GetPtr (i);
		AddMenuInfo (pInfo->nPage, (LPCSTR)pInfo->strName);
	}
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::DeleteMenuInfo								 */
/* ���e		:���ڏ����폜													 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

void CInfoTalkEventMENU::DeleteMenuInfo(int nNo)
{
	PSTTALKEVENTMENUINFO pInfo;

	if (nNo >= m_aMenuInfo.GetSize ()) {
		return;
	}

	pInfo = m_aMenuInfo[nNo];
	SAFE_DELETE (pInfo);
	m_aMenuInfo.RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::DeleteAllMenuInfo							 */
/* ���e		:���ڏ���S�č폜												 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

void CInfoTalkEventMENU::DeleteAllMenuInfo(void)
{
	int i, nCount;

	nCount = GetMenuInfoCount ();
	for (i = nCount - 1; i >= 0; i --) {
		DeleteMenuInfo (i);
	}
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::AddMenuInfo								 */
/* ���e		:���ڏ���ǉ�													 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

void CInfoTalkEventMENU::AddMenuInfo(int nPage, LPCSTR pszName)
{
	PSTTALKEVENTMENUINFO pInfo;

	pInfo = new STTALKEVENTMENUINFO;
	pInfo->nPage	= nPage;		/* �W�����v��y�[�W�ԍ� */
	pInfo->strName	= pszName;		/* ���ږ� */
	m_aMenuInfo.Add (pInfo);
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::GetMenuInfoCount							 */
/* ���e		:���ڐ����擾													 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

int CInfoTalkEventMENU::GetMenuInfoCount(void)
{
	return m_aMenuInfo.GetSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventMENU::GetPtr										 */
/* ���e		:���ڏ����擾													 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

PSTTALKEVENTMENUINFO CInfoTalkEventMENU::GetPtr(int nNo)
{
	if (nNo >= m_aMenuInfo.GetSize ()) {
		return NULL;
	}

	return m_aMenuInfo[nNo];
}

/* Copyright(C)URARA-works 2008 */
