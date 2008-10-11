/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapShadow.h											 */
/* ���e			:�}�b�v�e�N���X ��`�t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/04													 */
/* ========================================================================= */

#pragma once

#include "InfoAnime.h"


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoMapShadow : public CInfoBase
{
public:
			CInfoMapShadow();								/* �R���X�g���N�^ */
	virtual ~CInfoMapShadow();								/* �f�X�g���N�^ */

	int		GetElementCount		(void);								/* �v�f�����擾 */
	int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	void	Copy				(CInfoMapShadow *pSrc);				/* �R�s�[ */
	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	BOOL		TimerProc			(DWORD dwTime);					/* ���ԏ��� */
	int			GetAnimeCount		(void);							/* �A�j���[�V�����R�}�����擾 */
	void		AddAnime			(void);							/* �A�j���[�V�����R�}��ǉ� */
	void		DeleteAnime			(int nNo);						/* �A�j���[�V�����R�}���폜 */
	void		DeleteAllAnime		(void);							/* �A�j���[�V�����R�}��S�č폜 */
	PCInfoAnime	GetAnimePtr			(int nNo);						/* �A�j���[�V�����R�}���擾 */


public:
	/* �ۑ����Ȃ��f�[�^ */
	BYTE				m_byAnimeNo;						/* �A�j���[�V�����R�}�ԍ� */
	DWORD				m_dwLastAnime;						/* �Ō�ɃA�j���[�V������������ */

	/* �ۑ�����f�[�^ */
	BYTE				m_byViewType,						/* �\����� */
						m_byAnimeType,						/* �A�j���[�V������� */
						m_byAnimeCount,						/* �A�j���[�V�����R�}�� */
						m_byLevel;							/* �����x */
	WORD				m_wGrpID;							/* �O���t�B�b�NID */
	DWORD				m_dwShadowID;						/* �eID */
	POINT				m_ptViewPos;						/* �ҏW��ʂł̕\���ʒu */
	PARRAYANIMEINFO		m_paAnimeInfo;						/* �A�j���[�V������� */
} CInfoMapShadow, *PCInfoMapShadow;
typedef CmyArray<PCInfoMapShadow, PCInfoMapShadow>	  ARRAYMAPSHADOW;
typedef CmyArray<PCInfoMapShadow, PCInfoMapShadow>	*PARRAYMAPSHADOW;

/* Copyright(C)URARA-works 2007 */
