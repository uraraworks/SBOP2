/// @file PasswordHash.cpp
/// @brief アカウントパスワードのハッシュ化ユーティリティ 実装ファイル
/// @details PBKDF2-HMAC-SHA256 (Windows CNG / bcrypt.h) を用いる。

#include "StdAfx.h"
#include "PasswordHash.h"
#include "Platform/SvrPlatform.h"
#include <bcrypt.h>
#include <vector>
#include <cstdio>
#include <cstdlib>

#pragma comment(lib, "bcrypt.lib")

namespace
{
	const int kSaltLen = 16;	// salt のバイト長
	const int kHashLen = 32;	// hash のバイト長
	const unsigned int kMaxIterations = 1000000;	// Verify時に許容する反復回数の上限(DoS対策)
	const char kPrefix[] = "$s1$";

	// ---- Base64 (標準, パディングあり) ----
	const char kB64Table[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string	Base64Encode(const unsigned char *pData, size_t nLen)
	{
		std::string strRet;
		strRet.reserve(((nLen + 2) / 3) * 4);

		size_t i = 0;
		while (i + 3 <= nLen) {
			unsigned int n = (pData[i] << 16) | (pData[i + 1] << 8) | pData[i + 2];
			strRet += kB64Table[(n >> 18) & 0x3F];
			strRet += kB64Table[(n >> 12) & 0x3F];
			strRet += kB64Table[(n >> 6) & 0x3F];
			strRet += kB64Table[n & 0x3F];
			i += 3;
		}

		size_t nRest = nLen - i;
		if (nRest == 1) {
			unsigned int n = (pData[i] << 16);
			strRet += kB64Table[(n >> 18) & 0x3F];
			strRet += kB64Table[(n >> 12) & 0x3F];
			strRet += '=';
			strRet += '=';
		} else if (nRest == 2) {
			unsigned int n = (pData[i] << 16) | (pData[i + 1] << 8);
			strRet += kB64Table[(n >> 18) & 0x3F];
			strRet += kB64Table[(n >> 12) & 0x3F];
			strRet += kB64Table[(n >> 6) & 0x3F];
			strRet += '=';
		}

		return strRet;
	}

	// 1文字を6bit値に変換。不正文字は -1。'='は -2。
	int	Base64DecodeChar(char c)
	{
		if (c == '=') {
			return -2;
		}
		if (c >= 'A' && c <= 'Z') {
			return c - 'A';
		}
		if (c >= 'a' && c <= 'z') {
			return (c - 'a') + 26;
		}
		if (c >= '0' && c <= '9') {
			return (c - '0') + 52;
		}
		if (c == '+') {
			return 62;
		}
		if (c == '/') {
			return 63;
		}
		return -1;
	}

	// Base64 デコード。不正な場合は false を返す。
	bool	Base64Decode(const std::string &strSrc, std::vector<unsigned char> *pOut)
	{
		pOut->clear();

		if (strSrc.empty() || (strSrc.size() % 4) != 0) {
			return false;
		}

		size_t nLen = strSrc.size();
		for (size_t i = 0; i < nLen; i += 4) {
			int n0 = Base64DecodeChar(strSrc[i]);
			int n1 = Base64DecodeChar(strSrc[i + 1]);
			int n2 = Base64DecodeChar(strSrc[i + 2]);
			int n3 = Base64DecodeChar(strSrc[i + 3]);

			// パディングは末尾2文字のみ許可
			if (n0 < 0 || n1 < 0) {
				return false;
			}
			if (n0 == -2 || n1 == -2) {
				return false;
			}
			bool bPad2 = (n2 == -2);
			bool bPad3 = (n3 == -2);
			if (bPad2 && !bPad3) {
				return false;	// "=X" は不正
			}
			if (i + 4 != nLen && (bPad2 || bPad3)) {
				return false;	// パディングは最後のブロックのみ
			}

			unsigned int nVal = ((unsigned int)n0 << 18) | ((unsigned int)n1 << 12);
			pOut->push_back((unsigned char)((nVal >> 16) & 0xFF));

			if (!bPad2) {
				if (n2 < 0) {
					return false;
				}
				nVal |= ((unsigned int)n2 << 6);
				pOut->push_back((unsigned char)((nVal >> 8) & 0xFF));

				if (!bPad3) {
					if (n3 < 0) {
						return false;
					}
					nVal |= (unsigned int)n3;
					pOut->push_back((unsigned char)(nVal & 0xFF));
				}
			}
		}

		return true;
	}

	// PBKDF2-HMAC-SHA256 を計算する。失敗時は false。
	bool	Pbkdf2Sha256(
		const char *pszPassword,
		const unsigned char *pSalt, int nSaltLen,
		unsigned int nIterations,
		unsigned char *pOutHash, int nOutHashLen)
	{
		BCRYPT_ALG_HANDLE hAlg = NULL;
		NTSTATUS status = BCryptOpenAlgorithmProvider(
			&hAlg, BCRYPT_SHA256_ALGORITHM, NULL, BCRYPT_ALG_HANDLE_HMAC_FLAG);
		if (!BCRYPT_SUCCESS(status)) {
			return false;
		}

		size_t nPassLen = strlen(pszPassword);
		status = BCryptDeriveKeyPBKDF2(
			hAlg,
			(PUCHAR)pszPassword, (ULONG)nPassLen,
			(PUCHAR)pSalt, (ULONG)nSaltLen,
			nIterations,
			(PUCHAR)pOutHash, (ULONG)nOutHashLen,
			0);

		BCryptCloseAlgorithmProvider(hAlg, 0);

		return BCRYPT_SUCCESS(status) ? true : false;
	}

	// 定数時間比較 (長さが違えば false。値の比較自体は早期returnしない)
	bool	ConstantTimeEquals(const std::vector<unsigned char> &a, const std::vector<unsigned char> &b)
	{
		if (a.size() != b.size()) {
			return false;
		}

		unsigned char nDiff = 0;
		for (size_t i = 0; i < a.size(); ++i) {
			nDiff |= (a[i] ^ b[i]);
		}

		return (nDiff == 0);
	}

	// "$s1$<iter>$<salt>$<hash>" を分解する。
	bool	ParseStored(
		const std::string &strStored,
		unsigned int *pIterations,
		std::vector<unsigned char> *pSalt,
		std::vector<unsigned char> *pHash)
	{
		if (strStored.compare(0, sizeof(kPrefix) - 1, kPrefix) != 0) {
			return false;
		}

		std::string strBody = strStored.substr(sizeof(kPrefix) - 1);

		size_t nPos1 = strBody.find('$');
		if (nPos1 == std::string::npos) {
			return false;
		}
		std::string strIter = strBody.substr(0, nPos1);

		size_t nPos2 = strBody.find('$', nPos1 + 1);
		if (nPos2 == std::string::npos) {
			return false;
		}
		std::string strSalt = strBody.substr(nPos1 + 1, nPos2 - (nPos1 + 1));
		std::string strHash = strBody.substr(nPos2 + 1);

		if (strIter.empty() || strSalt.empty() || strHash.empty()) {
			return false;
		}

		// 反復回数は数字のみ許可
		for (size_t i = 0; i < strIter.size(); ++i) {
			if (strIter[i] < '0' || strIter[i] > '9') {
				return false;
			}
		}

		unsigned long nIter = strtoul(strIter.c_str(), NULL, 10);
		if (nIter <= 0 || nIter > kMaxIterations) {
			return false;
		}

		if (!Base64Decode(strSalt, pSalt)) {
			return false;
		}
		if (!Base64Decode(strHash, pHash)) {
			return false;
		}

		*pIterations = (unsigned int)nIter;
		return true;
	}
}

namespace PasswordHash
{
	bool	IsAcceptable(const char *pszPassword)
	{
		if (pszPassword == NULL) {
			return false;
		}
		if (pszPassword[0] == '\0') {
			return false;
		}

		for (const unsigned char *p = (const unsigned char *)pszPassword; *p != '\0'; ++ p) {
			// ASCII の表示可能文字のみ。空白(0x20)と DEL(0x7F) も不可。
			// 全角文字は UTF-8 で 0x80 以上のバイトになるためここで弾かれる。
			if (*p < 0x21 || *p > 0x7E) {
				return false;
			}
		}
		return true;
	}

	std::string	Hash(const char *pszPassword)
	{
		if (pszPassword == NULL) {
			return std::string();
		}

		unsigned char pSalt[kSaltLen];
		if (!SboPlatform::GenerateRandomBytes(pSalt, kSaltLen)) {
			// 乱数生成に失敗した場合は絶対にフォールバックしない
			return std::string();
		}

		unsigned char pHash[kHashLen];
		if (!Pbkdf2Sha256(pszPassword, pSalt, kSaltLen, kDefaultIterations, pHash, kHashLen)) {
			return std::string();
		}

		char szIter[16];
		_snprintf_s(szIter, sizeof(szIter), _TRUNCATE, "%u", kDefaultIterations);

		std::string strRet = kPrefix;
		strRet += szIter;
		strRet += '$';
		strRet += Base64Encode(pSalt, kSaltLen);
		strRet += '$';
		strRet += Base64Encode(pHash, kHashLen);

		return strRet;
	}

	bool	Verify(const char *pszStored, const char *pszPassword)
	{
		if (pszStored == NULL || pszPassword == NULL) {
			return false;
		}

		if (!IsHashed(pszStored)) {
			return false;
		}

		unsigned int nIterations = 0;
		std::vector<unsigned char> vSalt;
		std::vector<unsigned char> vStoredHash;

		if (!ParseStored(pszStored, &nIterations, &vSalt, &vStoredHash)) {
			return false;
		}
		if (vStoredHash.size() != (size_t)kHashLen) {
			return false;
		}

		unsigned char pCalcHash[kHashLen];
		if (!Pbkdf2Sha256(
			pszPassword,
			vSalt.empty() ? (const unsigned char *)"" : &vSalt[0], (int)vSalt.size(),
			nIterations, pCalcHash, kHashLen)) {
			return false;
		}

		std::vector<unsigned char> vCalcHash(pCalcHash, pCalcHash + kHashLen);
		return ConstantTimeEquals(vCalcHash, vStoredHash);
	}

	bool	IsHashed(const char *pszStored)
	{
		if (pszStored == NULL) {
			return false;
		}
		return (strncmp(pszStored, kPrefix, sizeof(kPrefix) - 1) == 0);
	}
}
