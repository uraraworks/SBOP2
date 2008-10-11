/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:SboUpdate.cpp												 */
/* ���e			:SBO�A�b�v�f�[�g���W���[�� �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/03/09													 */
/* ========================================================================= */

#include "stdafx.h"
#include "TextInput.h"


/* ========================================================================= */
/* �֐���	:QuitModule														 */
/* ���e		:�w�胂�W���[�����I��������										 */
/* ���t		:2008/03/09														 */
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
/* �֐���	:ReadFileList													 */
/* ���e		:�t�@�C�����X�g�ǂݍ���											 */
/* ���t		:2008/03/15														 */
/* ========================================================================= */

void ReadFileList(CmyStringArray &astrDst)
{
	int i, nCount;
	char szFileName[MAX_PATH];
	CTextInput TextInput;
	CmyString strTmp;

	astrDst.RemoveAll ();

	GetModuleFilePath (szFileName, sizeof (szFileName));
	strcat (szFileName, "FileList.txt");
	TextInput.Create (szFileName);

	nCount = TextInput.GetLineCount ();
	for (i = 0; i < nCount; i ++) {
		strTmp = TextInput.ReadLine ();
		astrDst.Add (strTmp);
	}
}


/* ========================================================================= */
/* �֐���	:CopyFileList													 */
/* ���e		:�t�@�C�����X�g�̃t�@�C�����R�s�[								 */
/* ���t		:2008/03/15														 */
/* ========================================================================= */

void CopyFileList(CmyStringArray &astrSrc)
{
	int i, nCount;
	BOOL bResult;
	char szPath[MAX_PATH], szFileNameSrc[MAX_PATH], szFileNameDst[MAX_PATH], szTmp[MAX_PATH];
	LPSTR pszFileName;

	GetModuleFilePath (szPath, sizeof (szPath));

	nCount = astrSrc.GetSize ();
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
			/* �R�s�[�ɐ��������̂Ńt�@�C�����폜 */
			DeleteFile (szFileNameSrc);
		}
	}
}


/* ========================================================================= */
/* �֐���	:ExecuteLauncher												 */
/* ���e		:�����`���[�N��													 */
/* ���t		:2008/03/15														 */
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
		MessageBox (NULL, "SBO�����`���[�̋N���Ɏ��s���܂���", "�G���[", MB_OK);
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
/* �֐���	:WinMain														 */
/* ���e		:�v���O�����J�n													 */
/* ���t		:2008/03/09														 */
/* ========================================================================= */

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char szPath[MAX_PATH];
	CmyStringArray astrFileList;

	GetModuleFilePath (szPath, sizeof (szPath));

	QuitModule ("�X�N���b�v�u�b�N�I�����C�� �����`���[");
	ReadFileList (astrFileList);
	CopyFileList (astrFileList);
	ExecuteLauncher ();

	return 0;
}

/* Copyright(C)URARA-works 2008 */
