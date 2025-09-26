/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MgrGrpData.h												 */
/* 内容			:グラフィックデータマネージャクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "../../SboGrpData/resource.h"
#include "Img32.h"
#include "third_party/lodepng.h"
#include "MgrGrpData.h"

#include <vector>
#include <cstdio>



/* ========================================================================= */
/* 関数名	:CMgrGrpData::CMgrGrpData										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

CMgrGrpData::CMgrGrpData()
{
	m_hDll					= NULL;
	m_nMapPartsCount		= 0;
	m_nMapShadowCount		= 0;
	m_pImgSystem			= NULL;
	m_pImgIcon				= NULL;
	m_pImgShadow			= NULL;
	m_pImgItem				= NULL;
	m_pImgNumS				= NULL;
	m_pImgNumM				= NULL;
	m_pImgNumL				= NULL;
	m_paImgMapParts			= new ARRAYCIMG;
	m_paImgMapShadow		= new ARRAYCIMG;
	m_paImgNPC				= new ARRAYCIMG;
	m_paImgBody				= new ARRAYCIMG;
	m_paImgEar				= new ARRAYCIMG;
	m_paImgCloth			= new ARRAYCIMG;
	m_paImgEye				= new ARRAYCIMG;
	m_paImgHairD			= new ARRAYCIMG;
	m_paImgHairU			= new ARRAYCIMG;
	m_paImgSP				= new ARRAYCIMG;
	m_paImgAcce				= new ARRAYCIMG;
	m_paImgMapPartsTmp		= new ARRAYCIMG;
	m_paImgMapShadowTmp		= new ARRAYCIMG;
	m_paImgItemTmp			= new ARRAYCIMG;
	m_paImgEfc32			= new ARRAYCIMG;
	m_paImgEfc64			= new ARRAYCIMG;
	m_paImgWeapon			= new ARRAYCIMG;
	m_paImgWeaponBow		= new ARRAYCIMG;
	m_paImgWeaponArrow		= new ARRAYCIMG;
	m_paImgWeaponGlove		= new ARRAYCIMG;
	m_paImgWeaponEtc		= new ARRAYCIMG;
	m_paImgEfcBalloon		= new ARRAYCIMG;
	m_paImg2x2Body			= new ARRAYCIMG;
	m_paImg2x2Cloth			= new ARRAYCIMG;
	m_paImg2x2Eye			= new ARRAYCIMG;
	m_paImg2x2Hair			= new ARRAYCIMG;
	m_paImg2x2SPCloth		= new ARRAYCIMG;
	m_paImg2x2SPHair		= new ARRAYCIMG;
	m_paImg2x2Arms			= new ARRAYCIMG;
	m_paImg2x2Shield		= new ARRAYCIMG;
	m_paImg2x2ArmsSP		= new ARRAYCIMG;
	m_paImg2x2Bow			= new ARRAYCIMG;
	m_paImg2x2NPC			= new ARRAYCIMG;
	m_paImg2x2CharShadow	= new ARRAYCIMG;
	m_paImg2x2NPCShadow		= new ARRAYCIMG;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::~CMgrGrpData										 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

CMgrGrpData::~CMgrGrpData()
{
	PCImg32 pImg;

	if (m_hDll) {
		FreeLibrary (m_hDll);
		m_hDll = NULL;
	}

	SAFE_DELETE (m_pImgSystem);
	SAFE_DELETE (m_pImgIcon);
	SAFE_DELETE (m_pImgShadow);
	SAFE_DELETE (m_pImgItem);
	SAFE_DELETE (m_pImgNumS);
	SAFE_DELETE (m_pImgNumM);
	SAFE_DELETE (m_pImgNumL);
	pImg = NULL;
	FreeInfo (m_paImgMapParts,		pImg);
	FreeInfo (m_paImgNPC,			pImg);
	FreeInfo (m_paImgBody,			pImg);
	FreeInfo (m_paImgEar,			pImg);
	FreeInfo (m_paImgCloth,			pImg);
	FreeInfo (m_paImgEye,			pImg);
	FreeInfo (m_paImgHairD,			pImg);
	FreeInfo (m_paImgHairU,			pImg);
	FreeInfo (m_paImgSP,			pImg);
	FreeInfo (m_paImgAcce,			pImg);
	FreeInfo (m_paImgMapPartsTmp,	pImg);
	FreeInfo (m_paImgMapShadowTmp,	pImg);
	FreeInfo (m_paImgItemTmp,		pImg);
	FreeInfo (m_paImgEfc32,			pImg);
	FreeInfo (m_paImgEfc64,			pImg);
	FreeInfo (m_paImgWeapon,		pImg);
	FreeInfo (m_paImgWeaponBow,		pImg);
	FreeInfo (m_paImgWeaponArrow,	pImg);
	FreeInfo (m_paImgWeaponGlove,	pImg);
	FreeInfo (m_paImgWeaponEtc,		pImg);
	FreeInfo (m_paImgEfcBalloon,	pImg);
	FreeInfo (m_paImg2x2Body,		pImg);
	FreeInfo (m_paImg2x2Cloth,		pImg);
	FreeInfo (m_paImg2x2Eye,		pImg);
	FreeInfo (m_paImg2x2Hair,		pImg);
	FreeInfo (m_paImg2x2SPCloth,	pImg);
	FreeInfo (m_paImg2x2SPHair,		pImg);
	FreeInfo (m_paImg2x2Arms,		pImg);
	FreeInfo (m_paImg2x2Shield,		pImg);
	FreeInfo (m_paImg2x2ArmsSP,		pImg);
	FreeInfo (m_paImg2x2Bow,		pImg);
	FreeInfo (m_paImg2x2NPC,		pImg);
	FreeInfo (m_paImg2x2CharShadow,	pImg);
	FreeInfo (m_paImg2x2NPCShadow,	pImg);
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::Load												 */
/* 内容		:読み込み														 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

BOOL CMgrGrpData::Load(void)
{
	BOOL bResult;
	int i;
	TCHAR szName[MAX_PATH];
	char szTmp[64];
	BOOL bRet;
	PCImg32 pImg;
	LPSTR pszTmp,
			apszBodyTbl[] = {
				"IDP_BODY_BST", "IDP_BODY_BST_EAR",
				"IDP_BODY_DRK", "IDP_BODY_DRK_EAR",
				"IDP_BODY_ELF", "IDP_BODY_ELF_EAR",
				"IDP_BODY_HUM", NULL,
				NULL, NULL
				},
			apsz2x2BodyTbl[] = {
				"IDP_2X2_BODY_HUM", NULL,
				NULL, NULL
				};

	bRet = FALSE;

	/* ファイル名の作成 */
	GetModuleFileName (NULL, szName, _countof (szName));
	LPTSTR pszPath = _tcsrchr (szName, _T('\\'));
	if (pszPath != NULL) {
		pszPath[1]	= _T('\0');
	} else {
		szName[0]	= _T('\0');
	}
	_tcscat_s (szName, _countof (szName), _T("SboGrpData.dll"));

	m_hDll = LoadLibrary (szName);
	if (m_hDll == NULL) {
		goto Exit;
	}

	/* システム */
	Read256 ("IDP_SYSTEM", &m_pImgSystem, 1);
	/* アイコン */
	Read256 ("IDP_ICON", &m_pImgIcon, 1);
	/* 影画像 */
	Read256 ("IDP_SHADOW", &m_pImgShadow, 1);
	/* アイテム画像 */
	Read256 ("IDP_ITEM_01", &m_pImgItem, 1);
	/* 数字(S) */
	Read256 ("IDP_NUM_S", &m_pImgNumS, 1);
	/* 数字(M) */
	Read256 ("IDP_NUM_M", &m_pImgNumM, 1);
	/* 数字(L) */
	Read256 ("IDP_NUM_L", &m_pImgNumL, 1);

	/* マップ */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_MAP_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgMapParts->Add (pImg);
	}
	m_nMapPartsCount = i;

	/* マップ影 */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_MAPSHADOW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgMapShadow->Add (pImg);
	}
	m_nMapShadowCount = i;

	/* 体 */
	for (i = 0; ; i += 2) {
		pszTmp = apszBodyTbl[i];
		if (pszTmp == NULL) {
			break;
		}
		pImg	= new CImg32;
		bResult	= Read256 (pszTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgBody->Add (pImg);

		pImg = NULL;
		/* 耳 */
		pszTmp = apszBodyTbl[i + 1];
		if (pszTmp) {
			pImg	= new CImg32;
			bResult	= Read256 (pszTmp, &pImg, 1);
			if (bResult == FALSE) {
				delete pImg;
			}
		}
		m_paImgEar->Add (pImg);
	}
	/* 服 */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_CLOTH_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgCloth->Add (pImg);
	}
	/* 目 */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_EYE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgEye->Add (pImg);
	}
	/* 髪(下) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_HAIR_D%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgHairD->Add (pImg);
	}
	/* 髪(上) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_HAIR_U%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgHairU->Add (pImg);
	}
	/* NPC */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_NPC_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgNPC->Add (pImg);
	}
	/* 特殊服 */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_CLOTH_SP%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgSP->Add (pImg);
	}
	/* アクセサリ */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_ACCE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgAcce->Add (pImg);
	}
	/* エフェクト(32) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_EFC_32_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgEfc32->Add (pImg);
	}
	/* エフェクト(64) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_EFC_64_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgEfc64->Add (pImg);
	}
	/* 武器 */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_WEAPON_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeapon->Add (pImg);
	}
	/* 武器(弓) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_WEAPON_BOW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeaponBow->Add (pImg);
	}
	/* 武器(矢) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_WEAPON_ARROW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeaponArrow->Add (pImg);
	}
	/* 武器(打撃) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_WEAPON_GLOVE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeaponGlove->Add (pImg);
	}
	/* 武器(その他) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_WEAPON_ETC_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeaponEtc->Add (pImg);
	}
	/* 噴出し */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_BALLOON_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgEfcBalloon->Add (pImg);
	}
	/* 2x2体 */
	for (i = 0; ; i += 2) {
		pszTmp = apsz2x2BodyTbl[i];
		if (pszTmp == NULL) {
			break;
		}
		pImg	= new CImg32;
		bResult	= Read256 (pszTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Body->Add (pImg);

		pImg = NULL;
		/* 耳 */
		pszTmp = apsz2x2BodyTbl[i + 1];
		if (pszTmp) {
			pImg	= new CImg32;
			bResult	= Read256 (pszTmp, &pImg, 1);
			if (bResult == FALSE) {
				delete pImg;
			}
		}
	}
	/* 服 */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_2X2_CLOTH_%02d", i);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Cloth->Add (pImg);
	}
	/* 目 */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_2X2_EYE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Eye->Add (pImg);
	}
	/* 髪 */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_2X2_HAIR_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Hair->Add (pImg);
	}
	/* 特殊服 */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_2X2_SP_CLOTH_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2SPCloth->Add (pImg);
	}
	/* 特殊髪 */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_2X2_SP_HAIR_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2SPHair->Add (pImg);
	}
	/* 持ち物(2x2) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_2X2_ARMS_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Arms->Add (pImg);
	}
	/* 盾(2x2) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_2X2_SHIELD_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Shield->Add (pImg);
	}
	/* 特殊持ち物(2x2) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_2X2_ARMS_SP_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2ArmsSP->Add (pImg);
	}
	/* 弓(2x2) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_2X2_BOW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Bow->Add (pImg);
	}
	/* NPC(2x2) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_2X2_NPC_%03d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2NPC->Add (pImg);
	}
	/* キャラ影(2x2) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_2X2_CHAR_SHADOW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2CharShadow->Add (pImg);
	}
	/* NPC影(2x2) */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_2X2_NPC_%03d_SHADOW", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2NPCShadow->Add (pImg);
	}


	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::CheckVersion										 */
/* 内容		:バージョンチェック												 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

BOOL CMgrGrpData::CheckVersion(LPCSTR pszVersion)
{
	BOOL bRet;
	char szTmp[64];

	bRet = FALSE;

	ZeroMemory (szTmp, sizeof (szTmp));
	/* バージョンチェック */
	LoadString (m_hDll, IDS_DLLVER, szTmp, sizeof (szTmp));
	if (strcmp (szTmp, pszVersion) != 0) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::Write												 */
/* 内容		:PNGデータの保存												 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CMgrGrpData::Write(LPCSTR pszFileName, CImg32 *pSrc)
{
	FILE *fp;
	unsigned width, height;
	std::vector<unsigned char> raw;
	std::vector<unsigned char> encoded;
	unsigned error;
	int x, y;

	if (pSrc == NULL) {
		return;
	}

	width = pSrc->Width ();
	height = pSrc->Height ();
	if ((width == 0) || (height == 0)) {
		return;
	}

	fp = fopen (pszFileName, "wb");
	if (fp == NULL) {
		return;
	}

	raw.resize ((size_t)width * height * 3);
	for (y = 0; y < (int)height; y ++) {
		const BYTE *pSrcLine = pSrc->GetBits () + (height - 1 - y) * (width * 4);
		unsigned char *pDst = &raw[(size_t)y * width * 3];
		for (x = 0; x < (int)width; x ++) {
			pDst[0] = pSrcLine[2];
			pDst[1] = pSrcLine[1];
			pDst[2] = pSrcLine[0];
			pDst += 3;
			pSrcLine += 4;
		}
	}

	error = lodepng::encode (encoded, raw, width, height, LCT_RGB, 8);
	if ((error == 0) && (!encoded.empty ())) {
		fwrite (&encoded[0], 1, encoded.size (), fp);
	}

	fclose (fp);
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibSystem										 */
/* 内容		:システム画像イメージを取得										 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibSystem(void)
{
	return m_pImgSystem;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibIcon										 */
/* 内容		:アイコン画像イメージを取得										 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibIcon(void)
{
	return m_pImgIcon;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibShadow										 */
/* 内容		:影画像イメージを取得											 */
/* 日付		:2007/04/19														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibShadow(void)
{
	return m_pImgShadow;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibMapParts									 */
/* 内容		:マップパーツ画像イメージを取得									 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibMapParts(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImgMapParts->size()) {
		goto Exit;
	}

	pImg = m_paImgMapParts->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibMapShadow									 */
/* 内容		:マップパーツ画像イメージを取得									 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibMapShadow(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImgMapShadow->size()) {
		goto Exit;
	}

	pImg = m_paImgMapShadow->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibItem										 */
/* 内容		:アイテム画像イメージを取得										 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibItem(int nNo)
{
	return m_pImgItem;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibNumS										 */
/* 内容		:数字(S)画像イメージを取得										 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibNumS(void)
{
	return m_pImgNumS;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibNumM										 */
/* 内容		:数字(M)画像イメージを取得										 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibNumM(void)
{
	return m_pImgNumM;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibNumL										 */
/* 内容		:数字(L)画像イメージを取得										 */
/* 日付		:2008/09/17														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibNumL(void)
{
	return m_pImgNumL;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibNPC											 */
/* 内容		:NPC画像イメージを取得											 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibNPC(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImgNPC->size()) {
		goto Exit;
	}

	pImg = m_paImgNPC->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibBody										 */
/* 内容		:体画像イメージを取得											 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibBody(WORD wFamilyID)
{
	int nNo;
	PCImg32 pImg;

	nNo		= -1;
	pImg	= NULL;

	switch (wFamilyID) {
	case FAMILYTYPE_HUMAN:	nNo = 3;	break;	/* ニンゲン */
	case FAMILYTYPE_ELF:	nNo = 2;	break;	/* エルフ */
	case FAMILYTYPE_BST:	nNo = 0;	break;	/* ジュウジン */
	case FAMILYTYPE_DAEMON:	nNo = 1;	break;	/* マゾク */
	default:
		goto Exit;
	}
	pImg = m_paImgBody->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibEar											 */
/* 内容		:耳画像イメージを取得											 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibEar(WORD wFamilyID)
{
	int nNo;
	PCImg32 pImg;

	nNo		= -1;
	pImg	= NULL;

	switch (wFamilyID) {
//	case FAMILYTYPE_HUMAN:	nNo = 3;	break;	/* ニンゲン */
	case FAMILYTYPE_ELF:	nNo = 2;	break;	/* エルフ */
	case FAMILYTYPE_BST:	nNo = 0;	break;	/* ジュウジン */
	case FAMILYTYPE_DAEMON:	nNo = 1;	break;	/* マゾク */
	default:
		goto Exit;
	}
	pImg = m_paImgEar->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibCloth										 */
/* 内容		:服画像イメージを取得											 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibCloth(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgCloth->size()) {
		goto Exit;
	}

	pImg = m_paImgCloth->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibEye											 */
/* 内容		:目画像イメージを取得											 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibEye(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgEye->size()) {
		goto Exit;
	}

	pImg = m_paImgEye->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibHairD										 */
/* 内容		:髪(下)画像イメージを取得										 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibHairD(WORD wHairColorID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wHairColorID >= m_paImgHairD->size()) {
		goto Exit;
	}

	pImg = m_paImgHairD->at(wHairColorID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibHairU										 */
/* 内容		:髪(上)画像イメージを取得										 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibHairU(WORD wHairColorID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wHairColorID >= m_paImgHairU->size()) {
		goto Exit;
	}

	pImg = m_paImgHairU->at(wHairColorID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibSP											 */
/* 内容		:特殊服画像イメージを取得										 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibSP(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgSP->size()) {
		goto Exit;
	}

	pImg = m_paImgSP->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibAcce										 */
/* 内容		:アクセサリ画像イメージを取得									 */
/* 日付		:2007/05/04														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibAcce(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgAcce->size()) {
		goto Exit;
	}

	pImg = m_paImgAcce->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibEfc32										 */
/* 内容		:エフェクト(32)画像イメージを取得								 */
/* 日付		:2008/08/02														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibEfc32(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgEfc32->size()) {
		goto Exit;
	}

	pImg = m_paImgEfc32->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibEfc(64)										 */
/* 内容		:エフェクト(64)画像イメージを取得								 */
/* 日付		:2008/08/02														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibEfc64(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgEfc64->size()) {
		goto Exit;
	}

	pImg = m_paImgEfc64->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibWeapon										 */
/* 内容		:武器画像イメージを取得											 */
/* 日付		:2007/07/09														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibWeapon(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgWeapon->size()) {
		goto Exit;
	}

	pImg = m_paImgWeapon->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibWeaponBow									 */
/* 内容		:武器(弓)画像イメージを取得										 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibWeaponBow(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgWeaponBow->size()) {
		goto Exit;
	}

	pImg = m_paImgWeaponBow->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibWeaponArrow									 */
/* 内容		:武器(矢)画像イメージを取得										 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibWeaponArrow(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgWeaponArrow->size()) {
		goto Exit;
	}

	pImg = m_paImgWeaponArrow->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibWeaponGlove									 */
/* 内容		:武器(打撃)画像イメージを取得									 */
/* 日付		:2007/12/02														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibWeaponGlove(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgWeaponGlove->size()) {
		goto Exit;
	}

	pImg = m_paImgWeaponGlove->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibWeaponEtc									 */
/* 内容		:武器(その他)画像イメージを取得									 */
/* 日付		:2007/12/02														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibWeaponEtc(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgWeaponEtc->size()) {
		goto Exit;
	}

	pImg = m_paImgWeaponEtc->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibEfcBalloon									 */
/* 内容		:噴出し画像イメージを取得										 */
/* 日付		:2007/12/26														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibEfcBalloon(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgEfcBalloon->size()) {
		goto Exit;
	}

	pImg = m_paImgEfcBalloon->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDib2x2Body										 */
/* 内容		:2x2体画像イメージを取得										 */
/* 日付		:2008/06/14														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2Body(WORD wFamilyID)
{
	int nNo;
	PCImg32 pImg;

	nNo		= -1;
	pImg	= NULL;

#if 0
	switch (wFamilyID) {
	case FAMILYTYPE_HUMAN:	nNo = 3;	break;	/* ニンゲン */
	case FAMILYTYPE_ELF:	nNo = 2;	break;	/* エルフ */
	case FAMILYTYPE_BST:	nNo = 0;	break;	/* ジュウジン */
	case FAMILYTYPE_DAEMON:	nNo = 1;	break;	/* マゾク */
	default:
		goto Exit;
	}
#endif
	pImg = m_paImg2x2Body->at(0);

//Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDib2x2Cloth									 */
/* 内容		:服画像イメージを取得											 */
/* 日付		:2008/06/17														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2Cloth(WORD wClothID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wClothID == 0) {
		goto Exit;
	}
	if (wClothID >= m_paImg2x2Cloth->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2Cloth->at(wClothID);

Exit:
	if (pImg == NULL) {
		pImg = m_paImg2x2Cloth->at(0);
	}
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDib2x2Eye										 */
/* 内容		:目画像イメージを取得											 */
/* 日付		:2008/06/17														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2Eye(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImg2x2Eye->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2Eye->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDib2x2Hair										 */
/* 内容		:髪画像イメージを取得											 */
/* 日付		:2008/06/17														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2Hair(WORD wHairColorID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wHairColorID == 0) {
		goto Exit;
	}
	wHairColorID --;
	if (wHairColorID >= m_paImg2x2Hair->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2Hair->at(wHairColorID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDib2x2SPCloth									 */
/* 内容		:特殊服画像イメージを取得										 */
/* 日付		:2008/06/20														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2SPCloth(WORD wSPClothID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wSPClothID == 0) {
		goto Exit;
	}
	wSPClothID --;
	if (wSPClothID >= m_paImg2x2SPCloth->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2SPCloth->at(wSPClothID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDib2x2SPHair									 */
/* 内容		:特殊髪画像イメージを取得										 */
/* 日付		:2008/06/20														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2SPHair(WORD wSPHairID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wSPHairID == 0) {
		goto Exit;
	}
	wSPHairID --;
	if (wSPHairID >= m_paImg2x2SPHair->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2SPHair->at(wSPHairID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDib2x2Arms										 */
/* 内容		:2x2持ち物画像イメージを取得									 */
/* 日付		:2008/06/21														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2Arms(WORD wArmsID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wArmsID >= m_paImg2x2Arms->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2Arms->at(wArmsID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDib2x2Shield									 */
/* 内容		:2x2盾画像イメージを取得										 */
/* 日付		:2008/06/21														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2Shield(WORD wShieldID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wShieldID >= m_paImg2x2Shield->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2Shield->at(wShieldID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDib2x2ArmsSP									 */
/* 内容		:2x2特殊持ち物画像イメージを取得								 */
/* 日付		:2008/06/30														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2ArmsSP(WORD wArmsID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wArmsID >= m_paImg2x2ArmsSP->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2ArmsSP->at(wArmsID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDib2x2Bow										 */
/* 内容		:2x2弓画像イメージを取得										 */
/* 日付		:2008/07/02														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2Bow(WORD wBowID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wBowID >= m_paImg2x2Bow->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2Bow->at(wBowID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDib2x2NPC										 */
/* 内容		:2x2NPC画像イメージを取得										 */
/* 日付		:2008/07/05														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2NPC(WORD wNPCID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wNPCID >= m_paImg2x2NPC->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2NPC->at(wNPCID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDib2x2CharShadow								 */
/* 内容		:2x2キャラ影画像イメージを取得									 */
/* 日付		:2008/08/02														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2CharShadow(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImg2x2CharShadow->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2CharShadow->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDib2x2NPCShadow								 */
/* 内容		:2x2NPC影画像イメージを取得										 */
/* 日付		:2008/08/24														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2NPCShadow(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImg2x2NPCShadow->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2NPCShadow->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDib											 */
/* 内容		:画像イメージを取得												 */
/* 日付		:2007/11/17														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib(DWORD dwGrpIDMain, DWORD dwGrpIDSub, DWORD dwParam)
{
	PCImg32 pRet;

	pRet = NULL;

	switch (dwGrpIDMain) {
	case GRPIDMAIN_CHAR:			/* キャラ画像 */
		pRet = GetDibBody ((WORD)dwParam);
		break;
	case GRPIDMAIN_WEAPON:			/* 武器 */
		pRet = GetDibWeapon (0);
		break;
	case GRPIDMAIN_WEAPON_BOW:		/* 弓 */
		pRet = GetDibWeaponBow (0);
		break;
	case GRPIDMAIN_EFFECT32:		/* エフェクト(32) */
		pRet = GetDibEfc32 (0);
		break;
	case GRPIDMAIN_WEAPON_GLOVE:	/* 武器(打撃) */
		pRet = GetDibWeaponGlove (0);
		break;
	case GRPIDMAIN_WEAPON_ETC:		/* 武器(その他) */
		pRet = GetDibWeaponEtc (0);
		break;
	case GRPIDMAIN_NPC:				/* キャラ(NPC) */
		pRet = GetDibNPC (0);
		break;
	case GRPIDMAIN_EFCBALLOON:		/* 噴出し */
		pRet = GetDibEfcBalloon (0);
		break;
	case GRPIDMAIN_2X2_CHAR:		/* キャラ(32x32)画像 */
		pRet = GetDib2x2Body ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_CLOTH:		/* 服(32x32) */
		pRet = GetDib2x2Cloth ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_EYE:			/* 目(32x32) */
		pRet = GetDib2x2Eye ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_HAIR:		/* 髪(32x32) */
		if (dwParam < 10000) {
			pRet = GetDib2x2Hair ((WORD)dwParam);
		} else {
			pRet = GetDib2x2SPHair ((WORD)dwParam - 10000);
		}
		break;
	case GRPIDMAIN_2X2_SPCLOTH:		/* 特殊服(32x32) */
		pRet = GetDib2x2SPCloth ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_SPHAIR:		/* 特殊髪(32x32) */
		pRet = GetDib2x2SPHair ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_ARMS:		/* 持ち物(2x2) */
		dwParam = dwGrpIDSub / GetGrpCountX (dwGrpIDMain);
		dwParam = dwParam / GetGrpCountY (dwGrpIDMain);
		pRet = GetDib2x2Arms ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_SHIELD:		/* 盾(2x2) */
		pRet = GetDib2x2Shield ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_ARMSSP:		/* 特殊持ち物(2x2) */
		pRet = GetDib2x2ArmsSP ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_BOW:			/* 弓(2x2) */
		pRet = GetDib2x2Bow ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_NPC:			/* NPC(2x2) */
		pRet = GetDib2x2NPC ((WORD)dwParam);
		break;
	case GRPIDMAIN_EFFECT64:		/* エフェクト(64) */
		pRet = GetDibEfc64 (0);
		break;
	case GRPIDMAIN_ICON32:			/* アイコン(２倍表示) */
		pRet = GetDibIcon ();
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibTmpLogo										 */
/* 内容		:URARA-worksロゴ画像イメージを取得								 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibTmpLogo(void)
{
	BOOL bResult;
	PCImg32 pImg;

	pImg = new CImg32;
	bResult	= Read256 ("IDP_LOGO", &pImg, 1);
	if (bResult == FALSE) {
		SAFE_DELETE (pImg);
	} else {
		pImg->SetColorKeyNo (255);
	}

	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibTmpTitle									 */
/* 内容		:タイトル画像イメージを取得										 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibTmpTitle(void)
{
	BOOL bResult;
	PCImg32 pImg;

	pImg	= new CImg32;
	bResult	= Read256 ("IDP_TITLE", &pImg, 1);
	if (bResult == FALSE) {
		SAFE_DELETE (pImg);
	} else {
		pImg->SetColorKeyNo (71);
	}

	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibTmpTitleBack								 */
/* 内容		:タイトル背景画像イメージを取得									 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibTmpTitleBack(void)
{
	BOOL bResult;
	PCImg32 pImg;

	pImg	= new CImg32;
	bResult	= Read256 ("IDP_TITLE_BACK", &pImg, 1);
	if (bResult == FALSE) {
		SAFE_DELETE (pImg);
	}

	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibTmpTitleCloud								 */
/* 内容		:タイトル雲画像イメージを取得									 */
/* 日付		:2008/05/02														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibTmpTitleCloud(void)
{
	BOOL bResult;
	PCImg32 pImg;

	pImg	= new CImg32;
	bResult	= Read256 ("IDP_TITLE_CLOUD", &pImg, 1);
	if (bResult == FALSE) {
		SAFE_DELETE (pImg);
	}

	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibTmpLoginMenuBack							 */
/* 内容		:ログインメニュー背景画像イメージを取得							 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibTmpLoginMenuBack(void)
{
	BOOL bResult;
	PCImg32 pImg;

	pImg	= new CImg32;
	bResult	= Read ("IDP_CHARSELECT_BACK", &pImg, 2);
	if (bResult == FALSE) {
		SAFE_DELETE (pImg);
	}

	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetMapPartsCount									 */
/* 内容		:マップパーツ数を取得											 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

int CMgrGrpData::GetMapPartsCount(void)
{
	return m_nMapPartsCount * 1024;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibMapPartsTmp									 */
/* 内容		:等倍マップパーツ画像イメージを取得								 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibMapPartsTmp(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImgMapPartsTmp->size()) {
		goto Exit;
	}

	pImg = m_paImgMapPartsTmp->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::ReadMapPartsTmp									 */
/* 内容		:等倍マップパーツ画像を読み込み									 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

void CMgrGrpData::ReadMapPartsTmp(void)
{
	int i;
	BOOL bResult;
	char szTmp[128];
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo (m_paImgMapPartsTmp, pImg);
	/* マップ */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_MAP_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgMapPartsTmp->Add (pImg);
	}
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::DeleteMapPartsTmp									 */
/* 内容		:等倍マップパーツ画像を削除										 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

void CMgrGrpData::DeleteMapPartsTmp(void)
{
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo (m_paImgMapPartsTmp, pImg);
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetMapShadowCount									 */
/* 内容		:マップ影画像数を取得											 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

int CMgrGrpData::GetMapShadowCount(void)
{
	return m_nMapShadowCount * 1024;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibMapShadowTmp								 */
/* 内容		:等倍マップ影画像イメージを取得									 */
/* 日付		:2007/06/03														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibMapShadowTmp(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImgMapShadowTmp->size()) {
		goto Exit;
	}

	pImg = m_paImgMapShadowTmp->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::ReadMapShadowTmp									 */
/* 内容		:等倍マップ影画像を読み込み										 */
/* 日付		:2007/06/03														 */
/* ========================================================================= */

void CMgrGrpData::ReadMapShadowTmp(void)
{
	int i;
	BOOL bResult;
	char szTmp[128];
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo (m_paImgMapShadowTmp, pImg);
	/* マップ */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_MAPSHADOW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgMapShadowTmp->Add (pImg);
	}
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::DeleteMapShadowTmp								 */
/* 内容		:等倍マップ影画像を削除											 */
/* 日付		:2007/06/03														 */
/* ========================================================================= */

void CMgrGrpData::DeleteMapShadowTmp(void)
{
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo (m_paImgMapShadowTmp, pImg);
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetItemCount										 */
/* 内容		:アイテム画像数を取得											 */
/* 日付		:2007/08/22														 */
/* ========================================================================= */

int CMgrGrpData::GetItemCount(void)
{
	return 1024;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetDibItemTmp										 */
/* 内容		:等倍アイテム画像イメージを取得									 */
/* 日付		:2007/08/12														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibItemTmp(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImgItemTmp->size()) {
		goto Exit;
	}

	pImg = m_paImgItemTmp->at(nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::ReadItemTmp										 */
/* 内容		:等倍アイテム画像を読み込み										 */
/* 日付		:2007/08/12														 */
/* ========================================================================= */

void CMgrGrpData::ReadItemTmp(void)
{
	int i;
	BOOL bResult;
	char szTmp[128];
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo (m_paImgItemTmp, pImg);
	/* アイテム */
	for (i = 0; ; i ++) {
		sprintf_s (szTmp, sizeof (szTmp), "IDP_ITEM_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgItemTmp->Add (pImg);
	}
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::DeleteItemTmp										 */
/* 内容		:等倍アイテム画像を削除											 */
/* 日付		:2007/08/12														 */
/* ========================================================================= */

void CMgrGrpData::DeleteItemTmp(void)
{
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo (m_paImgItemTmp, pImg);
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetGrpSize										 */
/* 内容		:画像１つあたりのサイズを取得									 */
/* 日付		:2007/11/17														 */
/* ========================================================================= */

int CMgrGrpData::GetGrpSize(DWORD dwGrpIDMain)
{
	int nRet;

	nRet = 0;

	switch (dwGrpIDMain) {
	case GRPIDMAIN_CHAR:			/* キャラ画像 */
	case GRPIDMAIN_NPC:				/* キャラ(NPC) */
	case GRPIDMAIN_EFCBALLOON:		/* 噴出し */
	case GRPIDMAIN_2X2_SHIELD:		/* 盾(2x2) */
	case GRPIDMAIN_ICON32:			/* アイコン(２倍表示) */
		nRet = 16;
		break;
	case GRPIDMAIN_WEAPON:			/* 武器 */
	case GRPIDMAIN_WEAPON_BOW:		/* 弓 */
	case GRPIDMAIN_EFFECT32:		/* エフェクト(32) */
	case GRPIDMAIN_WEAPON_GLOVE:	/* 武器(打撃) */
	case GRPIDMAIN_WEAPON_ETC:		/* 武器(その他) */
	case GRPIDMAIN_2X2_CHAR:		/* キャラ(32x32)画像 */
	case GRPIDMAIN_2X2_CLOTH:		/* 服(32x32) */
	case GRPIDMAIN_2X2_EYE:			/* 目(32x32) */
	case GRPIDMAIN_2X2_HAIR:		/* 髪(32x32) */
	case GRPIDMAIN_2X2_SPCLOTH:		/* 特殊服(32x32) */
	case GRPIDMAIN_2X2_SPHAIR:		/* 特殊髪(32x32) */
	case GRPIDMAIN_2X2_NPC:			/* NPC(2x2) */
		nRet = 32;
		break;
	case GRPIDMAIN_2X2_ARMS:		/* 持ち物(2x2) */
	case GRPIDMAIN_2X2_ARMSSP:		/* 特殊持ち物(2x2) */
	case GRPIDMAIN_2X2_BOW:			/* 弓(2x2) */
		nRet = 24;
		break;
	case GRPIDMAIN_EFFECT64:		/* エフェクト(64) */
		nRet = 64;
		break;
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetGrpCountX										 */
/* 内容		:画像１ファイルの横個数を取得									 */
/* 日付		:2007/11/17														 */
/* ========================================================================= */

int CMgrGrpData::GetGrpCountX(DWORD dwGrpIDMain)
{
	int nRet;

	nRet = 0;

	switch (dwGrpIDMain) {
	case GRPIDMAIN_CHAR:			/* キャラ画像 */
		nRet = 32;
		break;
	case GRPIDMAIN_NPC:				/* キャラ(NPC) */
		nRet = 16;
		break;
	case GRPIDMAIN_WEAPON:			/* 武器 */
		nRet = 32;
		break;
	case GRPIDMAIN_WEAPON_BOW:		/* 弓 */
	case GRPIDMAIN_WEAPON_GLOVE:	/* 武器(打撃) */
	case GRPIDMAIN_WEAPON_ETC:		/* 武器(その他) */
	case GRPIDMAIN_ICON32:			/* アイコン(２倍表示) */
		nRet = 20;
		break;
	case GRPIDMAIN_EFFECT32:		/* エフェクト(32) */
		nRet = 16;
		break;
	case GRPIDMAIN_EFCBALLOON:		/* 噴出し */
		nRet = 10;
		break;
	case GRPIDMAIN_2X2_CHAR:		/* キャラ(32x32)画像 */
	case GRPIDMAIN_2X2_CLOTH:		/* 服(32x32) */
	case GRPIDMAIN_2X2_HAIR:		/* 髪(32x32) */
	case GRPIDMAIN_2X2_SPCLOTH:		/* 特殊服(32x32) */
	case GRPIDMAIN_2X2_SPHAIR:		/* 特殊髪(32x32) */
	case GRPIDMAIN_2X2_NPC:			/* NPC(2x2) */
		nRet = 16;
		break;
	case GRPIDMAIN_2X2_EYE:			/* 目(32x32) */
	case GRPIDMAIN_2X2_ARMS:		/* 持ち物(2x2) */
		nRet = 12;
		break;
	case GRPIDMAIN_2X2_ARMSSP:		/* 特殊持ち物(2x2) */
	case GRPIDMAIN_2X2_BOW:			/* 弓(2x2) */
		nRet = 11;
		break;
	case GRPIDMAIN_2X2_SHIELD:		/* 盾(2x2) */
		nRet = 5;
		break;
	case GRPIDMAIN_EFFECT64:		/* エフェクト(64) */
		nRet = 8;
		break;
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetGrpCountY										 */
/* 内容		:画像１ファイルの縦個数を取得									 */
/* 日付		:2007/11/24														 */
/* ========================================================================= */

int CMgrGrpData::GetGrpCountY(DWORD dwGrpIDMain)
{
	int nRet;

	nRet = 0;

	switch (dwGrpIDMain) {
	case GRPIDMAIN_CHAR:			/* キャラ画像 */
		nRet = 1;
		break;
	case GRPIDMAIN_WEAPON:			/* 武器 */
	case GRPIDMAIN_WEAPON_BOW:		/* 弓 */
	case GRPIDMAIN_WEAPON_GLOVE:	/* 武器(打撃) */
	case GRPIDMAIN_WEAPON_ETC:		/* 武器(その他) */
		nRet = 18;
		break;
	case GRPIDMAIN_EFFECT32:		/* エフェクト(32) */
		nRet = 16;
		break;
	case GRPIDMAIN_NPC:				/* キャラ(NPC) */
		nRet = 32;
		break;
	case GRPIDMAIN_EFCBALLOON:		/* 噴出し */
		nRet = 15;
		break;
	case GRPIDMAIN_2X2_CHAR:		/* キャラ(32x32)画像 */
	case GRPIDMAIN_2X2_CLOTH:		/* 服(32x32) */
	case GRPIDMAIN_2X2_EYE:			/* 目(32x32) */
	case GRPIDMAIN_2X2_HAIR:		/* 髪(32x32) */
	case GRPIDMAIN_2X2_SPCLOTH:		/* 特殊服(32x32) */
	case GRPIDMAIN_2X2_SPHAIR:		/* 特殊髪(32x32) */
	case GRPIDMAIN_2X2_NPC:			/* NPC(2x2) */
		nRet = 8;
		break;
	case GRPIDMAIN_2X2_ARMS:		/* 持ち物(2x2) */
	case GRPIDMAIN_2X2_SHIELD:		/* 盾(2x2) */
	case GRPIDMAIN_2X2_ARMSSP:		/* 特殊持ち物(2x2) */
	case GRPIDMAIN_2X2_BOW:			/* 弓(2x2) */
		nRet = 25;
		break;
	case GRPIDMAIN_EFFECT64:		/* エフェクト(64) */
		nRet = 8;
		break;
	case GRPIDMAIN_ICON32:			/* アイコン(２倍表示) */
		nRet = 20;
		break;
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CMgrGrpData::GetGrpPos											 */
/* 内容		:指定画像の位置を取得											 */
/* 日付		:2007/11/24														 */
/* ========================================================================= */

void CMgrGrpData::GetGrpPos(
	DWORD dwGrpIDMain,		/* [in] 画像IDメイン */
	DWORD dwGrpIDSub,		/* [in] 画像IDサブ */
	POINT &ptDst,			/* [ou] 画像位置 */
	WORD wGrpID)			/* [in] 縦位置をずらす個数 */
{
	int nCountX, nCountY, nSize;
	DWORD dwGrpIDSubTmp;

	ptDst.x = ptDst.y = 0;
	if (dwGrpIDSub == 0) {
		return;
	}
	if (dwGrpIDSub == -1) {
		return;
	}

	nCountX	= GetGrpCountX (dwGrpIDMain);
	nCountY	= GetGrpCountY (dwGrpIDMain);
	nSize	= GetGrpSize (dwGrpIDMain);

	if (wGrpID != 0) {
		dwGrpIDSub = dwGrpIDSub % nCountX;
		dwGrpIDSub += (wGrpID * nCountX);
	}
	dwGrpIDSubTmp = dwGrpIDSub / nCountX;
	if (dwGrpIDSubTmp >= (DWORD)nCountY) {
		dwGrpIDSubTmp = dwGrpIDSubTmp % nCountY;
	}

	ptDst.x = (dwGrpIDSub % nCountX) * nSize;
	ptDst.y = dwGrpIDSubTmp * nSize;
}


/* ========================================================================= */
/* 関数名       :Read                                                                     */
/* 内容         :PNGデータの読み込み                                                         */
/* 日付         :2006/09/24                                                                 */
/* ========================================================================= */

BOOL CMgrGrpData::Read(LPSTR pszName, PCImg32 *pDib, int nSize)
{
        BOOL bRet;
        int x, y;
        HRSRC hResInfo;
        HGLOBAL hRes;
        DWORD dwResourceSize;
        const BYTE *pResourceData;
        std::vector<unsigned char> image;
        unsigned width, height;
        CImg32 *pDibNew;
        CImg32 *pDibTmp, *pDibTmp2;
        BYTE *pData;
        HDC hDCBmp, hDCBmp2;

        bRet            = FALSE;
        pDibTmp         = NULL;
        pDibTmp2        = NULL;

        hResInfo = FindResource (m_hDll, pszName, "PNG");
        if (hResInfo == NULL) {
                goto Exit;
        }
        hRes = LoadResource (m_hDll, hResInfo);
        if (hRes == NULL) {
                goto Exit;
        }

        dwResourceSize = SizeofResource (m_hDll, hResInfo);
        pResourceData = (const BYTE *)LockResource (hRes);
        if ((pResourceData == NULL) || (dwResourceSize == 0)) {
                goto Exit;
        }

        if (lodepng::decode (image, width, height, pResourceData, dwResourceSize)) {
                goto Exit;
        }
        if ((width == 0) || (height == 0)) {
                goto Exit;
        }

        if (*pDib == NULL) {
                *pDib = new CImg32;
        }
        pDibNew = *pDib;
        pDibTmp = new CImg32;

        pDibTmp->Create (width, height);
        pDibNew->CreateWithoutGdi (width * nSize, height * nSize);
        pData = pDibTmp->GetBits ();

        for (y = 0; y < (int)height; y ++) {
                BYTE *pDst = pData + (height - y - 1) * width * 4;
                const unsigned char *pSrc = &image[(size_t)y * width * 4];
                for (x = 0; x < (int)width; x ++) {
                        pDst[0] = pSrc[2];
                        pDst[1] = pSrc[1];
                        pDst[2] = pSrc[0];
                        pDst[3] = pSrc[3];
                        pDst += 4;
                        pSrc += 4;
                }
        }

        pDibTmp2 = new CImg32;
        pDibTmp2->Create (width * nSize, height * nSize);
        hDCBmp  = pDibTmp->Lock ();
        hDCBmp2 = pDibTmp2->Lock ();

        StretchBlt (hDCBmp2, 0, 0, width * nSize, height * nSize,
                        hDCBmp, 0, 0, width, height, SRCCOPY);

        pDibTmp->Unlock ();
        pDibTmp2->Unlock ();

        pDibNew->Blt (0, 0, width * nSize, height * nSize, pDibTmp2, 0, 0);

        bRet = TRUE;
Exit:
        SAFE_DELETE (pDibTmp);
        SAFE_DELETE (pDibTmp2);
        return bRet;
}


/* ========================================================================= */
/* 関数名       :Read256                                                                  */
/* 内容         :PNGデータの読み込み                                                         */
/* 日付         :2008/07/04                                                                 */
/* ========================================================================= */

BOOL CMgrGrpData::Read256(LPSTR pszName, PCImg32 *pDib, int nSize)
{
        BOOL bRet;
        int i, y;
        HRSRC hResInfo;
        HGLOBAL hRes;
        DWORD dwResourceSize;
        const BYTE *pResourceData;
        std::vector<unsigned char> image;
        unsigned width, height;
        lodepng::State state;
        CImg32 *pDibNew;
        BYTE *pData;

        bRet = FALSE;
        state.decoder.color_convert = 0;
        state.info_raw.colortype = LCT_PALETTE;
        state.info_raw.bitdepth = 8;

        hResInfo = FindResource (m_hDll, pszName, "PNG");
        if (hResInfo == NULL) {
                goto Exit;
        }
        hRes = LoadResource (m_hDll, hResInfo);
        if (hRes == NULL) {
                goto Exit;
        }

        dwResourceSize = SizeofResource (m_hDll, hResInfo);
        pResourceData = (const BYTE *)LockResource (hRes);
        if ((pResourceData == NULL) || (dwResourceSize == 0)) {
                goto Exit;
        }

        if (lodepng::decode (image, width, height, state, pResourceData, dwResourceSize)) {
                goto Exit;
        }
        if ((width == 0) || (height == 0)) {
                goto Exit;
        }
        if (state.info_png.color.colortype != LCT_PALETTE) {
                goto Exit;
        }
        if ((state.info_png.color.palettesize == 0) || (state.info_png.color.palettesize > 256)) {
                goto Exit;
        }
        if (image.size () != (size_t)width * height) {
                goto Exit;
        }

        if (*pDib == NULL) {
                *pDib = new CImg32;
        }
        pDibNew = *pDib;
        pDibNew->CreateWithoutGdi256 (width * nSize, height * nSize);
        pData = pDibNew->GetBits ();

        for (i = 0; i < (int)state.info_png.color.palettesize; i ++) {
                const unsigned char *pEntry = &state.info_png.color.palette[i * 4];
                pDibNew->SetPallet (i, pEntry[0], pEntry[1], pEntry[2]);
        }

        for (y = 0; y < (int)height; y ++) {
                BYTE *pDst = pData + (height - y - 1) * width;
                CopyMemory (pDst, &image[(size_t)y * width], width);
        }

        bRet = TRUE;
Exit:
        return bRet;
}

/* Copyright(C)URARA-works 2006 */
