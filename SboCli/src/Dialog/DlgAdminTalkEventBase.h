/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventBase.h									 */
/* ���e			:��b�C�x���g�̐ݒ�_�C�A���O�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "afxwin.h"

class CDlgAdminTalkEventNONE;
class CInfoTalkEventBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventBase : public CDlgAdminBase
{
public:
			CDlgAdminTalkEventBase(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminTalkEventBase();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData, CInfoTalkEventBase *pInfo = NULL);	/* ������ */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */
	void	Get			(CInfoTalkEventBase *&pDst);					/* �擾 */


protected:


protected:
	CDlgAdminTalkEventNONE	*m_pDlgType;	/* �ҏW���̃C�x���g��ʃ_�C�A���O */
	CInfoTalkEventBase		*m_pInfo;		/* �ҏW���̃C�x���g��� */



public:
	void OnOK();
	//{{AFX_DATA(CDlgAdminTalkEventBase)
	enum { IDD = IDD_TALKEVENT_BASE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventBase)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Type;
	afx_msg void OnCbnSelchangeType();
} CDlgAdminTalkEventBase, *PCDlgAdminTalkEventBase;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
