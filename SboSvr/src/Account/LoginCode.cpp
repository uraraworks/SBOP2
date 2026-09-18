/// @file LoginCode.cpp
/// @brief ログインコード・端末トークンの生成/正規化/ハッシュ 実装ファイル

#include "StdAfx.h"
#include "LoginCode.h"
#include "PasswordHash.h"
#include "Platform/SvrPlatform.h"

#include <cctype>
#include <cstring>

namespace
{
	// Crockford Base32: 0-9 と A-Z から I, L, O, U を除いた32文字
	const char kAlphabet[33] = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";

	/// @brief バイト列を Crockford Base32 に符号化する。
	///        nBytes*8 が5の倍数であること(端数ビットは扱わない)。
	std::string	EncodeCrockfordBase32(const unsigned char *pData, size_t nBytes)
	{
		std::string strOut;
		size_t nBits = nBytes * 8;
		size_t nChars = nBits / 5;
		strOut.reserve(nChars);

		for (size_t i = 0; i < nChars; ++i) {
			size_t nBitPos = i * 5;
			size_t nByteIndex = nBitPos / 8;
			size_t nBitOffsetInByte = nBitPos % 8;

			// 3バイト(24bit)の窓を作り、必要な5bitを取り出す。
			// 末尾で2バイト目・3バイト目が存在しない場合は0として扱う。
			unsigned int nCombined = (unsigned int)pData[nByteIndex] << 16;
			if (nByteIndex + 1 < nBytes) {
				nCombined |= (unsigned int)pData[nByteIndex + 1] << 8;
			}
			if (nByteIndex + 2 < nBytes) {
				nCombined |= (unsigned int)pData[nByteIndex + 2];
			}

			unsigned int nShift = 24 - nBitOffsetInByte - 5;
			unsigned int nVal = (nCombined >> nShift) & 0x1F;
			strOut += kAlphabet[nVal];
		}

		return strOut;
	}

	/// @brief バイト列を小文字16進文字列に変換する(SessionStore::ToHexLower と同等)。
	std::string	ToHexLower(const unsigned char *pData, size_t nLength)
	{
		static const char kHexTable[] = "0123456789abcdef";
		std::string strResult;
		strResult.reserve(nLength * 2);
		for (size_t i = 0; i < nLength; ++i) {
			strResult += kHexTable[(pData[i] >> 4) & 0x0F];
			strResult += kHexTable[pData[i] & 0x0F];
		}
		return strResult;
	}

	std::string	TrimAsciiSpace(const std::string &strInput)
	{
		size_t nStart = 0;
		while ((nStart < strInput.size()) && std::isspace((unsigned char)strInput[nStart])) {
			++nStart;
		}
		size_t nEnd = strInput.size();
		while ((nEnd > nStart) && std::isspace((unsigned char)strInput[nEnd - 1])) {
			--nEnd;
		}
		return strInput.substr(nStart, nEnd - nStart);
	}
}

namespace LoginCode
{
	std::string	GenerateCode(void)
	{
		// 80bit(=16文字×5bit) ちょうどの乱数を使う。端数ビットは出ない。
		unsigned char pRandom[10];
		if (!SboPlatform::GenerateRandomBytes(pRandom, sizeof(pRandom))) {
			return std::string();
		}

		std::string strCode = EncodeCrockfordBase32(pRandom, sizeof(pRandom));
		return strCode;
	}

	std::string	FormatCodeForDisplay(const std::string &strCode)
	{
		std::string strResult;
		strResult.reserve(strCode.size() + strCode.size() / 4);

		for (size_t i = 0; i < strCode.size(); ++i) {
			if ((i > 0) && (i % 4 == 0)) {
				strResult += '-';
			}
			strResult += strCode[i];
		}
		return strResult;
	}

	std::string	NormalizeCode(const std::string &strInput, bool &outValid)
	{
		outValid = false;

		std::string strStripped;
		strStripped.reserve(strInput.size());
		for (size_t i = 0; i < strInput.size(); ++i) {
			unsigned char ch = (unsigned char)strInput[i];
			if ((ch == '-') || (ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n')) {
				continue;
			}

			char up = (char)std::toupper(ch);
			if (up == 'O') {
				up = '0';
			} else if ((up == 'I') || (up == 'L')) {
				up = '1';
			}
			strStripped += up;
		}

		if (strStripped.size() != kCodeLength) {
			return std::string();
		}

		for (size_t i = 0; i < strStripped.size(); ++i) {
			if (std::strchr(kAlphabet, strStripped[i]) == NULL) {
				return std::string();
			}
		}

		outValid = true;
		return strStripped;
	}

	std::string	HashCode(const std::string &strNormalizedCode)
	{
		return PasswordHash::Sha256Hex(
			(const unsigned char *)strNormalizedCode.data(), strNormalizedCode.size());
	}

	std::string	GenerateDeviceToken(void)
	{
		unsigned char pRandom[kDeviceTokenBytes];
		if (!SboPlatform::GenerateRandomBytes(pRandom, sizeof(pRandom))) {
			return std::string();
		}
		return ToHexLower(pRandom, sizeof(pRandom));
	}

	std::string	HashDeviceToken(const std::string &strToken)
	{
		std::string strTrimmed = TrimAsciiSpace(strToken);
		return PasswordHash::Sha256Hex(
			(const unsigned char *)strTrimmed.data(), strTrimmed.size());
	}

	bool	IsAcceptableAccountName(const std::string &strName)
	{
		if (strName.empty()) {
			return false;
		}

		for (size_t i = 0; i < strName.size(); ++i) {
			unsigned char ch = (unsigned char)strName[i];
			// ASCII の表示可能文字(0x21〜0x7E)のみ。PasswordHash::IsAcceptable と同条件。
			if ((ch < 0x21) || (ch > 0x7E)) {
				return false;
			}
		}
		return true;
	}

	std::string	NormalizeAccountName(const std::string &strName)
	{
		std::string strResult = strName;
		for (size_t i = 0; i < strResult.size(); ++i) {
			unsigned char ch = (unsigned char)strResult[i];
			if ((ch >= 'A') && (ch <= 'Z')) {
				strResult[i] = (char)(ch - 'A' + 'a');
			}
		}
		return strResult;
	}
}
