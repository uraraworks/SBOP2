/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SboUpdate.cpp												 */
/* 内容			:SBOアップデートモジュール 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/03/09													 */
/* ========================================================================= */

#include "stdafx.h"
#include "TextInput.h"


/* ========================================================================= */
/* 関数名	:QuitModule														 */
/* 内容		:指定モジュールを終了させる										 */
/* 日付		:2008/03/09														 */
/* ========================================================================= */

void QuitModule(LPCSTR pszTitle)
{
	int i;
	HWND hWnd;

	for (i = 0; i < 10; i ++) {
		hWnd = FindWindow (NULL, pszTitle);
		if (hWnd == NULL) {
			break;
		}
		PostMessage (hWnd, WM_CLOSE, 0, 0);
		Sleep (500);
	}
}


/* ========================================================================= */
/* 関数名	:ReadFileList													 */
/* 内容		:ファイルリスト読み込み											 */
/* 日付		:2008/03/15														 */
/* ========================================================================= */

void ReadFileList(CmyStringArray &astrDst)
{
	int i, nCount;
	char szFileName[MAX_PATH];
	CTextInput TextInput;
	CmyString strTmp;

	astrDst.clear();

	GetModuleFilePath (szFileName, sizeof (szFileName));
	strcat (szFileName, "FileList.txt");
	TextInput.Create (szFileName);

	nCount = TextInput.GetLineCount ();
	for (i = 0; i < nCount; i ++) {
		strTmp = TextInput.ReadLine ();
		astrDst.push_back (strTmp);
	}
}


/* ========================================================================= */
/* 関数名	:CopyFileList													 */
/* 内容		:ファイルリストのファイルをコピー								 */
/* 日付		:2008/03/15														 */
/* ========================================================================= */

void CopyFileList(CmyStringArray &astrSrc)
{
	int i, nCount;
	BOOL bResult;
	char szPath[MAX_PATH], szFileNameSrc[MAX_PATH], szFileNameDst[MAX_PATH], szTmp[MAX_PATH];
	LPSTR pszFileName;

	GetModuleFilePath (szPath, sizeof (szPath));

	nCount = astrSrc.size();
	for (i = 0; i < nCount; i ++) {
		strcpy (szFileNameSrc, szPath);
		strcpy (szFileNameDst, szPath);
		strcat (szFileNameSrc, astrSrc[i]);
		strcat (szFileNameDst, "..\\");
		strcat (szFileNameDst, astrSrc[i]);

		strcpy (szTmp, szFileNameDst);
		pszFileName = PathFindFileName (szTmp);
		if (pszFileName) {
			*pszFileName = 0;
		}
		AllCreateDirectory (szTmp);

		bResult = CopyFile (szFileNameSrc, szFileNameDst, FALSE);
		if (bResult) {
			/* コピーに成功したのでファイルを削除 */
			DeleteFile (szFileNameSrc);
		}
	}
}


/* ========================================================================= */
/* 関数名	:ExecuteLauncher												 */
/* 内容		:ランチャー起動													 */
/* 日付		:2008/03/15														 */
/* ========================================================================= */

void ExecuteLauncher(void)
{
	BOOL bResult;
	char szPath[MAX_PATH], szTmp[MAX_PATH];
	STARTUPINFO stStartupInfo;
	PROCESS_INFORMATION stProcInfo;
	CmyString strFileName;

	GetModuleFilePath (szPath, sizeof (szPath));
	strFileName.Format ("%s..\\SboLaunch.exe", szPath);

	ZeroMemory (&stStartupInfo, sizeof (stStartupInfo));
	ZeroMemory (&stProcInfo, sizeof (stProcInfo));
	stStartupInfo.cb = sizeof (stStartupInfo);

	strcpy (szTmp, strFileName);
	bResult = CreateProcess (NULL, szTmp, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, szPath,  &stStartupInfo, &stProcInfo);
        if (bResult == FALSE) {
                MessageBox (NULL, _T("SBOランチャーの起動に失敗しました"), _T("エラー"), MB_OK);
		goto Exit;
	}

Exit:
	if (stProcInfo.hProcess) {
		CloseHandle (stProcInfo.hProcess);
	}
	if (stProcInfo.hThread) {
		CloseHandle (stProcInfo.hThread);
	}
}


/* ========================================================================= */
/* 関数名	:WinMain														 */
/* 内容		:プログラム開始													 */
/* 日付		:2008/03/09														 */
/* ========================================================================= */

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char szPath[MAX_PATH];
	CmyStringArray astrFileList;

	GetModuleFilePath (szPath, sizeof (szPath));

	QuitModule ("スクラップブックオンライン ランチャー");
	ReadFileList (astrFileList);
	CopyFileList (astrFileList);
	ExecuteLauncher ();

	return 0;
}

/* Copyright(C)URARA-works 2008 */
