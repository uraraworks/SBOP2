/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DataSlot.cpp												 */
/* ���e			:�f�[�^�X���b�g�N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "DataSlot.h"

/* ========================================================================= */
/* �֐���	:CDataSlot::CDataSlot											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

CDataSlot::CDataSlot()
{
	m_dwNewID = 1;
}


/* ========================================================================= */
/* �֐���	:CDataSlot::~CDataSlot											 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

CDataSlot::~CDataSlot()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CDataSlot::Destroy												 */
/* ���e		:�j��															 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

void CDataSlot::Destroy(void)
{
	int i, nCount;

	m_CritData.Enter ();

	/* �m�ۂ���Ă���f�[�^��S�ď��� */

	/* �f�[�^�X���b�g */
	nCount = m_DataSlot.GetSize ();
	for (i = 0; i < nCount; i ++) {
		PDATASLOTINFO pInfo;

		pInfo = &m_DataSlot[i];
		SAFE_DELETE_ARRAY (pInfo->pData);
	}
	m_DataSlot.RemoveAll ();

	m_CritData.Leave ();
}


/* ========================================================================= */
/* �֐���	:CDataSlot::Add													 */
/* ���e		:�f�[�^�ǉ�														 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

DWORD CDataSlot::Add(
	DWORD dwSize,		/* [in] �f�[�^�T�C�Y */
	LPBYTE pSrc)		/* [in] �ǉ�����f�[�^ */
{
	int nNo;
	DWORD dwRet;
	LPBYTE pData;
	DATASLOTINFO Info;

	m_CritData.Enter ();

	pData = new BYTE[dwSize];
	CopyMemory (pData, pSrc, dwSize);

	dwRet = (DWORD)pData;
	Info.dwID	= m_dwNewID;		/* �f�[�^ID */
	Info.dwSize	= dwSize;			/* �f�[�^�T�C�Y */
	Info.pData	= pData;			/* �f�[�^�o�b�t�@ */
	m_DataSlot.Add (Info);

	while (1) {
		m_dwNewID ++;
		if (m_dwNewID == 0) {
			continue;
		}
		nNo = GetID (m_dwNewID);
		if (nNo == -1) {
			break;
		}
	}

	m_CritData.Leave ();

	return Info.dwID;
}


/* ========================================================================= */
/* �֐���	:CDataSlot::AddDelete											 */
/* ���e		:�f�[�^�ǉ����ăo�b�t�@���폜									 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

DWORD CDataSlot::AddDelete(
	DWORD dwSize,		/* [in] �f�[�^�T�C�Y */
	LPBYTE *pSrc)		/* [in] �ǉ�����f�[�^ */
{
	DWORD dwRet;

	dwRet = Add (dwSize, *pSrc);
	SAFE_DELETE_ARRAY (*pSrc);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CDataSlot::GetSize												 */
/* ���e		:�f�[�^�T�C�Y�擾												 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

DWORD CDataSlot::GetSize(
	DWORD dwID)		/* [in] �f�[�^ID */
{
	int nID;
	DWORD dwRet;
	PDATASLOTINFO pInfo;

	dwRet = 0;
	m_CritData.Enter ();

	nID = GetID (dwID);
	/* �f�[�^��������Ȃ������H */
	if (nID < 0) {
		goto Exit;
	}

	pInfo = &m_DataSlot[nID];
	dwRet = pInfo->dwSize;

Exit:
	m_CritData.Leave ();

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CDataSlot::Copy												 */
/* ���e		:�f�[�^�擾														 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

BOOL CDataSlot::Copy(
	DWORD dwID,			/* [in] �f�[�^ID */
	LPBYTE pDst,		/* [out] �i�[��̃|�C���^ */
	BOOL bDelete)		/* [in] �擾��폜���邩�ǂ��� */
{
	int nID;
	BOOL bRet;
	PDATASLOTINFO pInfo;

	bRet = FALSE;

	m_CritData.Enter ();

	nID = GetID (dwID);
	/* �f�[�^��������Ȃ������H */
	if (nID < 0) {
		goto Exit;
	}

	pInfo = &m_DataSlot[nID];
	CopyMemory (pDst, pInfo->pData, pInfo->dwSize);

	/* �폜�w��H */
	if (bDelete) {
		SAFE_DELETE_ARRAY (pInfo->pData);
		m_DataSlot.RemoveAt (nID);
	}

	bRet = TRUE;

Exit:
	m_CritData.Leave ();

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CDataSlot::GetPtr												 */
/* ���e		:�f�[�^�A�h���X�擾												 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

LPBYTE CDataSlot::GetPtr(
	DWORD dwID)					/* [in] �f�[�^ID */
{
	int nID;
	LPBYTE pRet;
	PDATASLOTINFO pInfo;

	pRet = NULL;

	m_CritData.Enter ();

	if (dwID == 0) {
		goto Exit;
	}

	nID = GetID (dwID);
	/* �f�[�^��������Ȃ������H */
	if (nID < 0) {
		goto Exit;
	}

	pInfo = &m_DataSlot[nID];
	pRet = pInfo->pData;

Exit:
	m_CritData.Leave ();

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CDataSlot::Delete												 */
/* ���e		:�f�[�^�폜														 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

void CDataSlot::Delete(
	DWORD dwID)		/* [in] �f�[�^ID */
{
	int nID;
	PDATASLOTINFO pInfo;

	m_CritData.Enter ();

	nID = GetID (dwID);
	/* �f�[�^��������Ȃ������H */
	if (nID < 0) {
		goto Exit;
	}

	pInfo = &m_DataSlot[nID];
	SAFE_DELETE_ARRAY (pInfo->pData);
	m_DataSlot.RemoveAt (nID);

Exit:
	m_CritData.Leave ();
}


/* ========================================================================= */
/* �֐���	:CDataSlot::Lock												 */
/* ���e		:�f�[�^�̃��b�N													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

void CDataSlot::Lock(void)
{
	m_CritDataLock.Enter ();
}


/* ========================================================================= */
/* �֐���	:CDataSlot::Lock												 */
/* ���e		:�f�[�^�̃��b�N����												 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

void CDataSlot::UnLock(void)
{
	m_CritDataLock.Leave ();
}


/* ========================================================================= */
/* �֐���	:CDataSlot::Lock												 */
/* ���e		:�e�[�u��ID�擾													 */
/* ���t		:2008/06/07														 */
/* �߂�l	:�e�[�u��ID(-1:������Ȃ�����)								 */
/* ========================================================================= */

int CDataSlot::GetID(
	DWORD dwID)		/* [in] �f�[�^ID */
{
	int i, nCount, nRet;
	PDATASLOTINFO pInfo;

	nRet = -1;
	nCount = m_DataSlot.GetSize ();

	for (i = 0; i < nCount; i ++) {
		pInfo = &m_DataSlot[i];
		if (pInfo->dwID == dwID) {
			nRet = i;
			break;
		}
	}

	return nRet;
}

/* Copyright(C)URARA-works 2008 */
