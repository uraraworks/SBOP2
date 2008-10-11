/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	LibMusicLoader.cpp											 */
/* ���e�F		AflMusic���C�u�����ǂݍ��݃N���X �����t�@�C��				 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/10/18													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibMusicLoader.h"

/* ========================================================================= */
/* �֐����F	CLibMusicLoader::CLibMusicLoader								 */
/* ���e�F	�R���X�g���N�^													 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

CLibMusicLoader::CLibMusicLoader()
{
	m_hLib = NULL;
	Load ();
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::~CLibMusicLoader								 */
/* ���e�F	�f�X�g���N�^													 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

CLibMusicLoader::~CLibMusicLoader()
{
	Free ();
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::Load											 */
/* ���e�F	�ǂݍ���														 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

void CLibMusicLoader::Load(void)
{
	if (m_hLib) {
		return;
	}
	m_hLib = LoadLibrary ("LibMusic.dll");
	if (m_hLib == NULL) {
		return;
	}

	/* �e�֐��̃A�h���X���擾 */
	m_pOpen1				= (LIBMUSICOpen1)				GetProcAddress (m_hLib, "Open1");
	m_pOpen2				= (LIBMUSICOpen2)				GetProcAddress (m_hLib, "Open2");
	m_pClose				= (LIBMUSICClose)				GetProcAddress (m_hLib, "Close");
	m_pFadeIn				= (LIBMUSICFadeIn)				GetProcAddress (m_hLib, "FadeIn");
	m_pFadeOut				= (LIBMUSICFadeOut)				GetProcAddress (m_hLib, "FadeOut");
	m_pPlayTime				= (LIBMUSICPlayTime)			GetProcAddress (m_hLib, "PlayTime");
	m_pPlay1				= (LIBMUSICPlay1)				GetProcAddress (m_hLib, "Play1");
	m_pPlay2				= (LIBMUSICPlay2)				GetProcAddress (m_hLib, "Play2");
	m_pCont					= (LIBMUSICCont)				GetProcAddress (m_hLib, "Cont");
	m_pStop					= (LIBMUSICStop)				GetProcAddress (m_hLib, "Stop");
	m_pIsPlay				= (LIBMUSICIsPlay)				GetProcAddress (m_hLib, "IsPlay");
	m_pSetLoop				= (LIBMUSICSetLoop)				GetProcAddress (m_hLib, "SetLoop");
	m_pSetRelativeVolume	= (LIBMUSICSetRelativeVolume)	GetProcAddress (m_hLib, "SetRelativeVolume");
	m_pGetTitle				= (LIBMUSICGetTitle)			GetProcAddress (m_hLib, "GetTitle");
	m_pGetAllTime			= (LIBMUSICGetAllTime)			GetProcAddress (m_hLib, "GetAllTime");
	m_pGetPlayTime			= (LIBMUSICGetPlayTime)			GetProcAddress (m_hLib, "GetPlayTime");

	if (m_pOpen1 == NULL) {
		Free ();
	}
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::Free											 */
/* ���e�F	�J��															 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

void CLibMusicLoader::Free(void)
{
	if (m_hLib) {
		FreeLibrary (m_hLib);
		m_pOpen1				= NULL;
		m_pOpen2				= NULL;
		m_pClose				= NULL;
		m_pFadeIn				= NULL;
		m_pFadeOut				= NULL;
		m_pPlayTime				= NULL;
		m_pPlay1				= NULL;
		m_pPlay2				= NULL;
		m_pCont					= NULL;
		m_pStop					= NULL;
		m_pIsPlay				= NULL;
		m_pSetLoop				= NULL;
		m_pSetRelativeVolume	= NULL;
		m_pGetTitle				= NULL;
		m_pGetAllTime			= NULL;
		m_pGetPlayTime			= NULL;
	}
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::Open											 */
/* ���e�F	�t�@�C������MIDI�f�[�^��ǂݍ���								 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::Open(
	LPCSTR pFileName,		/* [in] �t�@�C���� */
	LPCSTR pType)			/* [in] ���\�[�X�� */
{
	if (m_pOpen1 == NULL) {
		return FALSE;
	}
	return m_pOpen1 (pFileName, pType);
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::Open											 */
/* ���e�F	���\�[�X����MIDI�f�[�^��ǂݍ���								 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::Open(
	HMODULE hResource,		/* [in] ���\�[�X�̃��W���[���n���h�� */
	HRSRC hSrc,				/* [in] ���\�[�X�n���h�� */
	LPCSTR pType)			/* [in] ���\�[�X�� */
{
	if (m_pOpen2 == NULL) {
		return FALSE;
	}
	return m_pOpen2 (hResource, hSrc, pType);
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::Close											 */
/* ���e�F	�f�[�^������������J��											 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::Close(void)
{
	if (m_pClose == NULL) {
		return FALSE;
	}
	return m_pClose ();
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::FadeIn											 */
/* ���e�F	�t�F�[�h�C�����s��												 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::FadeIn(
	INT nTime,			/* [in] �ʏ�̉��t��ԂɂȂ�܂ł̎���(ms) */
	INT nVolume)		/* [in] �����{�����[�� */
{
	if (m_pFadeIn == NULL) {
		return FALSE;
	}
	return m_pFadeIn (nTime, nVolume);
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::FadeOut										 */
/* ���e�F	�t�F�[�h�A�E�g���s��											 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::FadeOut(
	INT nTime)		/* [in] �{�����[����0�ɂȂ�܂ł̎���(ms) */
{
	if (m_pFadeOut == NULL) {
		return FALSE;
	}
	return m_pFadeOut (nTime);
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::PlayTime										 */
/* ���e�F	�w�莞�Ԃ��炩�牉�t�J�n										 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::PlayTime(
	DWORD dwTime)		/* [in] �Đ��J�n�ʒu(ms) */
{
	if (m_pPlayTime == NULL) {
		return FALSE;
	}
	return m_pPlayTime (dwTime);
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::Play											 */
/* ���e�F	�w��ʒu���牉�t�J�n											 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::Play(
	DWORDLONG dwlCount)		/* [in] �Đ��J�n�J�E���g�ʒu */
{
	if (m_pPlay1 == NULL) {
		return FALSE;
	}
	return m_pPlay1 (dwlCount);
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::Play											 */
/* ���e�F	���t�J�n														 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::Play(void)
{
	if (m_pPlay2 == NULL) {
		return FALSE;
	}
	return m_pPlay2 ();
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::Cont											 */
/* ���e�F	���t���ĊJ														 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::Cont(void)
{
	if (m_pCont == NULL) {
		return FALSE;
	}
	return m_pCont ();
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::Stop											 */
/* ���e�F	���t��~														 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

BOOL CLibMusicLoader::Stop(void)
{
	if (m_pStop == NULL) {
		return FALSE;
	}
	return m_pStop ();
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::IsPlay											 */
/* ���e�F	���t��������													 */
/* ���t�F	2005/10/18														 */
/* �߂�l�F	TRUE:���t��														 */
/* ========================================================================= */

BOOL CLibMusicLoader::IsPlay(void)
{
	if (m_pIsPlay == NULL) {
		return FALSE;
	}
	return m_pIsPlay ();
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::SetLoop										 */
/* ���e�F	���[�v�����邩�w��												 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

void CLibMusicLoader::SetLoop(
	BOOL bLoop)		/* [in] TRUE:���[�v������ */
{
	bool bLoopTmp;

	if (m_pSetLoop == NULL) {
		return;
	}
	bLoopTmp = (bLoop) ? true : false;
	m_pSetLoop (bLoopTmp);
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::SetRelativeVolume								 */
/* ���e�F	�{�����[���̐ݒ�												 */
/* ���t�F	2005/10/23														 */
/* ========================================================================= */

void CLibMusicLoader::SetRelativeVolume(
	INT nVolume)		/* [in] �{�����[�� */
{
	if (m_pSetRelativeVolume == NULL) {
		return;
	}
	m_pSetRelativeVolume (nVolume);
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::GetTitle										 */
/* ���e�F	�^�C�g�����擾													 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

LPCSTR CLibMusicLoader::GetTitle(void)
{
	if (m_pGetTitle == NULL) {
		return NULL;
	}
	return m_pGetTitle ();
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::GetAllTime										 */
/* ���e�F	�����t���Ԃ��擾												 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

DWORDLONG CLibMusicLoader::GetAllTime(void)
{
	if (m_pGetAllTime == NULL) {
		return 0;
	}
	return m_pGetAllTime ();
}


/* ========================================================================= */
/* �֐����F	CLibMusicLoader::GetPlayTime									 */
/* ���e�F	���t���Ԃ��擾													 */
/* ���t�F	2005/10/18														 */
/* ========================================================================= */

DWORDLONG CLibMusicLoader::GetPlayTime(void)
{
	if (m_pGetPlayTime == NULL) {
		return 0;
	}
	return m_pGetPlayTime ();
}

/* Copyright(C)URARA-works 2005 */
