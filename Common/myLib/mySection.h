/// @file mySection.h
/// @brief クリティカルセクションクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2004/09/01
/// @copyright Copyright(C)URARA-works 2004

#pragma once

// クラス宣言

class CmySection
{
public:
			CmySection();					// コンストラクタ
	virtual ~CmySection();					// デストラクタ

	void Enter(void);						// ロック
	void Leave(void);						// ロック解除


protected:
	CRITICAL_SECTION	m_Crit;				// クリティカルセクション
};
