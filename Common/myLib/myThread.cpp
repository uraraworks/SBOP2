/* Copyright(C)URARA-works 2007 */

/* ========================================================================= */
/* �t�@�C����	:myThread.cpp												 */
/* ���e			:�X���b�h�N���X �����t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/28													 */
/* ========================================================================= */

#include "stdafx.h"
#include <process.h>
#include "myThread.h"


/* ========================================================================= */
/* �֐���	:CmyString::CmyString											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

CmyThread::CmyThread()
{
	m_dwInitResult	= 0;
	m_hThread		= NULL;
	m_hInitEvent	= NULL;
	m_hExitEvent	= NULL;
}


/* ========================================================================= */
/* �֐���	:CmyThread::~CmyThread											 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

CmyThread::~CmyThread()
{
	Destroy();
}


/* ========================================================================= */
/* �֐���	:CmyThread::Create												 */
/* ���e		:�쐬															 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

BOOL CmyThread::Create(void)
{
	UINT threadID;

	if (m_hThread) {
		return FALSE;
	}

	/* �C�x���g���쐬 */
	m_hInitEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
	if (m_hInitEvent == NULL) {
		Destroy ();
		return FALSE;
	}
	m_hExitEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
	if (m_hExitEvent == NULL) {
		Destroy ();
		return FALSE;
	}

	/* �X���b�h���N�� */
	threadID = 0;
	m_hThread = (HANDLE)_beginthreadex (NULL, 0, ThreadEntry, (PVOID)this, 0, &threadID);
	if (m_hThread == NULL) {
		Destroy ();
		return FALSE;
	}

	/* �����������҂� */
	WaitForSingleObject (m_hInitEvent, INFINITE);
	if (m_dwInitResult) {
		Destroy ();
		return FALSE;
	}
	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CmyThread::Destroy												 */
/* ���e		:�j��															 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CmyThread::Destroy(void)
{
	/* �X���b�h���~ */
	if (m_hThread) {
		SetEvent (m_hExitEvent);
		WaitForSingleObject (m_hThread, INFINITE);
	}

	/* �C�x���g��j�� */
	if (m_hExitEvent) {
		CloseHandle (m_hExitEvent);
	}
	if (m_hInitEvent) {
		CloseHandle (m_hInitEvent);
	}
	if (m_hThread) {
		CloseHandle (m_hThread);
	}

	m_dwInitResult	= 0;
	m_hThread		= NULL;
	m_hInitEvent	= NULL;
	m_hExitEvent	= NULL;
}


/* ========================================================================= */
/* �֐���	:CmyThread::ThreadEntry											 */
/* ���e		:�G���g���|�C���g												 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

UINT __stdcall CmyThread::ThreadEntry(LPVOID lpParam)
{
	CmyThread* pThis = (CmyThread*) lpParam;

	pThis->m_dwInitResult = pThis->ThreadInit ();
	SetEvent (pThis->m_hInitEvent);

	if (pThis->m_dwInitResult == 0) {
		pThis->ThreadMain ();
	}

	pThis->ThreadExit ();
	return 0;
}


/* ========================================================================= */
/* �֐���	:CmyThread::ThreadInit											 */
/* ���e		:������															 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

DWORD CmyThread::ThreadInit(void)
{
	return 0;
}


/* ========================================================================= */
/* �֐���	:CmyThread::ThreadMain											 */
/* ���e		:���C������														 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CmyThread::ThreadMain(void)
{
	WaitForSingleObject (m_hExitEvent, INFINITE);
}


/* ========================================================================= */
/* �֐���	:CmyThread::ThreadExit											 */
/* ���e		:�I������														 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CmyThread::ThreadExit(void)
{
}

/* Copyright(C)URARA-works 2007 */
