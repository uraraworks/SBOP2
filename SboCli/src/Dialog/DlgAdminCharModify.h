/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharModify.h										 */
/* ���e			:�I���L�������ҏW�_�C�A���O�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/03/17													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoCharCli;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharModify : public CDlgAdminBase
{
public:
			CDlgAdminCharModify(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharModify();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);						/* ������ */
	void	Renew		(void);										/* �X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:
	void		Send		(BOOL bChgScreenPos);					/* ���M���� */
	COLORREF	GetColor	(int nNo);								/* �ԍ�����F���擾 */


protected:
	DWORD			m_dwCharID;					/* �Ō�ɑI�������L����ID */
	CInfoCharCli	*m_pInfoChar;				/* �ҏW���̃L������� */



public:
	//{{AFX_DATA(CDlgAdminCharModify)
	enum { IDD = IDD_CHAR_MODIFY };
	CComboBox	m_ctlMoveType;
	CComboBox	m_ctlMotionType;
	CComboBox	m_ctlSex;
	CComboBox	m_ctlColor;
	CString	m_strCharName;
	CString	m_strCharID;
	BOOL	m_bBlock;
	BOOL	m_bPush;
	DWORD	m_dwMapID;
	int		m_nPosX;
	int		m_nPosY;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharModify)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharModify)
	virtual BOOL OnInitDialog();
	afx_msg void OnSend();
	afx_msg void OnSetMoveType();
	afx_msg void OnTalk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharModify, *PCDlgAdminCharModify;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
