/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcVERSION.cpp								 */
/* ���e			:�T�[�o�[���C���t���[��(�o�[�W�����n��M����) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SBOVersion.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "GetMD5File.h"
#include "MgrData.h"
#include "MainFrame.h"


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcVERSION									 */
/* ���e		:��M����(�o�[�W�����n)											 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcVERSION(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_VERSION_REQ_VERSIONCHECK:		RecvProcVERSION_REQ_VERSIONCHECK	(pData, dwSessionID);	break;	/* �o�[�W�����`�F�b�N�v�� */
	case SBOCOMMANDID_SUB_VERSION_REQ_FILELISTCHECK:	RecvProcVERSION_REQ_FILELISTCHECK	(pData, dwSessionID);	break;	/* �t�@�C�����X�g�`�F�b�N�v�� */
	case SBOCOMMANDID_SUB_VERSION_REQ_FILELIST:			RecvProcVERSION_REQ_FILELIST		(pData, dwSessionID);	break;	/* �t�@�C�����X�g�v�� */
	case SBOCOMMANDID_SUB_VERSION_REQ_FILE:				RecvProcVERSION_REQ_FILE			(pData, dwSessionID);	break;	/* �t�@�C���v�� */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcVERSION_REQ_VERSIONCHECK					 */
/* ���e		:��M����(�o�[�W�����`�F�b�N�v��)								 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcVERSION_REQ_VERSIONCHECK(PBYTE pData, DWORD dwSessionID)
{
	int nResult;
	CPacketVERSION_REQ_VERSIONCHECK Packet;
	CPacketVERSION_RES_VERSIONCHECK PacketRes;

	Packet.Set (pData);

	nResult = VERSIONCHECKRES_NG_VERSION;
	if (Packet.m_dwVersion == VERSIONVAL) {
		nResult = VERSIONCHECKRES_OK;
	}

	PacketRes.Make (nResult);
	m_pSock->SendTo (dwSessionID, &PacketRes);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcVERSION_REQ_FILELISTCHECK					 */
/* ���e		:��M����(�t�@�C�����X�g�`�F�b�N�v��)							 */
/* ���t		:2007/08/29														 */
/* ========================================================================= */

void CMainFrame::RecvProcVERSION_REQ_FILELISTCHECK(PBYTE pData, DWORD dwSessionID)
{
	int nResult;
	char szTmp[MAX_PATH];
	CPacketVERSION_REQ_FILELISTCHECK Packet;
	CPacketVERSION_RES_FILELISTCHECK PacketVERSION_RES_FILELISTCHECK;
	CGetMD5File GetMD5File;
	CmyString strTmp;

	Packet.Set (pData);

	/* �t�@�C�����X�g�̃n�b�V�����擾 */
	GetModuleFilePath (szTmp, sizeof (szTmp));
	strTmp.Format ("%sSBOHashList.txt", szTmp);
	GetMD5File.Init ();
	GetMD5File.Update (strTmp);

	ZeroMemory (szTmp, sizeof (szTmp));
	GetMD5File.GetStr (szTmp);

	nResult = FILELISTCHECKRES_NG;
	if (Packet.m_strFileListHash == szTmp) {
		nResult = FILELISTCHECKRES_OK;
	}

	PacketVERSION_RES_FILELISTCHECK.Make (nResult);
	m_pSock->SendTo (dwSessionID, &PacketVERSION_RES_FILELISTCHECK);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcVERSION_REQ_FILELIST						 */
/* ���e		:��M����(�t�@�C�����X�g�v��)									 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcVERSION_REQ_FILELIST(PBYTE pData, DWORD dwSessionID)
{
	CPacketVERSION_RES_FILELIST PacketVERSION_RES_FILELIST;

	PacketVERSION_RES_FILELIST.Make (m_pMgrData->GetInfoFileList ());
	m_pSock->SendTo (dwSessionID, &PacketVERSION_RES_FILELIST);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcVERSION_REQ_FILE							 */
/* ���e		:��M����(�t�@�C���v��)											 */
/* ���t		:2008/01/27														 */
/* ========================================================================= */

void CMainFrame::RecvProcVERSION_REQ_FILE(PBYTE pData, DWORD dwSessionID)
{
	BOOL bReuslt;
	char szTmp[MAX_PATH];
	PBYTE pFileData;
	LPCSTR pszTmp;
	HANDLE hFile;
	DWORD dwResult;
	CPacketVERSION_REQ_FILE Packet;
	CPacketVERSION_RES_FILE PacketRes;
	CmyString strFileName;

	pFileData = NULL;
	Packet.Set (pData);

	GetModuleFilePath (szTmp, sizeof (szTmp));
	strFileName.Format ("%sUpdate\\%s", szTmp, (LPCSTR)Packet.m_strFileName);
	pszTmp = strstr ((LPCSTR)strFileName, "..");
	if (pszTmp) {
		return;
	}

	hFile = CreateFile (strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return;
	}

	SetFilePointer (hFile, Packet.m_dwOffset, NULL, FILE_BEGIN);
	pFileData	= ZeroNew (Packet.m_dwReqSize);
	dwResult	= 0;
	bReuslt		= ReadFile (hFile, pFileData, Packet.m_dwReqSize, &dwResult, NULL);
	if (bReuslt == FALSE) {
		goto Exit;
	}
	PacketRes.Make (Packet.m_dwOffset, Packet.m_dwReqSize, Packet.m_strFileName, pFileData);
	m_pSock->SendTo (dwSessionID, &PacketRes);

Exit:
	if (pFileData) {
		delete [] pFileData;
	}
	CloseHandle (hFile);
}

/* Copyright(C)URARA-works 2006 */
