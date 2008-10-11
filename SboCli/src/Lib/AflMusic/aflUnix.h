#if _MSC_VER >= 1100
#pragma once
#endif // _MSC_VER >= 1100
#ifndef __INC_AFLUNIX

//-----------------------------------------------------
#ifdef _WIN32
	#include <mmsystem.h>
//	#include <winsock.h>
	#define THANDLE HANDLE
	typedef int socklen_t;
#else
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/time.h>
	#include <sys/socket.h>
	#include <sys/un.h>
	#include <sys/ioctl.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <pthread.h>
	#include <unistd.h>
	#include <netdb.h>


	#define Sleep(msec)	usleep(msec*1000)
	#define ZeroMemory(dest,length)	memset(dest,0,length)

	typedef struct tagRect{int left,top,right,bottom;}RECT,*LPRECT,*PRECT;
	typedef const char *LPCTSTR,*LPCSTR,*PCTSTR,*PCSTR;
	typedef char *LPTSTR,*LPSTR,*PTSTR,*PSTR;
	typedef unsigned long DWORD,*LPDWORD,*PDWORD;
	typedef unsigned short WORD,*PWORD,*LPWORD;
	typedef unsigned char BYTE,*PBYTE,*LPBYTE;
	typedef unsigned int UINT,*PUINT,*LPUINT,BOOL;
	typedef unsigned long ULONG,*PULONG,LPULONG;
	typedef unsigned short USHORT,*PUSHORT,*LPUSHORT;
	typedef char CHAR,TCHAR,*PCHAR,*LPCHAR;
	typedef unsigned char UCHAR,*PUCHAR,*LPUCHAR;
	typedef short SHORT,*PSHORT,*LPSHORT;
	typedef void *LPVOID,*PVOID;
	typedef int *PINT,INT,HANDLE;
	typedef void const* LPCVOID;
	typedef float FLOAT,*PFLOAT;
	#define THANDLE pthread_t
	
	#define TRUE 1
	#define FALSE 0

	#define SOCKET int
	#define IPPORT_SMTP 25
	#define INVALID_SOCKET  (SOCKET)(~0)
	#define SOCKET_ERROR            (-1)
	#define ioctlsocket ioctl
	#define closesocket close
	
	inline long timeGetTime()
	{
		timeval tv;
		gettimeofday(&tv,NULL);
		return tv.tv_usec;
	}

#endif

#define __INC_AFLUNIX
#endif
