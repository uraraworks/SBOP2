/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoItemTypeBase.h											 */
/* 内容			:アイテム種別基底情報クラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/26													 */
/* ========================================================================= */

#pragma once
#include <vector>
#include "myArray.h"

#include "InfoBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* アイテム種別 */
enum {
	ITEMTYPEID_NONE = 0,				/* 効果無し */
	ITEMTYPEID_CLOTH,					/* 服 */
	ITEMTYPEID_ACCE,					/* アクセサリ */
	ITEMTYPEID_ARMS,					/* 持ち物 */
	ITEMTYPEID_SHIELD,					/* 盾 */
	ITEMTYPEID_HP,						/* HP増減 */
	ITEMTYPEID_LIGHT,					/* 灯り */
	ITEMTYPEID_MAX
};

/* 画像IDメイン(服) */
enum {
	ITEMGRPIDMAIN_CLOTH_NONE = 0,		/* 未設定 */
	ITEMGRPIDMAIN_CLOTH_SP,				/* 特殊服 */
	ITEMGRPIDMAIN_CLOTH_MAX
};

/* 画像IDメイン(持ち物) */
enum {
	ITEMGRPIDMAIN_ARMS_NONE = 0,		/* 未設定 */
	ITEMGRPIDMAIN_ARMS_BOW,				/* 弓 */
	ITEMGRPIDMAIN_WEAPON_GLOVE,			/* 武器(打撃) */
	ITEMGRPIDMAIN_WEAPON_ETC,			/* 武器(その他) */
	ITEMGRPIDMAIN_2X2_ARMS,				/* 持ち物(2x2) */
	ITEMGRPIDMAIN_2X2_SHIELD,			/* 盾(2x2) */
	ITEMGRPIDMAIN_2X2_ARMSSP,			/* 特殊持ち物(2x2) */
	ITEMGRPIDMAIN_2X2_BOW,				/* 弓(2x2) */
	ITEMGRPIDMAIN_ARMS_MAX
};

/* モーション種別 */
#define INFOITEMARMS_MOTION_NONE		0x00000000
#define INFOITEMARMS_MOTION_SWING		0x00000001		/* 振り */
#define INFOITEMARMS_MOTION_POKE		0x00000002		/* 突き */
#define INFOITEMARMS_MOTION_BOW			0x00000004		/* 弓 */
#define INFOITEMARMS_MOTION_BLOW		0x00000008		/* 打撃 */
#define INFOITEMARMS_MOTION_FISHING		0x00000010		/* 釣り */


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoItemTypeBase : public CInfoBase
{
public:
			CInfoItemTypeBase();									/* コンストラクタ */
	virtual ~CInfoItemTypeBase();									/* デストラクタ */

	virtual int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	virtual DWORD	GetDataSize			(void);								/* データサイズを取得 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	virtual LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	virtual DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	virtual PBYTE	GetSendData			(void);								/* 送信データを取得 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	virtual void	Copy				(CInfoItemTypeBase *pSrc);			/* コピー */
	virtual void	SetGrpID			(WORD *pwDst1, WORD *pwDst2);		/* 画像IDを設定 */


public:
	/* 保存しないデータ */

	/* 保存するデータ */
	BYTE		m_byDelAverage,					/* 使ってなくなる確率 */
				m_byTarget,						/* 使用対象 */
				m_byArea;						/* 使用範囲 */
	BOOL		m_bPutOn;						/* 上に置けるか判定 */
	WORD		m_wGrpIDMain,					/* 画像IDメイン */
				m_wGrpIDSub;					/* 画像IDサブ */
	DWORD		m_dwTypeID,						/* 種別ID */
				m_dwItemTypeID,					/* アイテム種別ID */
				m_dwGrpID,						/* 地面にある時の画像ID */
				m_dwIconGrpID,					/* バックパック内の画像ID */
				m_dwDropSoundID,				/* 落ちたときの効果音ID */
				m_dwUseEffectID,				/* 使った時に再生するエフェクトID */
				m_dwUseSoundID;					/* 使った時に再生する効果音ID */
	CmyString	m_strName;						/* アイテム名 */

	/* 持ち物 */
	DWORD		m_dwWeaponInfoID,				/* 武器情報ID */
				m_dwValue,						/* 攻撃力 */
				m_dwMoveWait,					/* 速度 */
				m_dwMoveCount;					/* 飛距離 */
	/* HP増減 */
	DWORD		m_dwValue2;						/* 効果(最大)最小はm_dwValue */
	/* 灯り */
//	DWORD		m_dwValue,						/* 灯りレベル */
//				m_dwValue2;						/* 持続時間 */
} CInfoItemTypeBase, *PCInfoItemTypeBase;
using ARRAYITEMTYPEINFO = CStdArray<PCInfoItemTypeBase>;
using PARRAYITEMTYPEINFO = ARRAYITEMTYPEINFO *;

/* Copyright(C)URARA-works 2007 */
