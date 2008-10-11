/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowSTYLESELECT.h										 */
/* ���e			:�e�p�I���E�B���h�E�N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/10													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;
class CMgrWindow;
class CMgrDraw;
class CWindowSEX;
class CWindowHAIRTYPE;
class CWindowHAIRCOLOR;
class CWindowEYECOLOR;
class CInfoCharCli;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowSTYLESELECT : public CWindowBase
{
public:
			CWindowSTYLESELECT();							/* �R���X�g���N�^ */
	virtual ~CWindowSTYLESELECT();							/* �f�X�g���N�^ */

	void	Create			(CMgrData *pMgrData);					/* �쐬 */
	void	SetFamilyType	(WORD wFamilyID);						/* �푰��ݒ� */
	void	SetInfoChar		(CInfoCharCli *pInfoCharCli);			/* �L��������ݒ� */
	void	OnWindowMsg		(int nType, DWORD dwPara);				/* ���b�Z�[�W�n���h��(WM_WINDOWMSG) */
	void	Draw			(CImg32 *pDst);							/* �`�� */
	BOOL	TimerProc		(void);									/* ���ԏ��� */


private:
	BOOL	OnUp		(void);								/* �L�[�n���h��(��) */
	BOOL	OnDown		(void);								/* �L�[�n���h��(��) */
	BOOL	OnLeft		(void);								/* �L�[�n���h��(��) */
	BOOL	OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL	OnZ			(BOOL bDown);						/* �L�[�n���h��(Y) */


public:
	CInfoCharCli		*m_pInfoCharCli;			/* �쐬���̃L������� */


private:
	BOOL		m_bAnime;							/* �A�j���[�V�������锻�� */
	int			m_nSex,								/* ���� */
				m_nSexBack,							/* ���ʕۑ��p */
				m_nDirection;						/* �����ԍ� */
	WORD		m_wFamilyID,						/* �푰ID */
				m_wFamilyIDBack,					/* �푰ID�ۑ��p */
				m_wHairTypeID,						/* ���^ID */
				m_wHairTypeIDBack,					/* ���^ID�ۑ��p */
				m_wHairColorID,						/* ���FID */
				m_wHairColorIDBack,					/* ���FID�ۑ��p */
				m_wEyeColorID,						/* �ڐFID */
				m_wEyeColorIDBack;					/* �ڐFID�ۑ��p */
	CMgrWindow			*m_pMgrWindow;				/* �E�B���h�E�}�l�[�W�� */
	CMgrDraw			*m_pMgrDraw;				/* �`��}�l�[�W�� */
	CWindowSEX			*m_pWindowSEX;				/* ���ʓ��̓E�B���h�E */
	CWindowHAIRTYPE		*m_pWindowHAIRTYPE;			/* ���^�I���E�B���h�E */
	CWindowHAIRCOLOR	*m_pWindowHAIRCOLOR;		/* ���F�I���E�B���h�E */
	CWindowEYECOLOR		*m_pWindowEYECOLOR;			/* �ڐF�I���E�B���h�E */
} CWindowSTYLESELECT, *PCWindowSTYLESELECT;

/* Copyright(C)URARA-works 2007 */
