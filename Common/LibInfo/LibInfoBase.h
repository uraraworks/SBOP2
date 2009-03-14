/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoBase.h												 */
/* ���e			:��񃉃C�u���� ���N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/30													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"
#include "mySection.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoBase : public CmySection
{
public:
			CLibInfoBase();							/* �R���X�g���N�^ */
	virtual ~CLibInfoBase();						/* �f�X�g���N�^ */

	virtual void Create			(void);						/* �쐬 */
	virtual void Destroy		(void);						/* �j�� */
	virtual BOOL Proc			(void);						/* ���� */

	virtual int			GetCount	(void);					/* �f�[�^�����擾 */
	virtual PCInfoBase	GetNew		(void);					/* �V�K�f�[�^���擾 */
	virtual PCInfoBase	RenewInfo	(int nNo);				/* ���e�ɉ����čX�V */
	virtual void		GetSaveNo	(ARRAYINT &anDst);		/* �ۑ�������̃C���f�b�N�X���擾 */
	virtual void		Add			(PCInfoBase pInfo);		/* �ǉ� */
	virtual PCInfoBase	GetPtr		(int nNo);				/* �����擾 */


public:
	int			m_nProcNo;				/* �������̃C���f�b�N�X */
	MapIDPtr	m_mapIDPtr;				/* ID�����p�}�b�v */
} CLibInfoBase, *PCLibInfoBase;

/* Copyright(C)URARA-works 2007 */
