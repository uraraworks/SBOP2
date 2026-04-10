/// @file MgrKeyInput.cpp
/// @brief キー入力管理クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/10/01
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "DInputUtil.h"
#include "MgrKeyInput.h"
#include "Platform/SDLInput.h"
#include "imgui.h"


CMgrKeyInput::CMgrKeyInput()
{
	ZeroMemory(m_abyCode,			sizeof(m_abyCode));
	ZeroMemory(m_abyKeyState,		sizeof(m_abyKeyState));
	ZeroMemory(m_abyKeyStateBack,	sizeof(m_abyKeyStateBack));

	m_pDInputUtil = new CDInputUtil;
}


CMgrKeyInput::~CMgrKeyInput()
{
	Destroy();
	SAFE_DELETE(m_pDInputUtil);
}


void CMgrKeyInput::Create(void)
{
	int i, aKeyTbl[] = {
		'Z',			// Z
		'X',			// X
		'S',			// S
		'A',			// A
		'R',			// R
		'I',			// I
		'B',			// B
		'F',			// F
		'G',			// G
		'H',			// H
		'J',			// J
		'K',			// K
		'L',			// L
		'C',			// C
		'P',			// P
		'M',			// M
		'N',			// N
		'V',			// V
		VK_OEM_3,		// @
		VK_RETURN,		// Enter
		VK_ESCAPE,		// Escape
		VK_TAB,			// Tab
		VK_UP,			// ↑
		VK_DOWN,		// ↓
		VK_LEFT,		// ←
		VK_RIGHT,		// →
		VK_SHIFT,		// Shift
		VK_CONTROL,		// Ctrl
		VK_SPACE,		// Space
		-1
	};
	ZeroMemory(m_abyCode, sizeof(m_abyCode));

	for (i = 0; ; i++) {
		if (aKeyTbl[i] == -1) {
			break;
		}
		m_abyCode[i] = aKeyTbl[i];
	}

	m_pDInputUtil->Create();
}


void CMgrKeyInput::Destroy(void)
{
}


void CMgrKeyInput::Renew(BYTE &byCode, BOOL &bDown)
{
	int i;
	BOOL bBreak;
	BYTE byCodeTmp;
	DWORD dwKeyInput;
	const Uint8 *pKeyboardState;

	byCode	= 0;
	bDown	= FALSE;

	// ImGui がキーボードをキャプチャ中はゲーム側のポーリングをスキップ
	// （メッセージログ入力欄や ImGui 系 UI にフォーカスがある場合、
	//   SDL_GetKeyboardState で直接取得したキー状態がゲームに流れないようにする）
	if (ImGui::GetCurrentContext() != NULL && ImGui::GetIO().WantCaptureKeyboard) {
		// 内部状態を「全キー離上」にリセットしておく
		// → ImGui キャプチャ解除の瞬間に古い押下エッジが誤検出されないようにするため
		ZeroMemory(m_abyKeyState,     sizeof(m_abyKeyState));
		ZeroMemory(m_abyKeyStateBack, sizeof(m_abyKeyStateBack));
		return;
	}

	// SDL キーボード状態を取得（SDLApp::Run() の SDL_PollEvent 後に更新済み）
	pKeyboardState = SDL_GetKeyboardState(NULL);

	// 入力状態の検出
	dwKeyInput = m_pDInputUtil->GetKeyState();
	for (i = 0; ; i++) {
		byCodeTmp = m_abyCode[i];
		if (byCodeTmp == 0) {
			break;
		}
		bBreak = TRUE;

		// キーボードは SDL 状態を主系とし、ジョイパッドだけ DirectInput を併用する
		m_abyKeyState[byCodeTmp] = CSDLInput::IsVKPressed(byCodeTmp, pKeyboardState) ? 0x80 : 0;
		switch (byCodeTmp) {
		case VK_UP:		// ↑
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_UP) ? 0x80 : 0;
			bBreak = FALSE;
			break;
		case VK_DOWN:	// ↓
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_DOWN) ? 0x80 : 0;
			bBreak = FALSE;
			break;
		case VK_LEFT:	// ←
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_LEFT) ? 0x80 : 0;
			bBreak = FALSE;
			break;
		case VK_RIGHT:	// →
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_RIGHT) ? 0x80 : 0;
			bBreak = FALSE;
			break;
		case 'X':		// X
			m_abyKeyState[byCodeTmp] |= (dwKeyInput & BUTTON_BUTTON1) ? 0x80 : 0;
			break;
		case 'Z':		// Z
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


void CMgrKeyInput::Reset(void)
{
	ZeroMemory(m_abyKeyState,     sizeof(m_abyKeyState));
	ZeroMemory(m_abyKeyStateBack, sizeof(m_abyKeyStateBack));

	// 現在押下中のキーを「押下中エッジ済み」状態として記録しておく
	// → 次フレームの Renew() で「変化なし」と判定され、誤って KEYDOWN エッジが
	//   再検出されないようにする（離上の瞬間だけ正しく KEYUP エッジを発生させる）
	const Uint8 *pKeyboardState = SDL_GetKeyboardState(NULL);
	for (int i = 0; ; i++) {
		BYTE byCodeTmp = m_abyCode[i];
		if (byCodeTmp == 0) {
			break;
		}
		if (CSDLInput::IsVKPressed(byCodeTmp, pKeyboardState)) {
			m_abyKeyState[byCodeTmp]     = 0x80;
			m_abyKeyStateBack[byCodeTmp] = 1;
		}
	}
}


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


void CMgrKeyInput::SetDevice(int nNo)
{
	m_pDInputUtil->SetDevice(nNo);
}


void CMgrKeyInput::SetDevice(GUID &stSrc)
{
	m_pDInputUtil->SetDevice(stSrc);
}


void CMgrKeyInput::Enum(void)
{
	m_pDInputUtil->Enum();
}


int CMgrKeyInput::GetDeviceCount(void)
{
	return m_pDInputUtil->GetDeviceCount();
}


BOOL CMgrKeyInput::GetDeviceName(int nNo, CmyString &strDst)
{
	return m_pDInputUtil->GetDeviceName(nNo, strDst);
}


BOOL CMgrKeyInput::GetGUID(
	int nNo,		// [in] 配列番号
	GUID &stDst)	// [out] 取得先
{
	return m_pDInputUtil->GetGUID(nNo, stDst);
}
