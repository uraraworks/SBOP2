/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:MakeFileList.h												 */
/* ���e			:�A�v���P�[�V�����N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/26													 */
/* ========================================================================= */

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CMakeFileListApp : public CWinApp
{
public:
	CMakeFileListApp();

	//{{AFX_VIRTUAL(CMakeFileListApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMakeFileListApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
