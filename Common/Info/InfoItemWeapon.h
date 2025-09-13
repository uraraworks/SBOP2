/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoItemWeapon.h											 */
/* 内容			:アイテム武器情報クラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/10													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoItemWeapon : public CInfoBase
{
public:
			CInfoItemWeapon();									/* コンストラクタ */
	virtual ~CInfoItemWeapon();									/* デストラクタ */

	virtual int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	virtual DWORD	GetDataSize			(void);								/* データサイズを取得 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	virtual LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	virtual DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	virtual PBYTE	GetSendData			(void);								/* 送信データを取得 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	virtual void	Copy				(CInfoItemWeapon *pSrc);			/* コピー */


public:
	/* 保存しないデータ */

	/* 保存するデータ */
	DWORD		m_dwWeaponInfoID,				/* 武器情報ID */
				m_dwMotionType,					/* 使用可能な攻撃モーション */
				m_dwMotionTypeStand,			/* 戦闘モード中の立ちモーション */
				m_dwMotionTypeWalk;				/* 戦闘モード中のすり足モーション */
	CmyString	m_strName;						/* 武器種別名 */
	ARRAYDWORD	m_adwEffectIDAtack,				/* 通常攻撃時のエフェクトID */
				m_adwEffectIDCritical;			/* クリティカル時のエフェクトID */

	/* 持ち物 */
} CInfoItemWeapon, *PCInfoItemWeapon;
typedef CmyArray<PCInfoItemWeapon, PCInfoItemWeapon>	   ARRAYWEAPONINFO;
typedef CmyArray<PCInfoItemWeapon, PCInfoItemWeapon>	 *PARRAYWEAPONINFO;

/* Copyright(C)URARA-works 2008 */
