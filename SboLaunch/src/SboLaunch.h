/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:SboLaunch.h												 */
/* ���e			:SBO�����`���[�A�v���P�[�V�����N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/01/27													 */
/* ========================================================================= */

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CSboLaunchApp : public CWinApp
{
public:
	CSboLaunchApp();			/* �R���X�g���N�^ */



public:
	//{{AFX_VIRTUAL(CSboLaunchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSboLaunchApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
/* Copyright(C)URARA-works 2008 */
