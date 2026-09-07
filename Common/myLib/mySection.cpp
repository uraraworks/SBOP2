/// @file mySection.cpp
/// @brief クリティカルセクションクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2004/09/01
/// @copyright Copyright(C)URARA-works 2004

#include "StdAfx.h"
#include "mySection.h"

// コンストラクタ

CmySection::CmySection()
{
}

// デストラクタ

CmySection::~CmySection()
{
}

// ロック

void CmySection::Enter(void)
{
	m_Crit.lock();
}

// ロック解除

void CmySection::Leave(void)
{
	m_Crit.unlock();
}
