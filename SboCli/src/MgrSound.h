/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C�����F	MgrSound.h													 */
/* ���e�F		�T�E���h�f�[�^�Ǘ��N���X ��`�t�@�C��						 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2006/05/09													 */
/* ========================================================================= */

#pragma once

#include <dmusici.h>
#include "Audiere.h"
using namespace audiere;
#include "SboSoundData.h"

class CDXAudio;
class CLibMusicLoader;
class CLibSboSoundLoader;

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */
/* BGM */
enum {
	BGMID_NONE = 0,
	BGMID_DAICHI_S,					/* daichi_s.ogg */
	BGMID_HISYOU,					/* hisyou.ogg */
	BGMID_SUISHA,					/* suisha.ogg */
	BGMID_FAIRYTALE,				/* fairytale.ogg */
	BGMID_TABLA_IMAGE,				/* tabla_image.ogg */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CMgrSound
{
public:
			CMgrSound();						/* �R���X�g���N�^ */
	virtual ~CMgrSound();						/* �f�X�g���N�^ */

	BOOL	Create		(void);							/* �쐬 */
	void	Destroy		(void);							/* �j�� */

	void	PlaySound		(DWORD dwSoundID);			/* ���ʉ��̍Đ� */
	void	PlayBGM			(int nNo);					/* BGM�̍Đ� */
	void	StopBGM			(void);						/* BGM�̒�~ */
	void	SetBGMVolume	(int nVolume);				/* BGM���ʐݒ� */
	void	SetSEVolume		(int nVolume);				/* ���ʉ��ʐݒ� */


protected:
	void	ReadSoundData	(void);						/* ���ʉ���ǂݍ��� */


protected:
	int						m_SEVolume;					/* ���ʉ��� */
	float					m_fBGMVolume;				/* BGM���� */
	HMODULE					m_hDllSoundData;			/* ���֘ADLL�̃n���h�� */
	CDXAudio				*m_pDXAudio;				/* �T�E���h���C�u���� */
	CLibMusicLoader			*m_pLibMusicLoader;			/* MIDI�Đ����C�u���� */
	CLibSboSoundLoader		*m_pLibSboSoundLoader;		/* �T�E���h�f�[�^���C�u���� */
	IDirectMusicSegment8	**m_apDMSSound;				/* ���ʉ� */
	AudioDevicePtr			*m_pDevicePtr;				/* BGM�p�f�o�C�X */
	OutputStream			*m_pOutputStreamPtr;		/* �Đ�����X�g���[�� */
} CMgrSound, *PCMgrSound;

/* Copyright(C)URARA-works 2006 */
