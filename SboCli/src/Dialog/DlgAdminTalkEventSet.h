/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventSet.h										 */
/* ���e			:��b�C�x���g�ݒ�_�C�A���O�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "afxcmn.h"

class CInfoTalkEvent;
class CDlgAdminTalkEventTab;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventSet : public CDlgAdminBase
{
public:
			CDlgAdminTalkEventSet(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminTalkEventSet();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:
	void	AddPage(void);			/* �y�[�W�̒ǉ� */


protected:
	CInfoTalkEvent			*m_pInfo;		/* �ҏW���̉�b�C�x���g */
	CDlgAdminTalkEventTab	*m_pDlgTab;		/* �^�u�_�C�A���O */


public:
	//{{AFX_DATA(CDlgAdminTalkEventSet)
	enum { IDD = IDD_TALKEVENT_SET };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventSet)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
} CDlgAdminTalkEventSet, *PCDlgAdminTalkEventSet;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
