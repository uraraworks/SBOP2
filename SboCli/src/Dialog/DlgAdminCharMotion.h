/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharMotion.h										 */
/* ���e			:�L�������[�V�����ݒ�_�C�A���O�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/27													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "StaticGrp.h"
#include "afxcmn.h"
#include "afxwin.h"

class CMgrGrpData;
class CWndSelectGrp;
class CLibInfoMotion;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharMotion : public CDlgAdminBase
{
public:
			CDlgAdminCharMotion(CWnd* pParent = NULL);			/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharMotion();							/* �f�X�g���N�^ */

	void	Init	(CMgrData *pMgrData, DWORD dwMotionTypeID, DWORD dwMotionListID, int nGrpIDSub);	/* ������ */
	void	GetList	(CLibInfoMotion *pDst);								/* �擾 */
	void	SetList	(CLibInfoMotion *pSrc);								/* �ݒ� */


protected:
	void	RenewSlider		(void);							/* �X���C�_�[���X�V */
	void	RenewPreview	(void);							/* �v���r���[�摜���X�V */
	void	OnSelectAnime	(void);							/* �R�}�I������ */
	void	Enable			(BOOL bEnable);					/* ���͋֎~�ݒ� */
	void	Set				(int nNo);						/* ���[�V�������֐ݒ� */
	void	Get				(int nNo);						/* ���[�V������񂩂�擾 */


protected:
	int				m_nSelect,				/* �I�𒆂̔ԍ� */
					m_nGrpIDSub;			/* �O���t�B�b�N�T�uID */
	DWORD			m_dwMotionTypeID,		/* �ҏW���̃��[�V�������ID */
					m_dwMotionListID,		/* �ҏW���̃��[�V�������X�gID */
					m_dwLastTimeAnime;		/* �O��̃A�j���[�V�������� */
	CWndSelectGrp	*m_pWndSelectGrp;		/* �摜�I���E�B���h�E */
	CLibInfoMotion	*m_pLibInfoMotion;		/* �ҏW���̃��[�V������� */



public:
	void OnOK ();
	//{{AFX_DATA(CDlgAdminCharMotion)
	enum { IDD = IDD_CHAR_MOTION };
	CSliderCtrl	m_ctlSlider;
	CComboBox	m_cmbTypePile3;
	CComboBox	m_cmbTypePile2;
	CComboBox	m_cmbTypePile1;
	CComboBox	m_cmbTypeChar;
	CComboBox	m_cmbSoundID;
	CComboBox	m_cmbProcID;
	CStaticGrp	m_ctlGrpPreview;
	CStaticGrp	m_ctlGrpPile3;
	CStaticGrp	m_ctlGrpPile2;
	CStaticGrp	m_ctlGrpPile1;
	CStaticGrp	m_ctlGrpChar;
	BOOL	m_bLoop;
	BOOL	m_bPile;
	BOOL	m_bRedrawHand;
	int		m_nViewTime;
	int		m_nLevel1;
	int		m_nLevel2;
	int		m_nLevel3;
	int m_nViewPosX0;
	int m_nViewPosY0;
	int m_nViewPosX1;
	int m_nViewPosY1;
	int m_nViewPosX2;
	int m_nViewPosY2;
	int m_nViewPosX3;
	int m_nViewPosY3;
	CString	m_strCount;
	CString	m_strNow;
	CSpinButtonCtrl m_ctlSpinX0;
	CSpinButtonCtrl m_ctlSpinY0;
	CSpinButtonCtrl m_ctlSpinX1;
	CSpinButtonCtrl m_ctlSpinY1;
	CSpinButtonCtrl m_ctlSpinX2;
	CSpinButtonCtrl m_ctlSpinY2;
	CSpinButtonCtrl m_ctlSpinX3;
	CSpinButtonCtrl m_ctlSpinY3;
	CListBox m_ctlDrawList;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharMotion)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharMotion)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeTypeChar();
	afx_msg void OnSelchangeTypePile1();
	afx_msg void OnSelchangeTypePile2();
	afx_msg void OnSelchangeTypePile3();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnChangeViewpos();
	afx_msg void OnBnClickedDrawUp();
	afx_msg void OnBnClickedDrawDown();
	//}}AFX_MSG
	LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharMotion, *PCDlgAdminCharMotion;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
