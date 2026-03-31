/// @file SaveLoadInfoMapParts.cpp
/// @brief データ保存・読み込みクラス(マップパーツ情報) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "InfoMapParts.h"
#include "SaveLoadInfoMapParts.h"

CSaveLoadInfoMapParts::CSaveLoadInfoMapParts()
{
	// ファイル名の設定
	SetFileName("SBODATA\\SboDataMapParts.dat");
}

CSaveLoadInfoMapParts::~CSaveLoadInfoMapParts()
{
}

void CSaveLoadInfoMapParts::SetHeaderInfo(PCInfoBase pInfo)
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
