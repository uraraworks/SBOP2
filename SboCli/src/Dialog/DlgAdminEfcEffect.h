/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminEfcEffect.h										 */
/* ���e			:�G�t�F�N�g���ݒ�_�C�A���O�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/06													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "StaticGrp.h"

class CMgrGrpData;
class CWndSelectGrp;
class CInfoEffect;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminEfcEffect : public CDlgAdminBase
{
public:
			CDlgAdminEfcEffect(CWnd* pParent = NULL);			/* �R���X�g���N�^ */
	virtual ~CDlgAdminEfcEffect();								/* �f�X�g���N�^ */

	void	Init	(CMgrData *pMgrData, BOOL bModify = FALSE);			/* ������ */
	void	GetInfo	(CInfoEffect *pDst);								/* �擾 */
	void	SetInfo	(CInfoEffect *pSrc);								/* �ݒ� */


protected:
	void	RenewSlider		(void);							/* �X���C�_�[���X�V */
	void	OnSelectAnime	(void);							/* �R�}�I������ */
	void	Enable			(BOOL bEnable);					/* ���͋֎~�ݒ� */
	void	Set				(int nNo);						/* �G�t�F�N�g���֐ݒ� */
	void	Get				(int nNo);						/* �G�t�F�N�g��񂩂�擾 */


protected:
	BOOL			m_bModify;						/* �ҏW���[�h */
	int				m_nSelect;						/* �I�𒆂̔ԍ� */
	DWORD			m_dwLastTimeAnime;				/* �O��̃A�j���[�V�������� */
	CWndSelectGrp	*m_pWndSelectGrp;				/* �摜�I���E�B���h�E */
	CInfoEffect		*m_pInfoEffect;					/* �ҏW���̃G�t�F�N�g��� */



public:
	void OnOK ();
	//{{AFX_DATA(CDlgAdminEfcEffect)
	enum { IDD = IDD_EFC_EFFECT };
	CSliderCtrl	m_ctlSlider;
	CComboBox	m_cmbSoundID;
	CComboBox	m_cmbType;
	CStaticGrp	m_ctlGrp;
	BOOL	m_bLoop;
	BOOL	m_bLoopSound;
	int		m_nViewTime;
	int		m_nLevel;
	CString	m_strCount;
	CString	m_strNow;
	CString	m_strName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminEfcEffect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminEfcEffect)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeType();
	//}}AFX_MSG
	LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
} CDlgAdminEfcEffect, *PCDlgAdminEfcEffect;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
