#if _MSC_VER >= 1100
#pragma once
#endif // _MSC_VER >= 1100
#ifndef __INC_AFLWINTOOL

#pragma warning( disable : 4786 )	//STLの警告外し
#include <string>
#include <map>

#ifdef _WIN32
#include <imm.h>
#include <strmif.h>
#else
#include <SDL2/SDL.h>
#endif

#include "aflStd.h"


//namespace Afl::Windows
namespace Afl{namespace Windows{

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflFRect
// 矩形サイズ小数点
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
typedef struct AflFRect
{
	FLOAT fTop,fLeft,fBottom,fRight;

	FLOAT getWidth()const
	{
		return fRight - fLeft;
	}
	FLOAT getHeight()const
	{
		return fBottom - fTop;
	}
}AFLFRECT,*LPAFLFRECT,*PAFLFRECT;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflSemaphore
// 同期制御用
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class AflSemaphore
{
public:
	AflSemaphore();
	~AflSemaphore();
	void clear();
	bool release(LONG lCount=1);
	void wait()const;

#ifdef _WIN32
	HANDLE getHandle()const;
#endif

protected:
#ifdef _WIN32
	HANDLE m_hEvent;
#else
	SDL_sem* m_pSem;
#endif
};


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflAdviseTimer
// 周期呼び出し用（Windows/COM/DirectShow依存 - 非Windows環境では不使用）
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#ifdef _WIN32
class AflAdviseTimer
{
public:
    AflAdviseTimer();
    ~AflAdviseTimer();
	bool startTimer(DWORD dwTime);
	bool stopTimer();
    void waitTimer();
 	bool release(LONG lCount=1);
    REFERENCE_TIME getTime();
protected:
    struct IReferenceClock* m_pReferenceClock;
    DWORD m_dwAdviseToken;
	AflSemaphore m_semaphore;
};
#endif // _WIN32


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflEvent
// 同期制御用
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class AflEvent
{
public:
	AflEvent();
	~AflEvent();
	void clear();
	bool release(LONG lCount=1);
	void wait()const;

#ifdef _WIN32
	HANDLE getHandle()const;
#endif

protected:
#ifdef _WIN32
	HANDLE m_hEvent;
#else
	SDL_sem* m_pSem;
#endif
};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflMediaTimer
// 周期呼び出し用
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class AflMediaTimer
{
public:
    AflMediaTimer();
    ~AflMediaTimer();
	bool startTimer(DWORD dwTime);
	bool stopTimer();
    void waitTimer();
 	bool release(LONG lCount=1);
    DWORD getTime();
protected:
#ifdef _WIN32
	AflEvent m_event;
	MMRESULT m_timerID;
	TIMECAPS m_timeCaps;
#else
	SDL_sem* m_pSem;
	SDL_TimerID m_timerID;
#endif
};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflTimeCounter
// 進行速度維持用カウンタ
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class AflTimeCounter
{
public:
	AflTimeCounter();
	~AflTimeCounter();
	void resetTime();
	DWORD getTime() const;
	INT getCount();
	void sleep() const;
	DWORD sleepTime() const;

	void setEnable(bool bFlag);
	void setTimeSync(DWORD dwTime);
	DWORD getTimeSync(){return m_dwSyncTime;}
	bool isEnable()const{return m_bEnable;}
protected:
	DWORD m_dwSyncTime;
	DWORD m_dwStartTime;
	INT m_iNowCount;
	DWORD m_dwSleepTime;
	bool m_bEnable;
};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflFileName
// ファイル名とパス制御（Windows専用実装）
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#ifdef _WIN32
class AflFileName
{
public:
	AflFileName(){}
	AflFileName(LPCSTR pFileName){setFileName(pFileName);}
	bool setFileName(LPCSTR pFileName);
	bool pushPath();
	bool popPath();
	bool changePath();
	std::string relativePath(LPCSTR pFileName);

	LPCSTR getFileName()const{return m_strFileName.c_str();}
	LPCSTR getPathName()const{return m_strPath.c_str();}

protected:
	std::string m_strFullPath;
	std::string m_strDrive;
	std::string m_strDir;
	std::string m_strFname;
	std::string m_strExt;
	std::string m_strPath;
	std::string m_strFileName;

	std::string m_strTempPath;
};
#endif // _WIN32

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflFep
// FEP制御用
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class AflRect : public RECT
{
public:
	operator LPRECT(){return this;}
	AflRect();
	AflRect(int nLeft,int nTop,int nHigh,int nRight,int nBottom,int nLow);
	AflRect(int nLeft,int nTop,int nRight,int nBottom);
	void setRect(int nLeft,int nTop,int nRight,int nBottom);
	void setRect(int nLeft,int nTop,int nHigh,int nRight,int nBottom,int nLow);
	void offsetRect(int nX,int nY,int nZ=0);
	bool isRectNull() const;
	int getWidth(){return right - left;}
	int getHeight(){return bottom - top;}
	int high,low;
};

#ifdef _WIN32
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflFont
// フォント系（GDI依存 - Windows専用）
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class AflFont
{
public:
	AflFont();
	~AflFont();
	void setSize(int nWidth,int nHeight){m_sizeFont.cx = nWidth;m_sizeFont.cy=nHeight;}
	void setSize(int nHeight){m_sizeFont.cx = 0;m_sizeFont.cy=nHeight;}
	int getSize(){return m_sizeFont.cy;}
	void setBold(int iBold){m_iFontBold = iBold;}
	void setFontName(LPCSTR pString);
	BOOL createFont();
	void deleteFont();
	operator HFONT(){return m_hFont;}
	bool getFontSize(LPSIZE pSize,LPCSTR pText,INT iLength=-1,INT iLimitWidth=0);
	bool getFontSize(LPSIZE pSize,LPCWSTR pText,INT iLength=-1,INT iLimitWidth=0);
	static bool getFontSize(LPSIZE pSize,HFONT hFont,LPCSTR pText,INT iLength=-1,INT iLimitWidth=0);
	static bool getFontSize(LPSIZE pSize,HFONT hFont,LPCWSTR pText,INT iLength=-1,INT iLimitWidth=0);

protected:
	HFONT m_hFont;
	SIZE m_sizeFont;
	std::string m_strFontName;
	INT m_iFontBold;
};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflFep
// FEP制御用（IMM依存 - Windows専用）
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class AflFep
{
public:
    AflFep();
    ~AflFep();
    bool release();
    bool setWindow(HWND hWnd);
	bool isOpen() const;
    bool getStatus(LPDWORD pdwConversion,LPDWORD pdwSentence) const;
    bool getString(std::string& strBuff) const;
    INT getConvertPos() const;
    INT getCursorCount() const;
    INT getCountCandidate();
    INT getCandidateIndex() const;
    LPCSTR getCandidateString(INT iIndex) const;
    INT getCandidateStart() const;
    INT getCandidateEnd() const;
	bool setOpenStatus(bool bFlag);

	HWND getWindow()const{return m_hWnd;}
protected:
    HWND m_hWnd;
    LPCANDIDATELIST m_pCandidateList;
};
#endif // _WIN32

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflThreadTimer
// 時間制御ループスレッド
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class AflThreadTimer
{
public:
	AflThreadTimer();
	~AflThreadTimer();
	void start(LPVOID pvoid);
	void stop();
	DWORD getInterval();
	void setInterval(DWORD dwInterval);
	void setRender(AflClassProc* paflClassProc);
	void setStart(AflClassProc* paflClassProc);
	void setEnd(AflClassProc* paflClassProc);
	void setAction(AflClassProc* paflClassProc);
	void setIdle(AflClassProc* paflClassProc);
	bool isWait()const{return m_aflTimeCounter.isEnable();}
	void setWait(bool bFlag){m_aflTimeCounter.setEnable(bFlag);}
protected:
	void onProc(LPVOID pvoid);
private:
	AflTimeCounter m_aflTimeCounter;
	AflThread m_thread;
	volatile bool m_bEnable;
	volatile bool m_bEnter;

	AflClassProc m_aflProc;

	AflClassProc* m_paflProcStart;
	AflClassProc* m_paflProcEnd;
	AflClassProc* m_paflProcAction;
	AflClassProc* m_paflProcIdle;
	AflClassProc* m_paflProcRender;
};
//namespace
};};

#define __INC_AFLWINTOOL
#endif
