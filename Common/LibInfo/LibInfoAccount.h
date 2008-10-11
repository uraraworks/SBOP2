/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoAccount.h											 */
/* ���e			:�A�J�E���g��񃉃C�u�������N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "InfoAccount.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoAccount : public CLibInfoBase
{
public:
			CLibInfoAccount();								/* �R���X�g���N�^ */
	virtual ~CLibInfoAccount();								/* �f�X�g���N�^ */

	void Create			(void);										/* �쐬 */
	void Destroy		(void);										/* �j�� */
	BOOL CheckPassword	(LPCSTR pszAccount, LPCSTR pszPassword);	/* �p�X���[�h�̃`�F�b�N */
	BOOL IsUseMacAddr	(LPCSTR pszMacAddr);						/* MAC�A�h���X���g�p�ς݂��`�F�b�N */

	virtual PCInfoBase GetNew	(void);								/* �V�K�f�[�^���擾 */

	int		GetCount		(void);									/* �f�[�^�����擾 */
	void	Add				(PCInfoBase pInfo);						/* �ǉ� */
	void	Delete			(int nNo);								/* �폜 */
	void	Delete			(DWORD dwAccountID);					/* �폜 */
	void	DeleteAll		(void);									/* �S�č폜 */
	DWORD	GetAccountID	(DWORD dwCharID);						/* �L����ID����A�J�E���gID���擾 */

	PCInfoBase		GetPtr			(int nNo);						/* �A�J�E���g�����擾 */
	PCInfoAccount	GetPtr			(DWORD dwAccountID);			/* �A�J�E���g�����擾 */
	PCInfoAccount	GetPtr			(LPCSTR pszAccount);			/* �A�J�E���g�����擾 */
	PCInfoAccount	GetPtrSessionID	(DWORD dwSessionID);			/* �A�J�E���g�����擾 */


protected:
	DWORD	GetNewID	(void);									/* �V�����A�J�E���gID���擾 */


protected:
	PARRAYINFOACCOUNT	m_paInfo;				/* �A�J�E���g��� */
} CLibInfoAccount, *PCLibInfoAccount;

/* Copyright(C)URARA-works 2006 */
