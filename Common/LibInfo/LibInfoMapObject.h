/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMapObject.h											 */
/* ���e			:�}�b�v�I�u�W�F�N�g��񃉃C�u�����N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/01													 */
/* ========================================================================= */

#pragma once

#include "InfoMapObject.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoMapObject : public CLibInfoBase
{
public:
			CLibInfoMapObject();						/* �R���X�g���N�^ */
	virtual ~CLibInfoMapObject();						/* �f�X�g���N�^ */

	void Create			(void);									/* �쐬 */
	void Destroy		(void);									/* �j�� */

	PCInfoBase GetNew	(void);									/* �V�K�f�[�^���擾 */

	int		GetCount	(void);									/* �f�[�^�����擾 */
	void	Add			(PCInfoBase pInfo);						/* �ǉ� */
	void	Delete		(int nNo);								/* �폜 */
	void	Delete		(DWORD dwObjectID);						/* �폜 */
	void	DeleteAll	(void);									/* �S�č폜 */
	void	Merge		(CLibInfoMapObject *pSrc);				/* ��荞�� */

	PCInfoBase	GetPtr (int nNo);								/* �����擾 */
	PCInfoBase	GetPtr (DWORD dwObjectID);						/* �����擾 */

	DWORD	GetSendDataSize		(void);							/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);							/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);					/* ���M�f�[�^�����荞�� */


protected:
	DWORD	GetNewID	(void);									/* �V����ID���擾 */


protected:
	PARRAYMAPOBJECT	m_paInfo;				/* �}�b�v�I�u�W�F�N�g��� */
} CLibInfoMapObject, *PCLibInfoMapObject;

/* Copyright(C)URARA-works 2008 */
