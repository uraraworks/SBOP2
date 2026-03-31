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
	InitializeCriticalSection(&m_Crit);
}

// デストラクタ

CmySection::~CmySection()
{
	DeleteCriticalSection(&m_Crit);
}

// ロック

void CmySection::Enter(void)
{
	EnterCriticalSection(&m_Crit);
}

// ロック解除

void CmySection::Leave(void)
{
	LeaveCriticalSection(&m_Crit);
}
