/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:SboCli.h													 */
/* ���e			:�A�v���P�[�V�����N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CSboCliApp : public CWinApp
{
public:
	CSboCliApp();								/* �R���X�g���N�^ */



public:
	//{{AFX_VIRTUAL(CSboCliApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSboCliApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2006 */
