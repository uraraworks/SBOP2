/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharModifyGrp.h									 */
/* ���e			:�I���L�������ҏW[�摜�̐ݒ�]�_�C�A���O�N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/02													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoCharCli;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharModifyGrp : public CDlgAdminBase
{
public:
			CDlgAdminCharModifyGrp(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharModifyGrp();						/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);						/* ������ */
	void	Renew		(void);										/* �X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:
	CInfoCharCli	*m_pInfoChar;					/* �ҏW���̃L������� */



public:
	//{{AFX_DATA(CDlgAdminCharModifyGrp)
	enum { IDD = IDD_CHAR_MODIFY_GRP };
	CString	m_strCharName;
	CString	m_strCharID;
	int		m_nGrpIDEye;
	int		m_nGrpIDEyeColor;
	int		m_nGrpIDHair;
	int		m_nGrpIDHairColor;
	int		m_nGrpIDCloth;
	int		m_nGrpIDSP;
	int		m_nGrpIDNPC;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharModifyGrp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharModifyGrp)
	virtual BOOL OnInitDialog();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharModifyGrp, *PCDlgAdminCharModifyGrp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
