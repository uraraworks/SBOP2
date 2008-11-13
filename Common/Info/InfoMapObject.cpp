/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapObject.cpp											 */
/* ���e			:�}�b�v�p�[�c�N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapObject.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_dwObjectID",		/* �I�u�W�F�N�gID */
	"m_dwAttr",			/* �I�u�W�F�N�g�̑��� */
	"m_nHideY",			/* �B���ォ��̃}�X�� */
	"m_sizeGrp",		/* �摜�T�C�Y */
	"m_bHit",			/* �����蔻�� */
	"m_strName",		/* �I�u�W�F�N�g�� */
	"m_pHit",			/* �����蔻��f�[�^ */
	"nAnimeCount",		/* �I�u�W�F�N�g�A�j���� */
	"anime_byWait",		/* �҂�����(�~�P�O�~���b) */
	"anime_byLevel",	/* �����x */
	"anime_pwGrpID",	/* �摜ID */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoMapObject::CInfoMapObject									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

CInfoMapObject::CInfoMapObject()
{
	m_dwObjectID	= 0;		/* �I�u�W�F�N�gID */
	m_dwAttr		= 0;		/* �I�u�W�F�N�g�̑��� */
	m_nHideY		= 0;		/* �B���ォ��̃}�X�� */
	m_bHit			= FALSE;	/* �����蔻�� */
	m_pHit			= NULL;		/* �����蔻��f�[�^ */
	ZeroMemory (&m_sizeGrp, sizeof (m_sizeGrp));		/* �摜�T�C�Y */

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::~CInfoMapObject								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

CInfoMapObject::~CInfoMapObject()
{
	DeleteAllAnime ();
	SAFE_DELETE_ARRAY (m_pHit);
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::GetElementCount								 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

int CInfoMapObject::GetElementCount(void)
{
	return m_nElementCount;
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::GetElementNo									 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

int CInfoMapObject::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoMapObject::GetDataSize									 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

DWORD CInfoMapObject::GetDataSize(void)
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
/* �֐���	:CInfoMapObject::GetDataSizeNo									 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

DWORD CInfoMapObject::GetDataSizeNo(int nNo)
{
	int nTmp;
	DWORD dwRet;

	dwRet = 0;

	nTmp = m_aInfoAnime.GetSize ();

	switch (nNo) {
	case 0:		dwRet = sizeof (m_dwObjectID);			break;		/* �I�u�W�F�N�gID */
	case 1:		dwRet = sizeof (m_dwAttr);				break;		/* �I�u�W�F�N�g�̑��� */
	case 2:		dwRet = sizeof (m_nHideY);				break;		/* �B���ォ��̃}�X�� */
	case 3:		dwRet = sizeof (m_sizeGrp);				break;		/* �摜�T�C�Y */
	case 4:		dwRet = sizeof (m_bHit);				break;		/* �����蔻�� */
	case 5:		dwRet = m_strName.GetLength () + 1;		break;		/* �I�u�W�F�N�g�� */
	case 6:		dwRet = m_sizeGrp.cx * m_sizeGrp.cy;	break;		/* �����蔻��f�[�^ */
	case 7:		dwRet = sizeof (nTmp);					break;		/* �I�u�W�F�N�g�A�j���� */
	case 8:		dwRet = sizeof (BYTE) * nTmp;			break;		/* �҂�����(�~�P�O�~���b) */
	case 9:		dwRet = sizeof (BYTE) * nTmp;			break;		/* �����x */
	case 10:	/* �摜ID */
		dwRet = sizeof (WORD) * m_sizeGrp.cx * m_sizeGrp.cy * nTmp;
		break;
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::GetName										 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

LPCSTR CInfoMapObject::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::GetWriteData									 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

PBYTE CInfoMapObject::GetWriteData(int nNo, PDWORD pdwSize)
{
	int i, nCount;
	PBYTE pRet, pSrc, pTmp;
	DWORD dwSize;
	PSTMAPOBJECTANIMEINFO pInfo;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;
	nCount		= m_aInfoAnime.GetSize ();

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:		pSrc = (PBYTE)&m_dwObjectID;			break;		/* �I�u�W�F�N�gID */
	case 1:		pSrc = (PBYTE)&m_dwAttr;				break;		/* �I�u�W�F�N�g�̑��� */
	case 2:		pSrc = (PBYTE)&m_nHideY;				break;		/* �B���ォ��̃}�X�� */
	case 3:		pSrc = (PBYTE)&m_sizeGrp;				break;		/* �摜�T�C�Y */
	case 4:		pSrc = (PBYTE)&m_bHit;					break;		/* �����蔻�� */
	case 5:		pSrc = (PBYTE)(LPCSTR)m_strName;		break;		/* �I�u�W�F�N�g�� */
	case 6:		/* �����蔻��f�[�^ */
		if (m_pHit) {
			CopyMemory (pRet, m_pHit, dwSize);
		} else {
			ZeroMemory (pRet, dwSize);
		}
		break;
	case 7:		/* �I�u�W�F�N�g�A�j���� */
		pSrc = (PBYTE)&nCount;
		break;
	case 8:		/* �҂�����(�~�P�O�~���b) */
		pTmp = pRet;
		for (i = 0; i < nCount; i ++) {
			pInfo = GetAnimePtr (i);
			CopyMemoryRenew (pTmp, &pInfo->byWait, sizeof (pInfo->byWait), pTmp);
		}
		break;
	case 9:		/* �����x */
		pTmp = pRet;
		for (i = 0; i < nCount; i ++) {
			pInfo = GetAnimePtr (i);
			CopyMemoryRenew (pTmp, &pInfo->byLevel, sizeof (pInfo->byLevel), pTmp);
		}
		break;
	case 10:	/* �摜ID */
		pTmp = pRet;
		for (i = 0; i < nCount; i ++) {
			pInfo = GetAnimePtr (i);
			dwSize = sizeof (WORD) * m_sizeGrp.cx * m_sizeGrp.cy;
			CopyMemoryRenew (pTmp, (PBYTE)pInfo->pwGrpID, dwSize, pTmp);
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
/* �֐���	:CInfoMapObject::ReadElementData								 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

DWORD CInfoMapObject::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	int i, nCount;
	PBYTE pDst, pSrcTmp;
	DWORD dwSize, dwSizeTmp;
	PSTMAPOBJECTANIMEINFO pInfo;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_dwObjectID;	dwSize = sizeof (m_dwObjectID);		break;		/* �I�u�W�F�N�gID */
	case 1:		pDst = (PBYTE)&m_dwAttr;		dwSize = sizeof (m_dwAttr);			break;		/* �I�u�W�F�N�g�̑��� */
	case 2:		pDst = (PBYTE)&m_nHideY;		dwSize = sizeof (m_nHideY);			break;		/* �B���ォ��̃}�X�� */
	case 3:		pDst = (PBYTE)&m_sizeGrp;		dwSize = sizeof (m_sizeGrp);		break;		/* �摜�T�C�Y */
	case 4:		pDst = (PBYTE)&m_bHit;			dwSize = sizeof (m_bHit);			break;		/* �����蔻�� */
	case 5:		/* �I�u�W�F�N�g�� */
		m_strName = (LPCSTR)pSrc;
		dwSize = m_strName.GetLength () + 1;
		break;
	case 6:		/* �����蔻��f�[�^ */
		SAFE_DELETE_ARRAY (m_pHit);
		dwSize = m_sizeGrp.cx * m_sizeGrp.cy;
		m_pHit = new BYTE[dwSize];
		CopyMemory (m_pHit, pSrc, dwSize);
		break;
	case 7:		/* �I�u�W�F�N�g�A�j���� */
		dwSize = sizeof (int);
		CopyMemory ((PBYTE)&nCount, pSrc, dwSize);
		for (i = 0; i < nCount; i ++) {
			AddAnime ();
		}
		break;
	case 8:		/* �҂�����(�~�P�O�~���b) */
		pSrcTmp = pSrc;
		nCount  = m_aInfoAnime.GetSize ();
		for (i = 0; i < nCount; i ++) {
			pInfo = GetAnimePtr (i);
			CopyMemoryRenew (&pInfo->byWait, pSrcTmp, sizeof (pInfo->byWait), pSrcTmp);
			dwSize += sizeof (pInfo->byWait);
		}
		break;
	case 9:		/* �����x */
		pSrcTmp = pSrc;
		nCount  = m_aInfoAnime.GetSize ();
		for (i = 0; i < nCount; i ++) {
			pInfo = GetAnimePtr (i);
			CopyMemoryRenew (&pInfo->byLevel, pSrcTmp, sizeof (pInfo->byLevel), pSrcTmp);
			dwSize += sizeof (pInfo->byLevel);
		}
		break;
	case 10:	/* �摜ID */
		pSrcTmp = pSrc;
		nCount  = m_aInfoAnime.GetSize ();
		for (i = 0; i < nCount; i ++) {
			pInfo = GetAnimePtr (i);
			dwSizeTmp = sizeof (WORD) * m_sizeGrp.cx * m_sizeGrp.cy;
			CopyMemoryRenew ((PBYTE)pInfo->pwGrpID, pSrcTmp, dwSizeTmp, pSrcTmp);
			dwSize += dwSizeTmp;
		}
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::Copy											 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CInfoMapObject::Copy(CInfoMapObject *pSrc)
{
	PBYTE pTmp;

	pTmp = pSrc->GetSendData ();
	SetSendData (pTmp);
	SAFE_DELETE_ARRAY (pTmp);
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

DWORD CInfoMapObject::GetSendDataSize(void)
{
	/* �ۑ��p�f�[�^�T�C�Y�Ɠ��� */
	return GetDataSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

PBYTE CInfoMapObject::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PSTMAPOBJECTANIMEINFO pInfo;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataTmp = pData;
	nCount   = m_aInfoAnime.GetSize ();

	CopyMemoryRenew (pDataTmp, &m_dwObjectID,	sizeof (m_dwObjectID),	pDataTmp);		/* �I�u�W�F�N�gID */
	CopyMemoryRenew (pDataTmp, &m_dwAttr,		sizeof (m_dwAttr),		pDataTmp);		/* �I�u�W�F�N�g�̑��� */
	CopyMemoryRenew (pDataTmp, &m_nHideY,		sizeof (m_nHideY),		pDataTmp);		/* �B���ォ��̃}�X�� */
	CopyMemoryRenew (pDataTmp, &m_sizeGrp,		sizeof (m_sizeGrp),		pDataTmp);		/* �摜�T�C�Y */
	CopyMemoryRenew (pDataTmp, &m_bHit,			sizeof (m_bHit),		pDataTmp);		/* �����蔻�� */
	CopyMemoryRenew (pDataTmp, &nCount,			sizeof (nCount),		pDataTmp);		/* �I�u�W�F�N�g�A�j���� */
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp);									/* �I�u�W�F�N�g�� */

	/* �����蔻��f�[�^ */
	dwSizeTmp = m_sizeGrp.cx * m_sizeGrp.cy;
	if (m_pHit) {
		CopyMemoryRenew (pDataTmp, m_pHit, dwSizeTmp, pDataTmp);
	} else {
		pDataTmp += dwSizeTmp;
	}

	dwSizeTmp = sizeof (WORD) * m_sizeGrp.cx * m_sizeGrp.cy;
	for (i = 0; i < nCount; i ++) {
		pInfo = GetAnimePtr (i);
		CopyMemoryRenew (pDataTmp, &pInfo->byWait,  sizeof (pInfo->byWait),  pDataTmp);	/* �҂�����(�~�P�O�~���b) */
		CopyMemoryRenew (pDataTmp, &pInfo->byLevel, sizeof (pInfo->byLevel), pDataTmp);	/* �����x */
		CopyMemoryRenew (pDataTmp, (PBYTE)pInfo->pwGrpID, dwSizeTmp, pDataTmp);			/* �摜ID */
	}

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

PBYTE CInfoMapObject::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	PBYTE pDataTmp;
	DWORD dwSizeTmp;
	PSTMAPOBJECTANIMEINFO pInfo;

	DeleteAllAnime ();
	pDataTmp = pSrc;

	CopyMemoryRenew (&m_dwObjectID,	pDataTmp, sizeof (m_dwObjectID),	pDataTmp);		/* �I�u�W�F�N�gID */
	CopyMemoryRenew (&m_dwAttr,		pDataTmp, sizeof (m_dwAttr),		pDataTmp);		/* �I�u�W�F�N�g�̑��� */
	CopyMemoryRenew (&m_nHideY,		pDataTmp, sizeof (m_nHideY),		pDataTmp);		/* �B���ォ��̃}�X�� */
	CopyMemoryRenew (&m_sizeGrp,	pDataTmp, sizeof (m_sizeGrp),		pDataTmp);		/* �摜�T�C�Y */
	CopyMemoryRenew (&m_bHit,		pDataTmp, sizeof (m_bHit),			pDataTmp);		/* �����蔻�� */
	CopyMemoryRenew (&nCount,		pDataTmp, sizeof (nCount),			pDataTmp);		/* �I�u�W�F�N�g�A�j���� */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp);									/* �I�u�W�F�N�g�� */

	/* �����蔻��f�[�^ */
	SAFE_DELETE_ARRAY (m_pHit);
	dwSizeTmp = m_sizeGrp.cx * m_sizeGrp.cy;
	m_pHit = new BYTE[dwSizeTmp];
	CopyMemoryRenew (m_pHit, pDataTmp, dwSizeTmp, pDataTmp);

	for (i = 0; i < nCount; i ++) {
		AddAnime ();
	}
	dwSizeTmp = sizeof (WORD) * m_sizeGrp.cx * m_sizeGrp.cy;
	for (i = 0; i < nCount; i ++) {
		pInfo = GetAnimePtr (i);
		CopyMemoryRenew (&pInfo->byWait,		pDataTmp, sizeof (pInfo->byWait),  pDataTmp);	/* �҂�����(�~�P�O�~���b) */
		CopyMemoryRenew (&pInfo->byLevel,		pDataTmp, sizeof (pInfo->byLevel), pDataTmp);	/* �����x */
		CopyMemoryRenew ((PBYTE)pInfo->pwGrpID, pDataTmp, dwSizeTmp, pDataTmp);					/* �摜ID */
	}

	return pDataTmp;
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::GetAnimeCount									 */
/* ���e		:�A�j���[�V�����R�}�����擾										 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

int CInfoMapObject::GetAnimeCount(void)
{
	return m_aInfoAnime.GetSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::AddAnime										 */
/* ���e		:�A�j���[�V�����R�}��ǉ�										 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CInfoMapObject::AddAnime(void)
{
	PSTMAPOBJECTANIMEINFO pInfo;

	pInfo = new STMAPOBJECTANIMEINFO;
	pInfo->byWait	= 0;		/* �҂�����(�~�P�O�~���b) */
	pInfo->byLevel	= 0;		/* �����x */
	pInfo->pwGrpID = new WORD[m_sizeGrp.cx * m_sizeGrp.cy];
	ZeroMemory ((PBYTE)pInfo->pwGrpID, sizeof (WORD) * m_sizeGrp.cx * m_sizeGrp.cy);

	m_aInfoAnime.Add (pInfo);
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::DeleteAnime									 */
/* ���e		:�A�j���[�V�����R�}���폜										 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CInfoMapObject::DeleteAnime(int nNo)
{
	PSTMAPOBJECTANIMEINFO pInfo;

	pInfo = GetAnimePtr (nNo);
	SAFE_DELETE_ARRAY (pInfo->pwGrpID);
	SAFE_DELETE (pInfo);

	m_aInfoAnime.RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::DeleteAllAnime									 */
/* ���e		:�A�j���[�V�����R�}��S�č폜									 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CInfoMapObject::DeleteAllAnime(void)
{
	int i, nCount;

	nCount = m_aInfoAnime.GetSize ();
	for (i = 0; i < nCount; i ++) {
		DeleteAnime (0);
	}
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::RenewGrpSize									 */
/* ���e		:�摜�T�C�Y���X�V												 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CInfoMapObject::RenewGrpSize(int cx/*-1*/, int cy/*-1*/)
{
	int i, nCount, x, y, xx, yy;
	PBYTE pTmp;
	PWORD pwTmp;
	PSTMAPOBJECTANIMEINFO pInfo;

	if ((m_sizeGrp.cx == cx) && (m_sizeGrp.cy == cy)) {
		return;
	}
	if (cx == -1) {
		cx = m_sizeGrp.cx;
	}
	if (cy == -1) {
		cy = m_sizeGrp.cy;
	}

	xx = m_sizeGrp.cx;
	yy = m_sizeGrp.cy;
	xx = min (xx, cx);
	yy = min (yy, cy);

	/* �����蔻��f�[�^ */
	pTmp = new BYTE[cx * cy];
	ZeroMemory (pTmp, cx * cy);
	if (m_pHit) {
		for (y = 0; y < yy; y ++) {
			for (x = 0; x < xx; x ++) {
				pTmp[cx * y + x] = m_pHit[m_sizeGrp.cx * y + x];
			}
		}
	}
	SAFE_DELETE_ARRAY (m_pHit);
	m_pHit = pTmp;

	/* �A�j�����̐ݒ� */
	nCount = m_aInfoAnime.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = GetAnimePtr (i);

		pwTmp = new WORD[cx * cy];
		ZeroMemory ((PBYTE)pwTmp, sizeof (WORD) * cx * cy);

		for (y = 0; y < yy; y ++) {
			for (x = 0; x < xx; x ++) {
				pwTmp[cx * y + x] = pInfo->pwGrpID[m_sizeGrp.cx * y + x];
			}
		}
		SAFE_DELETE_ARRAY (pInfo->pwGrpID);
		pInfo->pwGrpID = pwTmp;
	}
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::SetGrpID										 */
/* ���e		:�摜ID��ݒ�													 */
/* ���t		:2008/11/02														 */
/* ========================================================================= */

void CInfoMapObject::SetGrpID(int nNo, int x, int y, WORD wGrpID)
{
	PSTMAPOBJECTANIMEINFO pInfo;

	pInfo = GetAnimePtr (nNo);
	if (pInfo == NULL) {
		return;
	}
	if (x >= m_sizeGrp.cx) {
		return;
	}
	if (y >= m_sizeGrp.cy) {
		return;
	}

	pInfo->pwGrpID[y * m_sizeGrp.cx + x] = wGrpID;
}


/* ========================================================================= */
/* �֐���	:CInfoMapObject::GetAnimePtr									 */
/* ���e		:�A�j���[�V�����R�}���擾										 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

PSTMAPOBJECTANIMEINFO CInfoMapObject::GetAnimePtr(int nNo)
{
	int nCount;
	PSTMAPOBJECTANIMEINFO pRet;

	pRet = NULL;

	nCount = m_aInfoAnime.GetSize ();
	if (nNo >= nCount) {
		goto Exit;
	}

	pRet = m_aInfoAnime.GetAt (nNo);
Exit:
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
