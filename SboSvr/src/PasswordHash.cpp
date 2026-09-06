/// @file PasswordHash.cpp
/// @brief アカウントパスワードのハッシュ化ユーティリティ 実装ファイル
/// @details PBKDF2-HMAC-SHA256 を用いる。SHA-256/HMAC-SHA256/PBKDF2 は
///          RFC 6234 / RFC 2104 / RFC 8018 準拠の自前実装(外部ライブラリ・
///          OS API 不使用)。Windows/非Windows で同一コードが通る。

#include "StdAfx.h"
#include "PasswordHash.h"
#include "Platform/SvrPlatform.h"
#include <string.h>
#include <vector>
#include <cstdio>
#include <cstdlib>

namespace
{
	const int kSaltLen = 16;	// salt のバイト長
	const int kHashLen = 32;	// hash のバイト長
	const unsigned int kMaxIterations = 1000000;	// Verify時に許容する反復回数の上限(DoS対策)
	const char kPrefix[] = "$s1$";

	// ============================================================
	//  SHA-256 (RFC 6234 準拠の自前実装。外部ライブラリ・OS API 不使用)
	// ============================================================

	const unsigned int kSha256InitState[8] = {
		0x6a09e667u, 0xbb67ae85u, 0x3c6ef372u, 0xa54ff53au,
		0x510e527fu, 0x9b05688cu, 0x1f83d9abu, 0x5be0cd19u
	};

	const unsigned int kSha256K[64] = {
		0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u,
		0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u,
		0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u,
		0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u,
		0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu,
		0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau,
		0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u,
		0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u,
		0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u,
		0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
		0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u,
		0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u,
		0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u,
		0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
		0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u,
		0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u
	};

	inline unsigned int Sha256Rotr(unsigned int x, int n)
	{
		return (x >> n) | (x << (32 - n));
	}

	/// @brief 1ブロック(64バイト)分の SHA-256 圧縮関数
	void Sha256ProcessBlock(unsigned int state[8], const unsigned char block[64])
	{
		unsigned int w[64];
		int i;

		for (i = 0; i < 16; ++i) {
			w[i] = (static_cast<unsigned int>(block[i * 4 + 0]) << 24)
				 | (static_cast<unsigned int>(block[i * 4 + 1]) << 16)
				 | (static_cast<unsigned int>(block[i * 4 + 2]) << 8)
				 |  static_cast<unsigned int>(block[i * 4 + 3]);
		}
		for (i = 16; i < 64; ++i) {
			unsigned int s0 = Sha256Rotr(w[i - 15], 7) ^ Sha256Rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
			unsigned int s1 = Sha256Rotr(w[i - 2], 17) ^ Sha256Rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
			w[i] = w[i - 16] + s0 + w[i - 7] + s1;
		}

		unsigned int a = state[0];
		unsigned int b = state[1];
		unsigned int c = state[2];
		unsigned int d = state[3];
		unsigned int e = state[4];
		unsigned int f = state[5];
		unsigned int g = state[6];
		unsigned int h = state[7];

		for (i = 0; i < 64; ++i) {
			unsigned int S1 = Sha256Rotr(e, 6) ^ Sha256Rotr(e, 11) ^ Sha256Rotr(e, 25);
			unsigned int ch = (e & f) ^ ((~e) & g);
			unsigned int temp1 = h + S1 + ch + kSha256K[i] + w[i];
			unsigned int S0 = Sha256Rotr(a, 2) ^ Sha256Rotr(a, 13) ^ Sha256Rotr(a, 22);
			unsigned int maj = (a & b) ^ (a & c) ^ (b & c);
			unsigned int temp2 = S0 + maj;

			h = g;
			g = f;
			f = e;
			e = d + temp1;
			d = c;
			c = b;
			b = a;
			a = temp1 + temp2;
		}

		state[0] += a;
		state[1] += b;
		state[2] += c;
		state[3] += d;
		state[4] += e;
		state[5] += f;
		state[6] += g;
		state[7] += h;
	}

	/// @brief SHA-256 の逐次計算用コンテキスト。
	///
	/// PBKDF2 で同じ鍵(パスワード)から作った HMAC の内側/外側パディング
	/// 済み状態を使い回すため、状態(state)を単純な構造体としてコピー
	/// できるようにしてある(動的確保は一切しない)。
	struct Sha256Ctx
	{
		unsigned int state[8];
		unsigned long long nBitLen;	// これまでに Update した総ビット数
		unsigned char pBuf[64];
		size_t nBufLen;					// pBuf に溜まっている端数バイト数
	};

	void Sha256Init(Sha256Ctx *pCtx)
	{
		memcpy(pCtx->state, kSha256InitState, sizeof(pCtx->state));
		pCtx->nBitLen = 0;
		pCtx->nBufLen = 0;
	}

	void Sha256Update(Sha256Ctx *pCtx, const unsigned char *pData, size_t nLen)
	{
		pCtx->nBitLen += static_cast<unsigned long long>(nLen) * 8ULL;

		// 前回の端数と合わせて 64 バイト溜まったら圧縮する
		if (pCtx->nBufLen > 0) {
			size_t nFill = 64 - pCtx->nBufLen;
			if (nFill > nLen) {
				nFill = nLen;
			}
			memcpy(pCtx->pBuf + pCtx->nBufLen, pData, nFill);
			pCtx->nBufLen += nFill;
			pData += nFill;
			nLen -= nFill;

			if (pCtx->nBufLen == 64) {
				Sha256ProcessBlock(pCtx->state, pCtx->pBuf);
				pCtx->nBufLen = 0;
			}
		}

		while (nLen >= 64) {
			Sha256ProcessBlock(pCtx->state, pData);
			pData += 64;
			nLen -= 64;
		}

		if (nLen > 0) {
			memcpy(pCtx->pBuf, pData, nLen);
			pCtx->nBufLen = nLen;
		}
	}

	void Sha256Final(Sha256Ctx *pCtx, unsigned char pDigest[32])
	{
		unsigned char pPad[128];
		size_t nPadLen;

		memset(pPad, 0, sizeof(pPad));
		memcpy(pPad, pCtx->pBuf, pCtx->nBufLen);
		pPad[pCtx->nBufLen] = 0x80;

		nPadLen = (pCtx->nBufLen < 56) ? 64 : 128;
		for (int i = 0; i < 8; ++i) {
			pPad[nPadLen - 1 - i] = static_cast<unsigned char>((pCtx->nBitLen >> (i * 8)) & 0xFF);
		}

		Sha256ProcessBlock(pCtx->state, pPad);
		if (nPadLen == 128) {
			Sha256ProcessBlock(pCtx->state, pPad + 64);
		}

		for (int i = 0; i < 8; ++i) {
			pDigest[i * 4 + 0] = static_cast<unsigned char>((pCtx->state[i] >> 24) & 0xFF);
			pDigest[i * 4 + 1] = static_cast<unsigned char>((pCtx->state[i] >> 16) & 0xFF);
			pDigest[i * 4 + 2] = static_cast<unsigned char>((pCtx->state[i] >> 8) & 0xFF);
			pDigest[i * 4 + 3] = static_cast<unsigned char>( pCtx->state[i]       & 0xFF);
		}
	}

	/// @brief SHA-256 ダイジェスト(32バイト)を一括計算する
	void Sha256(const unsigned char *pData, size_t nLen, unsigned char pDigest[32])
	{
		Sha256Ctx Ctx;
		Sha256Init(&Ctx);
		Sha256Update(&Ctx, pData, nLen);
		Sha256Final(&Ctx, pDigest);
	}

	// ============================================================
	//  HMAC-SHA256 (RFC 2104 準拠)
	// ============================================================

	/// @brief 鍵で ipad/opad を吸収済みの HMAC コンテキスト。
	///
	/// PBKDF2 では同じ鍵(パスワード)に対して何万回も HMAC を計算するため、
	/// 鍵ブロックの吸収(Sha256Update(ipad/opad, 64バイト))を毎回やり直す
	/// のは無駄が大きい。ここで一度だけ吸収した状態を作っておき、
	/// PBKDF2 側は Ctx をコピーして続き(メッセージ)だけ Update すればよい。
	struct HmacSha256Ctx
	{
		Sha256Ctx InnerBase;	// ipad 吸収済み
		Sha256Ctx OuterBase;	// opad 吸収済み
	};

	void HmacSha256Init(HmacSha256Ctx *pCtx, const unsigned char *pKey, size_t nKeyLen)
	{
		unsigned char pKeyBlock[64];
		memset(pKeyBlock, 0, sizeof(pKeyBlock));

		if (nKeyLen > 64) {
			// 鍵がブロック長より長い場合は先にハッシュして短縮する
			unsigned char pKeyHash[32];
			Sha256(pKey, nKeyLen, pKeyHash);
			memcpy(pKeyBlock, pKeyHash, 32);
		} else if (nKeyLen > 0) {
			memcpy(pKeyBlock, pKey, nKeyLen);
		}

		unsigned char pIpad[64];
		unsigned char pOpad[64];
		for (int i = 0; i < 64; ++i) {
			pIpad[i] = static_cast<unsigned char>(pKeyBlock[i] ^ 0x36);
			pOpad[i] = static_cast<unsigned char>(pKeyBlock[i] ^ 0x5c);
		}

		Sha256Init(&pCtx->InnerBase);
		Sha256Update(&pCtx->InnerBase, pIpad, 64);

		Sha256Init(&pCtx->OuterBase);
		Sha256Update(&pCtx->OuterBase, pOpad, 64);
	}

	/// @brief 初期化済みの HmacSha256Ctx を使って1回分の HMAC を計算する。
	///        Ctx 自体は書き換えない(コピーして使う)ので使い回せる。
	void HmacSha256Compute(
		const HmacSha256Ctx *pCtx,
		const unsigned char *pData, size_t nDataLen,
		unsigned char pOutDigest[32])
	{
		Sha256Ctx Inner = pCtx->InnerBase;
		Sha256Update(&Inner, pData, nDataLen);
		unsigned char pInnerDigest[32];
		Sha256Final(&Inner, pInnerDigest);

		Sha256Ctx Outer = pCtx->OuterBase;
		Sha256Update(&Outer, pInnerDigest, 32);
		Sha256Final(&Outer, pOutDigest);
	}

	void HmacSha256(
		const unsigned char *pKey, size_t nKeyLen,
		const unsigned char *pData, size_t nDataLen,
		unsigned char pOutDigest[32])
	{
		HmacSha256Ctx Ctx;
		HmacSha256Init(&Ctx, pKey, nKeyLen);
		HmacSha256Compute(&Ctx, pData, nDataLen, pOutDigest);
	}

	// ============================================================
	//  PBKDF2-HMAC-SHA256 (RFC 8018 準拠)
	// ============================================================

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

	/// @brief PBKDF2-HMAC-SHA256 を計算する(RFC 8018)。dkLen は 32 バイト固定。
	///
	/// HMAC の鍵(パスワード)吸収は反復回数に関わらず1回だけ行い、
	/// 各反復では Sha256Ctx をコピーして続きを Update するだけにすることで、
	/// 反復回数が大きくても実用的な速度で計算できるようにしている。
	bool	Pbkdf2Sha256(
		const char *pszPassword,
		const unsigned char *pSalt, int nSaltLen,
		unsigned int nIterations,
		unsigned char *pOutHash, int nOutHashLen)
	{
		if (nIterations == 0 || nOutHashLen != kHashLen) {
			return false;
		}

		size_t nPassLen = strlen(pszPassword);

		HmacSha256Ctx Ctx;
		HmacSha256Init(&Ctx, (const unsigned char *)pszPassword, nPassLen);

		// dkLen(32) == hLen(32) なので、ブロックは1つ(INT(1) = 0x00000001)のみでよい
		unsigned char pSaltBlock[256 + 4];	// salt は Verify 経路では可変長(理論上任意長)だが、
											// このアプリの salt は常に kSaltLen(16)。安全側に確保。
		if (nSaltLen < 0 || (size_t)nSaltLen > sizeof(pSaltBlock) - 4) {
			return false;
		}
		if (nSaltLen > 0) {
			memcpy(pSaltBlock, pSalt, nSaltLen);
		}
		pSaltBlock[nSaltLen + 0] = 0x00;
		pSaltBlock[nSaltLen + 1] = 0x00;
		pSaltBlock[nSaltLen + 2] = 0x00;
		pSaltBlock[nSaltLen + 3] = 0x01;

		unsigned char pU[32];
		HmacSha256Compute(&Ctx, pSaltBlock, (size_t)nSaltLen + 4, pU);

		unsigned char pT[32];
		memcpy(pT, pU, 32);

		for (unsigned int i = 1; i < nIterations; ++i) {
			unsigned char pNext[32];
			HmacSha256Compute(&Ctx, pU, 32, pNext);
			memcpy(pU, pNext, 32);
			for (int j = 0; j < 32; ++j) {
				pT[j] ^= pU[j];
			}
		}

		memcpy(pOutHash, pT, kHashLen);
		return true;
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

	std::string	Sha256Hex(const unsigned char *pData, size_t nLen)
	{
		static const char kHex[] = "0123456789abcdef";
		unsigned char pDigest[32];
		Sha256(pData, nLen, pDigest);

		std::string strRet;
		strRet.reserve(64);
		for (int i = 0; i < 32; ++i) {
			strRet += kHex[(pDigest[i] >> 4) & 0x0F];
			strRet += kHex[ pDigest[i]       & 0x0F];
		}
		return strRet;
	}

	std::string	HmacSha256Hex(
		const unsigned char *pKey, size_t nKeyLen,
		const unsigned char *pData, size_t nDataLen)
	{
		static const char kHex[] = "0123456789abcdef";
		unsigned char pDigest[32];
		HmacSha256(pKey, nKeyLen, pData, nDataLen, pDigest);

		std::string strRet;
		strRet.reserve(64);
		for (int i = 0; i < 32; ++i) {
			strRet += kHex[(pDigest[i] >> 4) & 0x0F];
			strRet += kHex[ pDigest[i]       & 0x0F];
		}
		return strRet;
	}
}
