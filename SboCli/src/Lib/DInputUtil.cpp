/// @file DInputUtil.cpp
/// @brief DirectInputユーティリティクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/30
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "DInputUtil.h"

#include <SDL.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)	{ if(p)	{ (p)->Release();(p)=NULL; } }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)	{ if(p)	{ delete p;p=NULL; } }
#endif

#if defined(_WIN32) && !defined(__EMSCRIPTEN__)
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#endif

namespace {

#if !defined(_WIN32) || defined(__EMSCRIPTEN__)
typedef struct _DIDEVICEINSTANCE_STUB {
	GUID guidInstance;
	TCHAR tszInstanceName[MAX_PATH];
} DIDEVICEINSTANCE_STUB;
#endif

BOOL IsGuidEqual(const GUID &stLeft, const GUID &stRight)
{
	return (memcmp(&stLeft, &stRight, sizeof(GUID)) == 0) ? TRUE : FALSE;
}

#if !defined(_WIN32) || defined(__EMSCRIPTEN__)
GUID ConvertSDLGuid(const SDL_JoystickGUID &stSrc)
{
	GUID stDst;
	const Uint8 *pSrc;

	ZeroMemory(&stDst, sizeof(stDst));
	pSrc = (const Uint8 *)&stSrc;
	if (pSrc == NULL) {
		return stDst;
	}

	stDst.Data1 =
		((DWORD)pSrc[0]) |
		((DWORD)pSrc[1] << 8) |
		((DWORD)pSrc[2] << 16) |
		((DWORD)pSrc[3] << 24);
	stDst.Data2 = (WORD)(((WORD)pSrc[4]) | ((WORD)pSrc[5] << 8));
	stDst.Data3 = (WORD)(((WORD)pSrc[6]) | ((WORD)pSrc[7] << 8));
	CopyMemory(stDst.Data4, pSrc + 8, sizeof(stDst.Data4));

	return stDst;
}

BOOL CopyDeviceName(TCHAR *pszDst, size_t nDstCount, const char *pszSrc)
{
	if ((pszDst == NULL) || (nDstCount == 0)) {
		return FALSE;
	}

	pszDst[0] = _T('\0');
	if (pszSrc == NULL) {
		return TRUE;
	}

	MultiByteToWideChar(CP_UTF8, 0, pszSrc, -1, pszDst, (int)nDstCount);
	return TRUE;
}
#endif

} // namespace

CDInputUtil::CDInputUtil()
{
	m_pDInput = NULL;
	m_pDevice = NULL;
}

CDInputUtil::~CDInputUtil()
{
	Destroy();
}

BOOL CDInputUtil::Create(void)
{
#if defined(_WIN32) && !defined(__EMSCRIPTEN__)
	BOOL bRet;
	HRESULT hResult;

	bRet = FALSE;

	hResult = DirectInput8Create(
					GetModuleHandle(NULL),
					DIRECTINPUT_VERSION,
					IID_IDirectInput8,
					(PVOID *)&m_pDInput,
					NULL);
	if (FAILED(hResult)) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
#else
	return (SDL_WasInit(SDL_INIT_JOYSTICK) != 0) ? TRUE : FALSE;
#endif
}

void CDInputUtil::Destroy(void)
{
	DeleteAllDeviceInfo();

#if defined(_WIN32) && !defined(__EMSCRIPTEN__)
	SAFE_RELEASE(m_pDInput);
	SAFE_RELEASE(m_pDevice);
#else
	if (m_pDevice != NULL) {
		SDL_JoystickClose((SDL_Joystick *)m_pDevice);
	}
	m_pDInput = NULL;
	m_pDevice = NULL;
#endif
}

BOOL CDInputUtil::IsUseDevice(void)
{
	BOOL bRet;

	bRet = FALSE;
	if (m_pDevice == NULL) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}

DWORD CDInputUtil::GetKeyState(void)
{
#if defined(_WIN32) && !defined(__EMSCRIPTEN__)
	DWORD dwRet;
	HRESULT hr;
	DIJOYSTATE dijs;

	dwRet = 0;
	if (m_pDevice == NULL) {
		goto Exit;
	}

	// ジョイスティックにポールをかける
	m_pDevice->Poll();

	// デバイス状態を読み取る
	hr = m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &dijs);
	if (hr != DI_OK){
		m_pDevice->Acquire();
		goto Exit;
	}

	dwRet = 0;
	if (dijs.lY < 0) {
		dwRet |= BUTTON_UP;
	}
	if (dijs.lY > 0) {
		dwRet |= BUTTON_DOWN;
	}
	if (dijs.lX < 0) {
		dwRet |= BUTTON_LEFT;
	}
	if (dijs.lX > 0) {
		dwRet |= BUTTON_RIGHT;
	}
	if (dijs.rgbButtons[0] & 0x80) {
		dwRet |= BUTTON_BUTTON1;
	}
	if (dijs.rgbButtons[1] & 0x80) {
		dwRet |= BUTTON_BUTTON1;
	}
	if (dijs.rgbButtons[2] & 0x80) {
		dwRet |= BUTTON_BUTTON2;
	}
	if (dijs.rgbButtons[3] & 0x80) {
		dwRet |= BUTTON_BUTTON2;
	}

Exit:
	return dwRet;
#else
	SDL_Joystick *pJoystick;
	const Sint16 nThreshold = 16000;
	DWORD dwRet;

	dwRet = 0;
	pJoystick = (SDL_Joystick *)m_pDevice;
	if (pJoystick == NULL) {
		return 0;
	}

	SDL_JoystickUpdate();

	if (SDL_JoystickGetAxis(pJoystick, 1) <= -nThreshold) {
		dwRet |= BUTTON_UP;
	}
	if (SDL_JoystickGetAxis(pJoystick, 1) >= nThreshold) {
		dwRet |= BUTTON_DOWN;
	}
	if (SDL_JoystickGetAxis(pJoystick, 0) <= -nThreshold) {
		dwRet |= BUTTON_LEFT;
	}
	if (SDL_JoystickGetAxis(pJoystick, 0) >= nThreshold) {
		dwRet |= BUTTON_RIGHT;
	}
	if (SDL_JoystickGetButton(pJoystick, 0)) {
		dwRet |= BUTTON_BUTTON1;
	}
	if (SDL_JoystickGetButton(pJoystick, 1)) {
		dwRet |= BUTTON_BUTTON1;
	}
	if (SDL_JoystickGetButton(pJoystick, 2)) {
		dwRet |= BUTTON_BUTTON2;
	}
	if (SDL_JoystickGetButton(pJoystick, 3)) {
		dwRet |= BUTTON_BUTTON2;
	}

	return dwRet;
#endif
}

void CDInputUtil::SetDevice(int nNo, HWND hWnd)
{
#if defined(_WIN32) && !defined(__EMSCRIPTEN__)
	PDINPUTDEVICEINFO pDeviceInfo;

	if (nNo < 0) {
		SAFE_RELEASE(m_pDevice);
		return;
	}

	pDeviceInfo = m_aDeviceInfo[nNo];

	SetDevice(pDeviceInfo->guidInstance, hWnd);
#else
	GUID stGuid;

	UNREFERENCED_PARAMETER(hWnd);

	ZeroMemory(&stGuid, sizeof(stGuid));
	if ((nNo < 0) || (nNo >= static_cast<int>(m_aDeviceInfo.size()))) {
		if (m_pDevice != NULL) {
			SDL_JoystickClose((SDL_Joystick *)m_pDevice);
			m_pDevice = NULL;
		}
		return;
	}

	stGuid = m_aDeviceInfo[nNo]->guidInstance;
	SetDevice(stGuid, NULL);
#endif
}

void CDInputUtil::SetDevice(GUID &stSrc, HWND hWnd)
{
#if defined(_WIN32) && !defined(__EMSCRIPTEN__)
	HRESULT hr;

	SAFE_RELEASE(m_pDevice);
	hr = m_pDInput->CreateDevice(
			stSrc,
			&m_pDevice, NULL);
	if (FAILED(hr)) {
		SAFE_RELEASE(m_pDevice);
		return;
	}

	// ジョイスティック用のデータ・フォーマットを設定
	hr = m_pDevice->SetDataFormat(&c_dfDIJoystick);
	if (FAILED(hr)) {
		SAFE_RELEASE(m_pDevice);
		return;
	}

	// モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) {
		SAFE_RELEASE(m_pDevice);
		return;
	}

	// 軸の値の範囲を設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = -1000;
	diprg.lMax = 1000;

	// X軸の範囲を設定
	diprg.diph.dwObj = DIJOFS_X;
	hr = m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr)) {
		SAFE_RELEASE(m_pDevice);
		return;
	}

	// Y軸の範囲を設定
	diprg.diph.dwObj = DIJOFS_Y;
	hr = m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr)) {
		SAFE_RELEASE(m_pDevice);
		return;
	}

	// 各軸ごとに、無効のゾーン値を設定する。
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwHow = DIPH_BYOFFSET;
	dipdw.dwData = 2500;

	// X軸の無効ゾーンを設定
	dipdw.diph.dwObj = DIJOFS_X;
	hr = m_pDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	if (FAILED(hr)) {
		SAFE_RELEASE(m_pDevice);
		return;
	}

	// Y軸の無効ゾーンを設定
	dipdw.diph.dwObj = DIJOFS_Y;
	hr = m_pDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	if (FAILED(hr)) {
		SAFE_RELEASE(m_pDevice);
		return;
	}

	m_pDevice->Acquire();
#else
	int i, nCount;
	SDL_Joystick *pJoystick;

	UNREFERENCED_PARAMETER(hWnd);

	if (m_pDevice != NULL) {
		SDL_JoystickClose((SDL_Joystick *)m_pDevice);
		m_pDevice = NULL;
	}

	nCount = SDL_NumJoysticks();
	for (i = 0; i < nCount; i++) {
		if (IsGuidEqual(ConvertSDLGuid(SDL_JoystickGetDeviceGUID(i)), stSrc) == FALSE) {
			continue;
		}

		pJoystick = SDL_JoystickOpen(i);
		if (pJoystick == NULL) {
			continue;
		}

		m_pDevice = (LPDIRECTINPUTDEVICE8)pJoystick;
		break;
	}
#endif
}

BOOL CDInputUtil::Enum(void)
{
#if defined(_WIN32) && !defined(__EMSCRIPTEN__)
	BOOL bRet;
	HRESULT hResult;

	bRet = FALSE;
	if (m_pDInput == NULL) {
		goto Exit;
	}

	DeleteAllDeviceInfo();
	hResult = m_pDInput->EnumDevices(
					DI8DEVCLASS_GAMECTRL,
					EnumProc,
					this,
					DIEDFL_ATTACHEDONLY);
	if (FAILED(hResult)) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
#else
	int i, nCount;
	PDINPUTDEVICEINFO pInfo;

	DeleteAllDeviceInfo();

	nCount = SDL_NumJoysticks();
	for (i = 0; i < nCount; i++) {
		pInfo = new DINPUTDEVICEINFO;
		pInfo->guidInstance = ConvertSDLGuid(SDL_JoystickGetDeviceGUID(i));
		pInfo->strName = _T("");

		{
			const char *pszName = SDL_JoystickNameForIndex(i);
			TCHAR szName[MAX_PATH];
			CopyDeviceName(szName, _countof(szName), pszName);
			pInfo->strName = szName;
		}

		AddDeviceInfo(pInfo);
	}

	return (nCount > 0) ? TRUE : FALSE;
#endif
}

int CDInputUtil::GetDeviceCount(void)
{
	return m_aDeviceInfo.size();
}

BOOL CDInputUtil::GetDeviceName(
	int nNo,
	CmyString &strDst)
{
	BOOL bRet;
	PDINPUTDEVICEINFO pInfo;

	bRet = FALSE;
	if (nNo >= m_aDeviceInfo.size()) {
		goto Exit;
	}
	if (nNo < 0) {
		goto Exit;
	}

	pInfo = m_aDeviceInfo[nNo];
	strDst = pInfo->strName;

	bRet = TRUE;
Exit:
	return bRet;
}

BOOL CDInputUtil::GetGUID(
	int nNo,
	GUID &stDst)
{
	BOOL bRet;
	PDINPUTDEVICEINFO pInfo;

	bRet = FALSE;
	if (nNo >= m_aDeviceInfo.size()) {
		goto Exit;
	}
	if (nNo < 0) {
		goto Exit;
	}

	pInfo = m_aDeviceInfo[nNo];
	stDst = pInfo->guidInstance;

	bRet = TRUE;
Exit:
	return bRet;
}

BOOL CALLBACK CDInputUtil::EnumProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
#if defined(_WIN32) && !defined(__EMSCRIPTEN__)
	PDINPUTDEVICEINFO pInfo;
	PCDInputUtil pThis;

	pThis = (PCDInputUtil)pvRef;

	// デバイス情報を追加
	pInfo = new DINPUTDEVICEINFO;
	pInfo->guidInstance = lpddi->guidInstance;
	pInfo->strName = lpddi->tszInstanceName;
	pThis->AddDeviceInfo(pInfo);

	return DIENUM_CONTINUE;
#else
	UNREFERENCED_PARAMETER(lpddi);
	UNREFERENCED_PARAMETER(pvRef);
	return FALSE;
#endif
}

void CDInputUtil::DeleteDeviceInfo(int nNo)
{
	PDINPUTDEVICEINFO pInfo;

	pInfo = m_aDeviceInfo[nNo];
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_aDeviceInfo.size()))) {
		m_aDeviceInfo.erase(m_aDeviceInfo.begin() + nNo);
	}
}

void CDInputUtil::DeleteAllDeviceInfo(void)
{
	int i, nCount;

	nCount = m_aDeviceInfo.size();
	for (i = nCount - 1; i >= 0; i --) {
		DeleteDeviceInfo(i);
	}
}

void CDInputUtil::AddDeviceInfo(PDINPUTDEVICEINFO pDeviceInfo)
{
	m_aDeviceInfo.push_back(pDeviceInfo);
}
