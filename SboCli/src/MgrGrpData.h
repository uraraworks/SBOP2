/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ƒtƒ@ƒCƒ‹–¼	:MgrGrpData.h												 */
/* “à—e			:ƒOƒ‰ƒtƒBƒbƒNƒf[ƒ^ƒ}ƒl[ƒWƒƒƒNƒ‰ƒX ’è‹`ƒtƒ@ƒCƒ‹			 */
/* ì¬			:”N‚ª‚ç”N’†t‚¤‚ç‚ç(URARA-works)							 */
/* ì¬ŠJŽn“ú	:2006/09/24													 */
/* ========================================================================= */

#pragma once

class CImg32;

/* ========================================================================= */
/* ’è”éŒ¾																	 */
/* ========================================================================= */

typedef CmyArray<CImg32 *, CImg32 *>	  ARRAYCIMG;
typedef CmyArray<CImg32 *, CImg32 *>	*PARRAYCIMG;


/* ========================================================================= */
/* ƒNƒ‰ƒXéŒ¾																 */
/* ========================================================================= */

typedef class CMgrGrpData
{
public:
			CMgrGrpData();									/* ƒRƒ“ƒXƒgƒ‰ƒNƒ^ */
	virtual ~CMgrGrpData();									/* ƒfƒXƒgƒ‰ƒNƒ^ */

	BOOL	Load				(void);								/* “Ç‚Ýž‚Ý */
	BOOL	CheckVersion		(LPCSTR pszVersion);				/* ƒo[ƒWƒ‡ƒ“ƒ`ƒFƒbƒN */
	void	Write				(LPCSTR pszFileName, CImg32 *pSrc);	/* PNGƒf[ƒ^‚Ì•Û‘¶ */
	CImg32	*GetDibSystem		(void);								/* ƒVƒXƒeƒ€‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibIcon			(void);								/* ƒAƒCƒRƒ“‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibShadow		(void);								/* ‰e‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibMapParts		(int nNo);							/* ƒ}ƒbƒv‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibMapShadow	(int nNo);							/* ƒ}ƒbƒv‰e‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibItem			(int nNo);							/* ƒAƒCƒeƒ€‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibNumS			(void);								/* ”Žš(S)‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibNumM			(void);								/* ”Žš(M)‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibNumL			(void);								/* ”Žš(L)‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibNPC			(int nNo);							/* NPC‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibBody			(WORD wFamilyID);					/* ‘Ì‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibEar			(WORD wFamilyID);					/* Ž¨‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibCloth		(int nNo);							/* •ž‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibEye			(int nNo);							/* –Ú‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibHairD		(WORD wHairColorID);				/* ”¯(‰º)‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibHairU		(WORD wHairColorID);				/* ”¯(ã)‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibSP			(int nNo);							/* “ÁŽê•ž‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibAcce			(int nNo);							/* ƒAƒNƒZƒTƒŠ‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibEfc32		(int nNo);							/* ƒGƒtƒFƒNƒg(32)‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibEfc64		(int nNo);							/* ƒGƒtƒFƒNƒg(64)‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibWeapon		(int nNo);							/* •Ší‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibWeaponBow	(int nNo);							/* •Ší(‹|)‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibWeaponArrow	(int nNo);							/* •Ší(–î)‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibWeaponGlove	(int nNo);							/* •Ší(‘ÅŒ‚)‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibWeaponEtc	(int nNo);							/* •Ší(‚»‚Ì‘¼)‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibEfcBalloon	(int nNo);							/* •¬o‚µ‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDib2x2Body		(WORD wFamilyID);					/* 2x2‘Ì‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDib2x2Cloth		(WORD wClothID);					/* 2x2•ž‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDib2x2Eye		(int nNo);							/* 2x2–Ú‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDib2x2Hair		(WORD wHairColorID);				/* 2x2”¯‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDib2x2SPCloth	(WORD wSPClothID);					/* 2x2“ÁŽê•ž‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDib2x2SPHair	(WORD wSPHairID);					/* 2x2“ÁŽê”¯‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDib2x2Arms		(WORD wArmsID);						/* 2x2Ž‚¿•¨‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDib2x2Shield	(WORD wShieldID);					/* 2x2‚‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDib2x2ArmsSP	(WORD wArmsID);						/* 2x2“ÁŽêŽ‚¿•¨‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDib2x2Bow		(WORD wBowID);						/* 2x2‹|‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDib2x2NPC		(WORD wNPCID);						/* 2x2NPC‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDib2x2CharShadow(int nNo);							/* 2x2ƒLƒƒƒ‰‰e‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDib2x2NPCShadow	(int nNo);							/* 2x2NPC‰e‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */

	CImg32	*GetDib				(DWORD dwGrpIDMain, DWORD dwGrpIDSub, DWORD dwParam);	/* ‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */

	CImg32	*GetDibTmpLogo			(void);							/* URARA-worksƒƒS‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibTmpTitle			(void);							/* ƒ^ƒCƒgƒ‹‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibTmpTitleBack		(void);							/* ƒ^ƒCƒgƒ‹”wŒi‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibTmpTitleCloud	(void);							/* ƒ^ƒCƒgƒ‹‰_‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	CImg32	*GetDibTmpLoginMenuBack	(void);							/* ƒƒOƒCƒ“ƒƒjƒ…[‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */

	int		GetMapPartsCount	(void);								/* ƒ}ƒbƒvƒp[ƒc”‚ðŽæ“¾ */
	CImg32	*GetDibMapPartsTmp	(int nNo);							/* “™”{ƒ}ƒbƒv‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	void	ReadMapPartsTmp		(void);								/* “™”{ƒ}ƒbƒvƒp[ƒc‰æ‘œ‚ð“Ç‚Ýž‚Ý */
	void	DeleteMapPartsTmp	(void);								/* “™”{ƒ}ƒbƒvƒp[ƒc‰æ‘œ‚ðíœ */
	int		GetMapShadowCount	(void);								/* ƒ}ƒbƒv‰e‰æ‘œ”‚ðŽæ“¾ */
	CImg32	*GetDibMapShadowTmp	(int nNo);							/* “™”{ƒ}ƒbƒv‰e‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	void	ReadMapShadowTmp	(void);								/* “™”{ƒ}ƒbƒv‰e‰æ‘œ‚ð“Ç‚Ýž‚Ý */
	void	DeleteMapShadowTmp	(void);								/* “™”{ƒ}ƒbƒv‰e‰æ‘œ‚ðíœ */
	int		GetItemCount		(void);								/* ƒAƒCƒeƒ€‰æ‘œ”‚ðŽæ“¾ */
	CImg32	*GetDibItemTmp		(int nNo);							/* “™”{ƒAƒCƒeƒ€‰æ‘œƒCƒ[ƒW‚ðŽæ“¾ */
	void	ReadItemTmp			(void);								/* “™”{ƒAƒCƒeƒ€‰æ‘œ‚ð“Ç‚Ýž‚Ý */
	void	DeleteItemTmp		(void);								/* “™”{ƒAƒCƒeƒ€‰æ‘œ‚ðíœ */

	int		GetGrpSize			(DWORD dwGrpIDMain);				/* ‰æ‘œ‚P‚Â‚ ‚½‚è‚ÌƒTƒCƒY‚ðŽæ“¾ */
	int		GetGrpCountX		(DWORD dwGrpIDMain);				/* ‰æ‘œ‚Pƒtƒ@ƒCƒ‹‚Ì‰¡ŒÂ”‚ðŽæ“¾ */
	int		GetGrpCountY		(DWORD dwGrpIDMain);				/* ‰æ‘œ‚Pƒtƒ@ƒCƒ‹‚ÌcŒÂ”‚ðŽæ“¾ */
	void	GetGrpPos			(DWORD dwGrpIDMain, DWORD dwGrpIDSub, POINT &ptDst, WORD wGrpID = 0);	/* Žw’è‰æ‘œ‚ÌˆÊ’u‚ðŽæ“¾ */


private:
	BOOL	Read	(LPSTR pszName, CImg32 **pDib, int nSize);	/* PNGƒf[ƒ^‚Ì“Ç‚Ýž‚Ý */
	BOOL	Read256	(LPSTR pszName, CImg32 **pDib, int nSize);	/* PNGƒf[ƒ^‚Ì“Ç‚Ýž‚Ý */


private:
	HMODULE		m_hDll;					/* DLL‚Ìƒ‚ƒWƒ…[ƒ‹ƒnƒ“ƒhƒ‹ */

	int			m_nMapPartsCount,		/* ƒ}ƒbƒvƒp[ƒc‰æ‘œ” */
				m_nMapShadowCount;		/* ƒ}ƒbƒv‰e‰æ‘œ” */
	CImg32		*m_pImgSystem,			/* ƒVƒXƒeƒ€ */
				*m_pImgIcon,			/* ƒAƒCƒRƒ“ */
				*m_pImgShadow,			/* ‰e‰æ‘œ */
				*m_pImgItem,			/* ƒAƒCƒeƒ€‰æ‘œ */
				*m_pImgNumS,			/* ”Žš(S) */
				*m_pImgNumM,			/* ”Žš(M) */
				*m_pImgNumL;			/* ”Žš(L) */
	PARRAYCIMG	m_paImgMapParts,		/* ƒ}ƒbƒvƒp[ƒc */
				m_paImgMapShadow,		/* ƒ}ƒbƒv‰e */
				m_paImgNPC,				/* NPC */
				m_paImgBody,			/* ‘Ì */
				m_paImgEar,				/* Ž¨ */
				m_paImgCloth,			/* •ž */
				m_paImgEye,				/* –Ú */
				m_paImgHairD,			/* ”¯(‰º) */
				m_paImgHairU,			/* ”¯(ã) */
				m_paImgSP,				/* “ÁŽê•ž */
				m_paImgAcce,			/* ƒAƒNƒZƒTƒŠ */
				m_paImgMapPartsTmp,		/* “™”{ƒ}ƒbƒvƒp[ƒc */
				m_paImgMapShadowTmp,	/* “™”{ƒ}ƒbƒv‰e */
				m_paImgItemTmp,			/* “™”{ƒAƒCƒeƒ€ */
				m_paImgEfc32,			/* ƒGƒtƒFƒNƒg(32) */
				m_paImgEfc64,			/* ƒGƒtƒFƒNƒg(64) */
				m_paImgWeapon,			/* •Ší */
				m_paImgWeaponBow,		/* •Ší(‹|) */
				m_paImgWeaponArrow,		/* •Ší(–î) */
				m_paImgWeaponGlove,		/* •Ší(‘ÅŒ‚) */
				m_paImgWeaponEtc,		/* •Ší(‚»‚Ì‘¼) */
				m_paImgEfcBalloon,		/* •¬o‚µ */
				m_paImg2x2Body,			/* 2x2‘Ì */
				m_paImg2x2Cloth,		/* 2x2•ž */
				m_paImg2x2Eye,			/* 2x2–Ú */
				m_paImg2x2Hair,			/* 2x2”¯ */
				m_paImg2x2SPCloth,		/* 2x2“ÁŽê•ž */
				m_paImg2x2SPHair,		/* 2x2“ÁŽê”¯ */
				m_paImg2x2Arms,			/* 2x2Ž‚¿•¨ */
				m_paImg2x2Shield,		/* 2x2‚ */
				m_paImg2x2ArmsSP,		/* 2x2“ÁŽêŽ‚¿•¨ */
				m_paImg2x2Bow,			/* 2x2‹| */
				m_paImg2x2NPC,			/* 2x2NPC */
				m_paImg2x2CharShadow,	/* 2x2ƒLƒƒƒ‰‰e */
				m_paImg2x2NPCShadow;	/* 2x2NPC‰e */
} CMgrGrpData, *PCMgrGrpData;

/* Copyright(C)URARA-works 2006 */
