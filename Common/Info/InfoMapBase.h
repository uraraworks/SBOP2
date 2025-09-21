/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:InfoMapBase.h												 */
/* 内容			:マップ情報基底クラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/08													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

class CLibInfoMapParts;
class CLibInfoMapEvent;
class CInfoMapEventBase;
class CLibInfoMapObjectData;
class CLibInfoMapObject;
class CInfoMapObjectData;

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */
/* 天候 */
enum {
	WEATHERTYPE_NONE = 0,
	WEATHERTYPE_CLOUD,			/* 雲 */
	WEATHERTYPE_MISTY,			/* 霧 */
	WEATHERTYPE_SNOW,			/* 雪 */
	WEATHERTYPE_MAX
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoMapBase : public CInfoBase
{
public:
			CInfoMapBase();								/* コンストラクタ */
	virtual ~CInfoMapBase();							/* デストラクタ */

	void Create			(CLibInfoMapParts *pLibInfoMapParts);	/* 作成 */

	void Init			(int cx, int cy, WORD wParts, BOOL bDeleteMapEvent = TRUE);	/* 初期化 */
	void RenewSize		(int nDirection, int nSize);					/* サイズ更新 */
	void FillShadow		(int x, int y, int cx, int cy, WORD wShadowID);	/* 矩形を塗りつぶし */

	int		GetElementNo	(LPCSTR pszName);					/* 要素番号を取得 */
	DWORD	GetDataSize		(void);								/* データサイズを取得 */
	DWORD	GetDataSizeNo	(int nNo);							/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName			(int nNo);							/* 要素名を取得 */
	PBYTE	GetWriteData	(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData	(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	virtual void TimerProc	(DWORD dwTime);						/* 時間処理 */

	BOOL	IsMapEvent		(int x, int y);						/* 指定座標にマップイベントがあるか判定 */
	BOOL	IsHitMapEvent	(RECT *pPos);						/* 指定座標でマップイベントにぶつかるか判定 */
	int		GetMapEventType	(int x, int y);						/* 指定座標のマップイベント種別を取得 */
	WORD	GetParts		(int x, int y);						/* 指定座標のパーツ番号を取得 */
	void	SetParts		(int x, int y, DWORD dwPartsID);	/* 指定座標のパーツ番号を設定 */
	WORD	GetPartsPile	(int x, int y);						/* 指定座標の重ね合わせパーツ番号を取得 */
	void	SetPartsPile	(int x, int y, DWORD dwPartsID);	/* 指定座標の重ね合わせパーツ番号を設定 */
	BOOL	IsMove			(int x, int y, int nDirection);		/* 進入可能か判定 */
	BOOL	IsMoveOut		(int x, int y, int nDirection);		/* 脱出可能か判定 */
	void	DeleteParts		(DWORD dwPartsID);					/* 指定パーツを削除 */
	WORD	GetShadow		(int x, int y);						/* 指定座標の影番号を取得 */
	void	SetShadow		(int x, int y, DWORD dwShadowID);	/* 指定座標の影番号を設定 */
	void	DeleteShadow	(DWORD dwShadowID);					/* 指定影を削除 */
	DWORD	GetMapObject	(int x, int y);						/* 指定座標のマップオブジェクトIDを取得 */
	BOOL	IsFlg			(int x, int y, DWORD dwFlg);		/* 指定属性か判定 */
	BOOL	IsEnableBattle	(void);								/* 戦闘許可か判定 */

	DWORD	GetSendDataSize	(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData		(void);								/* 送信データを取得 */
	PBYTE	SetSendData		(PBYTE pSrc);						/* 送信データを設定 */

	void	Copy		(CInfoMapBase *pSrc);					/* コピー */
	void	RenewHitTmp	(void);									/* マップパーツ以外での当たり判定を更新 */

	/* マップイベント関連 */
	void				RenewMapEvent	(void);						/* マップイベント更新 */
	int					GetEventCount	(void);						/* マップイベント数を取得 */
	CInfoMapEventBase	*GetEvent		(int nNo);					/* マップイベントを取得 */
	CInfoMapEventBase	*GetEvent		(int x, int y);				/* マップイベントを取得 */
	/* マップオブジェクト配置データ関連 */
	int					GetMapObjectDataCount	(void);				/* マップオブジェクト配置データ数を取得 */
	CInfoMapObjectData	*GetObjectData			(int nNo);			/* マップオブジェクト配置データを取得 */
	CInfoMapObjectData	*GetObjectData			(int x, int y);		/* マップオブジェクト配置データを取得 */


public:
	SIZE		m_sizeMap;				/* マップサイズ */
	DWORD		m_dwMapID,				/* マップID */
				m_dwBGMID,				/* BGMID */
				m_dwWeatherType;		/* 天気種別 */
	BOOL		m_bEnableBattle,		/* 戦闘許可 */
				m_bRecovery;			/* 気絶後回復する */
	PBYTE		m_pbyMapEvent,			/* 各座標のマップイベント */
				m_pbyHitTmp;			/* マップパーツ以外での当たり判定 */
	PWORD		m_pwMap,				/* マップ */
				m_pwMapPile,			/* マップ重ね合わせ */
				m_pwMapShadow;			/* マップ影 */
	BYTE		m_byLevel;				/* 暗さレベル */
	CmyString	m_strMapName;			/* マップ名 */

	CLibInfoMapParts		*m_pLibInfoMapParts;		/* マップパーツ情報 */
	CLibInfoMapEvent		*m_pLibInfoMapEvent;		/* マップイベント情報 */
	CLibInfoMapObjectData	*m_pLibInfoMapObjectData;	/* マップオブジェクト配置データ */
	CLibInfoMapObject		*m_pLibInfoMapObject;		/* マップオブジェクト情報 */
} CInfoMapBase, *PCInfoMapBase;
typedef CmyArray<PCInfoMapBase, PCInfoMapBase>	  ARRAYINFOMAPBASE;
typedef CmyArray<PCInfoMapBase, PCInfoMapBase>	*PARRAYINFOMAPBASE;

/* Copyright(C)URARA-works 2006 */
