/* Copyright(C)URARA-works 2003-2005 */
/* ========================================================================= */
/* �t�@�C�����F	DXAudio.h													 */
/* ���e�F		DirectX Audio���g�����߂̃N���X								 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2003/03/16													 */
/* ========================================================================= */

#pragma once

#include <dmusici.h>

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDXAudio
{
public:
			CDXAudio	();										/* �R���X�g���N�^ */
	virtual	~CDXAudio	();										/* �f�X�g���N�^ */

	BOOL Create				(void);										/* ������ */
	void Destroy			(void);										/* �I�� */
	void SetResourceHandle	(HMODULE hResource);						/* ���\�[�X�n���h���̐ݒ� */

	BOOL GetSegFromRes		(HRSRC hSrc, IDirectMusicSegment8 **pSeg, BOOL bMidi = FALSE);	/* ���\�[�X����̃Z�O�����g�ǂݍ��� */
	void ReleaseSeg			(IDirectMusicSegment8 *pSeg);				/* �Z�O�����g�̊J�� */
	BOOL PlayPrimary		(IDirectMusicSegment8 *pSeg);				/* �v���C�}���Z�O�����g�Ƃ��čĐ� */
	BOOL PlaySecoundary		(IDirectMusicSegment8 *pSeg);				/* �Z�J���_���Z�O�����g�Ƃ��čĐ� */
	void SetVolPrimary		(long lVol);								/* �v���C�}���Z�O�����g�̉��ʐݒ� */
	void SetVolSecoundary	(long lVol);								/* �Z�J���_���Z�O�����g�̉��ʐݒ� */
	void Stop				(IDirectMusicSegment8 *pSeg, DWORD dwFlg);	/* �Đ���~ */
	void SetLoopPoints		(IDirectMusicSegment8 *pSeg, DWORD dwFlg);	/* ���[�v�͈͂̐ݒ� */
	BOOL IsPlaying			(IDirectMusicSegment8 *pSeg);				/* �Đ������� */


private:
	IDirectMusicPerformance8	*m_pPerformance;	/* �p�t�H�[�}���X�I�u�W�F�N�g */
	IDirectMusicAudioPath8		*m_pDefAudioPath;	/* �I�[�f�B�I�p�X */
	IDirectMusicAudioPath8		*m_pDefAudioPath2;	/* �I�[�f�B�I�p�X2 */
	IDirectMusicLoader8			*m_pLoader;			/* ���[�_�[ */
	HMODULE						m_hResource;		/* ���\�[�X�n���h�� */
} CDXAudio, *PCDXAudio;

/* Copyright(C)URARA-works 2003 */
