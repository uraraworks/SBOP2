/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoEffect.h												 */
/* ���e			:�G�t�F�N�g�N���X ��`�t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#pragma once

#include "InfoAnime.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoEffect : public CInfoBase
{
public:
			CInfoEffect();									/* �R���X�g���N�^ */
	virtual ~CInfoEffect();									/* �f�X�g���N�^ */

	int		GetElementCount		(void);								/* �v�f�����擾 */
	int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	void	Copy				(CInfoEffect *pSrc);				/* �R�s�[ */
	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	BOOL		TimerProc		(DWORD dwTime);						/* ���ԏ��� */
	int			GetAnimeCount	(void);								/* �A�j���[�V�����R�}�����擾 */
	void		AddAnime		(void);								/* �A�j���[�V�����R�}��ǉ� */
	void		DeleteAnime		(int nNo);							/* �A�j���[�V�����R�}���폜 */
	void		DeleteAllAnime	(void);								/* �A�j���[�V�����R�}��S�č폜 */
	PCInfoAnime	GetAnimePtr		(int nNo);							/* �A�j���[�V�����R�}���擾 */
	WORD		GetGrpID		(void);								/* ���݂̉摜ID���擾 */
	BYTE		GetLevel		(void);								/* ���݂̓����x���擾 */
	BOOL		IsAnimeEnd		(void);								/* �A�j���[�V�����I�������� */


public:
	/* �ۑ����Ȃ��f�[�^ */
	BOOL				m_bAnimeEnd;						/* �A�j���[�V�����I���H */
	BYTE				m_byAnimeNo;						/* �A�j���[�V�����R�}�ԍ� */
	DWORD				m_dwLastAnime;						/* �Ō�ɃA�j���[�V������������ */

	/* �ۑ�����f�[�^ */
	BOOL				m_bLoop,							/* ���[�v���� */
						m_bLoopSound;						/* ���[�v���Ɍ��ʉ����Đ����� */
	BYTE				m_byAnimeCount;						/* �A�j���[�V�����R�}�� */
	DWORD				m_dwEffectID,						/* �G�t�F�N�gID */
						m_dwSoundID,						/* ���ʉ�ID */
						m_dwGrpIDMain;						/* �摜���C��ID */
	CmyString			m_strName;							/* �G�t�F�N�g�� */
	PARRAYANIMEINFO		m_paAnimeInfo;						/* �A�j���[�V������� */
} CInfoEffect, *PCInfoEffect;
typedef CmyArray<PCInfoEffect, PCInfoEffect>	  ARRAYEFFECT;
typedef CmyArray<PCInfoEffect, PCInfoEffect>	*PARRAYEFFECT;

/* Copyright(C)URARA-works 2007 */
