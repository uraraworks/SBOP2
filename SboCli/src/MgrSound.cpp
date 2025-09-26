/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MgrSound.cpp												 */
/* 内容			:サウンドデータ管理クラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/05/09													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "resource.h"
#include "DXAudio.h"
#include "LibMusicLoader.h"
#include "LibSboSoundLoader.h"
#include "MgrSound.h"

// Audiere dependency removed

/* ========================================================================= */
/* 関数名	:CMgrSound::CMgrSound											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/05/09														 */
/* ========================================================================= */

CMgrSound::CMgrSound()
{
	m_dwSoundID				= 0;
	m_SEVolume				= -7;
	m_fBGMVolume			= 0.50f;
	m_hDllSoundData			= NULL;
	m_pDXAudio				= new CDXAudio;
	m_pLibMusicLoader		= new CLibMusicLoader;
	m_pLibSboSoundLoader	= new CLibSboSoundLoader;
	m_apDMSSound			= NULL;
    /* BGM: XAudio2 再生に切替のため未使用 */
}


/* ========================================================================= */
/* 関数名	:CMgrSound::~CMgrSound											 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/05/09														 */
/* ========================================================================= */

CMgrSound::~CMgrSound()
{
	Destroy ();

	SAFE_DELETE (m_pDXAudio);
	SAFE_DELETE (m_pLibMusicLoader);
	SAFE_DELETE (m_pLibSboSoundLoader);
	SAFE_DELETE_ARRAY (m_apDMSSound);
}


/* ========================================================================= */
/* 関数名	:CMgrSound::Create												 */
/* 内容		:作成															 */
/* 日付		:2006/05/09														 */
/* ========================================================================= */

BOOL CMgrSound::Create(void)
{
	BOOL bRet, bResult;
        TCHAR szPath[MAX_PATH];
        CmyString strFileName;

	bRet = FALSE;

        GetModuleFilePath (szPath, _countof (szPath));
        CString strModulePath (szPath);
        strFileName.Format (_T("%sSboSoundData.dll"), (LPCTSTR)strModulePath);

	bResult = m_pDXAudio->Create ();
	if (bResult == FALSE) {
		goto Exit;
	}
        m_hDllSoundData = LoadLibrary (strFileName);
	m_pDXAudio->SetResourceHandle (m_hDllSoundData);

	m_pLibMusicLoader->		Load ();
    m_pLibSboSoundLoader->	Load ();

	/* 効果音を読み込み */
	ReadSoundData ();

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CMgrSound::Destroy												 */
/* 内容		:破棄															 */
/* 日付		:2006/05/09														 */
/* ========================================================================= */

void CMgrSound::Destroy(void)
{
	if (m_pDXAudio) {
		m_pDXAudio->Destroy ();
	}
    if (m_pLibMusicLoader) {
        m_pLibMusicLoader->Free ();
    }
}


/* ========================================================================= */
/* 関数名	:CMgrSound::PlaySound											 */
/* 内容		:効果音の再生													 */
/* 日付		:2006/05/09														 */
/* ========================================================================= */

void CMgrSound::PlaySound(DWORD dwSoundID)
{
	int nNo;
	IDirectMusicSegment8 **pDMS;

	nNo = m_pLibSboSoundLoader->GetSoundNo (dwSoundID);
	if (nNo < 0) {
		return;
	}
	m_pDXAudio->SetVolSecoundary (m_SEVolume);

	pDMS = &m_apDMSSound[nNo];
	if (pDMS == NULL) {
		return;
	}
	m_pDXAudio->PlaySecoundary (*pDMS);
}


/* ========================================================================= */
/* 関数名	:CMgrSound::PlayBGM												 */
/* 内容		:BGMの再生														 */
/* 日付		:2006/05/09														 */
/* ========================================================================= */

void CMgrSound::PlayBGM(
	int nNo,		/* [in] BGMID */
	BOOL bPlay)		/* [in] 既に同じIDのBGMが再生中の時はそのままにしておく */
{
    char szTmp[MAX_PATH];
    TCHAR szBasePath[MAX_PATH];

	if (bPlay) {
		if (m_dwSoundID == (DWORD)nNo) {
			return;
		}
	}

        GetModuleFilePath (szBasePath, _countof (szBasePath));
        CStringA strBasePath = TStringToAnsi (szBasePath);
        strcpy_s (szTmp, strBasePath);
        strcat_s (szTmp, "BGM\\");
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
                strcat_s (szTmp, "daichi_s.ogg");
		break;
	case BGMID_HISYOU:
                strcat_s (szTmp, "hisyou.ogg");
		break;
	case BGMID_SUISHA:
                strcat_s (szTmp, "suisha.ogg");
		break;
	case BGMID_FAIRYTALE:
                strcat_s (szTmp, "fairytale.ogg");
		break;
	case BGMID_TABLA_IMAGE:
                strcat_s (szTmp, "tabla_image.ogg");
		break;
	case BGMID_FLOWED_PIANO:
                strcat_s (szTmp, "flowed piano.ogg");
		break;
	case BGMID_HUYUNOMATI_FULL:
                strcat_s (szTmp, "huyunomati_full.ogg");
		break;
	case BGMID_OYAKODON_NAMI:
                strcat_s (szTmp, "oyakodon_nami.ogg");
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


/* ========================================================================= */
/* 関数名	:CMgrSound::StopBGM												 */
/* 内容		:BGMの停止														 */
/* 日付		:2006/05/09														 */
/* ========================================================================= */

void CMgrSound::StopBGM(void)
{
	m_dwSoundID = 0;

    m_pDXAudio->StopBGM();
}


/* ========================================================================= */
/* 関数名	:CMgrSound::SetBGMVolume										 */
/* 内容		:BGM音量設定													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

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


/* ========================================================================= */
/* 関数名	:CMgrSound::SetSEVolume											 */
/* 内容		:効果音量設定													 */
/* 日付		:2007/05/11														 */
/* ========================================================================= */

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


/* ========================================================================= */
/* 関数名	:CMgrSound::ReadSoundData										 */
/* 内容		:効果音を読み込み												 */
/* 日付		:2007/05/03														 */
/* ========================================================================= */

void CMgrSound::ReadSoundData(void)
{
	int i, nCount, nResourceID;

	nCount = m_pLibSboSoundLoader->GetSoundCount ();

	SAFE_DELETE_ARRAY (m_apDMSSound);
	m_apDMSSound = new IDirectMusicSegment8*[nCount];

	for (i = 0; i < nCount; i ++) {
		nResourceID = m_pLibSboSoundLoader->GetSoundResourceID (i);
		if (nResourceID == 0) {
			continue;
		}
		m_pDXAudio->GetSegFromRes (
                            FindResource (m_hDllSoundData, MAKEINTRESOURCE (nResourceID), _T("WAVE")),
				&m_apDMSSound[i]);
	}
}

/* Copyright(C)URARA-works 2006 */
