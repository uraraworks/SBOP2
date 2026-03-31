/// @file SaveLoadInfoMapShadow.cpp
/// @brief データ保存・読み込みクラス(マップ影情報) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/06
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "InfoMapShadow.h"
#include "SaveLoadInfoMapShadow.h"

CSaveLoadInfoMapShadow::CSaveLoadInfoMapShadow()
{
	// ファイル名の設定
	SetFileName("SBODATA\\SboDataMapShadow.dat");
}

CSaveLoadInfoMapShadow::~CSaveLoadInfoMapShadow()
{
}

void CSaveLoadInfoMapShadow::SetHeaderInfo(PCInfoBase pInfo)
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
