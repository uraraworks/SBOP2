/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WndToolbar.h												 */
/* ���e			:�c�[���o�[�R���g���[���N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/09/13													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWndToolbar : public CToolBar
{
public:
			CWndToolbar();							/* �R���X�g���N�^ */
	virtual ~CWndToolbar();							/* �f�X�g���N�^ */


public:
	//{{AFX_VIRTUAL(CWndToolbar)
	public:
	protected:
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndToolbar)
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CWndToolbar, *PCWndToolbar;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
