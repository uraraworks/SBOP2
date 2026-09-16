/// @file AtackTargetDecision.cpp
/// @brief 攻撃対象になり得る移動種別か判定(純粋関数) 実装ファイル
/// @copyright Copyright(C)URARA-works

#include "AtackTargetDecision.h"

namespace AtackTargetDecision
{
	bool IsAtackTargetMoveType(int nMoveType)
	{
		switch (nMoveType) {
		case 0:		// CHARMOVETYPE_PC
		case 9:		// CHARMOVETYPE_BATTLE1
		case 10:	// CHARMOVETYPE_BATTLE2
		case 5:		// CHARMOVETYPE_ATACKANIME
			return true;
		default:
			return false;
		}
	}
}
