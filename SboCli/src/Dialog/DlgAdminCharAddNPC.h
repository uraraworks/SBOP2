/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharAddNPC.h										 */
/* ���e			:NPC�̒ǉ��_�C�A���O�N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/01													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoCharCli;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharAddNPC : public CDlgAdminBase
{
public:
			CDlgAdminCharAddNPC(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharAddNPC();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);						/* ������ */
	void	Renew		(void);										/* �X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:
	CInfoCharCli	*m_pInfoChar;					/* �ҏW���̃L������� */



public:
	//{{AFX_DATA(CDlgAdminCharAddNPC)
	enum { IDD = IDD_CHAR_ADDNPC };
	CComboBox	m_ctlFamilyType;
	CString	m_strCharName;
	int		m_nMapID;
	int		m_nPosX;
	int		m_nPosY;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharAddNPC)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharAddNPC)
	virtual BOOL OnInitDialog();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharAddNPC, *PCDlgAdminCharAddNPC;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
