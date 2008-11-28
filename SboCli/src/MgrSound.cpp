/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrSound.cpp												 */
/* ���e			:�T�E���h�f�[�^�Ǘ��N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/05/09													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "resource.h"
#include "DXAudio.h"
#include "LibMusicLoader.h"
#include "LibSboSoundLoader.h"
#include "MgrSound.h"

#pragma comment (lib, "src/lib/Audiere/audiere.lib")

/* ========================================================================= */
/* �֐���	:CMgrSound::CMgrSound											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/05/09														 */
/* ========================================================================= */

CMgrSound::CMgrSound()
{
	m_dwSoundID				= 0;
	m_SEVolume				= -7;
	m_fBGMVolume			= 0.50f;
	m_hDllSoundData			= NULL;
	m_pDXAudio				= new CDXAudio;
	m_pLibMusicLoader		= new CLibMusicLoader;
	m_pLibSboSoundLoader	= new CLibSboSoundLoader;
	m_apDMSSound			= NULL;
	m_pDevicePtr			= NULL;
	m_pOutputStreamPtr		= NULL;
}


/* ========================================================================= */
/* �֐���	:CMgrSound::~CMgrSound											 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/05/09														 */
/* ========================================================================= */

CMgrSound::~CMgrSound()
{
	Destroy ();

	SAFE_DELETE (m_pDXAudio);
	SAFE_DELETE (m_pLibMusicLoader);
	SAFE_DELETE (m_pLibSboSoundLoader);
	SAFE_DELETE_ARRAY (m_apDMSSound);
}


/* ========================================================================= */
/* �֐���	:CMgrSound::Create												 */
/* ���e		:�쐬															 */
/* ���t		:2006/05/09														 */
/* ========================================================================= */

BOOL CMgrSound::Create(void)
{
	BOOL bRet, bResult;
	char szPath[MAX_PATH];
	CmyString strFileName;

	bRet = FALSE;

	GetModuleFilePath (szPath, sizeof (szPath));
	strFileName.Format ("%sSboSoundData.dll", szPath);

	bResult = m_pDXAudio->Create ();
	if (bResult == FALSE) {
		goto Exit;
	}
	m_hDllSoundData = LoadLibrary (strFileName);
	m_pDXAudio->SetResourceHandle (m_hDllSoundData);

	m_pLibMusicLoader->		Load ();
	m_pLibSboSoundLoader->	Load ();

	m_pDevicePtr = new AudioDevicePtr(OpenDevice ());

	/* ���ʉ���ǂݍ��� */
	ReadSoundData ();

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CMgrSound::Destroy												 */
/* ���e		:�j��															 */
/* ���t		:2006/05/09														 */
/* ========================================================================= */

void CMgrSound::Destroy(void)
{
	if (m_pDXAudio) {
		m_pDXAudio->Destroy ();
	}
	if (m_pLibMusicLoader) {
		m_pLibMusicLoader->Free ();
	}
	SAFE_DELETE (m_pDevicePtr);
}


/* ========================================================================= */
/* �֐���	:CMgrSound::PlaySound											 */
/* ���e		:���ʉ��̍Đ�													 */
/* ���t		:2006/05/09														 */
/* ========================================================================= */

void CMgrSound::PlaySound(DWORD dwSoundID)
{
	int nNo;
	IDirectMusicSegment8 **pDMS;

	nNo = m_pLibSboSoundLoader->GetSoundNo (dwSoundID);
	if (nNo < 0) {
		return;
	}
	m_pDXAudio->SetVolSecoundary (m_SEVolume);

	pDMS = &m_apDMSSound[nNo];
	if (pDMS == NULL) {
		return;
	}
	m_pDXAudio->PlaySecoundary (*pDMS);
}


/* ========================================================================= */
/* �֐���	:CMgrSound::PlayBGM												 */
/* ���e		:BGM�̍Đ�														 */
/* ���t		:2006/05/09														 */
/* ========================================================================= */

void CMgrSound::PlayBGM(
	int nNo,		/* [in] BGMID */
	BOOL bPlay)		/* [in] ���ɓ���ID��BGM���Đ����̎��͂��̂܂܂ɂ��Ă��� */
{
	char szTmp[MAX_PATH];
	OutputStream *pOutputStream;

	if (bPlay) {
		if (m_dwSoundID == (DWORD)nNo) {
			return;
		}
	}

	GetModuleFilePath (szTmp, sizeof (szTmp));
	strcat (szTmp, "BGM\\");
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
		strcat (szTmp, "daichi_s.ogg");
		break;
	case BGMID_HISYOU:
		strcat (szTmp, "hisyou.ogg");
		break;
	case BGMID_SUISHA:
		strcat (szTmp, "suisha.ogg");
		break;
	case BGMID_FAIRYTALE:
		strcat (szTmp, "fairytale.ogg");
		break;
	case BGMID_TABLA_IMAGE:
		strcat (szTmp, "tabla_image.ogg");
		break;
	case BGMID_FLOWED_PIANO:
		strcat (szTmp, "flowed piano.ogg");
		break;
	case BGMID_HUYUNOMATI_FULL:
		strcat (szTmp, "huyunomati_full.ogg");
		break;
	case BGMID_OYAKODON_NAMI:
		strcat (szTmp, "oyakodon_nami.ogg");
		break;
	}
	m_dwSoundID = (DWORD)nNo;

	if (m_pOutputStreamPtr) {
		m_pOutputStreamPtr->stop ();
		m_pOutputStreamPtr = NULL;
	}
	pOutputStream = OpenSound (*m_pDevicePtr, szTmp, true);
	if (pOutputStream == NULL) {
		return;
	}
	m_pOutputStreamPtr = pOutputStream;
	m_pOutputStreamPtr->setRepeat (true);
	m_pOutputStreamPtr->setVolume(m_fBGMVolume);
	m_pOutputStreamPtr->play ();
}


/* ========================================================================= */
/* �֐���	:CMgrSound::StopBGM												 */
/* ���e		:BGM�̒�~														 */
/* ���t		:2006/05/09														 */
/* ========================================================================= */

void CMgrSound::StopBGM(void)
{
	m_dwSoundID = 0;

	if (m_pOutputStreamPtr == NULL) {
		return;
	}
	m_pOutputStreamPtr->stop ();
	m_pOutputStreamPtr = NULL;
}


/* ========================================================================= */
/* �֐���	:CMgrSound::SetBGMVolume										 */
/* ���e		:BGM���ʐݒ�													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

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
	if (m_pOutputStreamPtr) {
		m_pOutputStreamPtr->setVolume(m_fBGMVolume);
	}
}


/* ========================================================================= */
/* �֐���	:CMgrSound::SetSEVolume											 */
/* ���e		:���ʉ��ʐݒ�													 */
/* ���t		:2007/05/11														 */
/* ========================================================================= */

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


/* ========================================================================= */
/* �֐���	:CMgrSound::ReadSoundData										 */
/* ���e		:���ʉ���ǂݍ���												 */
/* ���t		:2007/05/03														 */
/* ========================================================================= */

void CMgrSound::ReadSoundData(void)
{
	int i, nCount, nResourceID;

	nCount = m_pLibSboSoundLoader->GetSoundCount ();

	SAFE_DELETE_ARRAY (m_apDMSSound);
	m_apDMSSound = new IDirectMusicSegment8*[nCount];

	for (i = 0; i < nCount; i ++) {
		nResourceID = m_pLibSboSoundLoader->GetSoundResourceID (i);
		if (nResourceID == 0) {
			continue;
		}
		m_pDXAudio->GetSegFromRes (
				FindResource (m_hDllSoundData, MAKEINTRESOURCE (nResourceID), "WAVE"),
				&m_apDMSSound[i]);
	}
}

/* Copyright(C)URARA-works 2006 */
