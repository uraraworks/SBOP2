#if _MSC_VER >= 1100
#pragma once
#endif // _MSC_VER >= 1100
#ifndef __INC_AFLSTD

#include <string>
#include <sys/stat.h>

#include "aflUnix.h"


//namespace Afl
namespace Afl{

#define isSJIS(a) ((BYTE)a >= 0x81 && (BYTE)a <= 0x9f || (BYTE)a >= 0xe0 && (BYTE)a<=0xfc)
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflTClassProc
// クラス関数コールバック用関数テンプレート
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template<class T,DWORD (T::*pFunction)(LPVOID)>
	DWORD AflTProc(LPVOID pClass,LPVOID pVoid)
	{
		return ((T*)pClass->*pFunction)(pVoid);
	}
#define AFLPROC(a,b) (&Afl::AflTProc<a,(DWORD (a::*)(LPVOID))&a::b>)
#define AFLCLASSPROC(a,b,c) (new Afl::AflClassProc(a,AFLPROC(b,c)))


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflCall
// クラス関数呼び出し補助クラス
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class AflClassProc
{
public:
	AflClassProc();
	AflClassProc(LPVOID pClass,DWORD (*pFunction)(LPVOID,LPVOID));
	void setAddress(LPVOID pClass,DWORD (*pFunction)(LPVOID,LPVOID));
	DWORD call(LPVOID pvoid);
    bool isAddress()const;

protected:
	DWORD (*m_pFunction)(LPVOID,LPVOID);
	LPVOID m_pClass;
};



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflClassCallBack
// クラス関数コールバック用基本クラス
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
typedef class AflClassCallBack AFLCLASSCALLBACK,*PAFLCLASSCALLBACK,*LPAFLCLASSCALLBACK;
class AflClassCallBack
{
public:
	virtual ~AflClassCallBack(){}
	virtual DWORD callProcess(LPVOID pvData) = 0;
};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflClassCallBack
// クラス関数コールバック用派生テンプレート
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template<class T> class AflTClassCallBack : public AflClassCallBack
{
public:
	AflTClassCallBack();
	virtual ~AflTClassCallBack(){}
	AflTClassCallBack(T* pClass,DWORD (T::*pAddress)(LPVOID));
	void setAddress(T* pClass,DWORD (T::*pAddress)(LPVOID));
	DWORD callProcess(LPVOID pvData);
	bool isAddress()const{return m_pClass && m_pAddress;}
private:
	T* m_pClass;
	DWORD (T::*m_pAddress)(LPVOID);
};

template<class T> AflTClassCallBack<T>::AflTClassCallBack()
{
	m_pClass = NULL;
	m_pAddress = NULL;
}
template<class T> AflTClassCallBack<T>::AflTClassCallBack(T* pClass,DWORD (T::*pAddress)(LPVOID))
{
	m_pClass = pClass;
	m_pAddress = pAddress;
}
template<class T> void AflTClassCallBack<T>::setAddress(T* pClass,DWORD (T::*pAddress)(LPVOID))
{
	m_pClass = pClass;
	m_pAddress = pAddress;
}
template<class T> DWORD AflTClassCallBack<T>::callProcess(LPVOID pvData)
{
	if(m_pClass && m_pAddress)
		return (m_pClass->*m_pAddress)((LPVOID)pvData);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflCom
// COMコンポーネント自動解放用
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

class AflCom
{
public:
	AflCom()
	{
		m_lComCount = 1;
	}
	virtual ~AflCom(){}
	virtual ULONG addRef()
	{
		m_lComCount++;
		return m_lComCount;
	}
	virtual ULONG release()
	{
		m_lComCount--;
		if(m_lComCount == 0)
			delete this;
		return m_lComCount;
	}
protected:
	ULONG m_lComCount;
};

template<class T> class AflTAutoCom
{
public:
	AflTAutoCom(T* pT=NULL)
	{
		m_pClass = pT;
	}
	~AflTAutoCom()
	{
		release();
	}
	AflTAutoCom& operator=(const AflTAutoCom<T>& rhs)
	{
		release();
		m_pClass = rhs.m_pClass;
		if(m_pClass)
			m_pClass->AddRef();
		return *this;
	}

	T& operator*() const
    {
		return *m_pClass;
	}
	T* operator->()const
	{
		return m_pClass;
	}
	T* get()const{return m_pClass;}
	bool release()
	{
		if(m_pClass)
		{
			m_pClass->Release();
			m_pClass = NULL;
			return true;
		}
		return false;
	}
protected:
	T* m_pClass;
};
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// aflThread
// スレッド実行用基本クラス
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
typedef class AflThread AFLTHREAD,*PAFLTHREAD,*LPAFLTHREAD;
class AflThread
{
public:
	static THANDLE createThread(LPVOID pAddress,LPVOID pvData,LPDWORD pdwId);
	static bool closeThread(THANDLE hThread);
	AflThread();
	~AflThread();
	bool closeThread();
	bool getExitCodeThread(PDWORD pdwCode=NULL);
	bool isActiveThread();
	bool startThread(AflClassProc* paflClassCallBack,LPVOID pvData);
	bool startThread(class AflThreadProc* paflThreadProc,LPVOID pvData);
	THANDLE getThreadHandle()const{return m_hThread;}
	DWORD getThreadID()const{return m_dwThreadID;}
protected:
	static DWORD threadProcServer(LPVOID pVoid);
	DWORD threadProcRouter(LPVOID pvData);

	THANDLE m_hThread;						//スレッドハンドル
	DWORD m_dwThreadID;						//スレッドID
	volatile  DWORD m_dwExitCode;			//終了コード
	volatile  bool m_bEnable;				//状態
	AflClassProc* m_paflClassCallBack;	//メンバコールバック用
	class AflThreadProc* m_paflThreadProc;	//スレッド派生クラス用
};

class AflThreadProc
{
	friend class AflThread;
protected:
	virtual DWORD onThreadProc(LPVOID pvData){return 0;}
};
//------------------------------------------------------------
// AflSyncObject
// 同期制御用
//------------------------------------------------------------
class AflSyncObject
{
public:
	virtual ~AflSyncObject(){};
	virtual bool lock() = 0;
	virtual bool unlock() = 0;
};

//------------------------------------------------------------
// AflCritical
// 同期制御用
//------------------------------------------------------------
#ifdef _WIN32
class AflCritical : public AflSyncObject
{
public:
	AflCritical(){::InitializeCriticalSection(&m_Sect);}
	~AflCritical(){::DeleteCriticalSection(&m_Sect);}
	bool lock(){::EnterCriticalSection(&m_Sect);return TRUE;}
	bool unlock(){::LeaveCriticalSection(&m_Sect);return TRUE;}
protected:
	CRITICAL_SECTION m_Sect;
};
#else
class AflCritical : public AflSyncObject
{
public:
	AflCritical(){::pthread_mutex_init(&m_Sect,NULL);}
	~AflCritical(){::pthread_mutex_destroy(&m_Sect);}
	bool lock(){return ::pthread_mutex_lock(&m_Sect);}
	bool unlock(){return ::pthread_mutex_unlock(&m_Sect);}
protected:
	pthread_mutex_t m_Sect;
};
#endif

//------------------------------------------------------------
// AflStringHistry
// 履歴付き文字列
//------------------------------------------------------------
class AflStringHistory
{
public:
	AflStringHistory();
	~AflStringHistory(){ReleaseData();}
	bool Create(INT iCount);
	bool AddString(LPCSTR pString);
	bool MoveLast(INT iIndex);
	bool DelString(INT iIndex);
	INT GetStringIndex(LPCSTR pString);
	bool ReleaseData();
	LPCSTR GetString(INT iIndex);
	LPCSTR GetLastString(INT iIndex);

protected:
	std::string** m_ppString;
	INT m_iCount;
	INT m_iLine;
	INT m_iWriteLine;

};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflFile
// ファイル制御用
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class AflFile
{

public:
	enum
	{
		file_binary = 0,
		file_ascii= 1,
		file_in = 2,
		file_out = 4
	};
	
	AflFile();
	virtual ~AflFile();
    bool open(LPCSTR pFileName,INT iMode=file_in);
	bool close();
	INT read(LPVOID pVoid,INT iSize) const;
	INT getLength() const;
	INT tell() const;
	bool isEof() const;
	void setSeek(long offset, int origin ) const;
	LPSTR gets(LPSTR pString,INT iSize);

protected:
	std::string m_strFileName;
	LPVOID m_pFileHandle;
};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// AflStd
// 汎用
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class AflStd
{
public:
	static int GetChr(const char* pData,char Data,int nLength);
	static int GetChr(const char* pData,char Data);
	static int GetNum16(const char* pData,int nLength);
	static char* StrCnv(const char* pData,char* pString1,char* pString2);
	static char* StrCnv1(const char* pData,int nCount,char* const ppString[]);
	static LPSTR replacString(LPCSTR pData,int nCount,LPCSTR pString[]);
};
//namespace
};
#define __INC_AFLSTD
#endif
