/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminEfcBalloon.h										 */
/* ���e			:���o�����ݒ�_�C�A���O�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/25													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "StaticGrp.h"

class CMgrGrpData;
class CWndSelectGrp;
class CLibInfoEfcBalloon;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminEfcBalloon : public CDlgAdminBase
{
public:
			CDlgAdminEfcBalloon(CWnd* pParent = NULL);			/* �R���X�g���N�^ */
	virtual ~CDlgAdminEfcBalloon();								/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData, DWORD dwListID, BOOL bModify = FALSE);	/* ������ */
	void	GetList		(CLibInfoEfcBalloon *pDst);						/* �擾 */
	void	SetList		(CLibInfoEfcBalloon *pSrc);						/* �ݒ� */
	DWORD	GetListID	(void)	{ return m_dwListID; }					/* ���ID���擾 */


protected:
	void	RenewSlider		(void);							/* �X���C�_�[���X�V */
	void	OnSelectAnime	(void);							/* �R�}�I������ */
	void	Enable			(BOOL bEnable);					/* ���͋֎~�ݒ� */
	void	Set				(int nNo);						/* ���o�����֐ݒ� */
	void	Get				(int nNo);						/* ���o����񂩂�擾 */


protected:
	BOOL				m_bModify;					/* �ҏW���[�h */
	int					m_nSelect;					/* �I�𒆂̔ԍ� */
	DWORD				m_dwLastTimeAnime;			/* �O��̃A�j���[�V�������� */
	CWndSelectGrp		*m_pWndSelectGrp;			/* �摜�I���E�B���h�E */
	CLibInfoEfcBalloon	*m_pLibInfoEfcBalloon;		/* �ҏW���̕��o����� */



public:
	void OnOK ();
	//{{AFX_DATA(CDlgAdminEfcBalloon)
	enum { IDD = IDD_EFC_BALLOON };
	CSliderCtrl	m_ctlSlider;
	CComboBox	m_cmbSoundID;
	CStaticGrp	m_ctlGrp;
	BOOL	m_bLoop;
	int		m_nViewTime;
	DWORD	m_dwListID;
	CString	m_strCount;
	CString	m_strNow;
	CString	m_strName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminEfcBalloon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminEfcBalloon)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
} CDlgAdminEfcBalloon, *PCDlgAdminEfcBalloon;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
