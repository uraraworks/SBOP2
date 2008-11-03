/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapBase.cpp											 */
/* ���e			:�}�b�v�����N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapEvent.h"
#include "LibInfoMapObjectData.h"
#include "InfoMapEvent.h"
#include "InfoMapBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"sizeMap",					/* �}�b�v�T�C�Y */
	"dwMapID",					/* �}�b�vID */
	"m_dwBGMID",				/* BGMID */
	"m_dwWeatherType",			/* �V�C��� */
	"m_byLevel",				/* �Â����x�� */
	"pwMap",					/* �}�b�v */
	"pwMapShadow",				/* �}�b�v�e */
	"strMapName",				/* �}�b�v�� */
	"pLibInfoMapEvent",			/* �}�b�v�C�x���g��� */
	"pLibInfoMapObjectData",	/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^ */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoMapBase::CInfoMapBase										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/10/08														 */
/* ========================================================================= */

CInfoMapBase::CInfoMapBase()
{
	m_sizeMap.cx = m_sizeMap.cy = 0;
	m_dwMapID		= 0;
	m_dwBGMID		= 0;
	m_dwWeatherType	= 0;
	m_pbyMapEvent	= NULL;
	m_pwMap			= NULL;
	m_pwMapShadow	= NULL;
	m_byLevel		= 0;

	m_pLibInfoMapParts		= NULL;
	m_pLibInfoMapEvent		= NULL;
	m_pLibInfoMapObjectData	= NULL;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::~CInfoMapBase									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/10/08														 */
/* ========================================================================= */

CInfoMapBase::~CInfoMapBase()
{
	SAFE_DELETE_ARRAY (m_pbyMapEvent);
	SAFE_DELETE_ARRAY (m_pwMap);
	SAFE_DELETE_ARRAY (m_pwMapShadow);
	SAFE_DELETE (m_pLibInfoMapEvent);
	SAFE_DELETE (m_pLibInfoMapObjectData);
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2007/05/02														 */
/* ========================================================================= */

void CInfoMapBase::Create(CLibInfoMapParts *pLibInfoMapParts)
{
	m_pLibInfoMapParts = pLibInfoMapParts;
	m_pLibInfoMapEvent = new CLibInfoMapEvent;
	m_pLibInfoMapEvent->Create ();
	m_pLibInfoMapObjectData = new CLibInfoMapObjectData;
	m_pLibInfoMapObjectData->Create ();
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::Init												 */
/* ���e		:������															 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

void CInfoMapBase::Init(
	int cx,								/* [in] ���� */
	int cy,								/* [in] �c�� */
	WORD wParts,						/* [in] �h��Ԃ��p�[�cID */
	BOOL bDeleteMapEvent/*=TRUE*/)		/* [in] �}�b�v�C�x���g������������ */
{
	int x, y;

	SAFE_DELETE_ARRAY (m_pwMap);
	SAFE_DELETE_ARRAY (m_pwMapShadow);
	if (bDeleteMapEvent) {
		SAFE_DELETE (m_pLibInfoMapEvent);
		SAFE_DELETE (m_pLibInfoMapObjectData);
	}
	m_sizeMap.cx = cx;
	m_sizeMap.cy = cy;

	if ((cx == 0) && (cy == 0)) {
		return;
	}

	m_pwMap				= new WORD[cx * cy];
	m_pwMapShadow		= new WORD[cx * cy];
	if (bDeleteMapEvent) {
		m_pLibInfoMapEvent	= new CLibInfoMapEvent;
		m_pLibInfoMapEvent->Create ();
		m_pLibInfoMapObjectData = new CLibInfoMapObjectData;
		m_pLibInfoMapObjectData->Create ();
	}

	for (y = 0; y < cy; y ++) {
		for (x = 0; x < cx; x ++) {
			m_pwMap[cx * y + x]			= wParts;
			m_pwMapShadow[cx * y + x]	= 0;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::RenewSize										 */
/* ���e		:�T�C�Y�X�V														 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

void CInfoMapBase::RenewSize(int nDirection, int nSize)
{
	int i, nCount, x, y, nSrcStartX, nSrcEndX, nDstStartX, nDstEndX, nSrcStartY, nSrcEndY, nDstStartY, nDstEndY,
		anPosX[] = {0, 0, 1, 0}, anPosY[] = {1, 0, 0, 0};
	PWORD pwMapBack, pwMapShadow;
	SIZE sizeBack;
	PCInfoMapEventBase pInfoMapEvent;
	PCInfoMapObjectData pInfoMapObjectData;

	if ((nDirection < 0) || (nDirection >= 4)) {
		return;
	}

	sizeBack		= m_sizeMap;
	pwMapBack		= m_pwMap;
	pwMapShadow		= m_pwMapShadow;
	m_pwMap			= NULL;
	m_pwMapShadow	= NULL;

	nSrcStartX	= 0;
	nSrcStartY	= 0;
	nSrcEndX	= sizeBack.cx;
	nSrcEndY	= sizeBack.cy;
	nDstStartX	= 0;
	nDstStartY	= 0;
	nDstEndX	= sizeBack.cx;
	nDstEndY	= sizeBack.cy;

	switch (nDirection) {
	case 0:		/* �� */
		Init (sizeBack.cx, sizeBack.cy + nSize, 0, FALSE);
		if (nSize < 0) {
			nSrcStartY	= nSize * -1;
			nSrcEndY	+= nSize;
		} else {
			nDstStartY	= nSize;
		}
		break;
	case 1:		/* �� */
		Init (sizeBack.cx, sizeBack.cy + nSize, 0, FALSE);
		if (nSize < 0) {
			nSrcEndY = sizeBack.cy + nSize;
		}
		break;
	case 2:		/* �� */
		Init (sizeBack.cx + nSize, sizeBack.cy, 0, FALSE);
		if (nSize < 0) {
			nSrcStartX	= nSize * -1;
			nSrcEndX	+= nSize;
		} else {
			nDstStartX	= nSize;
		}
		break;
	case 3:		/* �E */
		Init (sizeBack.cx + nSize, sizeBack.cy, 0, FALSE);
		if (nSize < 0) {
			nSrcEndX = sizeBack.cx + nSize;
		}
		break;
	}
	/* ���炷�ʒu���l�����Ȃ���V�����}�b�v�փR�s�[ */
	for (y = 0; y < nSrcEndY; y ++) {
		for (x = 0; x < nSrcEndX; x ++) {
			m_pwMap[(m_sizeMap.cx * (nDstStartY + y)) + x + nDstStartX] =
					pwMapBack[(sizeBack.cx * (nSrcStartY + y)) + x + nSrcStartX];
			m_pwMapShadow[(m_sizeMap.cx * (nDstStartY + y)) + x + nDstStartX] =
					pwMapShadow[(sizeBack.cx * (nSrcStartY + y)) + x + nSrcStartX];
		}
	}
	SAFE_DELETE_ARRAY (pwMapBack);
	SAFE_DELETE_ARRAY (pwMapShadow);

	nCount = m_pLibInfoMapEvent->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapEvent = (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr (i);
		pInfoMapEvent->RenewSize (nDirection, nSize);
	}
	nCount = m_pLibInfoMapObjectData->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapObjectData = (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetPtr (i);
		pInfoMapObjectData->RenewSize (nDirection, nSize);
	}

	RenewMapEvent ();
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::FillShadow										 */
/* ���e		:��`��h��Ԃ�												 */
/* ���t		:2007/06/09														 */
/* ========================================================================= */

void CInfoMapBase::FillShadow(int x, int y, int cx, int cy, WORD wShadowID)
{
	int xx, yy;

	for (yy = 0; yy < cy; yy ++) {
		for (xx = 0; xx < cx; xx ++) {
			if (x + xx >= m_sizeMap.cx) {
				continue;
			}
			if (y + yy >= m_sizeMap.cy) {
				continue;
			}
			m_pwMapShadow[cx * (y + yy) + x + xx] = wShadowID;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetElementNo										 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

int CInfoMapBase::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoMapBase::GetDataSize										 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

DWORD CInfoMapBase::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_sizeMap);
	dwRet += sizeof (m_dwMapID);
	dwRet += sizeof (m_dwBGMID);				/* BGMID */
	dwRet += sizeof (m_dwWeatherType);			/* �V�C��� */
	dwRet += sizeof (m_byLevel);				/* �Â����x�� */
	dwRet += ((m_sizeMap.cx * sizeof (WORD)) * m_sizeMap.cy);
	dwRet += ((m_sizeMap.cx * sizeof (WORD)) * m_sizeMap.cy);
	dwRet += (m_strMapName.GetLength () + 1);
	if (m_pLibInfoMapEvent) {
		dwRet += m_pLibInfoMapEvent->GetDataSize ();
	}
	if (m_pLibInfoMapObjectData) {
		dwRet += m_pLibInfoMapObjectData->GetDataSize ();
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetDataSizeNo									 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

DWORD CInfoMapBase::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof (m_sizeMap);								break;
	case 1:	dwRet = sizeof (m_dwMapID);								break;
	case 2:	dwRet = sizeof (m_dwBGMID);								break;	/* BGMID */
	case 3:	dwRet = sizeof (m_dwWeatherType);						break;	/* �V�C��� */
	case 4:	dwRet = sizeof (m_byLevel);								break;	/* �Â����x�� */
	case 5:	dwRet = (m_sizeMap.cx * sizeof (WORD)) * m_sizeMap.cy;	break;
	case 6:	dwRet = (m_sizeMap.cx * sizeof (WORD)) * m_sizeMap.cy;	break;
	case 7:	dwRet = m_strMapName.GetLength () + 1;					break;
	case 8:
		if (m_pLibInfoMapEvent) {
			dwRet = m_pLibInfoMapEvent->GetDataSize ();
		}
		break;
	case 9:
		if (m_pLibInfoMapObjectData) {
			dwRet = m_pLibInfoMapObjectData->GetDataSize ();
		}
		break;
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetName											 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

LPCSTR CInfoMapBase::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetWriteData										 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

PBYTE CInfoMapBase::GetWriteData(int nNo, PDWORD pdwSize)
{
	PBYTE pRet, pSrc;
	DWORD dwSize;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:	pSrc = (PBYTE)&m_sizeMap;			break;
	case 1:	pSrc = (PBYTE)&m_dwMapID;			break;
	case 2:	pSrc = (PBYTE)&m_dwBGMID;			break;		/* BGMID */
	case 3:	pSrc = (PBYTE)&m_dwWeatherType;		break;		/* �V�C��� */
	case 4:	pSrc = (PBYTE)&m_byLevel;			break;		/* �Â����x�� */
	case 5:	pSrc = (PBYTE)m_pwMap;				break;
	case 6:	pSrc = (PBYTE)m_pwMapShadow;		break;
	case 7:	pSrc = (PBYTE)(LPCSTR)m_strMapName;	break;
	case 8:
		if (m_pLibInfoMapEvent) {
			pSrc = m_pLibInfoMapEvent->GetWriteData (pdwSize);
			CopyMemory (pRet, pSrc, dwSize);
			SAFE_DELETE_ARRAY (pSrc);
			goto Exit;
		}
		break;
	case 9:
		if (m_pLibInfoMapObjectData) {
			pSrc = m_pLibInfoMapObjectData->GetWriteData (pdwSize);
			CopyMemory (pRet, pSrc, dwSize);
			SAFE_DELETE_ARRAY (pSrc);
			goto Exit;
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
/* �֐���	:CInfoMapBase::ReadElementData									 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2005/05/01														 */
/* ========================================================================= */

DWORD CInfoMapBase::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = (PBYTE)&m_sizeMap;		dwSize = sizeof (m_sizeMap);		break;
	case 1:	pDst = (PBYTE)&m_dwMapID;		dwSize = sizeof (m_dwMapID);		break;
	case 2:	pDst = (PBYTE)&m_dwBGMID;		dwSize = sizeof (m_dwBGMID);		break;		/* BGMID */
	case 3:	pDst = (PBYTE)&m_dwWeatherType;	dwSize = sizeof (m_dwWeatherType);	break;		/* �V�C��� */
	case 4:	pDst = (PBYTE)&m_byLevel;		dwSize = sizeof (m_byLevel);		break;		/* �Â����x�� */
	case 5:
		SAFE_DELETE_ARRAY (m_pwMap);
		m_pwMap	= new WORD[m_sizeMap.cx * m_sizeMap.cy];
		pDst	= (PBYTE)m_pwMap;
		dwSize	= (m_sizeMap.cx * sizeof (WORD)) * m_sizeMap.cy;
		break;
	case 6:
		SAFE_DELETE_ARRAY (m_pwMapShadow);
		m_pwMapShadow	= new WORD[m_sizeMap.cx * m_sizeMap.cy];
		pDst			= (PBYTE)m_pwMapShadow;
		dwSize			= (m_sizeMap.cx * sizeof (WORD)) * m_sizeMap.cy;
		break;
	case 7:
		m_strMapName = (LPCSTR)pSrc;
		dwSize = m_strMapName.GetLength () + 1;
		break;
	case 8:
		if (m_pLibInfoMapEvent) {
			dwSize = m_pLibInfoMapEvent->ReadElementData (pSrc);
		}
		break;
	case 9:
		if (m_pLibInfoMapObjectData) {
			dwSize = m_pLibInfoMapObjectData->ReadElementData (pSrc);
		}
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::TimerProc										 */
/* ���e		:���ԏ���														 */
/* ���t		:2006/10/08														 */
/* ========================================================================= */

void CInfoMapBase::TimerProc(DWORD dwTime)
{
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::IsMapEvent										 */
/* ���e		:�w����W�Ƀ}�b�v�C�x���g�����邩����							 */
/* ���t		:2008/06/28														 */
/* �߂�l	:TRUE:�}�b�v�C�x���g����										 */
/* ========================================================================= */

BOOL CInfoMapBase::IsMapEvent(int x, int y)
{
	BOOL bRet;

	bRet = FALSE;
	if (m_pbyMapEvent == NULL) {
		goto Exit;
	}
	if (x >= m_sizeMap.cx) {
		goto Exit;
	}
	if (y >= m_sizeMap.cy) {
		goto Exit;
	}
	if ((x < 0) || (y < 0)) {
		goto Exit;
	}
	if (m_pbyMapEvent[y * m_sizeMap.cx + x] == 0) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetMapEventType									 */
/* ���e		:�w����W�̃}�b�v�C�x���g��ʂ��擾								 */
/* ���t		:2008/07/21														 */
/* ========================================================================= */

int CInfoMapBase::GetMapEventType(int x, int y)
{
	int nRet;
	PCInfoMapEventBase pInfoMapEventBase;

	nRet = 0;

	pInfoMapEventBase = GetEvent (x, y);
	if (pInfoMapEventBase == NULL) {
		goto Exit;
	}

	nRet = pInfoMapEventBase->m_nType;
Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetParts											 */
/* ���e		:�w����W�̃p�[�c�ԍ����擾										 */
/* ���t		:2006/10/08														 */
/* ========================================================================= */

WORD CInfoMapBase::GetParts(int x, int y)
{
	WORD wRet;

	wRet = 0;

	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		goto Exit;
	}

	wRet = m_pwMap[m_sizeMap.cx * y + x];
Exit:
	return wRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::SetParts											 */
/* ���e		:�w����W�̃p�[�c�ԍ���ݒ�										 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CInfoMapBase::SetParts(int x, int y, DWORD dwPartsID)
{
	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		return;
	}

	m_pwMap[m_sizeMap.cx * y + x] = (WORD)dwPartsID;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::IsMove											 */
/* ���e		:�i���\������													 */
/* �߂�l	:TRUE:�ʂ�� FALSE:�Ԃ���										 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

BOOL CInfoMapBase::IsMove(int x, int y, int nDirection)
{
	BOOL bRet, bResult;
	DWORD dwPartsID;
	PCInfoMapParts pInfoMapParts;

	bRet = FALSE;

	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		goto Exit;
	}
	if (m_pLibInfoMapParts == NULL) {
		goto Exit;
	}

	dwPartsID		= GetParts (x, y);
	pInfoMapParts	= (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (dwPartsID);
	if (pInfoMapParts == NULL) {
		goto Exit;
	}
	bResult = pInfoMapParts->IsBlock (nDirection);
	if (bResult == FALSE) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::IsMoveOut										 */
/* ���e		:�E�o�\������													 */
/* �߂�l	:TRUE:�� FALSE:�s��												 */
/* ���t		:2007/05/26														 */
/* ========================================================================= */

BOOL CInfoMapBase::IsMoveOut(int x, int y, int nDirection)
{
	int anDirection[] = {1, 0, 3, 2, 6, 7, 4, 5};
	BOOL bRet, bResult;
	DWORD dwPartsID;
	PCInfoMapParts pInfoMapParts;

	bRet = FALSE;

	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		goto Exit;
	}
	if (m_pLibInfoMapParts == NULL) {
		goto Exit;
	}

	dwPartsID		= GetParts (x, y);
	pInfoMapParts	= (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (dwPartsID);
	if (pInfoMapParts == NULL) {
		goto Exit;
	}
	bResult = pInfoMapParts->IsBlock (anDirection[nDirection]);
	if (bResult == FALSE) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::DeleteParts										 */
/* ���e		:�w��p�[�c���폜												 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

void CInfoMapBase::DeleteParts(DWORD dwPartsID)
{
	int x, y;
	WORD wPartsID;

	for (y = 0; y < m_sizeMap.cy; y ++) {
		for (x = 0; x < m_sizeMap.cx; x ++) {
			wPartsID = m_pwMap[m_sizeMap.cx * y + x];
			if (wPartsID == dwPartsID) {
				m_pwMap[m_sizeMap.cx * y + x] = 0;
			}
		}
	}
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetShadow										 */
/* ���e		:�w����W�̉e�ԍ����擾											 */
/* ���t		:2007/06/03														 */
/* ========================================================================= */

WORD CInfoMapBase::GetShadow(int x, int y)
{
	WORD wRet;

	wRet = 0;

	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		goto Exit;
	}

	wRet = m_pwMapShadow[m_sizeMap.cx * y + x];
Exit:
	return wRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::SetShadow										 */
/* ���e		:�w����W�̉e�ԍ���ݒ�											 */
/* ���t		:2007/06/03														 */
/* ========================================================================= */

void CInfoMapBase::SetShadow(int x, int y, DWORD dwShadowID)
{
	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		return;
	}

	m_pwMapShadow[m_sizeMap.cx * y + x] = (WORD)dwShadowID;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::DeleteShadow										 */
/* ���e		:�w��e���폜													 */
/* ���t		:2007/06/03														 */
/* ========================================================================= */

void CInfoMapBase::DeleteShadow(DWORD dwShadowID)
{
	int x, y;
	WORD wShadowID;

	for (y = 0; y < m_sizeMap.cy; y ++) {
		for (x = 0; x < m_sizeMap.cx; x ++) {
			wShadowID = m_pwMapShadow[m_sizeMap.cx * y + x];
			if (wShadowID == dwShadowID) {
				m_pwMapShadow[m_sizeMap.cx * y + x] = 0;
			}
		}
	}
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetMapObject										 */
/* ���e		:�w����W�̃}�b�v�I�u�W�F�N�gID���擾							 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

DWORD CInfoMapBase::GetMapObject(int x, int y)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoMapObjectData pInfo;

	dwRet = 0;

	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		goto Exit;
	}

	nCount = m_pLibInfoMapObjectData->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetPtr (i);
		if (!((pInfo->m_ptPos.x == x) && (pInfo->m_ptPos.y == y))) {
			continue;
		}
		dwRet = pInfo->m_dwObjectID;
		break;
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::IsFlg											 */
/* ���e		:�w�葮��������													 */
/* ���t		:2008/01/04														 */
/* ========================================================================= */

BOOL CInfoMapBase::IsFlg(int x, int y, DWORD dwFlg)
{
	BOOL bRet;
	DWORD dwPartsID;
	PCInfoMapParts pInfoMapParts;

	bRet = FALSE;
	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		goto Exit;
	}
	if (m_pLibInfoMapParts == NULL) {
		goto Exit;
	}
	dwPartsID		= GetParts (x, y);
	pInfoMapParts	= (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (dwPartsID);
	if (pInfoMapParts == NULL) {
		goto Exit;
	}
	if ((pInfoMapParts->m_dwPartsType & dwFlg) == 0) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetSendDataSize									 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

DWORD CInfoMapBase::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_sizeMap);
	dwRet += sizeof (m_dwMapID);
	dwRet += sizeof (m_dwBGMID);				/* BGMID */
	dwRet += sizeof (m_dwWeatherType);			/* �V�C��� */
	dwRet += sizeof (m_byLevel);				/* �Â����x�� */
	dwRet += ((m_sizeMap.cx * sizeof (WORD)) * m_sizeMap.cy);
	dwRet += ((m_sizeMap.cx * sizeof (WORD)) * m_sizeMap.cy);
	dwRet += (m_strMapName.GetLength () + 1);
	if (m_pLibInfoMapEvent) {
		dwRet += m_pLibInfoMapEvent->GetSendDataSize ();
	}
	if (m_pLibInfoMapObjectData) {
		dwRet += m_pLibInfoMapObjectData->GetSendDataSize ();
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetSendData										 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

PBYTE CInfoMapBase::GetSendData(void)
{
	PBYTE pRet, pData, pDataTmp, pDataLibInfoTmp;
	DWORD dwSize, dwSizeLibInfo;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	pDataTmp	= pData;

	CopyMemoryRenew (pDataTmp, &m_sizeMap,			sizeof (m_sizeMap),			pDataTmp);	/* �}�b�v�T�C�Y */
	CopyMemoryRenew (pDataTmp, &m_dwMapID,			sizeof (m_dwMapID),			pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (pDataTmp, &m_dwBGMID,			sizeof (m_dwBGMID),			pDataTmp);	/* BGMID */
	CopyMemoryRenew (pDataTmp, &m_dwWeatherType,	sizeof (m_dwWeatherType),	pDataTmp);	/* �V�C��� */
	CopyMemoryRenew (pDataTmp, &m_byLevel,			sizeof (m_byLevel),			pDataTmp);	/* �Â����x�� */
	strcpyRenew ((LPSTR)pDataTmp, m_strMapName, pDataTmp);					/* �}�b�v�� */
	if (m_pwMap) {
		CopyMemoryRenew (pDataTmp, (PBYTE)m_pwMap, (m_sizeMap.cx * sizeof (WORD)) * m_sizeMap.cy, pDataTmp);	/* �}�b�v */
	}
	if (m_pwMapShadow) {
		CopyMemoryRenew (pDataTmp, (PBYTE)m_pwMapShadow, (m_sizeMap.cx * sizeof (WORD)) * m_sizeMap.cy, pDataTmp);	/* �}�b�v�e */
	}
	if (m_pLibInfoMapEvent) {
		pDataLibInfoTmp	= m_pLibInfoMapEvent->GetSendData ();
		dwSizeLibInfo	= m_pLibInfoMapEvent->GetSendDataSize ();
		CopyMemoryRenew (pDataTmp, pDataLibInfoTmp, dwSizeLibInfo, pDataTmp);	/* �}�b�v�C�x���g��� */
		SAFE_DELETE_ARRAY (pDataLibInfoTmp);
	}
	if (m_pLibInfoMapObjectData) {
		pDataLibInfoTmp	= m_pLibInfoMapObjectData->GetSendData ();
		dwSizeLibInfo	= m_pLibInfoMapObjectData->GetSendDataSize ();
		CopyMemoryRenew (pDataTmp, pDataLibInfoTmp, dwSizeLibInfo, pDataTmp);	/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^ */
		SAFE_DELETE_ARRAY (pDataLibInfoTmp);
	}

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::SetSendData										 */
/* ���e		:���M�f�[�^��ݒ�												 */
/* ���t		:2007/01/04														 */
/* �߂�l	:����������̃A�h���X											 */
/* ========================================================================= */

PBYTE CInfoMapBase::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_sizeMap,		pDataTmp, sizeof (m_sizeMap),		pDataTmp);	/* �}�b�v�T�C�Y */
	CopyMemoryRenew (&m_dwMapID,		pDataTmp, sizeof (m_dwMapID),		pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (&m_dwBGMID,		pDataTmp, sizeof (m_dwBGMID),		pDataTmp);	/* BGMID */
	CopyMemoryRenew (&m_dwWeatherType,	pDataTmp, sizeof (m_dwWeatherType),	pDataTmp);	/* �V�C��� */
	CopyMemoryRenew (&m_byLevel,		pDataTmp, sizeof (m_byLevel),		pDataTmp);	/* �Â����x�� */
	StoreRenew (m_strMapName, (LPCSTR)pDataTmp, pDataTmp);					/* �}�b�v�� */
	Init (m_sizeMap.cx, m_sizeMap.cy, 0);
	if ((m_sizeMap.cx != 0) || (m_sizeMap.cy != 0)) {
		CopyMemoryRenew ((PBYTE)m_pwMap,		pDataTmp, (m_sizeMap.cx * sizeof (WORD)) * m_sizeMap.cy, pDataTmp);	/* �}�b�v */
		CopyMemoryRenew ((PBYTE)m_pwMapShadow,	pDataTmp, (m_sizeMap.cx * sizeof (WORD)) * m_sizeMap.cy, pDataTmp);	/* �}�b�v�e */
	}
	if (m_pLibInfoMapEvent) {
		pDataTmp = m_pLibInfoMapEvent->SetSendData (pDataTmp);
	}
	if (m_pLibInfoMapObjectData) {
		pDataTmp = m_pLibInfoMapObjectData->SetSendData (pDataTmp);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::Copy												 */
/* ���e		:�R�s�[															 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

void CInfoMapBase::Copy(CInfoMapBase *pSrc)
{
	PBYTE pTmp;

	CopyMemory (&m_sizeMap, &pSrc->m_sizeMap, sizeof (m_sizeMap));
	m_dwMapID		= pSrc->m_dwMapID;			/* �}�b�vID */
	m_dwBGMID		= pSrc->m_dwBGMID;			/* BGMID */
	m_dwWeatherType	= pSrc->m_dwWeatherType;	/* �V�C��� */
	m_byLevel		= pSrc->m_byLevel;			/* �Â����x�� */
	m_strMapName	= pSrc->m_strMapName;
	Init (pSrc->m_sizeMap.cx, pSrc->m_sizeMap.cy, 0);
	if ((m_sizeMap.cx != 0) || (m_sizeMap.cy != 0)) {
		CopyMemory ((PBYTE)m_pwMap,			pSrc->m_pwMap,		(m_sizeMap.cx * sizeof (WORD)) * m_sizeMap.cy);
		CopyMemory ((PBYTE)m_pwMapShadow,	pSrc->m_pwMapShadow, (m_sizeMap.cx * sizeof (WORD)) * m_sizeMap.cy);
	}
	pTmp = pSrc->m_pLibInfoMapEvent->GetSendData ();
	m_pLibInfoMapEvent->SetSendData (pTmp);
	SAFE_DELETE_ARRAY (pTmp);
	pTmp = pSrc->m_pLibInfoMapObjectData->GetSendData ();
	m_pLibInfoMapObjectData->SetSendData (pTmp);
	SAFE_DELETE_ARRAY (pTmp);
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::RenewMapEvent									 */
/* ���e		:�}�b�v�C�x���g�X�V												 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

void CInfoMapBase::RenewMapEvent(void)
{
	int i, x, y, nCount, nCountX, nCountY;
	PCInfoMapEventBase pInfo;

	SAFE_DELETE_ARRAY (m_pbyMapEvent);

	m_pbyMapEvent = ZeroNew (m_sizeMap.cx * m_sizeMap.cy);

	if (m_pLibInfoMapEvent) {
		nCount = m_pLibInfoMapEvent->GetCount ();
		for (i = 0; i < nCount; i ++) {
			pInfo = (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr (i);

			/* �͈͂Ŕ���H */
			if (pInfo->m_nHitType == MAPEVENTHITTYPE_AREA) {
				nCountX = pInfo->m_ptPos2.x - pInfo->m_ptPos.x + 1;
				nCountY = pInfo->m_ptPos2.y - pInfo->m_ptPos.y + 1;
				for (y = 0; y < nCountY; y ++) {
					for (x = 0; x < nCountX; x ++) {
						m_pbyMapEvent[(pInfo->m_ptPos.y + y) * m_sizeMap.cx + (pInfo->m_ptPos.x + x)] = 1;
					}
				}

			} else {
				m_pbyMapEvent[pInfo->m_ptPos.y * m_sizeMap.cx + pInfo->m_ptPos.x] = 1;
			}
		}
	}
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetEventCount									 */
/* ���e		:�}�b�v�C�x���g�����擾											 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

int CInfoMapBase::GetEventCount(void)
{
	int nRet;

	nRet = 0;
	if (m_pLibInfoMapEvent == NULL) {
		goto Exit;
	}

	nRet = m_pLibInfoMapEvent->GetCount ();
Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetEvent											 */
/* ���e		:�}�b�v�C�x���g���擾											 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

CInfoMapEventBase *CInfoMapBase::GetEvent(int nNo)
{
	PCInfoMapEventBase pRet;

	pRet = NULL;
	if (m_pLibInfoMapEvent == NULL) {
		goto Exit;
	}

	pRet = (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr (nNo);
Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetEvent											 */
/* ���e		:�}�b�v�C�x���g���擾											 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

CInfoMapEventBase *CInfoMapBase::GetEvent(
	int x,		/* [in] �}�b�v���WX */
	int y)		/* [in] �}�b�v���WY */
{
	int i, nCount;
	PCInfoMapEventBase pRet, pInfo;

	pRet = NULL;
	if (m_pLibInfoMapEvent == NULL) {
		goto Exit;
	}

	nCount = m_pLibInfoMapEvent->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr (i);
		switch (pInfo->m_nHitType) {
		case MAPEVENTHITTYPE_AREA:			/* �͈͂Ŕ��� */
			if ((x >= pInfo->m_ptPos.x) && (x <= pInfo->m_ptPos2.x)) {
				if ((y >= pInfo->m_ptPos.y) && (y <= pInfo->m_ptPos2.y)) {
					pRet = pInfo;
				}
			}
			break;
		default:
			if ((pInfo->m_ptPos.x == x) && (pInfo->m_ptPos.y == y)) {
				pRet = pInfo;
			}
			break;
		}
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetMapObjectDataCount							 */
/* ���e		:�}�b�v�I�u�W�F�N�g�z�u�f�[�^�����擾							 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

int CInfoMapBase::GetMapObjectDataCount(void)
{
	int nRet;

	nRet = 0;
	if (m_pLibInfoMapObjectData == NULL) {
		goto Exit;
	}

	nRet = m_pLibInfoMapObjectData->GetCount ();
Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetObjectData									 */
/* ���e		:�}�b�v�I�u�W�F�N�g�z�u�f�[�^���擾								 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

CInfoMapObjectData *CInfoMapBase::GetObjectData(int nNo)
{
	PCInfoMapObjectData pRet;

	pRet = NULL;
	if (m_pLibInfoMapObjectData == NULL) {
		goto Exit;
	}

	pRet = (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetPtr (nNo);
Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapBase::GetEvent											 */
/* ���e		:�}�b�v�I�u�W�F�N�g�z�u�f�[�^���擾								 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

CInfoMapObjectData *CInfoMapBase::GetObjectData(
	int x,		/* [in] �}�b�v���WX */
	int y)		/* [in] �}�b�v���WY */
{
	int i, nCount;
	PCInfoMapObjectData pRet, pInfo;

	pRet = NULL;
	if (m_pLibInfoMapObjectData == NULL) {
		goto Exit;
	}

	nCount = m_pLibInfoMapObjectData->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetPtr (i);
		if ((pInfo->m_ptPos.x == x) && (pInfo->m_ptPos.y == y)) {
			pRet = pInfo;
		}
	}

Exit:
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
