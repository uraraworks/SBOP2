/// @file DInputUtil.cpp
/// @brief ジョイスティックユーティリティクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/30
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "DInputUtil.h"

#include <SDL.h>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)	{ if(p)	{ delete p;p=NULL; } }
#endif

namespace {

// GUID の等値比較
BOOL IsGuidEqual(const GUID &stLeft, const GUID &stRight)
{
	return (memcmp(&stLeft, &stRight, sizeof(GUID)) == 0) ? TRUE : FALSE;
}

// SDL_JoystickGUID を GUID 型に変換する
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

// SDL から取得した UTF-8 文字列を TCHAR 配列にコピーする
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

} // namespace

CDInputUtil::CDInputUtil()
{
	m_pJoystick = NULL;
}

CDInputUtil::~CDInputUtil()
{
	Destroy();
}

BOOL CDInputUtil::Create(void)
{
	// SDL ジョイスティックサブシステムが初期化済みかを確認する
	return (SDL_WasInit(SDL_INIT_JOYSTICK) != 0) ? TRUE : FALSE;
}

void CDInputUtil::Destroy(void)
{
	DeleteAllDeviceInfo();

	if (m_pJoystick != NULL) {
		SDL_JoystickClose(m_pJoystick);
	}
	m_pJoystick = NULL;
}

BOOL CDInputUtil::IsUseDevice(void)
{
	BOOL bRet;

	bRet = FALSE;
	if (m_pJoystick == NULL) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}

DWORD CDInputUtil::GetKeyState(void)
{
	const Sint16 nThreshold = 16000;
	DWORD dwRet;

	dwRet = 0;
	if (m_pJoystick == NULL) {
		return 0;
	}

	SDL_JoystickUpdate();

	if (SDL_JoystickGetAxis(m_pJoystick, 1) <= -nThreshold) {
		dwRet |= BUTTON_UP;
	}
	if (SDL_JoystickGetAxis(m_pJoystick, 1) >= nThreshold) {
		dwRet |= BUTTON_DOWN;
	}
	if (SDL_JoystickGetAxis(m_pJoystick, 0) <= -nThreshold) {
		dwRet |= BUTTON_LEFT;
	}
	if (SDL_JoystickGetAxis(m_pJoystick, 0) >= nThreshold) {
		dwRet |= BUTTON_RIGHT;
	}
	if (SDL_JoystickGetButton(m_pJoystick, 0)) {
		dwRet |= BUTTON_BUTTON1;
	}
	if (SDL_JoystickGetButton(m_pJoystick, 1)) {
		dwRet |= BUTTON_BUTTON1;
	}
	if (SDL_JoystickGetButton(m_pJoystick, 2)) {
		dwRet |= BUTTON_BUTTON2;
	}
	if (SDL_JoystickGetButton(m_pJoystick, 3)) {
		dwRet |= BUTTON_BUTTON2;
	}

	return dwRet;
}

void CDInputUtil::SetDevice(int nNo)
{
	GUID stGuid;

	ZeroMemory(&stGuid, sizeof(stGuid));
	if ((nNo < 0) || (nNo >= static_cast<int>(m_aDeviceInfo.size()))) {
		// 無効番号のときはデバイスを解放する
		if (m_pJoystick != NULL) {
			SDL_JoystickClose(m_pJoystick);
			m_pJoystick = NULL;
		}
		return;
	}

	stGuid = m_aDeviceInfo[nNo]->guidInstance;
	SetDevice(stGuid);
}

void CDInputUtil::SetDevice(GUID &stSrc)
{
	int i, nCount;
	SDL_Joystick *pJoystick;

	// 現在開いているデバイスを先に閉じる
	if (m_pJoystick != NULL) {
		SDL_JoystickClose(m_pJoystick);
		m_pJoystick = NULL;
	}

	// GUID が一致するジョイスティックを探してオープンする
	nCount = SDL_NumJoysticks();
	for (i = 0; i < nCount; i++) {
		if (IsGuidEqual(ConvertSDLGuid(SDL_JoystickGetDeviceGUID(i)), stSrc) == FALSE) {
			continue;
		}

		pJoystick = SDL_JoystickOpen(i);
		if (pJoystick == NULL) {
			continue;
		}

		m_pJoystick = pJoystick;
		break;
	}
}

BOOL CDInputUtil::Enum(void)
{
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
