/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:DlgDbg.h													 */
/* ���e			:�f�o�b�O���_�C�A���O�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/04/05													 */
/* ========================================================================= */

#pragma once

#include "resource.h"
#include "LayoutHelper.h"

class CMgrData;
class CUraraSockTCPSBO;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgDbg : public CDialog, CLayoutHelper
{
public:
			CDlgDbg(CWnd* pParent = NULL);				/* �R���X�g���N�^ */
	virtual ~CDlgDbg();									/* �f�X�g���N�^ */

	BOOL Create	(HWND hWndParent, CMgrData *pMgrData);			/* �쐬 */
	void Renew	(void);											/* �X�V */


protected:


protected:
	CMgrData			*m_pMgrData;			/* �f�[�^�Ǘ� */
	CUraraSockTCPSBO	*m_pSock;				/* �ʐM�}�l�[�W�� */


public:
	void OnOK () {}
	void OnCancel () {}
	//{{AFX_DATA(CDlgDbg)
	enum { IDD = IDD_DBGDLG };
	CString	m_strOnline;
	CString	m_strCharCount;
	CString	m_strPing;
	CString	m_strPos;
	CString	m_strThrowghput;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgDbg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgDbg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
} CDlgDbg, *PCDlgDbg;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2009 */
