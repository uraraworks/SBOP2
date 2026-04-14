/// @file MgrGrpData.cpp
/// @brief グラフィックデータマネージャクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#if !defined(_WINDLL)
#include <SDL.h>
#endif
#include "../../SboGrpData/resource.h"
#include "Img32.h"
#include "third_party/lodepng.h"
#include "MgrGrpData.h"

#include <vector>
#include <cstdio>

#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>

EM_JS(void, SBOP2_DebugMarkGrpLoad, (
	int foundBase,
	int systemExists,
	int systemRead,
	int titleExists,
	int titleBackExists,
	int titleCloudExists
), {
	Module.sbop2GrpFoundBase = foundBase;
	Module.sbop2GrpSystemExists = systemExists;
	Module.sbop2GrpSystemRead = systemRead;
	Module.sbop2GrpTitleExists = titleExists;
	Module.sbop2GrpTitleBackExists = titleBackExists;
	Module.sbop2GrpTitleCloudExists = titleCloudExists;
	Module.sbop2GrpDiagCount = (Module.sbop2GrpDiagCount || 0) + 1;
	Module.sbop2Stage = 8;
	});

EM_JS(void, SBOP2_DebugMarkCloudLoad, (int loaded, int w, int h), {
	Module.sbop2CloudLoaded = loaded;
	Module.sbop2CloudW = w;
	Module.sbop2CloudH = h;
	});
#endif


static BOOL ReadBinaryFile(LPCTSTR pszFilePath, std::vector<unsigned char> &data)
{
	FILE *fp;
	long nSize;

#ifdef UNICODE
	if (_tfopen_s(&fp, pszFilePath, L"rb") != 0) {
		fp = NULL;
	}
#else
	fp = fopen(pszFilePath, "rb");
#endif
	if (fp == NULL) {
		return FALSE;
	}

	if (fseek(fp, 0, SEEK_END) != 0) {
		fclose(fp);
		return FALSE;
	}
	nSize = ftell(fp);
	if (nSize <= 0) {
		fclose(fp);
		return FALSE;
	}
	if (fseek(fp, 0, SEEK_SET) != 0) {
		fclose(fp);
		return FALSE;
	}

	data.resize((size_t)nSize);
	if (fread(&data[0], 1, (size_t)nSize, fp) != (size_t)nSize) {
		fclose(fp);
		data.clear();
		return FALSE;
	}

	fclose(fp);
	return TRUE;
}

static BOOL FindSboGrpResBasePath(TCHAR *pszDst, size_t nDstCount)
{
	TCHAR szBasePath[MAX_PATH];
	LPCTSTR apszBaseCandidates[] = {
		_T(".\\SboGrpData\\res\\"),
		_T("..\\SboGrpData\\res\\"),
		_T("..\\..\\SboGrpData\\res\\"),
		NULL
	};
	int i;

#if !defined(_WIN32)
	if ((pszDst != NULL) && (nDstCount > 0)) {
		_tcscpy_s(pszDst, nDstCount, _T("/SboGrpData/res/"));
		if (PathFileExists(_T("/SboGrpData/res/system.png"))) {
			return TRUE;
		}
		pszDst[0] = _T('\0');
	}
#endif

	for (i = 0; apszBaseCandidates[i] != NULL; i++) {
		GetModuleFilePath(szBasePath, _countof(szBasePath));
		_tcscat_s(szBasePath, _countof(szBasePath), apszBaseCandidates[i]);

		TCHAR szCheckPath[MAX_PATH];
		_tcscpy_s(szCheckPath, _countof(szCheckPath), szBasePath);
		_tcscat_s(szCheckPath, _countof(szCheckPath), _T("system.png"));
		if (PathFileExists(szCheckPath)) {
			_tcscpy_s(pszDst, nDstCount, szBasePath);
			return TRUE;
		}
	}

	if (nDstCount > 0) {
		pszDst[0] = _T('\0');
	}
	return FALSE;
}

static void BuildSboGrpResPath(TCHAR *pszDst, size_t nDstCount, LPCTSTR pszBasePath, LPCTSTR pszFileName)
{
	if ((pszDst == NULL) || (nDstCount == 0)) {
		return;
	}

	pszDst[0] = _T('\0');
	if ((pszBasePath == NULL) || (pszFileName == NULL)) {
		return;
	}

	_tcscpy_s(pszDst, nDstCount, pszBasePath);
	_tcscat_s(pszDst, nDstCount, pszFileName);

#if !defined(_WIN32)
	// Emscripten VFS（および Linux）はバックスラッシュをパス区切りとして解釈しないため
	// GetFileNameForResource が返す Windows 形式パスをスラッシュに変換する
	for (TCHAR *p = pszDst; *p != _T('\0'); p++) {
		if (*p == _T('\\')) {
			*p = _T('/');
		}
	}
#endif
}

/// リソース名（ANSI）から対応する相対ファイル名（TCHAR）を取得
/// @return ファイル名が見つかればTRUE
static BOOL GetFileNameForResource(LPCSTR pszResName, TCHAR *pszDst, size_t nDstCount)
{
	// SboGrpData.rc のリソース定義に基づくマッピング
	static const struct { const char* resName; LPCTSTR fileName; } s_map[] = {
		{ "IDP_MAP_01",             _T("map01.png") },
		{ "IDP_MAP_02",             _T("map02.png") },
		{ "IDP_MAP_03",             _T("map03.png") },
		{ "IDP_SYSTEM",             _T("system.png") },
		{ "IDP_LOGO",               _T("URARA-works-logo.png") },
		{ "IDP_TITLE_BACK",         _T("title_back.png") },
		{ "IDP_TITLE",              _T("title.png") },
		{ "IDP_BODY_HUM",           _T("body\\hum.png") },
		{ "IDP_BODY_BST",           _T("body\\bst.png") },
		{ "IDP_BODY_BST_EAR",       _T("body\\bst_ear.png") },
		{ "IDP_BODY_DRK",           _T("body\\drk.png") },
		{ "IDP_BODY_DRK_EAR",       _T("body\\drk_ear.png") },
		{ "IDP_BODY_ELF",           _T("body\\elf.png") },
		{ "IDP_BODY_ELF_EAR",       _T("body\\elf_ear.png") },
		{ "IDP_HAIR_D01",           _T("hair\\hair_d01.png") },
		{ "IDP_HAIR_D02",           _T("hair\\hair_d02.png") },
		{ "IDP_HAIR_D03",           _T("hair\\hair_d03.png") },
		{ "IDP_HAIR_D04",           _T("hair\\hair_d04.png") },
		{ "IDP_HAIR_D05",           _T("hair\\hair_d05.png") },
		{ "IDP_HAIR_D06",           _T("hair\\hair_d06.png") },
		{ "IDP_HAIR_D07",           _T("hair\\hair_d07.png") },
		{ "IDP_HAIR_D08",           _T("hair\\hair_d08.png") },
		{ "IDP_HAIR_D09",           _T("hair\\hair_d09.png") },
		{ "IDP_HAIR_D10",           _T("hair\\hair_d10.png") },
		{ "IDP_HAIR_D11",           _T("hair\\hair_d11.png") },
		{ "IDP_HAIR_D12",           _T("hair\\hair_d12.png") },
		{ "IDP_HAIR_D13",           _T("hair\\hair_d13.png") },
		{ "IDP_HAIR_U01",           _T("hair\\hair_u01.png") },
		{ "IDP_HAIR_U02",           _T("hair\\hair_u02.png") },
		{ "IDP_HAIR_U03",           _T("hair\\hair_u03.png") },
		{ "IDP_HAIR_U04",           _T("hair\\hair_u04.png") },
		{ "IDP_HAIR_U05",           _T("hair\\hair_u05.png") },
		{ "IDP_HAIR_U06",           _T("hair\\hair_u06.png") },
		{ "IDP_HAIR_U07",           _T("hair\\hair_u07.png") },
		{ "IDP_HAIR_U08",           _T("hair\\hair_u08.png") },
		{ "IDP_HAIR_U09",           _T("hair\\hair_u09.png") },
		{ "IDP_HAIR_U10",           _T("hair\\hair_u10.png") },
		{ "IDP_HAIR_U11",           _T("hair\\hair_u11.png") },
		{ "IDP_HAIR_U12",           _T("hair\\hair_u12.png") },
		{ "IDP_HAIR_U13",           _T("hair\\hair_u13.png") },
		{ "IDP_CLOTH_01",           _T("clothes\\cloth01.png") },
		{ "IDP_CLOTH_02",           _T("clothes\\cloth02.png") },
		{ "IDP_CLOTH_SP01",         _T("clothes\\sp01.png") },
		{ "IDP_EYE_01",             _T("eyes\\eye01.png") },
		{ "IDP_EYE_02",             _T("eyes\\eye02.png") },
		{ "IDP_EYE_03",             _T("eyes\\eye03.png") },
		{ "IDP_EYE_04",             _T("eyes\\eye04.png") },
		{ "IDP_EYE_05",             _T("eyes\\eye05.png") },
		{ "IDP_EYE_06",             _T("eyes\\eye06.png") },
		{ "IDP_CHARSELECT_BACK",    _T("char_back.png") },
		{ "IDP_SHADOW",             _T("shadow.png") },
		{ "IDP_MAPSHADOW_01",       _T("map_shadow01.png") },
		{ "IDP_ACCE_01",            _T("acce01.png") },
		{ "IDP_ACCE_02",            _T("acce02.png") },
		{ "IDP_ITEM_01",            _T("item.png") },
		{ "IDP_MARK",               _T("mark.png") },
		{ "IDP_WEAPON_01",          _T("weapons\\weapon01.png") },
		{ "IDP_WEAPON_02",          _T("weapons\\weapon02.png") },
		{ "IDP_WEAPON_BOW_01",      _T("weapons\\weapon_bow01.png") },
		{ "IDP_WEAPON_ARROW_01",    _T("weapons\\weapon_arrow01.png") },
		{ "IDP_WEAPON_GLOVE_01",    _T("weapons\\weapon_glove01.png") },
		{ "IDP_WEAPON_ETC_01",      _T("weapons\\weapon_etc01.png") },
		{ "IDP_BALLOON_01",         _T("balloon_01.png") },
		{ "IDP_NPC_01",             _T("NPC\\npc01.png") },
		{ "IDP_NPC_02",             _T("NPC\\npc02.png") },
		{ "IDP_TITLE_CLOUD",        _T("title_cloud.png") },
		{ "IDP_ICON",               _T("icon.png") },
		{ "IDP_2X2_BODY_HUM",       _T("2x2\\2x2_hum.png") },
		{ "IDP_2X2_HUM_HAIR_U01",   _T("2x2\\2x2_hum_hair_u01.png") },
		{ "IDP_2X2_EYE_01",         _T("2x2\\2x2_eye01.png") },
		{ "IDP_2X2_EYE_02",         _T("2x2\\2x2_eye02.png") },
		{ "IDP_2X2_EYE_03",         _T("2x2\\2x2_eye03.png") },
		{ "IDP_2X2_EYE_04",         _T("2x2\\2x2_eye04.png") },
		{ "IDP_2X2_EYE_05",         _T("2x2\\2x2_eye05.png") },
		{ "IDP_2X2_CLOTH_00",       _T("2x2\\2x2_cloth00.png") },
		{ "IDP_2X2_CLOTH_01",       _T("2x2\\2x2_cloth01.png") },
		{ "IDP_2X2_CLOTH_02",       _T("2x2\\2x2_cloth02.png") },
		{ "IDP_2X2_CLOTH_03",       _T("2x2\\2x2_cloth03.png") },
		{ "IDP_2X2_HAIR_01",        _T("2x2\\2x2_hair01.png") },
		{ "IDP_2X2_HAIR_02",        _T("2x2\\2x2_hair02.png") },
		{ "IDP_2X2_HAIR_03",        _T("2x2\\2x2_hair03.png") },
		{ "IDP_2X2_HAIR_04",        _T("2x2\\2x2_hair04.png") },
		{ "IDP_2X2_SP_CLOTH_01",    _T("2x2\\2x2_sp_cloth01.png") },
		{ "IDP_2X2_SP_CLOTH_02",    _T("2x2\\2x2_sp_cloth02.png") },
		{ "IDP_2X2_SP_HAIR_01",     _T("2x2\\2x2_sp_hair01.png") },
		{ "IDP_2X2_SP_HAIR_02",     _T("2x2\\2x2_sp_hair02.png") },
		{ "IDP_2X2_ARMS_01",        _T("2x2\\2x2_arms01.png") },
		{ "IDP_2X2_ARMS_02",        _T("2x2\\2x2_arms02.png") },
		{ "IDP_2X2_ARMS_03",        _T("2x2\\2x2_arms03.png") },
		{ "IDP_2X2_ARMS_SP_01",     _T("2x2\\2x2_arms_sp01.png") },
		{ "IDP_2X2_SHIELD_01",      _T("2x2\\2x2_shield01.png") },
		{ "IDP_2X2_BOW_01",         _T("2x2\\2x2_bow01.png") },
		{ "IDP_2X2_NPC_001",        _T("2x2\\2x2_npc001.png") },
		{ "IDP_2X2_NPC_002",        _T("2x2\\2x2_npc002.png") },
		{ "IDP_2X2_NPC_003",        _T("2x2\\2x2_npc003.png") },
		{ "IDP_2X2_NPC_004",        _T("2x2\\2x2_npc004.png") },
		{ "IDP_2X2_NPC_005",        _T("2x2\\2x2_npc005.png") },
		{ "IDP_2X2_NPC_006",        _T("2x2\\2x2_npc006.png") },
		{ "IDP_2X2_NPC_001_SHADOW", _T("2x2\\2x2_npc001_shadow.png") },
		{ "IDP_2X2_NPC_002_SHADOW", _T("2x2\\2x2_npc002_shadow.png") },
		{ "IDP_2X2_NPC_003_SHADOW", _T("2x2\\2x2_npc003_shadow.png") },
		{ "IDP_2X2_NPC_004_SHADOW", _T("2x2\\2x2_npc004_shadow.png") },
		{ "IDP_2X2_NPC_005_SHADOW", _T("2x2\\2x2_npc005_shadow.png") },
		{ "IDP_2X2_NPC_006_SHADOW", _T("2x2\\2x2_npc006_shadow.png") },
		{ "IDP_2X2_CHAR_SHADOW_01", _T("2x2\\2x2_char_shadow_01.png") },
		{ "IDP_NUM_S",              _T("num_s.png") },
		{ "IDP_NUM_M",              _T("num_m.png") },
		{ "IDP_NUM_L",              _T("num_l.png") },
		{ "IDP_EFC_32_01",          _T("efc_32_01.png") },
		{ "IDP_EFC_64_01",          _T("efc_64_01.png") },
	};
	for (int i = 0; i < _countof(s_map); i++) {
		if (strcmp(pszResName, s_map[i].resName) == 0) {
			_tcscpy_s(pszDst, nDstCount, s_map[i].fileName);
			return TRUE;
		}
	}
	return FALSE;
}


CMgrGrpData::CMgrGrpData()
{
	m_hDll				= NULL;
	m_nMapPartsCount	= 0;
	m_nMapShadowCount	= 0;
	m_pImgSystem		= NULL;
	m_pImgIcon			= NULL;
	m_pImgShadow		= NULL;
	m_pImgItem			= NULL;
	m_pImgNumS			= NULL;
	m_pImgNumM			= NULL;
	m_pImgNumL			= NULL;
	m_paImgMapParts		= new ARRAYCIMG;
	m_paImgMapShadow	= new ARRAYCIMG;
	m_paImgNPC			= new ARRAYCIMG;
	m_paImgBody			= new ARRAYCIMG;
	m_paImgEar			= new ARRAYCIMG;
	m_paImgCloth		= new ARRAYCIMG;
	m_paImgEye			= new ARRAYCIMG;
	m_paImgHairD		= new ARRAYCIMG;
	m_paImgHairU		= new ARRAYCIMG;
	m_paImgSP			= new ARRAYCIMG;
	m_paImgAcce			= new ARRAYCIMG;
	m_paImgMapPartsTmp	= new ARRAYCIMG;
	m_paImgMapShadowTmp	= new ARRAYCIMG;
	m_paImgItemTmp		= new ARRAYCIMG;
	m_paImgEfc32		= new ARRAYCIMG;
	m_paImgEfc64		= new ARRAYCIMG;
	m_paImgWeapon		= new ARRAYCIMG;
	m_paImgWeaponBow	= new ARRAYCIMG;
	m_paImgWeaponArrow	= new ARRAYCIMG;
	m_paImgWeaponGlove	= new ARRAYCIMG;
	m_paImgWeaponEtc	= new ARRAYCIMG;
	m_paImgEfcBalloon	= new ARRAYCIMG;
	m_paImg2x2Body		= new ARRAYCIMG;
	m_paImg2x2Cloth		= new ARRAYCIMG;
	m_paImg2x2Eye		= new ARRAYCIMG;
	m_paImg2x2Hair		= new ARRAYCIMG;
	m_paImg2x2SPCloth	= new ARRAYCIMG;
	m_paImg2x2SPHair	= new ARRAYCIMG;
	m_paImg2x2Arms		= new ARRAYCIMG;
	m_paImg2x2Shield	= new ARRAYCIMG;
	m_paImg2x2ArmsSP	= new ARRAYCIMG;
	m_paImg2x2Bow		= new ARRAYCIMG;
	m_paImg2x2NPC		= new ARRAYCIMG;
	m_paImg2x2CharShadow = new ARRAYCIMG;
	m_paImg2x2NPCShadow	= new ARRAYCIMG;
}


CMgrGrpData::~CMgrGrpData()
{
	PCImg32 pImg;

	if (m_hDll) {
#if !defined(_WINDLL)
		SDL_UnloadObject(m_hDll);
#else
		FreeLibrary((HMODULE)m_hDll);
#endif
		m_hDll = NULL;
	}

	SAFE_DELETE(m_pImgSystem);
	SAFE_DELETE(m_pImgIcon);
	SAFE_DELETE(m_pImgShadow);
	SAFE_DELETE(m_pImgItem);
	SAFE_DELETE(m_pImgNumS);
	SAFE_DELETE(m_pImgNumM);
	SAFE_DELETE(m_pImgNumL);
	pImg = NULL;
	FreeInfo(m_paImgMapParts,		pImg);
	FreeInfo(m_paImgNPC,			pImg);
	FreeInfo(m_paImgBody,			pImg);
	FreeInfo(m_paImgEar,			pImg);
	FreeInfo(m_paImgCloth,			pImg);
	FreeInfo(m_paImgEye,			pImg);
	FreeInfo(m_paImgHairD,			pImg);
	FreeInfo(m_paImgHairU,			pImg);
	FreeInfo(m_paImgSP,				pImg);
	FreeInfo(m_paImgAcce,			pImg);
	FreeInfo(m_paImgMapPartsTmp,	pImg);
	FreeInfo(m_paImgMapShadowTmp,	pImg);
	FreeInfo(m_paImgItemTmp,		pImg);
	FreeInfo(m_paImgEfc32,			pImg);
	FreeInfo(m_paImgEfc64,			pImg);
	FreeInfo(m_paImgWeapon,			pImg);
	FreeInfo(m_paImgWeaponBow,		pImg);
	FreeInfo(m_paImgWeaponArrow,	pImg);
	FreeInfo(m_paImgWeaponGlove,	pImg);
	FreeInfo(m_paImgWeaponEtc,		pImg);
	FreeInfo(m_paImgEfcBalloon,		pImg);
	FreeInfo(m_paImg2x2Body,		pImg);
	FreeInfo(m_paImg2x2Cloth,		pImg);
	FreeInfo(m_paImg2x2Eye,			pImg);
	FreeInfo(m_paImg2x2Hair,		pImg);
	FreeInfo(m_paImg2x2SPCloth,		pImg);
	FreeInfo(m_paImg2x2SPHair,		pImg);
	FreeInfo(m_paImg2x2Arms,		pImg);
	FreeInfo(m_paImg2x2Shield,		pImg);
	FreeInfo(m_paImg2x2ArmsSP,		pImg);
	FreeInfo(m_paImg2x2Bow,			pImg);
	FreeInfo(m_paImg2x2NPC,			pImg);
	FreeInfo(m_paImg2x2CharShadow,	pImg);
	FreeInfo(m_paImg2x2NPCShadow,	pImg);
}


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

	// ファイル名の作成
	BuildModuleRelativePath(szName, _countof(szName), _T("SboGrpData.dll"));
#if !defined(_WINDLL)
	// SboCli: SDL_LoadObject でクロスプラットフォーム対応
	{
		std::string ansiDllPath = TStringToAnsiStd(szName);
		m_hDll = SDL_LoadObject(ansiDllPath.c_str());
	}
#else
	// SboCliAdminMfc: Win32 API
	m_hDll = (void*)LoadLibrary(szName);
#endif
	if (m_hDll == NULL) {
		goto Exit;
	}

	// システム
	Read256("IDP_SYSTEM", &m_pImgSystem, 1);
	// アイコン
	Read256("IDP_ICON", &m_pImgIcon, 1);
	// 影画像
	Read256("IDP_SHADOW", &m_pImgShadow, 1);
	// アイテム画像
	Read256("IDP_ITEM_01", &m_pImgItem, 1);
	// 数字(S)
	Read256("IDP_NUM_S", &m_pImgNumS, 1);
	// 数字(M)
	Read256("IDP_NUM_M", &m_pImgNumM, 1);
	// 数字(L)
	Read256("IDP_NUM_L", &m_pImgNumL, 1);

	// マップ
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_MAP_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgMapParts->Add(pImg);
	}
	m_nMapPartsCount = i;

	// マップ影
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_MAPSHADOW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgMapShadow->Add(pImg);
	}
	m_nMapShadowCount = i;

	// 体
	for (i = 0; ; i += 2) {
		pszTmp = apszBodyTbl[i];
		if (pszTmp == NULL) {
			break;
		}
		pImg	= new CImg32;
		bResult	= Read256(pszTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgBody->Add(pImg);

		pImg = NULL;
		// 耳
		pszTmp = apszBodyTbl[i + 1];
		if (pszTmp) {
			pImg	= new CImg32;
			bResult	= Read256(pszTmp, &pImg, 1);
			if (bResult == FALSE) {
				delete pImg;
			}
		}
		m_paImgEar->Add(pImg);
	}
	// 服
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_CLOTH_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgCloth->Add(pImg);
	}
	// 目
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_EYE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgEye->Add(pImg);
	}
	// 髪(下)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_HAIR_D%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgHairD->Add(pImg);
	}
	// 髪(上)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_HAIR_U%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgHairU->Add(pImg);
	}
	// NPC
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_NPC_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgNPC->Add(pImg);
	}
	// 特殊服
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_CLOTH_SP%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgSP->Add(pImg);
	}
	// アクセサリ
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_ACCE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgAcce->Add(pImg);
	}
	// エフェクト(32)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_EFC_32_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgEfc32->Add(pImg);
	}
	// エフェクト(64)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_EFC_64_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgEfc64->Add(pImg);
	}
	// 武器
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_WEAPON_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeapon->Add(pImg);
	}
	// 武器(弓)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_WEAPON_BOW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeaponBow->Add(pImg);
	}
	// 武器(矢)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_WEAPON_ARROW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeaponArrow->Add(pImg);
	}
	// 武器(打撃)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_WEAPON_GLOVE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeaponGlove->Add(pImg);
	}
	// 武器(その他)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_WEAPON_ETC_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeaponEtc->Add(pImg);
	}
	// 噴出し
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_BALLOON_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgEfcBalloon->Add(pImg);
	}
	// 2x2体
	for (i = 0; ; i += 2) {
		pszTmp = apsz2x2BodyTbl[i];
		if (pszTmp == NULL) {
			break;
		}
		pImg	= new CImg32;
		bResult	= Read256(pszTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Body->Add(pImg);

		pImg = NULL;
		// 耳
		pszTmp = apsz2x2BodyTbl[i + 1];
		if (pszTmp) {
			pImg	= new CImg32;
			bResult	= Read256(pszTmp, &pImg, 1);
			if (bResult == FALSE) {
				delete pImg;
			}
		}
	}
	// 服
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_CLOTH_%02d", i);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Cloth->Add(pImg);
	}
	// 目
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_EYE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Eye->Add(pImg);
	}
	// 髪
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_HAIR_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Hair->Add(pImg);
	}
	// 特殊服
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_SP_CLOTH_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2SPCloth->Add(pImg);
	}
	// 特殊髪
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_SP_HAIR_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2SPHair->Add(pImg);
	}
	// 持ち物(2x2)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_ARMS_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Arms->Add(pImg);
	}
	// 盾(2x2)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_SHIELD_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Shield->Add(pImg);
	}
	// 特殊持ち物(2x2)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_ARMS_SP_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2ArmsSP->Add(pImg);
	}
	// 弓(2x2)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_BOW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Bow->Add(pImg);
	}
	// NPC(2x2)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_NPC_%03d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2NPC->Add(pImg);
	}
	// キャラ影(2x2)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_CHAR_SHADOW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2CharShadow->Add(pImg);
	}
	// NPC影(2x2)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_NPC_%03d_SHADOW", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2NPCShadow->Add(pImg);
	}


	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CMgrGrpData::LoadLocalTitleAssets(void)
{
	TCHAR szBasePath[MAX_PATH];
	BOOL bLoaded;
	TCHAR szFilePath[MAX_PATH];
	BOOL bSystemExists;
	BOOL bSystemRead;
	BOOL bTitleExists;
	BOOL bTitleBackExists;
	BOOL bTitleCloudExists;
	int i;
	PCImg32 pImg;
	BOOL bResult;
	char szTmp[64];
	LPSTR pszTmp,
		apszBodyTbl[] = {
			"IDP_BODY_BST", "IDP_BODY_BST_EAR",
			"IDP_BODY_DRK", "IDP_BODY_DRK_EAR",
			"IDP_BODY_ELF", "IDP_BODY_ELF_EAR",
			"IDP_BODY_HUM", NULL,
			NULL, NULL
		};

	if (!FindSboGrpResBasePath(szBasePath, _countof(szBasePath))) {
#if defined(__EMSCRIPTEN__)
		SBOP2_DebugMarkGrpLoad(0, 0, 0, 0, 0, 0);
#endif
		return FALSE;
	}

	bLoaded = TRUE;
	bSystemExists = FALSE;
	bSystemRead = FALSE;
	bTitleExists = FALSE;
	bTitleBackExists = FALSE;
	bTitleCloudExists = FALSE;

	BuildSboGrpResPath(szFilePath, _countof(szFilePath), szBasePath, _T("system.png"));
	bSystemExists = PathFileExists(szFilePath);
	bSystemRead = Read256File(szFilePath, &m_pImgSystem, 1);
	bLoaded &= bSystemRead;

	BuildSboGrpResPath(szFilePath, _countof(szFilePath), szBasePath, _T("title.png"));
	bTitleExists = PathFileExists(szFilePath);
	bLoaded &= bTitleExists;
	BuildSboGrpResPath(szFilePath, _countof(szFilePath), szBasePath, _T("title_back.png"));
	bTitleBackExists = PathFileExists(szFilePath);
	bLoaded &= bTitleBackExists;
	BuildSboGrpResPath(szFilePath, _countof(szFilePath), szBasePath, _T("title_cloud.png"));
	bTitleCloudExists = PathFileExists(szFilePath);
	bLoaded &= bTitleCloudExists;

#if defined(__EMSCRIPTEN__)
	SBOP2_DebugMarkGrpLoad(1, bSystemExists ? 1 : 0, bSystemRead ? 1 : 0,
		bTitleExists ? 1 : 0, bTitleBackExists ? 1 : 0, bTitleCloudExists ? 1 : 0);
#endif

	// キャラ選択画面で必要なキャラクター画像をロード
	// （DLL なしのブラウザ版でも Load() の代わりにファイルから読み込む）
	// 体・耳
	for (i = 0; ; i += 2) {
		pszTmp = apszBodyTbl[i];
		if (pszTmp == NULL) {
			break;
		}
		pImg	= new CImg32;
		bResult	= Read256(pszTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgBody->Add(pImg);

		pImg = NULL;
		// 耳
		pszTmp = apszBodyTbl[i + 1];
		if (pszTmp) {
			pImg	= new CImg32;
			bResult	= Read256(pszTmp, &pImg, 1);
			if (bResult == FALSE) {
				delete pImg;
				pImg = NULL;
			}
		}
		m_paImgEar->Add(pImg);
	}
	// 服
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_CLOTH_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgCloth->Add(pImg);
	}
	// 目
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_EYE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgEye->Add(pImg);
	}
	// 髪(下)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_HAIR_D%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgHairD->Add(pImg);
	}
	// 髪(上)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_HAIR_U%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgHairU->Add(pImg);
	}
	// 特殊服
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_CLOTH_SP%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgSP->Add(pImg);
	}
	// アクセサリ
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_ACCE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgAcce->Add(pImg);
	}

	// 2x2 体（HUMAN のみ）
	{
		pImg	= new CImg32;
		bResult	= Read256("IDP_2X2_BODY_HUM", &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
		} else {
			m_paImg2x2Body->Add(pImg);
		}
	}
	// 2x2 服
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_CLOTH_%02d", i);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Cloth->Add(pImg);
	}
	// 2x2 目
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_EYE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Eye->Add(pImg);
	}
	// 2x2 髪
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_HAIR_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Hair->Add(pImg);
	}
	// 2x2 特殊服
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_SP_CLOTH_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2SPCloth->Add(pImg);
	}
	// 2x2 特殊髪
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_SP_HAIR_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2SPHair->Add(pImg);
	}
	// 2x2 武器(arms)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_ARMS_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Arms->Add(pImg);
	}
	// 2x2 シールド
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_SHIELD_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Shield->Add(pImg);
	}
	// 2x2 特殊武器(arms SP)
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_ARMS_SP_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2ArmsSP->Add(pImg);
	}
	// 2x2 弓
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_BOW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Bow->Add(pImg);
	}
	// 2x2 NPC
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_NPC_%03d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2NPC->Add(pImg);
	}
	// 2x2 NPC 影
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_2X2_NPC_%03d_SHADOW", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2NPCShadow->Add(pImg);
	}
	// 2x2 キャラ影（固定1枚）
	{
		pImg	= new CImg32;
		bResult	= Read256("IDP_2X2_CHAR_SHADOW_01", &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
		} else {
			m_paImg2x2CharShadow->Add(pImg);
		}
	}

	fprintf(stderr, "INFO: LoadLocalTitleAssets: body=%d ear=%d cloth=%d eye=%d hairD=%d hairU=%d sp=%d acce=%d "
		"2x2_body=%d 2x2_cloth=%d 2x2_eye=%d 2x2_hair=%d 2x2_arms=%d 2x2_npc=%d\n",
		(int)m_paImgBody->size(), (int)m_paImgEar->size(),
		(int)m_paImgCloth->size(), (int)m_paImgEye->size(),
		(int)m_paImgHairD->size(), (int)m_paImgHairU->size(),
		(int)m_paImgSP->size(), (int)m_paImgAcce->size(),
		(int)m_paImg2x2Body->size(), (int)m_paImg2x2Cloth->size(),
		(int)m_paImg2x2Eye->size(), (int)m_paImg2x2Hair->size(),
		(int)m_paImg2x2Arms->size(), (int)m_paImg2x2NPC->size());

	return bLoaded;
}


BOOL CMgrGrpData::CheckVersion(LPCSTR pszVersion)
{
	BOOL bRet;
	TCHAR szTmp[64];

	bRet = FALSE;

	ZeroMemory(szTmp, sizeof(szTmp));
	// バージョンチェック
	LoadString((HINSTANCE)m_hDll, IDS_DLLVER, szTmp, _countof(szTmp));
	std::string strDllVersion = TStringToAnsiStd(szTmp);
	if (strDllVersion != pszVersion) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


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

	width = pSrc->Width();
	height = pSrc->Height();
	if ((width == 0) || (height == 0)) {
		return;
	}

	fp = fopen(pszFileName, "wb");
	if (fp == NULL) {
		return;
	}

	raw.resize((size_t)width * height * 3);
	for (y = 0; y < (int)height; y++) {
		const BYTE *pSrcLine = pSrc->GetBits() + (height - 1 - y) * (width * 4);
		unsigned char *pDst = &raw[(size_t)y * width * 3];
		for (x = 0; x < (int)width; x++) {
			pDst[0] = pSrcLine[2];
			pDst[1] = pSrcLine[1];
			pDst[2] = pSrcLine[0];
			pDst += 3;
			pSrcLine += 4;
		}
	}

	error = lodepng::encode(encoded, raw, width, height, LCT_RGB, 8);
	if ((error == 0) && (!encoded.empty())) {
		fwrite(&encoded[0], 1, encoded.size(), fp);
	}

	fclose(fp);
}


PCImg32 CMgrGrpData::GetDibSystem(void)
{
	return m_pImgSystem;
}


PCImg32 CMgrGrpData::GetDibIcon(void)
{
	return m_pImgIcon;
}


PCImg32 CMgrGrpData::GetDibShadow(void)
{
	return m_pImgShadow;
}


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


PCImg32 CMgrGrpData::GetDibItem(int nNo)
{
	return m_pImgItem;
}


PCImg32 CMgrGrpData::GetDibNumS(void)
{
	return m_pImgNumS;
}


PCImg32 CMgrGrpData::GetDibNumM(void)
{
	return m_pImgNumM;
}


PCImg32 CMgrGrpData::GetDibNumL(void)
{
	return m_pImgNumL;
}


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


PCImg32 CMgrGrpData::GetDibBody(WORD wFamilyID)
{
	int nNo;
	PCImg32 pImg;

	nNo	= -1;
	pImg = NULL;

	switch (wFamilyID) {
	case FAMILYTYPE_HUMAN:	nNo = 3;	break;	// ニンゲン
	case FAMILYTYPE_ELF:	nNo = 2;	break;	// エルフ
	case FAMILYTYPE_BST:	nNo = 0;	break;	// ジュウジン
	case FAMILYTYPE_DAEMON:	nNo = 1;	break;	// マゾク
	default:
		goto Exit;
	}
	if ((nNo < 0) || ((size_t)nNo >= m_paImgBody->size())) {
		goto Exit;
	}
	pImg = m_paImgBody->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDibEar(WORD wFamilyID)
{
	int nNo;
	PCImg32 pImg;

	nNo	= -1;
	pImg = NULL;

	switch (wFamilyID) {
//	case FAMILYTYPE_HUMAN:	nNo = 3;	break;	// ニンゲン
	case FAMILYTYPE_ELF:	nNo = 2;	break;	// エルフ
	case FAMILYTYPE_BST:	nNo = 0;	break;	// ジュウジン
	case FAMILYTYPE_DAEMON:	nNo = 1;	break;	// マゾク
	default:
		goto Exit;
	}
	if ((nNo < 0) || ((size_t)nNo >= m_paImgEar->size())) {
		goto Exit;
	}
	pImg = m_paImgEar->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDibCloth(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max(0, nNo);
	if (nNo >= m_paImgCloth->size()) {
		goto Exit;
	}

	pImg = m_paImgCloth->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDibEye(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max(0, nNo);
	if (nNo >= m_paImgEye->size()) {
		goto Exit;
	}

	pImg = m_paImgEye->at(nNo);

Exit:
	return pImg;
}


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


PCImg32 CMgrGrpData::GetDibSP(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max(0, nNo);
	if (nNo >= m_paImgSP->size()) {
		goto Exit;
	}

	pImg = m_paImgSP->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDibAcce(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max(0, nNo);
	if (nNo >= m_paImgAcce->size()) {
		goto Exit;
	}

	pImg = m_paImgAcce->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDibEfc32(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max(0, nNo);
	if (nNo >= m_paImgEfc32->size()) {
		goto Exit;
	}

	pImg = m_paImgEfc32->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDibEfc64(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max(0, nNo);
	if (nNo >= m_paImgEfc64->size()) {
		goto Exit;
	}

	pImg = m_paImgEfc64->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDibWeapon(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max(0, nNo);
	if (nNo >= m_paImgWeapon->size()) {
		goto Exit;
	}

	pImg = m_paImgWeapon->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDibWeaponBow(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max(0, nNo);
	if (nNo >= m_paImgWeaponBow->size()) {
		goto Exit;
	}

	pImg = m_paImgWeaponBow->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDibWeaponArrow(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max(0, nNo);
	if (nNo >= m_paImgWeaponArrow->size()) {
		goto Exit;
	}

	pImg = m_paImgWeaponArrow->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDibWeaponGlove(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max(0, nNo);
	if (nNo >= m_paImgWeaponGlove->size()) {
		goto Exit;
	}

	pImg = m_paImgWeaponGlove->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDibWeaponEtc(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max(0, nNo);
	if (nNo >= m_paImgWeaponEtc->size()) {
		goto Exit;
	}

	pImg = m_paImgWeaponEtc->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDibEfcBalloon(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max(0, nNo);
	if (nNo >= m_paImgEfcBalloon->size()) {
		goto Exit;
	}

	pImg = m_paImgEfcBalloon->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDib2x2Body(WORD wFamilyID)
{
	int nNo;
	PCImg32 pImg;

	nNo	= -1;
	pImg = NULL;

	switch (wFamilyID) {
	case FAMILYTYPE_HUMAN:	nNo = 0;	break;	// ニンゲン
	default:
		goto Exit;
	}
	if ((nNo < 0) || ((size_t)nNo >= m_paImg2x2Body->size())) {
		goto Exit;
	}
	pImg = m_paImg2x2Body->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDib2x2Cloth(WORD wClothID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wClothID >= m_paImg2x2Cloth->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2Cloth->at(wClothID);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDib2x2Eye(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max(0, nNo);
	if (nNo >= m_paImg2x2Eye->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2Eye->at(nNo);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDib2x2Hair(WORD wHairColorID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wHairColorID >= m_paImg2x2Hair->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2Hair->at(wHairColorID);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDib2x2SPCloth(WORD wSPClothID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wSPClothID >= m_paImg2x2SPCloth->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2SPCloth->at(wSPClothID);

Exit:
	return pImg;
}


PCImg32 CMgrGrpData::GetDib2x2SPHair(WORD wSPHairID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wSPHairID >= m_paImg2x2SPHair->size()) {
		goto Exit;
	}

	pImg = m_paImg2x2SPHair->at(wSPHairID);

Exit:
	return pImg;
}


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


PCImg32 CMgrGrpData::GetDib(DWORD dwGrpIDMain, DWORD dwGrpIDSub, DWORD dwParam)
{
	PCImg32 pRet;

	pRet = NULL;

	switch (dwGrpIDMain) {
	case GRPIDMAIN_CHAR:			// キャラ画像
		pRet = GetDibBody((WORD)dwParam);
		break;
	case GRPIDMAIN_WEAPON:			// 武器
		pRet = GetDibWeapon(0);
		break;
	case GRPIDMAIN_WEAPON_BOW:		// 弓
		pRet = GetDibWeaponBow(0);
		break;
	case GRPIDMAIN_EFFECT32:		// エフェクト(32)
		pRet = GetDibEfc32(0);
		break;
	case GRPIDMAIN_WEAPON_GLOVE:	// 武器(打撃)
		pRet = GetDibWeaponGlove(0);
		break;
	case GRPIDMAIN_WEAPON_ETC:		// 武器(その他)
		pRet = GetDibWeaponEtc(0);
		break;
	case GRPIDMAIN_NPC:				// キャラ(NPC)
		pRet = GetDibNPC(0);
		break;
	case GRPIDMAIN_EFCBALLOON:		// 噴出し
		pRet = GetDibEfcBalloon(0);
		break;
	case GRPIDMAIN_2X2_CHAR:		// キャラ(32x32)画像
		pRet = GetDib2x2Body((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_CLOTH:		// 服(32x32)
		pRet = GetDib2x2Cloth((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_EYE:			// 目(32x32)
		pRet = GetDib2x2Eye((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_HAIR:		// 髪(32x32)
		if (dwParam < 10000) {
			pRet = GetDib2x2Hair((WORD)dwParam);
		} else {
			pRet = GetDib2x2SPHair((WORD)dwParam - 10000);
		}
		break;
	case GRPIDMAIN_2X2_SPCLOTH:		// 特殊服(32x32)
		pRet = GetDib2x2SPCloth((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_SPHAIR:		// 特殊髪(32x32)
		pRet = GetDib2x2SPHair((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_ARMS:		// 持ち物(2x2)
		dwParam = dwGrpIDSub / GetGrpCountX(dwGrpIDMain);
		dwParam = dwParam / GetGrpCountY(dwGrpIDMain);
		pRet = GetDib2x2Arms((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_SHIELD:		// 盾(2x2)
		pRet = GetDib2x2Shield((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_ARMSSP:		// 特殊持ち物(2x2)
		pRet = GetDib2x2ArmsSP((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_BOW:			// 弓(2x2)
		pRet = GetDib2x2Bow((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_NPC:			// NPC(2x2)
		pRet = GetDib2x2NPC((WORD)dwParam);
		break;
	case GRPIDMAIN_EFFECT64:		// エフェクト(64)
		pRet = GetDibEfc64(0);
		break;
	case GRPIDMAIN_ICON32:			// アイコン(２倍表示)
		pRet = GetDibIcon();
		break;
	}

	return pRet;
}


PCImg32 CMgrGrpData::GetDibTmpLogo(void)
{
	BOOL bResult;
	PCImg32 pImg;
	TCHAR szBasePath[MAX_PATH];
	TCHAR szFilePath[MAX_PATH];

	pImg = new CImg32;
	bResult	= Read256("IDP_LOGO", &pImg, 1);
	if (bResult == FALSE) {
		if (FindSboGrpResBasePath(szBasePath, _countof(szBasePath))) {
			BuildSboGrpResPath(szFilePath, _countof(szFilePath), szBasePath, _T("URARA-works-logo.png"));
			bResult = Read256File(szFilePath, &pImg, 1);
		}
		if (bResult == FALSE) {
			SAFE_DELETE(pImg);
		}
	}
	if (pImg) {
		pImg->SetColorKeyNo(255);
	}

	return pImg;
}


PCImg32 CMgrGrpData::GetDibTmpTitle(void)
{
	BOOL bResult;
	PCImg32 pImg;
	TCHAR szBasePath[MAX_PATH];
	TCHAR szFilePath[MAX_PATH];

	pImg	= new CImg32;
	bResult	= Read256("IDP_TITLE", &pImg, 1);
	if (bResult == FALSE) {
		if (FindSboGrpResBasePath(szBasePath, _countof(szBasePath))) {
			BuildSboGrpResPath(szFilePath, _countof(szFilePath), szBasePath, _T("title.png"));
			bResult = Read256File(szFilePath, &pImg, 1);
		}
		if (bResult == FALSE) {
			SAFE_DELETE(pImg);
		}
	}
	if (pImg) {
		pImg->SetColorKeyNo(71);
	}

	return pImg;
}


PCImg32 CMgrGrpData::GetDibTmpTitleBack(void)
{
	BOOL bResult;
	PCImg32 pImg;
	TCHAR szBasePath[MAX_PATH];
	TCHAR szFilePath[MAX_PATH];

	pImg	= new CImg32;
	bResult	= Read256("IDP_TITLE_BACK", &pImg, 1);
	if (bResult == FALSE) {
		if (FindSboGrpResBasePath(szBasePath, _countof(szBasePath))) {
			BuildSboGrpResPath(szFilePath, _countof(szFilePath), szBasePath, _T("title_back.png"));
			bResult = Read256File(szFilePath, &pImg, 1);
		}
		if (bResult == FALSE) {
			SAFE_DELETE(pImg);
		}
	}

	return pImg;
}


PCImg32 CMgrGrpData::GetDibTmpTitleCloud(void)
{
	BOOL bResult;
	PCImg32 pImg;
	TCHAR szBasePath[MAX_PATH];
	TCHAR szFilePath[MAX_PATH];

	pImg	= new CImg32;
	bResult	= Read256("IDP_TITLE_CLOUD", &pImg, 1);
	if (bResult == FALSE) {
		if (FindSboGrpResBasePath(szBasePath, _countof(szBasePath))) {
			BuildSboGrpResPath(szFilePath, _countof(szFilePath), szBasePath, _T("title_cloud.png"));
			bResult = Read256File(szFilePath, &pImg, 1);
		}
		if (bResult == FALSE) {
			SAFE_DELETE(pImg);
		}
	}
	if (pImg) {
		// パレットインデックス 0 がマゼンタ（#FF00FF）= 透明色として使用
		// title.png が SetColorKeyNo(71) を明示設定するのと同様に、雲も明示的に設定する
		pImg->SetColorKeyNo(0);
#if defined(__EMSCRIPTEN__)
		SBOP2_DebugMarkCloudLoad(1, pImg->Width(), pImg->Height());
#endif
	} else {
#if defined(__EMSCRIPTEN__)
		SBOP2_DebugMarkCloudLoad(0, 0, 0);
#endif
	}

	return pImg;
}


PCImg32 CMgrGrpData::GetDibTmpLoginMenuBack(void)
{
	BOOL bResult;
	PCImg32 pImg;

	pImg	= new CImg32;
	bResult	= Read("IDP_CHARSELECT_BACK", &pImg, 2);
	if (bResult == FALSE) {
		SAFE_DELETE(pImg);
	}

	return pImg;
}


int CMgrGrpData::GetMapPartsCount(void)
{
	return m_nMapPartsCount * 1024;
}


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


void CMgrGrpData::ReadMapPartsTmp(void)
{
	int i;
	BOOL bResult;
	char szTmp[128];
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo(m_paImgMapPartsTmp, pImg);
	// マップ
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_MAP_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgMapPartsTmp->Add(pImg);
	}
}


void CMgrGrpData::DeleteMapPartsTmp(void)
{
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo(m_paImgMapPartsTmp, pImg);
}


int CMgrGrpData::GetMapShadowCount(void)
{
	return m_nMapShadowCount * 1024;
}


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


void CMgrGrpData::ReadMapShadowTmp(void)
{
	int i;
	BOOL bResult;
	char szTmp[128];
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo(m_paImgMapShadowTmp, pImg);
	// マップ
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_MAPSHADOW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgMapShadowTmp->Add(pImg);
	}
}


void CMgrGrpData::DeleteMapShadowTmp(void)
{
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo(m_paImgMapShadowTmp, pImg);
}


int CMgrGrpData::GetItemCount(void)
{
	return 1024;
}


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


void CMgrGrpData::ReadItemTmp(void)
{
	int i;
	BOOL bResult;
	char szTmp[128];
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo(m_paImgItemTmp, pImg);
	// アイテム
	for (i = 0; ; i++) {
		sprintf_s(szTmp, sizeof(szTmp), "IDP_ITEM_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256(szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgItemTmp->Add(pImg);
	}
}


void CMgrGrpData::DeleteItemTmp(void)
{
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo(m_paImgItemTmp, pImg);
}


int CMgrGrpData::GetGrpSize(DWORD dwGrpIDMain)
{
	int nRet;

	nRet = 0;

	switch (dwGrpIDMain) {
	case GRPIDMAIN_CHAR:			// キャラ画像
	case GRPIDMAIN_NPC:				// キャラ(NPC)
	case GRPIDMAIN_EFCBALLOON:		// 噴出し
	case GRPIDMAIN_2X2_SHIELD:		// 盾(2x2)
	case GRPIDMAIN_ICON32:			// アイコン(２倍表示)
		nRet = 16;
		break;
	case GRPIDMAIN_WEAPON:			// 武器
	case GRPIDMAIN_WEAPON_BOW:		// 弓
	case GRPIDMAIN_EFFECT32:		// エフェクト(32)
	case GRPIDMAIN_WEAPON_GLOVE:	// 武器(打撃)
	case GRPIDMAIN_WEAPON_ETC:		// 武器(その他)
	case GRPIDMAIN_2X2_CHAR:		// キャラ(32x32)画像
	case GRPIDMAIN_2X2_CLOTH:		// 服(32x32)
	case GRPIDMAIN_2X2_EYE:			// 目(32x32)
	case GRPIDMAIN_2X2_HAIR:		// 髪(32x32)
	case GRPIDMAIN_2X2_SPCLOTH:		// 特殊服(32x32)
	case GRPIDMAIN_2X2_SPHAIR:		// 特殊髪(32x32)
	case GRPIDMAIN_2X2_NPC:			// NPC(2x2)
		nRet = 32;
		break;
	case GRPIDMAIN_2X2_ARMS:		// 持ち物(2x2)
	case GRPIDMAIN_2X2_ARMSSP:		// 特殊持ち物(2x2)
	case GRPIDMAIN_2X2_BOW:			// 弓(2x2)
		nRet = 24;
		break;
	case GRPIDMAIN_EFFECT64:		// エフェクト(64)
		nRet = 64;
		break;
	}

	return nRet;
}


int CMgrGrpData::GetGrpCountX(DWORD dwGrpIDMain)
{
	int nRet;

	nRet = 0;

	switch (dwGrpIDMain) {
	case GRPIDMAIN_CHAR:			// キャラ画像
		nRet = 32;
		break;
	case GRPIDMAIN_NPC:				// キャラ(NPC)
		nRet = 16;
		break;
	case GRPIDMAIN_WEAPON:			// 武器
		nRet = 32;
		break;
	case GRPIDMAIN_WEAPON_BOW:		// 弓
	case GRPIDMAIN_WEAPON_GLOVE:	// 武器(打撃)
	case GRPIDMAIN_WEAPON_ETC:		// 武器(その他)
	case GRPIDMAIN_ICON32:			// アイコン(２倍表示)
		nRet = 20;
		break;
	case GRPIDMAIN_EFFECT32:		// エフェクト(32)
		nRet = 16;
		break;
	case GRPIDMAIN_EFCBALLOON:		// 噴出し
		nRet = 10;
		break;
	case GRPIDMAIN_2X2_CHAR:		// キャラ(32x32)画像
	case GRPIDMAIN_2X2_CLOTH:		// 服(32x32)
	case GRPIDMAIN_2X2_HAIR:		// 髪(32x32)
	case GRPIDMAIN_2X2_SPCLOTH:		// 特殊服(32x32)
	case GRPIDMAIN_2X2_SPHAIR:		// 特殊髪(32x32)
	case GRPIDMAIN_2X2_NPC:			// NPC(2x2)
		nRet = 16;
		break;
	case GRPIDMAIN_2X2_EYE:			// 目(32x32)
	case GRPIDMAIN_2X2_ARMS:		// 持ち物(2x2)
		nRet = 12;
		break;
	case GRPIDMAIN_2X2_ARMSSP:		// 特殊持ち物(2x2)
	case GRPIDMAIN_2X2_BOW:			// 弓(2x2)
		nRet = 11;
		break;
	case GRPIDMAIN_2X2_SHIELD:		// 盾(2x2)
		nRet = 5;
		break;
	case GRPIDMAIN_EFFECT64:		// エフェクト(64)
		nRet = 8;
		break;
	}

	return nRet;
}


int CMgrGrpData::GetGrpCountY(DWORD dwGrpIDMain)
{
	int nRet;

	nRet = 0;

	switch (dwGrpIDMain) {
	case GRPIDMAIN_CHAR:			// キャラ画像
		nRet = 1;
		break;
	case GRPIDMAIN_WEAPON:			// 武器
	case GRPIDMAIN_WEAPON_BOW:		// 弓
	case GRPIDMAIN_WEAPON_GLOVE:	// 武器(打撃)
	case GRPIDMAIN_WEAPON_ETC:		// 武器(その他)
		nRet = 18;
		break;
	case GRPIDMAIN_EFFECT32:		// エフェクト(32)
		nRet = 16;
		break;
	case GRPIDMAIN_NPC:				// キャラ(NPC)
		nRet = 32;
		break;
	case GRPIDMAIN_EFCBALLOON:		// 噴出し
		nRet = 15;
		break;
	case GRPIDMAIN_2X2_CHAR:		// キャラ(32x32)画像
	case GRPIDMAIN_2X2_CLOTH:		// 服(32x32)
	case GRPIDMAIN_2X2_EYE:			// 目(32x32)
	case GRPIDMAIN_2X2_HAIR:		// 髪(32x32)
	case GRPIDMAIN_2X2_SPCLOTH:		// 特殊服(32x32)
	case GRPIDMAIN_2X2_SPHAIR:		// 特殊髪(32x32)
	case GRPIDMAIN_2X2_NPC:			// NPC(2x2)
		nRet = 8;
		break;
	case GRPIDMAIN_2X2_ARMS:		// 持ち物(2x2)
	case GRPIDMAIN_2X2_SHIELD:		// 盾(2x2)
	case GRPIDMAIN_2X2_ARMSSP:		// 特殊持ち物(2x2)
	case GRPIDMAIN_2X2_BOW:			// 弓(2x2)
		nRet = 25;
		break;
	case GRPIDMAIN_EFFECT64:		// エフェクト(64)
		nRet = 8;
		break;
	case GRPIDMAIN_ICON32:			// アイコン(２倍表示)
		nRet = 20;
		break;
	}

	return nRet;
}


void CMgrGrpData::GetGrpPos(
	DWORD dwGrpIDMain,		// [in] 画像IDメイン
	DWORD dwGrpIDSub,		// [in] 画像IDサブ
	POINT &ptDst,			// [ou] 画像位置
	WORD wGrpID)			// [in] 縦位置をずらす個数
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

	nCountX	= GetGrpCountX(dwGrpIDMain);
	nCountY	= GetGrpCountY(dwGrpIDMain);
	nSize	= GetGrpSize(dwGrpIDMain);

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


BOOL CMgrGrpData::Read(LPCSTR pszName, PCImg32 *pDib, int nSize)
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
	CImg32 *pDibTmp;
	BYTE *pData;

	bRet		= FALSE;
	pDibTmp		= NULL;

	// ファイルベース読み込みを先行試行（リソースAPI不使用でも動作可能にする）
	std::vector<unsigned char> fileData;
	{
		TCHAR szBasePath[MAX_PATH];
		TCHAR szFileName[MAX_PATH];
		TCHAR szFilePath[MAX_PATH];
		if (GetFileNameForResource(pszName, szFileName, _countof(szFileName))) {
			if (FindSboGrpResBasePath(szBasePath, _countof(szBasePath))) {
				BuildSboGrpResPath(szFilePath, _countof(szFilePath), szBasePath, szFileName);
				ReadBinaryFile(szFilePath, fileData);
			}
		}
	}

	if (!fileData.empty()) {
		// ファイルから取得済み
		dwResourceSize = (DWORD)fileData.size();
		pResourceData = &fileData[0];
	} else {
		// リソースAPIにフォールバック
		CString strName = AnsiToTString(pszName);
		LPCTSTR pszNameT = strName;
		hResInfo = FindResource((HMODULE)m_hDll, pszNameT, _T("PNG"));
		if (hResInfo == NULL) {
			goto Exit;
		}
		hRes = LoadResource((HMODULE)m_hDll, hResInfo);
		if (hRes == NULL) {
			goto Exit;
		}
		dwResourceSize = SizeofResource((HMODULE)m_hDll, hResInfo);
		pResourceData = (const BYTE *)LockResource(hRes);
		if ((pResourceData == NULL) || (dwResourceSize == 0)) {
			goto Exit;
		}
	}

	if (lodepng::decode(image, width, height, pResourceData, dwResourceSize)) {
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

	pDibTmp->Create(width, height);
	pDibNew->CreateWithoutGdi(width * nSize, height * nSize);
	pData = pDibTmp->GetBits();

	for (y = 0; y < (int)height; y++) {
		BYTE *pDst = pData + (height - y - 1) * width * 4;
		const unsigned char *pSrc = &image[(size_t)y * width * 4];
		for (x = 0; x < (int)width; x++) {
			pDst[0] = pSrc[2];
			pDst[1] = pSrc[1];
			pDst[2] = pSrc[0];
			pDst[3] = pSrc[3];
			pDst += 4;
			pSrc += 4;
		}
	}

	// GDI StretchBlt を BltStretchNearest に置換（GDI/HDC不使用）
	pDibNew->BltStretchNearest(0, 0, width * nSize, height * nSize,
		pDibTmp, 0, 0, width, height);

	bRet = TRUE;
Exit:
	SAFE_DELETE(pDibTmp);
	return bRet;
}


BOOL CMgrGrpData::Read256(LPCSTR pszName, PCImg32 *pDib, int nSize)
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

	// ファイルベース読み込みを先行試行（リソースAPI不使用でも動作可能にする）
	std::vector<unsigned char> fileData256;
	{
		TCHAR szBasePath[MAX_PATH];
		TCHAR szFileName[MAX_PATH];
		TCHAR szFilePath[MAX_PATH];
		if (GetFileNameForResource(pszName, szFileName, _countof(szFileName))) {
			if (FindSboGrpResBasePath(szBasePath, _countof(szBasePath))) {
				BuildSboGrpResPath(szFilePath, _countof(szFilePath), szBasePath, szFileName);
				ReadBinaryFile(szFilePath, fileData256);
			}
		}
	}

	if (!fileData256.empty()) {
		// ファイルから取得済み
		dwResourceSize = (DWORD)fileData256.size();
		pResourceData = &fileData256[0];
	} else {
		// リソースAPIにフォールバック
		CString strName = AnsiToTString(pszName);
		LPCTSTR pszNameT = strName;
		hResInfo = FindResource((HMODULE)m_hDll, pszNameT, _T("PNG"));
		if (hResInfo == NULL) {
			goto Exit;
		}
		hRes = LoadResource((HMODULE)m_hDll, hResInfo);
		if (hRes == NULL) {
			goto Exit;
		}
		dwResourceSize = SizeofResource((HMODULE)m_hDll, hResInfo);
		pResourceData = (const BYTE *)LockResource(hRes);
		if ((pResourceData == NULL) || (dwResourceSize == 0)) {
			goto Exit;
		}
	}

	if (lodepng::decode(image, width, height, state, pResourceData, dwResourceSize)) {
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
	if (image.size() != (size_t)width * height) {
		goto Exit;
	}

	if (*pDib == NULL) {
		*pDib = new CImg32;
	}
	pDibNew = *pDib;
	pDibNew->CreateWithoutGdi256(width * nSize, height * nSize);
	pData = pDibNew->GetBits();

	for (i = 0; i < (int)state.info_png.color.palettesize; i++) {
		const unsigned char *pEntry = &state.info_png.color.palette[i * 4];
		pDibNew->SetPallet(i, pEntry[0], pEntry[1], pEntry[2]);
	}

	for (y = 0; y < (int)height; y++) {
		BYTE *pDst = pData + (height - y - 1) * width;
		CopyMemory(pDst, &image[(size_t)y * width], width);
	}

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CMgrGrpData::Read256File(LPCTSTR pszFilePath, PCImg32 *pDib, int nSize)
{
	BOOL bRet;
	int i, y;
	std::vector<unsigned char> fileData;
	std::vector<unsigned char> image;
	unsigned width, height;
	lodepng::State state;
	CImg32 *pDibNew;
	BYTE *pData;

	bRet = FALSE;
	state.decoder.color_convert = 0;
	state.info_raw.colortype = LCT_PALETTE;
	state.info_raw.bitdepth = 8;

	if (!ReadBinaryFile(pszFilePath, fileData)) {
		goto Exit;
	}

	if (lodepng::decode(image, width, height, state, &fileData[0], fileData.size())) {
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

	if (*pDib == NULL) {
		*pDib = new CImg32;
	}
	pDibNew = *pDib;
	pDibNew->CreateWithoutGdi256(width * nSize, height * nSize);
	pData = pDibNew->GetBits();

	for (i = 0; i < (int)state.info_png.color.palettesize; i++) {
		const unsigned char *pEntry = &state.info_png.color.palette[i * 4];
		pDibNew->SetPallet(i, pEntry[0], pEntry[1], pEntry[2]);
	}

	for (y = 0; y < (int)height; y++) {
		BYTE *pDst = pData + (height - y - 1) * width;
		CopyMemory(pDst, &image[(size_t)y * width], width);
	}

	bRet = TRUE;
Exit:
	return bRet;
}
