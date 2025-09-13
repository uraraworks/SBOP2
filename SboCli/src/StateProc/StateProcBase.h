/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ƒtƒ@ƒCƒ‹–¼	:StateProcBase.h											 */
/* “à—e			:ó‘Ôˆ—Šî’êƒNƒ‰ƒX ’è‹`ƒtƒ@ƒCƒ‹							 */
/* ì¬			:”N‚ª‚ç”N’†t‚¤‚ç‚ç(URARA-works)							 */
/* ì¬ŠJŽn“ú	:2006/10/01													 */
/* ========================================================================= */

#pragma once

class CUraraSockTCPSBO;
class CMgrData;
class CMgrSound;
class CMgrDraw;
class CMgrLayer;
class CMgrWindow;
class CMgrKeyInput;

/* ========================================================================= */
/* ƒNƒ‰ƒXéŒ¾																 */
/* ========================================================================= */

typedef class CStateProcBase
{
public:
			CStateProcBase();							/* ƒRƒ“ƒXƒgƒ‰ƒNƒ^ */
	virtual ~CStateProcBase();							/* ƒfƒXƒgƒ‰ƒNƒ^ */

	virtual void Create			(CMgrData *pMgrData, CUraraSockTCPSBO *pSock);	/* ì¬ */

	virtual void Init				(void);								/* ‰Šú‰» */
	virtual BOOL TimerProc			(void);								/* ŽžŠÔˆ— */
	virtual void KeyProc			(BYTE byCode, BOOL bDown);			/* ƒL[ˆ— */
	virtual void OnLButtonDown		(int x, int y);						/* ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_LBUTTONDOWN) */
	virtual void OnRButtonDown		(int x, int y);						/* ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_RBUTTONDOWN) */
	virtual void OnRButtonDblClk	(int x, int y);						/* ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_RBUTTONDBLCLK) */
	virtual void OnMouseMove		(int x, int y);						/* ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_MOUSEMOVE) */
	virtual void OnKeyDown			(UINT vk);							/* ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_KEYDOWN) */
	virtual void OnWindowMsg		(int nType, DWORD dwPara);			/* ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_WINDOWMSG) */
	virtual void OnMainFrame		(DWORD dwCommand, DWORD dwParam);	/* ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_MAINFRAME) */
	virtual void OnAdminMsg			(int nType, DWORD dwPara);			/* ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_ADMINMSG) */
			void OnMgrDraw			(int nCode, DWORD dwPara);			/* ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_MGRDRAW) */


protected:
	virtual BOOL OnUp		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(ª) */
	virtual BOOL OnDown		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(«) */
	virtual BOOL OnLeft		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(©) */
	virtual BOOL OnRight	(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(¨) */
	virtual BOOL OnX		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(X) */
	virtual BOOL OnZ		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(Z) */
	virtual BOOL OnS		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(S) */
	virtual BOOL OnA		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(A) */
	virtual BOOL OnB		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(B) */
	virtual BOOL OnR		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(R) */
	virtual BOOL OnI		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(I) */
	virtual BOOL OnN		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(N) */
	virtual BOOL OnV		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(V) */
	virtual BOOL OnF		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(F) */
	virtual BOOL OnJ		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(J) */
	virtual BOOL OnL		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(L) */
	virtual BOOL OnAt		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(@) */
	virtual BOOL OnEnter	(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(Enter) */
	virtual BOOL OnEscape	(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(Escape) */
	virtual BOOL OnTab		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(Tab) */
	virtual BOOL OnShift	(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(Shift) */
	virtual BOOL OnCtrl		(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(Ctrl) */
	virtual BOOL OnSpace	(BOOL bDown);							/* ƒL[ƒnƒ“ƒhƒ‰(Space) */
	virtual void OnMgrDrawSTART_FADEIN	(DWORD dwPara) {}			/* ƒtƒF[ƒhƒCƒ“ŠJŽn */
	virtual void OnMgrDrawSTART_FADEOUT	(DWORD dwPara) {}			/* ƒtƒF[ƒhƒAƒEƒgŠJŽn */
	virtual void OnMgrDrawEND_FADEIN	(DWORD dwPara) {}			/* ƒtƒF[ƒhƒCƒ“Š®—¹ */
	virtual void OnMgrDrawEND_FADEOUT	(DWORD dwPara) {}			/* ƒtƒF[ƒhƒAƒEƒgŠ®—¹ */


protected:
	HWND			m_hWndMain,							/* ƒƒCƒ“ƒtƒŒ[ƒ€‚ÌƒEƒBƒ“ƒhƒEƒnƒ“ƒhƒ‹ */
					m_hWndAdmin;						/* ŠÇ—ŽÒƒEƒBƒ“ƒhƒE‚ÌƒEƒBƒ“ƒhƒEƒnƒ“ƒhƒ‹ */

	CUraraSockTCPSBO	*m_pSock;						/* ’ÊMƒ}ƒl[ƒWƒƒ */
	CMgrData			*m_pMgrData;					/* ƒf[ƒ^ŠÇ— */
	CMgrSound			*m_pMgrSound;					/* ƒTƒEƒ“ƒhŠÇ— */
	CMgrDraw			*m_pMgrDraw;					/* •`‰æŠÇ— */
	CMgrLayer			*m_pMgrLayer;					/* ƒŒƒCƒ„[ŠÇ— */
	CMgrWindow			*m_pMgrWindow;					/* ƒEƒBƒ“ƒhƒEŠÇ— */
	CMgrKeyInput		*m_pMgrKeyInput;				/* ƒL[“ü—ÍŠÇ— */
} CStateProcBase, *PCStateProcBase;

/* Copyright(C)URARA-works 2006 */
