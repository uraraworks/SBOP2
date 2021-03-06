/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* t@C¼	:PacketCHAR_STATUS.cpp										 */
/* àe			:R}h(Ln:Xe[^XîñÊm) Àt@C			 */
/* ì¬			:NªçNt¤çç(URARA-works)							 */
/* ì¬Jnú	:2008/06/29													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketCHAR_STATUS.h"

/* ========================================================================= */
/* Ö¼	:CPacketCHAR_STATUS::CPacketCHAR_STATUS							 */
/* àe		:RXgN^													 */
/* út		:2008/06/29														 */
/* ========================================================================= */

CPacketCHAR_STATUS::CPacketCHAR_STATUS()
{
	m_byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	m_byCmdSub	= SBOCOMMANDID_SUB_CHAR_STATUS;

	m_wAtackGauge			= 0;	/* A^bNQ[W */
	m_wDefenseGauge			= 0;	/* fBtFXQ[W */
	m_wLevel				= 0;	/* x */
	m_wStamina				= 0;	/* X^~i */
	m_wPower				= 0;	/* rÍ */
	m_wStrength				= 0;	/* ÌÍ */
	m_wMagic				= 0;	/* Í */
	m_wSkillful				= 0;	/* íp */
	m_wAbillityAT			= 0;	/* UZ\ */
	m_wAbillityDF			= 0;	/* häZ\ */
	m_wPAtack				= 0;	/* UÍ */
	m_wPDefense				= 0;	/* häÍ */
	m_wPMagic				= 0;	/* @Í */
	m_wPMagicDefense		= 0;	/* @häÍ */
	m_wPHitAverage			= 0;	/* ½¦ */
	m_wPAvoidAverage		= 0;	/* ñð¦ */
	m_wPCriticalAverage		= 0;	/* NeBJ¦ */
	m_wAttrFire				= 0;	/* ®«[Î] */
	m_wAttrWind				= 0;	/* ®«[] */
	m_wAttrWater			= 0;	/* ®«[] */
	m_wAttrEarth			= 0;	/* ®«[y] */
	m_wAttrLight			= 0;	/* ®«[õ] */
	m_wAttrDark				= 0;	/* ®«[Å] */
	m_dwCharID				= 0;	/* LID */
	m_dwMoveWait			= 0;	/* Ú®Ò¿Ô */
	m_dwMoveWaitBattle		= 0;	/* í¬Ú®Ò¿Ô */
	m_dwExp					= 0;	/* o±l */
	m_dwHP					= 0;	/* HP */
	m_dwMaxHP				= 0;	/* ÅåHP */
	m_dwSP					= 0;	/* SP */
	m_dwMaxSP				= 0;	/* ÅåSP */
	m_dwLightTime			= 0;	/* èp±Ô */
	m_nDropItemAverage		= 0;	/* ACehbv¦ */
	m_nMoveAverage			= 0;	/* Ú®m¦ */
	m_nMoveAverageBattle	= 0;	/* í¬Ú®m¦ */
	m_nLightLevel			= 0;	/* èx */
	ZeroMemory (&m_sizeSearchDistance, sizeof (m_sizeSearchDistance));	/* ôGÍÍ */
}


/* ========================================================================= */
/* Ö¼	:CPacketCHAR_STATUS::~CPacketCHAR_STATUS						 */
/* àe		:fXgN^													 */
/* út		:2008/06/29														 */
/* ========================================================================= */

CPacketCHAR_STATUS::~CPacketCHAR_STATUS()
{
}


/* ========================================================================= */
/* Ö¼	:CPacketCHAR_STATUS::Make										 */
/* àe		:pPbgðì¬													 */
/* út		:2008/06/29														 */
/* ========================================================================= */

void CPacketCHAR_STATUS::Make(
	CInfoCharBase *pInfo)		/* [in] Lîñ */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)				+
			 sizeof (pInfo->m_dwCharID)			+
			 sizeof (m_wAtackGauge)				+	/* A^bNQ[W */
			 sizeof (m_wDefenseGauge)			+	/* fBtFXQ[W */
			 sizeof (m_wLevel)					+	/* x */
			 sizeof (m_wStamina)				+	/* X^~i */
			 sizeof (m_wPower)					+	/* rÍ */
			 sizeof (m_wStrength)				+	/* ÌÍ */
			 sizeof (m_wMagic)					+	/* Í */
			 sizeof (m_wSkillful)				+	/* íp */
			 sizeof (m_wAbillityAT)				+	/* UZ\ */
			 sizeof (m_wAbillityDF)				+	/* häZ\ */
			 sizeof (m_wPAtack)					+	/* UÍ */
			 sizeof (m_wPDefense)				+	/* häÍ */
			 sizeof (m_wPMagic)					+	/* @Í */
			 sizeof (m_wPMagicDefense)			+	/* @häÍ */
			 sizeof (m_wPHitAverage)			+	/* ½¦ */
			 sizeof (m_wPAvoidAverage)			+	/* ñð¦ */
			 sizeof (m_wPCriticalAverage)		+	/* NeBJ¦ */
			 sizeof (m_wAttrFire)				+	/* ®«[Î] */
			 sizeof (m_wAttrWind)				+	/* ®«[] */
			 sizeof (m_wAttrWater)				+	/* ®«[] */
			 sizeof (m_wAttrEarth)				+	/* ®«[y] */
			 sizeof (m_wAttrLight)				+	/* ®«[õ] */
			 sizeof (m_wAttrDark)				+	/* ®«[Å] */
			 sizeof (pInfo->m_dwMoveWait)		+
			 sizeof (pInfo->m_dwMoveWaitBattle)	+
			 sizeof (pInfo->m_dwExp)			+	/* o±l */
			 sizeof (pInfo->m_dwHP)				+
			 sizeof (pInfo->m_dwMaxHP)			+
			 sizeof (pInfo->m_dwSP)				+
			 sizeof (pInfo->m_dwMaxSP)			+
			 sizeof (pInfo->m_dwLightTime)		+	/* èp±Ô */
			 sizeof (pInfo->m_nDropItemAverage)	+
			 sizeof (pInfo->m_nMoveAverage)		+
			 sizeof (pInfo->m_nMoveAverageBattle) +
			 sizeof (pInfo->m_nLightLevel)		+	/* èx */
			 sizeof (m_sizeSearchDistance);			/* ôGÍÍ */

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= m_byCmdMain;
	pPacketBase->byCmdSub	= m_byCmdSub;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwCharID,				sizeof (m_dwCharID),			pDataTmp);	/* LID */
//	CopyMemoryRenew (pDataTmp, &pInfo->m_wAtackGauge,			sizeof (m_wAtackGauge),			pDataTmp);	/* A^bNQ[W */
//	CopyMemoryRenew (pDataTmp, &pInfo->m_wDefenseGauge,			sizeof (m_wDefenseGauge),		pDataTmp);	/* fBtFXQ[W */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wLevel,				sizeof (m_wLevel),				pDataTmp);	/* x */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wStamina,				sizeof (m_wStamina),			pDataTmp);	/* X^~i */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPower,				sizeof (m_wPower),				pDataTmp);	/* rÍ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wStrength,				sizeof (m_wStrength),			pDataTmp);	/* ÌÍ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wMagic,				sizeof (m_wMagic),				pDataTmp);	/* Í */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wSkillful,				sizeof (m_wSkillful),			pDataTmp);	/* íp */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAbillityAT,			sizeof (m_wAbillityAT),			pDataTmp);	/* UZ\ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAbillityDF,			sizeof (m_wAbillityDF),			pDataTmp);	/* häZ\ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPAtack,				sizeof (m_wPAtack),				pDataTmp);	/* UÍ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPDefense,				sizeof (m_wPDefense),			pDataTmp);	/* häÍ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPMagic,				sizeof (m_wPMagic),				pDataTmp);	/* @Í */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPMagicDefense,		sizeof (m_wPMagicDefense),		pDataTmp);	/* @häÍ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPHitAverage,			sizeof (m_wPHitAverage),		pDataTmp);	/* ½¦ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPAvoidAverage,		sizeof (m_wPAvoidAverage),		pDataTmp);	/* ñð¦ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wPCriticalAverage,		sizeof (m_wPCriticalAverage),	pDataTmp);	/* NeBJ¦ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrFire,				sizeof (m_wAttrFire),			pDataTmp);	/* ®«[Î] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrWind,				sizeof (m_wAttrWind),			pDataTmp);	/* ®«[] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrWater,			sizeof (m_wAttrWater),			pDataTmp);	/* ®«[] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrEarth,			sizeof (m_wAttrEarth),			pDataTmp);	/* ®«[y] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrLight,			sizeof (m_wAttrLight),			pDataTmp);	/* ®«[õ] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wAttrDark,				sizeof (m_wAttrDark),			pDataTmp);	/* ®«[Å] */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwMoveWait,			sizeof (m_dwMoveWait),			pDataTmp);	/* Ú®Ò¿Ô */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwMoveWaitBattle,		sizeof (m_dwMoveWaitBattle),	pDataTmp);	/* í¬Ú®Ò¿Ô */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwExp,					sizeof (m_dwExp),				pDataTmp);	/* o±l */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwHP,					sizeof (m_dwHP),				pDataTmp);	/* HP */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwMaxHP,				sizeof (m_dwMaxHP),				pDataTmp);	/* ÅåHP */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwSP,					sizeof (m_dwSP),				pDataTmp);	/* SP */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwMaxSP,				sizeof (m_dwMaxSP),				pDataTmp);	/* ÅåSP */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwLightTime,			sizeof (m_dwLightTime),			pDataTmp);	/* èp±Ô */
	CopyMemoryRenew (pDataTmp, &pInfo->m_nDropItemAverage,		sizeof (m_nDropItemAverage),	pDataTmp);	/* ACehbv¦ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_nMoveAverage,			sizeof (m_nMoveAverage),		pDataTmp);	/* Ú®m¦ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_nMoveAverageBattle,	sizeof (m_nMoveAverageBattle),	pDataTmp);	/* í¬Ú®m¦ */
	CopyMemoryRenew (pDataTmp, &pInfo->m_nLightLevel,			sizeof (m_nLightLevel),			pDataTmp);	/* èx */
	CopyMemoryRenew (pDataTmp, &pInfo->m_sizeSearchDistance,	sizeof (m_sizeSearchDistance),	pDataTmp);	/* ôGÍÍ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* Ö¼	:CPacketCHAR_STATUS::Set										 */
/* àe		:pPbgðÝè													 */
/* út		:2008/06/29														 */
/* ========================================================================= */

PBYTE CPacketCHAR_STATUS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,				pDataTmp, sizeof (m_dwCharID),				pDataTmp);	/* LID */
//	CopyMemoryRenew (&m_wAtackGauge,			pDataTmp, sizeof (m_wAtackGauge),			pDataTmp);	/* A^bNQ[W */
//	CopyMemoryRenew (&m_wDefenseGauge,			pDataTmp, sizeof (m_wDefenseGauge),			pDataTmp);	/* fBtFXQ[W */
	CopyMemoryRenew (&m_wLevel,					pDataTmp, sizeof (m_wLevel),				pDataTmp);	/* x */
	CopyMemoryRenew (&m_wStamina,				pDataTmp, sizeof (m_wStamina),				pDataTmp);	/* X^~i */
	CopyMemoryRenew (&m_wPower,					pDataTmp, sizeof (m_wPower),				pDataTmp);	/* rÍ */
	CopyMemoryRenew (&m_wStrength,				pDataTmp, sizeof (m_wStrength),				pDataTmp);	/* ÌÍ */
	CopyMemoryRenew (&m_wMagic,					pDataTmp, sizeof (m_wMagic),				pDataTmp);	/* Í */
	CopyMemoryRenew (&m_wSkillful,				pDataTmp, sizeof (m_wSkillful),				pDataTmp);	/* íp */
	CopyMemoryRenew (&m_wAbillityAT,			pDataTmp, sizeof (m_wAbillityAT),			pDataTmp);	/* UZ\ */
	CopyMemoryRenew (&m_wAbillityDF,			pDataTmp, sizeof (m_wAbillityDF),			pDataTmp);	/* häZ\ */
	CopyMemoryRenew (&m_wPAtack,				pDataTmp, sizeof (m_wPAtack),				pDataTmp);	/* UÍ */
	CopyMemoryRenew (&m_wPDefense,				pDataTmp, sizeof (m_wPDefense),				pDataTmp);	/* häÍ */
	CopyMemoryRenew (&m_wPMagic,				pDataTmp, sizeof (m_wPMagic),				pDataTmp);	/* @Í */
	CopyMemoryRenew (&m_wPMagicDefense,			pDataTmp, sizeof (m_wPMagicDefense),		pDataTmp);	/* @häÍ */
	CopyMemoryRenew (&m_wPHitAverage,			pDataTmp, sizeof (m_wPHitAverage),			pDataTmp);	/* ½¦ */
	CopyMemoryRenew (&m_wPAvoidAverage,			pDataTmp, sizeof (m_wPAvoidAverage),		pDataTmp);	/* ñð¦ */
	CopyMemoryRenew (&m_wPCriticalAverage,		pDataTmp, sizeof (m_wPCriticalAverage),		pDataTmp);	/* NeBJ¦ */
	CopyMemoryRenew (&m_wAttrFire,				pDataTmp, sizeof (m_wAttrFire),				pDataTmp);	/* ®«[Î] */
	CopyMemoryRenew (&m_wAttrWind,				pDataTmp, sizeof (m_wAttrWind),				pDataTmp);	/* ®«[] */
	CopyMemoryRenew (&m_wAttrWater,				pDataTmp, sizeof (m_wAttrWater),			pDataTmp);	/* ®«[] */
	CopyMemoryRenew (&m_wAttrEarth,				pDataTmp, sizeof (m_wAttrEarth),			pDataTmp);	/* ®«[y] */
	CopyMemoryRenew (&m_wAttrLight,				pDataTmp, sizeof (m_wAttrLight),			pDataTmp);	/* ®«[õ] */
	CopyMemoryRenew (&m_wAttrDark,				pDataTmp, sizeof (m_wAttrDark),				pDataTmp);	/* ®«[Å] */
	CopyMemoryRenew (&m_dwMoveWait,				pDataTmp, sizeof (m_dwMoveWait),			pDataTmp);	/* Ú®Ò¿Ô */
	CopyMemoryRenew (&m_dwMoveWaitBattle,		pDataTmp, sizeof (m_dwMoveWaitBattle),		pDataTmp);	/* í¬Ú®Ò¿Ô */
	CopyMemoryRenew (&m_dwExp,					pDataTmp, sizeof (m_dwExp),					pDataTmp);	/* o±l */
	CopyMemoryRenew (&m_dwHP,					pDataTmp, sizeof (m_dwHP),					pDataTmp);	/* HP */
	CopyMemoryRenew (&m_dwMaxHP,				pDataTmp, sizeof (m_dwMaxHP),				pDataTmp);	/* ÅåHP */
	CopyMemoryRenew (&m_dwSP,					pDataTmp, sizeof (m_dwSP),					pDataTmp);	/* SP */
	CopyMemoryRenew (&m_dwMaxSP,				pDataTmp, sizeof (m_dwMaxSP),				pDataTmp);	/* ÅåSP */
	CopyMemoryRenew (&m_dwLightTime,			pDataTmp, sizeof (m_dwLightTime),			pDataTmp);	/* èp±Ô */
	CopyMemoryRenew (&m_nDropItemAverage,		pDataTmp, sizeof (m_nDropItemAverage),		pDataTmp);	/* ACehbv¦ */
	CopyMemoryRenew (&m_nMoveAverage,			pDataTmp, sizeof (m_nMoveAverage),			pDataTmp);	/* Ú®m¦ */
	CopyMemoryRenew (&m_nMoveAverageBattle,		pDataTmp, sizeof (m_nMoveAverageBattle),	pDataTmp);	/* í¬Ú®m¦ */
	CopyMemoryRenew (&m_nLightLevel,			pDataTmp, sizeof (m_nLightLevel),			pDataTmp);	/* èx */
	CopyMemoryRenew (&m_sizeSearchDistance,		pDataTmp, sizeof (m_sizeSearchDistance),	pDataTmp);	/* ôGÍÍ */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* Ö¼	:CPacketCHAR_STATUS::SetParam									 */
/* àe		:wèLîñÖp[^ð½f								 */
/* út		:2008/06/29														 */
/* ========================================================================= */

void CPacketCHAR_STATUS::SetParam(CInfoCharBase *pInfo)
{
	if (pInfo == NULL) {
		return;
	}

//	pInfo->m_wAtackGauge		= m_wAtackGauge;		/* A^bNQ[W */
//	pInfo->m_wDefenseGauge		= m_wDefenseGauge;		/* fBtFXQ[W */
	pInfo->m_wLevel				= m_wLevel;				/* x */
	pInfo->m_wStamina			= m_wStamina;			/* X^~i */
	pInfo->m_wPower				= m_wPower;				/* rÍ */
	pInfo->m_wStrength			= m_wStrength;			/* ÌÍ */
	pInfo->m_wMagic				= m_wMagic;				/* Í */
	pInfo->m_wSkillful			= m_wSkillful;			/* íp */
	pInfo->m_wAbillityAT		= m_wAbillityAT;		/* UZ\ */
	pInfo->m_wAbillityDF		= m_wAbillityDF;		/* häZ\ */
	pInfo->m_wPAtack			= m_wPAtack;			/* UÍ */
	pInfo->m_wPDefense			= m_wPDefense;			/* häÍ */
	pInfo->m_wPMagic			= m_wPMagic;			/* @Í */
	pInfo->m_wPMagicDefense		= m_wPMagicDefense;		/* @häÍ */
	pInfo->m_wPHitAverage		= m_wPHitAverage;		/* ½¦ */
	pInfo->m_wPAvoidAverage		= m_wPAvoidAverage;		/* ñð¦ */
	pInfo->m_wPCriticalAverage	= m_wPCriticalAverage;	/* NeBJ¦ */
	pInfo->m_wAttrFire			= m_wAttrFire;			/* ®«[Î] */
	pInfo->m_wAttrWind			= m_wAttrWind;			/* ®«[] */
	pInfo->m_wAttrWater			= m_wAttrWater;			/* ®«[] */
	pInfo->m_wAttrEarth			= m_wAttrEarth;			/* ®«[y] */
	pInfo->m_wAttrLight			= m_wAttrLight;			/* ®«[õ] */
	pInfo->m_wAttrDark			= m_wAttrDark;			/* ®«[Å] */
	pInfo->m_dwMoveWait			= m_dwMoveWait;			/* Ú®Ò¿Ô */
	pInfo->m_dwMoveWaitBattle	= m_dwMoveWaitBattle;	/* í¬Ú®Ò¿Ô */
	pInfo->m_dwExp				= m_dwExp;				/* o±l */
	pInfo->m_dwHP				= m_dwHP;				/* HP */
	pInfo->m_dwMaxHP			= m_dwMaxHP;			/* ÅåHP */
	pInfo->m_dwSP				= m_dwSP;				/* SP */
	pInfo->m_dwMaxSP			= m_dwMaxSP;			/* ÅåSP */
	pInfo->m_nDropItemAverage	= m_nDropItemAverage;	/* ACehbv¦ */
	pInfo->m_nMoveAverage		= m_nMoveAverage;		/* Ú®m¦ */
	pInfo->m_nMoveAverageBattle	= m_nMoveAverageBattle;	/* í¬Ú®m¦ */
	pInfo->m_nLightLevel		= m_nLightLevel;		/* èx */
	pInfo->m_sizeSearchDistance	= m_sizeSearchDistance;	/* ôGÍÍ */
}

/* Copyright(C)URARA-works 2008 */
