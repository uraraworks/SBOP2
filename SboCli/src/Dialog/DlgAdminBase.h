/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminBase.h												 */
/* ���e			:�Ǘ��҃E�B���h�E���_�C�A���O�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/03/18													 */
/* ========================================================================= */

#pragma once

#include "LayoutHelper.h"

class CMgrData;
class CUraraSockTCPSBO;
class CInfoItemTypeBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminBase : public CDialog, public CLayoutHelper
{
public:
			CDlgAdminBase(int nResourceID, CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminBase();										/* �f�X�g���N�^ */

	virtual void	Init			(CMgrData *pMgrData);					/* ������ */
	virtual void	Renew			(void);									/* �X�V */
	virtual void	OnAdminMsg		(int nType, DWORD dwPara);				/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */
	virtual void	OnMainFrame		(DWORD dwCommand, DWORD dwParam);		/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */
	virtual void	Set				(CInfoItemTypeBase *pSrc)  {}			/* �ҏW���e��ݒ� */
	virtual void	Get				(CInfoItemTypeBase *&pDst) {}			/* �ҏW���e���擾 */


protected:
	void	SelectCmb		(CComboBox *pCmb, DWORD dwID);	/* ���ڂ�I�� */


protected:
	CMgrData			*m_pMgrData;			/* �f�[�^�}�l�[�W�� */
	CUraraSockTCPSBO	*m_pSock;				/* �ʐM�}�l�[�W�� */
	CWnd				*m_pWndParent;			/* �e�E�B���h�E */



public:
	//{{AFX_DATA(CDlgAdminBase)
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminBase)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminBase, *PCDlgAdminBase;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
