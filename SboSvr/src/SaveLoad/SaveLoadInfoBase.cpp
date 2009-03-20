/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:SaveLoadInfoBase.cpp										 */
/* ���e			:�f�[�^�ۑ��E�ǂݍ��݊��N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/04/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SaveLoadInfoBase.h"


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::CSaveLoadInfoBase							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/04/26														 */
/* ========================================================================= */

CSaveLoadInfoBase::CSaveLoadInfoBase()
{
	m_pData			= NULL;
	m_dwDataSize	= 0;
	m_dwHeaderSize	= 0;
	m_pLibInfoBase	= NULL;

	m_pHeaderInfo = new ARRAYHEADERINFO;
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::~CSaveLoadInfoBase							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2005/04/26														 */
/* ========================================================================= */

CSaveLoadInfoBase::~CSaveLoadInfoBase()
{
	DelAllHeaderInfo ();

	SAFE_DELETE (m_pHeaderInfo);
	SAFE_DELETE_ARRAY (m_pData);
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::WriteData									 */
/* ���e		:�f�[�^�̏�������												 */
/* ���t		:2005/04/26														 */
/* ========================================================================= */

void CSaveLoadInfoBase::WriteData(void)
{
	HANDLE hFile;
	DWORD dwBytes;

	hFile = CreateFile (m_strFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) {
		return;
	}

	dwBytes = 0;
	WriteFile (hFile, m_pData, m_dwHeaderSize + m_dwDataSize + 8, &dwBytes, NULL);
	CloseHandle (hFile);
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::ReadData									 */
/* ���e		:�f�[�^�̓ǂݍ���												 */
/* ���t		:2005/04/26														 */
/* ========================================================================= */

BOOL CSaveLoadInfoBase::ReadData(void)
{
	HANDLE hFile;
	DWORD dwBytes;
	BOOL bRet;

	bRet = FALSE;
	hFile = CreateFile (m_strFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) {
		goto Exit;
	}

	/* �t�@�C���T�C�Y�擾 */
	m_dwDataSize = GetFileSize (hFile, NULL);
	if ((int)m_dwDataSize == -1) {
		goto Exit;
	}

	m_pData = new BYTE[m_dwDataSize];
	/* �t�@�C�����e��S���ǂݍ��� */
	ReadFile (hFile, m_pData, m_dwDataSize, &dwBytes, NULL);

	bRet = TRUE;
Exit:
	if (hFile != INVALID_HANDLE_VALUE) {
		CloseHandle (hFile);
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::Save										 */
/* ���e		:�t�@�C���ɏ�������												 */
/* ���t		:2005/04/26														 */
/* ========================================================================= */

void CSaveLoadInfoBase::Save(PCLibInfoBase pSrc)
{
	PCInfoBase pInfo;

	pInfo = NULL;
	if (pSrc == NULL) {
		goto Exit;
	}

	m_pLibInfoBase = pSrc;
	pInfo = m_pLibInfoBase->GetNew ();

	SetHeaderInfo (pInfo);
	MakeWriteData ();
	WriteData ();

Exit:
	SAFE_DELETE (pInfo);
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::Load										 */
/* ���e		:�t�@�C������ǂݍ���											 */
/* ���t		:2005/04/26														 */
/* ========================================================================= */

void CSaveLoadInfoBase::Load(PCLibInfoBase pDst)
{
	BOOL bResult;
	PCInfoBase pInfo;

	pInfo = NULL;
	if (pDst == NULL) {
		goto Exit;
	}

	m_pLibInfoBase = pDst;
	pInfo = m_pLibInfoBase->GetNew ();

	SetHeaderInfo (pInfo);
	bResult = ReadData ();
	if (bResult == FALSE) {
		goto Exit;
	}
	ReadHeader ();

Exit:
	SAFE_DELETE (pInfo);
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::SetHeaderInfo								 */
/* ���e		:�w�b�_����ݒ�												 */
/* ���t		:2005/05/01														 */
/* ========================================================================= */

void CSaveLoadInfoBase::SetHeaderInfo(PCInfoBase pInfo)
{
	int i;
	LPCSTR pszName;

	for (i = 0; ; i ++) {
		pszName = pInfo->GetName (i);
		if (pszName == NULL) {
			break;
		}
		AddHeaderInfo (pszName);
	}
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::MakeWriteData								 */
/* ���e		:�ۑ��f�[�^�̍쐬												 */
/* ���t		:2005/04/26														 */
/* ========================================================================= */

void CSaveLoadInfoBase::MakeWriteData(void)
{
	int i, j, nCount, nCountElement, nCountData;
	PBYTE pData, pTmp;
	DWORD dwSize, dwColumnCount, dwDataCount;
	PHEADERINFO pInfo, pInfoElement;
	PCInfoBase pInfoData;
	ARRAYINT anSaveNo;

	if (m_pLibInfoBase == NULL) {
		return;
	}

	m_pLibInfoBase->GetSaveNo (anSaveNo);

	/***************************
		�e�v�f�̃T�C�Y��ݒ�
	***************************/

	m_dwDataSize	= 0;
	nCountElement	= m_pHeaderInfo->GetSize ();
	nCountData		= anSaveNo.GetSize ();

	for (i = 0; i < nCountElement; i ++) {
		pInfoElement = m_pHeaderInfo->GetAt (i);
		pInfoElement->dwOffset		= 0;
		pInfoElement->dwDataSize	= 0;

		if (i > 0) {
			pInfoElement->dwOffset = m_dwDataSize;
		}
		pInfoElement->dwOffset += (m_dwHeaderSize + 8);

		for (j = 0; j < nCountData; j ++) {
			pInfoData = m_pLibInfoBase->GetPtr (anSaveNo[j]);
			pInfoElement->dwDataSize += pInfoData->GetDataSizeNo (i);
		}
		m_dwDataSize += pInfoElement->dwDataSize;
	}


	/*************************
		�ۑ��p�f�[�^���m��
	*************************/

	m_pData = new BYTE[m_dwHeaderSize + m_dwDataSize + 8];
	ZeroMemory (m_pData, m_dwHeaderSize + m_dwDataSize + 8);
	pData = m_pData;

	dwColumnCount	= m_pHeaderInfo->GetSize ();
	dwDataCount		= anSaveNo.GetSize ();

	/* �J���������������� */
	CopyMemoryDataDst (pData, (PBYTE)&dwColumnCount, sizeof (dwColumnCount));
	/* �f�[�^������������ */
	CopyMemoryDataDst (pData, (PBYTE)&dwDataCount, sizeof (dwDataCount));

	/* �w�b�_�������������� */
	nCount = (int)dwColumnCount;
	for (i = 0; i < nCount; i ++) {
		pInfo = m_pHeaderInfo->GetAt (i);

		strcpy ((LPSTR)pData, pInfo->szName);
		pData += (strlen (pInfo->szName) + 1);

		CopyMemoryDataDst (pData, (PBYTE)&pInfo->dwOffset, sizeof (pInfo->dwOffset));
	}

	/* �f�[�^�������쐬 */
	for (i = 0; i < nCountElement; i ++) {
		for (j = 0; j < (int)dwDataCount; j ++) {
			dwSize		= 0;
			pInfoData	= m_pLibInfoBase->GetPtr (anSaveNo[j]);
			pTmp		= pInfoData->GetWriteData (i, &dwSize);
			CopyMemoryDataDst (pData, pTmp, dwSize);
			SAFE_DELETE_ARRAY (pTmp);
		}
	}
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::ReadHeader									 */
/* ���e		:�w�b�_���̓ǂݍ���											 */
/* ���t		:2005/04/27														 */
/* ========================================================================= */

void CSaveLoadInfoBase::ReadHeader(void)
{
	int nNo;
	PBYTE pData, pDataBack;
	DWORD i, j, dwCountElement, dwDataCount, dwSize;
	PHEADERINFO pInfoElement, pInfoTmp;
	PCInfoBase pInfo;

	pData			= m_pData;
	pInfoTmp		= new HEADERINFO;
	pInfoElement	= NULL;

	/* �J��������ǂݍ��� */
	CopyMemoryDataSrc ((PBYTE)&dwCountElement, pData, sizeof (dwCountElement));
	/* �f�[�^����ǂݍ��� */
	CopyMemoryDataSrc ((PBYTE)&dwDataCount, pData, sizeof (dwDataCount));
	/* �w�b�_����ǂݍ��� */
	for (i = 0; i < dwCountElement; i ++) {
		ZeroMemory (pInfoTmp, sizeof (HEADERINFO));
		strcpy (pInfoTmp->szName, (LPCSTR)pData);
		pData += (strlen (pInfoTmp->szName) + 1);

		CopyMemoryDataSrc ((PBYTE)&pInfoTmp->dwOffset, pData, sizeof (pInfoTmp->dwOffset));
		pInfoElement = GetHeaderInfo (pInfoTmp->szName);
		if (pInfoElement == NULL) {
			continue;
		}
		pInfoElement->dwOffset = pInfoTmp->dwOffset;
	}

	/* ��̃f�[�^���쐬 */
	for (i = 0; i < dwDataCount; i ++) {
		pInfo = m_pLibInfoBase->GetNew ();
		m_pLibInfoBase->Add (pInfo);
	}

	/* �f�[�^�擪�����̃A�h���X��ۑ� */
	pDataBack = m_pData;


	/***********************
		�f�[�^��ǂݍ���
	***********************/

	dwCountElement = m_pHeaderInfo->GetSize ();
	for (i = 0; i < dwCountElement; i ++) {
		pInfoElement = m_pHeaderInfo->GetAt (i);
		if (pInfoElement->dwOffset == 0) {
			continue;
		}
		pData = pDataBack + pInfoElement->dwOffset;

		for (j = 0; j < dwDataCount; j ++) {
			pInfo	= m_pLibInfoBase->RenewInfo (j);
			nNo		= pInfo->GetElementNo (pInfoElement->szName);
			dwSize	= pInfo->ReadElementData (pData, nNo);
			pData	+= dwSize;
			if ((DWORD)(pData - m_pData) >= m_dwDataSize) {
				break;
			}
		}
		if ((DWORD)(pData - m_pData) >= m_dwDataSize) {
			break;
		}
	}
	m_pLibInfoBase->RenewIDPtr ();

	SAFE_DELETE (pInfoTmp);
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::SetFileName									 */
/* ���e		:�t�@�C������ݒ�												 */
/* ���t		:2005/04/27														 */
/* ========================================================================= */

void CSaveLoadInfoBase::SetFileName(LPCSTR pszName)
{
	char szName[MAX_PATH];
	LPSTR pszTmp;

	/* �t�@�C�����̍쐬 */
	GetModuleFileName (NULL, szName, MAX_PATH);
	pszTmp = strrchr (szName, '\\');
	pszTmp[1] = 0;
	strcat (szName, pszName);

	m_strFileName = szName;
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::AddHeaderInfo								 */
/* ���e		:�w�b�_����ǉ�												 */
/* ���t		:2005/04/29														 */
/* ========================================================================= */

void CSaveLoadInfoBase::AddHeaderInfo(LPCSTR pszName)
{
	PHEADERINFO pInfo;

	pInfo = new HEADERINFO;
	ZeroMemory (pInfo, sizeof (HEADERINFO));

	strcpy (pInfo->szName, pszName);
	m_dwHeaderSize += sizeof (DWORD);
	m_dwHeaderSize += (strlen (pszName) + 1);

	m_pHeaderInfo->Add (pInfo);
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::GetHeaderInfo								 */
/* ���e		:�v�f������w�b�_�����擾										 */
/* ���t		:2005/04/30														 */
/* ========================================================================= */

PHEADERINFO CSaveLoadInfoBase::GetHeaderInfo(LPCSTR pszName)
{
	int i, nCount;
	PHEADERINFO pRet, pInfo;

	pRet	= NULL;
	nCount	= m_pHeaderInfo->GetSize ();

	for (i = 0; i < nCount; i ++) {
		pInfo = m_pHeaderInfo->GetAt (i);
		if (strcmp (pInfo->szName, pszName) == 0) {
			pRet = pInfo;
			break;
		}
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::DelAllHeaderInfo							 */
/* ���e		:�w�b�_�����폜												 */
/* ���t		:2005/05/03														 */
/* ========================================================================= */

void CSaveLoadInfoBase::DelAllHeaderInfo(void)
{
	int i, nCount;

	if (m_pHeaderInfo) {
		nCount = m_pHeaderInfo->GetSize ();
		for (i = 0; i < nCount; i ++) {
			PHEADERINFO pInfo;

			pInfo = m_pHeaderInfo->GetAt (i);
			SAFE_DELETE (pInfo);
		}
		m_pHeaderInfo->RemoveAll ();
		m_dwHeaderSize = 0;
	}
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::CopyMemoryDataDst							 */
/* ���e		:�R�s�[���ăR�s�[��|�C���^��i�߂�								 */
/* ���t		:2005/04/30														 */
/* ========================================================================= */

void CSaveLoadInfoBase::CopyMemoryDataDst(PBYTE &pDst, PBYTE pSrc, DWORD dwSize)
{
	CopyMemory (pDst, pSrc, dwSize);
	pDst += dwSize;
}


/* ========================================================================= */
/* �֐���	:CSaveLoadInfoBase::CopyMemoryDataSrc							 */
/* ���e		:�R�s�[���ăR�s�[���|�C���^��i�߂�								 */
/* ���t		:2005/04/30														 */
/* ========================================================================= */

void CSaveLoadInfoBase::CopyMemoryDataSrc(PBYTE pDst, PBYTE &pSrc, DWORD dwSize)
{
	CopyMemory (pDst, pSrc, dwSize);
	pSrc += dwSize;
}

/* Copyright(C)URARA-works 2005 */
