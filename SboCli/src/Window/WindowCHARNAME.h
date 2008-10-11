/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:WindowCHARNAME.h											 */
/* ���e			:�L���������̓E�B���h�E�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/08													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowCHARNAME : public CWindowBase
{
public:
			CWindowCHARNAME();								/* �R���X�g���N�^ */
	virtual ~CWindowCHARNAME();								/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */
	void	SetActive	(BOOL bActive);								/* �A�N�e�B�u���ݒ� */


private:
	void	MakeWindow	(void);										/* �E�B���h�E�쐬 */
	static LRESULT CALLBACK CharNameWndProc		(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* �L���������͗��v���V�[�W�� */


public:
	CmyString	m_strName;						/* ���͂��ꂽ�L������ */


private:
	HWND	m_hWndCharName;							/* �L���������͗� */

	WNDPROC	m_OrgWndProcCharName;					/* �L���������͗��̌��E�B���h�E�v���V�[�W�� */
} CWindowCHARNAME, *PCWindowCHARNAME;

/* Copyright(C)URARA-works 2006 */
