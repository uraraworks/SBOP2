/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowITEMMENU.h											 */
/* ���e			:�A�C�e�����j���[�E�B���h�E�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/30													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CInfoCharCli;
class CLibInfoItem;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowITEMMENU : public CWindowBase
{
public:
			CWindowITEMMENU();							/* �R���X�g���N�^ */
	virtual ~CWindowITEMMENU();							/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);					/* �쐬 */
	void	Draw		(CImg32 *pDst);							/* �`�� */

	DWORD	GetSelectItemID	(void)	{ return m_dwSelectItemID;	}	/* �I������ăA�C�e��ID���擾 */
	int		GetPos			(void)	{ return m_nPos;			}	/* �J�[�\���ʒu���擾 */


protected:
	BOOL	OnUp		(void);								/* �L�[�n���h��(��) */
	BOOL	OnDown		(void);								/* �L�[�n���h��(��) */
	BOOL	OnLeft		(void);								/* �L�[�n���h��(��) */
	BOOL	OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL	OnB			(BOOL bDown);						/* �L�[�n���h��(B) */
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL	OnZ			(BOOL bDown);						/* �L�[�n���h��(Z) */


protected:
	DWORD			m_dwSelectItemID;		/* �I�����ꂽ�A�C�e��ID */
	CmyString		m_strName;				/* �I�𒆂̃A�C�e���� */
	CInfoCharCli	*m_pPlayerChar;			/* ���쒆�̃L������� */
	CLibInfoItem	*m_pLibInfoItem;		/* �A�C�e����񃉃C�u���� */
} CWindowITEMMENU, *PCWindowITEMMENU;

/* Copyright(C)URARA-works 2007 */
