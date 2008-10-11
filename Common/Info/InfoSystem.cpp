/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoSystem.cpp												 */
/* 内容			:システム情報クラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/25													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoSystem.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"wStamina",					/* スタミナ */
	"wPower",					/* 腕力 */
	"wStrength",				/* 体力 */
	"wMagic",					/* 魔力 */
	"wSkillful",				/* 器用 */
	"wAbillityAT",				/* 攻撃技能 */
	"wAbillityDF",				/* 防御技能 */
	"wPAtack",					/* 攻撃力 */
	"wPDefense",				/* 防御力 */
	"wPMagic",					/* 魔法力 */
	"wPMagicDefense",			/* 魔法防御力 */
	"wPHitAverage",				/* 命中率 */
	"wPAvoidAverage",			/* 回避率 */
	"wPCriticalAverage",		/* クリティカル率 */
	"wAttrFire",				/* 属性[火] */
	"wAttrWind",				/* 属性[風] */
	"wAttrWater",				/* 属性[水] */
	"wAttrEarth",				/* 属性[土] */
	"wAttrLight",				/* 属性[光] */
	"wAttrDark",				/* 属性[闇] */
	"dwMaxHP",					/* 最大HP */
	"dwMaxSP",					/* 最大SP */
	"dwInitPosMapID",			/* 初期位置マップID */
	"ptInitPos",				/* 初期位置座標 */
	NULL
};

/* ========================================================================= */
/* 関数名	:CInfoSystem::CInfoSystem										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/09/25														 */
/* ========================================================================= */

CInfoSystem::CInfoSystem()
{
	m_pInitCharStatus = new STSYSTEM_INITCHARSTATUS;

	ZeroMemory (m_pInitCharStatus, sizeof (STSYSTEM_INITCHARSTATUS));
	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoSystem::~CInfoSystem										 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/09/25														 */
/* ========================================================================= */

CInfoSystem::~CInfoSystem()
{
	SAFE_DELETE (m_pInitCharStatus);
}


/* ========================================================================= */
/* 関数名	:CInfoSystem::GetElementNo										 */
/* 内容		:要素番号を取得													 */
/* 日付		:2008/09/25														 */
/* ========================================================================= */

int CInfoSystem::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = -1;

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSystem::GetDataSize										 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/09/25														 */
/* ========================================================================= */

DWORD CInfoSystem::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_pInitCharStatus->wStamina);				/* スタミナ */
	dwRet += sizeof (m_pInitCharStatus->wPower);				/* 腕力 */
	dwRet += sizeof (m_pInitCharStatus->wStrength);				/* 体力 */
	dwRet += sizeof (m_pInitCharStatus->wMagic);				/* 魔力 */
	dwRet += sizeof (m_pInitCharStatus->wSkillful);				/* 器用 */
	dwRet += sizeof (m_pInitCharStatus->wAbillityAT);			/* 攻撃技能 */
	dwRet += sizeof (m_pInitCharStatus->wAbillityDF);			/* 防御技能 */
	dwRet += sizeof (m_pInitCharStatus->wPAtack);				/* 攻撃力 */
	dwRet += sizeof (m_pInitCharStatus->wPDefense);				/* 防御力 */
	dwRet += sizeof (m_pInitCharStatus->wPMagic);				/* 魔法力 */
	dwRet += sizeof (m_pInitCharStatus->wPMagicDefense);		/* 魔法防御力 */
	dwRet += sizeof (m_pInitCharStatus->wPHitAverage);			/* 命中率 */
	dwRet += sizeof (m_pInitCharStatus->wPAvoidAverage);		/* 回避率 */
	dwRet += sizeof (m_pInitCharStatus->wPCriticalAverage);		/* クリティカル率 */
	dwRet += sizeof (m_pInitCharStatus->wAttrFire);				/* 属性[火] */
	dwRet += sizeof (m_pInitCharStatus->wAttrWind);				/* 属性[風] */
	dwRet += sizeof (m_pInitCharStatus->wAttrWater);			/* 属性[水] */
	dwRet += sizeof (m_pInitCharStatus->wAttrEarth);			/* 属性[土] */
	dwRet += sizeof (m_pInitCharStatus->wAttrLight);			/* 属性[光] */
	dwRet += sizeof (m_pInitCharStatus->wAttrDark);				/* 属性[闇] */
	dwRet += sizeof (m_pInitCharStatus->dwMaxHP);				/* 最大HP */
	dwRet += sizeof (m_pInitCharStatus->dwMaxSP);				/* 最大SP */
	dwRet += sizeof (m_pInitCharStatus->dwInitPosMapID);		/* 初期位置マップID */
	dwRet += sizeof (m_pInitCharStatus->ptInitPos);				/* 初期位置座標 */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSystem::GetDataSizeNo										 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2008/09/25														 */
/* ========================================================================= */

DWORD CInfoSystem::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_pInitCharStatus->wStamina);			break;	/* スタミナ */
	case 1:		dwRet = sizeof (m_pInitCharStatus->wPower);				break;	/* 腕力 */
	case 2:		dwRet = sizeof (m_pInitCharStatus->wStrength);			break;	/* 体力 */
	case 3:		dwRet = sizeof (m_pInitCharStatus->wMagic);				break;	/* 魔力 */
	case 4:		dwRet = sizeof (m_pInitCharStatus->wSkillful);			break;	/* 器用 */
	case 5:		dwRet = sizeof (m_pInitCharStatus->wAbillityAT);		break;	/* 攻撃技能 */
	case 6:		dwRet = sizeof (m_pInitCharStatus->wAbillityDF);		break;	/* 防御技能 */
	case 7:		dwRet = sizeof (m_pInitCharStatus->wPAtack);			break;	/* 攻撃力 */
	case 8:		dwRet = sizeof (m_pInitCharStatus->wPDefense);			break;	/* 防御力 */
	case 9:		dwRet = sizeof (m_pInitCharStatus->wPMagic);			break;	/* 魔法力 */
	case 10:	dwRet = sizeof (m_pInitCharStatus->wPMagicDefense);		break;	/* 魔法防御力 */
	case 11:	dwRet = sizeof (m_pInitCharStatus->wPHitAverage);		break;	/* 命中率 */
	case 12:	dwRet = sizeof (m_pInitCharStatus->wPAvoidAverage);		break;	/* 回避率 */
	case 13:	dwRet = sizeof (m_pInitCharStatus->wPCriticalAverage);	break;	/* クリティカル率 */
	case 14:	dwRet = sizeof (m_pInitCharStatus->wAttrFire);			break;	/* 属性[火] */
	case 15:	dwRet = sizeof (m_pInitCharStatus->wAttrWind);			break;	/* 属性[風] */
	case 16:	dwRet = sizeof (m_pInitCharStatus->wAttrWater);			break;	/* 属性[水] */
	case 17:	dwRet = sizeof (m_pInitCharStatus->wAttrEarth);			break;	/* 属性[土] */
	case 18:	dwRet = sizeof (m_pInitCharStatus->wAttrLight);			break;	/* 属性[光] */
	case 19:	dwRet = sizeof (m_pInitCharStatus->wAttrDark);			break;	/* 属性[闇] */
	case 20:	dwRet = sizeof (m_pInitCharStatus->dwMaxHP);			break;	/* 最大HP */
	case 21:	dwRet = sizeof (m_pInitCharStatus->dwMaxSP);			break;	/* 最大SP */
	case 22:	dwRet = sizeof (m_pInitCharStatus->dwInitPosMapID);		break;	/* 初期位置マップID */
	case 23:	dwRet = sizeof (m_pInitCharStatus->ptInitPos);			break;	/* 初期位置座標 */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSystem::GetName											 */
/* 内容		:要素名を取得													 */
/* 日付		:2008/09/25														 */
/* ========================================================================= */

LPCSTR CInfoSystem::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoSystem::GetWriteData										 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/09/25														 */
/* ========================================================================= */

PBYTE CInfoSystem::GetWriteData(int nNo, PDWORD pdwSize)
{
	PBYTE pRet, pSrc;
	DWORD dwSize;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:		pSrc = (PBYTE)&m_pInitCharStatus->wStamina;				break;	/* スタミナ */
	case 1:		pSrc = (PBYTE)&m_pInitCharStatus->wPower;				break;	/* 腕力 */
	case 2:		pSrc = (PBYTE)&m_pInitCharStatus->wStrength;			break;	/* 体力 */
	case 3:		pSrc = (PBYTE)&m_pInitCharStatus->wMagic;				break;	/* 魔力 */
	case 4:		pSrc = (PBYTE)&m_pInitCharStatus->wSkillful;			break;	/* 器用 */
	case 5:		pSrc = (PBYTE)&m_pInitCharStatus->wAbillityAT;			break;	/* 攻撃技能 */
	case 6:		pSrc = (PBYTE)&m_pInitCharStatus->wAbillityDF;			break;	/* 防御技能 */
	case 7:		pSrc = (PBYTE)&m_pInitCharStatus->wPAtack;				break;	/* 攻撃力 */
	case 8:		pSrc = (PBYTE)&m_pInitCharStatus->wPDefense;			break;	/* 防御力 */
	case 9:		pSrc = (PBYTE)&m_pInitCharStatus->wPMagic;				break;	/* 魔法力 */
	case 10:	pSrc = (PBYTE)&m_pInitCharStatus->wPMagicDefense;		break;	/* 魔法防御力 */
	case 11:	pSrc = (PBYTE)&m_pInitCharStatus->wPHitAverage;			break;	/* 命中率 */
	case 12:	pSrc = (PBYTE)&m_pInitCharStatus->wPAvoidAverage;		break;	/* 回避率 */
	case 13:	pSrc = (PBYTE)&m_pInitCharStatus->wPCriticalAverage;	break;	/* クリティカル率 */
	case 14:	pSrc = (PBYTE)&m_pInitCharStatus->wAttrFire;			break;	/* 属性[火] */
	case 15:	pSrc = (PBYTE)&m_pInitCharStatus->wAttrWind;			break;	/* 属性[風] */
	case 16:	pSrc = (PBYTE)&m_pInitCharStatus->wAttrWater;			break;	/* 属性[水] */
	case 17:	pSrc = (PBYTE)&m_pInitCharStatus->wAttrEarth;			break;	/* 属性[土] */
	case 18:	pSrc = (PBYTE)&m_pInitCharStatus->wAttrLight;			break;	/* 属性[光] */
	case 19:	pSrc = (PBYTE)&m_pInitCharStatus->wAttrDark;			break;	/* 属性[闇] */
	case 20:	pSrc = (PBYTE)&m_pInitCharStatus->dwMaxHP;				break;	/* 最大HP */
	case 21:	pSrc = (PBYTE)&m_pInitCharStatus->dwMaxSP;				break;	/* 最大SP */
	case 22:	pSrc = (PBYTE)&m_pInitCharStatus->dwInitPosMapID;		break;	/* 初期位置マップID */
	case 23:	pSrc = (PBYTE)&m_pInitCharStatus->ptInitPos;			break;	/* 初期位置座標 */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSystem::ReadElementData									 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2008/09/25														 */
/* ========================================================================= */

DWORD CInfoSystem::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_pInitCharStatus->wStamina;				dwSize = sizeof (m_pInitCharStatus->wStamina);			break;	/* スタミナ */
	case 1:		pDst = (PBYTE)&m_pInitCharStatus->wPower;				dwSize = sizeof (m_pInitCharStatus->wPower);			break;	/* 腕力 */
	case 2:		pDst = (PBYTE)&m_pInitCharStatus->wStrength;			dwSize = sizeof (m_pInitCharStatus->wStrength);			break;	/* 体力 */
	case 3:		pDst = (PBYTE)&m_pInitCharStatus->wMagic;				dwSize = sizeof (m_pInitCharStatus->wMagic);			break;	/* 魔力 */
	case 4:		pDst = (PBYTE)&m_pInitCharStatus->wSkillful;			dwSize = sizeof (m_pInitCharStatus->wSkillful);			break;	/* 器用 */
	case 5:		pDst = (PBYTE)&m_pInitCharStatus->wAbillityAT;			dwSize = sizeof (m_pInitCharStatus->wAbillityAT);		break;	/* 攻撃技能 */
	case 6:		pDst = (PBYTE)&m_pInitCharStatus->wAbillityDF;			dwSize = sizeof (m_pInitCharStatus->wAbillityDF);		break;	/* 防御技能 */
	case 7:		pDst = (PBYTE)&m_pInitCharStatus->wPAtack;				dwSize = sizeof (m_pInitCharStatus->wPAtack);			break;	/* 攻撃力 */
	case 8:		pDst = (PBYTE)&m_pInitCharStatus->wPDefense;			dwSize = sizeof (m_pInitCharStatus->wPDefense);			break;	/* 防御力 */
	case 9:		pDst = (PBYTE)&m_pInitCharStatus->wPMagic;				dwSize = sizeof (m_pInitCharStatus->wPMagic);			break;	/* 魔法力 */
	case 10:	pDst = (PBYTE)&m_pInitCharStatus->wPMagicDefense;		dwSize = sizeof (m_pInitCharStatus->wPMagicDefense);	break;	/* 魔法防御力 */
	case 11:	pDst = (PBYTE)&m_pInitCharStatus->wPHitAverage;			dwSize = sizeof (m_pInitCharStatus->wPHitAverage);		break;	/* 命中率 */
	case 12:	pDst = (PBYTE)&m_pInitCharStatus->wPAvoidAverage;		dwSize = sizeof (m_pInitCharStatus->wPAvoidAverage);	break;	/* 回避率 */
	case 13:	pDst = (PBYTE)&m_pInitCharStatus->wPCriticalAverage;	dwSize = sizeof (m_pInitCharStatus->wPCriticalAverage);	break;	/* クリティカル率 */
	case 14:	pDst = (PBYTE)&m_pInitCharStatus->wAttrFire;			dwSize = sizeof (m_pInitCharStatus->wAttrFire);			break;	/* 属性[火] */
	case 15:	pDst = (PBYTE)&m_pInitCharStatus->wAttrWind;			dwSize = sizeof (m_pInitCharStatus->wAttrWind);			break;	/* 属性[風] */
	case 16:	pDst = (PBYTE)&m_pInitCharStatus->wAttrWater;			dwSize = sizeof (m_pInitCharStatus->wAttrWater);		break;	/* 属性[水] */
	case 17:	pDst = (PBYTE)&m_pInitCharStatus->wAttrEarth;			dwSize = sizeof (m_pInitCharStatus->wAttrEarth);		break;	/* 属性[土] */
	case 18:	pDst = (PBYTE)&m_pInitCharStatus->wAttrLight;			dwSize = sizeof (m_pInitCharStatus->wAttrLight);		break;	/* 属性[光] */
	case 19:	pDst = (PBYTE)&m_pInitCharStatus->wAttrDark;			dwSize = sizeof (m_pInitCharStatus->wAttrDark);			break;	/* 属性[闇] */
	case 20:	pDst = (PBYTE)&m_pInitCharStatus->dwMaxHP;				dwSize = sizeof (m_pInitCharStatus->dwMaxHP);			break;	/* 最大HP */
	case 21:	pDst = (PBYTE)&m_pInitCharStatus->dwMaxSP;				dwSize = sizeof (m_pInitCharStatus->dwMaxSP);			break;	/* 最大SP */
	case 22:	pDst = (PBYTE)&m_pInitCharStatus->dwInitPosMapID;		dwSize = sizeof (m_pInitCharStatus->dwInitPosMapID);	break;	/* 初期位置マップID */
	case 23:	pDst = (PBYTE)&m_pInitCharStatus->ptInitPos;			dwSize = sizeof (m_pInitCharStatus->ptInitPos);			break;	/* 初期位置座標 */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoSystem::GetSendDataSize									 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/09/25														 */
/* ========================================================================= */

DWORD CInfoSystem::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (STSYSTEM_INITCHARSTATUS);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSystem::GetSendData										 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/09/25														 */
/* ========================================================================= */

PBYTE CInfoSystem::GetSendData(void)
{
	PBYTE pRet, pData, pDataTmp;
	DWORD dwSize;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	pDataTmp	= pData;

	CopyMemoryRenew (pDataTmp, m_pInitCharStatus, sizeof (STSYSTEM_INITCHARSTATUS), pDataTmp);	/* キャラステータス初期値 */

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSystem::SetSendData										 */
/* 内容		:送信データを設定												 */
/* 日付		:2008/09/25														 */
/* 戻り値	:処理した後のアドレス											 */
/* ========================================================================= */

PBYTE CInfoSystem::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (m_pInitCharStatus, pDataTmp, sizeof (STSYSTEM_INITCHARSTATUS), pDataTmp);	/* キャラステータス初期値 */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSystem::Copy												 */
/* 内容		:コピー															 */
/* 日付		:2008/09/25														 */
/* ========================================================================= */

void CInfoSystem::Copy(CInfoSystem *pSrc)
{
	CopyMemory (m_pInitCharStatus, pSrc->m_pInitCharStatus, sizeof (STSYSTEM_INITCHARSTATUS));	/* キャラステータス初期値 */
}

/* Copyright(C)URARA-works 2008 */
