/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMapParts.h											 */
/* ���e			:�}�b�v�p�[�c��񃉃C�u�����N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/28													 */
/* ========================================================================= */

#pragma once

#include "InfoMapParts.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoMapParts : public CLibInfoBase
{
public:
			CLibInfoMapParts();							/* �R���X�g���N�^ */
	virtual ~CLibInfoMapParts();						/* �f�X�g���N�^ */

	void Create			(void);									/* �쐬 */
	void Destroy		(void);									/* �j�� */
	BOOL Proc			(void);									/* ���� */

	PCInfoBase GetNew	(void);									/* �V�K�f�[�^���擾 */

	int		GetCount	(void);									/* �f�[�^�����擾 */
	void	Add			(PCInfoBase pInfo);						/* �ǉ� */
	void	RenewIDPtr	(void);									/* ID�����p�}�b�v���X�V */
	void	Delete		(int nNo);								/* �폜 */
	void	Delete		(DWORD dwPartsID);						/* �폜 */
	void	DeleteAll	(void);									/* �S�č폜 */
	void	Merge		(CLibInfoMapParts *pSrc);				/* ��荞�� */
	void	ResetAnime	(void);									/* �A�j���[�V������Ԃ������� */

	PCInfoBase	GetPtr (int nNo);								/* �����擾 */
	PCInfoBase	GetPtr (DWORD dwPartsID);						/* �����擾 */
	PCInfoBase	GetPtr (POINT *pPos);							/* �����擾 */

	DWORD	GetSendDataSize		(void);							/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);							/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);					/* ���M�f�[�^�����荞�� */


protected:
	DWORD	GetNewID	(void);									/* �V����ID���擾 */


protected:
	DWORD	m_dwNewIDTmp;						/* �V�KID�쐬�p */
	PARRAYMAPPARTS	m_paInfo;					/* �}�b�v�p�[�c��� */
} CLibInfoMapParts, *PCLibInfoMapParts;

/* Copyright(C)URARA-works 2007 */
