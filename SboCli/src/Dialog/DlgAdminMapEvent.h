/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEvent.h											 */
/* ���e			:�}�b�v�C�x���g�ꗗ�_�C�A���O�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/24													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoMapBase;
class CLibInfoMapEvent;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminMapEvent : public CDlgAdminBase
{
public:
			CDlgAdminMapEvent(CWnd* pParent = NULL);			/* �R���X�g���N�^ */
	virtual ~CDlgAdminMapEvent();								/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	Renew		(void);											/* �ꗗ���X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:


protected:
	CWnd				*m_pWndNotify;				/* �ʒm��E�B���h�E */
	CInfoMapBase		*m_pInfoMap;				/* �ҏW���̃}�b�v��� */
	CLibInfoMapEvent	*m_pLibInfoMapEvent;		/* �ҏW���̃}�b�v�C�x���g��� */



public:
	//{{AFX_DATA(CDlgAdminMapEvent)
	enum { IDD = IDD_MAP_EVENT };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEvent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEvent)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnCopy();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminMapEvent, *PCDlgAdminMapEvent;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
