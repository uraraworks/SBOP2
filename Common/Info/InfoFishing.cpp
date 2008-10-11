/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoFishing.cpp											 */
/* ���e			:�ނ���N���X �����t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/02													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoFishing.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_dwFishingID",	/* �ނ�ID */
	"m_nAverage",		/* �ނ��m�� */
	"m_strName",		/* �ނ�ꖼ */
	"nArrayCount",		/* �ނ���p�����[�^�� */
	"a_dwItemTypeID",	/* �A�C�e�����ID */
	"a_nAverage",		/* �ނ肫��m�� */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoFishing::CInfoFishing										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

CInfoFishing::CInfoFishing()
{
	m_dwFishingID	= 0;
	m_nAverage		= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoFishing::~CInfoFishing									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

CInfoFishing::~CInfoFishing()
{
}


/* ========================================================================= */
/* �֐���	:CInfoFishing::GetElementNo										 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

int CInfoFishing::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoFishing::GetDataSize										 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

DWORD CInfoFishing::GetDataSize(void)
{
	int nCount;
	DWORD dwRet;

	dwRet = 0;
	nCount = m_apParam.GetSize ();

	dwRet += sizeof (m_dwFishingID);		/* �ނ�ID */
	dwRet += sizeof (m_nAverage);			/* �ނ��m�� */
	dwRet += (m_strName.GetLength () + 1);	/* �ނ�ꖼ */
	dwRet += sizeof (int);					/* �ނ���p�����[�^�� */
	/* �ނ���p�����[�^ */
	dwRet += (nCount * sizeof (DWORD));		/* �A�C�e�����ID */
	dwRet += (nCount * sizeof (int));		/* �ނ肫��m�� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoFishing::GetDataSizeNo									 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

DWORD CInfoFishing::GetDataSizeNo(int nNo)
{
	int nCount;
	DWORD dwRet;

	dwRet = 0;
	nCount = m_apParam.GetSize ();

	switch (nNo) {
	case 0:		dwRet = sizeof (m_dwFishingID);			break;	/* �ނ�ID */
	case 1:		dwRet = sizeof (m_nAverage);			break;	/* �ނ��m�� */
	case 2:		dwRet = (m_strName.GetLength () + 1);	break;	/* �ނ�ꖼ */
	case 3:		dwRet = nCount * sizeof (int);			break;	/* �ނ���p�����[�^�� */
	case 4:		dwRet = nCount * sizeof (DWORD);		break;	/* �A�C�e�����ID */
	case 5:		dwRet = nCount * sizeof (int);			break;	/* �ނ肫��m�� */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoFishing::GetName											 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

LPCSTR CInfoFishing::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoFishing::GetWriteData										 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

PBYTE CInfoFishing::GetWriteData(int nNo, PDWORD pdwSize)
{
	int i, nCount, *pnTmp;
	PBYTE pRet, pSrc;
	DWORD dwSize, *pdwTmp;
	PINFOFISHINGPARAM pInfo;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;
	nCount		= m_apParam.GetSize ();

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:		pSrc = (PBYTE)&m_dwFishingID;		break;	/* �ނ�ID */
	case 1:		pSrc = (PBYTE)&m_nAverage;			break;	/* �ނ��m�� */
	case 2:		pSrc = (PBYTE)(LPCSTR)m_strName;	break;	/* �ނ�ꖼ */
	case 3:		pSrc = (PBYTE)&nCount;				break;	/* �ނ���p�����[�^�� */
	case 4:		/* �A�C�e�����ID */
		pdwTmp = (PDWORD)pRet;
		for (i = 0; i < nCount; i ++) {
			pInfo = GetPtr (i);
			pdwTmp[i] = pInfo->dwItemTypeID;
		}
		break;
	case 5:		/* �ނ肫��m�� */
		pnTmp = (int *)pRet;
		for (i = 0; i < nCount; i ++) {
			pInfo = GetPtr (i);
			pnTmp[i] = pInfo->nAverage;
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
/* �֐���	:CInfoFishing::ReadElementData									 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

DWORD CInfoFishing::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	int i, nCount, *pnTmp;
	PBYTE pDst;
	DWORD dwSize, *pdwTmp;
	PINFOFISHINGPARAM pInfo;

	pDst	= NULL;
	dwSize	= GetDataSizeNo (nNo);
	nCount	= m_apParam.GetSize ();

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_dwFishingID;		break;	/* �ނ�ID */
	case 1:		pDst = (PBYTE)&m_nAverage;			break;	/* �ނ��m�� */
	case 2:		/* �ނ�ꖼ */
		m_strName = (LPCSTR)pSrc;
		dwSize = m_strName.GetLength () + 1;
		break;
	case 3:		/* �ނ���p�����[�^�� */
		CopyMemory ((PBYTE)&nCount, pSrc, dwSize);
		for (i = 0; i < nCount; i ++) {
			AddParam (0, 0);
		}
		break;
	case 4:		/* �A�C�e�����ID */
		pdwTmp = (PDWORD)pSrc;
		for (i = 0; i < nCount; i ++) {
			pInfo = GetPtr (i);
			pInfo->dwItemTypeID = pdwTmp[i];
		}
		break;
	case 5:		/* �ނ肫��m�� */
		pnTmp = (int *)pSrc;
		for (i = 0; i < nCount; i ++) {
			pInfo = GetPtr (i);
			pInfo->nAverage = pnTmp[i];
		}
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoFishing::GetSendDataSize									 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

DWORD CInfoFishing::GetSendDataSize(void)
{
	return GetDataSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoFishing::GetSendData										 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

PBYTE CInfoFishing::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PINFOFISHINGPARAM pInfo;

	nCount	= m_apParam.GetSize ();
	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwFishingID,	sizeof (m_dwFishingID),	pDataTmp);	/* �ނ�ID */
	CopyMemoryRenew (pDataTmp, &m_nAverage,		sizeof (m_nAverage),	pDataTmp);	/* �ނ��m�� */
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp);								/* �ނ�ꖼ */
	CopyMemoryRenew (pDataTmp, &nCount,			sizeof (nCount),		pDataTmp);	/* �ނ���p�����[�^�� */
	for (i = 0; i < nCount; i ++) {
		pInfo = GetPtr (i);
		CopyMemoryRenew (pDataTmp, &pInfo->dwItemTypeID,	sizeof (pInfo->dwItemTypeID),	pDataTmp);	/* �A�C�e�����ID */
		CopyMemoryRenew (pDataTmp, &pInfo->nAverage,		sizeof (pInfo->nAverage),		pDataTmp);	/* �ނ肫��m�� */
	}

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoFishing::SetSendData										 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

PBYTE CInfoFishing::SetSendData(PBYTE pSrc)
{
	int i, nCount, nAverage;
	DWORD dwItemTypeID;
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwFishingID,	pDataTmp, sizeof (m_dwFishingID),	pDataTmp);	/* �ނ�ID */
	CopyMemoryRenew (&m_nAverage,		pDataTmp, sizeof (m_nAverage),		pDataTmp);	/* �ނ��m�� */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp);									/* �ނ�ꖼ */
	CopyMemoryRenew (&nCount,			pDataTmp, sizeof (nCount),			pDataTmp);	/* �ނ���p�����[�^�� */

	DeleteAllParam ();
	for (i = 0; i < nCount; i ++) {
		CopyMemoryRenew (&dwItemTypeID,	pDataTmp, sizeof (dwItemTypeID),	pDataTmp);	/* �A�C�e�����ID */
		CopyMemoryRenew (&nAverage,		pDataTmp, sizeof (nAverage),		pDataTmp);	/* �ނ肫��m�� */
		AddParam (dwItemTypeID, nAverage);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoFishing::GetCount											 */
/* ���e		:�ނ���p�����[�^�����擾										 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

int CInfoFishing::GetCount(void)
{
	return m_apParam.GetSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoFishing::AddParam											 */
/* ���e		:�ނ���p�����[�^��ǉ�										 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

void CInfoFishing::AddParam(
	DWORD dwItemTypeID,		/* [in] �A�C�e�����ID */
	int nAverage)			/* [in] �ނ肫��m�� */
{
	PINFOFISHINGPARAM pInfo;

	pInfo = new INFOFISHINGPARAM;
	pInfo->dwItemTypeID	= dwItemTypeID;
	pInfo->nAverage		= nAverage;

	m_apParam.Add (pInfo);
}


/* ========================================================================= */
/* �֐���	:CInfoFishing::DeleteParam										 */
/* ���e		:�ނ���p�����[�^���폜										 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

void CInfoFishing::DeleteParam(int nNo)
{
	PINFOFISHINGPARAM pInfo;

	if (nNo >= m_apParam.GetSize ()) {
		return;
	}
	pInfo = m_apParam[nNo];
	SAFE_DELETE (pInfo);
	m_apParam.RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CInfoFishing::DeleteAllParam									 */
/* ���e		:�ނ���p�����[�^��S�č폜									 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

void CInfoFishing::DeleteAllParam(void)
{
	int i, nCount;

	nCount = GetCount ();
	for (i = nCount - 1; i >= 0; i --) {
		DeleteParam (i);
	}
}


/* ========================================================================= */
/* �֐���	:CInfoFishing::DeleteAllParam									 */
/* ���e		:�ނ���p�����[�^��S�č폜									 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

PINFOFISHINGPARAM CInfoFishing::GetPtr(int nNo)
{
	PINFOFISHINGPARAM pRet;

	pRet = NULL;
	if (nNo >= m_apParam.GetSize ()) {
		goto Exit;
	}

	pRet = m_apParam[nNo];
Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoFishing::Copy												 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

void CInfoFishing::Copy(CInfoFishing *pSrc)
{
	int i, nCount;
	PINFOFISHINGPARAM pInfo;

	m_dwFishingID	= pSrc->m_dwFishingID;
	m_nAverage		= pSrc->m_nAverage;
	m_strName		= pSrc->m_strName;
	nCount			= pSrc->GetCount ();

	DeleteAllParam ();
	for (i = 0; i < nCount; i ++) {
		pInfo = pSrc->GetPtr (i);
		AddParam (pInfo->dwItemTypeID, pInfo->nAverage);
	}
}

/* Copyright(C)URARA-works 2008 */
