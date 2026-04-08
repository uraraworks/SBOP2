/// @file MgrSound.cpp
/// @brief サウンドデータ管理クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/05/09
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include <SDL.h>			// SDL_LoadObject / SDL_UnloadObject 用
#include "resource.h"
#include "DXAudio.h"
#include "LibSboSoundLoader.h"
#include "MgrSound.h"

// Audiere dependency removed

CMgrSound::CMgrSound()
{
	m_dwSoundID			= 0;
	m_SEVolume			= -7;
	m_fBGMVolume		= 0.50f;
	m_hDllSoundData		= NULL;
	m_pDXAudio			= new CDXAudio;
	m_pLibSboSoundLoader = new CLibSboSoundLoader;
	m_apDMSSound		= NULL;
	// BGM: XAudio2 再生に切替のため未使用
}


CMgrSound::~CMgrSound()
{
	Destroy();

	SAFE_DELETE(m_pDXAudio);
	SAFE_DELETE(m_pLibSboSoundLoader);
	SAFE_DELETE_ARRAY(m_apDMSSound);
}


BOOL CMgrSound::Create(void)
{
	BOOL bRet, bResult;
	// BuildModuleRelativePath は TCHAR* を受け取るため TCHAR 配列を使用
	TCHAR szPath[MAX_PATH];

	bRet = FALSE;

	// TCHAR でパスを構築してから ANSI 文字列に変換（SDL_LoadObject は char* を受け取る）
	BuildModuleRelativePath(szPath, _countof(szPath), _T("SboSoundData.dll"));
	std::string ansiPath = TStringToAnsiStd(szPath);

	bResult = m_pDXAudio->Create();
	if (bResult == FALSE) {
#if !defined(_WIN32)
		bRet = TRUE;
#endif
		goto Exit;
	}

#if !defined(_WIN32)
	// 非Windowsではタイトル表示を優先し、音声資産が無くても継続する
	bRet = TRUE;
	goto Exit;
#endif

	// Win32 LoadLibrary の代わりに SDL_LoadObject でクロスプラットフォーム対応
	m_hDllSoundData = SDL_LoadObject(ansiPath.c_str());
	if (m_hDllSoundData == NULL) {
		// タイトル表示までは無音でも進められるようにする
		bRet = TRUE;
		goto Exit;
	}
	m_pDXAudio->SetResourceHandle(m_hDllSoundData);

	m_pLibSboSoundLoader->Load();

	// 効果音を読み込み
	ReadSoundData();

	bRet = TRUE;
Exit:
	return bRet;
}


void CMgrSound::Destroy(void)
{
	if (m_pDXAudio) {
		m_pDXAudio->Destroy();
	}
	// SDL_LoadObject で読み込んだDLLを解放
	if (m_hDllSoundData) {
		SDL_UnloadObject(m_hDllSoundData);
		m_hDllSoundData = NULL;
	}
}


void CMgrSound::PlaySound(DWORD dwSoundID)
{
	int nNo;
	IDirectMusicSegment8 **pDMS;

#if !defined(_WIN32)
	UNREFERENCED_PARAMETER(dwSoundID);
	return;
#endif

	nNo = m_pLibSboSoundLoader->GetSoundNo(dwSoundID);
	if (nNo < 0) {
		return;
	}
	m_pDXAudio->SetVolSecoundary(m_SEVolume);

	pDMS = &m_apDMSSound[nNo];
	if (pDMS == NULL) {
		return;
	}
	m_pDXAudio->PlaySecoundary(*pDMS);
}


void CMgrSound::PlayBGM(
	int nNo,	// [in] BGMID
	BOOL bPlay)	// [in] 既に同じIDのBGMが再生中の時はそのままにしておく
{
	char szTmp[MAX_PATH];
	TCHAR szBasePath[MAX_PATH];

	if (bPlay) {
		if (m_dwSoundID == (DWORD)nNo) {
			return;
		}
	}

	BuildModuleRelativePath(szBasePath, _countof(szBasePath), _T("BGM\\"));
	std::string strBasePath = TStringToAnsiStd(szBasePath);
	strcpy_s(szTmp, strBasePath.c_str());
	switch (nNo) {
//	case 0:
//		strcat (szTmp, "v4.ogg");
//		break;
//	case 1:
//		strcat (szTmp, "o12.ogg");
//		break;
//	case 2:
//		strcat (szTmp, "shinma_kourin.ogg");
//		break;
	case BGMID_DAICHI_S:
		strcat_s(szTmp, "daichi_s.ogg");
		break;
	case BGMID_HISYOU:
		strcat_s(szTmp, "hisyou.ogg");
		break;
	case BGMID_SUISHA:
		strcat_s(szTmp, "suisha.ogg");
		break;
	case BGMID_FAIRYTALE:
		strcat_s(szTmp, "fairytale.ogg");
		break;
	case BGMID_TABLA_IMAGE:
		strcat_s(szTmp, "tabla_image.ogg");
		break;
	case BGMID_FLOWED_PIANO:
		strcat_s(szTmp, "flowed piano.ogg");
		break;
	case BGMID_HUYUNOMATI_FULL:
		strcat_s(szTmp, "huyunomati_full.ogg");
		break;
	case BGMID_OYAKODON_NAMI:
		strcat_s(szTmp, "oyakodon_nami.ogg");
		break;
	}
	m_dwSoundID = (DWORD)nNo;

	// 再生中のBGMを停止し、新しいBGMをXAudio2で再生
	m_pDXAudio->StopBGM();
	if (!m_pDXAudio->PlayBGMFile(szTmp, TRUE, m_fBGMVolume)) {
		// .ogg 失敗時は .wav にフォールバック（資産変換時用）
		size_t len = strlen(szTmp);
		if (len >= 4 && _stricmp(szTmp + len - 4, ".ogg") == 0) {
			strcpy_s(szTmp + len - 4, _countof(szTmp) - (len - 4), ".wav");
			if (!m_pDXAudio->PlayBGMFile(szTmp, TRUE, m_fBGMVolume)) {
				return;
			}
		} else {
			return;
		}
	}
}


void CMgrSound::StopBGM(void)
{
	m_dwSoundID = 0;

	m_pDXAudio->StopBGM();
}


void CMgrSound::SetBGMVolume(int nVolume)
{
	switch (nVolume) {
	case 0:
		m_fBGMVolume = 0;
		break;
	case 1:
		m_fBGMVolume = 0.25f;
		break;
	case 2:
		m_fBGMVolume = 0.50f;
		break;
	case 3:
		m_fBGMVolume = 0.75f;
		break;
	case 4:
		m_fBGMVolume = 1.00f;
		break;
	}
	m_pDXAudio->SetBGMVolume(m_fBGMVolume);
}


void CMgrSound::SetSEVolume(int nVolume)
{
	switch (nVolume) {
	case 0:
		m_SEVolume = -127;
		break;
	case 1:
		m_SEVolume = -15;
		break;
	case 2:
		m_SEVolume = -7;
		break;
	case 3:
		m_SEVolume = -1;
		break;
	case 4:
		m_SEVolume = 0;
		break;
	}
}


void CMgrSound::ReadSoundData(void)
{
	int i, nCount, nResourceID;

#if !defined(_WIN32)
	return;
#endif

	nCount = m_pLibSboSoundLoader->GetSoundCount();

	SAFE_DELETE_ARRAY(m_apDMSSound);
	m_apDMSSound = new IDirectMusicSegment8*[nCount]();

	for (i = 0; i < nCount; i++) {
		nResourceID = m_pLibSboSoundLoader->GetSoundResourceID(i);
		if (nResourceID == 0) {
			continue;
		}
		m_pDXAudio->GetSegFromRes(
			// m_hDllSoundData は void* なので HMODULE にキャストして Win32 API に渡す
			// （Windowsビルドでのみ到達するコードパス）
			FindResource((HMODULE)m_hDllSoundData, MAKEINTRESOURCE(nResourceID), _T("WAVE")),
			&m_apDMSSound[i]);
	}
}
