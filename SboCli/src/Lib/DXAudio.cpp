/* Copyright(C)URARA-works 2003 */
/* ========================================================================= */
/* �t�@�C�����F	DXAudio.cpp													 */
/* ���e�F		DirectX Audio���g�����߂̃N���X								 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2003/03/16													 */
/* ========================================================================= */

#include "stdafx.h"
#include <windowsx.h>

#include "DXAudio.h"

#pragma comment (lib, "dxguid.lib")

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

#define SAFE_RELEASE(x)	{if(x){(x)->Release();x=NULL;}}


/* ========================================================================= */
/* �֐����F	CDXAudio::CDXAudio												 */
/* ���e�F	�R���X�g���N�^													 */
/* ���t�F	2003/03/16														 */
/* ========================================================================= */

CDXAudio::CDXAudio()
{
	m_pPerformance		= NULL;
	m_pDefAudioPath		= NULL;
	m_pDefAudioPath2	= NULL;
	m_pLoader			= NULL;
	m_hResource			= NULL;
}


/* ========================================================================= */
/* �֐����F	CDXAudio::~CDXAudio												 */
/* ���e�F	�f�X�g���N�^													 */
/* ���t�F	2003/03/16														 */
/* ========================================================================= */

CDXAudio::~CDXAudio()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐����F	CDXAudio::Create												 */
/* ���e�F	������															 */
/* ���t�F	2003/03/16														 */
/* ========================================================================= */

BOOL CDXAudio::Create(void)
{
	HRESULT hr;

	/* �p�t�H�[�}���X�̍쐬 */
	hr = CoCreateInstance (
				CLSID_DirectMusicPerformance,
				NULL,
				CLSCTX_INPROC,
				IID_IDirectMusicPerformance8,
				(PVOID *)&m_pPerformance);
	if (FAILED (hr)) {
		return FALSE;
	}

	/* ������ */
	m_pPerformance->InitAudio (
			NULL, NULL, NULL,
			DMUS_APATH_DYNAMIC_STEREO,
			64,
			DMUS_AUDIOF_ALL,
			NULL);
	if (FAILED (hr)) {
		return FALSE;
	}

	/* �f�t�H���g�̃I�[�f�B�I�p�X���擾 */
	m_pPerformance->GetDefaultAudioPath (&m_pDefAudioPath);
	if (FAILED (hr)) {
		return FALSE;
	}
	m_pPerformance->CreateStandardAudioPath (DMUS_APATH_DYNAMIC_STEREO, 64, TRUE, &m_pDefAudioPath2);

	hr = CoCreateInstance (
				CLSID_DirectMusicLoader,
				NULL,
				CLSCTX_INPROC,
				IID_IDirectMusicLoader8,
				(PVOID *)&m_pLoader);
	if (FAILED (hr)) {
		return FALSE;
	}

	return TRUE;
}


/* ========================================================================= */
/* �֐����F	CDXAudio::Destroy												 */
/* ���e�F	�I������														 */
/* ���t�F	2003/03/16														 */
/* ========================================================================= */

void CDXAudio::Destroy(void)
{
	if (m_pPerformance) {
		m_pPerformance->Stop (NULL, NULL, 0, 0);
		m_pPerformance->CloseDown ();
	}
	SAFE_RELEASE (m_pDefAudioPath);
	SAFE_RELEASE (m_pDefAudioPath2);
	SAFE_RELEASE (m_pPerformance);
	SAFE_RELEASE (m_pLoader);
}


/* ========================================================================= */
/* �֐����F	CDXAudio::SetResourceHandle										 */
/* ���e�F	���\�[�X�n���h���̐ݒ�											 */
/* ���t�F	2003/09/01														 */
/* ========================================================================= */

void CDXAudio::SetResourceHandle(HMODULE hResource)
{
	m_hResource = hResource;
}


/* ========================================================================= */
/* �֐����F	CDXAudio::GetSegFromRes											 */
/* ���e�F	���\�[�X����̃Z�O�����g�ǂݍ���								 */
/* ���t�F	2003/03/16														 */
/* ========================================================================= */

BOOL CDXAudio::GetSegFromRes(HRSRC hSrc, IDirectMusicSegment8 **pSeg, BOOL bMidi)
{
	HRESULT hr;
	DMUS_OBJECTDESC ObjDesc;
	IDirectMusicSegment8 *pTmp;

	/* ���\�[�X��ǂݍ��� */
	HGLOBAL hRes = LoadResource (m_hResource, hSrc);

	/* �\���̂�ݒ� */
	ObjDesc.dwSize		= sizeof (DMUS_OBJECTDESC);
	ObjDesc.guidClass	= CLSID_DirectMusicSegment;
	ObjDesc.dwValidData	= DMUS_OBJ_CLASS | DMUS_OBJ_MEMORY;
	ObjDesc.pbMemData	= (BYTE *)LockResource (hRes);
	ObjDesc.llMemLength	= SizeofResource (m_hResource, hSrc);

	/* �Z�O�����g��ǂݍ��� */
	pTmp = *pSeg;

	hr = m_pLoader->GetObject (&ObjDesc, IID_IDirectMusicSegment8, (void **)&pTmp);
	if (FAILED (hr)) {
		return FALSE;
	}

	if (bMidi) {
		pTmp->SetParam (GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL);
	}

	/* �o���h���_�E�����[�h */
	hr = pTmp->Download (m_pPerformance);
	if (FAILED (hr)) {
		return FALSE;
	}

	*pSeg = pTmp;
	return TRUE;
}


/* ========================================================================= */
/* �֐����F	CDXAudio::ReleaseSeg											 */
/* ���e�F	�Z�O�����g�J��													 */
/* ���t�F	2003/03/16														 */
/* ========================================================================= */

void CDXAudio::ReleaseSeg(IDirectMusicSegment8 *pSeg)
{
	if (pSeg == NULL) {
		return;
	}

	pSeg->Unload (m_pPerformance);
	SAFE_RELEASE (pSeg);
}


/* ========================================================================= */
/* �֐����F	CDXAudio::PlayPrimary											 */
/* ���e�F	�v���C�}���Z�O�����g�Ƃ��čĐ�									 */
/* ���t�F	2003/07/13														 */
/* ========================================================================= */

BOOL CDXAudio::PlayPrimary(IDirectMusicSegment8 *pSeg)
{
	BOOL bRet;
	HRESULT hr;

	bRet = FALSE;
	if (m_pPerformance == NULL) {
		goto Exit;
	}

	hr = m_pPerformance->PlaySegmentEx (
				pSeg,
				NULL, NULL, 0, 0, NULL, NULL,
				m_pDefAudioPath2);
	if (FAILED (hr)) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐����F	CDXAudio::PlaySecoundary										 */
/* ���e�F	�Z�J���_���Z�O�����g�Ƃ��čĐ�									 */
/* ���t�F	2003/03/16														 */
/* ========================================================================= */

BOOL CDXAudio::PlaySecoundary(IDirectMusicSegment8 *pSeg)
{
	BOOL bRet;
	HRESULT hr;

	bRet = FALSE;
	if (m_pPerformance == NULL) {
		goto Exit;
	}
	if (m_pDefAudioPath == NULL) {
		goto Exit;
	}

	hr = m_pPerformance->PlaySegmentEx (
				pSeg,
				NULL, NULL,
				DMUS_SEGF_SECONDARY,
				0, NULL, NULL,
				m_pDefAudioPath);
	if (FAILED (hr)) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐����F	CDXAudio::SetVolPrimary											 */
/* ���e�F	�v���C�}���Z�O�����g�̉��ʐݒ�									 */
/* ���t�F	2003/11/30														 */
/* ========================================================================= */

void CDXAudio::SetVolPrimary(long lVol)
{
	if (m_pDefAudioPath2 == NULL) {
		return;
	}
	lVol = max (lVol, -96);
	lVol = min (lVol, 0);
	lVol *= 100;
	m_pDefAudioPath2->SetVolume (lVol, 0);
}


/* ========================================================================= */
/* �֐����F	CDXAudio::SetVolSecoundary										 */
/* ���e�F	�Z�J���_���Z�O�����g�̉��ʐݒ�									 */
/* ���t�F	2003/11/30														 */
/* ========================================================================= */

void CDXAudio::SetVolSecoundary(long lVol)
{
	if (m_pDefAudioPath == NULL) {
		return;
	}
	lVol = max (lVol, -100);
	lVol = min (lVol, 10);
	lVol *= 100;
	m_pDefAudioPath->SetVolume (lVol, 0);
}


/* ========================================================================= */
/* �֐����F	CDXAudio::Stop													 */
/* ���e�F	�Đ���~														 */
/* ���t�F	2003/03/16														 */
/* ========================================================================= */

void CDXAudio::Stop(IDirectMusicSegment8 *pSeg, DWORD dwFlg)
{
	if (m_pPerformance == NULL) {
		return;
	}

	if (dwFlg == 1) {
		m_pPerformance->StopEx (m_pDefAudioPath2, 0, 0);
		dwFlg = 0;
	}
	/* �Đ���~ */
	m_pPerformance->StopEx (pSeg, 0, dwFlg);
}


/* ========================================================================= */
/* �֐����F	CDXAudio::SetLoopPoints											 */
/* ���e�F	���[�v�͈͂̐ݒ�												 */
/* ���t�F	2003/03/22														 */
/* ========================================================================= */

void CDXAudio::SetLoopPoints(IDirectMusicSegment8 *pSeg, DWORD dwFlg)
{
	if (pSeg == NULL) {
		return;
	}

	pSeg->SetLoopPoints (0, 0);
	pSeg->SetRepeats (dwFlg);
}


/* ========================================================================= */
/* �֐����F	CDXAudio::IsPlaying												 */
/* ���e�F	�Đ�������														 */
/* ���t�F	2003/05/05														 */
/* ========================================================================= */

BOOL CDXAudio::IsPlaying(IDirectMusicSegment8 *pSeg)
{
	BOOL bRet;

	bRet = FALSE;
	if (pSeg == NULL) {
		goto Exit;
	}

	if (m_pPerformance->IsPlaying (pSeg, 0) != S_OK) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2003 */
