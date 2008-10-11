#pragma unmanaged

#include <windows.h>


#include "aflWinTool.h"
#include <direct.h>
#include <sys/stat.h> 
#include <stdio.h>
/*

#ifdef _MSC_VER
	#ifdef _DEBUG	//���������[�N�e�X�g
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
// ��������p
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
AflSemaphore::AflSemaphore()
{
	m_hEvent = CreateSemaphore(NULL,0,100000,NULL);
}
AflSemaphore::~AflSemaphore()
{
    CloseHandle(m_hEvent);
}
void AflSemaphore::clear()
{
	while(ReleaseSemaphore(m_hEvent,1,NULL));
}
bool AflSemaphore::release(LONG lCount)
{
	return ReleaseSemaphore(m_hEvent,lCount,NULL)!=0;
}
HANDLE AflSemaphore::getHandle()const
{
	return m_hEvent;
}
void AflSemaphore::wait()const
{
    WaitForSingleObject(m_hEvent,INFINITE);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflEvent
// ��������p
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
AflEvent::AflEvent()
{
	m_hEvent = CreateEvent(NULL,false,false,NULL);
}
AflEvent::~AflEvent()
{
    CloseHandle(m_hEvent);
}
void AflEvent::clear()
{
	ResetEvent(m_hEvent);
}
bool AflEvent::release(LONG lCount)
{
	return ReleaseSemaphore(m_hEvent,lCount,NULL)!=0;
}
HANDLE AflEvent::getHandle()const
{
	return m_hEvent;
}
void AflEvent::wait()const
{
    WaitForSingleObject(m_hEvent,INFINITE);
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflAdviseTimer
// �����Ăяo���p
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
AflAdviseTimer::AflAdviseTimer()
{
	//DirectShow�p��IReferenceClock��GUID
    static const GUID IID_IReferenceClock = {0x56a86897,0x0ad4,0x11ce,0xb0,0x3a,0x00,0x20,0xaf,0x0b,0xa7,0x70};
    static const GUID CLSID_SystemClock = {0xe436ebb1, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70};
    //COM�𗘗p�\��
	::CoInitialize(NULL);
    //DirectShow�pIReferenceClock�𗘗p�\��
	::CoCreateInstance( CLSID_SystemClock , NULL , CLSCTX_INPROC_SERVER , IID_IReferenceClock , (LPVOID*)&m_pReferenceClock);
	//�����J�E���^�̏�����
	m_dwAdviseToken = 0;
}
AflAdviseTimer::~AflAdviseTimer()
{
	stopTimer();
    //IReferenceClock�̉��
	m_pReferenceClock->Release();
	//COM���p����
    ::CoUninitialize();
}
bool AflAdviseTimer::startTimer(DWORD dwTime)
{
	stopTimer();
	//���݂̃J�E���g���J�n�J�E���^�ɐݒ�
    REFERENCE_TIME rtBase;
    m_pReferenceClock->GetTime(&rtBase);
    //�Z�}�t�H�C�x���g�̐ݒ�
	return m_pReferenceClock->AdvisePeriodic(rtBase,dwTime,(HEVENT)m_semaphore.getHandle(),&m_dwAdviseToken) == S_OK;
}
bool AflAdviseTimer::stopTimer()
{
	if(!m_dwAdviseToken)
		return false;
	//�C�x���g�̓����~
	bool bFlag = m_pReferenceClock->Unadvise(m_dwAdviseToken) == S_OK;
	m_dwAdviseToken = 0;
	return bFlag;
}
void AflAdviseTimer::waitTimer()
{
	//���Ԃ�����܂őҋ@
	m_semaphore.wait();
}
bool AflAdviseTimer::release(LONG lCount)
{
	return m_semaphore.release();
}
REFERENCE_TIME AflAdviseTimer::getTime()
{
    //���݂̃J�E���g���擾
	REFERENCE_TIME rtBase;
    m_pReferenceClock->GetTime(&rtBase);
    return rtBase;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflMediaTimer
// �����Ăяo���p
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
AflMediaTimer::AflMediaTimer()
{
	timeGetDevCaps(&m_timeCaps,sizeof(m_timeCaps));
	timeBeginPeriod(m_timeCaps.wPeriodMin);
	m_timerID = 0;
}
AflMediaTimer::~AflMediaTimer()
{
	stopTimer();
	timeEndPeriod(m_timeCaps.wPeriodMin);
}
bool AflMediaTimer::startTimer(DWORD dwTime)
{
	stopTimer();
    //�Z�}�t�H�C�x���g�̐ݒ�
	m_timerID = timeSetEvent(m_timeCaps.wPeriodMin,0,
		(LPTIMECALLBACK)m_event.getHandle(),0,TIME_PERIODIC | TIME_CALLBACK_EVENT_SET);
	return m_timerID != 0;
}
bool AflMediaTimer::stopTimer()
{
	if(!m_timerID)
		return false;
	//�C�x���g�̓����~
	bool bFlag = timeKillEvent(m_timerID) == S_OK;
	m_timerID = 0;
	return bFlag;
}
void AflMediaTimer::waitTimer()
{
	//���Ԃ�����܂őҋ@
	m_event.wait();
}
bool AflMediaTimer::release(LONG lCount)
{
	return m_event.release();
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflTimeCounter
// �i�s���x�ێ��p�J�E���^
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
AflTimeCounter::AflTimeCounter()
{
	//�^�C�}�[�̕���\�ݒ�
	TIMECAPS timeCaps;
	timeGetDevCaps(&timeCaps,sizeof(TIMECAPS));       
	timeBeginPeriod(timeCaps.wPeriodMin);

	m_bEnable = true;
	m_dwSyncTime = 10;
	resetTime();
}
AflTimeCounter::~AflTimeCounter()
{
	//�^�C�}�[�̕���\���A
	TIMECAPS timeCaps;
	timeGetDevCaps(&timeCaps,sizeof(TIMECAPS));       
	timeEndPeriod(timeCaps.wPeriodMin);
}
void AflTimeCounter::setTimeSync(DWORD dwTime)
{
	m_dwSyncTime = dwTime;
	resetTime();
}
void AflTimeCounter::resetTime()
{
	m_iNowCount = 0;
	m_dwStartTime = timeGetTime();
}
DWORD AflTimeCounter::getTime() const
{
	return timeGetTime() - m_dwStartTime;
}
INT AflTimeCounter::getCount()
{
	//�E�G�C�g�����̃X�L�b�v����
/*	if(!m_bEnable)
	{
		m_iNowCount =  getTime() / m_dwSyncTime;
		m_dwSleepTime = 0;
		return 1;
	}
*/
	//�ʏ폈��
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
	if(m_bEnable)
		Sleep(m_dwSleepTime);
	else
		Sleep(0);
}
DWORD AflTimeCounter::sleepTime() const
{
	if(m_bEnable)
		return m_dwSleepTime;
	return 0;
}void AflTimeCounter::setEnable(bool bFlag)
{
	m_bEnable = bFlag;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflFileName
// �t�@�C�����ƃp�X����
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
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


//------------------------------------------------------------
// AflRect
// �R�������W�Ǘ�
//------------------------------------------------------------

//-----------------------------------------------------
//	������
//-----------------------------------------------------
AflRect::AflRect()
{
	left=right=top=bottom=high=low=0;
}
//-----------------------------------------------------
//	������
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
//	���W�ݒ�
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
//	���W�ړ�
//-----------------------------------------------------
void AflRect::offsetRect(int nX,int nY,int nZ)
{
	left += nX; right += nX;
	top += nY; bottom += nY;
	high += nZ; low += nZ;
}
//-----------------------------------------------------
//	�S��0���ǂ����̔���
//-----------------------------------------------------
bool AflRect::isRectNull() const
{
	if(left | right | top | bottom | high | low)
		return false;
	else
		return true;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflFont
// �t�H���g�n
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//-----------------------------------------------------
AflFont::AflFont()
{
	m_hFont = 0;
	m_iFontBold = FW_SEMIBOLD;
	m_sizeFont.cy = 16;
	m_sizeFont.cx = 0;
	m_strFontName = "�l�r �S�V�b�N";
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
	m_hFont = ::CreateFont(m_sizeFont.cy,m_sizeFont.cx,0,0,m_iFontBold,FALSE,FALSE,FALSE,
		DEFAULT_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,
		m_strFontName.c_str());
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
	//�t�H���g�T�C�Y�̎擾
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
	//�t�H���g�T�C�Y�̎擾
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
// FEP����p
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
	//�R���e�L�X�g�̉��
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
		//������FEP�̔j��
		HWND hFep = ImmGetDefaultIMEWnd(hWnd);
		//SendMessage(hFep,WM_CLOSE,0,0);

	    //�E�C���h�E�Ɋ֘A�Â����Ă���FEP�̎擾
		m_hWnd = hWnd;
	}
	return true;
}
//-----------------------------------------------------
bool AflFep::isOpen() const
{
	//FEP�̊�����Ԃ��擾
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
	//FEP�̓��̓��[�h
	 HIMC hImc = ImmGetContext(m_hWnd);
	bool bRet = ImmGetConversionStatus(hImc,pdwConversion,pdwSentence)!=false;
	ImmReleaseContext(m_hWnd,hImc);
	return bRet;
}
//-----------------------------------------------------
bool AflFep::getString(std::string& strBuff) const
{
    //FEP���͕�����̎擾
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
	//�ϊ��ʒu�̎擾
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
	//�J�[�\���ʒu�̎擾
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
	//�O��g�����ϊ����̉��
    if(m_pCandidateList)
	{
        delete m_pCandidateList;
		m_pCandidateList = NULL;
	}
    //�ϊ����̎擾
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
	//���ݑI�𒆂̕ϊ����̎擾
    return m_pCandidateList->dwSelection;
}
//-----------------------------------------------------
LPCSTR AflFep::getCandidateString(INT iIndex) const
{
	//�w�肳�ꂽ�C���f�b�N�X�̕ϊ������擾
	if(!m_pCandidateList)
		return NULL;
    return (LPCSTR)((LPBYTE)m_pCandidateList+m_pCandidateList->dwOffset[iIndex]);
}
//-----------------------------------------------------
INT AflFep::getCandidateStart() const
{
	//�ϊ����C���f�b�N�X�̊J�n�ʒu�̎擾
    return m_pCandidateList->dwPageStart;
}
//-----------------------------------------------------
INT AflFep::getCandidateEnd() const
{
	//�ϊ����C���f�b�N�X�̏I���ʒu�̎擾
    int iCount = (int)(m_pCandidateList->dwPageStart + m_pCandidateList->dwPageSize);
    if(iCount > (int)m_pCandidateList->dwCount)
        iCount = m_pCandidateList->dwCount;
    return iCount;
}
//-----------------------------------------------------

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflThreadTimer
// ���Ԑ��䃋�[�v�X���b�h
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
		Sleep(1);
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
		//�K�v�����
		INT iCount = m_aflTimeCounter.getCount();
		
		//���s�����p���[�v
		if(m_bEnable && m_paflProcAction) 
		{
			for(i=0;m_bEnable && i<iCount;i++)
				m_paflProcAction->call(pvoid);
		}
		//�A�C�h�������p
		if(m_bEnable && m_paflProcIdle)
		{
			m_paflProcIdle->call(pvoid);
		}
		//�����_�����O�p
		if(m_bEnable && m_paflProcRender)
		{
			//�����_�����O�̕K�v�����m�F
			if(m_bEnable && iCount > 0 || !m_aflTimeCounter.isEnable())
			{
				m_paflProcRender->call(pvoid);
			}
		}
		if(m_bEnable)
			Sleep(m_aflTimeCounter.sleepTime());
	}

	if(m_paflProcEnd)
		m_paflProcEnd->call(pvoid);

	m_bEnter = false;
}
//namespace
};};

