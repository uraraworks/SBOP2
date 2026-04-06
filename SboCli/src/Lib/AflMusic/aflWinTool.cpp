#pragma unmanaged

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <SDL2/SDL.h>
#endif

#include "aflWinTool.h"
#include <sys/stat.h>
#include <stdio.h>
/*

#ifdef _MSC_VER
	#ifdef _DEBUG	//メモリリークテスト
		#include <crtdbg.h>
		#define malloc(a) _malloc_dbg(a,_NORMAL_BLOCK,__FILE__,__LINE__)
		inline void*  operator new(size_t size, LPCSTR strFileName, INT iLine)
			{return _malloc_dbg(size, _NORMAL_BLOCK, strFileName, iLine);}
		inline void operator delete(void *pVoid, LPCSTR strFileName, INT iLine)
			{_free_dbg(pVoid, _NORMAL_BLOCK);}
		#define new new(__FILE__, __LINE__)
		#define CHECK_MEMORY_LEAK _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF);
	#else
		#define CHECK_MEMORY_LEAK
	#endif //_DEBUG
#else
		#define CHECK_MEMORY_LEAK
#endif

*/

#ifdef _MSC_VER
	#pragma comment(lib, "imm32.lib")
	#pragma comment(lib, "winmm.lib")
#endif

//namespace Afl::Windows
namespace Afl{namespace Windows{

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflSemaphore
// 同期制御用
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
AflSemaphore::AflSemaphore()
{
#ifdef _WIN32
	m_hEvent = CreateSemaphore(NULL,0,100000,NULL);
#else
	// SDL セマフォを初期カウント0で生成
	m_pSem = SDL_CreateSemaphore(0);
#endif
}
AflSemaphore::~AflSemaphore()
{
#ifdef _WIN32
	CloseHandle(m_hEvent);
#else
	SDL_DestroySemaphore(m_pSem);
#endif
}
void AflSemaphore::clear()
{
#ifdef _WIN32
	// カウントを上限まで増やして「フラッシュ」する（元の実装を維持）
	while(ReleaseSemaphore(m_hEvent,1,NULL));
#else
	// セマフォのカウントを0に戻す（保留シグナルを全消費）
	while(SDL_SemTryWait(m_pSem) == 0) {}
#endif
}
bool AflSemaphore::release(LONG lCount)
{
#ifdef _WIN32
	return ReleaseSemaphore(m_hEvent,lCount,NULL)!=0;
#else
	// lCount 回シグナルを発行する
	bool bResult = true;
	for(LONG i = 0; i < lCount; i++)
	{
		if(SDL_SemPost(m_pSem) != 0)
			bResult = false;
	}
	return bResult;
#endif
}
#ifdef _WIN32
HANDLE AflSemaphore::getHandle()const
{
	return m_hEvent;
}
#endif
void AflSemaphore::wait()const
{
#ifdef _WIN32
	WaitForSingleObject(m_hEvent,INFINITE);
#else
	SDL_SemWait(m_pSem);
#endif
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflEvent
// 同期制御用
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
AflEvent::AflEvent()
{
#ifdef _WIN32
	m_hEvent = CreateEvent(NULL,false,false,NULL);
#else
	// SDL セマフォで auto-reset event を模倣（初期カウント0）
	m_pSem = SDL_CreateSemaphore(0);
#endif
}
AflEvent::~AflEvent()
{
#ifdef _WIN32
	CloseHandle(m_hEvent);
#else
	SDL_DestroySemaphore(m_pSem);
#endif
}
void AflEvent::clear()
{
#ifdef _WIN32
	ResetEvent(m_hEvent);
#else
	// 保留シグナルを全て消費してカウントを0に戻す
	while(SDL_SemTryWait(m_pSem) == 0) {}
#endif
}
bool AflEvent::release(LONG lCount)
{
#ifdef _WIN32
	return ReleaseSemaphore(m_hEvent,lCount,NULL)!=0;
#else
	// lCount 回シグナルを発行する
	bool bResult = true;
	for(LONG i = 0; i < lCount; i++)
	{
		if(SDL_SemPost(m_pSem) != 0)
			bResult = false;
	}
	return bResult;
#endif
}
#ifdef _WIN32
HANDLE AflEvent::getHandle()const
{
	return m_hEvent;
}
#endif
void AflEvent::wait()const
{
#ifdef _WIN32
	WaitForSingleObject(m_hEvent,INFINITE);
#else
	SDL_SemWait(m_pSem);
#endif
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflAdviseTimer
// 周期呼び出し用（COM/DirectShow依存 - Windows専用）
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#ifdef _WIN32
AflAdviseTimer::AflAdviseTimer()
{
	//DirectShow用のIReferenceClockのGUID
    static const GUID IID_IReferenceClock = {0x56a86897,0x0ad4,0x11ce,0xb0,0x3a,0x00,0x20,0xaf,0x0b,0xa7,0x70};
    static const GUID CLSID_SystemClock = {0xe436ebb1, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70};
    //COMを利用可能に
	::CoInitialize(NULL);
    //DirectShow用IReferenceClockを利用可能に
	::CoCreateInstance( CLSID_SystemClock , NULL , CLSCTX_INPROC_SERVER , IID_IReferenceClock , (LPVOID*)&m_pReferenceClock);
	//周期カウンタの初期化
	m_dwAdviseToken = 0;
}
AflAdviseTimer::~AflAdviseTimer()
{
	stopTimer();
    //IReferenceClockの解放
	m_pReferenceClock->Release();
	//COM利用完了
    ::CoUninitialize();
}
bool AflAdviseTimer::startTimer(DWORD dwTime)
{
	stopTimer();
	//現在のカウントを開始カウンタに設定
    REFERENCE_TIME rtBase;
    m_pReferenceClock->GetTime(&rtBase);
    //セマフォイベントの設定
	return m_pReferenceClock->AdvisePeriodic(rtBase,dwTime,(HEVENT)m_semaphore.getHandle(),&m_dwAdviseToken) == S_OK;
}
bool AflAdviseTimer::stopTimer()
{
	if(!m_dwAdviseToken)
		return false;
	//イベントの動作停止
	bool bFlag = m_pReferenceClock->Unadvise(m_dwAdviseToken) == S_OK;
	m_dwAdviseToken = 0;
	return bFlag;
}
void AflAdviseTimer::waitTimer()
{
	//時間が来るまで待機
	m_semaphore.wait();
}
bool AflAdviseTimer::release(LONG lCount)
{
	return m_semaphore.release();
}
REFERENCE_TIME AflAdviseTimer::getTime()
{
    //現在のカウントを取得
	REFERENCE_TIME rtBase;
    m_pReferenceClock->GetTime(&rtBase);
    return rtBase;
}
#endif // _WIN32

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflMediaTimer
// 周期呼び出し用
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifndef _WIN32
// SDL タイマーコールバック: 毎インターバルでセマフォにシグナルを発行する
static Uint32 SDLTimerCallback(Uint32 interval, void* param)
{
	SDL_SemPost((SDL_sem*)param);
	return interval; // 繰り返し継続
}
#endif

AflMediaTimer::AflMediaTimer()
{
#ifdef _WIN32
	timeGetDevCaps(&m_timeCaps,sizeof(m_timeCaps));
	timeBeginPeriod(m_timeCaps.wPeriodMin);
	m_timerID = 0;
#else
	// SDL セマフォを初期カウント0で生成
	m_pSem = SDL_CreateSemaphore(0);
	m_timerID = 0;
#endif
}
AflMediaTimer::~AflMediaTimer()
{
	stopTimer();
#ifdef _WIN32
	timeEndPeriod(m_timeCaps.wPeriodMin);
#else
	SDL_DestroySemaphore(m_pSem);
#endif
}
bool AflMediaTimer::startTimer(DWORD dwTime)
{
	stopTimer();
#ifdef _WIN32
    //セマフォイベントの設定
	m_timerID = timeSetEvent(m_timeCaps.wPeriodMin,0,
		(LPTIMECALLBACK)m_event.getHandle(),0,TIME_PERIODIC | TIME_CALLBACK_EVENT_SET);
	return m_timerID != 0;
#else
	// SDLの最小分解能(1ms)でタイマーを起動し、毎インターバルでセマフォをシグナル
	m_timerID = SDL_AddTimer(1, SDLTimerCallback, m_pSem);
	return m_timerID != 0;
#endif
}
bool AflMediaTimer::stopTimer()
{
	if(!m_timerID)
		return false;
	//イベントの動作停止
#ifdef _WIN32
	bool bFlag = timeKillEvent(m_timerID) == S_OK;
#else
	bool bFlag = SDL_RemoveTimer(m_timerID) == SDL_TRUE;
#endif
	m_timerID = 0;
	return bFlag;
}
void AflMediaTimer::waitTimer()
{
	//時間が来るまで待機
#ifdef _WIN32
	m_event.wait();
#else
	SDL_SemWait(m_pSem);
#endif
}
bool AflMediaTimer::release(LONG lCount)
{
#ifdef _WIN32
	return m_event.release();
#else
	// lCount 回シグナルを発行する
	bool bResult = true;
	for(LONG i = 0; i < lCount; i++)
	{
		if(SDL_SemPost(m_pSem) != 0)
			bResult = false;
	}
	return bResult;
#endif
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflTimeCounter
// 進行速度維持用カウンタ
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
AflTimeCounter::AflTimeCounter()
{
#ifdef _WIN32
	//タイマーの分解能設定
	TIMECAPS timeCaps;
	timeGetDevCaps(&timeCaps,sizeof(TIMECAPS));
	timeBeginPeriod(timeCaps.wPeriodMin);
#endif
	// SDLはタイマー分解能をSDL内部で管理するため非Windows環境では不要

	m_bEnable = true;
	m_dwSyncTime = 10;
	resetTime();
}
AflTimeCounter::~AflTimeCounter()
{
#ifdef _WIN32
	//タイマーの分解能復帰
	TIMECAPS timeCaps;
	timeGetDevCaps(&timeCaps,sizeof(TIMECAPS));
	timeEndPeriod(timeCaps.wPeriodMin);
#endif
	// 非Windows環境ではSDLが管理するため何もしない
}
void AflTimeCounter::setTimeSync(DWORD dwTime)
{
	m_dwSyncTime = dwTime;
	resetTime();
}
void AflTimeCounter::resetTime()
{
	m_iNowCount = 0;
#ifdef _WIN32
	m_dwStartTime = timeGetTime();
#else
	// SDL_GetTicks() でミリ秒タイムスタンプを取得
	m_dwStartTime = SDL_GetTicks();
#endif
}
DWORD AflTimeCounter::getTime() const
{
#ifdef _WIN32
	return timeGetTime() - m_dwStartTime;
#else
	return SDL_GetTicks() - m_dwStartTime;
#endif
}
INT AflTimeCounter::getCount()
{
	//ウエイト無効のスキップ処理
/*	if(!m_bEnable)
	{
		m_iNowCount =  getTime() / m_dwSyncTime;
		m_dwSleepTime = 0;
		return 1;
	}
*/
	//通常処理
	INT iOldCount;
	DWORD dwTime;
	INT iCount;

	iOldCount = m_iNowCount;
	dwTime = getTime();
	m_iNowCount =  dwTime / m_dwSyncTime;
	iCount = m_iNowCount - iOldCount;

	if(!iCount)
		m_dwSleepTime = dwTime % m_dwSyncTime;
	else
		m_dwSleepTime = 0;
	return iCount;
}
void AflTimeCounter::sleep() const
{
#ifdef _WIN32
	if(m_bEnable)
		Sleep(m_dwSleepTime);
	else
		Sleep(0);
#else
	if(m_bEnable)
		SDL_Delay(m_dwSleepTime);
	else
		SDL_Delay(0);
#endif
}
DWORD AflTimeCounter::sleepTime() const
{
	if(m_bEnable)
		return m_dwSleepTime;
	return 0;
}
void AflTimeCounter::setEnable(bool bFlag)
{
	m_bEnable = bFlag;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflFileName
// ファイル名とパス制御（Windows専用実装）
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#ifdef _WIN32
bool AflFileName::setFileName(LPCSTR pFileName)
{
	CHAR cPath[_MAX_PATH];
	CHAR cDrive[_MAX_DRIVE];
	CHAR cDir[_MAX_DIR];
	CHAR cFname[_MAX_FNAME];
	CHAR cExt[_MAX_EXT];

	_fullpath(cPath,pFileName,_MAX_PATH);
	_splitpath(cPath,cDrive,cDir,cFname,cExt);

	m_strFullPath = cPath;
	m_strDrive = cDrive;
	m_strDir = cDir;
	m_strFname = cFname;
	m_strExt = cExt;

	m_strPath = m_strDrive.c_str();
	m_strPath += m_strDir.c_str();

	m_strFileName = m_strFname.c_str();
	m_strFileName += m_strExt.c_str();

	return true;
}
std::string AflFileName::relativePath(LPCSTR pFileName)
{
	std::string strFileName;
	AflFileName fileName(pFileName);

	LPCSTR pBasePath = fileName.getPathName();
	LPCSTR pPath = getPathName();

	INT i;
	INT iBaseCount;
	INT iLength = (INT)strspn(pBasePath,pPath);
	if(iLength)
	{
		for(;iLength>0;iLength--)
			if(pBasePath[iLength] == '\\' || pBasePath[iLength] == '/')
				break;
		iLength++;

		for(iBaseCount=0,i=iLength;pBasePath[i];i++)
			if(pBasePath[i] == '\\' || pBasePath[i] == '/')
				iBaseCount++;
		if(pPath[i] || pBasePath[i])
		{
			for(i=0;i<iBaseCount;i++)
				strFileName += "..\\";

			strFileName += pPath + iLength;
		}
	}
	else
		strFileName += pPath;

	strFileName += m_strFileName.c_str();

	return strFileName;
}
bool AflFileName::pushPath()
{
	TCHAR cPath[_MAX_PATH];
	m_strTempPath = _getcwd(cPath,_MAX_PATH);
	return true;
}
bool AflFileName::popPath()
{
	if(m_strTempPath[0] >= 'A' && m_strTempPath[0] <= 'Z')
		_chdrive(m_strTempPath[0] - 'A' + 1);
	else if(m_strTempPath[0] >= 'a' && m_strTempPath[0] <= 'z')
		_chdrive(m_strTempPath[0] - 'a' + 1);
	chdir(m_strTempPath.c_str());
	return true;
}

bool AflFileName::changePath()
{
	if(m_strDrive[0] >= 'A' && m_strDrive[0] <= 'Z')
		_chdrive(m_strDrive[0] - 'A' + 1);
	else if(m_strDrive[0] >= 'a' && m_strDrive[0] <= 'z')
		_chdrive(m_strDrive[0] - 'a' + 1);
	chdir(m_strPath.c_str());
	return true;
}
#endif // _WIN32


//------------------------------------------------------------
// AflRect
// ３次元座標管理
//------------------------------------------------------------

//-----------------------------------------------------
//	初期化
//-----------------------------------------------------
AflRect::AflRect()
{
	left=right=top=bottom=high=low=0;
}
//-----------------------------------------------------
//	初期化
//-----------------------------------------------------
AflRect::AflRect(int nLeft,int nTop,int nHigh,int nRight,int nBottom,int nLow)
{
	setRect(nLeft,nTop,nHigh,nRight,nBottom,nLow);
}
AflRect::AflRect(int nLeft,int nTop,int nRight,int nBottom)
{
	setRect(nLeft,nTop,nRight,nBottom);
}
//-----------------------------------------------------
//	座標設定
//-----------------------------------------------------
void AflRect::setRect(int nLeft,int nTop,int nHigh,int nRight,int nBottom,int nLow)
{
	left = nLeft; right=nRight;
	top = nTop; bottom = nBottom;
	high = nHigh; low = nLow;
}
void AflRect::setRect(int nLeft,int nTop,int nRight,int nBottom)
{
	left = nLeft; right=nRight;
	top = nTop; bottom = nBottom;
	high = 0; low = 0;
}
//-----------------------------------------------------
//	座標移動
//-----------------------------------------------------
void AflRect::offsetRect(int nX,int nY,int nZ)
{
	left += nX; right += nX;
	top += nY; bottom += nY;
	high += nZ; low += nZ;
}
//-----------------------------------------------------
//	全て0かどうかの判定
//-----------------------------------------------------
bool AflRect::isRectNull() const
{
	if(left | right | top | bottom | high | low)
		return false;
	else
		return true;
}

#ifdef _WIN32
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflFont
// フォント系（GDI依存 - Windows専用）
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//-----------------------------------------------------
AflFont::AflFont()
{
	m_hFont = 0;
	m_iFontBold = FW_SEMIBOLD;
	m_sizeFont.cy = 16;
	m_sizeFont.cx = 0;
	m_strFontName = "ＭＳ ゴシック";
}
//-----------------------------------------------------
AflFont::~AflFont()
{
	deleteFont();
}
//-----------------------------------------------------
void AflFont::setFontName(LPCSTR pString)
{
	m_strFontName = pString;
}
//-----------------------------------------------------
BOOL AflFont::createFont()
{
        deleteFont();

#ifdef _UNICODE
        std::wstring strFontNameW;
        if (!m_strFontName.empty()) {
                int nLength = MultiByteToWideChar(CP_ACP, 0, m_strFontName.c_str(), -1, NULL, 0);
                if (nLength > 0) {
                        strFontNameW.resize(nLength - 1);
                        MultiByteToWideChar(CP_ACP, 0, m_strFontName.c_str(), -1, &strFontNameW[0], nLength);
                }
        }
        LPCTSTR pszFontName = strFontNameW.empty() ? TEXT("") : strFontNameW.c_str();
#else
        LPCTSTR pszFontName = m_strFontName.c_str();
#endif

        m_hFont = ::CreateFont(m_sizeFont.cy,m_sizeFont.cx,0,0,m_iFontBold,FALSE,FALSE,FALSE,
                DEFAULT_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,
                pszFontName);
        return m_hFont!=0;
}
//-----------------------------------------------------
void AflFont::deleteFont()
{
	if(m_hFont)
	{
		::DeleteObject(m_hFont);
		m_hFont = 0;
	}
}
//-----------------------------------------------------
bool AflFont::getFontSize(LPSIZE pSize,LPCSTR pText,INT iLength,INT iLimitWidth)
{
	return getFontSize(pSize,m_hFont,pText,iLength,iLimitWidth);
}
//-----------------------------------------------------
bool AflFont::getFontSize(LPSIZE pSize,LPCWSTR pText,INT iLength,INT iLimitWidth)
{
	return getFontSize(pSize,m_hFont,pText,iLength,iLimitWidth);
}
//-----------------------------------------------------
bool AflFont::getFontSize(LPSIZE pSize,HFONT hFont,LPCSTR pText,INT iLength,INT iLimitWidth)
{
	if(iLength < 0)
		iLength = (INT)lstrlen(pText);
	//フォントサイズの取得
	SIZE sizeFont;
	INT i;
	HDC hdmDC = CreateCompatibleDC(NULL);
	HFONT holdFont = (HFONT)SelectObject(hdmDC,hFont);

	GetTextExtentPoint32(hdmDC," ",1,&sizeFont);

	INT iWidth = 0;
	INT iHeight = 0;
	INT iMaxWidth = 0;
	for(i=0;i<iLength;i++)
	{
		bool bFlag = isSJIS(pText[i]);
		if(pText[i] == '\t')
		{
			GetTextExtentPoint32(hdmDC," ",1,&sizeFont);
			iWidth += sizeFont.cx*4;
			iWidth -= iWidth % (sizeFont.cx*4);
		}
		else if(pText[i] != '\n')
			GetTextExtentPoint32(hdmDC,pText+i,bFlag+1,&sizeFont);
		else
			sizeFont.cx = 0;
		if(pText[i] == '\n' || iLimitWidth>0 && sizeFont.cx + iWidth > iLimitWidth)
		{
			iHeight += sizeFont.cy;
			if(iWidth > iMaxWidth)
				iMaxWidth = iWidth;
			iWidth = sizeFont.cx;
		}
		else
			iWidth += sizeFont.cx;
		i += bFlag;
	}
	iHeight += sizeFont.cy;
	SelectObject(hdmDC,holdFont);
	DeleteDC(hdmDC);

	if(iWidth > iMaxWidth)
		iMaxWidth = iWidth;

	if(pSize)
	{
		pSize->cx = iMaxWidth;
		pSize->cy = iHeight;
	}
	return true;
}

//-----------------------------------------------------
bool AflFont::getFontSize(LPSIZE pSize,HFONT hFont,LPCWSTR pText,INT iLength,INT iLimitWidth)
{
	if(iLength < 0)
		iLength = lstrlenW(pText);
	//フォントサイズの取得
	SIZE sizeFont;
	INT i;
	HDC hdmDC = CreateCompatibleDC(NULL);
	HFONT holdFont = (HFONT)SelectObject(hdmDC,hFont);

	GetTextExtentPoint32W(hdmDC,L" ",1,&sizeFont);

	INT iWidth = 0;
	INT iHeight = 0;
	INT iMaxWidth = 0;
	for(i=0;i<iLength;i++)
	{
		bool bFlag = isSJIS(pText[i]);
		if(pText[i] == '\t')
		{
			GetTextExtentPoint32W(hdmDC,L" ",1,&sizeFont);
			iWidth += sizeFont.cx*4;
			iWidth -= iWidth % (sizeFont.cx*4);
		}
		else if(pText[i] != '\n')
			GetTextExtentPoint32W(hdmDC,pText+i,bFlag+1,&sizeFont);
		else
			sizeFont.cx = 0;
		if(pText[i] == '\n' || iLimitWidth && sizeFont.cx + iWidth > iLimitWidth)
		{
			iHeight += sizeFont.cy;
			if(iWidth > iMaxWidth)
				iMaxWidth = iWidth;
			iWidth = sizeFont.cx;
		}
		else
			iWidth += sizeFont.cx;
		i += bFlag;
	}
	iHeight += sizeFont.cy;
	SelectObject(hdmDC,holdFont);
	DeleteDC(hdmDC);

	if(iWidth > iMaxWidth)
		iMaxWidth = iWidth;

	if(pSize)
	{
		pSize->cx = iMaxWidth;
		pSize->cy = iHeight;
	}
	return true;
}
//-----------------------------------------------------

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflFep
// FEP制御用（IMM依存 - Windows専用）
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//-----------------------------------------------------
AflFep::AflFep()
{
    m_hWnd = 0;
    m_pCandidateList = NULL;
}
//-----------------------------------------------------
AflFep::~AflFep()
{
    release();
}
//-----------------------------------------------------
bool AflFep::release()
{
	//コンテキストの解放
    if(m_pCandidateList)
    {
		delete m_pCandidateList;
		m_pCandidateList = NULL;
	}

    return true;
}
//-----------------------------------------------------
bool AflFep::setWindow(HWND hWnd)
{
    release();
	if(hWnd)
	{
		//既存のFEPの破壊
		HWND hFep = ImmGetDefaultIMEWnd(hWnd);
		//SendMessage(hFep,WM_CLOSE,0,0);

	    //ウインドウに関連づけられているFEPの取得
		m_hWnd = hWnd;
	}
	return true;
}
//-----------------------------------------------------
bool AflFep::isOpen() const
{
	//FEPの活動状態を取得
	HIMC hImc = ImmGetContext(m_hWnd);
	bool bRet = ImmGetOpenStatus(hImc)!=false;
	ImmReleaseContext(m_hWnd,hImc);
	return bRet;
}
//-----------------------------------------------------
bool AflFep::setOpenStatus(bool bFlag)
{
	HIMC hImc = ImmGetContext(m_hWnd);
	bool bRet = ImmSetOpenStatus(hImc,bFlag)!=false;
	ImmReleaseContext(m_hWnd,hImc);
	return bRet;
}
//-----------------------------------------------------
bool AflFep::getStatus(LPDWORD pdwConversion,LPDWORD pdwSentence) const
{
	//FEPの入力モード
	 HIMC hImc = ImmGetContext(m_hWnd);
	bool bRet = ImmGetConversionStatus(hImc,pdwConversion,pdwSentence)!=false;
	ImmReleaseContext(m_hWnd,hImc);
	return bRet;
}
//-----------------------------------------------------
bool AflFep::getString(std::string& strBuff) const
{
    //FEP入力文字列の取得
    LONG lByte;
	 HIMC hImc = ImmGetContext(m_hWnd);
    lByte = ImmGetCompositionString(hImc,GCS_COMPSTR,NULL,0);
	if(lByte > 0)
	{
		LPSTR pBuff = new CHAR[lByte+1];
		ImmGetCompositionString(hImc,GCS_COMPSTR,pBuff,lByte);
		pBuff[lByte] = 0;
		strBuff = pBuff;
		delete pBuff;
	}
	ImmReleaseContext(m_hWnd,hImc);
    return true;
}
//-----------------------------------------------------
INT AflFep::getConvertPos() const
{
	//変換位置の取得
    DWORD dwBuff[2];
	HIMC hImc = ImmGetContext(m_hWnd);
    if(ImmGetCompositionString(hImc,GCS_COMPCLAUSE,dwBuff,sizeof(DWORD)*2) <= 0)
	{
		ImmReleaseContext(m_hWnd,hImc);
		return 0;
	}
	ImmReleaseContext(m_hWnd,hImc);
    return dwBuff[1];
}
//-----------------------------------------------------
INT AflFep::getCursorCount() const
{
	//カーソル位置の取得
	HIMC hImc = ImmGetContext(m_hWnd);
    INT iRet = ImmGetCompositionString(hImc,GCS_CURSORPOS,NULL,0);
	ImmReleaseContext(m_hWnd,hImc);
	if(iRet > 0)
		return iRet;
	return iRet;
}
//-----------------------------------------------------
INT AflFep::getCountCandidate()
{
	//前回使った変換候補の解放
    if(m_pCandidateList)
	{
        delete m_pCandidateList;
		m_pCandidateList = NULL;
	}
    //変換候補の取得
	HIMC hImc = ImmGetContext(m_hWnd);
	INT iCandidateSize = ImmGetCandidateList(hImc,0,NULL,0);
	if(!iCandidateSize)
	{
		ImmReleaseContext(m_hWnd,hImc);
		return 0;
	}
	ImmReleaseContext(m_hWnd,hImc);
    m_pCandidateList = (LPCANDIDATELIST)new BYTE[iCandidateSize];
    ImmGetCandidateList(hImc,0,m_pCandidateList,iCandidateSize);
    return m_pCandidateList->dwCount;
}
//-----------------------------------------------------
INT AflFep::getCandidateIndex() const
{
	//現在選択中の変換候補の取得
    return m_pCandidateList->dwSelection;
}
//-----------------------------------------------------
LPCSTR AflFep::getCandidateString(INT iIndex) const
{
	//指定されたインデックスの変換候補を取得
	if(!m_pCandidateList)
		return NULL;
    return (LPCSTR)((LPBYTE)m_pCandidateList+m_pCandidateList->dwOffset[iIndex]);
}
//-----------------------------------------------------
INT AflFep::getCandidateStart() const
{
	//変換候補インデックスの開始位置の取得
    return m_pCandidateList->dwPageStart;
}
//-----------------------------------------------------
INT AflFep::getCandidateEnd() const
{
	//変換候補インデックスの終了位置の取得
    int iCount = (int)(m_pCandidateList->dwPageStart + m_pCandidateList->dwPageSize);
    if(iCount > (int)m_pCandidateList->dwCount)
        iCount = m_pCandidateList->dwCount;
    return iCount;
}
//-----------------------------------------------------
#endif // _WIN32

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflThreadTimer
// 時間制御ループスレッド
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

AflThreadTimer::AflThreadTimer()
{
	m_bEnable = false;
	m_bEnter = false;
	m_paflProcStart = NULL;
	m_paflProcEnd = NULL;
	m_paflProcAction = NULL;
	m_paflProcIdle = NULL;
	m_paflProcRender = NULL;

	m_aflProc.setAddress(this,AFLPROC(AflThreadTimer,onProc));
}
AflThreadTimer::~AflThreadTimer()
{
	stop();
	if(m_paflProcStart)
		delete m_paflProcStart;
	if(m_paflProcEnd)
		delete m_paflProcEnd;
	if(m_paflProcAction)
		delete m_paflProcAction;
	if(m_paflProcIdle)
		delete m_paflProcIdle;
	if(m_paflProcRender)
		delete m_paflProcRender;
}
void AflThreadTimer::start(LPVOID pvoid)
{
	stop();
	m_aflTimeCounter.resetTime();
	m_bEnable = true;
	m_bEnter = true;
	m_thread.startThread(&m_aflProc,pvoid);
}
void AflThreadTimer::stop()
{
	INT i;
	m_bEnable = false;
	for(i=0;i<1000 && m_bEnter;i++)
#ifdef _WIN32
		Sleep(1);
#else
		SDL_Delay(1);
#endif
}
DWORD AflThreadTimer::getInterval()
{
	return m_aflTimeCounter.getTimeSync();
}
void AflThreadTimer::setInterval(DWORD dwInterval)
{
	return m_aflTimeCounter.setTimeSync(dwInterval);
}
void AflThreadTimer::setRender(AflClassProc* paflClassProc)
{
	if(m_paflProcRender)
		delete m_paflProcRender;
	m_paflProcRender = paflClassProc;
}

void AflThreadTimer::setStart(AflClassProc* paflClassProc)
{
	if(m_paflProcStart)
		delete m_paflProcStart;
	m_paflProcStart = paflClassProc;
}
void AflThreadTimer::setEnd(AflClassProc* paflClassProc)
{
	if(m_paflProcEnd)
		delete m_paflProcEnd;
	m_paflProcEnd = paflClassProc;
}
void AflThreadTimer::setAction(AflClassProc* paflClassProc)
{
	if(m_paflProcAction)
		delete m_paflProcAction;
	m_paflProcAction = paflClassProc;
}
void AflThreadTimer::setIdle(AflClassProc* paflClassProc)
{
	if(m_paflProcIdle)
		delete m_paflProcIdle;
	m_paflProcIdle = paflClassProc;
}

void AflThreadTimer::onProc(LPVOID pvoid)
{
	if(m_paflProcStart)
		m_paflProcStart->call(pvoid);

	while(m_bEnable)
	{
		INT i;
		//必要動作回数
		INT iCount = m_aflTimeCounter.getCount();

		//実行処理用ループ
		if(m_bEnable && m_paflProcAction)
		{
			for(i=0;m_bEnable && i<iCount;i++)
				m_paflProcAction->call(pvoid);
		}
		//アイドル処理用
		if(m_bEnable && m_paflProcIdle)
		{
			m_paflProcIdle->call(pvoid);
		}
		//レンダリング用
		if(m_bEnable && m_paflProcRender)
		{
			//レンダリングの必要性を確認
			if(m_bEnable && iCount > 0 || !m_aflTimeCounter.isEnable())
			{
				m_paflProcRender->call(pvoid);
			}
		}
		if(m_bEnable)
		{
#ifdef _WIN32
			Sleep(m_aflTimeCounter.sleepTime());
#else
			SDL_Delay(m_aflTimeCounter.sleepTime());
#endif
		}
	}

	if(m_paflProcEnd)
		m_paflProcEnd->call(pvoid);

	m_bEnter = false;
}
//namespace
};};
