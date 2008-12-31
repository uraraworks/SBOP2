/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowSKILLMENU.h											 */
/* ���e			:�X�L�����j���[�E�B���h�E�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/31													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CInfoCharCli;
class CLibInfoSkill;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowSKILLMENU : public CWindowBase
{
public:
			CWindowSKILLMENU();							/* �R���X�g���N�^ */
	virtual ~CWindowSKILLMENU();						/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);					/* �쐬 */
	void	Draw		(CImg32 *pDst);							/* �`�� */
	void	DragOff		(void);									/* �h���b�O���� */

	DWORD	GetSelectID	(void)	{ return m_dwSelectID;	}		/* �I������Ă���X�L��ID���擾 */
	int		GetPos		(void)	{ return m_nPos;		}		/* �J�[�\���ʒu���擾 */
	void	SetType		(int nType) { m_nType = nType; }		/* �X�L����ʂ�ݒ� */


protected:
	BOOL	OnUp		(void);								/* �L�[�n���h��(��) */
	BOOL	OnDown		(void);								/* �L�[�n���h��(��) */
	BOOL	OnLeft		(void);								/* �L�[�n���h��(��) */
	BOOL	OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL	OnS			(BOOL bDown);						/* �L�[�n���h��(S) */
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL	OnZ			(BOOL bDown);						/* �L�[�n���h��(Z) */
	void	GetDrawPos	(int nPos, int &nDstX, int &nDstY);	/* �`��ʒu���擾 */


protected:
	int				m_nType;				/* �X�L����� */
	DWORD			m_dwSelectID;			/* �I�����ꂽ�X�L��ID */
	CmyString		m_strName;				/* �I�𒆂̃X�L���� */
	CInfoCharCli	*m_pPlayerChar;			/* ���쒆�̃L������� */
	CLibInfoSkill	*m_pLibInfoSkill;		/* �X�L����񃉃C�u���� */
} CWindowSKILLMENU, *PCWindowSKILLMENU;

/* Copyright(C)URARA-works 2008 */
