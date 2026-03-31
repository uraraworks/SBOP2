/// @file crc.cpp
/// @brief CRC計算クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/03/29
/// @copyright Copyright(C)URARA-works 2005
/// @see http://www.rfc-editor.org/rfc/rfc1952.txt

#include "stdafx.h"
#include "crc.h"

// コンストラクタ

CCRC::CCRC()
{
	m_padwCRC = new DWORD[256];
	MakeTbl();
}

// デストラクタ

CCRC::~CCRC()
{
	if (m_padwCRC) {
		delete [] m_padwCRC;
		m_padwCRC = NULL;
	}
}

// CRCを更新

DWORD CCRC::Update(
	DWORD dwSrc,	// [in] 計算元のCRC値
	PBYTE pBuf,		// [in] 計算したいバッファ
	DWORD dwSize)	// [in] バッファサイズ
{
	DWORD i, dwTmp = dwSrc ^ 0xFFFFFFFF;

	for (i = 0; i < dwSize; i ++) {
		dwTmp = m_padwCRC[(dwTmp ^ pBuf[i]) & 0xFF] ^ (dwTmp >> 8);
	}

	return (dwTmp ^ 0xFFFFFFFF);
}

// CRCを取得

DWORD CCRC::GetCRC(
	PBYTE pBuf,		// [in] 計算したいバッファ
	DWORD dwSize)	// [in] バッファサイズ
{
	return Update(0, pBuf, dwSize);
}

// 計算用テーブルを作成

void CCRC::MakeTbl(void)
{
	DWORD i, j, dwTmp;

	for (i = 0; i < 256; i++) {
		dwTmp = i;
		for (j = 0; j < 8; j++) {
			if (dwTmp & 1) {
				dwTmp = 0xEDB88320 ^ (dwTmp >> 1);

			} else {
				dwTmp = dwTmp >> 1;
			}
		}
		m_padwCRC[i] = dwTmp;
	}
}
