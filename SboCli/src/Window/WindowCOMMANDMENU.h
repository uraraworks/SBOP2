/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowCOMMANDMENU.h										 */
/* ���e			:�R�}���h���j���[�E�B���h�E�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/02													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowCOMMANDMENU : public CWindowBase
{
public:
			CWindowCOMMANDMENU();						/* �R���X�g���N�^ */
	virtual ~CWindowCOMMANDMENU();						/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);					/* �쐬 */
	void	Draw		(CImg32 *pDst);							/* �`�� */
	int		GetPosSub	(void)	{ return m_nPosSub; }			/* �T�u���j���[�̈ʒu���擾 */


protected:
	BOOL	OnUp			(void);							/* �L�[�n���h��(��) */
	BOOL	OnDown			(void);							/* �L�[�n���h��(��) */
	BOOL	OnLeft			(void);							/* �L�[�n���h��(��) */
	BOOL	OnRight			(void);							/* �L�[�n���h��(��) */
	BOOL	OnB				(BOOL bDown);					/* �L�[�n���h��(B) */
	BOOL	OnX				(BOOL bDown);					/* �L�[�n���h��(X) */
	BOOL	OnZ				(BOOL bDown);					/* �L�[�n���h��(Z) */
	BOOL	OnSpace			(BOOL bDown);					/* �L�[�n���h��(Space) */
	void	RenewCommand	(void);							/* �I�𒆂̃R�}���h���X�V */


protected:
	int			m_nPosSub;					/* �T�u���j���[�̈ʒu */
	CmyString	m_strCommand;				/* �I�𒆂̃R�}���h */
} CWindowCOMMANDMENU, *PCWindowCOMMANDMENU;

/* Copyright(C)URARA-works 2008 */
