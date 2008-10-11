/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoEffect.cpp												 */
/* ���e			:�G�t�F�N�g�N���X �����t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoEffect.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"byAnimeCount",		/* �A�j���[�V�����R�}�� */
	"dwEffectID",		/* �G�t�F�N�gID */
	"dwSoundID",		/* ���ʉ�ID */
	"bLoop",			/* ���[�v���� */
	"m_bLoopSound",		/* ���[�v���Ɍ��ʉ����Đ����� */
	"m_dwGrpIDMain",	/* �摜���C��ID */
	"m_strName",		/* �G�t�F�N�g�� */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoEffect::CInfoEffect										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CInfoEffect::CInfoEffect()
{
	m_bLoop				= FALSE;
	m_bLoopSound		= FALSE;
	m_byAnimeCount		= 0;
	m_dwEffectID		= 0;
	m_dwSoundID			= 0;
	m_dwGrpIDMain		= GRPIDMAIN_EFFECT32;

	m_bAnimeEnd			= FALSE;
	m_byAnimeNo			= 0;
	m_dwLastAnime		= 0;

	m_paAnimeInfo = new ARRAYANIMEINFO;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoEffect::~CInfoEffect										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CInfoEffect::~CInfoEffect()
{
	if (m_paAnimeInfo) {
		DeleteAllAnime ();
	}

	SAFE_DELETE (m_paAnimeInfo);
}


/* ========================================================================= */
/* �֐���	:CInfoEffect::GetElementCount									 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

int CInfoEffect::GetElementCount(void)
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
/* �֐���	:CInfoEffect::GetElementNo										 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

int CInfoEffect::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoEffect::GetDataSize										 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

DWORD CInfoEffect::GetDataSize(void)
{
	int i, nCount;
	DWORD dwRet;

	dwRet = sizeof (m_bLoop)		+
			sizeof (m_bLoopSound)	+
			sizeof (m_byAnimeCount)	+
			sizeof (m_dwEffectID)	+
			sizeof (m_dwSoundID)	+
			sizeof (m_dwGrpIDMain)	+
			(m_strName.GetLength () + 1);

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
/* �֐���	:CInfoEffect::GetDataSizeNo										 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

DWORD CInfoEffect::GetDataSizeNo(int nNo)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAnime pAnime;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_byAnimeCount);		break;	/* �A�j���[�V�����R�}�� */
	case 1:		dwRet = sizeof (m_dwEffectID);			break;	/* �G�t�F�N�gID */
	case 2:		dwRet = sizeof (m_dwSoundID);			break;	/* ���ʉ�ID */
	case 3:		dwRet = sizeof (m_bLoop);				break;	/* ���[�v���� */
	case 4:		dwRet = sizeof (m_bLoopSound);			break;	/* ���[�v���Ɍ��ʉ����Đ����� */
	case 5:		dwRet = sizeof (m_dwGrpIDMain);			break;	/* �摜���C��ID */
	case 6:		dwRet = (m_strName.GetLength () + 1);	break;	/* �G�t�F�N�g�� */
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
/* �֐���	:CInfoEffect::GetName											 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

LPCSTR CInfoEffect::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoEffect::GetWriteData										 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

PBYTE CInfoEffect::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:		pSrc = &m_byAnimeCount;				break;	/* �A�j���[�V�����R�}�� */
	case 1:		pSrc = (PBYTE)&m_dwEffectID;		break;	/* �G�t�F�N�gID */
	case 2:		pSrc = (PBYTE)&m_dwSoundID;			break;	/* ���ʉ�ID */
	case 3:		pSrc = (PBYTE)&m_bLoop;				break;	/* ���[�v���� */
	case 4:		pSrc = (PBYTE)&m_bLoopSound;		break;	/* ���[�v���Ɍ��ʉ����Đ����� */
	case 5:		pSrc = (PBYTE)&m_dwGrpIDMain;		break;	/* �摜���C��ID */
	case 6:		pSrc = (PBYTE)(LPCSTR)m_strName;	break;	/* �G�t�F�N�g�� */
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
/* �֐���	:CInfoEffect::ReadElementData									 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

DWORD CInfoEffect::ReadElementData(
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
	case 0:
		dwSize = sizeof (BYTE);
		CopyMemory (&m_byAnimeCount, pSrc, dwSize);
		for (i = 0; i < m_byAnimeCount; i ++) {
			pAnime = new CInfoAnime;
			m_paAnimeInfo->Add (pAnime);
		}
		break;
	case 1:	pDst = (PBYTE)&m_dwEffectID;	dwSize = sizeof (m_dwEffectID);		break;	/* �G�t�F�N�gID */
	case 2:	pDst = (PBYTE)&m_dwSoundID;		dwSize = sizeof (m_dwSoundID);		break;	/* ���ʉ�ID */
	case 3:	pDst = (PBYTE)&m_bLoop;			dwSize = sizeof (m_bLoop);			break;	/* ���[�v���� */
	case 4:	pDst = (PBYTE)&m_bLoopSound;	dwSize = sizeof (m_bLoopSound);		break;	/* ���[�v���Ɍ��ʉ����Đ����� */
	case 5:	pDst = (PBYTE)&m_dwGrpIDMain;	dwSize = sizeof (m_dwGrpIDMain);	break;	/* �摜���C��ID */
	case 6: 																		  	/* �G�t�F�N�g�� */
		m_strName = (LPCSTR)pSrc;
		dwSize = m_strName.GetLength () + 1;
		break;
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
/* �֐���	:CInfoEffect::Copy												 */
/* ���e		:�R�s�[															 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CInfoEffect::Copy(CInfoEffect *pSrc)
{
	PBYTE pTmp;

	pTmp = pSrc->GetSendData ();
	SetSendData (pTmp);
	SAFE_DELETE_ARRAY (pTmp);
}


/* ========================================================================= */
/* �֐���	:CInfoEffect::GetSendDataSize									 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

DWORD CInfoEffect::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAnime pAnime;

	dwRet = sizeof (m_bLoop)		+
			sizeof (m_bLoopSound)	+
			sizeof (m_dwEffectID)	+
			sizeof (m_dwSoundID)	+
			sizeof (m_dwGrpIDMain)	+
			sizeof (m_byAnimeCount)	+
			(m_strName.GetLength () + 1);

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
/* �֐���	:CInfoEffect::GetSendData										 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

PBYTE CInfoEffect::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp, pDataAnimeTmp;
	DWORD dwSize, dwSizeAnime;
	PCInfoAnime pAnime;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwEffectID,	sizeof (m_dwEffectID),		pDataTmp);	/* �G�t�F�N�gID */
	CopyMemoryRenew (pDataTmp, &m_dwSoundID,	sizeof (m_dwSoundID),		pDataTmp);	/* ���ʉ�ID */
	CopyMemoryRenew (pDataTmp, &m_dwGrpIDMain,	sizeof (m_dwGrpIDMain),		pDataTmp);	/* �摜���C��ID */
	CopyMemoryRenew (pDataTmp, &m_byAnimeCount,	sizeof (m_byAnimeCount),	pDataTmp);	/* �A�j���[�V�����R�}�� */
	CopyMemoryRenew (pDataTmp, &m_bLoop,		sizeof (m_bLoop),			pDataTmp);	/* ���[�v���� */
	CopyMemoryRenew (pDataTmp, &m_bLoopSound,	sizeof (m_bLoopSound),		pDataTmp);	/* ���[�v���Ɍ��ʉ����Đ����� */
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp); 							  	/* �G�t�F�N�g�� */

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
/* �֐���	:CInfoEffect::SetSendData										 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

PBYTE CInfoEffect::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	PBYTE pDataTmp;
	PCInfoAnime pAnime;

	DeleteAllAnime ();
	pDataTmp = pSrc;

	CopyMemoryRenew (&m_dwEffectID,		pDataTmp, sizeof (m_dwEffectID),	pDataTmp);	/* �G�t�F�N�gID */
	CopyMemoryRenew (&m_dwSoundID,		pDataTmp, sizeof (m_dwSoundID),		pDataTmp);	/* ���ʉ�ID */
	CopyMemoryRenew (&m_dwGrpIDMain,	pDataTmp, sizeof (m_dwGrpIDMain),	pDataTmp);	/* �摜���C��ID */
	CopyMemoryRenew (&m_byAnimeCount,	pDataTmp, sizeof (m_byAnimeCount),	pDataTmp);	/* �A�j���[�V�����R�}�� */
	CopyMemoryRenew (&m_bLoop,			pDataTmp, sizeof (m_bLoop),			pDataTmp);	/* ���[�v���� */
	CopyMemoryRenew (&m_bLoopSound,		pDataTmp, sizeof (m_bLoopSound),	pDataTmp);	/* ���[�v���Ɍ��ʉ����Đ����� */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp); 							 	/* �G�t�F�N�g�� */

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
/* �֐���	:CInfoEffect::TimerProc											 */
/* ���e		:���ԏ���														 */
/* �߂�l	:TRUE:�������� FALSE:�������Ă��Ȃ�								 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

BOOL CInfoEffect::TimerProc(DWORD dwTime)
{
	BOOL bRet;
	BYTE byAnimeNoBack;
	DWORD dwTmp;
	PCInfoAnime pAnime;

	bRet = FALSE;
	byAnimeNoBack = m_byAnimeNo;

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	pAnime = m_paAnimeInfo->GetAt (m_byAnimeNo);
	dwTmp  = (DWORD)pAnime->m_byWait * 10;
	if (dwTime - m_dwLastAnime >= dwTmp) {
		if (m_dwLastAnime > 0) {
			m_byAnimeNo ++;
		}
		m_dwLastAnime = dwTime;
	}
	if (m_byAnimeNo >= m_byAnimeCount) {
		if (m_bLoop) {
			m_byAnimeNo = 0;
		} else {
			m_byAnimeNo = m_byAnimeCount - 1;
			m_bAnimeEnd = TRUE;
		}
	}

Exit:
	if (byAnimeNoBack != m_byAnimeNo) {
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoEffect::GetAnimeCount										 */
/* ���e		:�A�j���[�V�����R�}�����擾										 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

int CInfoEffect::GetAnimeCount(void)
{
	return m_paAnimeInfo->GetSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoEffect::AddAnime											 */
/* ���e		:�A�j���[�V�����R�}��ǉ�										 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CInfoEffect::AddAnime(void)
{
	PCInfoAnime pInfo;

	pInfo = new CInfoAnime;
	m_paAnimeInfo->Add (pInfo);
	m_byAnimeCount = m_paAnimeInfo->GetSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoEffect::DeleteAnime										 */
/* ���e		:�A�j���[�V�����R�}���폜										 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CInfoEffect::DeleteAnime(int nNo)
{
	PCInfoAnime pInfo;

	pInfo = m_paAnimeInfo->GetAt (nNo);
	m_paAnimeInfo->RemoveAt (nNo);
	SAFE_DELETE (pInfo);
	m_byAnimeCount = m_paAnimeInfo->GetSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoEffect::DeleteAllAnime									 */
/* ���e		:�A�j���[�V�����R�}��S�č폜									 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CInfoEffect::DeleteAllAnime(void)
{
	int i, nCount;

	nCount = m_paAnimeInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		DeleteAnime (0);
	}
}


/* ========================================================================= */
/* �֐���	:CInfoEffect::GetAnimePtr										 */
/* ���e		:�A�j���[�V�����R�}���擾										 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

PCInfoAnime CInfoEffect::GetAnimePtr(int nNo)
{
	int nCount;
	PCInfoAnime pRet;

	pRet = NULL;

	nCount = m_paAnimeInfo->GetSize ();
	if ((nNo < 0) || (nNo >= nCount)) {
		goto Exit;
	}

	pRet = m_paAnimeInfo->GetAt (nNo);
Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoEffect::GetGrpID											 */
/* ���e		:���݂̉摜ID���擾												 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

WORD CInfoEffect::GetGrpID(void)
{
	WORD wRet;
	PCInfoAnime pAnime;

	wRet = 0;

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	pAnime = m_paAnimeInfo->GetAt (m_byAnimeNo);
	if (pAnime == NULL) {
		goto Exit;
	}

	wRet = pAnime->m_wGrpIDBase;
Exit:
	return wRet;
}


/* ========================================================================= */
/* �֐���	:CInfoEffect::GetLevel											 */
/* ���e		:���݂̓����x���擾												 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

BYTE CInfoEffect::GetLevel(void)
{
	BYTE byRet;
	PCInfoAnime pAnime;

	byRet = 0;

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	pAnime = m_paAnimeInfo->GetAt (m_byAnimeNo);
	if (pAnime == NULL) {
		goto Exit;
	}

	byRet = pAnime->m_byLevel;
Exit:
	return byRet;
}


/* ========================================================================= */
/* �֐���	:CInfoEffect::IsAnimeEnd										 */
/* ���e		:�A�j���[�V�����I��������										 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

BOOL CInfoEffect::IsAnimeEnd(void)
{
	return m_bAnimeEnd;
}

/* Copyright(C)URARA-works 2007 */
