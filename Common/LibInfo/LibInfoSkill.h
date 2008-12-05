/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoSkill.h												 */
/* ���e			:�X�L����񃉃C�u�����N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/04													 */
/* ========================================================================= */

#pragma once

#include "InfoSkillBase.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoSkill : public CLibInfoBase
{
public:
			CLibInfoSkill();							/* �R���X�g���N�^ */
	virtual ~CLibInfoSkill();							/* �f�X�g���N�^ */

	void Create			(void);									/* �쐬 */
	void Destroy		(void);									/* �j�� */

	PCInfoBase GetNew	(int nType);							/* �V�K�f�[�^���擾 */

	int		GetCount	(void);									/* �f�[�^�����擾 */
	void	Add			(PCInfoBase pInfo);						/* �ǉ� */
	void	Delete		(int nNo);								/* �폜 */
	void	Delete		(DWORD dwSkillID);						/* �폜 */
	void	DeleteAll	(void);									/* �S�č폜 */
	void	Merge		(CLibInfoSkill *pSrc);					/* ��荞�� */

	PCInfoBase	GetPtr (int nNo);								/* �����擾 */
	PCInfoBase	GetPtr (DWORD dwSkillID);						/* �����擾 */

	DWORD	GetSendDataSize		(void);							/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);							/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);					/* ���M�f�[�^�����荞�� */


protected:
	DWORD	GetNewID	(void);									/* �V����ID���擾 */


protected:
	PARRAYSKILLBASEINFO	m_paInfo;				/* �X�L����� */
} CLibInfoSkill, *PCLibInfoSkill;

/* Copyright(C)URARA-works 2008 */
