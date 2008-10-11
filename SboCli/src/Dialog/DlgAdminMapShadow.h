/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapShadow.h										 */
/* ���e			:�}�b�v�e�ҏW�_�C�A���O�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/06													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "StaticMapShadow.h"

class CMgrGrpData;
class CImg32;
class CLibInfoMapShadow;
class CUraraSockTCPSBO;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CDlgAdminMapShadow : public CDlgAdminBase
{
public:
			CDlgAdminMapShadow(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminMapShadow();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);						/* ������ */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:
	void	RenewMessage			(LPCSTR pszMsg);	/* ���b�Z�[�W�����X�V */
	void	MakeShadowImage			(void);				/* �e�ꗗ�摜���쐬 */
	DWORD	GetSelectMapShadowID	(void);				/* �I�𒆂̉eID���擾 */


protected:
	int		m_nSelectType,						/* �ݒ荀�� */
			m_nState;							/* ��� */
	DWORD	m_dwSelectShadowID;					/* �I�𒆂̉eID */
	CPoint	m_ptCursor,							/* �J�[�\���̂���ʒu */
			m_ptMoveSrc,						/* �ړ����̈ʒu */
			m_ptMoveDst;						/* �ړ���̈ʒu */

	CMgrGrpData			*m_pMgrGrpData;			/* �O���t�B�b�N�f�[�^�Ǘ� */
	CImg32				*m_pImgShadow;			/* �e�ꗗ�摜 */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;	/* �ҏW���̃}�b�v�e��� */
	CUraraSockTCPSBO	*m_pSock;				/* �ʐM�}�l�[�W�� */



public:
	//{{AFX_DATA(CDlgAdminMapShadow)
	enum { IDD = IDD_MAP_SHADOW };
	CStaticMapShadow	m_ctlMapShadow;
	CScrollBar	m_ctlScroll;
	CComboBox	m_cbType;
	int		m_nTypeSelect;
	CString	m_strID;
	CString	m_strMsg;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapShadow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapShadow)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
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
