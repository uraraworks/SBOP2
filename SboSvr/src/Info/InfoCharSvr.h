/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ƒtƒ@ƒCƒ‹–¼	:InfoCharSvr.h												 */
/* “à—e			:ƒLƒƒƒ‰î•ñƒT[ƒo[ƒNƒ‰ƒX ’è‹`ƒtƒ@ƒCƒ‹						 */
/* ì¬			:”N‚ª‚ç”N’†t‚¤‚ç‚ç(URARA-works)							 */
/* ì¬ŠJŽn“ú	:2007/01/14													 */
/* ========================================================================= */

#pragma once

#include "InfoCharBase.h"

class CInfoMapBase;
class CLibInfoCharSvr;

/* ========================================================================= */
/* ’è”’è‹`																	 */
/* ========================================================================= */

/* s“®ID */
enum {
	CHARPROCID_NONE = 0,
	CHARPROCID_FISHING,								/* ’Þ‚è */
	CHARPROCID_FISHING_HIT,							/* ’Þ‚è(ƒqƒbƒg) */
	CHARPROCID_MAPMOVEIN,							/* ƒ}ƒbƒv“àˆÚ“® */
	CHARPROCID_MAPMOVEOUT,							/* ƒ}ƒbƒvŠOˆÚ“® */
	CHARPROCID_SWOON,								/* ‹Câ */
	CHARPROCID_INVINCIBLE,							/* –³“G */
	CHARPROCID_MAX
};

/* s“®î•ñ */
typedef struct _CHARPROCINFO {
	DWORD	dwProcID,								/* s“®ID */
			dwProcSetTime,							/* ˆ—Ý’èŽžŠÔ */
			dwProcStartTime,						/* ˆ—ŠJŽnŽžŠÔ */
			dwPara;									/* ƒpƒ‰ƒ[ƒ^ */
} CHARPROCINFO, *PCHARPROCINFO;


/* ========================================================================= */
/* ƒNƒ‰ƒXéŒ¾																 */
/* ========================================================================= */

typedef class CInfoCharSvr : public CInfoCharBase
{
public:
			CInfoCharSvr();								/* ƒRƒ“ƒXƒgƒ‰ƒNƒ^ */
	virtual ~CInfoCharSvr();							/* ƒfƒXƒgƒ‰ƒNƒ^ */

			void SetSpeak		(LPCSTR pszSpeak);				/* ”­Œ¾“à—e‚ðXV */
	virtual void SetMoveState	(int nMoveState);				/* ˆÚ“®ó‘Ô‚ð•ÏX */
	virtual void SetProcState	(int nProcState);				/* s“®ó‘Ô‚ð•ÏX */
			void SetMap			(CInfoMapBase *pInfoMap);		/* ƒ}ƒbƒvî•ñ‚ðÝ’è */
			void SetLibInfoChar	(CLibInfoCharSvr *pLibInfoChar);/* ƒLƒƒƒ‰î•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðÝ’è */
			void SetEfcBalloon	(DWORD dwEfcBalloonID);			/* •¬o‚µ‚ðÝ’è */
			void SetMotion		(DWORD dwMotionID);				/* ƒ‚[ƒVƒ‡ƒ“‚ðÝ’è */

	virtual BOOL  IsEnableBattle	(void);						/* í“¬ó‘Ô‚É‘JˆÚ‚Å‚«‚é‚©”»’è */
	virtual BOOL  IsEnableMove		(void);						/* ˆÚ“®‚Å‚«‚éó‘Ô‚©”»’è */
	virtual void  CopyAll			(CInfoCharSvr *pSrc);		/* ”h¶æ‚Ìî•ñ‚à‘S‚ÄƒRƒs[ */
	virtual void  Copy				(CInfoCharBase *pSrc);		/* ƒRƒs[ */
	virtual void  ProcAtack			(void);						/* ˆ—(UŒ‚‚µ‚½Žž) */
	virtual BOOL  ProcHit			(CInfoCharSvr *pInfoChar);	/* ˆ—(UŒ‚‚ðŽó‚¯‚½Žž) */
	virtual BOOL  TimerProc			(DWORD dwTime);				/* ŽžŠÔˆ— */
	virtual BOOL  TimerProcMOVE		(DWORD dwTime);				/* ŽžŠÔˆ—(ˆÚ“®) */
	virtual BOOL  IsAtackTarget		(void);						/* UŒ‚‘ÎÛ‚Æ‚È‚é‚©”»’è */
	virtual DWORD GetHitEffectID	(void);						/* ƒqƒbƒgŽž‚É‘ŠŽè‚É•\Ž¦‚·‚éƒGƒtƒFƒNƒgID‚ðŽæ“¾ */
	virtual DWORD GetDamage			(void);						/* ƒ_ƒ[ƒW’l‚ðŽæ“¾ */

			void Proc				(DWORD dwTime);				/* s“®ˆ— */
	virtual BOOL ProcFISHING		(DWORD dwPara);				/* s“®ˆ—(’Þ‚è) */
	virtual BOOL ProcFISHING_HIT	(DWORD dwPara);				/* s“®ˆ—(’Þ‚è(ƒqƒbƒg)) */
	virtual BOOL ProcMAPMOVEIN		(DWORD dwPara);				/* s“®ˆ—(ƒ}ƒbƒv“àˆÚ“®) */
	virtual BOOL ProcMAPMOVEOUT		(DWORD dwPara);				/* s“®ˆ—(ƒ}ƒbƒvŠOˆÚ“®) */
	virtual BOOL ProcSWOON			(DWORD dwPara);				/* s“®ˆ—(‹Câ) */
	virtual BOOL ProcINVINCIBLE		(DWORD dwPara);				/* s“®ˆ—(–³“G) */

	void DeleteProcInfo		(int nNo);							/* s“®î•ñ‚ðíœ */
	void DeleteProcInfo		(DWORD dwProcID);					/* s“®î•ñ‚ðíœ */
	void DeleteAllProcInfo	(void);								/* s“®î•ñ‚ð‘S‚Äíœ */
	void AddProcInfo		(DWORD dwProcID, DWORD dwStartTime, DWORD dwPara);	/* s“®î•ñ‚ð’Ç‰Á */

	/* ˆÚ“®Ží•Ê‚É‚æ‚éˆ— */
	virtual void IncPutCount	(void);							/* ”­¶NPC”‚ð‘‰Á */
	virtual void DecPutCount	(void);							/* ”­¶NPC”‚ðŒ¸­ */


public:
	int		m_nReserveChgEfect,			/* ƒGƒtƒFƒNƒg•ÏX—\–ñ */
			m_nReserveChgMoveState,		/* ˆÚ“®Ží•Ê•ÏX—\–ñ */
			m_nMoveCount;				/* ˆÚ“®•à” */
	BOOL	m_bChgPos,					/* À•W‚ª•ÏX‚³‚ê‚½ */
			m_bChgMap,					/* ƒ}ƒbƒv‚ª•ÏX‚³‚ê‚½ */
			m_bChgUpdatePos,			/* ƒNƒ‰ƒCƒAƒ“ƒg‘¤‚ÅŽ©ƒLƒƒƒ‰‚ÌÀ•W‚ðXV‚·‚é */
			m_bChgSpeak,				/* ”­Œ¾“à—e‚ª•ÏX‚³‚ê‚½ */
			m_bChgInfo,					/* ƒLƒƒƒ‰î•ñ‚ª•ÏX‚³‚ê‚½ */
			m_bChgMoveState,			/* ˆÚ“®ó‘Ô‚ª•ÏX‚³‚ê‚½ */
			m_bChgProcState,			/* s“®ó‘Ô‚ª•ÏX‚³‚ê‚½ */
			m_bChgScreenPos,			/* ‰æ–ÊˆÊ’u‚ð•ÏX‚·‚é */
			m_bChgPosRenew,				/* À•W‚ª•ÏX‚³‚ê‚½ */
			m_bChgGrp,					/* ‰æ‘œ‚ª•ÏX‚³‚ê‚½ */
			m_bChgEfcBalloon,			/* •¬o‚µ‚ª•ÏX‚³‚ê‚½ */
			m_bChgMotion,				/* ƒ‚[ƒVƒ‡ƒ“‚ª•ÏX‚³‚ê‚½ */
			m_bChgStatus,				/* ƒXƒe[ƒ^ƒX‚ª•ÏX‚³‚ê‚½ */
			m_bChgFishingHit,			/* ’Þ‚ê‚½ */
			m_bChgMoveCount,			/* Žw’è•à”‚ÌˆÚ“® */
			m_bChgPutNpc,				/* NPC”­¶ */
			m_bChgTargetChar,			/* ‹ß‚­‚ÌƒLƒƒƒ‰‚Éƒ^[ƒQƒbƒg•ÏX */
			m_bWaitCheckMapEvent,		/* ƒ}ƒbƒvƒCƒxƒ“ƒgƒ`ƒFƒbƒN‘Ò‚¿ */
			m_bDropItem,				/* ƒAƒCƒeƒ€ƒhƒƒbƒv */
			m_bAtack,					/* UŒ‚ */
			m_bRenewTargetPos,			/* ƒ^[ƒQƒbƒgÀ•WXV */
			m_bProcMoveMapIn,			/* ƒ}ƒbƒv“àˆÚ“® */
			m_bProcMoveMapOut,			/* ƒ}ƒbƒvŠOˆÚ“® */
			m_bProcMoveMarkPos,			/* ‹L˜^ˆÊ’u‚ÖˆÚ“® */
			m_bProcSwoon,				/* ‹Câ */
			m_bProcInvincible,			/* –³“G */
			m_bStateFadeInOut,			/* ƒtƒF[ƒhƒCƒ“EƒAƒEƒg’† */
			m_bStatusInvincible;		/* –³“G’† */
	DWORD	m_dwLastTimeChg,			/* ó‘Ô•ÏXŽžŠÔ */
			m_dwChgWait,				/* ó‘Ô•ÏX‘Ò‚¿ŽžŠÔ */
			m_dwEfcBalloonID,			/* •¬o‚µID */
			m_dwMotionID,				/* ƒ‚[ƒVƒ‡ƒ“ID */
			m_dwMoveCount;				/* ˆÚ“®•à” */

	CInfoMapBase	*m_pInfoMap;		/* ƒ}ƒbƒvî•ñ */
	CLibInfoCharSvr	*m_pLibInfoCharSvr;	/* ƒLƒƒƒ‰î•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CmyArray<PCHARPROCINFO, PCHARPROCINFO>	m_apProcInfo;	/* s“®î•ñ */
} CInfoCharSvr, *PCInfoCharSvr;

/* Copyright(C)URARA-works 2007 */
