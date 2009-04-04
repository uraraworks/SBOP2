/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoCharBase.h											 */
/* ���e			:�L������񃉃C�u�������N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/01													 */
/* ========================================================================= */

#pragma once

#include "InfoCharBase.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoCharBase : public CLibInfoBase
{
public:
			CLibInfoCharBase();							/* �R���X�g���N�^ */
	virtual ~CLibInfoCharBase();						/* �f�X�g���N�^ */

	virtual void	Create		(void);							/* �쐬 */
	virtual void	Destroy		(void);							/* �j�� */
	virtual BOOL	Proc		(void);							/* ���� */
	virtual void	RenewSize	(DWORD dwMapID, int nDirection, int nSize);	/* �}�b�v�T�C�Y�X�V */

			PCInfoBase	GetNew	(void);									/* �V�K�f�[�^���擾 */
	virtual PCInfoBase	GetNew	(int nType);							/* �V�K�f�[�^���擾 */

	void	GetSaveNo			(ARRAYINT &anDst);						/* �ۑ�������̃C���f�b�N�X���擾 */
	int		GetCount			(void);									/* �f�[�^�����擾 */
	int		GetCountScreen		(PCInfoCharBase pCharBase);				/* �f�[�^�����擾(��ʓ��̃L����) */
	void	Add					(PCInfoBase pInfo);						/* �ǉ� */
	DWORD	Add					(PCInfoCharBase pChar);					/* �ǉ� */
	void	CopyAdd				(PCInfoCharBase pChar);					/* �R�s�[���Ēǉ� */
	void	Delete				(int nNo);								/* �폜 */
	void	Delete				(DWORD dwCharID);						/* �폜 */
	void	DeleteAll			(void);									/* �S�č폜 */
	void	SortY				(void);									/* Y���W���Ƀ\�[�g */
	BOOL	IsBlockChar			(PCInfoCharBase pChar, int nDirection);	/* ����O�łԂ��邩�`�F�b�N */
	BOOL	IsUseName			(LPCSTR pszName);						/* ���O���g�p����Ă��邩�`�F�b�N */
	BOOL	NameCheck			(LPCSTR pszName);						/* ���O�Ɏg�p�ł��Ȃ����O�����邩�`�F�b�N */
	int		GetTurnDirection	(int nDirection);						/* �t�������擾 */
	void	GetDistance			(SIZE &sizeDst, PCInfoCharBase pInfoCharSrc, PCInfoCharBase pInfoCharDst, BOOL bFrontPos = FALSE);/* �L�������W�ŋ������擾 */

	BOOL	IsScreenInside	(PCInfoCharBase pCharBase, PCInfoCharBase pCharTarget);	/* ��ʓ��ɂ��邩�`�F�b�N */

	virtual	DWORD	GetFrontCharID		(DWORD dwCharID, int nDirection = -1);	/* ����O�̃L����ID���擾 */
	virtual	DWORD	GetFrontCharIDPush	(DWORD dwCharID, int nDirection = -1);	/* ����O�̉�����L����ID���擾 */
	virtual	DWORD	GetHitCharID		(DWORD dwCharIDBase, int x, int y);		/* �w����W�ɓ�����L����ID���擾 */
	virtual	void	SetPtr				(DWORD dwCharID, PCInfoCharBase pChar);	/* �L���������X�V */
			PCInfoBase	GetPtr			(int nNo);								/* �L���������擾 */
			PCInfoBase	GetPtr			(DWORD dwCharID);						/* �L���������擾 */
			PCInfoBase	GetPtrAccountID	(DWORD dwAccountID);					/* �L���������擾 */
			PCInfoBase	GetPtrFront		(PCInfoCharBase pChar, int nDirection);	/* ����O�̃L���������擾 */

	DWORD	GetSendDataSize		(void);							/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);							/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);					/* ���M�f�[�^�����荞�� */

	DWORD	GetDataSizeScreen	(PCInfoCharBase pCharBase);		/* �f�[�^�T�C�Y���擾(��ʓ��̃L����) */
	PBYTE	GetDataScreen		(PCInfoCharBase pCharBase);		/* �f�[�^���擾(��ʓ��̃L����) */


protected:
	DWORD	GetNewID		(void);								/* �V�����L����ID���擾 */


protected:
	DWORD	m_dwNewIDTmp;						/* �V�KID�쐬�p */
	PARRAYINFOCHARBASE	m_paInfo;				/* �L������� */
} CLibInfoCharBase, *PCLibInfoCharBase;

/* Copyright(C)URARA-works 2006 */
