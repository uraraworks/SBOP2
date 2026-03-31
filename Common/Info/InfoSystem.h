/// @file InfoSystem.h
/// @brief システム情報クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/09/25
/// @copyright Copyright(C)URARA-works 2008

#pragma once
#include <vector>
#include "myArray.h"

#include "InfoBase.h"
// キャラステータス初期値
typedef struct _STSYSTEM_INITCHARSTATUS {
	WORD	wStamina,	// スタミナ
				wPower,	// 腕力
				wStrength,	// 体力
				wMagic,	// 魔力
				wSkillful,	// 器用
				wAbillityAT,	// 攻撃技能
				wAbillityDF,	// 防御技能
				wPAtack,	// 攻撃力
				wPDefense,	// 防御力
				wPMagic,	// 魔法力
				wPMagicDefense,	// 魔法防御力
				wPHitAverage,	// 命中率
				wPAvoidAverage,	// 回避率
				wPCriticalAverage,	// クリティカル率
				wAttrFire,	// 属性[火]
				wAttrWind,	// 属性[風]
				wAttrWater,	// 属性[水]
				wAttrEarth,	// 属性[土]
				wAttrLight,	// 属性[光]
				wAttrDark;	// 属性[闇]
	DWORD	dwMaxHP,	// 最大HP
				dwMaxSP,	// 最大SP
				dwInitPosMapID;	// 初期位置マップID
	POINT	ptInitPos;	// 初期位置座標
} STSYSTEM_INITCHARSTATUS, *PSTSYSTEM_INITCHARSTATUS;

typedef class CInfoSystem : public CInfoBase
{
public:
			CInfoSystem();	// コンストラクタ
	virtual ~CInfoSystem();	// デストラクタ

	int	GetElementNo(LPCSTR pszName);	// 要素番号を取得
	DWORD	GetDataSize(void);	// データサイズを取得
	DWORD	GetDataSizeNo(int nNo);	// 指定要素のデータサイズを取得
	LPCSTR	GetName(int nNo);	// 要素名を取得
	PBYTE	GetWriteData(int nNo, PDWORD pdwSize);	// 指定要素の保存用データを取得
	DWORD	ReadElementData(PBYTE pSrc, int nNo);	// 指定要素データを読み込み

	DWORD	GetSendDataSize(void);	// 送信データサイズを取得
	PBYTE	GetSendData(void);	// 送信データを取得
	PBYTE	SetSendData(PBYTE pSrc);	// 送信データから取り込み

	void	Copy(CInfoSystem *pSrc);	// コピー

public:
	PSTSYSTEM_INITCHARSTATUS	m_pInitCharStatus;	// キャラステータス初期値
} CInfoSystem, *PCInfoSystem;
using ARRAYSYSTEMINFO = CStdArray<PCInfoSystem>;
using PARRAYSYSTEMINFO = ARRAYSYSTEMINFO *;

