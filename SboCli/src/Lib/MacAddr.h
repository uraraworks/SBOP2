/// @file MacAddr.h
/// @brief MACアドレス取得クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/04/12
/// @copyright Copyright(C)URARA-works 2005

#pragma once

// クラス宣言

typedef class CMacAddr
{
public:
			CMacAddr();								// コンストラクタ
	virtual ~CMacAddr();							// デストラクタ

	BOOL	Get(PBYTE pDst, int nNo = 0);			// MACアドレスを取得
	BOOL	GetStr(LPSTR pszDst, int nNo = 0);		// MACアドレスを取得(文字列)
	int		GetCount(void);							// NIC数を取得
	BOOL	GetDeviceName(LPSTR pszDst, int nNo = 0);	// デバイス名を取得


protected:
	int		GetNICCount(void);	// NIC数を取得


protected:
	int		m_nCount;			// NIC数
} CMacAddr, *PCMacAddr;
