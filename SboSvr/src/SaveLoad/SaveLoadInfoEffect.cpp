/// @file SaveLoadInfoEffect.cpp
/// @brief データ保存・読み込みクラス(エフェクト情報) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/26
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "InfoEffect.h"
#include "SaveLoadInfoEffect.h"

CSaveLoadInfoEffect::CSaveLoadInfoEffect()
{
	// ファイル名の設定
	SetFileName("SBODATA\\SboDataEffect.dat");
}

CSaveLoadInfoEffect::~CSaveLoadInfoEffect()
{
}

void CSaveLoadInfoEffect::SetHeaderInfo(PCInfoBase pInfo)
{
	int i;
	LPCSTR pszName;
	CStringA strHeader;
	CInfoAnime AnimeTmp;

	CSaveLoadInfoBase::SetHeaderInfo(pInfo);

	for (i = 0; ; i ++) {
		pszName = AnimeTmp.GetName(i);
		if (pszName == NULL) {
			break;
		}
		strHeader.Format("%s%s", PREFIX_INFOANIME, pszName);
		AddHeaderInfo(strHeader);
	}
}
