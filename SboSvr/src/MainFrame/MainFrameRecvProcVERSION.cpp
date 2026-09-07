/// @file MainFrameRecvProcVERSION.cpp
/// @brief サーバーメインフレーム(バージョン系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/05
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"

// fopen 等の標準 C ファイル IO 用
#include <cstdio>

#include "SBOVersion.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "GetMD5File.h"
#include "MgrData.h"
#include "MainFrame.h"
#include "Platform/SvrPlatform.h"

void CMainFrame::RecvProcVERSION(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_VERSION_REQ_VERSIONCHECK:	RecvProcVERSION_REQ_VERSIONCHECK(pData, dwSessionID);	break;	// バージョンチェック要求
	case SBOCOMMANDID_SUB_VERSION_REQ_FILELISTCHECK:	RecvProcVERSION_REQ_FILELISTCHECK(pData, dwSessionID);	break;	// ファイルリストチェック要求
	case SBOCOMMANDID_SUB_VERSION_REQ_FILELIST:	RecvProcVERSION_REQ_FILELIST(pData, dwSessionID);	break;	// ファイルリスト要求
	case SBOCOMMANDID_SUB_VERSION_REQ_FILE:	RecvProcVERSION_REQ_FILE(pData, dwSessionID);	break;	// ファイル要求
	}
}

void CMainFrame::RecvProcVERSION_REQ_VERSIONCHECK(PBYTE pData, DWORD dwSessionID)
{
	int nResult;
	CPacketVERSION_REQ_VERSIONCHECK Packet;
	CPacketVERSION_RES_VERSIONCHECK PacketRes;

	Packet.Set(pData);

	nResult = VERSIONCHECKRES_NG_VERSION;
	if (Packet.m_dwVersion == VERSIONVAL) {
		nResult = VERSIONCHECKRES_OK;
	}

	PacketRes.Make(nResult);
	m_pSock->SendTo(dwSessionID, &PacketRes);
}

void CMainFrame::RecvProcVERSION_REQ_FILELISTCHECK(PBYTE pData, DWORD dwSessionID)
{
        int nResult;
        char szHash[33];
	CPacketVERSION_REQ_FILELISTCHECK Packet;
	CPacketVERSION_RES_FILELISTCHECK PacketVERSION_RES_FILELISTCHECK;
	CGetMD5File GetMD5File;
	CmyString strTmp;

	Packet.Set(pData);

	// ファイルリストのハッシュを取得
	// 実行ファイルのディレクトリ取得は SboPlatform::GetExeDirectory() に集約済み
	// (末尾に区切り文字を含む点は GetModuleFilePath と同じ)
        CString strPath = AnsiToTString(SboPlatform::GetExeDirectory().c_str());
        strTmp.Format(_T("%sSBOHashList.txt"), (LPCTSTR)strPath);
        GetMD5File.Init();
        GetMD5File.Update(strTmp);

        ZeroMemory(szHash, sizeof (szHash));
        GetMD5File.GetStr(szHash);

        nResult = FILELISTCHECKRES_NG;
        if (Packet.m_strFileListHash == szHash) {
                nResult = FILELISTCHECKRES_OK;
        }

	PacketVERSION_RES_FILELISTCHECK.Make(nResult);
	m_pSock->SendTo(dwSessionID, &PacketVERSION_RES_FILELISTCHECK);
}

void CMainFrame::RecvProcVERSION_REQ_FILELIST(PBYTE pData, DWORD dwSessionID)
{
	CPacketVERSION_RES_FILELIST PacketVERSION_RES_FILELIST;

	PacketVERSION_RES_FILELIST.Make(m_pMgrData->GetInfoFileList());
	m_pSock->SendTo(dwSessionID, &PacketVERSION_RES_FILELIST);
}

void CMainFrame::RecvProcVERSION_REQ_FILE(PBYTE pData, DWORD dwSessionID)
{
	BOOL bReuslt;
	PBYTE pFileData;
	LPCSTR pszTmp;
	FILE *pFile;
	DWORD dwResult;
	CPacketVERSION_REQ_FILE Packet;
	CPacketVERSION_RES_FILE PacketRes;
	CmyString strFileName;

	pFileData = NULL;
	Packet.Set(pData);

        // 実行ファイルのディレクトリ取得は SboPlatform::GetExeDirectory() に集約済み
        CString strBasePath = AnsiToTString(SboPlatform::GetExeDirectory().c_str());
        strFileName.Format(_T("%sUpdate\\%s"), (LPCTSTR)strBasePath, (LPCTSTR)Packet.m_strFileName);
	pszTmp = strstr((LPCSTR)strFileName, "..");
	if (pszTmp) {
		return;
	}

	pFile = fopen(strFileName, "rb");
	if (pFile == NULL) {
		return;
	}

	fseek(pFile, Packet.m_dwOffset, SEEK_SET);
	pFileData	= ZeroNew(Packet.m_dwReqSize);
	dwResult	= (DWORD)fread(pFileData, 1, Packet.m_dwReqSize, pFile);
	bReuslt	= (ferror(pFile) == 0);
	if (bReuslt == FALSE) {
		goto Exit;
	}
	PacketRes.Make(Packet.m_dwOffset, Packet.m_dwReqSize, Packet.m_strFileName, pFileData);
	m_pSock->SendTo(dwSessionID, &PacketRes);

Exit:
	if (pFileData) {
		delete [] pFileData;
	}
	fclose(pFile);
}
