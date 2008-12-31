/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrGrpData.h												 */
/* ���e			:�O���t�B�b�N�f�[�^�}�l�[�W���N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "../../SboGrpData/resource.h"
#include "Img32.h"
#include "png.h"
#include "MgrGrpData.h"


/* ========================================================================= */
/* �\���̒�`																 */
/* ========================================================================= */

typedef struct{
	PBYTE	Buffer;
	int		Pos;
} TPngFileBuffer;


/* ========================================================================= */
/* �֐���	:CMgrGrpData::CMgrGrpData										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/09/24														 */
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
/* �֐���	:CMgrGrpData::~CMgrGrpData										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/09/24														 */
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
/* �֐���	:CMgrGrpData::Load												 */
/* ���e		:�ǂݍ���														 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

BOOL CMgrGrpData::Load(void)
{
	BOOL bResult;
	int i;
	char szName[MAX_PATH], szTmp[64];
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

	/* �t�@�C�����̍쐬 */
	GetModuleFileName (NULL, szName, MAX_PATH);
	pszTmp		= strrchr (szName, '\\');
	pszTmp[1]	= 0;
	strcat (szName, "SboGrpData.dll");

	m_hDll = LoadLibrary (szName);
	if (m_hDll == NULL) {
		goto Exit;
	}

	/* �V�X�e�� */
	Read256 ("IDP_SYSTEM", &m_pImgSystem, 1);
	/* �A�C�R�� */
	Read256 ("IDP_ICON", &m_pImgIcon, 1);
	/* �e�摜 */
	Read256 ("IDP_SHADOW", &m_pImgShadow, 1);
	/* �A�C�e���摜 */
	Read256 ("IDP_ITEM_01", &m_pImgItem, 1);
	/* ����(S) */
	Read256 ("IDP_NUM_S", &m_pImgNumS, 1);
	/* ����(M) */
	Read256 ("IDP_NUM_M", &m_pImgNumM, 1);
	/* ����(L) */
	Read256 ("IDP_NUM_L", &m_pImgNumL, 1);

	/* �}�b�v */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_MAP_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgMapParts->Add (pImg);
	}
	m_nMapPartsCount = i;

	/* �}�b�v�e */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_MAPSHADOW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgMapShadow->Add (pImg);
	}
	m_nMapShadowCount = i;

	/* �� */
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
		/* �� */
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
	/* �� */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_CLOTH_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgCloth->Add (pImg);
	}
	/* �� */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_EYE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgEye->Add (pImg);
	}
	/* ��(��) */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_HAIR_D%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgHairD->Add (pImg);
	}
	/* ��(��) */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_HAIR_U%02d", i + 1);
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
		wsprintf (szTmp, "IDP_NPC_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgNPC->Add (pImg);
	}
	/* ���ꕞ */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_CLOTH_SP%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgSP->Add (pImg);
	}
	/* �A�N�Z�T�� */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_ACCE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgAcce->Add (pImg);
	}
	/* �G�t�F�N�g(32) */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_EFC_32_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgEfc32->Add (pImg);
	}
	/* �G�t�F�N�g(64) */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_EFC_64_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgEfc64->Add (pImg);
	}
	/* ���� */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_WEAPON_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeapon->Add (pImg);
	}
	/* ����(�|) */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_WEAPON_BOW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeaponBow->Add (pImg);
	}
	/* ����(��) */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_WEAPON_ARROW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeaponArrow->Add (pImg);
	}
	/* ����(�Ō�) */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_WEAPON_GLOVE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeaponGlove->Add (pImg);
	}
	/* ����(���̑�) */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_WEAPON_ETC_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgWeaponEtc->Add (pImg);
	}
	/* ���o�� */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_BALLOON_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImgEfcBalloon->Add (pImg);
	}
	/* 2x2�� */
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
		/* �� */
		pszTmp = apsz2x2BodyTbl[i + 1];
		if (pszTmp) {
			pImg	= new CImg32;
			bResult	= Read256 (pszTmp, &pImg, 1);
			if (bResult == FALSE) {
				delete pImg;
			}
		}
	}
	/* �� */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_2X2_CLOTH_%02d", i);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Cloth->Add (pImg);
	}
	/* �� */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_2X2_EYE_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Eye->Add (pImg);
	}
	/* �� */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_2X2_HAIR_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Hair->Add (pImg);
	}
	/* ���ꕞ */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_2X2_SP_CLOTH_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2SPCloth->Add (pImg);
	}
	/* ���ꔯ */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_2X2_SP_HAIR_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2SPHair->Add (pImg);
	}
	/* ������(2x2) */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_2X2_ARMS_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Arms->Add (pImg);
	}
	/* ��(2x2) */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_2X2_SHIELD_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2Shield->Add (pImg);
	}
	/* ���ꎝ����(2x2) */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_2X2_ARMS_SP_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2ArmsSP->Add (pImg);
	}
	/* �|(2x2) */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_2X2_BOW_%02d", i + 1);
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
		wsprintf (szTmp, "IDP_2X2_NPC_%03d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2NPC->Add (pImg);
	}
	/* �L�����e(2x2) */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_2X2_CHAR_SHADOW_%02d", i + 1);
		pImg	= new CImg32;
		bResult	= Read256 (szTmp, &pImg, 1);
		if (bResult == FALSE) {
			delete pImg;
			break;
		}
		m_paImg2x2CharShadow->Add (pImg);
	}
	/* NPC�e(2x2) */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_2X2_NPC_%03d_SHADOW", i + 1);
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
/* �֐���	:CMgrGrpData::CheckVersion										 */
/* ���e		:�o�[�W�����`�F�b�N												 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

BOOL CMgrGrpData::CheckVersion(LPCSTR pszVersion)
{
	BOOL bRet;
	char szTmp[64];

	bRet = FALSE;

	ZeroMemory (szTmp, sizeof (szTmp));
	/* �o�[�W�����`�F�b�N */
	LoadString (m_hDll, IDS_DLLVER, szTmp, sizeof (szTmp));
	if (strcmp (szTmp, pszVersion) != 0) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::Write												 */
/* ���e		:PNG�f�[�^�̕ۑ�												 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrGrpData::Write(LPCSTR pszFileName, CImg32 *pSrc)
{
	DWORD dwFileSize;
	PBYTE *pSaveData;
	FILE *fp;
	int x, y;
	png_structp png_ptr;
	png_infop info_ptr;

	fp = fopen (pszFileName, "wb");
	if (fp == NULL) {
		return;
	}

	png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info_ptr = png_create_info_struct (png_ptr);
	png_init_io (png_ptr, fp);
	png_set_IHDR (png_ptr, info_ptr, pSrc->Width (), pSrc->Height (),
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info (png_ptr, info_ptr);


	dwFileSize = pSrc->GetImageSize ();
	pSaveData = new PBYTE[pSrc->Height ()];
	ZeroMemory (pSaveData, sizeof (PBYTE) * pSrc->Height ());

	LPBYTE pData = pSrc->GetBits ();
	for (y = 0; y < pSrc->Height (); y ++) {
		pData = pSrc->GetBits () + y * (pSrc->Width () * 4);
		PBYTE pDataTmp = new BYTE[pSrc->Width () * 3];
		for (x = 0; x < pSrc->Width (); x ++) {
			pDataTmp[x * 3 + 0] = pData[2];
			pDataTmp[x * 3 + 1] = pData[1];
			pDataTmp[x * 3 + 2] = pData[0];
			pData += 4;
		}
		pSaveData[pSrc->Height () - 1 - y] = pDataTmp;
	}

	png_write_image (png_ptr, pSaveData);

	png_write_end (png_ptr, info_ptr); 
	png_destroy_write_struct (&png_ptr, &info_ptr); 
	fclose(fp);

	for (y = 0; y < pSrc->Height (); y ++) {
		delete [] pSaveData[y];
	}
	delete [] pSaveData;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibSystem										 */
/* ���e		:�V�X�e���摜�C���[�W���擾										 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibSystem(void)
{
	return m_pImgSystem;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibIcon										 */
/* ���e		:�A�C�R���摜�C���[�W���擾										 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibIcon(void)
{
	return m_pImgIcon;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibShadow										 */
/* ���e		:�e�摜�C���[�W���擾											 */
/* ���t		:2007/04/19														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibShadow(void)
{
	return m_pImgShadow;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibMapParts									 */
/* ���e		:�}�b�v�p�[�c�摜�C���[�W���擾									 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibMapParts(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImgMapParts->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgMapParts->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibMapShadow									 */
/* ���e		:�}�b�v�p�[�c�摜�C���[�W���擾									 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibMapShadow(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImgMapShadow->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgMapShadow->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibItem										 */
/* ���e		:�A�C�e���摜�C���[�W���擾										 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibItem(int nNo)
{
	return m_pImgItem;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibNumS										 */
/* ���e		:����(S)�摜�C���[�W���擾										 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibNumS(void)
{
	return m_pImgNumS;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibNumM										 */
/* ���e		:����(M)�摜�C���[�W���擾										 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibNumM(void)
{
	return m_pImgNumM;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibNumL										 */
/* ���e		:����(L)�摜�C���[�W���擾										 */
/* ���t		:2008/09/17														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibNumL(void)
{
	return m_pImgNumL;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibNPC											 */
/* ���e		:NPC�摜�C���[�W���擾											 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibNPC(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImgNPC->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgNPC->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibBody										 */
/* ���e		:�̉摜�C���[�W���擾											 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibBody(WORD wFamilyID)
{
	int nNo;
	PCImg32 pImg;

	nNo		= -1;
	pImg	= NULL;

	switch (wFamilyID) {
	case FAMILYTYPE_HUMAN:	nNo = 3;	break;	/* �j���Q�� */
	case FAMILYTYPE_ELF:	nNo = 2;	break;	/* �G���t */
	case FAMILYTYPE_BST:	nNo = 0;	break;	/* �W���E�W�� */
	case FAMILYTYPE_DAEMON:	nNo = 1;	break;	/* �}�]�N */
	default:
		goto Exit;
	}
	pImg = m_paImgBody->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibEar											 */
/* ���e		:���摜�C���[�W���擾											 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibEar(WORD wFamilyID)
{
	int nNo;
	PCImg32 pImg;

	nNo		= -1;
	pImg	= NULL;

	switch (wFamilyID) {
//	case FAMILYTYPE_HUMAN:	nNo = 3;	break;	/* �j���Q�� */
	case FAMILYTYPE_ELF:	nNo = 2;	break;	/* �G���t */
	case FAMILYTYPE_BST:	nNo = 0;	break;	/* �W���E�W�� */
	case FAMILYTYPE_DAEMON:	nNo = 1;	break;	/* �}�]�N */
	default:
		goto Exit;
	}
	pImg = m_paImgEar->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibCloth										 */
/* ���e		:���摜�C���[�W���擾											 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibCloth(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgCloth->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgCloth->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibEye											 */
/* ���e		:�ډ摜�C���[�W���擾											 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibEye(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgEye->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgEye->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibHairD										 */
/* ���e		:��(��)�摜�C���[�W���擾										 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibHairD(WORD wHairColorID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wHairColorID >= m_paImgHairD->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgHairD->GetAt (wHairColorID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibHairU										 */
/* ���e		:��(��)�摜�C���[�W���擾										 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibHairU(WORD wHairColorID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wHairColorID >= m_paImgHairU->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgHairU->GetAt (wHairColorID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibSP											 */
/* ���e		:���ꕞ�摜�C���[�W���擾										 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibSP(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgSP->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgSP->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibAcce										 */
/* ���e		:�A�N�Z�T���摜�C���[�W���擾									 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibAcce(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgAcce->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgAcce->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibEfc32										 */
/* ���e		:�G�t�F�N�g(32)�摜�C���[�W���擾								 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibEfc32(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgEfc32->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgEfc32->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibEfc(64)										 */
/* ���e		:�G�t�F�N�g(64)�摜�C���[�W���擾								 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibEfc64(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgEfc64->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgEfc64->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibWeapon										 */
/* ���e		:����摜�C���[�W���擾											 */
/* ���t		:2007/07/09														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibWeapon(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgWeapon->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgWeapon->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibWeaponBow									 */
/* ���e		:����(�|)�摜�C���[�W���擾										 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibWeaponBow(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgWeaponBow->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgWeaponBow->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibWeaponArrow									 */
/* ���e		:����(��)�摜�C���[�W���擾										 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibWeaponArrow(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgWeaponArrow->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgWeaponArrow->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibWeaponGlove									 */
/* ���e		:����(�Ō�)�摜�C���[�W���擾									 */
/* ���t		:2007/12/02														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibWeaponGlove(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgWeaponGlove->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgWeaponGlove->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibWeaponEtc									 */
/* ���e		:����(���̑�)�摜�C���[�W���擾									 */
/* ���t		:2007/12/02														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibWeaponEtc(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgWeaponEtc->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgWeaponEtc->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibEfcBalloon									 */
/* ���e		:���o���摜�C���[�W���擾										 */
/* ���t		:2007/12/26														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibEfcBalloon(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImgEfcBalloon->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgEfcBalloon->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDib2x2Body										 */
/* ���e		:2x2�̉摜�C���[�W���擾										 */
/* ���t		:2008/06/14														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2Body(WORD wFamilyID)
{
	int nNo;
	PCImg32 pImg;

	nNo		= -1;
	pImg	= NULL;

#if 0
	switch (wFamilyID) {
	case FAMILYTYPE_HUMAN:	nNo = 3;	break;	/* �j���Q�� */
	case FAMILYTYPE_ELF:	nNo = 2;	break;	/* �G���t */
	case FAMILYTYPE_BST:	nNo = 0;	break;	/* �W���E�W�� */
	case FAMILYTYPE_DAEMON:	nNo = 1;	break;	/* �}�]�N */
	default:
		goto Exit;
	}
#endif
	pImg = m_paImg2x2Body->GetAt (0);

//Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDib2x2Cloth									 */
/* ���e		:���摜�C���[�W���擾											 */
/* ���t		:2008/06/17														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2Cloth(WORD wClothID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wClothID == 0) {
		goto Exit;
	}
	if (wClothID >= m_paImg2x2Cloth->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImg2x2Cloth->GetAt (wClothID);

Exit:
	if (pImg == NULL) {
		pImg = m_paImg2x2Cloth->GetAt (0);
	}
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDib2x2Eye										 */
/* ���e		:�ډ摜�C���[�W���擾											 */
/* ���t		:2008/06/17														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2Eye(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	nNo  = max (0, nNo);
	if (nNo >= m_paImg2x2Eye->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImg2x2Eye->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDib2x2Hair										 */
/* ���e		:���摜�C���[�W���擾											 */
/* ���t		:2008/06/17														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2Hair(WORD wHairColorID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wHairColorID == 0) {
		goto Exit;
	}
	wHairColorID --;
	if (wHairColorID >= m_paImg2x2Hair->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImg2x2Hair->GetAt (wHairColorID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDib2x2SPCloth									 */
/* ���e		:���ꕞ�摜�C���[�W���擾										 */
/* ���t		:2008/06/20														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2SPCloth(WORD wSPClothID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wSPClothID == 0) {
		goto Exit;
	}
	wSPClothID --;
	if (wSPClothID >= m_paImg2x2SPCloth->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImg2x2SPCloth->GetAt (wSPClothID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDib2x2SPHair									 */
/* ���e		:���ꔯ�摜�C���[�W���擾										 */
/* ���t		:2008/06/20														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2SPHair(WORD wSPHairID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wSPHairID == 0) {
		goto Exit;
	}
	wSPHairID --;
	if (wSPHairID >= m_paImg2x2SPHair->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImg2x2SPHair->GetAt (wSPHairID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDib2x2Arms										 */
/* ���e		:2x2�������摜�C���[�W���擾									 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2Arms(WORD wArmsID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wArmsID >= m_paImg2x2Arms->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImg2x2Arms->GetAt (wArmsID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDib2x2Shield									 */
/* ���e		:2x2���摜�C���[�W���擾										 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2Shield(WORD wShieldID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wShieldID >= m_paImg2x2Shield->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImg2x2Shield->GetAt (wShieldID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDib2x2ArmsSP									 */
/* ���e		:2x2���ꎝ�����摜�C���[�W���擾								 */
/* ���t		:2008/06/30														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2ArmsSP(WORD wArmsID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wArmsID >= m_paImg2x2ArmsSP->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImg2x2ArmsSP->GetAt (wArmsID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDib2x2Bow										 */
/* ���e		:2x2�|�摜�C���[�W���擾										 */
/* ���t		:2008/07/02														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2Bow(WORD wBowID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wBowID >= m_paImg2x2Bow->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImg2x2Bow->GetAt (wBowID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDib2x2NPC										 */
/* ���e		:2x2NPC�摜�C���[�W���擾										 */
/* ���t		:2008/07/05														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2NPC(WORD wNPCID)
{
	PCImg32 pImg;

	pImg = NULL;
	if (wNPCID >= m_paImg2x2NPC->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImg2x2NPC->GetAt (wNPCID);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDib2x2CharShadow								 */
/* ���e		:2x2�L�����e�摜�C���[�W���擾									 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2CharShadow(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImg2x2CharShadow->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImg2x2CharShadow->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDib2x2NPCShadow								 */
/* ���e		:2x2NPC�e�摜�C���[�W���擾										 */
/* ���t		:2008/08/24														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib2x2NPCShadow(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImg2x2NPCShadow->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImg2x2NPCShadow->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDib											 */
/* ���e		:�摜�C���[�W���擾												 */
/* ���t		:2007/11/17														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDib(DWORD dwGrpIDMain, DWORD dwGrpIDSub, DWORD dwParam)
{
	PCImg32 pRet;

	pRet = NULL;

	switch (dwGrpIDMain) {
	case GRPIDMAIN_CHAR:			/* �L�����摜 */
		pRet = GetDibBody ((WORD)dwParam);
		break;
	case GRPIDMAIN_WEAPON:			/* ���� */
		pRet = GetDibWeapon (0);
		break;
	case GRPIDMAIN_WEAPON_BOW:		/* �| */
		pRet = GetDibWeaponBow (0);
		break;
	case GRPIDMAIN_EFFECT32:		/* �G�t�F�N�g(32) */
		pRet = GetDibEfc32 (0);
		break;
	case GRPIDMAIN_WEAPON_GLOVE:	/* ����(�Ō�) */
		pRet = GetDibWeaponGlove (0);
		break;
	case GRPIDMAIN_WEAPON_ETC:		/* ����(���̑�) */
		pRet = GetDibWeaponEtc (0);
		break;
	case GRPIDMAIN_NPC:				/* �L����(NPC) */
		pRet = GetDibNPC (0);
		break;
	case GRPIDMAIN_EFCBALLOON:		/* ���o�� */
		pRet = GetDibEfcBalloon (0);
		break;
	case GRPIDMAIN_2X2_CHAR:		/* �L����(32x32)�摜 */
		pRet = GetDib2x2Body ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_CLOTH:		/* ��(32x32) */
		pRet = GetDib2x2Cloth ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_EYE:			/* ��(32x32) */
		pRet = GetDib2x2Eye ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_HAIR:		/* ��(32x32) */
		if (dwParam < 10000) {
			pRet = GetDib2x2Hair ((WORD)dwParam);
		} else {
			pRet = GetDib2x2SPHair ((WORD)dwParam - 10000);
		}
		break;
	case GRPIDMAIN_2X2_SPCLOTH:		/* ���ꕞ(32x32) */
		pRet = GetDib2x2SPCloth ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_SPHAIR:		/* ���ꔯ(32x32) */
		pRet = GetDib2x2SPHair ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_ARMS:		/* ������(2x2) */
		dwParam = dwGrpIDSub / GetGrpCountX (dwGrpIDMain);
		dwParam = dwParam / GetGrpCountY (dwGrpIDMain);
		pRet = GetDib2x2Arms ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_SHIELD:		/* ��(2x2) */
		pRet = GetDib2x2Shield ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_ARMSSP:		/* ���ꎝ����(2x2) */
		pRet = GetDib2x2ArmsSP ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_BOW:			/* �|(2x2) */
		pRet = GetDib2x2Bow ((WORD)dwParam);
		break;
	case GRPIDMAIN_2X2_NPC:			/* NPC(2x2) */
		pRet = GetDib2x2NPC ((WORD)dwParam);
		break;
	case GRPIDMAIN_EFFECT64:		/* �G�t�F�N�g(64) */
		pRet = GetDibEfc64 (0);
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibTmpLogo										 */
/* ���e		:URARA-works���S�摜�C���[�W���擾								 */
/* ���t		:2007/05/01														 */
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
/* �֐���	:CMgrGrpData::GetDibTmpTitle									 */
/* ���e		:�^�C�g���摜�C���[�W���擾										 */
/* ���t		:2007/05/01														 */
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
/* �֐���	:CMgrGrpData::GetDibTmpTitleBack								 */
/* ���e		:�^�C�g���w�i�摜�C���[�W���擾									 */
/* ���t		:2007/05/01														 */
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
/* �֐���	:CMgrGrpData::GetDibTmpTitleCloud								 */
/* ���e		:�^�C�g���_�摜�C���[�W���擾									 */
/* ���t		:2008/05/02														 */
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
/* �֐���	:CMgrGrpData::GetDibTmpLoginMenuBack							 */
/* ���e		:���O�C�����j���[�w�i�摜�C���[�W���擾							 */
/* ���t		:2007/05/01														 */
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
/* �֐���	:CMgrGrpData::GetMapPartsCount									 */
/* ���e		:�}�b�v�p�[�c�����擾											 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

int CMgrGrpData::GetMapPartsCount(void)
{
	return m_nMapPartsCount * 1024;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibMapPartsTmp									 */
/* ���e		:���{�}�b�v�p�[�c�摜�C���[�W���擾								 */
/* ���t		:2007/05/14														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibMapPartsTmp(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImgMapPartsTmp->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgMapPartsTmp->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::ReadMapPartsTmp									 */
/* ���e		:���{�}�b�v�p�[�c�摜��ǂݍ���									 */
/* ���t		:2007/05/14														 */
/* ========================================================================= */

void CMgrGrpData::ReadMapPartsTmp(void)
{
	int i;
	BOOL bResult;
	char szTmp[128];
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo (m_paImgMapPartsTmp, pImg);
	/* �}�b�v */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_MAP_%02d", i + 1);
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
/* �֐���	:CMgrGrpData::DeleteMapPartsTmp									 */
/* ���e		:���{�}�b�v�p�[�c�摜���폜										 */
/* ���t		:2007/05/14														 */
/* ========================================================================= */

void CMgrGrpData::DeleteMapPartsTmp(void)
{
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo (m_paImgMapPartsTmp, pImg);
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetMapShadowCount									 */
/* ���e		:�}�b�v�e�摜�����擾											 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

int CMgrGrpData::GetMapShadowCount(void)
{
	return m_nMapShadowCount * 1024;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibMapShadowTmp								 */
/* ���e		:���{�}�b�v�e�摜�C���[�W���擾									 */
/* ���t		:2007/06/03														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibMapShadowTmp(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImgMapShadowTmp->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgMapShadowTmp->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::ReadMapShadowTmp									 */
/* ���e		:���{�}�b�v�e�摜��ǂݍ���										 */
/* ���t		:2007/06/03														 */
/* ========================================================================= */

void CMgrGrpData::ReadMapShadowTmp(void)
{
	int i;
	BOOL bResult;
	char szTmp[128];
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo (m_paImgMapShadowTmp, pImg);
	/* �}�b�v */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_MAPSHADOW_%02d", i + 1);
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
/* �֐���	:CMgrGrpData::DeleteMapShadowTmp								 */
/* ���e		:���{�}�b�v�e�摜���폜											 */
/* ���t		:2007/06/03														 */
/* ========================================================================= */

void CMgrGrpData::DeleteMapShadowTmp(void)
{
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo (m_paImgMapShadowTmp, pImg);
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetItemCount										 */
/* ���e		:�A�C�e���摜�����擾											 */
/* ���t		:2007/08/22														 */
/* ========================================================================= */

int CMgrGrpData::GetItemCount(void)
{
	return 1024;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetDibItemTmp										 */
/* ���e		:���{�A�C�e���摜�C���[�W���擾									 */
/* ���t		:2007/08/12														 */
/* ========================================================================= */

PCImg32 CMgrGrpData::GetDibItemTmp(int nNo)
{
	PCImg32 pImg;

	pImg = NULL;
	if (nNo >= m_paImgItemTmp->GetSize ()) {
		goto Exit;
	}

	pImg = m_paImgItemTmp->GetAt (nNo);

Exit:
	return pImg;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::ReadItemTmp										 */
/* ���e		:���{�A�C�e���摜��ǂݍ���										 */
/* ���t		:2007/08/12														 */
/* ========================================================================= */

void CMgrGrpData::ReadItemTmp(void)
{
	int i;
	BOOL bResult;
	char szTmp[128];
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo (m_paImgItemTmp, pImg);
	/* �A�C�e�� */
	for (i = 0; ; i ++) {
		wsprintf (szTmp, "IDP_ITEM_%02d", i + 1);
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
/* �֐���	:CMgrGrpData::DeleteItemTmp										 */
/* ���e		:���{�A�C�e���摜���폜											 */
/* ���t		:2007/08/12														 */
/* ========================================================================= */

void CMgrGrpData::DeleteItemTmp(void)
{
	PCImg32 pImg;

	pImg = NULL;

	FreeInfo (m_paImgItemTmp, pImg);
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetGrpSize										 */
/* ���e		:�摜�P������̃T�C�Y���擾									 */
/* ���t		:2007/11/17														 */
/* ========================================================================= */

int CMgrGrpData::GetGrpSize(DWORD dwGrpIDMain)
{
	int nRet;

	nRet = 0;

	switch (dwGrpIDMain) {
	case GRPIDMAIN_CHAR:			/* �L�����摜 */
	case GRPIDMAIN_NPC:				/* �L����(NPC) */
	case GRPIDMAIN_EFCBALLOON:		/* ���o�� */
	case GRPIDMAIN_2X2_SHIELD:		/* ��(2x2) */
		nRet = 16;
		break;
	case GRPIDMAIN_WEAPON:			/* ���� */
	case GRPIDMAIN_WEAPON_BOW:		/* �| */
	case GRPIDMAIN_EFFECT32:		/* �G�t�F�N�g(32) */
	case GRPIDMAIN_WEAPON_GLOVE:	/* ����(�Ō�) */
	case GRPIDMAIN_WEAPON_ETC:		/* ����(���̑�) */
	case GRPIDMAIN_2X2_CHAR:		/* �L����(32x32)�摜 */
	case GRPIDMAIN_2X2_CLOTH:		/* ��(32x32) */
	case GRPIDMAIN_2X2_EYE:			/* ��(32x32) */
	case GRPIDMAIN_2X2_HAIR:		/* ��(32x32) */
	case GRPIDMAIN_2X2_SPCLOTH:		/* ���ꕞ(32x32) */
	case GRPIDMAIN_2X2_SPHAIR:		/* ���ꔯ(32x32) */
	case GRPIDMAIN_2X2_NPC:			/* NPC(2x2) */
		nRet = 32;
		break;
	case GRPIDMAIN_2X2_ARMS:		/* ������(2x2) */
	case GRPIDMAIN_2X2_ARMSSP:		/* ���ꎝ����(2x2) */
	case GRPIDMAIN_2X2_BOW:			/* �|(2x2) */
		nRet = 24;
		break;
	case GRPIDMAIN_EFFECT64:		/* �G�t�F�N�g(64) */
		nRet = 64;
		break;
	}

	return nRet;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetGrpCountX										 */
/* ���e		:�摜�P�t�@�C���̉������擾									 */
/* ���t		:2007/11/17														 */
/* ========================================================================= */

int CMgrGrpData::GetGrpCountX(DWORD dwGrpIDMain)
{
	int nRet;

	nRet = 0;

	switch (dwGrpIDMain) {
	case GRPIDMAIN_CHAR:			/* �L�����摜 */
		nRet = 32;
		break;
	case GRPIDMAIN_NPC:				/* �L����(NPC) */
		nRet = 16;
		break;
	case GRPIDMAIN_WEAPON:			/* ���� */
		nRet = 32;
		break;
	case GRPIDMAIN_WEAPON_BOW:		/* �| */
	case GRPIDMAIN_WEAPON_GLOVE:	/* ����(�Ō�) */
	case GRPIDMAIN_WEAPON_ETC:		/* ����(���̑�) */
		nRet = 20;
		break;
	case GRPIDMAIN_EFFECT32:		/* �G�t�F�N�g(32) */
		nRet = 16;
		break;
	case GRPIDMAIN_EFCBALLOON:		/* ���o�� */
		nRet = 10;
		break;
	case GRPIDMAIN_2X2_CHAR:		/* �L����(32x32)�摜 */
	case GRPIDMAIN_2X2_CLOTH:		/* ��(32x32) */
	case GRPIDMAIN_2X2_HAIR:		/* ��(32x32) */
	case GRPIDMAIN_2X2_SPCLOTH:		/* ���ꕞ(32x32) */
	case GRPIDMAIN_2X2_SPHAIR:		/* ���ꔯ(32x32) */
	case GRPIDMAIN_2X2_NPC:			/* NPC(2x2) */
		nRet = 16;
		break;
	case GRPIDMAIN_2X2_EYE:			/* ��(32x32) */
	case GRPIDMAIN_2X2_ARMS:		/* ������(2x2) */
		nRet = 12;
		break;
	case GRPIDMAIN_2X2_ARMSSP:		/* ���ꎝ����(2x2) */
	case GRPIDMAIN_2X2_BOW:			/* �|(2x2) */
		nRet = 11;
		break;
	case GRPIDMAIN_2X2_SHIELD:		/* ��(2x2) */
		nRet = 5;
		break;
	case GRPIDMAIN_EFFECT64:		/* �G�t�F�N�g(64) */
		nRet = 8;
		break;
	}

	return nRet;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetGrpCountY										 */
/* ���e		:�摜�P�t�@�C���̏c�����擾									 */
/* ���t		:2007/11/24														 */
/* ========================================================================= */

int CMgrGrpData::GetGrpCountY(DWORD dwGrpIDMain)
{
	int nRet;

	nRet = 0;

	switch (dwGrpIDMain) {
	case GRPIDMAIN_CHAR:			/* �L�����摜 */
		nRet = 1;
		break;
	case GRPIDMAIN_WEAPON:			/* ���� */
	case GRPIDMAIN_WEAPON_BOW:		/* �| */
	case GRPIDMAIN_WEAPON_GLOVE:	/* ����(�Ō�) */
	case GRPIDMAIN_WEAPON_ETC:		/* ����(���̑�) */
		nRet = 18;
		break;
	case GRPIDMAIN_EFFECT32:		/* �G�t�F�N�g(32) */
		nRet = 16;
		break;
	case GRPIDMAIN_NPC:				/* �L����(NPC) */
		nRet = 32;
		break;
	case GRPIDMAIN_EFCBALLOON:		/* ���o�� */
		nRet = 15;
		break;
	case GRPIDMAIN_2X2_CHAR:		/* �L����(32x32)�摜 */
	case GRPIDMAIN_2X2_CLOTH:		/* ��(32x32) */
	case GRPIDMAIN_2X2_EYE:			/* ��(32x32) */
	case GRPIDMAIN_2X2_HAIR:		/* ��(32x32) */
	case GRPIDMAIN_2X2_SPCLOTH:		/* ���ꕞ(32x32) */
	case GRPIDMAIN_2X2_SPHAIR:		/* ���ꔯ(32x32) */
	case GRPIDMAIN_2X2_NPC:			/* NPC(2x2) */
		nRet = 8;
		break;
	case GRPIDMAIN_2X2_ARMS:		/* ������(2x2) */
	case GRPIDMAIN_2X2_SHIELD:		/* ��(2x2) */
	case GRPIDMAIN_2X2_ARMSSP:		/* ���ꎝ����(2x2) */
	case GRPIDMAIN_2X2_BOW:			/* �|(2x2) */
		nRet = 25;
		break;
	case GRPIDMAIN_EFFECT64:		/* �G�t�F�N�g(64) */
		nRet = 8;
		break;
	}

	return nRet;
}


/* ========================================================================= */
/* �֐���	:CMgrGrpData::GetGrpPos											 */
/* ���e		:�w��摜�̈ʒu���擾											 */
/* ���t		:2007/11/24														 */
/* ========================================================================= */

void CMgrGrpData::GetGrpPos(
	DWORD dwGrpIDMain,		/* [in] �摜ID���C�� */
	DWORD dwGrpIDSub,		/* [in] �摜ID�T�u */
	POINT &ptDst,			/* [ou] �摜�ʒu */
	WORD wGrpID)			/* [in] �c�ʒu�����炷�� */
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
/* �֐���	:PngReadFunc													 */
/* ���e		:PNG�ǂݍ��ݗp�R�[���o�b�N�֐�									 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

static void PngReadFunc(png_struct *Png,png_bytep buf,png_size_t size)
{
	TPngFileBuffer *PngFileBuffer = (TPngFileBuffer*)png_get_io_ptr (Png);
    memcpy (buf, PngFileBuffer->Buffer+PngFileBuffer->Pos, size);
	PngFileBuffer->Pos += size;
}


/* ========================================================================= */
/* �֐���	:Read															 */
/* ���e		:PNG�f�[�^�̓ǂݍ���											 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

BOOL CMgrGrpData::Read(LPSTR pszName, PCImg32 *pDib, int nSize)
{
	BOOL bRet;
	int x, y, BitDepth, ColorType, InterlaceType, CompressionType,
		FilterType, PaletteCount;
	PBYTE pTmp, pData;
	HDC hDCBmp, hDCBmp2;
	png_uint_32 Width, Height;
	png_struct *pPng;
	png_info *pInfo;
	png_byte *pLineBuf;
	TPngFileBuffer PngFileBuffer = {NULL, 0};
	CImg32 *pDibNew;
	CImg32 *pDibTmp, *pDibTmp2;

	bRet			= FALSE;
	PaletteCount	= 0;
	pPng			= NULL;
	pInfo			= NULL;
	pLineBuf		= NULL;
	pDibTmp			= NULL;
	pDibTmp2		= NULL;

	/* �w�肳�ꂽ���\�[�X��ǂݍ��� */
	HGLOBAL hRes = LoadResource (m_hDll, FindResource (m_hDll, pszName, "PNG"));
	if (hRes == NULL) {
		goto Exit;
	}

	if (*pDib == NULL) {
		*pDib = new CImg32;
	}
	pDibTmp	= new CImg32;
	pDibNew	= *pDib;
	pPng	= NULL;
	pInfo	= NULL;

	pPng	= png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	pInfo	= png_create_info_struct (pPng);

	PngFileBuffer.Buffer	= (BYTE *)LockResource (hRes);
	PngFileBuffer.Pos		= 0;

	png_set_read_fn (pPng, (png_voidp)&PngFileBuffer, (png_rw_ptr)PngReadFunc);

	png_read_info (pPng, pInfo);
	png_get_IHDR (pPng, pInfo, &Width, &Height, &BitDepth, &ColorType, &InterlaceType, &CompressionType, &FilterType);
	BitDepth = 8;

	png_set_bgr (pPng);

	png_colorp Palette;
	if (BitDepth == 8) {
		png_get_PLTE (pPng, pInfo, &Palette, &PaletteCount);
	}

	pLineBuf = new png_byte[Width * BitDepth / 8];

	pDibTmp->Create (Width, Height);
	pDibNew->CreateWithoutGdi (Width * nSize, Height * nSize);
	pData	= pDibTmp->GetBits ();
	pTmp	= pData;

	/* �P���C������PNG�f�[�^��ǂݍ��� */
	for (y = 0; y < (int)Height; y ++) {
		png_read_row (pPng, pLineBuf, NULL);
		pTmp = pData + (Height - y - 1) * Width * 4;
		   for (x = 0; x < (int)Width; x ++) {
			BYTE *Src = pLineBuf + x * BitDepth / 8;
			if (BitDepth == 8) {
				pTmp[0] = Palette[*Src].blue;
				pTmp[1] = Palette[*Src].green;
				pTmp[2] = Palette[*Src].red;
			} else {
				CopyMemory (pTmp, Src, 3);
			}
			pTmp += 4;
		}
	}
	png_destroy_read_struct (&pPng, &pInfo, NULL);

	pDibTmp2 = new CImg32;
	pDibTmp2->Create (Width * nSize, Height * nSize);
	hDCBmp	= pDibTmp->Lock ();
	hDCBmp2	= pDibTmp2->Lock ();

	/* �w��T�C�Y�Ɋg�債��DIB�ɃR�s�[ */
	StretchBlt (hDCBmp2, 0, 0, Width * nSize, Height * nSize,
			hDCBmp, 0, 0, Width, Height, SRCCOPY);

	pDibTmp->Unlock ();
	pDibTmp2->Unlock ();

	pDibNew->Blt (0, 0, Width * nSize, Height * nSize, pDibTmp2, 0, 0);

	bRet = TRUE;
Exit:
	SAFE_DELETE (pDibTmp);
	SAFE_DELETE (pDibTmp2);
	SAFE_DELETE_ARRAY (pLineBuf);
	return bRet;
}


/* ========================================================================= */
/* �֐���	:Read256														 */
/* ���e		:PNG�f�[�^�̓ǂݍ���											 */
/* ���t		:2008/07/04														 */
/* ========================================================================= */

BOOL CMgrGrpData::Read256(LPSTR pszName, PCImg32 *pDib, int nSize)
{
	BOOL bRet;
	int i, x, y, BitDepth, ColorType, InterlaceType, CompressionType,
		FilterType, PaletteCount;
	PBYTE pTmp, pData;
	png_uint_32 Width, Height;
	png_struct *pPng;
	png_info *pInfo;
	png_byte *pLineBuf;
	TPngFileBuffer PngFileBuffer = {NULL, 0};
	CImg32 *pDibNew;
	CImg32 *pDibTmp, *pDibTmp2;

	bRet			= FALSE;
	PaletteCount	= 0;
	pPng			= NULL;
	pInfo			= NULL;
	pLineBuf		= NULL;
	pDibTmp			= NULL;
	pDibTmp2		= NULL;

	/* �w�肳�ꂽ���\�[�X��ǂݍ��� */
	HGLOBAL hRes = LoadResource (m_hDll, FindResource (m_hDll, pszName, "PNG"));
	if (hRes == NULL) {
		goto Exit;
	}

	if (*pDib == NULL) {
		*pDib = new CImg32;
	}
	pDibNew	= *pDib;
	pPng	= NULL;
	pInfo	= NULL;

	pPng	= png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	pInfo	= png_create_info_struct (pPng);

	PngFileBuffer.Buffer	= (BYTE *)LockResource (hRes);
	PngFileBuffer.Pos		= 0;

	png_set_read_fn (pPng, (png_voidp)&PngFileBuffer, (png_rw_ptr)PngReadFunc);

	png_read_info (pPng, pInfo);
	png_get_IHDR (pPng, pInfo, &Width, &Height, &BitDepth, &ColorType, &InterlaceType, &CompressionType, &FilterType);
	BitDepth = 8;

	png_set_bgr (pPng);

	png_colorp Palette;
	if (BitDepth == 8) {
		png_get_PLTE (pPng, pInfo, &Palette, &PaletteCount);
	}

	pLineBuf = new png_byte[Width * BitDepth / 8];

	pDibNew->CreateWithoutGdi256 (Width * nSize, Height * nSize);
	pData	= pDibNew->GetBits ();
	pTmp	= pData;

	for (i = 0; i < PaletteCount; i ++) {
		pDibNew->SetPallet (i, Palette[i].red, Palette[i].green, Palette[i].blue);
	}

	/* �P���C������PNG�f�[�^��ǂݍ��� */
	for (y = 0; y < (int)Height; y ++) {
		png_read_row (pPng, pLineBuf, NULL);
		pTmp = pData + (Height - y - 1) * Width;
		BYTE *Src = pLineBuf + 0 * BitDepth / 8;
		for (x = 0; x < (int)Width; x ++) {
			if (BitDepth == 8) {
				pTmp[x] = Src[x];
			}
		}
	}
	png_destroy_read_struct (&pPng, &pInfo, NULL);

	bRet = TRUE;
Exit:
	SAFE_DELETE_ARRAY (pLineBuf);
	return bRet;
}

/* Copyright(C)URARA-works 2006 */
