/// @file TextOutput.cpp
/// @brief テキスト出力クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/04/01
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#include <stdio.h>
#ifdef _WIN32
#include <strsafe.h>
#else
#include <sys/time.h>
#endif
#include "../myLib/myString.h"
#include "TextOutput.h"

CTextOutput::CTextOutput()
{
	m_pszFileName	= NULL;
	m_bHeader	= FALSE;
	m_bReturn	= TRUE;

	// クリティカルセクションオブジェクトを初期化
	InitializeCriticalSection(&m_csWrite);
}

CTextOutput::~CTextOutput()
{
	Destroy();

	DeleteCriticalSection(&m_csWrite);
}

BOOL CTextOutput::Create(
	LPCSTR pszFileName,	// [in] 出力するファイル名
	BOOL bHeader,	// [in] ヘッダを書き込む？
	BOOL bReturn)	// [in] 改行コードを書き込む？
{
	BOOL bRet;

	bRet = FALSE;

	if (m_pszFileName) {
		goto Exit;
	}

	m_bHeader = bHeader;
	m_bReturn = bReturn;

	m_pszFileName = new char[strlen(pszFileName) + 1];
	strcpy(m_pszFileName, pszFileName);

	bRet = TRUE;
Exit:
	return bRet;
}

void CTextOutput::Destroy(void)
{
	if (m_pszFileName) {
		delete [] m_pszFileName;
		m_pszFileName = NULL;
	}
}

void CTextOutput::Write(
	LPCSTR format, ...)	// [in] 出力するフォーマット付き文字列
{
	int nLen;
	char szTmp[256];
	va_list ap;

	// 引数の取り出し
	va_start(ap, format);

	nLen = _vsnprintf(szTmp, sizeof(szTmp), format, ap);
	if (nLen <= 0) {
		goto Exit;
	}

	WriteProc(szTmp);

Exit:
	return;
}

void CTextOutput::WriteProc(
	LPCSTR pszText)	// [in] 出力するNULL終端文字列
{
	FILE* pFile;
	char szTmp[128];
#ifdef _WIN32
	SYSTEMTIME stSysTime;
#else
	struct timeval tv;
	struct tm* pTm;
#endif

	// 排他開始
	EnterCriticalSection(&m_csWrite);

	if ((m_pszFileName == NULL) || (pszText == NULL)) {
		goto Exit;
	}

	// ファイルを追記モードで開く（ファイルが存在しない場合は新規作成）
	pFile = fopen(m_pszFileName, "ab");
	if (pFile == NULL) {
		goto Exit;
	}

	// ヘッダを書き込む？
	if (m_bHeader) {
#ifdef _WIN32
		// Windows: GetLocalTime でミリ秒を取得
		GetLocalTime(&stSysTime);
		StringCchPrintfA(szTmp, _countof(szTmp), "%04d/%02d/%02d %02d:%02d:%02d:%03d\t",
				stSysTime.wYear,   stSysTime.wMonth,  stSysTime.wDay,
				stSysTime.wHour,   stSysTime.wMinute, stSysTime.wSecond,
				stSysTime.wMilliseconds);
#else
		// 非Windows: gettimeofday でミリ秒を取得
		gettimeofday(&tv, NULL);
		pTm = localtime(&tv.tv_sec);
		snprintf(szTmp, sizeof(szTmp), "%04d/%02d/%02d %02d:%02d:%02d:%03d\t",
				pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday,
				pTm->tm_hour, pTm->tm_min, pTm->tm_sec,
				(int)(tv.tv_usec / 1000));
#endif
		fwrite(szTmp, 1, strlen(szTmp), pFile);
	}

	// 本文を書き込む
	fwrite(pszText, 1, strlen(pszText), pFile);

	// 改行コードを書き込む？
	if (m_bReturn) {
		fwrite("\r\n", 1, 2, pFile);
	}

	fclose(pFile);

Exit:
	// 排他終了
	LeaveCriticalSection(&m_csWrite);
}

