/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MainFrame.cpp												 */
/* ���e			:�N���C�A���g���C���t���[�� �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include <comdef.h>
#include "resource.h"
#include "SBOVersion.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "ParamUtil.h"
#include "InfoMapBase.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapShadow.h"
#include "LibInfoItem.h"
#include "LibInfoItemWeapon.h"
#include "LibInfoEfcBalloon.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "StateProc.h"
#include "WindowLOGIN.h"
#include "LayerMap.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "MgrWindow.h"
#include "MgrKeyInput.h"
#include "MgrSound.h"
#include "MainFrame.h"


/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

#define CLNAME "SboCli"								/* �o�^�N���X�� */
#define TIMERID_TOOLCHECK	100						/* �c�[���`�F�b�N�^�C�}�[ */
#define TIMER_TOOLCHECK		1000					/* �c�[���`�F�b�N�^�C�}�[���� */

/* ���b�Z�[�W�R�}���h��� */
enum {
	MSGCMDTYPE_NONE = 0,
	MSGCMDTYPE_CHGFACE,								/* �\��ύX */
	MSGCMDTYPE_CHGHAIR,								/* ���ύX */
	MSGCMDTYPE_CHGCLOTH,							/* �����ύX */
	MSGCMDTYPE_CHGACCE,								/* �A�N�Z�T���ύX */
	MSGCMDTYPE_BGMVOLUME,							/* BGM���ʐݒ� */
	MSGCMDTYPE_CHGCOLOR,							/* �F�ύX */
	MSGCMDTYPE_SEVOLUME,							/* ���ʉ��ʐݒ� */
	MSGCMDTYPE_CHGARMS,								/* �������ύX */
	MSGCMDTYPE_CHGSHIELD,							/* ���ύX */
	MSGCMDTYPE_SETITEM,								/* �z�u�A�C�e���ύX */
	MSGCMDTYPE_MAKEITEM,							/* �A�C�e���쐬 */
	MSGCMDTYPE_BALLOON,								/* ���o�� */
	MSGCMDTYPE_DICE,								/* �T�C�R�� */
	MSGCMDTYPE_RND,									/* �����_�� */
	MSGCMDTYPE_NOW,									/* ���ݎ��� */
	MSGCMDTYPE_EFFECT,								/* �G�t�F�N�g */
	MSGCMDTYPE_WHERE,								/* �ł��W�܂��Ă���ꏊ */
	MSGCMDTYPE_MAX
};


/* ========================================================================= */
/* �֐���	:CMainFrame::CMainFrame											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/05/04														 */
/* ========================================================================= */

CMainFrame::CMainFrame()
{
	m_hWnd				= NULL;
	m_bWindowActive		= TRUE;
	m_bRenewCharInfo	= FALSE;
	m_nGameState		= GAMESTATE_NONE;
	m_nDrawCount		= 30;
	m_nFPS				= 0;
	m_dwLastTimeCheck	= 0;
	m_dwDrawTime		= 0;

	m_pStateProc	= NULL;
	m_pMgrData		= new CMgrData;
	m_pSock			= new CUraraSockTCPSBO;
	m_pMgrGrpData	= new CMgrGrpData;

	m_pMgrDraw				= NULL;
	m_pMgrLayer				= NULL;
	m_pMgrWindow			= NULL;
	m_pMgrKeyInput			= NULL;
	m_pMgrSound				= NULL;
	m_pLibInfoChar			= NULL;
	m_pLibInfoMap			= NULL;
	m_pLibInfoMapParts		= NULL;
	m_pLibInfoMapShadow		= NULL;
	m_pLibInfoItemType		= NULL;
	m_pLibInfoItem			= NULL;
	m_pLibInfoItemWeapon	= NULL;
	m_pLibInfoMotion		= NULL;
	m_pLibInfoMotionType	= NULL;
	m_pLibInfoEfcBalloon	= NULL;
	m_pLibInfoEffect		= NULL;
	m_pLibInfoSystem		= NULL;

	m_hCom = CoInitializeEx (0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	ZeroMemory (&m_stSystemTime, sizeof (m_stSystemTime));
}


/* ========================================================================= */
/* �֐���	:CMainFrame::~CMainFrame										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

CMainFrame::~CMainFrame()
{
	SAFE_DELETE (m_pStateProc);
	SAFE_DELETE (m_pMgrGrpData);
	SAFE_DELETE (m_pSock);
	SAFE_DELETE (m_pMgrData);

	if (SUCCEEDED (m_hCom)) {
		CoUninitialize ();
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::MainLoop											 */
/* ���e		:���C�����[�v													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

int CMainFrame::MainLoop(HINSTANCE hInstance)
{
	char szBuf[256];
	BYTE byFps, byNowFrame, byFrameBack, byFrameTmp;
	DWORD dwStyle, dwTimeTmp, dwTimeFps, dwTimeStart;
	BOOL bResult, bDraw;
	RECT rcTmp;
	MSG msg;
	TIMECAPS tc;
	WNDCLASS wc;

	wc.hInstance		= hInstance;
	wc.lpszClassName	= CLNAME;
	wc.lpfnWndProc		= (WNDPROC)WndProcEntry;
	wc.style			= CS_DBLCLKS;
	wc.hIcon			= LoadIcon (hInstance, MAKEINTRESOURCE (IDI_SBO));
	wc.hCursor			= LoadCursor ((HINSTANCE)NULL, IDC_ARROW);
	wc.lpszMenuName		= NULL;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hbrBackground	= (HBRUSH)GetStockObject (BLACK_BRUSH);

	/* �E�B���h�E�̃N���X��o�^ */
	if (!RegisterClass (&wc)) {
		return FALSE;
	}

	/* �E�B���h�E�T�C�Y���v�Z */
	dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_MINIMIZEBOX;
	SetRect (&rcTmp, 0, 0, SCRSIZEX, SCRSIZEY);
	AdjustWindowRect (&rcTmp, dwStyle, FALSE);

	/* �E�B���h�E�쐬 */
	wsprintf (szBuf, "%s %s", WNDTITLE, VERTEXT);
	m_hWnd = CreateWindow (
				CLNAME,
				szBuf,
				dwStyle,
				CW_USEDEFAULT, CW_USEDEFAULT,
				rcTmp.right - rcTmp.left, rcTmp.bottom - rcTmp.top,
				NULL,
				NULL,
				hInstance,
				this);
	if (m_hWnd == NULL) {
		return FALSE;
	}

	timeGetDevCaps (&tc, sizeof (TIMECAPS));
	/* �}���`���f�B�A�^�C�}�[�̃T�[�r�X���x���ő�� */
	timeBeginPeriod (tc.wPeriodMin);

	byFps		= 0;
	m_nFPS		= 1000 / m_nDrawCount + 1;
	byFrameBack	= 0;
	dwTimeStart	= timeGetTime ();

	while (1) {
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			if (msg.message == WM_SYSKEYDOWN) {
				if ((msg.wParam == VK_MENU) || (msg.wParam == VK_F10)) {
					continue;
				}
			}
			TranslateMessage (&msg);
			DispatchMessage (&msg);

		} else {
			dwTimeTmp	= timeGetTime ();
			bResult		= TimerProc ();
			bDraw		|= bResult;
			/* ���ԓI�ɕ`�悷�邾�낤�t���[���ԍ������߂� */
			byNowFrame = (BYTE)((dwTimeTmp - dwTimeStart) / m_nFPS);
			if ((byFps <= m_nDrawCount) && (byFrameBack != byNowFrame)) {
				if (byFps && (byNowFrame - byFrameBack != 1)) {
					bDraw = FALSE;
				}
				byFrameTmp = (BYTE)(m_dwDrawTime / m_nFPS);
				if (byFrameTmp >= byNowFrame) {
					bDraw = FALSE;
				}
				KeyProc ();
				if (bDraw) {
					InvalidateRect (m_hWnd, NULL, FALSE);
					OnPaint (m_hWnd);
					byFps ++;
				}
				bDraw = FALSE;
				byFrameBack = byNowFrame;
			}

			if (dwTimeTmp > dwTimeStart + 1000) {
				/* FPS�̍X�V */
				dwTimeStart = dwTimeFps = timeGetTime ();
				m_dwDrawTime = 0;
				byFps = 0;
				bDraw = FALSE;
			}
		}
	}

	timeEndPeriod (tc.wPeriodMin);
	UnregisterClass (CLNAME, hInstance);

	/* �I�����b�Z�[�W�ɂ��v���O�����I�� */
	return (int)msg.wParam;
}


/* ========================================================================= */
/* �֐���	:CMainFrame::DisConnectProc										 */
/* ���e		:�ؒf����														 */
/* ���t		:2005/06/12														 */
/* ========================================================================= */

void CMainFrame::DisConnectProc(int nID)
{
	PCWindowLOGIN pWindow;

	m_pSock->Destroy ();

	pWindow = (PCWindowLOGIN)m_pMgrWindow->GetWindow (WINDOWTYPE_LOGIN);
	if (pWindow == NULL) {
		return;
	}

	switch (nID) {
	case DISCONNECTID_CONNECT:		/* �ڑ����s */
		m_pMgrWindow->MakeWindowMSG ("�T�[�o�[�ɐڑ��ł��܂���ł���", 3000);
		pWindow->Enable (TRUE);
		break;
	case DISCONNECTID_VERSION:		/* �o�[�W�����s��v */
		m_pMgrWindow->MakeWindowMSG ("�N���C�A���g���ŐV�ɂ��Ă�������", 3000);
		pWindow->Enable (TRUE);
		break;
	case DISCONNECTID_USER:			/* ���� */
	case DISCONNECTID_DISABLE:		/* �֎~ */
		pWindow->Enable (TRUE);
		break;
	case DISCONNECTID_PASSWORD:		/* �p�X���[�h�s��v */
		m_pMgrWindow->MakeWindowMSG ("�p�X���[�h���Ԉ���Ă��܂�", 3000);
		pWindow->Enable (TRUE);
		break;
	case DISCONNECTID_LOGIN:		/* ���O�C���ς� */
		m_pMgrWindow->MakeWindowMSG ("���O�C���ς݂ł�", 3000);
		pWindow->Enable (TRUE);
		break;
	case DISCONNECTID_MAC:			/* �쐬�ς� */
		m_pMgrWindow->MakeWindowMSG ("����PC�ł̓A�J�E���g�쐬�ς݂ł�", 3000);
		pWindow->Enable (TRUE);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::ChgMoveState										 */
/* ���e		:�v���C���[�ړ���ԕύX����										 */
/* ���t		:2008/06/29														 */
/* ========================================================================= */

void CMainFrame::ChgMoveState(
	BOOL bChgBGM)		/* [in] TRUE:BGM��ς��� */
{
	BOOL bChgWait;
	DWORD dwMoveWait;
	PCLayerMap pLayerMap;
	PCInfoMapBase pInfoMap;
	PCInfoCharCli pPlayerChar;

	bChgWait = FALSE;
	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}
	pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (pPlayerChar == NULL) {
		return;
	}
	pInfoMap = m_pMgrData->GetMap ();

	dwMoveWait = pPlayerChar->GetMoveWait ();
	if (pPlayerChar->m_bChargeAtack) {
		bChgWait = TRUE;
		dwMoveWait *= 2;
	}

	switch (pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_MOVE:			/* �ړ��� */
	case CHARMOVESTATE_BATTLEMOVE:		/* �퓬�ړ��� */
	case CHARMOVESTATE_BATTLEATACK:		/* �퓬�U���� */
		bChgWait = TRUE;
		break;
	case CHARMOVESTATE_STAND:			/* ���� */
	case CHARMOVESTATE_SWOON:			/* �C�� */
		if (bChgBGM) {
			m_pMgrSound->PlayBGM (pInfoMap->m_dwBGMID);
		}
		bChgWait = TRUE;
		break;
	case CHARMOVESTATE_BATTLE:			/* �퓬�� */
		if (bChgBGM) {
			m_pMgrSound->PlaySound (SOUNDID_CHARI14_A);
			m_pMgrSound->PlayBGM (BGMID_DAICHI_S);
		}
		bChgWait = TRUE;
		break;
	}
	if (bChgWait) {
		pLayerMap->SetScrollWait (dwMoveWait);
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RenewItemArea										 */
/* ���e		:��ʓ��̃A�C�e�������X�V										 */
/* ���t		:2008/06/29														 */
/* ========================================================================= */

void CMainFrame::RenewItemArea(void)
{
	RECT rcTmp;
	PCLayerMap pLayerMap;
	PCInfoCharCli pPlayerChar;

	pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (pPlayerChar == NULL) {
		return;
	}
	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}

	rcTmp.left	 = pLayerMap->m_nViewX - 2;
	rcTmp.right	 = pLayerMap->m_nViewX + (DRAW_PARTS_X * 2) + 2;
	rcTmp.top	 = pLayerMap->m_nViewY - 2;
	rcTmp.bottom = pLayerMap->m_nViewY + (DRAW_PARTS_Y * 2) + 2;
	m_pLibInfoItem->SetArea (pPlayerChar->m_dwMapID, &rcTmp);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::WndProcEntry										 */
/* ���e		:���C���E�B���h�E�v���V�[�W��(�G���g���|�C���g)					 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

LRESULT CALLBACK CMainFrame::WndProcEntry(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pThis;

	if (msg == WM_CREATE) {
		SetWindowLong (hWnd, GWL_USERDATA, (LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams));
	}

	/* ���[�U�f�[�^���� this �|�C���^���擾���A�������s�� */
	pThis = (CMainFrame *)GetWindowLong (hWnd, GWL_USERDATA);
	if (pThis) {
		return pThis->WndProc (hWnd, msg, wParam, lParam);
	}
	return DefWindowProc (hWnd, msg, wParam, lParam);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::SendChat											 */
/* ���e		:�`���b�g����													 */
/* ���t		:2008/07/25														 */
/* ========================================================================= */

void CMainFrame::SendChat(int nType, LPCSTR pszMsg, DWORD *pdwDst)
{
	int nTmp, nCmdType, nCount;
	PCInfoCharCli pPlayerChar;
	CParamUtil ParamUtil;

	pPlayerChar = m_pMgrData->GetPlayerChar ();

	nCmdType = MSGCMDTYPE_NONE;
	ParamUtil.Set (pszMsg);
	nCount = ParamUtil.GetCount ();
	if (nCount > 0) {
		nCmdType = GetMsgCmdType (pszMsg);
	}

	switch (nCmdType) {
	case MSGCMDTYPE_CHGFACE:			/* �\��ύX */
		{
			CPacketMSGCMD_CHGFACE Packet;

			Packet.Make (pPlayerChar->m_dwCharID, atoi (ParamUtil.GetParam (1)));
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_CHGHAIR:			/* ���ύX */
		{
			CPacketMSGCMD_PARA1 Packet;

			Packet.Make (MSGCMDTYPE_CHGHAIR, pPlayerChar->m_dwCharID, atoi (ParamUtil.GetParam (1)));
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_CHGCLOTH:			/* �����ύX */
		{
			CPacketMSGCMD_CHGCLOTH Packet;

			Packet.Make (pPlayerChar->m_dwCharID, atoi (ParamUtil.GetParam (1)));
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_CHGACCE:			/* �A�N�Z�T���ύX */
		{
			CPacketMSGCMD_CHGACCE Packet;

			Packet.Make (pPlayerChar->m_dwCharID, atoi (ParamUtil.GetParam (1)));
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_BGMVOLUME:			/* BGM���ʐݒ� */
		nTmp = atoi (ParamUtil.GetParam (1));
		m_pMgrData->SetBGMVolume (nTmp);
		m_pMgrSound->SetBGMVolume (nTmp);
		break;
	case MSGCMDTYPE_CHGCOLOR:			/* �F�ύX */
		{
			CPacketMSGCMD_CHGCOLOR Packet;

			Packet.Make (pPlayerChar->m_dwCharID, atoi (ParamUtil.GetParam (1)));
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_SEVOLUME:			/* ���ʉ��ʐݒ� */
		nTmp = atoi (ParamUtil.GetParam (1));
		m_pMgrData->SetSEVolume (nTmp);
		m_pMgrSound->SetSEVolume (nTmp);
		break;
	case MSGCMDTYPE_CHGARMS:			/* �������ύX */
		{
			CPacketMSGCMD_CHGARMS Packet;

			Packet.Make (pPlayerChar->m_dwCharID, atoi (ParamUtil.GetParam (1)));
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_CHGSHIELD:			/* ���ύX */
		{
			int nTmp;
			LPCSTR pszTmp;
			CPacketMSGCMD_PARA1 Packet;

			nTmp = 0;
			pszTmp = ParamUtil.GetParam (1);
			if (pszTmp) {
				nTmp = atoi (pszTmp);
			}
			Packet.Make (SBOCOMMANDID_SUB_MSGCMD_CHGSHIELD, pPlayerChar->m_dwCharID, nTmp);
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_SETITEM:			/* �z�u�A�C�e���ύX */
		m_pMgrData->SetSetItemID (atoi (ParamUtil.GetParam (1)));
		break;
	case MSGCMDTYPE_MAKEITEM:			/* �A�C�e���쐬 */
		{
			CPacketMSGCMD_MAKEITEM Packet;

			Packet.Make (pPlayerChar->m_dwCharID, atoi (ParamUtil.GetParam (1)));
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_BALLOON:			/* ���o�� */
		{
			LPCSTR pszTmp;
			CPacketMSGCMD_PARA1 Packet;

			pszTmp = ParamUtil.GetParam (0);
			Packet.Make (SBOCOMMANDID_SUB_MSGCMD_BALLOON, pPlayerChar->m_dwCharID, atoi (&pszTmp[1]));
			m_pSock->Send (&Packet);
			if (pdwDst) {
				*pdwDst = atoi (&pszTmp[1]);
			}
		}
		break;
	case MSGCMDTYPE_DICE:			/* �T�C�R�� */
		{
			CPacketMSGCMD_PARA1 Packet;

			Packet.Make (SBOCOMMANDID_SUB_MSGCMD_DICE, pPlayerChar->m_dwCharID, 0);
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_RND:			/* �����_�� */
		{
			int nTmp;
			LPCSTR pszTmp;
			CPacketMSGCMD_PARA1 Packet;

			nTmp = 0;
			pszTmp = ParamUtil.GetParam (1);
			if (pszTmp) {
				nTmp = atoi (pszTmp);
			}
			Packet.Make (SBOCOMMANDID_SUB_MSGCMD_RND, pPlayerChar->m_dwCharID, nTmp);
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_NOW:			/* ���ݎ��� */
		{
			SYSTEMTIME sysTime;
			CPacketCHAR_REQ_CHAT Packet;
			CmyString strTmp;

			GetLocalTime (&sysTime);
			strTmp.Format ("%02d:%02d:%02d", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			Packet.Make (nType, pPlayerChar->m_dwCharID, (LPCSTR)strTmp);
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_EFFECT:			/* �G�t�F�N�g */
		{
			LPCSTR pszTmp;
			CPacketMSGCMD_PARA1 Packet;

			pszTmp = ParamUtil.GetParam (0);
			Packet.Make (SBOCOMMANDID_SUB_MSGCMD_EFFECT, pPlayerChar->m_dwCharID, atoi (&pszTmp[1]));
			m_pSock->Send (&Packet);
			if (pdwDst) {
				*pdwDst = atoi (&pszTmp[1]);
			}
		}
		break;
	case MSGCMDTYPE_WHERE:			/* �ł��W�܂��Ă���ꏊ */
		{
			CPacketMSGCMD_PARA1 Packet;

			Packet.Make (SBOCOMMANDID_SUB_MSGCMD_WHERE, pPlayerChar->m_dwCharID, 0);
			m_pSock->Send (&Packet);
		}
		break;
	default:
		{
			CPacketCHAR_REQ_CHAT Packet;

			Packet.Make (nType, pPlayerChar->m_dwCharID, pszMsg);
			m_pSock->Send (&Packet);
		}
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::WndProc											 */
/* ���e		:���C���E�B���h�E�v���V�[�W��									 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

LRESULT CMainFrame::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	HANDLE_MSG (hWnd, WM_CREATE,		OnCreate);
	HANDLE_MSG (hWnd, WM_CLOSE,			OnClose);
	HANDLE_MSG (hWnd, WM_DESTROY,		OnDestroy);
	HANDLE_MSG (hWnd, WM_PAINT,			OnPaint);
	HANDLE_MSG (hWnd, WM_TIMER,			OnTimer);
	HANDLE_MSG (hWnd, WM_COMMAND,		OnCommand);
	HANDLE_MSG (hWnd, WM_ACTIVATE,		OnActivate);
	HANDLE_MSG (hWnd, WM_KEYUP,			OnKeyUp);
	HANDLE_MSG (hWnd, WM_LBUTTONDOWN,	OnLButtonDown);
	HANDLE_MSG (hWnd, WM_RBUTTONDOWN,	OnRButtonDown);
	HANDLE_MSG (hWnd, WM_RBUTTONDBLCLK,	OnRButtonDblClk);
	HANDLE_MSG (hWnd, WM_MOUSEMOVE,		OnMouseMove);

	case WM_INITEND:				/* ���������� */
		OnInitEnd (hWnd);
		break;
	case WM_CTLCOLORSTATIC:
		{
			int nID = GetWindowLong ((HWND)lParam, GWL_ID);
			HDC hDC = (HDC)wParam;

			if (nID != IDC_SAVEPASSWORD) {
				return DefWindowProc (hWnd, msg, wParam, lParam);
			}
			SetBkMode (hDC, TRANSPARENT);
			SetTextColor (hDC, RGB(0, 0, 0));
		}
		return (LRESULT)GetStockObject(NULL_BRUSH);
	case WM_MAINFRAME:				/* ���C���t���[���ւ̒ʒm */
		OnMainFrame (wParam, lParam);
		break;
	case WM_WINDOWMSG:				/* �E�B���h�E����̒ʒm */
		OnWindowMsg (wParam, lParam);
		break;
	case WM_ADMINMSG:				/* �Ǘ��҃E�B���h�E�֘A�̒ʒm */
		OnAdminMsg (wParam, lParam);
		break;
	case WM_MGRDRAW:				/* �`��Ǘ�����̒ʒm */
		OnMgrDraw (wParam, lParam);
		break;
	case WM_ERASEBKGND:
		break;

	default:
		if ((msg >= URARASOCK_MSGBASE) && (msg < URARASOCK_MSGBASE + WM_URARASOCK_MAX)) {
			switch (msg - URARASOCK_MSGBASE) {
			case WM_URARASOCK_CONNECT:		/* �ڑ� */
				OnConnect ();
				break;
			case WM_URARASOCK_DISCONNECT:	/* �ؒf */
				OnDisConnect ();
				break;
			case WM_URARASOCK_RECV:			/* ��M */
				OnRecv ((PBYTE)wParam);
				break;
			}
		} else {
			return DefWindowProc (hWnd, msg, wParam, lParam);
		}
	}
	return 0;
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnCreate											 */
/* ���e		:���b�Z�[�W�n���h��(WM_CREATE)									 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

BOOL CMainFrame::OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	char szFileName[MAX_PATH];
	POINT pt;

	ZeroMemory (szFileName, sizeof (szFileName));
	GetModuleFileName (NULL, szFileName, MAX_PATH);
	strcpy (szFileName + strlen (szFileName) - 3, "ini");

	pt.x = GetPrivateProfileInt ("Pos", "MainX", -1, szFileName);
	pt.y = GetPrivateProfileInt ("Pos", "MainY", -1, szFileName);
	if (!((pt.x == -1) && (pt.y == -1))) {
		SetWindowPos (hWnd, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE);
	}

	SetTimer (hWnd, TIMERID_TOOLCHECK, TIMER_TOOLCHECK, NULL);
	PostMessage (hWnd, WM_INITEND, 0, 0);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnInitEnd											 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITEND)									 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMainFrame::OnInitEnd(HWND hWnd)
{
	BOOL bRet, bResult;
	char szName[MAX_PATH], szTmp[MAX_PATH];
	LPSTR pszTmp;
	GUID stGuid;

	bRet = FALSE;

	/* �e�}�l�[�W���N���X���쐬 */
	m_pMgrData->	SetWindowInfo (GetModuleHandle (NULL), hWnd);
	m_pMgrData->	Create (this, m_pMgrGrpData);
	m_pMgrData->	SetUraraSockTCP (m_pSock);

	m_pMgrDraw				= m_pMgrData->GetMgrDraw ();
	m_pMgrLayer				= m_pMgrData->GetMgrLayer ();
	m_pMgrWindow			= m_pMgrData->GetMgrWindow ();
	m_pMgrKeyInput			= m_pMgrData->GetMgrKeyInput ();
	m_pMgrSound				= m_pMgrData->GetMgrSound ();
	m_pLibInfoChar			= m_pMgrData->GetLibInfoChar ();
	m_pLibInfoMap			= m_pMgrData->GetLibInfoMap ();
	m_pLibInfoMapParts		= m_pMgrData->GetLibInfoMapParts ();
	m_pLibInfoMapShadow		= m_pMgrData->GetLibInfoMapShadow ();
	m_pLibInfoItemType		= m_pMgrData->GetLibInfoItemType ();
	m_pLibInfoItem			= m_pMgrData->GetLibInfoItem ();
	m_pLibInfoItemWeapon	= m_pMgrData->GetLibInfoItemWeapon ();
	m_pLibInfoMotion		= m_pMgrData->GetLibInfoMotion ();
	m_pLibInfoMotionType	= m_pMgrData->GetLibInfoMotionType ();
	m_pLibInfoEfcBalloon	= m_pMgrData->GetLibInfoEfcBalloon ();
	m_pLibInfoEffect		= m_pMgrData->GetLibInfoEffect ();
	m_pLibInfoSystem		= m_pMgrData->GetLibInfoSystem ();

	/* �O���t�B�b�N�f�[�^DLL�̓ǂݍ��� */
	bResult = m_pMgrGrpData->Load ();
	if (bResult == FALSE) {
		MessageBox (hWnd, "�O���t�B�b�N�f�[�^DLL�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
		goto Exit;
	}

	stGuid = m_pMgrData->GetInputGuid ();
	m_pMgrKeyInput->SetDevice (stGuid, hWnd);

	GetModuleFileName (NULL, szName, MAX_PATH);
	pszTmp		= strrchr (szName, '\\');
	pszTmp[1]	= 0;
	wsprintf (szTmp, "%sss", szName);
	CreateDirectory (szTmp, NULL);
	wsprintf (szTmp, "%sLog", szName);
	CreateDirectory (szTmp, NULL);

	ChgGameState (GAMESTATE_LOGO);
	ShowWindow (hWnd, SW_SHOW);

	PostMessage (hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWVIEWSET, 0);

	bRet = TRUE;
Exit:
	if (bRet == FALSE) {
		PostMessage (hWnd, WM_CLOSE, 0, 0);
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnClose											 */
/* ���e		:���b�Z�[�W�n���h��(WM_CLOSE)									 */
/* ���t		:2008/06/09														 */
/* ========================================================================= */

void CMainFrame::OnClose(HWND hWnd)
{
	RECT rc;
	char szFileName[MAX_PATH];
	HWND hWndTmp;
	CmyString strTmp;

	ZeroMemory (szFileName, sizeof (szFileName));

	GetModuleFileName (NULL, szFileName, MAX_PATH);
	strcpy (szFileName + strlen (szFileName) - 3, "ini");

	if ((IsIconic (hWnd) == FALSE) && (IsWindowVisible (hWnd))) {
		GetWindowRect (hWnd, &rc);

		/* ���C���E�B���h�E */
		strTmp.Format ("%d", rc.left);
		WritePrivateProfileString ("Pos", "MainX", strTmp, szFileName);
		strTmp.Format ("%d", rc.top);
		WritePrivateProfileString ("Pos", "MainY", strTmp, szFileName);

		if (m_nGameState == GAMESTATE_MAP) {
			/* ���O�E�B���h�E */
			((CStateProcMAP *)m_pStateProc)->GetMsgLogRect (rc);
			strTmp.Format ("%d", rc.left);
			WritePrivateProfileString ("Pos", "LogLeft", strTmp, szFileName);
			strTmp.Format ("%d", rc.top);
			WritePrivateProfileString ("Pos", "LogTop", strTmp, szFileName);
			strTmp.Format ("%d", rc.right);
			WritePrivateProfileString ("Pos", "LogRight", strTmp, szFileName);
			strTmp.Format ("%d", rc.bottom);
			WritePrivateProfileString ("Pos", "LogBottom", strTmp, szFileName);

			hWndTmp = m_pMgrData->GetAdminWindow ();
			if (hWndTmp) {
				/* �Ǘ��҃E�B���h�E */
				GetWindowRect (hWndTmp, &rc);
				strTmp.Format ("%d", rc.left);
				WritePrivateProfileString ("Pos", "AdminX", strTmp, szFileName);
				strTmp.Format ("%d", rc.top);
				WritePrivateProfileString ("Pos", "AdminY", strTmp, szFileName);
			}
		}
	}
	m_pMgrData->SaveIniData ();
	m_pSock->Destroy ();

	DestroyWindow (hWnd);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnDestroy											 */
/* ���e		:���b�Z�[�W�n���h��(WM_DESTROY)									 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMainFrame::OnDestroy(HWND hWnd)
{
	PostQuitMessage (0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnPaint											 */
/* ���e		:���b�Z�[�W�n���h��(WM_PAINT)									 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMainFrame::OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	DWORD dwTmp;
	HDC hDC;

	dwTmp	= timeGetTime ();
	hDC		= BeginPaint (hWnd, &ps);

	m_pMgrDraw->Draw (hDC);

	dwTmp = timeGetTime () - dwTmp;
	m_dwDrawTime += dwTmp;

	EndPaint (hWnd, &ps);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnTimer											 */
/* ���e		:���b�Z�[�W�n���h��(WM_TIMER)									 */
/* ���t		:2008/08/03														 */
/* ========================================================================= */

void CMainFrame::OnTimer(HWND hWnd, UINT id)
{
	switch (id) {
	case TIMERID_TOOLCHECK:		/* �c�[���`�F�b�N�^�C�}�[ */
		{
			DWORD dwTmp, dwScond, dwTimeTmp;
			SYSTEMTIME sysTime;

			if (m_dwLastTimeCheck == 0) {
				GetLocalTime (&m_stSystemTime);
				m_dwLastTimeCheck = timeGetTime ();
				break;
			}
			GetLocalTime (&sysTime);
			dwTimeTmp = timeGetTime ();
			if (sysTime.wMinute != m_stSystemTime.wMinute) {
				m_dwLastTimeCheck = 0;
				break;
			}
			if (sysTime.wSecond == m_stSystemTime.wSecond) {
				break;
			}
			dwTmp	= (dwTimeTmp - m_dwLastTimeCheck) / 1000;
			dwScond	= abs (sysTime.wSecond - m_stSystemTime.wSecond);
			if (dwScond == dwTmp) {
				m_dwLastTimeCheck = 0;
				break;
			}
			if (dwTmp < dwScond * 3) {
				m_dwLastTimeCheck = 0;
				break;
			}
			PostMessage (hWnd, WM_CLOSE, 0, 0);
		}
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnCommand											 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2005/06/11														 */
/* ========================================================================= */

void CMainFrame::OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify)
{
	/* �{�^���������ꂽ�H */
	if (codeNotify == BN_CLICKED) {
		/* �e�E�B���h�E�ŏ��������� */
		PostMessage (hWndCtl, WM_COMMAND, MAKELONG (codeNotify, id), (LPARAM)hWndCtl);
		return;
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnActivate											 */
/* ���e		:���b�Z�[�W�n���h��(WM_ACTIVATE)								 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CMainFrame::OnActivate(HWND hWnd, UINT state, HWND hwndActDeact, BOOL fMinimized)
{
	/* ��A�N�e�B�u�ɂȂ�H */
	if (state == WA_INACTIVE) {
		m_bWindowActive = FALSE;
		m_pMgrKeyInput->Reset ();

	} else {
		m_bWindowActive = TRUE;
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnKeyUp											 */
/* ���e		:���b�Z�[�W�n���h��(WM_KEYUP)									 */
/* ���t		:2007/02/13														 */
/* ========================================================================= */

void CMainFrame::OnKeyUp(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	if (m_pStateProc == NULL) {
		return;
	}
	if (vk == VK_SNAPSHOT) {
		m_pMgrDraw->SaveScreenShot ();
	}
}


/* ========================================================================= */
/* �֐���	:OnLButtonDown													 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2007/03/17														 */
/* ========================================================================= */

void CMainFrame::OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnLButtonDown (x, y);
}


/* ========================================================================= */
/* �֐���	:OnRButtonDown													 */
/* ���e		:���b�Z�[�W�n���h��(WM_RBUTTONDOWN)								 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CMainFrame::OnRButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnRButtonDown (x, y);
}


/* ========================================================================= */
/* �֐���	:OnRButtonDblClk												 */
/* ���e		:���b�Z�[�W�n���h��(WM_RBUTTONDBLCLK)							 */
/* ���t		:2007/09/16														 */
/* ========================================================================= */

void CMainFrame::OnRButtonDblClk(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnRButtonDblClk (x, y);
}


/* ========================================================================= */
/* �֐���	:OnMouseMove													 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEMOVE)								 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CMainFrame::OnMouseMove(HWND hWnd, int x, int y, UINT keyFlags)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnMouseMove (x, y);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnMgrDraw											 */
/* ���e		:���b�Z�[�W�n���h��(WM_MGTDRAW)									 */
/* ���t		:2007/02/27														 */
/* ========================================================================= */

void CMainFrame::OnMgrDraw(int nCode, DWORD dwPara)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnMgrDraw (nCode, dwPara);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnWindowMsg										 */
/* ���e		:���b�Z�[�W�n���h��(WM_WINDOWMSG)								 */
/* ���t		:2005/06/22														 */
/* ========================================================================= */

void CMainFrame::OnWindowMsg(int nCode, DWORD dwPara)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnWindowMsg (nCode, dwPara);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnAdminMsg											 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

void CMainFrame::OnAdminMsg(int nCode, DWORD dwPara)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnAdminMsg (nCode, dwPara);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnMainFrame										 */
/* ���e		:���b�Z�[�W�n���h��(WM_MAINFRAME)								 */
/* ���t		:2006/11/03														 */
/* ========================================================================= */

void CMainFrame::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	if (m_pStateProc == NULL) {
		return;
	}
	switch (dwCommand) {
	case MAINFRAMEMSG_CHGSTATE:			/* ��ԕύX */
		ChgGameState (dwParam);
		break;
	case MAINFRAMEMSG_CONNECT:			/* �ڑ� */
		Connect ();
		break;
	case MAINFRAMEMSG_RENEWVIEWSET:		/* �\���ݒ�X�V */
		m_nDrawCount = 30;
		if (m_pMgrData->GetOption60Frame () == TRUE) {
			m_nDrawCount = 60;
		}
		m_nFPS = 1000 / m_nDrawCount + 1;
		break;
	default:
		m_pStateProc->OnMainFrame (dwCommand, dwParam);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnConnect											 */
/* ���e		:���b�Z�[�W�n���h��(WM_URARASOCK_CONNECT)						 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::OnConnect(void)
{
	CPacketVERSION_REQ_VERSIONCHECK Packet;

	Packet.Make (VERSIONVAL);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* �֐����F	CMainFrame::OnDisConnect										 */
/* ���e�F	���b�Z�[�W�n���h��(WM_URARASOCK_DISCONNECT)						 */
/* ���t�F	2005/06/12														 */
/* ========================================================================= */

void CMainFrame::OnDisConnect(void)
{
	switch (m_nGameState) {
	case GAMESTATE_LOGIN:		/* ���O�C����� */
		DisConnectProc (DISCONNECTID_CONNECT);
		break;
	case GAMESTATE_MAP:			/* �}�b�v��� */
		m_pMgrData->AddSystemMsg (TRUE, "�T�[�o�[�Ƃ̐ڑ����؂�܂���", RGB (0, 200, 255));
		m_pMgrSound->PlaySound (SOUNDID_W_CHAT);

		SendMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
		FlashMainWindow ();
		ChgGameState (GAMESTATE_DISCONNECT);
		break;
	default:					/* ���̑� */
		ChgGameState (GAMESTATE_DISCONNECT);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::OnRecv												 */
/* ���e		:���b�Z�[�W�n���h��(WM_URARASOCK_RECV)							 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::OnRecv(PBYTE pData)
{
	CPacketBase Packet;

	Packet.Set (pData);

	switch (Packet.m_byCmdMain) {
	case SBOCOMMANDID_MAIN_VERSION:	RecvProcVERSION	(Packet.m_byCmdSub, pData);		break;		/* �o�[�W�����n */
	case SBOCOMMANDID_MAIN_CONNECT:	RecvProcCONNECT	(Packet.m_byCmdSub, pData);		break;		/* �ڑ��n */
	case SBOCOMMANDID_MAIN_ACCOUNT:	RecvProcACCOUNT	(Packet.m_byCmdSub, pData);		break;		/* �A�J�E���g�n */
	case SBOCOMMANDID_MAIN_CHAR:	RecvProcCHAR	(Packet.m_byCmdSub, pData);		break;		/* �L�����n */
	case SBOCOMMANDID_MAIN_MAP:		RecvProcMAP		(Packet.m_byCmdSub, pData);		break;		/* �}�b�v�n */
	case SBOCOMMANDID_MAIN_ITEM:	RecvProcITEM	(Packet.m_byCmdSub, pData);		break;		/* �A�C�e���n */
	case SBOCOMMANDID_MAIN_ADMIN:	RecvProcADMIN	(Packet.m_byCmdSub, pData);		break;		/* �Ǘ��Ҍn */
	case SBOCOMMANDID_MAIN_EFFECT:	RecvProcEFFECT	(Packet.m_byCmdSub, pData);		break;		/* �G�t�F�N�g�n */
	case SBOCOMMANDID_MAIN_SYSTEM:	RecvProcSYSTEM	(Packet.m_byCmdSub, pData);		break;		/* �V�X�e���n */
	}

	m_pSock->DeleteRecvData (pData);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::TimerProc											 */
/* ���e		:���ԏ���														 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

BOOL CMainFrame::TimerProc(void)
{
	BOOL bRet, bDraw, bResult;
	PCInfoCharCli pPlayerChar;

	bRet = FALSE;
	bDraw = FALSE;
	bDraw |= m_pMgrDraw->			TimerProc ();
	bDraw |= m_pMgrLayer->			TimerProc ();
	bDraw |= m_pMgrWindow->			TimerProc ();
	bDraw |= m_pLibInfoChar->		Proc ();
	bDraw |= m_pLibInfoMapParts->	Proc ();
	bDraw |= m_pLibInfoMapShadow->	Proc ();

	if (m_pStateProc) {
		bDraw |= m_pStateProc->TimerProc ();
	}

	if (m_bRenewCharInfo) {
		m_bRenewCharInfo = FALSE;
		pPlayerChar = m_pMgrData->GetPlayerChar ();
		if (pPlayerChar) {
			/* �d�Ȃ蒲�� */
			m_pLibInfoChar->SortY ();

			bResult = m_pLibInfoChar->DeleteOutScreen (pPlayerChar);
			if (bResult) {
				PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARCOUNT, m_pLibInfoChar->GetCount ());
			}
		}
	}

	MsgWaitForMultipleObjects (0, NULL, FALSE, 1, QS_ALLINPUT);

	bRet = bDraw;
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CMainFrame::KeyProc											 */
/* ���e		:�L�[���͏���													 */
/* ���t		:2007/03/19														 */
/* ========================================================================= */

void CMainFrame::KeyProc(void)
{
	int i;
	BOOL bResult, bDown;
	BYTE byCode, byCodeTmp, abyCode[] = {VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, 0};

	if (m_bWindowActive == FALSE) {
		return;
	}

	/* �E�B���h�E�ŕK�v�ł���΃E�B���h�E�A�����łȂ���Ώ�ԏ����̃L�[�������s�� */
	m_pMgrKeyInput->Renew (byCode, bDown);
	if (byCode == 0) {
		for (i = 0; ; i ++) {
			byCodeTmp = abyCode[i];
			if (byCodeTmp == 0) {
				break;
			}
			bResult = m_pMgrKeyInput->IsInput (byCodeTmp);
			if (bResult) {
				byCode	= byCodeTmp;
				bDown	= TRUE;
				break;
			}
		}
	}
	if ((byCode == 0) && (bDown == FALSE)) {
		return;
	}
	bResult = m_pMgrWindow->IsKeyInput ();
	if (bResult) {
		m_pMgrWindow->KeyProc (byCode, bDown);

	} else {
		if (m_pStateProc) {
			m_pStateProc->KeyProc (byCode, bDown);
		}
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::ChgGameState										 */
/* ���e		:�Q�[����Ԃ�؂�ւ���											 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CMainFrame::ChgGameState(int nGameState)
{
	m_pMgrLayer->	DeleteAll ();
	m_pMgrWindow->	DeleteAll ();
	SAFE_DELETE (m_pStateProc);

	m_nGameState = nGameState;

	switch (nGameState) {
	case GAMESTATE_LOGO:		m_pStateProc = new CStateProcLOGO;			break;	/* URARA-works���S */
	case GAMESTATE_LOGIN:		m_pStateProc = new CStateProcLOGIN;			break;	/* ���O�C����� */
	case GAMESTATE_DISCONNECT:	m_pStateProc = new CStateProcDISCONNECT;	break;	/* �ؒf */
	case GAMESTATE_INFO:		m_pStateProc = new CStateProcINFO;			break;	/* ���m�点��� */
	case GAMESTATE_LOGINMENU:	m_pStateProc = new CStateProcLOGINMENU;		break;	/* ���j���[��� */
	case GAMESTATE_MAP:			m_pStateProc = new CStateProcMAP;			break;	/* �}�b�v��� */
	}

	if (m_pStateProc) {
		m_pStateProc->Create (m_pMgrData, m_pSock);
		m_pStateProc->Init ();
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::Connect											 */
/* ���e		:�T�[�o�[�֐ڑ�													 */
/* ���t		:2005/06/09														 */
/* ========================================================================= */

void CMainFrame::Connect(void)
{
	WORD wPort;
	CmyString strTmp;

	strTmp	= m_pMgrData->GetServerAddr ();
	wPort	= m_pMgrData->GetServerPort ();

	m_pMgrWindow->MakeWindowMSG ("�T�[�o�[�ɐڑ����Ă��܂�");
	m_pSock->Connect (m_hWnd, URARASOCK_MSGBASE, URARASOCK_PRECHECK, wPort, strTmp);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::FlashMainWindow									 */
/* ���e		:�E�B���h�E��_�ł�����											 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

void CMainFrame::FlashMainWindow(void)
{
	UINT uState;
	FLASHWINFO stFwi;
	APPBARDATA stAbd;

	if (m_bWindowActive) {
		return;
	}
	ZeroMemory (&stAbd, sizeof (stAbd));
	stAbd.cbSize = sizeof (stAbd);
	uState = SHAppBarMessage (ABM_GETSTATE, &stAbd);
	if (uState & ABS_AUTOHIDE) {
		return;
	}
	if (m_pMgrData->GetOptionTaskbar () == FALSE) {
		return;
	}

	ZeroMemory (&stFwi, sizeof (stFwi));
	stFwi.cbSize	= sizeof (stFwi);
	stFwi.hwnd		= m_hWnd;
	stFwi.dwFlags	= FLASHW_TRAY;
	stFwi.uCount	= 4;
	stFwi.dwTimeout	= 300;
	FlashWindowEx (&stFwi);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::GetMsgCmdType										 */
/* ���e		:�`���b�g���͏���												 */
/* ���t		:2007/05/02														 */
/* ========================================================================= */

int CMainFrame::GetMsgCmdType(LPCSTR pszText)
{
	int nRet, nCount;
	LPCSTR pszTmp;
	CParamUtil ParamUtil;

	nRet = MSGCMDTYPE_NONE;

	if (pszText[0] == '#') {
		return MSGCMDTYPE_BALLOON;
	}

	if (pszText[0] == '$') {
		return MSGCMDTYPE_EFFECT;
	}

	if (pszText[0] != '/') {
		goto Exit;
	}

	ParamUtil.Set (pszText);
	nCount	= ParamUtil.GetCount ();
	pszTmp	= ParamUtil.GetParam (0);

	/* �\��ύX */
	if (strcmp (&pszTmp[1], "face") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGFACE;
		}

	/* ���ύX */
	} else if (strcmp (&pszTmp[1], "hair") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGHAIR;
		}

	/* �����ύX */
	} else if (strcmp (&pszTmp[1], "cloth") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGCLOTH;
		}

	/* �A�N�Z�T���ύX */
	} else if (strcmp (&pszTmp[1], "acce") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGACCE;
		}

	/* BGM���ʐݒ� */
	} else if (strcmp (&pszTmp[1], "bgm") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_BGMVOLUME;
		}

	/* �F�ύX */
	} else if (strcmp (&pszTmp[1], "color") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGCOLOR;
		}

	/* ���ʉ��ʐݒ� */
	} else if (strcmp (&pszTmp[1], "se") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_SEVOLUME;
		}

	/* �������ύX */
	} else if (strcmp (&pszTmp[1], "arms") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGARMS;
		}

	/* ���ύX */
	} else if (strcmp (&pszTmp[1], "shield") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGSHIELD;
		}

	/* �z�u�A�C�e���ύX */
	} else if (strcmp (&pszTmp[1], "item") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_SETITEM;
		}

	/* �A�C�e���쐬 */
	} else if (strcmp (&pszTmp[1], "makeitem") == 0) {
		if (m_pMgrData->GetAdminLevel () > ADMINLEVEL_NONE) {
			if (nCount == 2) {
				nRet = MSGCMDTYPE_MAKEITEM;
			}
		}

	/* ���o�� */
//	} else if (strcmp (&pszTmp[1], "balloon") == 0) {
//		if (nCount == 2) {
//			nRet = MSGCMDTYPE_BALLOON;
//		}

	/* �T�C�R�� */
	} else if (strcmp (&pszTmp[1], "dice") == 0) {
		if (nCount == 1) {
			nRet = MSGCMDTYPE_DICE;
		}

	/* �����_�� */
	} else if (strcmp (&pszTmp[1], "rnd") == 0) {
		if (nCount <= 2) {
			nRet = MSGCMDTYPE_RND;
		}

	/* ���ݎ��� */
	} else if (strcmp (&pszTmp[1], "now") == 0) {
		if (nCount == 1) {
			nRet = MSGCMDTYPE_NOW;
		}

	/* �ł��W�܂��Ă���ꏊ */
	} else if (strcmp (&pszTmp[1], "where") == 0) {
		if (nCount == 1) {
			nRet = MSGCMDTYPE_WHERE;
		}
	}

Exit:
	return nRet;
}

/* Copyright(C)URARA-works 2006 */
