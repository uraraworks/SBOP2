/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoSystem.h											 */
/* ���e			:�V�X�e����񃉃C�u�����N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/09/28													 */
/* ========================================================================= */

#pragma once

#include "InfoSystem.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoSystem : public CLibInfoBase
{
public:
			CLibInfoSystem();								/* �R���X�g���N�^ */
	virtual ~CLibInfoSystem();								/* �f�X�g���N�^ */

	void Create			(void);										/* �쐬 */
	void Destroy		(void);										/* �j�� */

	virtual PCInfoBase GetNew	(void);								/* �V�K�f�[�^���擾 */

	int		GetCount	(void);										/* �f�[�^�����擾 */
	void	Add			(PCInfoBase pInfo);							/* �ǉ� */
	void	Delete		(int nNo);									/* �폜 */
	void	DeleteAll	(void);										/* �S�č폜 */

	PCInfoBase	GetPtr	(int nNo = 0);								/* �V�X�e�������擾 */


protected:
	PCInfoSystem	m_pInfo;				/* �V�X�e����� */
} CLibInfoSystem, *PCLibInfoSystem;

/* Copyright(C)URARA-works 2008 */
