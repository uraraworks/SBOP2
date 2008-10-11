/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoEffect.h											 */
/* ���e			:�G�t�F�N�g��񃉃C�u�����N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#pragma once

#include "InfoEffect.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoEffect : public CLibInfoBase
{
public:
			CLibInfoEffect();							/* �R���X�g���N�^ */
	virtual ~CLibInfoEffect();							/* �f�X�g���N�^ */

	void Create			(void);									/* �쐬 */
	void Destroy		(void);									/* �j�� */
	BOOL Proc			(void);									/* ���� */

	PCInfoBase GetNew	(void);									/* �V�K�f�[�^���擾 */

	int		GetCount	(void);									/* �f�[�^�����擾 */
	void	Add			(PCInfoBase pInfo);						/* �ǉ� */
	void	Delete		(int nNo);								/* �폜 */
	void	Delete		(DWORD dwEffectID);						/* �폜 */
	void	DeleteAll	(void);									/* �S�č폜 */
	void	GetName		(DWORD dwEffectID, CmyString &strDst);	/* �G�t�F�N�g�����擾 */

	PCInfoBase	GetPtr (int nNo);								/* �����擾 */
	PCInfoBase	GetPtr (DWORD dwEffectID);						/* �����擾 */

	DWORD	GetSendDataSize		(void);							/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);							/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);					/* ���M�f�[�^�����荞�� */


protected:
	DWORD	GetNewID	(void);									/* �V����ID���擾 */


protected:
	PARRAYEFFECT	m_paInfo;				/* �G�t�F�N�g��� */
} CLibInfoEffect, *PCLibInfoEffect;

/* Copyright(C)URARA-works 2007 */
