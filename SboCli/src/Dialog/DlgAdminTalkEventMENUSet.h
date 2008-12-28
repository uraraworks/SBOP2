/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventMENUSet.h									 */
/* ���e			:��b�C�x���g�I�����ڂ̐ݒ�_�C�A���O�N���X ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/28													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "afxwin.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventMENUSet : public CDlgAdminBase
{
public:
			CDlgAdminTalkEventMENUSet(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminTalkEventMENUSet();						/* �f�X�g���N�^ */

	void	Init(CMgrData *pMgrData, int nPage);						/* ������ */


protected:


public:
	int		m_nPage,		/* �I�𒆂̃y�[�W�ԍ� */
			m_nPageCount;	/* �y�[�W�� */



public:
	void OnOK();
	//{{AFX_DATA(CDlgAdminTalkEventMENUSet)
	enum { IDD = IDD_TALKEVENT_MENU_SET };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventMENUSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventMENUSet)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CComboBox m_Combo;
} CDlgAdminTalkEventMENUSet, *PCDlgAdminTalkEventMENUSet;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
