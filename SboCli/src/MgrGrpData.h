/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MgrGrpData.h												 */
/* 内容			:グラフィックデータマネージャクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
/* ========================================================================= */

#pragma once
#include <vector>

class CImg32;

/* ========================================================================= */
/* 定数宣言																	 */
/* ========================================================================= */

typedef std::vector<CImg32 *>	  ARRAYCIMG;
typedef std::vector<CImg32 *>	*PARRAYCIMG;


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CMgrGrpData
{
public:
			CMgrGrpData();									/* コンストラクタ */
	virtual ~CMgrGrpData();									/* デストラクタ */

	BOOL	Load				(void);								/* 読み込み */
	BOOL	CheckVersion		(LPCSTR pszVersion);				/* バージョンチェック */
	void	Write				(LPCSTR pszFileName, CImg32 *pSrc);	/* PNGデータの保存 */
	CImg32	*GetDibSystem		(void);								/* システム画像イメージを取得 */
	CImg32	*GetDibIcon			(void);								/* アイコン画像イメージを取得 */
	CImg32	*GetDibShadow		(void);								/* 影画像イメージを取得 */
	CImg32	*GetDibMapParts		(int nNo);							/* マップ画像イメージを取得 */
	CImg32	*GetDibMapShadow	(int nNo);							/* マップ影画像イメージを取得 */
	CImg32	*GetDibItem			(int nNo);							/* アイテム画像イメージを取得 */
	CImg32	*GetDibNumS			(void);								/* 数字(S)画像イメージを取得 */
	CImg32	*GetDibNumM			(void);								/* 数字(M)画像イメージを取得 */
	CImg32	*GetDibNumL			(void);								/* 数字(L)画像イメージを取得 */
	CImg32	*GetDibNPC			(int nNo);							/* NPC画像イメージを取得 */
	CImg32	*GetDibBody			(WORD wFamilyID);					/* 体画像イメージを取得 */
	CImg32	*GetDibEar			(WORD wFamilyID);					/* 耳画像イメージを取得 */
	CImg32	*GetDibCloth		(int nNo);							/* 服画像イメージを取得 */
	CImg32	*GetDibEye			(int nNo);							/* 目画像イメージを取得 */
	CImg32	*GetDibHairD		(WORD wHairColorID);				/* 髪(下)画像イメージを取得 */
	CImg32	*GetDibHairU		(WORD wHairColorID);				/* 髪(上)画像イメージを取得 */
	CImg32	*GetDibSP			(int nNo);							/* 特殊服画像イメージを取得 */
	CImg32	*GetDibAcce			(int nNo);							/* アクセサリ画像イメージを取得 */
	CImg32	*GetDibEfc32		(int nNo);							/* エフェクト(32)画像イメージを取得 */
	CImg32	*GetDibEfc64		(int nNo);							/* エフェクト(64)画像イメージを取得 */
	CImg32	*GetDibWeapon		(int nNo);							/* 武器画像イメージを取得 */
	CImg32	*GetDibWeaponBow	(int nNo);							/* 武器(弓)画像イメージを取得 */
	CImg32	*GetDibWeaponArrow	(int nNo);							/* 武器(矢)画像イメージを取得 */
	CImg32	*GetDibWeaponGlove	(int nNo);							/* 武器(打撃)画像イメージを取得 */
	CImg32	*GetDibWeaponEtc	(int nNo);							/* 武器(その他)画像イメージを取得 */
	CImg32	*GetDibEfcBalloon	(int nNo);							/* 噴出し画像イメージを取得 */
	CImg32	*GetDib2x2Body		(WORD wFamilyID);					/* 2x2体画像イメージを取得 */
	CImg32	*GetDib2x2Cloth		(WORD wClothID);					/* 2x2服画像イメージを取得 */
	CImg32	*GetDib2x2Eye		(int nNo);							/* 2x2目画像イメージを取得 */
	CImg32	*GetDib2x2Hair		(WORD wHairColorID);				/* 2x2髪画像イメージを取得 */
	CImg32	*GetDib2x2SPCloth	(WORD wSPClothID);					/* 2x2特殊服画像イメージを取得 */
	CImg32	*GetDib2x2SPHair	(WORD wSPHairID);					/* 2x2特殊髪画像イメージを取得 */
	CImg32	*GetDib2x2Arms		(WORD wArmsID);						/* 2x2持ち物画像イメージを取得 */
	CImg32	*GetDib2x2Shield	(WORD wShieldID);					/* 2x2盾画像イメージを取得 */
	CImg32	*GetDib2x2ArmsSP	(WORD wArmsID);						/* 2x2特殊持ち物画像イメージを取得 */
	CImg32	*GetDib2x2Bow		(WORD wBowID);						/* 2x2弓画像イメージを取得 */
	CImg32	*GetDib2x2NPC		(WORD wNPCID);						/* 2x2NPC画像イメージを取得 */
	CImg32	*GetDib2x2CharShadow(int nNo);							/* 2x2キャラ影画像イメージを取得 */
	CImg32	*GetDib2x2NPCShadow	(int nNo);							/* 2x2NPC影画像イメージを取得 */

	CImg32	*GetDib				(DWORD dwGrpIDMain, DWORD dwGrpIDSub, DWORD dwParam);	/* 画像イメージを取得 */

	CImg32	*GetDibTmpLogo			(void);							/* URARA-worksロゴ画像イメージを取得 */
	CImg32	*GetDibTmpTitle			(void);							/* タイトル画像イメージを取得 */
	CImg32	*GetDibTmpTitleBack		(void);							/* タイトル背景画像イメージを取得 */
	CImg32	*GetDibTmpTitleCloud	(void);							/* タイトル雲画像イメージを取得 */
	CImg32	*GetDibTmpLoginMenuBack	(void);							/* ログインメニュー画像イメージを取得 */

	int		GetMapPartsCount	(void);								/* マップパーツ数を取得 */
	CImg32	*GetDibMapPartsTmp	(int nNo);							/* 等倍マップ画像イメージを取得 */
	void	ReadMapPartsTmp		(void);								/* 等倍マップパーツ画像を読み込み */
	void	DeleteMapPartsTmp	(void);								/* 等倍マップパーツ画像を削除 */
	int		GetMapShadowCount	(void);								/* マップ影画像数を取得 */
	CImg32	*GetDibMapShadowTmp	(int nNo);							/* 等倍マップ影画像イメージを取得 */
	void	ReadMapShadowTmp	(void);								/* 等倍マップ影画像を読み込み */
	void	DeleteMapShadowTmp	(void);								/* 等倍マップ影画像を削除 */
	int		GetItemCount		(void);								/* アイテム画像数を取得 */
	CImg32	*GetDibItemTmp		(int nNo);							/* 等倍アイテム画像イメージを取得 */
	void	ReadItemTmp			(void);								/* 等倍アイテム画像を読み込み */
	void	DeleteItemTmp		(void);								/* 等倍アイテム画像を削除 */

	int		GetGrpSize			(DWORD dwGrpIDMain);				/* 画像１つあたりのサイズを取得 */
	int		GetGrpCountX		(DWORD dwGrpIDMain);				/* 画像１ファイルの横個数を取得 */
	int		GetGrpCountY		(DWORD dwGrpIDMain);				/* 画像１ファイルの縦個数を取得 */
	void	GetGrpPos			(DWORD dwGrpIDMain, DWORD dwGrpIDSub, POINT &ptDst, WORD wGrpID = 0);	/* 指定画像の位置を取得 */


private:
	BOOL	Read	(LPSTR pszName, CImg32 **pDib, int nSize);	/* PNGデータの読み込み */
	BOOL	Read256	(LPSTR pszName, CImg32 **pDib, int nSize);	/* PNGデータの読み込み */


private:
	HMODULE		m_hDll;					/* DLLのモジュールハンドル */

	int			m_nMapPartsCount,		/* マップパーツ画像数 */
				m_nMapShadowCount;		/* マップ影画像数 */
	CImg32		*m_pImgSystem,			/* システム */
				*m_pImgIcon,			/* アイコン */
				*m_pImgShadow,			/* 影画像 */
				*m_pImgItem,			/* アイテム画像 */
				*m_pImgNumS,			/* 数字(S) */
				*m_pImgNumM,			/* 数字(M) */
				*m_pImgNumL;			/* 数字(L) */
	PARRAYCIMG	m_paImgMapParts,		/* マップパーツ */
				m_paImgMapShadow,		/* マップ影 */
				m_paImgNPC,				/* NPC */
				m_paImgBody,			/* 体 */
				m_paImgEar,				/* 耳 */
				m_paImgCloth,			/* 服 */
				m_paImgEye,				/* 目 */
				m_paImgHairD,			/* 髪(下) */
				m_paImgHairU,			/* 髪(上) */
				m_paImgSP,				/* 特殊服 */
				m_paImgAcce,			/* アクセサリ */
				m_paImgMapPartsTmp,		/* 等倍マップパーツ */
				m_paImgMapShadowTmp,	/* 等倍マップ影 */
				m_paImgItemTmp,			/* 等倍アイテム */
				m_paImgEfc32,			/* エフェクト(32) */
				m_paImgEfc64,			/* エフェクト(64) */
				m_paImgWeapon,			/* 武器 */
				m_paImgWeaponBow,		/* 武器(弓) */
				m_paImgWeaponArrow,		/* 武器(矢) */
				m_paImgWeaponGlove,		/* 武器(打撃) */
				m_paImgWeaponEtc,		/* 武器(その他) */
				m_paImgEfcBalloon,		/* 噴出し */
				m_paImg2x2Body,			/* 2x2体 */
				m_paImg2x2Cloth,		/* 2x2服 */
				m_paImg2x2Eye,			/* 2x2目 */
				m_paImg2x2Hair,			/* 2x2髪 */
				m_paImg2x2SPCloth,		/* 2x2特殊服 */
				m_paImg2x2SPHair,		/* 2x2特殊髪 */
				m_paImg2x2Arms,			/* 2x2持ち物 */
				m_paImg2x2Shield,		/* 2x2盾 */
				m_paImg2x2ArmsSP,		/* 2x2特殊持ち物 */
				m_paImg2x2Bow,			/* 2x2‹| */
				m_paImg2x2NPC,			/* 2x2NPC */
				m_paImg2x2CharShadow,	/* 2x2キャラ影 */
				m_paImg2x2NPCShadow;	/* 2x2NPC‰e */
} CMgrGrpData, *PCMgrGrpData;

/* Copyright(C)URARA-works 2006 */
