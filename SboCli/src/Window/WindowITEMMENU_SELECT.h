/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowITEMMENU_SELECT.h									 */
/* ���e			:�A�C�e���ǂ����邩���j���[�E�B���h�E�N���X ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/13													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CInfoItem;
class CLibInfoItem;

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

enum {
	ITEMMENU_SELECT_COMMAND_NONE = 0,	
	ITEMMENU_SELECT_COMMAND_PUT,			/* �n�ʂɒu�� */
	ITEMMENU_SELECT_COMMAND_EQUIP,			/* �������� */
	ITEMMENU_SELECT_COMMAND_EQUIP_UNSET,	/* �������O�� */
	ITEMMENU_SELECT_COMMAND_USE,			/* �g�� */
	ITEMMENU_SELECT_COMMAND_MAX
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowITEMMENU_SELECT : public CWindowBase
{
public:
			CWindowITEMMENU_SELECT();						/* �R���X�g���N�^ */
	virtual ~CWindowITEMMENU_SELECT();						/* �f�X�g���N�^ */

	void	Create			(CMgrData *pMgrData);					/* �쐬 */
	void	Draw			(CImg32 *pDst);							/* �`�� */
	void	SetItemID		(int nType, DWORD dwItemID);			/* �A�C�e��ID���w�� */
	int		GetType			(void)	{ return m_nType; }				/* �A�C�e���̎�ނ��擾 */


protected:
	BOOL	OnUp		(void);								/* �L�[�n���h��(��) */
	BOOL	OnDown		(void);								/* �L�[�n���h��(��) */
	BOOL	OnLeft		(void);								/* �L�[�n���h��(��) */
	BOOL	OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL	OnZ			(BOOL bDown);						/* �L�[�n���h��(Z) */


protected:
	int				m_nType;					/* �A�C�e���̎�� */
	CInfoItem		*m_pInfoItem;				/* �������̃A�C�e����� */
	CLibInfoItem	*m_pLibInfoItem;			/* �A�C�e����񃉃C�u���� */
	ARRAYINT		m_anCommand;				/* �R�}���h��� */
} CWindowITEMMENU_SELECT, *PCWindowITEMMENU_SELECT;

/* Copyright(C)URARA-works 2007 */
