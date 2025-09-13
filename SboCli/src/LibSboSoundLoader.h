/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibSboSoundLoader.h										 */
/* 内容			:SBOサウンドデータライブラリ読み込みクラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/03													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* 関数ポインタ定義															 */
/* ========================================================================= */

typedef int		(*LIBSBOSOUNDGetSoundCount)			(void);
typedef int		(*LIBSBOSOUNDGetSoundResourceID)	(int nNo);
typedef DWORD	(*LIBSBOSOUNDGetSoundID)			(int nNo);
typedef int		(*LIBSBOSOUNDGetSoundNo)			(DWORD dwSoundID);
typedef LPCSTR	(*LIBSBOSOUNDGetSoundName)			(DWORD dwSoundID);


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibSboSoundLoader
{
public:
			CLibSboSoundLoader();					/* コンストラクタ */
	virtual	~CLibSboSoundLoader();					/* デストラクタ */

	void	Load (void);									/* 読み込み */
	void	Free (void);									/* 開放 */

	int		GetSoundCount		(void);						/* 効果音数を取得 */
	int		GetSoundResourceID	(int nNo);					/* 効果音のリソースIDを取得 */
	DWORD	GetSoundID			(int nNo);					/* 効果音IDを取得 */
	int		GetSoundNo			(DWORD dwSoundID);			/* 効果音のインデックスを取得 */
	LPCSTR	GetSoundName		(DWORD dwSoundID);			/* 効果音名を取得 */


protected:
	HMODULE				m_hLib;		/* DLLのモジュールハンドル */

	/* 以下関数ポインタ */
	LIBSBOSOUNDGetSoundCount		m_pGetSoundCount;
	LIBSBOSOUNDGetSoundResourceID	m_pGetSoundResourceID;
	LIBSBOSOUNDGetSoundID			m_pGetSoundID;
	LIBSBOSOUNDGetSoundNo			m_pGetSoundNo;
	LIBSBOSOUNDGetSoundName			m_pGetSoundName;
} CLibSboSoundLoader, *PCLibSboSoundLoader;

/* Copyright(C)URARA-works 2007 */
