/// @file DInputUtil.cpp
/// @brief DirectInputユーティリティクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/30
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "DInputUtil.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)	{ if(p)	{ (p)->Release();(p)=NULL; } }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)	{ if(p)	{ delete p;p=NULL; } }
#endif

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

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
}

void CDInputUtil::Destroy(void)
{
	DeleteAllDeviceInfo();
	SAFE_RELEASE(m_pDInput);
	SAFE_RELEASE(m_pDevice);
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
}

void CDInputUtil::SetDevice(int nNo, HWND hWnd)
{
	PDINPUTDEVICEINFO pDeviceInfo;

	if (nNo < 0) {
		SAFE_RELEASE(m_pDevice);
		return;
	}

	pDeviceInfo = m_aDeviceInfo[nNo];

	SetDevice(pDeviceInfo->guidInstance, hWnd);
}

void CDInputUtil::SetDevice(GUID &stSrc, HWND hWnd)
{
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
}

BOOL CDInputUtil::Enum(void)
{
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
}

int CDInputUtil::GetDeviceCount(void)
{
	return m_aDeviceInfo.size();
}

BOOL CDInputUtil::GetDeviceName(
	int nNo,			// [in] 配列番号
	CmyString &strDst)	// [out] デバイス名
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
	int nNo,		// [in] 配列番号
	GUID &stDst)	// [out] 取得先
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
	PDINPUTDEVICEINFO pInfo;
	PCDInputUtil pThis;

	pThis = (PCDInputUtil)pvRef;

	// デバイス情報を追加
	pInfo = new DINPUTDEVICEINFO;
	pInfo->guidInstance = lpddi->guidInstance;
	pInfo->strName = lpddi->tszInstanceName;
	pThis->AddDeviceInfo(pInfo);

	return DIENUM_CONTINUE;
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
