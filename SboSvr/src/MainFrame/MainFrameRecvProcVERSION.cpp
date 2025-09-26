/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MainFrameRecvProcVERSION.cpp								 */
/* 内容			:サーバーメインフレーム(バージョン系受信処理) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
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
/* 関数名	:CMainFrame::RecvProcVERSION									 */
/* 内容		:受信処理(バージョン系)											 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcVERSION(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_VERSION_REQ_VERSIONCHECK:		RecvProcVERSION_REQ_VERSIONCHECK	(pData, dwSessionID);	break;	/* バージョンチェック要求 */
	case SBOCOMMANDID_SUB_VERSION_REQ_FILELISTCHECK:	RecvProcVERSION_REQ_FILELISTCHECK	(pData, dwSessionID);	break;	/* ファイルリストチェック要求 */
	case SBOCOMMANDID_SUB_VERSION_REQ_FILELIST:			RecvProcVERSION_REQ_FILELIST		(pData, dwSessionID);	break;	/* ファイルリスト要求 */
	case SBOCOMMANDID_SUB_VERSION_REQ_FILE:				RecvProcVERSION_REQ_FILE			(pData, dwSessionID);	break;	/* ファイル要求 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcVERSION_REQ_VERSIONCHECK					 */
/* 内容		:受信処理(バージョンチェック要求)								 */
/* 日付		:2006/11/05														 */
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
/* 関数名	:CMainFrame::RecvProcVERSION_REQ_FILELISTCHECK					 */
/* 内容		:受信処理(ファイルリストチェック要求)							 */
/* 日付		:2007/08/29														 */
/* ========================================================================= */

void CMainFrame::RecvProcVERSION_REQ_FILELISTCHECK(PBYTE pData, DWORD dwSessionID)
{
        int nResult;
        TCHAR szPath[MAX_PATH];
        char szHash[33];
	CPacketVERSION_REQ_FILELISTCHECK Packet;
	CPacketVERSION_RES_FILELISTCHECK PacketVERSION_RES_FILELISTCHECK;
	CGetMD5File GetMD5File;
	CmyString strTmp;

	Packet.Set (pData);

	/* ファイルリストのハッシュを取得 */
        GetModuleFilePath (szPath, _countof (szPath));
        CString strPath (szPath);
        strTmp.Format(_T("%sSBOHashList.txt"), (LPCTSTR)strPath);
        GetMD5File.Init ();
        GetMD5File.Update (strTmp);

        ZeroMemory (szHash, sizeof (szHash));
        GetMD5File.GetStr (szHash);

        nResult = FILELISTCHECKRES_NG;
        if (Packet.m_strFileListHash == szHash) {
                nResult = FILELISTCHECKRES_OK;
        }

	PacketVERSION_RES_FILELISTCHECK.Make (nResult);
	m_pSock->SendTo (dwSessionID, &PacketVERSION_RES_FILELISTCHECK);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcVERSION_REQ_FILELIST						 */
/* 内容		:受信処理(ファイルリスト要求)									 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcVERSION_REQ_FILELIST(PBYTE pData, DWORD dwSessionID)
{
	CPacketVERSION_RES_FILELIST PacketVERSION_RES_FILELIST;

	PacketVERSION_RES_FILELIST.Make (m_pMgrData->GetInfoFileList ());
	m_pSock->SendTo (dwSessionID, &PacketVERSION_RES_FILELIST);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcVERSION_REQ_FILE							 */
/* 内容		:受信処理(ファイル要求)											 */
/* 日付		:2008/01/27														 */
/* ========================================================================= */

void CMainFrame::RecvProcVERSION_REQ_FILE(PBYTE pData, DWORD dwSessionID)
{
	BOOL bReuslt;
        TCHAR szPath[MAX_PATH];
	PBYTE pFileData;
	LPCSTR pszTmp;
	HANDLE hFile;
	DWORD dwResult;
	CPacketVERSION_REQ_FILE Packet;
	CPacketVERSION_RES_FILE PacketRes;
	CmyString strFileName;

	pFileData = NULL;
	Packet.Set (pData);

        GetModuleFilePath (szPath, _countof (szPath));
        CString strBasePath (szPath);
        strFileName.Format(_T("%sUpdate\\%s"), (LPCTSTR)strBasePath, (LPCTSTR)Packet.m_strFileName);
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
