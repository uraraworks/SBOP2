/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMapShadow.h											 */
/* ���e			:�}�b�v�e��񃉃C�u�����N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/05													 */
/* ========================================================================= */

#pragma once

#include "InfoMapShadow.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoMapShadow : public CLibInfoBase
{
public:
			CLibInfoMapShadow();						/* �R���X�g���N�^ */
	virtual ~CLibInfoMapShadow();						/* �f�X�g���N�^ */

	void Create			(void);									/* �쐬 */
	void Destroy		(void);									/* �j�� */
	BOOL Proc			(void);									/* ���� */

	PCInfoBase GetNew	(void);									/* �V�K�f�[�^���擾 */

	int		GetCount	(void);									/* �f�[�^�����擾 */
	void	Add			(PCInfoBase pInfo);						/* �ǉ� */
	void	Delete		(int nNo);								/* �폜 */
	void	Delete		(DWORD dwShadowID);						/* �폜 */
	void	DeleteAll	(void);									/* �S�č폜 */
	void	Merge		(CLibInfoMapShadow *pSrc);				/* ��荞�� */
	void	ResetAnime	(void);									/* �A�j���[�V������Ԃ������� */

	PCInfoBase	GetPtr (int nNo);								/* �����擾 */
	PCInfoBase	GetPtr (DWORD dwShadowID);						/* �����擾 */
	PCInfoBase	GetPtr (POINT *pPos);							/* �����擾 */

	DWORD	GetSendDataSize		(void);							/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);							/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);					/* ���M�f�[�^�����荞�� */


protected:
	DWORD	GetNewID	(void);									/* �V����ID���擾 */
	void	RenewIDPtr	(void);									/* ID�����p�}�b�v���X�V */


protected:
	PARRAYMAPSHADOW	m_paInfo;				/* �}�b�v�e��� */
} CLibInfoMapShadow, *PCLibInfoMapShadow;

/* Copyright(C)URARA-works 2007 */
