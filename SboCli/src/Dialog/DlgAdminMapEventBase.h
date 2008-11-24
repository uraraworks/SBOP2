/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEventBase.h										 */
/* ���e			:�}�b�v�C�x���g�ҏW�_�C�A���O�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/24													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CDlgAdminMapEventNONE;
class CInfoMapEventBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminMapEventBase : public CDlgAdminBase
{
public:
			CDlgAdminMapEventBase(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminMapEventBase();						/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData, CWnd **pWndNotify);	/* ������ */
	void	Get			(CInfoMapEventBase *&pDst);					/* �ҏW���e���擾 */
	void	SetModify	(CInfoMapEventBase *pSrc);					/* �ҏW���[�h�Ƃ��Đݒ� */


protected:
	int			m_nEventType,				/* �C�x���g��� */
				m_nHitType,					/* �����蔻���� */
				m_nHitDirection;			/* ������� */
	BOOL		m_bModeModify;				/* �ҏW���[�h���� */
	CWnd		**m_ppWndNotify;			/* �ʒm��E�B���h�E */
	CDlgAdminMapEventNONE	*m_pDlgType;	/* �ҏW���̃C�x���g��ʃ_�C�A���O */
	CInfoMapEventBase		*m_pInfo;		/* �ҏW���̃C�x���g��� */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminMapEventBase)
	enum { IDD = IDD_MAP_EVENT_BASE };
	CComboBox	m_ctlType;
	CComboBox	m_ctlHitType;
	CComboBox	m_ctlDirection;
	int		m_nPosX;
	int		m_nPosY;
	int		m_nPosX2;
	int		m_nPosY2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEventBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEventBase)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeType();
	afx_msg void OnSelchangeHitType();
	afx_msg void OnSelchangeDirection();
	afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminMapEventBase, *PCDlgAdminMapEventBase;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
