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
#ifdef __EMSCRIPTEN__
#include "Platform/SoundDataTableBrowser.h"
#include <emscripten.h>
#include <cstring>
#endif

// Audiere dependency removed

#ifdef __EMSCRIPTEN__
// ブラウザ版の BGM は .data に同梱せず、起動後に個別取得する。
// 8 本で 8.4MB あり、同梱すると初回ロードがその分そのまま遅くなるため。
namespace {

struct BrowserBgmFile {
	int         id;
	const char *pszFile;
};

const BrowserBgmFile s_aBrowserBgmFiles[] = {
	{ BGMID_DAICHI_S,        "daichi_s.ogg"        },
	{ BGMID_HISYOU,          "hisyou.ogg"          },
	{ BGMID_SUISHA,          "suisha.ogg"          },
	{ BGMID_FAIRYTALE,       "fairytale.ogg"       },
	{ BGMID_TABLA_IMAGE,     "tabla_image.ogg"     },
	{ BGMID_FLOWED_PIANO,    "flowed piano.ogg"    },
	{ BGMID_HUYUNOMATI_FULL, "huyunomati_full.ogg" },
	{ BGMID_OYAKODON_NAMI,   "oyakodon_nami.ogg"   },
};

// コールバックからインスタンスへ戻るための参照。CMgrSound は 1 個しか作られない。
CMgrSound *s_pBrowserMgrSound = NULL;

// URL に使えるようスペースだけ %20 に置き換える(ファイル名に空白を含むものがある)
void BuildBgmUrl(char *pszDst, size_t nDstSize, const char *pszFile)
{
	size_t nPos = 0;
	const char *pszPrefix = "BGM/";
	for (const char *p = pszPrefix; (*p != '\0') && (nPos + 1 < nDstSize); ++p) {
		pszDst[nPos++] = *p;
	}
	for (const char *p = pszFile; *p != '\0'; ++p) {
		if (*p == ' ') {
			if (nPos + 3 >= nDstSize) { break; }
			pszDst[nPos++] = '%';
			pszDst[nPos++] = '2';
			pszDst[nPos++] = '0';
		} else {
			if (nPos + 1 >= nDstSize) { break; }
			pszDst[nPos++] = *p;
		}
	}
	pszDst[nPos] = '\0';
}

void BrowserBgmOnLoad(const char *pszFile)
{
	if (s_pBrowserMgrSound != NULL) {
		s_pBrowserMgrSound->OnBrowserBgmLoaded(pszFile);
	}
}

void BrowserBgmOnError(const char *pszFile)
{
	SDL_Log("BGM の取得に失敗しました: %s", (pszFile != NULL) ? pszFile : "(null)");
}

} // namespace
#endif

CMgrSound::CMgrSound()
{
	m_dwSoundID			= 0;
	m_SEVolume			= -7;
	m_fBGMVolume		= 0.50f;
	m_hDllSoundData		= NULL;
	m_pDXAudio			= new CDXAudio;
	m_pLibSboSoundLoader = new CLibSboSoundLoader;
	m_apDMSSound		= NULL;
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

	bRet = FALSE;

	bResult = m_pDXAudio->Create();
	if (bResult == FALSE) {
		// オーディオデバイス初期化失敗時はタイトル表示を優先し無音で継続
		bRet = TRUE;
		goto Exit;
	}

#ifdef __EMSCRIPTEN__
	// Web版: SboSoundData.dll は存在しないため、静的テーブルからWAVファイルを直接ロード
	ReadSoundData();

	// BGM は .data に同梱していないので、起動後に個別取得する。
	// 取得できた順にデコードしてキャッシュへ入れ、鳴らすべきものなら再生を始める。
	s_pBrowserMgrSound = this;
	// emscripten_async_wget は書き込み先の親ディレクトリが無いと失敗する
	EM_ASM({
		try { FS.mkdirTree('/BGM'); } catch (e) {}
	});
	for (size_t i = 0; i < sizeof(s_aBrowserBgmFiles)/sizeof(s_aBrowserBgmFiles[0]); i++) {
		char szUrl[256];
		char szPath[256];
		BuildBgmUrl(szUrl, sizeof(szUrl), s_aBrowserBgmFiles[i].pszFile);
		snprintf(szPath, sizeof(szPath), "/BGM/%s", s_aBrowserBgmFiles[i].pszFile);
		emscripten_async_wget(szUrl, szPath, BrowserBgmOnLoad, BrowserBgmOnError);
	}
#else
	{
		// BuildModuleRelativePath は TCHAR* を受け取るため TCHAR 配列を使用
		TCHAR szPath[MAX_PATH];
		// TCHAR でパスを構築してから ANSI 文字列に変換（SDL_LoadObject は char* を受け取る）
		BuildModuleRelativePath(szPath, _countof(szPath), _T("SboSoundData.dll"));
		std::string ansiPath = TStringToAnsiStd(szPath);

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
	}
#endif

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
	void **pDMS;

	// 効果音テーブル未生成（DLL読み込み失敗時など）は無音で継続
	if (m_apDMSSound == NULL) {
		return;
	}

#ifdef __EMSCRIPTEN__
	// Web版: 静的テーブルから SOUNDID → インデックスを解決
	nNo = SoundDataBrowser::GetSoundNo(dwSoundID);
#else
	nNo = m_pLibSboSoundLoader->GetSoundNo(dwSoundID);
#endif
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


#ifdef __EMSCRIPTEN__
void CMgrSound::OnBrowserBgmLoaded(const char *pszMemfsPath)
{
	if (pszMemfsPath == NULL) {
		return;
	}

	// "/BGM/xxx.ogg" の末尾のファイル名から BGMID を引く
	const char *pszName = strrchr(pszMemfsPath, '/');
	pszName = (pszName != NULL) ? (pszName + 1) : pszMemfsPath;

	int nID = BGMID_NONE;
	for (size_t i = 0; i < sizeof(s_aBrowserBgmFiles)/sizeof(s_aBrowserBgmFiles[0]); i++) {
		if (strcmp(pszName, s_aBrowserBgmFiles[i].pszFile) == 0) {
			nID = s_aBrowserBgmFiles[i].id;
			break;
		}
	}
	if (nID == BGMID_NONE) {
		return;
	}

	if (!m_pDXAudio->PreloadBGM(nID, pszMemfsPath)) {
		SDL_Log("BGM のデコードに失敗しました: %s", pszMemfsPath);
		return;
	}

	// 取得を待っている間に再生要求が来ていた場合はここで鳴らし始める
	if (m_dwSoundID == (DWORD)nID) {
		m_pDXAudio->StopBGM();
		m_pDXAudio->PlayBGMCached(nID, TRUE, m_fBGMVolume);
	}
}
#endif


void CMgrSound::PlayBGM(
	int nNo,	// [in] BGMID
	BOOL bPlay)	// [in] 既に同じIDのBGMが再生中の時はそのままにしておく
{
	char szTmp[MAX_PATH];

	if (bPlay) {
		if (m_dwSoundID == (DWORD)nNo) {
			return;
		}
	}

#ifdef __EMSCRIPTEN__
	// Web版: キャッシュから即時再生（プリデコード済みの場合）
	m_dwSoundID = (DWORD)nNo;
	m_pDXAudio->StopBGM();
	if (m_pDXAudio->PlayBGMCached(nNo, TRUE, m_fBGMVolume)) {
		return;
	}
	// キャッシュヒットしなかった場合は既存のファイル再生にフォールバック
	strcpy_s(szTmp, "/BGM/");
#else
	{
		TCHAR szBasePath[MAX_PATH];
		BuildModuleRelativePath(szBasePath, _countof(szBasePath), _T("BGM\\"));
		std::string strBasePath = TStringToAnsiStd(szBasePath);
		strcpy_s(szTmp, strBasePath.c_str());
	}
#endif
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
#ifdef __EMSCRIPTEN__
	// Web版: SoundDataTableBrowser から WAVファイル名を取得してファイルI/Oでロード
	// WAVファイルは --preload-file で /WAVE/ にマウントされていることが前提
	int nCount = SoundDataBrowser::GetSoundCount();

	SAFE_DELETE_ARRAY(m_apDMSSound);
	m_apDMSSound = new void*[nCount]();

	for (int i = 0; i < nCount; i++) {
		const char* fileName = SoundDataBrowser::GetWavFileName(i);
		if (fileName == NULL || fileName[0] == '\0') continue;
		char szPath[512];
		snprintf(szPath, sizeof(szPath), "/WAVE/%s", fileName);
		m_pDXAudio->LoadWavFromFile(szPath, &m_apDMSSound[i]);
	}
#elif defined(_WIN32)
	// Windows版: SboSoundData.dll のリソースから WAV を読み込む
	int i, nCount, nResourceID;

	nCount = m_pLibSboSoundLoader->GetSoundCount();

	SAFE_DELETE_ARRAY(m_apDMSSound);
	m_apDMSSound = new void*[nCount]();

	for (i = 0; i < nCount; i++) {
		nResourceID = m_pLibSboSoundLoader->GetSoundResourceID(i);
		if (nResourceID == 0) {
			continue;
		}
		m_pDXAudio->GetSegFromRes(
			// m_hDllSoundData は void* なので HMODULE にキャストして Win32 API に渡す
			FindResource((HMODULE)m_hDllSoundData, MAKEINTRESOURCE(nResourceID), _T("WAVE")),
			&m_apDMSSound[i]);
	}
#else
	// 非Windowsかつ非Emscripten: 効果音テーブルを生成しない（PlaySound 側で NULL チェックされる）
	return;
#endif
}
