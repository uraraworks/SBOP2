/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoMapShadow.cpp									 */
/* 内容			:データ保存・読み込みクラス(マップ影情報) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/06													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapShadow.h"
#include "SaveLoadInfoMapShadow.h"


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoMapShadow::CSaveLoadInfoMapShadow					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

CSaveLoadInfoMapShadow::CSaveLoadInfoMapShadow()
{
	/* ファイル名の設定 */
	SetFileName ("SBODATA\\SboDataMapShadow.dat");
}


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoMapShadow::~CSaveLoadInfoMapShadow				 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

CSaveLoadInfoMapShadow::~CSaveLoadInfoMapShadow()
{
}


/* ========================================================================= */
/* 関数名	:CSaveLoadInfoMapShadow::SetHeaderInfo							 */
/* 内容		:ヘッダ情報を設定												 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CSaveLoadInfoMapShadow::SetHeaderInfo(PCInfoBase pInfo)
{
	int i;
	LPCSTR pszName;
	CStringA strHeader;
	CInfoAnime AnimeTmp;

	CSaveLoadInfoBase::SetHeaderInfo (pInfo);

	for (i = 0; ; i ++) {
		pszName = AnimeTmp.GetName (i);
		if (pszName == NULL) {
			break;
		}
		strHeader.Format ("%s%s", PREFIX_INFOANIME, pszName);
		AddHeaderInfo (strHeader);
	}
}

/* Copyright(C)URARA-works 2007 */
