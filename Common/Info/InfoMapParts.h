/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapParts.h												 */
/* ���e			:�}�b�v�p�[�c�N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/04/25													 */
/* ========================================================================= */

#pragma once

#include "InfoAnime.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

#define BIT_PARTSHIT_BLOCK		(0x00000001)		/* �Ԃ��� */
#define BIT_PARTSHIT_PILE		(0x00000002)		/* �d�Ȃ� */
#define BIT_PARTSHIT_PILEBACK	(0x00000004)		/* ���n���d�˂� */
#define BIT_PARTSHIT_FISHING	(0x00000008)		/* �ނ�� */

#define BIT_PARTSBLOCK_UP		(0x01)				/* ��ɂԂ��� */
#define BIT_PARTSBLOCK_DOWN		(0x02)				/* ���ɂԂ��� */
#define BIT_PARTSBLOCK_LEFT		(0x04)				/* ���ɂԂ��� */
#define BIT_PARTSBLOCK_RIGHT	(0x08)				/* �E�ɂԂ��� */

#define BIT_PARTSMOVE_UP		(0x01)				/* ��Ɉړ����� */
#define BIT_PARTSMOVE_DOWN		(0x02)				/* ���Ɉړ����� */
#define BIT_PARTSMOVE_LEFT		(0x04)				/* ���Ɉړ����� */
#define BIT_PARTSMOVE_RIGHT		(0x08)				/* �E�Ɉړ����� */


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoMapParts : public CInfoBase
{
public:
			CInfoMapParts();								/* �R���X�g���N�^ */
	virtual ~CInfoMapParts();								/* �f�X�g���N�^ */

	int		GetElementCount		(void);								/* �v�f�����擾 */
	int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	void	Copy				(CInfoMapParts *pSrc);				/* �R�s�[ */
	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	BOOL		IsBlock				(int nDirection);				/* �Ԃ��蔻�� */
	BOOL		TimerProc			(DWORD dwTime);					/* ���ԏ��� */
	int			GetMoveDirection	(void);							/* �ړ�����������擾 */
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
						m_byLevel,							/* �����x */
						m_byBlockDirection,					/* �����ɂ��Ԃ��蔻�� */
						m_byMoveDirection;					/* ���ƈړ�������� */
	WORD				m_wGrpIDBase,						/* �O���t�B�b�NID(���n) */
						m_wGrpIDPile;						/* �O���t�B�b�NID(�d�ˍ��킹�pID) */
	DWORD				m_dwPartsID,						/* �p�[�cID */
						m_dwPartsType;						/* �p�[�c�̑��� */
	POINT				m_ptViewPos;						/* �ҏW��ʂł̕\���ʒu */
	PARRAYANIMEINFO		m_paAnimeInfo;						/* �A�j���[�V������� */
} CInfoMapParts, *PCInfoMapParts;
typedef CmyArray<PCInfoMapParts, PCInfoMapParts>	  ARRAYMAPPARTS;
typedef CmyArray<PCInfoMapParts, PCInfoMapParts>	*PARRAYMAPPARTS;

/* Copyright(C)URARA-works 2005 */
