/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MgrKeyInput.cpp											 */
/* 内容			:キー入力管理クラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "DInputUtil.h"
#include "MgrKeyInput.h"


/* ========================================================================= */
/* 関数名	:CMgrKeyInput::CMgrKeyInput										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

CMgrKeyInput::CMgrKeyInput()
{
	ZeroMemory (m_abyCode,			sizeof (m_abyCode));
	ZeroMemory (m_abyKeyState,		sizeof (m_abyKeyState));
	ZeroMemory (m_abyKeyStateBack,	sizeof (m_abyKeyStateBack));

	m_pDInputUtil = new CDInputUtil;
}


/* ========================================================================= */
/* 関数名	:CMgrKeyInput::~CMgrKeyInput									 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

CMgrKeyInput::~CMgrKeyInput()
{
	Destroy ();
	SAFE_DELETE (m_pDInputUtil);
}


/* ========================================================================= */
/* 関数名	:CMgrKeyInput::Create											 */
/* 内容		:作成															 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CMgrKeyInput::Create(void)
{
	int i, aKeyTbl[] = {
		'Z',			/* Z */
		'X',			/* X */
		'S',			/* S */
		'A',			/* A */
		'R',			/* R */
		'I',			/* I */
		'B',			/* B */
		'F',			/* F */
		'G',			/* G */
		'H',			/* H */
		'L',			/* L */
		'C',			/* C */
		'P',			/* P */
		'M',			/* M */
		'N',			/* N */
		VK_OEM_3,		/* @ */
		VK_RETURN,		/* Enter */
		VK_ESCAPE,		/* Escape */
		VK_TAB,			/* Tab */
		VK_UP,			/* ↑ */
		VK_DOWN,		/* ↓ */
		VK_LEFT,		/* ← */
		VK_RIGHT,		/* → */
		VK_SHIFT,		/* Shift */
		VK_CONTROL,		/* Ctrl */
		VK_SPACE,		/* Space */
		-1
	};
	ZeroMemory (m_abyCode, sizeof (m_abyCode));

	for (i = 0; ; i ++) {
		if (aKeyTbl[i] == -1) {
			break;
		}
		m_abyCode[i] = aKeyTbl[i];
	}

	m_pDInputUtil->Create ();
}


/* ========================================================================= */
/* 関数名	:CMgrKeyInput::Destroy											 */
/* 内容		:破棄															 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CMgrKeyInput::Destroy(void)
{
}


/* ========================================================================= */
/* 関数名	:CMgrKeyInput::Renew											 */
/* 内容		:状態更新														 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CMgrKeyInput::Renew(BYTE &byCode, BOOL &bDown)
{
	int i;
	BOOL bBreak;
	BYTE byCodeTmp;
	DWORD dwKeyInput;

//	GetKeyboardState (m_abyKeyState);

	byCode	= 0;
	bDown	= FALSE;

	/* 入力状態の検出 */
	dwKeyInput = m_pDInputUtil->GetKeyState ();
	for (i = 0; ; i ++) {
		byCodeTmp = m_abyCode[i];
		if (byCodeTmp == 0) {
			break;
		}
		bBreak = TRUE;
		m_abyKeyState[byCodeTmp] = (BYTE)GetKeyState (byCodeTmp);
		switch (byCodeTmp) {
		case VK_UP:			/* ↑ */
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_UP) ? 0x80 : 0;
			bBreak = FALSE;
			break;
		case VK_DOWN:		/* ↓ */
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_DOWN) ? 0x80 : 0;
			bBreak = FALSE;
			break;
		case VK_LEFT:		/* ← */
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_LEFT) ? 0x80 : 0;
			bBreak = FALSE;
			break;
		case VK_RIGHT:		/* → */
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_RIGHT) ? 0x80 : 0;
			bBreak = FALSE;
			break;
		case 'X':			/* X */
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_BUTTON1) ? 0x80 : 0;
			break;
		case 'Z':			/* Z */
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_BUTTON2) ? 0x80 : 0;
			break;
		}
		if ((m_abyKeyState[byCodeTmp] & 0x80) && (m_abyKeyStateBack[byCodeTmp] == 0)) {
			m_abyKeyStateBack[byCodeTmp] = 1;
			byCode	= byCodeTmp;
			bDown	= TRUE;
			if (bBreak) {
				break;
			}

		} else if (((m_abyKeyState[byCodeTmp] & 0x80) == 0) && (m_abyKeyStateBack[byCodeTmp] == 1)) {
			m_abyKeyStateBack[byCodeTmp] = 0;
			byCode	= byCodeTmp;
			bDown	= FALSE;
			if (bBreak) {
				break;
			}
		}
	}
}


/* ========================================================================= */
/* 関数名	:CMgrKeyInput::Reset											 */
/* 内容		:初期化															 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CMgrKeyInput::Reset(void)
{
	ZeroMemory (m_abyKeyState,		sizeof (m_abyKeyState));
	ZeroMemory (m_abyKeyStateBack,	sizeof (m_abyKeyStateBack));
}


/* ========================================================================= */
/* 関数名	:CMgrKeyInput::IsInput											 */
/* 内容		:押下判定														 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

BOOL CMgrKeyInput::IsInput(BYTE byCode)
{
	BOOL bRet;

	bRet = FALSE;

	if ((m_abyKeyState[byCode] & 0x80) == 0) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CMgrKeyInput::SetDevice										 */
/* 内容		:使用デバイスを設定												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CMgrKeyInput::SetDevice(int nNo, HWND hWnd)
{
	m_pDInputUtil->SetDevice (nNo, hWnd);
}


/* ========================================================================= */
/* 関数名	:CMgrKeyInput::SetDevice										 */
/* 内容		:使用デバイスを設定												 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

void CMgrKeyInput::SetDevice(GUID &stSrc, HWND hWnd)
{
	m_pDInputUtil->SetDevice (stSrc, hWnd);
}


/* ========================================================================= */
/* 関数名	:CMgrKeyInput::Enum												 */
/* 内容		:ジョイパッドを列挙												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CMgrKeyInput::Enum(void)
{
	m_pDInputUtil->Enum ();
}


/* ========================================================================= */
/* 関数名	:CMgrKeyInput::GetDeviceCount									 */
/* 内容		:デバイス数を取得												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

int CMgrKeyInput::GetDeviceCount(void)
{
	return m_pDInputUtil->GetDeviceCount ();
}


/* ========================================================================= */
/* 関数名	:CMgrKeyInput::GetDeviceName									 */
/* 内容		:デバイス名を取得												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

BOOL CMgrKeyInput::GetDeviceName(int nNo, CmyString &strDst)
{
	return m_pDInputUtil->GetDeviceName (nNo, strDst);
}


/* ========================================================================= */
/* 関数名	:CMgrKeyInput::GetGUID											 */
/* 内容		:指定デバイスのGUIDを取得										 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

BOOL CMgrKeyInput::GetGUID(
	int nNo,			/* [in] 配列番号 */
	GUID &stDst)		/* [out] 取得先 */
{
	return m_pDInputUtil->GetGUID (nNo, stDst);
}

/* Copyright(C)URARA-works 2006 */
