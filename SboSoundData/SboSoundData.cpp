/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:SboSoundData.cpp											 */
/* ���e			:���֘ADLL �����t�@�C��										 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include <stdlib.h>
#include "resource.h"
#define SOUNDDATA_EXPORTS
#include "SboSoundData.h"

/* ========================================================================= */
/* �\���̂̒�`																 */
/* ========================================================================= */

/* ���ʉ���� */
typedef struct _SOUNDDATAINFO {
	DWORD	dwSoundID;			/* ���ʉ�ID */
	int		nResourceID;		/* ���\�[�XID */
	LPCSTR	pszSoundName;		/* ���ʉ��� */
} SOUNDDATAINFO, *PSOUNDDATAINFO;


/* ========================================================================= */
/* �萔�̒�`																 */
/* ========================================================================= */

static SOUNDDATAINFO g_aSoundDataTbl[] = {
	SOUNDID_OK_PI73,			IDR_WAVE_PI73,			"���艹(pi73.wav)",
	SOUNDID_FOOT_ASI_2,			IDR_WAVE_ASI_2,			"����(asi_2.wav)",
	SOUNDID_CURSORMOVE,			IDR_WAVE_SELECT,		"�J�[�\���ړ�",
	SOUNDID_CANCEL,				IDR_WAVE_PUSH51_C,		"�L�����Z��",
	SOUNDID_CHARI14_A,			IDR_WAVE_CHARI14_A,		"�퓬�J�n",
	SOUNDID_SWING24_C,			IDR_WAVE_SWING24_C,		"����U��",
	SOUNDID_SWING25_C,			IDR_WAVE_SWING25_C,		"����˂�",
	SOUNDID_HIT0,				IDR_WAVE_HIT0,			"�q�b�g0",
	SOUNDID_HIT1,				IDR_WAVE_HIT1,			"�q�b�g1",
	SOUNDID_DROP_MONEY,			IDR_WAVE_MONEY,			"�h���b�v:����",
	SOUNDID_DROP_ITEM_SMALL,	IDR_WAVE_ITEM_SMALL,	"�h���b�v:���A�C�e��",
	SOUNDID_DROP_ITEM_MIDIUM,	IDR_WAVE_ITEM_MIDIUM,	"�h���b�v:���A�C�e��",
	SOUNDID_DROP_ITEM_LARGE,	IDR_WAVE_ITEM_LARGE,	"�h���b�v:��A�C�e��",
	SOUNDID_DROP_MATERIAL,		IDR_WAVE_MATERIAL,		"�h���b�v:�f��",
	SOUNDID_DROP_WEAPON,		IDR_WAVE_WEAPON,		"�h���b�v:����",
	SOUNDID_DROP_CLOTH,			IDR_WAVE_CLOTH,			"�h���b�v:���E���[�u",
	SOUNDID_CRASH20_D,			IDR_WAVE_CRASH20_D,		"�M�����鉹",
	SOUNDID_KUSA,				IDR_WAVE_KUSA,			"�������鉹",
	SOUNDID_KAI,				IDR_WAVE_KAI,			"�L�����鉹",
	SOUNDID_CHAT,				IDR_WAVE_CHAT,			"�`���b�g",
	SOUNDID_W_CHAT,				IDR_WAVE_W_CHAT,		"�A�i�E���X",
	SOUNDID_CLAP02,				IDR_WAVE_CLAP02,		"����2",
	SOUNDID_CLAP03,				IDR_WAVE_CLAP03,		"����3",
	SOUNDID_CLAP0203,			IDR_WAVE_CLAP02,		"����2������3",
	SOUNDID_BALLOON1,			IDR_WAVE_BALLOON1,		"���o��",
	SOUNDID_BIKKURI,			IDR_WAVE_BIKKURI,		"�т�����",
	SOUNDID_WHISTLES,			IDR_WAVE_WHISTLES,		"�z�C�b�X���r",
	SOUNDID_WHISTLEL,			IDR_WAVE_WHISTLEL,		"�z�C�b�X���k",
	SOUNDID_GIMON,				IDR_WAVE_GIMON,			"�^��",
	SOUNDID_NIKU,				IDR_WAVE_NIKU,			"��",
	SOUNDID_KIZUKU,				IDR_WAVE_KIZUKU,		"�C�t��",
	SOUNDID_PINPONPANPON,		IDR_PINPONPANPON,		"�����O�̂��m�点��",
	SOUNDID_SWOON,				IDR_WAVE_SWOON,			"�C��",
	SOUNDID_URARA_WORKS,		IDR_WAVE_URARA_WORKS,	"�E�������[�N�X",
	SOUNDID_PUYON_ATACK,		IDR_WAVE_PUYON_ATACK,	"�v�����U��",
	SOUNDID_LIQUID,				IDR_WAVE_LIQUID,		"�t��",
	SOUNDID_BOW_ATACK,			IDR_WAVE_BOW_ATACK,		"�|�U��",
	SOUNDID_MEAL,				IDR_WAVE_MEAL,			"�H��",
	SOUNDID_DEFENSE,			IDR_WAVE_DEFENSE,		"�h��",
	SOUNDID_SHIELD,				IDR_WAVE_SHIELD,		"���\��",
	SOUNDID_PUYON_MOVE,			IDR_WAVE_PUYON_MOVE,	"�v�����ړ�",
	SOUNDID_PUYON_DEAD,			IDR_WAVE_PUYON_DEAD,	"�v�������S",
	SOUNDID_EXIST,				IDR_WAVE_EXIST,			"����",
	SOUNDID_TRASH,				IDR_WAVE_TRASH,			"�S�~���J���E�̂Ă�",
	SOUNDID_DROP_BIN,			IDR_WAVE_DROP_BIN,		"�h���b�v�F�r��",
	SOUNDID_OPEN_BAG,			IDR_WAVE_OPEN_BAG,		"�o�b�O�J��",
	SOUNDID_MISS,				IDR_WAVE_MISS,			"�~�X",
	SOUNDID_LVUP,				IDR_WAVE_LVUP,			"���x���A�b�v",
	SOUNDID_CRITICAL1,			IDR_WAVE_CRITICAL1,		"�a���N���e�B�J��",
	SOUNDID_HIT2,				IDR_WAVE_HIT2,			"�a���q�b�g�P",
	SOUNDID_HIT3,				IDR_WAVE_HIT3,			"�a���q�b�g�Q",
	SOUNDID_OPEN_WINDOW,		IDR_WAVE_OPEN_WINDOW,	"���J��",
	SOUNDID_CRITICAL2,			IDR_WAVE_CRITICAL2,		"�Ō��N���e�B�J��",
	SOUNDID_CRITICAL3,			IDR_WAVE_CRITICAL3,		"��N���e�B�J��",
	SOUNDID_HIT4,				IDR_WAVE_HIT4,			"��q�b�g",
	SOUNDID_CHARGE,				IDR_WAVE_CHARGE,		"����",
	SOUNDID_CHARGE2,			IDR_WAVE_CHARGE2,		"���߂Q",
	SOUNDID_DR_BASS01,			IDR_WAVE_DR_BASS01,		"�x�[�X�P",
	SOUNDID_DR_BASS02,			IDR_WAVE_DR_BASS02,		"�x�[�X�Q",
	SOUNDID_DR_BASS05,			IDR_WAVE_DR_BASS05,		"�x�[�X�T",
	SOUNDID_DR_CLAP01,			IDR_WAVE_DR_CLAP01,		"�N���b�v�P",
	SOUNDID_DR_SNE02,			IDR_WAVE_DR_SNE02,		"�X�l�A�Q",
	SOUNDID_KIE_ATACK,			IDR_WAVE_KIE_ATACK,		"�L�[�U��",
	SOUNDID_KIE_DEAD,			IDR_WAVE_KIE_DEAD,		"�L�[���S",
	SOUNDID_FLY,				IDR_WAVE_FLY,			"�H�΂���",
	0, 0, ""
};


/* ========================================================================= */
/* �O���[�o���ϐ��̒�`														 */
/* ========================================================================= */

static int	g_nSoundDataCount;							/* ���ʉ��� */


/* ========================================================================= */
/* �֐��̒�`																 */
/* ========================================================================= */

SOUNDDATA_API	int		GetSoundCount		(void);					/* ���ʉ������擾 */
SOUNDDATA_API	int		GetSoundResourceID	(int nNo);				/* ���ʉ��̃��\�[�XID���擾 */
SOUNDDATA_API	int		GetSoundNo			(DWORD dwSoundID);		/* ���ʉ��̃C���f�b�N�X���擾 */
SOUNDDATA_API	LPCSTR	GetSoundName		(DWORD dwSoundID);		/* ���ʉ������擾 */
static			void	Init				(void);					/* ������ */
static			PSOUNDDATAINFO	GetSoundDataPtr	(DWORD dwSoundID);	/* ���ʉ������擾 */


/* ========================================================================= */
/* �֐���	:DllMain														 */
/* ���e		:DLL���C������													 */
/* ���t		:2007/05/03														 */
/* ========================================================================= */

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	Init ();

    return TRUE;
}


/* ========================================================================= */
/* �֐���	:GetSoundCount													 */
/* ���e		:���ʉ������擾													 */
/* ���t		:2007/05/03														 */
/* ========================================================================= */

SOUNDDATA_API int GetSoundCount(void)
{
	return g_nSoundDataCount;
}


/* ========================================================================= */
/* �֐���	:GetSoundResourceID												 */
/* ���e		:���ʉ��̃��\�[�XID���擾										 */
/* ���t		:2007/05/03														 */
/* ========================================================================= */

SOUNDDATA_API int GetSoundResourceID(int nNo)
{
	int nRet;
	PSOUNDDATAINFO pSoundData;

	nRet = 0;

	if (nNo >= g_nSoundDataCount) {
		goto Exit;
	}
	pSoundData = &g_aSoundDataTbl[nNo];

	nRet = pSoundData->nResourceID;
Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:GetSoundID														 */
/* ���e		:���ʉ�ID���擾���擾											 */
/* ���t		:2007/09/19														 */
/* ========================================================================= */

SOUNDDATA_API DWORD GetSoundID(int nNo)
{
	DWORD dwRet;
	PSOUNDDATAINFO pSoundData;

	dwRet = 0;

	if (nNo >= g_nSoundDataCount) {
		goto Exit;
	}
	pSoundData = &g_aSoundDataTbl[nNo];

	dwRet = pSoundData->dwSoundID;
Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:GetSoundNo														 */
/* ���e		:���ʉ��̃C���f�b�N�X���擾										 */
/* ���t		:2007/05/03														 */
/* ========================================================================= */

SOUNDDATA_API int GetSoundNo(DWORD dwSoundID)
{
	int i, nRet;
	PSOUNDDATAINFO pSoundData;

	nRet = -1;

	if (dwSoundID == SOUNDID_CLAP0203) {
		dwSoundID = ((rand () % 100) > 50) ? SOUNDID_CLAP02 : SOUNDID_CLAP03;
	}

	for (i = 0; i < g_nSoundDataCount; i ++) {
		pSoundData = &g_aSoundDataTbl[i];
		if (pSoundData->dwSoundID != dwSoundID) {
			continue;
		}
		nRet = i;
		break;
	}

	return nRet;
}


/* ========================================================================= */
/* �֐���	:GetSoundName													 */
/* ���e		:���ʉ������擾													 */
/* ���t		:2007/05/03														 */
/* ========================================================================= */

SOUNDDATA_API LPCSTR GetSoundName(DWORD dwSoundID)
{
	LPCSTR pszRet;
	PSOUNDDATAINFO pSoundData;

	pszRet = "";

	pSoundData = GetSoundDataPtr (dwSoundID);
	if (pSoundData == NULL) {
		goto Exit;
	}

	pszRet = pSoundData->pszSoundName;
Exit:
	return pszRet;
}


/* ========================================================================= */
/* �֐���	:Init															 */
/* ���e		:������															 */
/* ���t		:2007/05/03														 */
/* ========================================================================= */

void Init(void)
{
	int i;
	PSOUNDDATAINFO pSoundData;

	/* ���ʉ��������߂� */
	g_nSoundDataCount = 0;
	for (i = 0; ; i ++) {
		pSoundData = &g_aSoundDataTbl[i];
		if (pSoundData->dwSoundID == 0) {
			break;
		}
		g_nSoundDataCount ++;
	}
}


/* ========================================================================= */
/* �֐���	:GetSoundDataPtr												 */
/* ���e		:���ʉ������擾												 */
/* ���t		:2007/05/03														 */
/* ========================================================================= */

PSOUNDDATAINFO GetSoundDataPtr(DWORD dwSoundID)
{
	int i;
	PSOUNDDATAINFO pRet, pSoundData;

	pRet = NULL;

	/* ���ʉ��������߂� */
	for (i = 0; i < g_nSoundDataCount; i ++) {
		pSoundData = &g_aSoundDataTbl[i];
		if (pSoundData->dwSoundID != dwSoundID) {
			continue;
		}
		pRet = pSoundData;
		break;
	}

	return pRet;
}

/* Copyright(C)URARA-works 2007 */
