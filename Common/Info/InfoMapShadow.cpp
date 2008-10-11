/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapShadow.cpp											 */
/* ���e			:�}�b�v�e�N���X �����t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/04													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapShadow.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"byViewType",		/* �\����� */
	"byAnimeType",		/* �A�j���[�V������� */
	"byAnimeCount",		/* �A�j���[�V�����R�}�� */
	"byLevel",			/* �����x */
	"wGrpID",			/* �O���t�B�b�NID */
	"dwShadowID",		/* �eID */
	"ptViewPos",		/* �ҏW��ʂł̕\���ʒu */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::CInfoMapShadow									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

CInfoMapShadow::CInfoMapShadow()
{
	m_byViewType		= 0;
	m_byAnimeType		= 0;
	m_byAnimeCount		= 0;
	m_byLevel			= 0;
	m_wGrpID			= 0;
	m_dwShadowID		= 0;
	ZeroMemory (&m_ptViewPos, sizeof (m_ptViewPos));

	m_byAnimeNo			= 0;
	m_dwLastAnime		= 0;

	m_paAnimeInfo = new ARRAYANIMEINFO;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::~CInfoMapShadow								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

CInfoMapShadow::~CInfoMapShadow()
{
	if (m_paAnimeInfo) {
		DeleteAllAnime ();
	}

	SAFE_DELETE (m_paAnimeInfo);
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::GetElementCount								 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

int CInfoMapShadow::GetElementCount(void)
{
	int nRet;
	PCInfoAnime pAnimeTmp;

	pAnimeTmp = new CInfoAnime;

	nRet = m_nElementCount;
	nRet += pAnimeTmp->GetElementCount ();

	SAFE_DELETE (pAnimeTmp);
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::GetElementNo									 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

int CInfoMapShadow::GetElementNo(LPCSTR pszName)
{
	int i, nRet;
	char szTmp[128];
	LPCSTR pszTmp;

	nRet = -1;

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}

	if (nRet < 0) {
		PCInfoAnime pAnimeTmp;

		pszTmp = strstr (pszName, PREFIX_INFOANIME);
		if (pszTmp == NULL) {
			goto Exit;
		}
		strcpy (szTmp, &pszName[strlen (PREFIX_INFOANIME)]);
		pAnimeTmp	= new CInfoAnime;
		nRet		= pAnimeTmp->GetElementNo (szTmp);
		SAFE_DELETE (pAnimeTmp);

		if (nRet < 0) {
			goto Exit;
		}
		nRet += i;
	}

Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::GetDataSize									 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

DWORD CInfoMapShadow::GetDataSize(void)
{
	int i, nCount;
	DWORD dwRet;

	dwRet = sizeof (m_byViewType)		+
			sizeof (m_byAnimeType)		+
			sizeof (m_byAnimeCount)		+
			sizeof (m_byLevel)			+
			sizeof (m_wGrpID)			+
			sizeof (m_dwShadowID)		+
			sizeof (m_ptViewPos);

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	nCount = m_paAnimeInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		PCInfoAnime pAnime;

		pAnime = m_paAnimeInfo->GetAt (i);
		dwRet += pAnime->GetDataSize ();
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::GetDataSizeNo									 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

DWORD CInfoMapShadow::GetDataSizeNo(int nNo)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAnime pAnime;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_byViewType);			break;
	case 1:		dwRet = sizeof (m_byAnimeType);			break;
	case 2:		dwRet = sizeof (m_byAnimeCount);		break;
	case 3:		dwRet = sizeof (m_byLevel);				break;
	case 4:		dwRet = sizeof (m_wGrpID);				break;
	case 5:		dwRet = sizeof (m_dwShadowID);			break;
	case 6:		dwRet = sizeof (m_ptViewPos);			break;
	default:
		nCount = m_paAnimeInfo->GetSize ();
		for (i = 0; i < nCount; i ++) {
			pAnime	= m_paAnimeInfo->GetAt (i);
			dwRet	+= pAnime->GetDataSizeNo (nNo - m_nElementCount);
		}
		break;
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::GetName										 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

LPCSTR CInfoMapShadow::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::GetWriteData									 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

PBYTE CInfoMapShadow::GetWriteData(int nNo, PDWORD pdwSize)
{
	int i, nCount;
	PBYTE pRet, pSrc;
	DWORD dwSize;
	PCInfoAnime pAnime;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:	pSrc = &m_byViewType;			break;
	case 1:	pSrc = &m_byAnimeType;			break;
	case 2:	pSrc = &m_byAnimeCount;			break;
	case 3:	pSrc = &m_byLevel;				break;
	case 4:	pSrc = (PBYTE)&m_wGrpID;		break;
	case 5:	pSrc = (PBYTE)&m_dwShadowID;	break;
	case 6:	pSrc = (PBYTE)&m_ptViewPos;		break;
	default:
		{
			PBYTE pTmp;

			pTmp = pRet;
			nCount = m_paAnimeInfo->GetSize ();
			for (i = 0; i < nCount; i ++) {
				PBYTE pSrcTmp;
				DWORD dwSizeTmp;

				pAnime	= m_paAnimeInfo->GetAt (i);
				pSrcTmp	= pAnime->GetWriteData (nNo - m_nElementCount, &dwSizeTmp);
				CopyMemoryRenew (pTmp, pSrcTmp, dwSizeTmp, pTmp);
				SAFE_DELETE_ARRAY (pSrcTmp);
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
/* �֐���	:CInfoMapShadow::ReadElementData								 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

DWORD CInfoMapShadow::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	int i;
	PBYTE pDst, pSrcTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoAnime pAnime;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = &m_byViewType;	dwSize = sizeof (m_byViewType);		break;
	case 1:	pDst = &m_byAnimeType;	dwSize = sizeof (m_byAnimeType);	break;
	case 2:
		dwSize = sizeof (BYTE);
		CopyMemory (&m_byAnimeCount, pSrc, dwSize);
		for (i = 0; i < m_byAnimeCount; i ++) {
			pAnime = new CInfoAnime;
			m_paAnimeInfo->Add (pAnime);
		}
		break;
	case 3:	pDst = &m_byLevel;				dwSize = sizeof (m_byLevel);		break;
	case 4:	pDst = (PBYTE)&m_wGrpID;		dwSize = sizeof (m_wGrpID);			break;
	case 5:	pDst = (PBYTE)&m_dwShadowID;	dwSize = sizeof (m_dwShadowID);		break;
	case 6:	pDst = (PBYTE)&m_ptViewPos;		dwSize = sizeof (m_ptViewPos);		break;
	default:
		pSrcTmp	= pSrc;
		for (i = 0; i < m_byAnimeCount; i ++) {
			pAnime		= m_paAnimeInfo->GetAt (i);
			dwSizeTmp	= pAnime->ReadElementData (pSrcTmp, nNo - m_nElementCount);
			dwSize		+= dwSizeTmp;
			pSrcTmp		+= dwSizeTmp;
		}
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::Copy											 */
/* ���e		:�R�s�[															 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

void CInfoMapShadow::Copy(CInfoMapShadow *pSrc)
{
	PBYTE pTmp;

	pTmp = pSrc->GetSendData ();
	SetSendData (pTmp);
	SAFE_DELETE_ARRAY (pTmp);
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

DWORD CInfoMapShadow::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAnime pAnime;

	dwRet = sizeof (m_dwShadowID)		+
			sizeof (m_byViewType)		+
			sizeof (m_byAnimeType)		+
			sizeof (m_byAnimeCount)		+
			sizeof (m_byLevel)			+
			sizeof (m_wGrpID)			+
			sizeof (m_ptViewPos);

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	nCount = m_paAnimeInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pAnime = m_paAnimeInfo->GetAt (i);
		dwRet += pAnime->GetSendDataSize ();
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

PBYTE CInfoMapShadow::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp, pDataAnimeTmp;
	DWORD dwSize, dwSizeAnime;
	PCInfoAnime pAnime;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwShadowID,	sizeof (m_dwShadowID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byViewType,	sizeof (m_byViewType),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byAnimeType,	sizeof (m_byAnimeType),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byAnimeCount,	sizeof (m_byAnimeCount),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byLevel,		sizeof (m_byLevel),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpID,		sizeof (m_wGrpID),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_ptViewPos,	sizeof (m_ptViewPos),		pDataTmp);

	nCount = m_paAnimeInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pAnime = m_paAnimeInfo->GetAt (i);
		pDataAnimeTmp	= pAnime->GetSendData ();
		dwSizeAnime		= pAnime->GetSendDataSize ();
		CopyMemoryRenew (pDataTmp, pDataAnimeTmp, dwSizeAnime, pDataTmp);
		SAFE_DELETE_ARRAY (pDataAnimeTmp);
	}

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

PBYTE CInfoMapShadow::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	PBYTE pDataTmp;
	PCInfoAnime pAnime;

	DeleteAllAnime ();
	pDataTmp = pSrc;

	CopyMemoryRenew (&m_dwShadowID,		pDataTmp, sizeof (m_dwShadowID),	pDataTmp);
	CopyMemoryRenew (&m_byViewType,		pDataTmp, sizeof (m_byViewType),	pDataTmp);
	CopyMemoryRenew (&m_byAnimeType,	pDataTmp, sizeof (m_byAnimeType),	pDataTmp);
	CopyMemoryRenew (&m_byAnimeCount,	pDataTmp, sizeof (m_byAnimeCount),	pDataTmp);
	CopyMemoryRenew (&m_byLevel,		pDataTmp, sizeof (m_byLevel),		pDataTmp);
	CopyMemoryRenew (&m_wGrpID,			pDataTmp, sizeof (m_wGrpID),		pDataTmp);
	CopyMemoryRenew (&m_ptViewPos,		pDataTmp, sizeof (m_ptViewPos),		pDataTmp);

	nCount = m_byAnimeCount;
	for (i = 0; i < nCount; i ++) {
		pAnime = new CInfoAnime;
		pAnime->SetSendData (pDataTmp);
		pDataTmp += pAnime->GetSendDataSize ();
		m_paAnimeInfo->Add (pAnime);
	}

	return pDataTmp;
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::TimerProc										 */
/* ���e		:���ԏ���														 */
/* �߂�l	:TRUE:�������� FALS:�������Ă��Ȃ�								 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

BOOL CInfoMapShadow::TimerProc(DWORD dwTime)
{
	BOOL bRet;
	BYTE byAnimeNoBack;
	PCInfoAnime pAnime;

	bRet = FALSE;
	byAnimeNoBack = m_byAnimeNo;

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	pAnime = m_paAnimeInfo->GetAt (m_byAnimeNo);
	if (dwTime - m_dwLastAnime >= (DWORD)(pAnime->m_byWait * 10)) {
		if (m_dwLastAnime > 0) {
			m_byAnimeNo ++;
		}
		m_dwLastAnime = dwTime;
	}
	m_byAnimeNo = (m_byAnimeNo >= m_byAnimeCount) ? 0 : m_byAnimeNo;

Exit:
	if (byAnimeNoBack != m_byAnimeNo) {
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::GetAnimeCount									 */
/* ���e		:�A�j���[�V�����R�}�����擾										 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

int CInfoMapShadow::GetAnimeCount(void)
{
	return m_paAnimeInfo->GetSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::AddAnime										 */
/* ���e		:�A�j���[�V�����R�}��ǉ�										 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

void CInfoMapShadow::AddAnime(void)
{
	PCInfoAnime pInfo;

	pInfo = new CInfoAnime;
	m_paAnimeInfo->Add (pInfo);
	m_byAnimeCount = m_paAnimeInfo->GetSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::DeleteAnime									 */
/* ���e		:�A�j���[�V�����R�}���폜										 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

void CInfoMapShadow::DeleteAnime(int nNo)
{
	PCInfoAnime pInfo;

	pInfo = m_paAnimeInfo->GetAt (nNo);
	m_paAnimeInfo->RemoveAt (nNo);
	SAFE_DELETE (pInfo);
	m_byAnimeCount = m_paAnimeInfo->GetSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::DeleteAllAnime									 */
/* ���e		:�A�j���[�V�����R�}��S�č폜									 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

void CInfoMapShadow::DeleteAllAnime(void)
{
	int i, nCount;

	nCount = m_paAnimeInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		DeleteAnime (0);
	}
}


/* ========================================================================= */
/* �֐���	:CInfoMapShadow::GetAnimePtr									 */
/* ���e		:�A�j���[�V�����R�}���擾										 */
/* ���t		:2007/06/04														 */
/* ========================================================================= */

PCInfoAnime CInfoMapShadow::GetAnimePtr(int nNo)
{
	int nCount;
	PCInfoAnime pRet;

	pRet = NULL;

	nCount = m_paAnimeInfo->GetSize ();
	if (nNo >= nCount) {
		goto Exit;
	}

	pRet = m_paAnimeInfo->GetAt (nNo);
Exit:
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
