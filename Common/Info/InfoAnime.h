/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:InfoAnime.h												 */
/* ���e			:�A�j���[�V�������N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/04/29													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

#define PREFIX_INFOANIME		"pAnime_"					/* �v�f���̃v���t�B�b�N�X */


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoAnime : public CInfoBase
{
public:
			CInfoAnime();									/* �R���X�g���N�^ */
	virtual ~CInfoAnime();									/* �f�X�g���N�^ */

	int		GetElementCount		(void);								/* �v�f�����擾 */
	int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	void	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */


public:
	int			m_nElementCount;							/* �v�f�� */

	/* �ۑ����Ȃ��f�[�^ */

	/* �ۑ�����f�[�^ */
	BYTE		m_byWait,									/* �҂�����(�~�P�O�~���b) */
				m_byLevel;									/* �����x */
	WORD		m_wGrpIDBase,								/* �O���t�B�b�NID(���n) */
				m_wGrpIDPile;								/* �O���t�B�b�NID(�d�ˍ��킹�pID) */
} CInfoAnime, *PCInfoAnime;
typedef CmyArray<PCInfoAnime, PCInfoAnime>	   ARRAYANIMEINFO;
typedef CmyArray<PCInfoAnime, PCInfoAnime>	 *PARRAYANIMEINFO;

/* Copyright(C)URARA-works 2005 */
