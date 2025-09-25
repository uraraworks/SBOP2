/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoEffect.cpp										 */
/* 内容			:データ保存・読み込みクラス(エフェクト情報) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoEffect.h"
#include "SaveLoadInfoEffect.h"


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoEffect::CSaveLoadInfoEffect						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

CSaveLoadInfoEffect::CSaveLoadInfoEffect()
{
	/* ファイル名の設定 */
	SetFileName ("SBODATA\\SboDataEffect.dat");
}


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoEffect::~CSaveLoadInfoEffect						 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

CSaveLoadInfoEffect::~CSaveLoadInfoEffect()
{
}


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoEffect::SetHeaderInfo								 */
/* 内容		:ヘッダ情報を設定												 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CSaveLoadInfoEffect::SetHeaderInfo(PCInfoBase pInfo)
{
	int i;
	char szTmp[256];
	LPCSTR pszName;
	CInfoAnime AnimeTmp;

	CSaveLoadInfoBase::SetHeaderInfo (pInfo);

	for (i = 0; ; i ++) {
		pszName = AnimeTmp.GetName (i);
		if (pszName == NULL) {
			break;
		}
		wsprintf (szTmp, _T("%s%s"), PREFIX_INFOANIME, pszName);
		AddHeaderInfo (szTmp);
	}
}

/* Copyright(C)URARA-works 2007 */
