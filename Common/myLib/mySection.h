/// @file mySection.h
/// @brief クリティカルセクションクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2004/09/01
/// @copyright Copyright(C)URARA-works 2004
///
/// 中身は std::recursive_mutex。以前は Windows の CRITICAL_SECTION を
/// 直接持っていたが、脱Windows 対応のため置き換えた。
///
/// **再帰可能なミューテックスであることが必要。** CRITICAL_SECTION は
/// 同じスレッドから何度でも入れる仕様で、CLibInfoBase がこれを継承して
/// いるため、単純な std::mutex にすると自己デッドロックしうる。

#pragma once

#include <mutex>

// クラス宣言

class CmySection
{
public:
			CmySection();					// コンストラクタ
	virtual ~CmySection();					// デストラクタ

	void Enter(void);						// ロック
	void Leave(void);						// ロック解除


protected:
	std::recursive_mutex	m_Crit;			// 排他用(再帰可能)
};
