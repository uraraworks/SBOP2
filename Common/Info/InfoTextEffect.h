/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoTextEffect.h											 */
/* ���e			:�����G�t�F�N�g�N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/13													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �\����� */
enum {
	TEXTEFFECTTYPE_NORMAL = 0,				/* �ʏ�\�� */
	TEXTEFFECTTYPE_WAVE,					/* 1�������E�F�[�u�\�� */
};
/* �����T�C�Y(���l�p) */
enum {
	TEXTEFFECTNUMSIZE_SMALL = 0,			/* �� */
	TEXTEFFECTNUMSIZE_MEDIUM,				/* �� */
	TEXTEFFECTNUMSIZE_LARGE,				/* �� */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoTextEffect : public CInfoBase
{
public:
			CInfoTextEffect();										/* �R���X�g���N�^ */
	virtual ~CInfoTextEffect();										/* �f�X�g���N�^ */

	BOOL	TimerProc	(DWORD dwTime);										/* ���ԏ��� */
	void	Set			(DWORD dwData, int nType, int nColor, int nSize);	/* �ݒ�(���l�f�[�^) */
	void	Set			(LPCSTR pszData, int nType, int nColor, int nSize);	/* �ݒ�(�����f�[�^) */

	int		GetGrpCount	(void);										/* �摜�����擾 */
	int		GetGrpNo	(int nNo);									/* �w��ԍ��̐��l���擾 */
	BOOL	GetDrawPos	(int nNo, POINT &ptDst, BYTE &byLevel);		/* �w��ԍ��̕\���ʒu�␳�l���擾 */
	DWORD	GetData		(void)	{ return m_dwData; }				/* ���l�f�[�^���擾 */


public:
	int				m_nType,				/* �\����� */
					m_nColor,				/* �F */
					m_nSize;				/* �T�C�Y */
	BOOL			m_bAnimeEnd;			/* �A�j���[�V�����I���H */
	BYTE			m_byAnimeNo;			/* �A�j���[�V�����R�}�ԍ� */
	BYTE			m_byAnimeCount;			/* �A�j���[�V�����R�}�� */
	DWORD			m_dwLastAnime,			/* �Ō�ɃA�j���[�V������������ */
					m_dwData;				/* �f�[�^ */
	CmyString		m_strData;				/* �f�[�^ */
} CInfoTextEffect, *PCInfoTextEffect;
typedef CmyArray<PCInfoTextEffect, PCInfoTextEffect>	  ARRAYTEXTEFFECT;
typedef CmyArray<PCInfoTextEffect, PCInfoTextEffect>	*PARRAYTEXTEFFECT;

/* Copyright(C)URARA-works 2008 */
