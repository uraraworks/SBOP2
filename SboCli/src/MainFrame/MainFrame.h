/// @file MainFrame.h
/// @brief クライアントメインフレーム 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "Platform/IGameLoopHost.h"

class CMgrData;
class CUraraSockTCPSBO;
class CMgrGrpData;
class CMgrDraw;
class CMgrLayer;
class CMgrWindow;
class CMgrKeyInput;
class CMgrSound;
class CLibInfoCharCli;
class CLibInfoDisable;
class CLibInfoMapBase;
class CLibInfoMapObject;
class CLibInfoMapParts;
class CLibInfoMapShadow;
class CLibInfoItemType;
class CLibInfoItem;
class CLibInfoItemWeapon;
class CLibInfoEffect;
class CLibInfoMotion;
class CLibInfoMotionType;
class CLibInfoEfcBalloon;
class CLibInfoSystem;
class CLibInfoSkill;
class CStateProcBase;

// クラス宣言

class CMainFrame : public IGameLoopHost
{
public:
	CMainFrame(); // コンストラクタ
	virtual ~CMainFrame(); // デストラクタ

	int MainLoop(HINSTANCE hInstance); // メインループ
	void DisConnectProc(int nID); // 切断処理
	void ChgMoveState(BOOL bChgBGM); // プレイヤー移動状態変更処理
	void RenewItemArea(void); // 画面内のアイテム情報を更新
	void SendChat(int nType, LPCSTR pszMsg, DWORD *pdwDst=NULL); // チャット送信

	// IGameLoopHost 実装
	virtual BOOL OnSDLInit(SDL_Window *pWindow);
	virtual void OnSDLFocusChanged(BOOL bActive);
	virtual BOOL OnFrame(void);
	virtual void OnSDLKeyUp(int vk);
	virtual void OnSDLMouseMove(int x, int y);
	virtual void OnSDLMouseLeftButtonDown(int x, int y, BOOL bDoubleClick);
	virtual void OnSDLMouseRightButtonDown(int x, int y, BOOL bDoubleClick);
	virtual void OnSDLMouseRightButtonDoubleClick(int x, int y);
	virtual void OnDraw(SDL_Renderer *pRenderer);
	virtual BOOL IsQuit(void);
	virtual void OnSDLDestroy(void);

private:
	static LRESULT CALLBACK WndProcEntry(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
	void OnInitEnd(HWND hWnd);
	void OnDestroy(HWND hWnd);
	void OnPaint(HWND hWnd);
	void OnTimer(HWND hWnd, UINT id);
	void OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify);
	void OnKeyUp(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
	void OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void OnRButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void OnRButtonDblClk(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void OnMouseMove(HWND hWnd, int x, int y, UINT keyFlags);
	void OnMgrDraw(int nType, DWORD dwPara);
	void OnWindowMsg(int nType, DWORD dwPara);
	void OnMainFrame(DWORD dwCommand, DWORD dwParam);
	void OnAdminMsg(int nType, DWORD dwPara);
	void OnRecvCommand(DWORD dwMain, DWORD dwSub);
	void OnConnect(void);
	void OnDisConnect(void);
	void OnRecv(PBYTE pData);

	BOOL TimerProc(void);
	void KeyProc(void);
	void ChgGameState(int nGameState);
	void Connect(void);
	void FlashMainWindow(void);
	int GetMsgCmdType(LPCSTR pszText);

	// 受信処理 (MainFrameRecvProcVERSION.cpp)
	void RecvProcVERSION(BYTE byCmdSub, PBYTE pData);
	void RecvProcVERSION_RES_VERSIONCHECK(PBYTE pData);
	void RecvProcVERSION_RES_FILELISTCHECK(PBYTE pData);

	// 受信処理 (MainFrameRecvProcCONNECT.cpp)
	void RecvProcCONNECT(BYTE byCmdSub, PBYTE pData);
	void RecvProcCONNECT_RES_LOGIN(PBYTE pData);
	void RecvProcCONNECT_RES_PLAY(PBYTE pData);
	void RecvProcCONNECT_KEEPALIVE(PBYTE pData);

	// 受信処理 (MainFrameRecvProcACCOUNT.cpp)
	void RecvProcACCOUNT(BYTE byCmdSub, PBYTE pData);
	void RecvProcACCOUNT_RES_ACCOUNTINFO(PBYTE pData);
	void RecvProcACCOUNT_RES_MAKECHAR(PBYTE pData);

	// 受信処理 (MainFrameRecvProcCHAR.cpp)
	void RecvProcCHAR(BYTE byCmdSub, PBYTE pData);
	void RecvProcCHAR_RES_CHARINFO(PBYTE pData);
	void RecvProcCHAR_CHARINFO(PBYTE pData);
	void RecvProcCHAR_MOVE_START(PBYTE pData);
	void RecvProcCHAR_MOVE_DIR_CHANGE(PBYTE pData);
	void RecvProcCHAR_MOVE_STOP(PBYTE pData);
	void RecvProcCHAR_MOVE_CORE(DWORD dwCharID, int nDirection, int nPacketPosX, int nPacketPosY, BOOL bUpdate, BOOL bForceStop);
	void RecvProcCHAR_POS_SYNC(PBYTE pData);
	void RecvProcCHAR_STATE(PBYTE pData);
	void RecvProcCHAR_PROCSTATE(PBYTE pData);
	void RecvProcCHAR_CHAT(PBYTE pData);
	void RecvProcCHAR_CHARID(PBYTE pData);
	void RecvProcCHAR_RES_PUTGET(PBYTE pData);
	void RecvProcCHAR_SET_EFFECT(PBYTE pData);
	void RecvProcCHAR_ITEMINFO(PBYTE pData);
	void RecvProcCHAR_RES_TAIL(PBYTE pData);
	void RecvProcCHAR_MODIFY_PARAM(PBYTE pData);
	void RecvProcCHAR_GRP(PBYTE pData);
	void RecvProcCHAR_MOTION(PBYTE pData);
	void RecvProcCHAR_MOTIONTYPE(PBYTE pData);
	void RecvProcCHAR_SET_EFCBALLOON(PBYTE pData);
	void RecvProcCHAR_SET_MOTION(PBYTE pData);
	void RecvProcCHAR_RES_CHECKMAPEVENT(PBYTE pData);
	void RecvProcCHAR_STATUS(PBYTE pData);
	void RecvProcCHAR_TEXTEFFECT(PBYTE pData);
	void RecvProcCHAR_STATE_CHARGE(PBYTE pData);
	void RecvProcCHAR_RES_TALKEVENT(PBYTE pData);
	void RecvProcCHAR_SKILLINFO(PBYTE pData);

	// 受信処理 (MainFrameRecvProcMAP.cpp)
	void RecvProcMAP(BYTE byCmdSub, PBYTE pData);
	void RecvProcMAP_RES_MAPINFO(PBYTE pData);
	void RecvProcMAP_ONLINE(PBYTE pData);
	void RecvProcMAP_SYSTEMMSG(PBYTE pData);
	void RecvProcMAP_FORMATMSG(PBYTE pData);
	void RecvProcMAP_MAPOBJECT(PBYTE pData);
	void RecvProcMAP_MAPOBJECTDATA(PBYTE pData);
	void RecvProcMAP_DELETEMAPOBJECTDATA(PBYTE pData);
	void RecvProcMAP_MAPPARTS(PBYTE pData);
	void RecvProcMAP_SETPARTS(PBYTE pData);
	void RecvProcMAP_RENEWMAPSIZE(PBYTE pData);
	void RecvProcMAP_DELETEPARTS(PBYTE pData);
	void RecvProcMAP_MAPSHADOW(PBYTE pData);
	void RecvProcMAP_SETMAPSHADOW(PBYTE pData);
	void RecvProcMAP_DELETEMAPSHADOW(PBYTE pData);
	void RecvProcMAP_MAPNAME(PBYTE pData);
	void RecvProcMAP_MAPEVENT(PBYTE pData);
	void RecvProcMAP_DELETEEVENT(PBYTE pData);
	void RecvProcMAP_FADEINOUT(PBYTE pData);

	// 受信処理 (MainFrameRecvProcITEM.cpp)
	void RecvProcITEM(BYTE byCmdSub, PBYTE pData);
	void RecvProcITEM_RES_ITEMINFO(PBYTE pData);
	void RecvProcITEM_ITEMINFO(PBYTE pData);
	void RecvProcITEM_DELETEITEMINFO(PBYTE pData);
	void RecvProcITEM_RES_ITEMTYPEINFO(PBYTE pData);
	void RecvProcITEM_ITEMTYPEINFO(PBYTE pData);
	void RecvProcITEM_DELETEITEMTYPEINFO(PBYTE pData);
	void RecvProcITEM_RES_ITEMWEAPONINFO(PBYTE pData);
	void RecvProcITEM_ITEMWEAPONINFO(PBYTE pData);

	// 受信処理 (MainFrameRecvProcADMIN.cpp)
	void RecvProcADMIN(BYTE byCmdSub, PBYTE pData);
	void RecvProcADMIN_RES_ADMINLEVEL(PBYTE pData);
	void RecvProcADMIN_RENEWADMINLEVEL(PBYTE pData);
	void RecvProcADMIN_PLAYSOUND(PBYTE pData);
	void RecvProcADMIN_CHAR_RES_ACCOUNT(PBYTE pData);
	void RecvProcADMIN_CHAR_RES_ONLINE(PBYTE pData);
	void RecvProcADMIN_DISABLE_RES_INFO(PBYTE pData);

	// 受信処理 (MainFrameRecvProcEFFECT.cpp)
	void RecvProcEFFECT(BYTE byCmdSub, PBYTE pData);
	void RecvProcEFFECT_RES_EFFECTINFO(PBYTE pData);
	void RecvProcEFFECT_EFFECTINFO(PBYTE pData);
	void RecvProcEFFECT_DELETEEFFECTINFO(PBYTE pData);
	void RecvProcEFFECT_BALLOONINFO(PBYTE pData);

	// 受信処理 (MainFrameRecvProcSYSTEM.cpp)
	void RecvProcSYSTEM(BYTE byCmdSub, PBYTE pData);
	void RecvProcSYSTEM_INFO(PBYTE pData);

	// 受信処理 (MainFrameRecvProcSKILL.cpp)
	void RecvProcSKILL(BYTE byCmdSub, PBYTE pData);
	void RecvProcSKILL_SKILLINFO(PBYTE pData);

private:
	HWND m_hWnd;
	BOOL m_bWindowActive,
		m_bRenewCharInfo;
	int m_nGameState,
		m_nDrawCount,
		m_nFPS;
	DWORD m_dwLastTimeCheck,
		m_dwDrawTime;
	HRESULT m_hCom;
	SYSTEMTIME m_stSystemTime;

	CMgrData *m_pMgrData;
	CUraraSockTCPSBO *m_pSock;
	CMgrGrpData *m_pMgrGrpData;
	CMgrDraw *m_pMgrDraw;
	CMgrLayer *m_pMgrLayer;
	CMgrWindow *m_pMgrWindow;
	CMgrKeyInput *m_pMgrKeyInput;
	CMgrSound *m_pMgrSound;
	CLibInfoCharCli *m_pLibInfoChar;
	CLibInfoDisable *m_pLibInfoDisable;
	CLibInfoMapBase *m_pLibInfoMap;
	CLibInfoMapObject *m_pLibInfoMapObject;
	CLibInfoMapParts *m_pLibInfoMapParts;
	CLibInfoMapShadow *m_pLibInfoMapShadow;
	CLibInfoItemType *m_pLibInfoItemType;
	CLibInfoItem *m_pLibInfoItem;
	CLibInfoItemWeapon *m_pLibInfoItemWeapon;
	CLibInfoEffect *m_pLibInfoEffect;
	CLibInfoMotion *m_pLibInfoMotion;
	CLibInfoMotionType *m_pLibInfoMotionType;
	CLibInfoEfcBalloon *m_pLibInfoEfcBalloon;
	CLibInfoSystem *m_pLibInfoSystem;
	CLibInfoSkill *m_pLibInfoSkill;
	CStateProcBase *m_pStateProc;
};
