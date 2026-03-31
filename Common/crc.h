/// @file crc.h
/// @brief CRC計算クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/03/29
/// @copyright Copyright(C)URARA-works 2005

#pragma once

// クラス宣言

class CCRC
{
public:
			CCRC();										// コンストラクタ
	virtual ~CCRC();									// デストラクタ

	DWORD Update(DWORD dwSrc, PBYTE pBuf, DWORD dwSize);	// CRCを更新
	DWORD GetCRC(PBYTE pBuf, DWORD dwSize);					// CRCを取得


private:
	void MakeTbl(void);									// 計算用テーブルを作成


private:
	PDWORD		m_padwCRC;								// 計算用テーブル
};
