/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharModifyItem.h									 */
/* ���e			:�I���L�������ҏW[�����A�C�e���̐ݒ�]�_�C�A���O�N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoCharCli;
class CLibInfoItem;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharModifyItem : public CDlgAdminBase
{
public:
			CDlgAdminCharModifyItem(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharModifyItem();						/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);						/* ������ */
	void	Renew		(void);										/* �X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);			/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */


protected:
	CInfoCharCli	*m_pInfoChar;					/* �ҏW���̃L������� */
	CLibInfoItem	*m_pLibInfoItem;				/* �A�C�e����񃉃C�u���� */



public:
	//{{AFX_DATA(CDlgAdminCharModifyItem)
	enum { IDD = IDD_CHAR_MODIFY_ITEM };
	CListCtrl	m_List;
	CString	m_strCharName;
	CString	m_strCharID;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharModifyItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharModifyItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddnew();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharModifyItem, *PCDlgAdminCharModifyItem;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
