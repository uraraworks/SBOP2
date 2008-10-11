#ifdef _WIN32
	#pragma unmanaged
	#include <windows.h>
	#include <process.h>
#endif
#include "aflStd.h"

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
		#define CHECK_MEMORY_LEAK _
		CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF);
	#else
		#define CHECK_MEMORY_LEAK
	#endif //_DEBUG
#else
		#define CHECK_MEMORY_LEAK
#endif 
*/

#include "stdio.h"

//namespace Afl
namespace Afl{

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflCall
// クラス関数呼び出し補助クラス
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
AflClassProc::AflClassProc()
{
	m_pClass = NULL;
	m_pFunction = NULL;

}
AflClassProc::AflClassProc(LPVOID pClass,DWORD (*pFunction)(LPVOID,LPVOID))
{
	setAddress(pClass,pFunction);
}
void AflClassProc::setAddress(LPVOID pClass,DWORD (*pFunction)(LPVOID,LPVOID))
{
	m_pClass = pClass;
	m_pFunction = pFunction;
}
DWORD AflClassProc::call(LPVOID pvoid)
{
	return (*m_pFunction)(m_pClass,pvoid);
}
bool AflClassProc::isAddress()const
{
	return m_pClass && m_pFunction;
}

//------------------------------------------------------------
THANDLE AflThread::createThread(LPVOID pAddress,LPVOID pvData,LPDWORD pdwId)
{
	THANDLE hThread;
	#ifdef _WIN32
		#ifdef _MT //Cランタイムスレッド
			hThread = (THANDLE)_beginthreadex(NULL,0,(unsigned(__stdcall*)(LPVOID))pAddress,pvData,0,(LPUINT)pdwId);	//スレッドの発生
		#else		//WIN32APIスレッド
			hThread = ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)pAddress,pvData,0,pdwId);				//スレッドの発生
		#endif
	#else			//POSIXスレッド
		pthread_create(&hThread,NULL,(LPVOID(*)(LPVOID))pAddress,pvData);
	#endif
	return hThread;
}
//------------------------------------------------------------
bool AflThread::closeThread(THANDLE hThread)
{
	if(hThread)
	{
	#ifdef _WIN32	//WIN32ハンドル用
		::CloseHandle(hThread);
	#else			//POSIXハンドル用
		pthread_detach(hThread);
	#endif
		return true;
	}
	return false;
}
//------------------------------------------------------------
AflThread::AflThread()
{
	m_paflClassCallBack = NULL;
	m_paflThreadProc = NULL;
	m_hThread = 0;
	m_dwThreadID = 0;
	m_dwExitCode = 0;
}
//------------------------------------------------------------
AflThread::~AflThread()
{
	closeThread();
}
//------------------------------------------------------------
bool AflThread::closeThread()
{
	bool bRet = closeThread(m_hThread);
	m_hThread = 0;
	return bRet;
}
//------------------------------------------------------------
bool AflThread::getExitCodeThread(PDWORD pdwCode)
{
	if(!m_bEnable)
	{
		*pdwCode = m_dwExitCode;
		return true;
	}
	return false;
}
//------------------------------------------------------------
bool AflThread::isActiveThread()
{
	return m_bEnable;
}
//------------------------------------------------------------
bool AflThread::startThread(AflClassProc* paflClassCallBack,LPVOID pvData)
{
	//スレッドに渡す値の設定
	LPVOID adwThreadData[] = {this,pvData};
	LPVOID pdwThreadData = new LPVOID[sizeof(adwThreadData)/sizeof(LPVOID)];
	memcpy(pdwThreadData,adwThreadData,sizeof(adwThreadData));
	
	closeThread();	//既に存在するスレッドを停止
	m_paflClassCallBack = paflClassCallBack;

	m_bEnable = true;
	//スレッドの作成
	m_hThread = createThread((LPVOID)threadProcServer,pdwThreadData,&m_dwThreadID);
    //スレッド作成後のウエイト
    while(!m_bEnable)
		Sleep(0);
	return m_hThread != 0;
}
//------------------------------------------------------------
bool AflThread::startThread(AflThreadProc* paflThreadProc,LPVOID pvData)
{
	//スレッドに渡す値の設定
	LPVOID adwThreadData[] = {this,pvData};
	LPVOID pdwThreadData = new LPVOID[sizeof(adwThreadData)/sizeof(LPVOID)];
	memcpy(pdwThreadData,adwThreadData,sizeof(adwThreadData));
	
	closeThread();	//既に存在するスレッドを停止
	m_paflThreadProc = paflThreadProc;

	m_bEnable = true;
	//スレッドの作成
	m_hThread = createThread((LPVOID)threadProcServer,pdwThreadData,&m_dwThreadID);
    //スレッド作成後のウエイト
    while(!m_bEnable)
		Sleep(0);
	return m_hThread != 0;
}
//------------------------------------------------------------
DWORD AflThread::threadProcServer(LPVOID pVoid)
{
	//スレッド初期化用データの設定
	LPVOID* ppThreadData = (LPVOID*)pVoid;
	LPAFLTHREAD pAflThread = (LPAFLTHREAD)ppThreadData[0];
	LPVOID pvData = ppThreadData[1];
	delete[] ppThreadData;
	//仮想関数の呼び出し
	pAflThread->m_dwExitCode = pAflThread->threadProcRouter(pvData);
	pAflThread->m_bEnable = false;
	return pAflThread->m_dwExitCode;
}
DWORD AflThread::threadProcRouter(LPVOID pvData)
{
	if(m_paflClassCallBack)
		return m_paflClassCallBack->call(pvData);	//ユーザクラスのメンバ関数へ
	else if(m_paflThreadProc)
		return m_paflThreadProc->onThreadProc(pvData);	//ユーザクラスのメンバ関数へ
	return 0;
}


//------------------------------------------------------------
// AflStringHistory
// 履歴付き文字列
//------------------------------------------------------------
AflStringHistory::AflStringHistory()
{
	m_iCount = 0;
	m_iLine = 0;
	m_ppString = NULL;
}
bool AflStringHistory::Create(int nCount)
{
	ReleaseData();
	m_ppString = new std::string*[nCount];		
	ZeroMemory(m_ppString,sizeof(std::string*)*nCount);
	m_iCount = nCount;
	return true;
}
bool AflStringHistory::AddString(LPCSTR pString)
{
	if(!m_ppString)
		return false;
	int iIndex;
	iIndex = GetStringIndex(pString);
	if(iIndex == -1)
	{
		DelString(0);
		m_ppString[m_iCount-1] = new std::string;
		*m_ppString[m_iCount-1] = pString;
	}
	else
	{
		MoveLast(iIndex);
	}
	return true;
}
bool AflStringHistory::MoveLast(int iIndex)
{
	if(!m_ppString)
		return false;
	int i;
	std::string* pString = m_ppString[iIndex];
	for(i=iIndex;i<m_iCount-1;i++)
		m_ppString[i] = m_ppString[i+1];
	m_ppString[i] = pString;
	return true;
}
bool AflStringHistory::DelString(int iIndex)
{
	if(!m_ppString)
		return false;
	int i;
	delete m_ppString[iIndex];
	for(i=iIndex;i<m_iCount-1;i++)
		m_ppString[i] = m_ppString[i+1];
	m_ppString[i] = NULL;
	return true;
}
int AflStringHistory::GetStringIndex(LPCSTR pString)
{
	int i;
	for(i=0;i<m_iCount;i++)
	{
		if(m_ppString[i] && strcmp((LPCSTR)m_ppString[i]->c_str(),pString) == 0)
			return i;
	}
	return -1;
}
bool AflStringHistory::ReleaseData()
{
	int i;
	if(m_ppString)
	{
		for(i=0;i<m_iCount;i++)
		{
			if(m_ppString[i])
				delete m_ppString[i];
		}
		delete[] m_ppString;
		m_ppString = NULL;
		m_iCount = 0;
	}
	return true;
}
LPCSTR AflStringHistory::GetString(int iIndex)
{
	if(!m_ppString || !m_ppString[iIndex] || iIndex >= m_iCount)
		return NULL;
	return (LPCSTR)m_ppString[iIndex]->c_str();
}
LPCSTR AflStringHistory::GetLastString(int iIndex)
{
	if(!m_ppString || iIndex<0)
		return NULL;
	int i;
	int nCount = -1;
	for(i=m_iCount-1;i>=0 && nCount!=iIndex;i--)
	{
		if(m_ppString[i])
		{
			nCount++;
		}
	}
	i++;
	if(i < 0)
		return NULL;
	if(!m_ppString[i])
		return NULL;
	return (LPCSTR)m_ppString[i]->c_str();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflFile
// ファイル制御用
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
AflFile::AflFile()
{
	m_pFileHandle = NULL;
}
AflFile::~AflFile()
{
	close();
}
bool AflFile::open(LPCSTR pFileName,INT iMode)
{
	m_strFileName = pFileName;

	close();
	static char cMode[4];
	if(iMode & file_out)
		strcpy(cMode,"w");
	else
		strcpy(cMode,"r");

	if(iMode & file_ascii)
		strcat(cMode,"t");
	else
		strcat(cMode,"b");

	m_pFileHandle = fopen(pFileName,cMode);
	return m_pFileHandle != 0;
}
bool AflFile::close()
{
	if(!m_pFileHandle)
		return false;
	return fclose((FILE*)m_pFileHandle) == 0;
}
INT AflFile::read(LPVOID pVoid,INT iSize) const
{
	if(m_pFileHandle)
		return (INT)fread(pVoid,1,iSize,(FILE*)m_pFileHandle);
	return 0;
}
INT AflFile::getLength() const
{
	return tell();
}
INT AflFile::tell() const
{
	struct stat statFile; 
	fstat(fileno((FILE*)m_pFileHandle),&statFile);
	return statFile.st_size;
}
bool AflFile::isEof() const
{
	return feof((FILE*)m_pFileHandle)!=0;
}
void AflFile::setSeek(long offset, int origin ) const
{
	fseek((FILE*)m_pFileHandle,offset,origin);
}
LPSTR AflFile::gets(LPSTR pString,INT iSize)
{
	return fgets(pString,iSize,(FILE*)m_pFileHandle);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflStd
// 汎用
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

int AflStd::GetChr(const char* pData,char Data,int nLength)
{
	int i,j;
	for(i=0,j=0;i<nLength;i++)
	{
		if(pData[i] == Data)
			j++;
	}
	return j;
}
int AflStd::GetChr(const char* pData,char Data)
{
	int i,j;
	for(i=0,j=0;pData[i] != 0;i++)
	{
		if(pData[i] == Data)
			j++;
	}
	return j;
}
int AflStd::GetNum16(const char* pData,int nLength)
{
	int nData = 0;
	for(int i=0;i < nLength;i++)
	{
		nData *= 16;
		if(pData[i] >= '0' && pData[i] <= '9')
		{
			nData += (int)pData[i] - '0';
		}
		else if(pData[i] >= 'A' && pData[i] <= 'F')
		{
			nData += (int)pData[i] - 'A' + 10;
		}
		else if(pData[i] >= 'a' && pData[i] <= 'f')
		{
			nData += (int)pData[i] - 'a' + 10;
		}
	}
	return nData;
}
char* AflStd::StrCnv(const char* pData,char* pString1,char* pString2)
{
	int i,j;
	const char* pBuff;
	int nLength1 = (int)strlen(pString1);
	int nLength2 = (int)strlen(pString2);
	pBuff=pData;
	for(i=0;;i++)
	{
		pBuff = strstr(pBuff,pString1);
		if(pBuff == NULL)
			break;
		pBuff++;
	}
	char* pDest = new char[1 + strlen(pData) + (nLength2 - nLength1) * i];

	pBuff=pData;
	for(i=0,j=0;pData[i] != 0;)
	{
		pBuff = strstr(pBuff,pString1);
		if(pBuff != NULL)
		{
			pBuff++;
		}
		for(;&pData[i+1] != pBuff && pData[i] != 0;i++,j++)
			pDest[j] = pData[i];
		if(pBuff != NULL)
		{
			strncpy(&pDest[j],pString2,nLength2);
			i+=nLength1;
			j+=nLength2;
		}
	}
	pDest[j] = 0;
	return pDest;
}
LPSTR AflStd::replacString(LPCSTR pData,int nCount,LPCSTR pString[])
{
	int i,j,k,l;
	int* pnCount = new int[nCount];
	const char* pBuff = pData;
	
	for(j=0;j<nCount;j++)
	{
		pnCount[j] = 0;
	}
	for(i=0;;i++)
	{
		char* pBuff2 = 0;
		for(l=0,k=0;k<nCount;k++)
		{
			char* pBuff3 = strstr(pBuff,pString[k*2]);
			if((pBuff3 < pBuff2 || pBuff2 == 0) && pBuff3 != 0)
			{
				pBuff2 = pBuff3;
				l = k;  	
			}
		}
		if(pBuff2==0)
			break;
		pBuff = pBuff2;
		++pnCount[l];
		pBuff+=strlen(pString[l*2]);
	}
	
	int nLength=0;
	for(j=0;j<nCount;j++)
	{
		nLength += ((int)strlen(pString[j*2+1]) - (int)strlen(pString[j*2])) * pnCount[j];
	}
	char* pDest = new char[1 + strlen(pData) + nLength];
	pBuff=pData;

	for(i=0,j=0;pData[i] != 0;)
	{
		char* pBuff2 = 0;
		for(l=0,k=0;k<nCount;k++)
		{
			char* pBuff3 = strstr(pBuff,pString[k*2]);
			if((pBuff3 < pBuff2 || pBuff2 == 0) && pBuff3 != 0)
			{
				pBuff2 = pBuff3;
				l = k;
			}
		}
		k=l;
		pBuff = pBuff2;

		for(;&pData[i] != pBuff && pData[i] != 0;i++,j++)
			pDest[j] = pData[i];
		if(pBuff != NULL)
		{
			pBuff += strlen(pString[k*2]);
			strcpy(&pDest[j],pString[k*2+1]);
			i+=(int)strlen(pString[k*2]);
			j+=(int)strlen(pString[k*2+1]);
		}
	}
	pDest[j] = 0;
	delete pnCount;
	return pDest;
}
//namespace
};
