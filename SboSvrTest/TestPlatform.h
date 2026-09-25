/// @file TestPlatform.h
/// @brief テスト用のプラットフォーム差分吸収（一時ファイル・スリープ）
/// @copyright Copyright(C)URARA-works
///
/// テストが直接使っていた Win32 API（GetTempFileNameA / DeleteFileA /
/// CreateFileW / Sleep 等）を、Windows と POSIX(Linux) の両方で動く形に
/// まとめる。Windows では従来と同じ API を呼ぶ。

#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#ifndef _WIN32
#include <unistd.h>
#endif

namespace SboTest
{
	/// 一時ディレクトリを返す（末尾は区切り文字）
	inline std::string GetTempDir(void)
	{
#ifdef _WIN32
		char szDir[MAX_PATH];
		GetTempPathA(MAX_PATH, szDir);
		return std::string(szDir);
#else
		const char *pszDir = getenv("TMPDIR");
		std::string strDir = ((pszDir != NULL) && (pszDir[0] != '\0')) ? pszDir : "/tmp";
		if (strDir[strDir.size() - 1] != '/') {
			strDir.push_back('/');
		}
		return strDir;
#endif
	}

	/// 一時ディレクトリを返す（wchar_t 版。末尾は区切り文字）
	inline std::wstring GetTempDirW(void)
	{
#ifdef _WIN32
		wchar_t szDir[MAX_PATH];
		GetTempPathW(MAX_PATH, szDir);
		return std::wstring(szDir);
#else
		std::string strDir = GetTempDir();
		return std::wstring(strDir.begin(), strDir.end());
#endif
	}

	/// 空の一時ファイルを作り、そのパスを返す（GetTempFileNameA 相当）
	inline std::string CreateTempFile(const char *pszPrefix)
	{
#ifdef _WIN32
		char szPath[MAX_PATH];
		GetTempFileNameA(GetTempDir().c_str(), pszPrefix, 0, szPath);
		return std::string(szPath);
#else
		std::string strPath = GetTempDir() + pszPrefix + "XXXXXX";
		std::string strBuf(strPath);
		int nFd = mkstemp(&strBuf[0]);
		if (nFd >= 0) {
			close(nFd);
		}
		return strBuf;
#endif
	}

	/// ファイルを消す（DeleteFileA 相当）
	inline void DeleteFileUtf8(const std::string &strPath)
	{
#ifdef _WIN32
		DeleteFileA(strPath.c_str());
#else
		unlink(strPath.c_str());
#endif
	}

	/// wchar_t のパスを実行環境のファイル名に直す
	inline std::string NativePathFromW(const std::wstring &strPath)
	{
#ifdef _WIN32
		(void)strPath;
		return std::string();	// Windows では wchar_t のまま扱うので使わない
#else
		return WstringToUtf8(strPath.c_str(), strPath.size());
#endif
	}

	/// 内容を書いたファイルを作る（CreateFileW + WriteFile 相当）
	inline void WriteFileW(const std::wstring &strPath, const char *pszContent)
	{
#ifdef _WIN32
		HANDLE hFile = CreateFileW(strPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE) {
			if (pszContent != NULL) {
				DWORD dwWritten = 0;
				WriteFile(hFile, pszContent, static_cast<DWORD>(strlen(pszContent)), &dwWritten, NULL);
			}
			CloseHandle(hFile);
		}
#else
		FILE *pFile = fopen(NativePathFromW(strPath).c_str(), "wb");
		if (pFile != NULL) {
			if (pszContent != NULL) {
				fwrite(pszContent, 1, strlen(pszContent), pFile);
			}
			fclose(pFile);
		}
#endif
	}

	/// ファイルを消す（DeleteFileW 相当）
	inline void DeleteFileW(const std::wstring &strPath)
	{
#ifdef _WIN32
		::DeleteFileW(strPath.c_str());
#else
		unlink(NativePathFromW(strPath).c_str());
#endif
	}

	/// パスの区切り文字（wchar_t）
	inline wchar_t PathSeparatorW(void)
	{
#ifdef _WIN32
		return L'\\';
#else
		return L'/';
#endif
	}

	/// 指定ミリ秒待つ（Sleep 相当）
	inline void SleepMs(unsigned int uMs)
	{
#ifdef _WIN32
		Sleep(uMs);
#else
		usleep(uMs * 1000);
#endif
	}
}
