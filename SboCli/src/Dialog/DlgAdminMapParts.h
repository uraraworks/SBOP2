/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapParts.h											 */
/* ���e			:�}�b�v�p�[�c�ҏW�_�C�A���O�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/25													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "StaticMapParts.h"

class CMgrGrpData;
class CImg32;
class CLibInfoMapParts;
class CUraraSockTCPSBO;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CDlgAdminMapParts : public CDlgAdminBase
{
public:
			CDlgAdminMapParts(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminMapParts();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);						/* ������ */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:
	void	RenewMessage		(LPCSTR pszMsg);	/* ���b�Z�[�W�����X�V */
	void	RenewPartsInfo		(void);				/* �p�[�c�����X�V */
	void	MakePartsImage		(void);				/* �p�[�c�ꗗ�摜���쐬 */
	DWORD	GetSelectMapPartsID	(void);				/* �I�𒆂̃}�b�v�p�[�cID���擾 */


protected:
	int		m_nSelectType,						/* �ݒ荀�� */
			m_nState;							/* ��� */
	DWORD	m_dwSelectPartsID;					/* �I�𒆂̃}�b�v�p�[�cID */
	CPoint	m_ptCursor,							/* �J�[�\���̂���p�[�c�ʒu */
			m_ptMoveSrc,						/* �ړ����̈ʒu */
			m_ptMoveDst;						/* �ړ���̈ʒu */

	CMgrGrpData			*m_pMgrGrpData;			/* �O���t�B�b�N�f�[�^�Ǘ� */
	CImg32				*m_pImgParts;			/* �p�[�c�ꗗ�摜 */
	CLibInfoMapParts	*m_pLibInfoMapParts;	/* �ҏW���̃}�b�v�p�[�c��� */
	CUraraSockTCPSBO	*m_pSock;				/* �ʐM�}�l�[�W�� */


public:
	//{{AFX_DATA(CDlgAdminMapParts)
	enum { IDD = IDD_MAP_PARTS };
	CStaticMapParts	m_ctlMapParts;
	CScrollBar	m_ctlScroll;
	CComboBox	m_cbType;
	int		m_nTypeSelect;
	CString	m_strID;
	CString	m_strMsg;
	CString	m_strAttr;
	CString	m_strAttr2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapParts)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapParts)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeType();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	afx_msg LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
