/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapObject.h										 */
/* ���e			:�}�b�v�I�u�W�F�N�g�ꗗ�_�C�A���O�N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/09/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoMapBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminMapObject : public CDlgAdminBase
{
public:
			CDlgAdminMapObject(CWnd* pParent = NULL);			/* �R���X�g���N�^ */
	virtual ~CDlgAdminMapObject();								/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	Renew		(void);											/* �ꗗ���X�V */


protected:


protected:
	CWnd				*m_pWndNotify;				/* �ʒm��E�B���h�E */
	CInfoMapBase		*m_pInfoMap;				/* �ҏW���̃}�b�v��� */



public:
	//{{AFX_DATA(CDlgAdminMapObject)
	enum { IDD = IDD_MAP_EVENT };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapObject)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapObject)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnCopy();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminMapObject, *PCDlgAdminMapObject;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
