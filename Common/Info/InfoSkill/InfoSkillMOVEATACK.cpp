/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ƒtƒ@ƒCƒ‹–¼	:InfoSkillMOVEATACK.cpp										 */
/* “à—e			:ƒXƒLƒ‹î•ñ(ˆÚ“®‚µ‚ÄUŒ‚)ƒNƒ‰ƒX ŽÀ‘•ƒtƒ@ƒCƒ‹				 */
/* ì¬			:”N‚ª‚ç”N’†t‚¤‚ç‚ç(URARA-works)							 */
/* ì¬ŠJŽn“ú	:2009/01/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoSkillMOVEATACK.h"

/* ========================================================================= */
/* ’è”’è‹`																	 */
/* ========================================================================= */

/* ƒwƒbƒ_î•ñ */
static LPCSTR s_aszName[] = {
	"m_dwTartgetType",		/* UŒ‚‘ÎÛ */
	"m_dwHitEffectID",		/* ƒqƒbƒgŽž‚Ì•\Ž¦ƒGƒtƒFƒNƒg */
	"m_dwEffectID",			/* Œü‚«‚É‚æ‚é•\Ž¦ƒGƒtƒFƒNƒg */
	"m_dwPutType",			/* ”­ŽËŽí•Ê */
	"m_dwAliveTime",		/* ‘Ï‹vŽžŠÔ */
	"m_dwWaitTime",			/* ˆÚ“®‘¬“x */
	"m_dwValue1",			/* Œø‰Ê1 */
	"m_dwValue2",			/* Œø‰Ê2 */
	"m_dwDistance",			/* ŽË’ö‹——£ */
	"m_bHitQuit",			/* ƒqƒbƒg‚·‚é‚ÆÁ–Å */
	"m_bDistanceDelete",	/* ŽË’ö‹——£‚Ü‚Ås‚­‚ÆÁ‚¦‚é */
	NULL
};


/* ========================================================================= */
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::CInfoSkillMOVEATACK						 */
/* “à—e		:ƒRƒ“ƒXƒgƒ‰ƒNƒ^													 */
/* “ú•t		:2009/01/08														 */
/* ========================================================================= */

CInfoSkillMOVEATACK::CInfoSkillMOVEATACK()
{
	m_nClassType		= INFOSKILLTYPE_MOVEATACK;
	m_dwHitEffectID		= 0;
	m_dwTartgetType		= 0;
	m_dwPutType			= 0;
	m_dwAliveTime		= 0;
	m_dwWaitTime		= 0;
	m_dwValue1			= 0;
	m_dwValue2			= 0;
	m_dwDistance		= 0;
	m_bHitQuit			= FALSE;
	m_bDistanceDelete	= FALSE;

	ZeroMemory (m_adwEffectID, sizeof (m_adwEffectID));

	for (m_nElementCountDerivation = 0; s_aszName[m_nElementCountDerivation] != NULL; m_nElementCountDerivation ++) {}
}


/* ========================================================================= */
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::~CInfoSkillMOVEATACK						 */
/* “à—e		:ƒfƒXƒgƒ‰ƒNƒ^													 */
/* “ú•t		:2009/01/08														 */
/* ========================================================================= */

CInfoSkillMOVEATACK::~CInfoSkillMOVEATACK()
{
}


/* ========================================================================= */
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::GetElementNoTmp							 */
/* “à—e		:—v‘f”Ô†‚ðŽæ“¾													 */
/* “ú•t		:2009/01/08														 */
/* ========================================================================= */

int CInfoSkillMOVEATACK::GetElementNoTmp(LPCSTR pszName)
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
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::GetDataSizeTmp							 */
/* “à—e		:ƒf[ƒ^ƒTƒCƒY‚ðŽæ“¾												 */
/* “ú•t		:2009/01/08														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::GetDataSizeTmp(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwTartgetType);		/* UŒ‚‘ÎÛ */
	dwRet += sizeof (m_dwHitEffectID);		/* ƒqƒbƒgŽž‚Ì•\Ž¦ƒGƒtƒFƒNƒg */
	dwRet += sizeof (m_adwEffectID);		/* Œü‚«‚É‚æ‚é•\Ž¦ƒGƒtƒFƒNƒg */
	dwRet += sizeof (m_dwPutType);			/* ”­ŽËŽí•Ê */
	dwRet += sizeof (m_dwAliveTime);		/* ‘Ï‹vŽžŠÔ */
	dwRet += sizeof (m_dwWaitTime);			/* ˆÚ“®‘¬“x */
	dwRet += sizeof (m_dwValue1);			/* Œø‰Ê1 */
	dwRet += sizeof (m_dwValue2);			/* Œø‰Ê2 */
	dwRet += sizeof (m_dwDistance);			/* ŽË’ö‹——£ */
	dwRet += sizeof (m_bHitQuit);			/* ƒqƒbƒg‚·‚é‚ÆÁ–Å */
	dwRet += sizeof (m_bDistanceDelete);	/* ŽË’ö‹——£‚Ü‚Ås‚­‚ÆÁ‚¦‚é */

	return dwRet;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::GetDataSizeNoTmp							 */
/* “à—e		:Žw’è—v‘f‚Ìƒf[ƒ^ƒTƒCƒY‚ðŽæ“¾									 */
/* “ú•t		:2009/01/08														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::GetDataSizeNoTmp(int nNo)
{
	DWORD dwRet;

	dwRet = 0;
	switch (nNo) {
	case 0:		dwRet = sizeof (m_dwTartgetType);	break;		/* UŒ‚‘ÎÛ */
	case 1:		dwRet = sizeof (m_dwHitEffectID);	break;		/* ƒqƒbƒgŽž‚Ì•\Ž¦ƒGƒtƒFƒNƒg */
	case 2:		dwRet = sizeof (m_adwEffectID);		break;		/* Œü‚«‚É‚æ‚é•\Ž¦ƒGƒtƒFƒNƒg */
	case 3:		dwRet = sizeof (m_dwPutType);		break;		/* ”­ŽËŽí•Ê */
	case 4:		dwRet = sizeof (m_dwAliveTime);		break;		/* ‘Ï‹vŽžŠÔ */
	case 5:		dwRet = sizeof (m_dwWaitTime);		break;		/* ˆÚ“®‘¬“x */
	case 6:		dwRet = sizeof (m_dwValue1);		break;		/* Œø‰Ê1 */
	case 7:		dwRet = sizeof (m_dwValue2);		break;		/* Œø‰Ê2 */
	case 8:		dwRet = sizeof (m_dwDistance);		break;		/* ŽË’ö‹——£ */
	case 9:		dwRet = sizeof (m_bHitQuit);		break;		/* ƒqƒbƒg‚·‚é‚ÆÁ–Å */
	case 10:	dwRet = sizeof (m_bDistanceDelete);	break;		/* ŽË’ö‹——£‚Ü‚Ås‚­‚ÆÁ‚¦‚é */
	}

	return dwRet;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::GetNameTmp								 */
/* “à—e		:—v‘f–¼‚ðŽæ“¾													 */
/* “ú•t		:2009/01/08														 */
/* ========================================================================= */

LPCSTR CInfoSkillMOVEATACK::GetNameTmp(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::GetWriteDataTmp							 */
/* “à—e		:Žw’è—v‘f‚Ì•Û‘¶—pƒf[ƒ^‚ðŽæ“¾									 */
/* “ú•t		:2009/01/08														 */
/* ========================================================================= */

PBYTE CInfoSkillMOVEATACK::GetWriteDataTmp(int nNo, PDWORD pdwSize)
{
	PBYTE pRet, pSrc;
	DWORD dwSize;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNoTmp (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:		pSrc = (PBYTE)&m_dwTartgetType;		break;		/* UŒ‚‘ÎÛ */
	case 1:		pSrc = (PBYTE)&m_dwHitEffectID;		break;		/* ƒqƒbƒgŽž‚Ì•\Ž¦ƒGƒtƒFƒNƒg */
	case 2:		pSrc = (PBYTE)&m_adwEffectID;		break;		/* Œü‚«‚É‚æ‚é•\Ž¦ƒGƒtƒFƒNƒg */
	case 3:		pSrc = (PBYTE)&m_dwPutType;			break;		/* ”­ŽËŽí•Ê */
	case 4:		pSrc = (PBYTE)&m_dwAliveTime;		break;		/* ‘Ï‹vŽžŠÔ */
	case 5:		pSrc = (PBYTE)&m_dwWaitTime;		break;		/* ˆÚ“®‘¬“x */
	case 6:		pSrc = (PBYTE)&m_dwValue1;			break;		/* Œø‰Ê1 */
	case 7:		pSrc = (PBYTE)&m_dwValue2;			break;		/* Œø‰Ê2 */
	case 8:		pSrc = (PBYTE)&m_dwDistance;		break;		/* ŽË’ö‹——£ */
	case 9:		pSrc = (PBYTE)&m_bHitQuit;			break;		/* ƒqƒbƒg‚·‚é‚ÆÁ–Å */
	case 10:	pSrc = (PBYTE)&m_bDistanceDelete;	break;		/* ŽË’ö‹——£‚Ü‚Ås‚­‚ÆÁ‚¦‚é */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::ReadElementDataTmp						 */
/* “à—e		:Žw’è—v‘fƒf[ƒ^‚ð“Ç‚Ýž‚Ý										 */
/* “ú•t		:2009/01/08														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::ReadElementDataTmp(
	PBYTE pSrc,		/* [in] ƒf[ƒ^‚Ì“Ç‚Ýž‚ÝŒ³ */
	int nNo)		/* [in] —v‘f”Ô† */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_dwTartgetType;		dwSize = sizeof (m_dwTartgetType);		break;		/* UŒ‚‘ÎÛ */
	case 1:		pDst = (PBYTE)&m_dwHitEffectID;		dwSize = sizeof (m_dwHitEffectID);		break;		/* ƒqƒbƒgŽž‚Ì•\Ž¦ƒGƒtƒFƒNƒg */
	case 2:		pDst = (PBYTE)&m_adwEffectID;		dwSize = sizeof (m_adwEffectID);		break;		/* Œü‚«‚É‚æ‚é•\Ž¦ƒGƒtƒFƒNƒg */
	case 3:		pDst = (PBYTE)&m_dwPutType;			dwSize = sizeof (m_dwPutType);			break;		/* ”­ŽËŽí•Ê */
	case 4:		pDst = (PBYTE)&m_dwAliveTime;		dwSize = sizeof (m_dwAliveTime);		break;		/* ‘Ï‹vŽžŠÔ */
	case 5:		pDst = (PBYTE)&m_dwWaitTime;		dwSize = sizeof (m_dwWaitTime);			break;		/* ˆÚ“®‘¬“x */
	case 6:		pDst = (PBYTE)&m_dwValue1;			dwSize = sizeof (m_dwValue1);			break;		/* Œø‰Ê1 */
	case 7:		pDst = (PBYTE)&m_dwValue2;			dwSize = sizeof (m_dwValue2);			break;		/* Œø‰Ê2 */
	case 8:		pDst = (PBYTE)&m_dwDistance;		dwSize = sizeof (m_dwDistance);			break;		/* ŽË’ö‹——£ */
	case 9:		pDst = (PBYTE)&m_bHitQuit;			dwSize = sizeof (m_bHitQuit);			break;		/* ƒqƒbƒg‚·‚é‚ÆÁ–Å */
	case 10:	pDst = (PBYTE)&m_bDistanceDelete;	dwSize = sizeof (m_bDistanceDelete);	break;		/* ŽË’ö‹——£‚Ü‚Ås‚­‚ÆÁ‚¦‚é */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::GetDerivationSize							 */
/* “à—e		:”h¶ƒf[ƒ^ƒTƒCƒY‚ðŽæ“¾											 */
/* “ú•t		:2009/01/10														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::GetDerivationSize(void)
{
	int i, nCount;
	DWORD dwRet;

	dwRet = sizeof (int);		/* —v‘f” */
	nCount = m_nElementCountDerivation;
	for (i = 0; i < nCount; i ++) {
		dwRet += (strlen (GetNameTmp (i)) + 1);		/* —v‘f–¼ */
		dwRet += sizeof (DWORD);					/* ƒf[ƒ^ƒTƒCƒY */
		dwRet += GetDataSizeNoTmp (i);				/* ƒf[ƒ^ */
	}

	return dwRet;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::GetDerivationWriteData					 */
/* “à—e		:”h¶ƒf[ƒ^‚Ì•Û‘¶—pƒf[ƒ^‚ðŽæ“¾									 */
/* “ú•t		:2009/01/10														 */
/* ========================================================================= */

PBYTE CInfoSkillMOVEATACK::GetDerivationWriteData(PDWORD pdwSize)
{
	int i, nCount;
	PBYTE pRet, pRetTmp, pTmp;
	DWORD dwSize, dwTmp;

	dwSize	= GetDerivationSize ();
	pRet	= ZeroNew (dwSize);
	pRetTmp	= pRet;

	nCount = m_nElementCountDerivation;
	CopyMemoryRenew (pRetTmp, &nCount, sizeof (nCount), pRetTmp);	/* —v‘f” */
	for (i = 0; i < nCount; i ++) {
		strcpyRenew ((LPSTR)pRetTmp, GetNameTmp (i), pRetTmp); 		/* —v‘f–¼ */
		dwTmp = GetDataSizeNoTmp (i);
		CopyMemoryRenew (pRetTmp, &dwTmp, sizeof (dwTmp), pRetTmp);	/* ƒf[ƒ^ƒTƒCƒY */
		pTmp = GetWriteDataTmp (i, &dwTmp);
		CopyMemoryRenew (pRetTmp, pTmp, dwTmp, pRetTmp);			/* ƒf[ƒ^ */
		SAFE_DELETE_ARRAY (pTmp);
	}

	return pRet;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::ReadDerivationData						 */
/* “à—e		:”h¶ƒf[ƒ^‚ð“Ç‚Ýž‚Ý											 */
/* “ú•t		:2009/01/10														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::ReadDerivationData(PBYTE pSrc)
{
	int i, nCount, nNoTmp;
	PBYTE pSrcTmp;
	DWORD dwSizeTmp;
	CmyString strTmp;

	pSrcTmp = pSrc;

	CopyMemoryRenew (&nCount, pSrcTmp, sizeof (nCount), pSrcTmp);	/* —v‘f” */
	for (i = 0; i < nCount; i ++) {
		StoreRenew (strTmp, (LPCSTR)pSrcTmp, pSrcTmp);						/* —v‘f–¼ */
		CopyMemoryRenew (&dwSizeTmp, pSrcTmp, sizeof (dwSizeTmp), pSrcTmp);	/* ƒf[ƒ^ƒTƒCƒY */
		nNoTmp = GetElementNoTmp ((LPCSTR)strTmp);
		if (nNoTmp >= 0) {
			dwSizeTmp = ReadElementDataTmp (pSrcTmp, nNoTmp);
		}
		pSrcTmp += dwSizeTmp;
	}

	return (DWORD)(pSrcTmp - pSrc);
}


/* ========================================================================= */
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::GetSendDataSize							 */
/* “à—e		:‘—Mƒf[ƒ^ƒTƒCƒY‚ðŽæ“¾											 */
/* “ú•t		:2009/01/08														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoSkillBase::GetSendDataSize ();
	dwRet += sizeof (m_dwTartgetType);		/* UŒ‚‘ÎÛ */
	dwRet += sizeof (m_dwHitEffectID);		/* ƒqƒbƒgŽž‚Ì•\Ž¦ƒGƒtƒFƒNƒg */
	dwRet += sizeof (m_adwEffectID);		/* Œü‚«‚É‚æ‚é•\Ž¦ƒGƒtƒFƒNƒg */
	dwRet += sizeof (m_dwPutType);			/* ”­ŽËŽí•Ê */
	dwRet += sizeof (m_dwAliveTime);		/* ‘Ï‹vŽžŠÔ */
	dwRet += sizeof (m_dwWaitTime);			/* ˆÚ“®‘¬“x */
	dwRet += sizeof (m_dwValue1);			/* Œø‰Ê1 */
	dwRet += sizeof (m_dwValue2);			/* Œø‰Ê2 */
	dwRet += sizeof (m_dwDistance);			/* ŽË’ö‹——£ */
	dwRet += sizeof (m_bHitQuit);			/* ƒqƒbƒg‚·‚é‚ÆÁ–Å */
	dwRet += sizeof (m_bDistanceDelete);	/* ŽË’ö‹——£‚Ü‚Ås‚­‚ÆÁ‚¦‚é */

	return dwRet;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::GetSendData								 */
/* “à—e		:‘—Mƒf[ƒ^‚ðŽæ“¾												 */
/* “ú•t		:2009/01/08														 */
/* ========================================================================= */

PBYTE CInfoSkillMOVEATACK::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	dwSize	 = GetSendDataSize ();
	pData	 = ZeroNew (dwSize);
	pDataTmp = pData;

	pDataBase  = CInfoSkillBase::GetSendData ();
	dwSizeBase = CInfoSkillBase::GetSendDataSize ();
	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);
	SAFE_DELETE_ARRAY (pDataBase);

	CopyMemoryRenew (pDataTmp, &m_dwTartgetType,	sizeof (m_dwTartgetType),	pDataTmp);	/* UŒ‚‘ÎÛ */
	CopyMemoryRenew (pDataTmp, &m_dwHitEffectID,	sizeof (m_dwHitEffectID),	pDataTmp);	/* ƒqƒbƒgŽž‚Ì•\Ž¦ƒGƒtƒFƒNƒg */
	CopyMemoryRenew (pDataTmp, &m_adwEffectID,		sizeof (m_adwEffectID),		pDataTmp);	/* Œü‚«‚É‚æ‚é•\Ž¦ƒGƒtƒFƒNƒg */
	CopyMemoryRenew (pDataTmp, &m_dwPutType,		sizeof (m_dwPutType),		pDataTmp);	/* ”­ŽËŽí•Ê */
	CopyMemoryRenew (pDataTmp, &m_dwAliveTime,		sizeof (m_dwAliveTime),		pDataTmp);	/* ‘Ï‹vŽžŠÔ */
	CopyMemoryRenew (pDataTmp, &m_dwWaitTime,		sizeof (m_dwWaitTime),		pDataTmp);	/* ˆÚ“®‘¬“x */
	CopyMemoryRenew (pDataTmp, &m_dwValue1,			sizeof (m_dwValue1),		pDataTmp);	/* Œø‰Ê1 */
	CopyMemoryRenew (pDataTmp, &m_dwValue2,			sizeof (m_dwValue2),		pDataTmp);	/* Œø‰Ê2 */
	CopyMemoryRenew (pDataTmp, &m_dwDistance,		sizeof (m_dwDistance),		pDataTmp);	/* ŽË’ö‹——£ */
	CopyMemoryRenew (pDataTmp, &m_bHitQuit,			sizeof (m_bHitQuit),		pDataTmp);	/* ƒqƒbƒg‚·‚é‚ÆÁ–Å */
	CopyMemoryRenew (pDataTmp, &m_bDistanceDelete,	sizeof (m_bDistanceDelete),	pDataTmp);	/* ŽË’ö‹——£‚Ü‚Ås‚­‚ÆÁ‚¦‚é */

	return pData;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::SetSendData								 */
/* “à—e		:‘—Mƒf[ƒ^‚©‚çŽæ‚èž‚Ý											 */
/* “ú•t		:2009/01/08														 */
/* ========================================================================= */

PBYTE CInfoSkillMOVEATACK::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = CInfoSkillBase::SetSendData (pSrc);
	CopyMemoryRenew (&m_dwTartgetType,		pDataTmp, sizeof (m_dwTartgetType),		pDataTmp);	/* UŒ‚‘ÎÛ */
	CopyMemoryRenew (&m_dwHitEffectID,		pDataTmp, sizeof (m_dwHitEffectID),		pDataTmp);	/* ƒqƒbƒgŽž‚Ì•\Ž¦ƒGƒtƒFƒNƒg */
	CopyMemoryRenew (&m_adwEffectID,		pDataTmp, sizeof (m_adwEffectID),		pDataTmp);	/* Œü‚«‚É‚æ‚é•\Ž¦ƒGƒtƒFƒNƒg */
	CopyMemoryRenew (&m_dwPutType,			pDataTmp, sizeof (m_dwPutType),			pDataTmp);	/* ”­ŽËŽí•Ê */
	CopyMemoryRenew (&m_dwAliveTime,		pDataTmp, sizeof (m_dwAliveTime),		pDataTmp);	/* ‘Ï‹vŽžŠÔ */
	CopyMemoryRenew (&m_dwWaitTime,			pDataTmp, sizeof (m_dwWaitTime),		pDataTmp);	/* ˆÚ“®‘¬“x */
	CopyMemoryRenew (&m_dwValue1,			pDataTmp, sizeof (m_dwValue1),			pDataTmp);	/* Œø‰Ê1 */
	CopyMemoryRenew (&m_dwValue2,			pDataTmp, sizeof (m_dwValue2),			pDataTmp);	/* Œø‰Ê2 */
	CopyMemoryRenew (&m_dwDistance,			pDataTmp, sizeof (m_dwDistance),		pDataTmp);	/* ŽË’ö‹——£ */
	CopyMemoryRenew (&m_bHitQuit,			pDataTmp, sizeof (m_bHitQuit),			pDataTmp);	/* ƒqƒbƒg‚·‚é‚ÆÁ–Å */
	CopyMemoryRenew (&m_bDistanceDelete,	pDataTmp, sizeof (m_bDistanceDelete),	pDataTmp);	/* ŽË’ö‹——£‚Ü‚Ås‚­‚ÆÁ‚¦‚é */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* ŠÖ”–¼	:CInfoSkillMOVEATACK::Copy										 */
/* “à—e		:ƒRƒs[															 */
/* “ú•t		:2009/01/08														 */
/* ========================================================================= */

void CInfoSkillMOVEATACK::Copy(CInfoSkillBase *pSrc)
{
	PCInfoSkillMOVEATACK pSrcTmp;

	if (pSrc == NULL) {
		return;
	}
	pSrcTmp = (PCInfoSkillMOVEATACK)pSrc;

	CInfoSkillBase::Copy (pSrc);
	if (pSrc->m_nClassType != m_nClassType) {
		return;
	}

	m_dwHitEffectID		= pSrcTmp->m_dwHitEffectID;		/* ƒqƒbƒgŽž‚Ì•\Ž¦ƒGƒtƒFƒNƒg */
	m_dwTartgetType		= pSrcTmp->m_dwTartgetType;		/* UŒ‚‘ÎÛ */
	m_dwPutType			= pSrcTmp->m_dwPutType;			/* ”­ŽËŽí•Ê */
	m_dwAliveTime		= pSrcTmp->m_dwAliveTime;		/* ‘Ï‹vŽžŠÔ */
	m_dwWaitTime		= pSrcTmp->m_dwWaitTime;		/* ˆÚ“®‘¬“x */
	m_dwValue1			= pSrcTmp->m_dwValue1;			/* Œø‰Ê1 */
	m_dwValue2			= pSrcTmp->m_dwValue2;			/* Œø‰Ê2 */
	m_dwDistance		= pSrcTmp->m_dwDistance;		/* ŽË’ö‹——£ */
	m_bHitQuit			= pSrcTmp->m_bHitQuit;			/* ƒqƒbƒg‚·‚é‚ÆÁ–Å */
	m_bDistanceDelete	= pSrcTmp->m_bDistanceDelete;	/* ŽË’ö‹——£‚Ü‚Ås‚­‚ÆÁ‚¦‚é */

	/* Œü‚«‚É‚æ‚é•\Ž¦ƒGƒtƒFƒNƒg */
	CopyMemory (m_adwEffectID, pSrcTmp->m_adwEffectID, sizeof (m_adwEffectID));
}

/* Copyright(C)URARA-works 2009 */
