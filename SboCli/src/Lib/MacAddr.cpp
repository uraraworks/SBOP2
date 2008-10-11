/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	MacAddr.cpp													 */
/* ���e�F		MAC�A�h���X�擾�N���X �����t�@�C��							 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/04/12													 */
/* ========================================================================= */

#include "StdAfx.h"
#include <Iphlpapi.h>
#include "MacAddr.h"

#pragma comment(lib, "Iphlpapi.lib")


/* ========================================================================= */
/* �֐����F	CMacAddr::CMacAddr												 */
/* ���e�F	�R���X�g���N�^													 */
/* ���t�F	2005/04/12														 */
/* ========================================================================= */

CMacAddr::CMacAddr()
{
	m_nCount = GetNICCount ();
}


/* ========================================================================= */
/* �֐����F	CMacAddr::~CMacAddr												 */
/* ���e�F	�f�X�g���N�^													 */
/* ���t�F	2005/04/12														 */
/* ========================================================================= */

CMacAddr::~CMacAddr()
{
}


/* ========================================================================= */
/* �֐����F	CMacAddr::Get													 */
/* ���e�F	MAC�A�h���X���擾												 */
/* ���t�F	2005/04/12														 */
/* ========================================================================= */

BOOL CMacAddr::Get(
	PBYTE pDst,		/* [ou] �擾����MAC�A�h���X(�o�C�i��6�o�C�g) */
	int nNo)		/* [in] �擾������NIC�ԍ�(0���ŏ�) */
{
	int		nCount;
	DWORD	dwResult, dwSize;
	BOOL	bRet;
	PBYTE	pBuff;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADDR_STRING pIPAddress;

	bRet	= FALSE;
	dwSize	= 0;

	/* �K�v�ȃT�C�Y�����߂� */
	GetAdaptersInfo (NULL, &dwSize);

	pBuff			= new BYTE[dwSize];
	pAdapterInfo	= (PIP_ADAPTER_INFO)pBuff;

	/* �����擾 */
	dwResult = GetAdaptersInfo (pAdapterInfo, &dwSize);
	if (dwResult != ERROR_SUCCESS) {
		goto Exit;
	}

	for (nCount = 0; ; nCount ++) {
		if (nCount == nNo) {
			pIPAddress = &pAdapterInfo->IpAddressList;
			if (!((pIPAddress->Context == 0) ||
				 (strcmp (pIPAddress->IpAddress.String, "0.0.0.0") == 0) ||
				 (strcmp (pIPAddress->IpAddress.String, "127.0.0.1") == 0))) {
				CopyMemory (pDst, pAdapterInfo->Address, 6);
				bRet = TRUE;
				break;

			} else {
				nCount --;
			}
		}

		pAdapterInfo = pAdapterInfo->Next;
		if (pAdapterInfo == NULL) {
			break;
		}
	}

Exit:
	if (pBuff) {
		delete [] pBuff;
	}
	return bRet;
}


/* ========================================================================= */
/* �֐����F	CMacAddr::GetMACAddrStr											 */
/* ���e�F	MAC�A�h���X���擾(������)										 */
/* ���t�F	2005/04/12														 */
/* ========================================================================= */

BOOL CMacAddr::GetStr(
	LPSTR pszDst,		/* [ou] �擾����MAC�A�h���X */
	int nNo)			/* [in] �擾������NIC�ԍ�(0���ŏ�) */
{
	BOOL bResult;
	BYTE byMACAddr[6];

	ZeroMemory (byMACAddr, sizeof (byMACAddr));
	bResult = Get (byMACAddr, nNo);
	wsprintf (pszDst, "%02X-%02X-%02X-%02X-%02X-%02X",
			byMACAddr[0], byMACAddr[1], byMACAddr[2], 
			byMACAddr[3], byMACAddr[4], byMACAddr[5]);

	return bResult;
}


/* ========================================================================= */
/* �֐����F	CMacAddr::GetCount												 */
/* ���e�F	NIC�����擾														 */
/* ���t�F	2005/04/12														 */
/* ========================================================================= */

int CMacAddr::GetCount(void)
{
	return m_nCount;
}


/* ========================================================================= */
/* �֐����F	CMacAddr::GetDeviceName											 */
/* ���e�F	�f�o�C�X�����擾												 */
/* ���t�F	2005/04/12														 */
/* ========================================================================= */

BOOL CMacAddr::GetDeviceName(
	LPSTR pszDst,	/* [ou] �擾�����f�o�C�X��(�\���ȃT�C�Y���K�v) */
	int nNo)		/* [in] �擾������NIC�ԍ�(0���ŏ�) */
{
	int		nCount;
	DWORD	dwResult, dwSize;
	BOOL	bRet;
	PBYTE	pBuff;
	LPSTR	pszTmp;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADDR_STRING pIPAddress;

	bRet	= FALSE;
	dwSize	= 0;

	/* �K�v�ȃT�C�Y�����߂� */
	GetAdaptersInfo (NULL, &dwSize);

	pBuff			= new BYTE[dwSize];
	pAdapterInfo	= (PIP_ADAPTER_INFO)pBuff;

	/* �����擾 */
	dwResult = GetAdaptersInfo (pAdapterInfo, &dwSize);
	if (dwResult != ERROR_SUCCESS) {
		goto Exit;
	}

	for (nCount = 0; ; nCount ++) {
		if (nCount == nNo) {
			pIPAddress = &pAdapterInfo->IpAddressList;
			if (!((pIPAddress->Context == 0) ||
				 (strcmp (pIPAddress->IpAddress.String, "0.0.0.0") == 0)) ||
				 (strcmp (pIPAddress->IpAddress.String, "127.0.0.1") == 0)) {
				strcpy (pszDst, pAdapterInfo->Description);
				pszTmp = strstr (pszDst, "-");
				if (pszTmp) {
					*pszTmp = 0;
				}
				bRet = TRUE;
				break;

			} else {
				nCount --;
			}
		}

		pAdapterInfo = pAdapterInfo->Next;
		if (pAdapterInfo == NULL) {
			break;
		}
	}

Exit:
	if (pBuff) {
		delete [] pBuff;
	}
	return bRet;
}


/* ========================================================================= */
/* �֐����F	CMacAddr::GetNICCount											 */
/* ���e�F	NIC�����擾														 */
/* ���t�F	2005/04/12														 */
/* ========================================================================= */

int CMacAddr::GetNICCount(void)
{
	int		nRet;
	DWORD	dwResult, dwSize;
	PBYTE	pBuff;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADDR_STRING pIPAddress;

	nRet	= 0;
	dwSize	= 0;

	/* �K�v�ȃT�C�Y�����߂� */
	GetAdaptersInfo (NULL, &dwSize);

	pBuff			= new BYTE[dwSize];
	pAdapterInfo	= (PIP_ADAPTER_INFO)pBuff;

	/* �����擾 */
	dwResult = GetAdaptersInfo (pAdapterInfo, &dwSize);
	if (dwResult != ERROR_SUCCESS) {
		goto Exit;
	}

	for (nRet = 1; ; nRet ++) {
		pIPAddress = &pAdapterInfo->IpAddressList;
		if (((pIPAddress->Context == 0) ||
			 (strcmp (pIPAddress->IpAddress.String, "0.0.0.0") == 0)) ||
			 (strcmp (pIPAddress->IpAddress.String, "127.0.0.1") == 0)) {
			nRet --;
		}
		pAdapterInfo = pAdapterInfo->Next;
		if (pAdapterInfo == NULL) {
			break;
		}
	}

Exit:
	if (pBuff) {
		delete [] pBuff;
	}
	return nRet;
}

/* Copyright(C)URARA-works 2005 */
