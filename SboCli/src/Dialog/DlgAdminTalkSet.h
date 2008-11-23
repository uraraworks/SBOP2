/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkSet.h											 */
/* ���e			:��b�f�[�^�ݒ�_�C�A���O�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/22													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminTalkSet : public CDlgAdminBase
{
public:
			CDlgAdminTalkSet(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminTalkSet();						/* �f�X�g���N�^ */


public:
	void OnOK();
	//{{AFX_DATA(CDlgAdminTalkSet)
	enum { IDD = IDD_TALK_SET };
	CString	m_strTalk;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkSet)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminTalkSet, *PCDlgAdminTalkSet;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
