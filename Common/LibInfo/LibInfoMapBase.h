/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMapBase.h											 */
/* ���e			:�}�b�v��񃉃C�u�������N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/15													 */
/* ========================================================================= */

#pragma once

#include "InfoMapBase.h"
#include "LibInfoBase.h"

class CLibInfoMapParts;
class CLibInfoMapObject;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoMapBase : public CLibInfoBase
{
public:
			CLibInfoMapBase();							/* �R���X�g���N�^ */
	virtual ~CLibInfoMapBase();							/* �f�X�g���N�^ */

	void Create			(CLibInfoMapParts *pLibInfoMapParts);	/* �쐬 */
	void Destroy		(void);									/* �j�� */

	PCInfoBase GetNew	(void);									/* �V�K�f�[�^���擾 */

	void	RenewMapEvent	(void);								/* �}�b�v�C�x���g�X�V */
	void	RenewHitTmp		(void);								/* �}�b�v�p�[�c�ȊO�ł̓����蔻����X�V */
	void	SetMapObject	(CLibInfoMapObject *pLibInfo);		/* �}�b�v�I�u�W�F�N�g����ݒ� */
	int		GetCount		(void);								/* �f�[�^�����擾 */
	void	Add				(PCInfoBase pInfo);					/* �ǉ� */
	void	Delete			(int nNo);							/* �폜 */
	void	Delete			(DWORD dwMapID);					/* �폜 */
	void	DeleteAll		(void);								/* �S�č폜 */
	void	DeleteParts		(DWORD dwPartsID);					/* �w��p�[�c���폜 */
	void	DeleteShadow	(DWORD dwShadowID);					/* �w��}�b�v�e���폜 */

	PCInfoBase	GetPtr (int nNo);								/* �}�b�v�����擾 */
	PCInfoBase	GetPtr (DWORD dwMapID);							/* �}�b�v�����擾 */


protected:
	DWORD	GetNewID	(void);									/* �V�����}�b�vID���擾 */


protected:
	PARRAYINFOMAPBASE	m_paInfo;				/* �}�b�v��� */
	CLibInfoMapParts	*m_pLibInfoMapParts;	/* �}�b�v�p�[�c��� */
} CLibInfoMapBase, *PCLibInfoMapBase;

/* Copyright(C)URARA-works 2006 */
