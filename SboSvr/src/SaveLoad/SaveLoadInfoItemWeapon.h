/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:SaveLoadInfoItemWeapon.h									 */
/* 内容			:データ保存・読み込みクラス(武器情報) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/11													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CSaveLoadInfoItemWeapon : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoItemWeapon();						/* コンストラクタ */
	virtual ~CSaveLoadInfoItemWeapon();						/* デストラクタ */
} CSaveLoadInfoItemWeapon, *PCSaveLoadInfoItemWeapon;

/* Copyright(C)URARA-works 2008 */
