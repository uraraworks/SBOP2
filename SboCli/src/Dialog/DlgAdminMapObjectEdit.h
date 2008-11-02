/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapObjectEdit.h									 */
/* ���e			:�}�b�v�I�u�W�F�N�g�ҏW�_�C�A���O�N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/09/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "StaticMapObjectGrp.h"
#include "afxcmn.h"

class CMgrData;
class CWndMapPartsGrp;
class CInfoMapObject;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CDlgAdminMapObjectEdit : public CDlgAdminBase
{
public:
			CDlgAdminMapObjectEdit(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual	~CDlgAdminMapObjectEdit();						/* �f�X�g���N�^ */

	void	SetData	(CInfoMapObject *pSrc);							/* �ҏW�p�Ƀf�[�^��ݒ� */
	void	GetData	(CInfoMapObject *pDst);							/* �ҏW�����f�[�^���擾 */


protected:
	void	Set				(void);				/* ��ʂɔ��f */
	void	Get				(void);				/* ��ʂ���擾 */
	void	OnSelectAnime	(void);				/* �R�}�I������ */


public:
	CWndMapPartsGrp		*m_pWndMapPartsGrp;		/* �}�b�v�p�[�c�摜�ꗗ */
	CInfoMapObject		*m_pInfoMapObject;		/* �ҏW���̃}�b�v�I�u�W�F�N�g��� */



public:
	//{{AFX_DATA(CDlgAdminMapObjectEdit)
	enum { IDD = IDD_MAP_OBJECT_EDIT };
	CComboBox	m_cbGrpNo;
	int		m_nGrpNo;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapObjectEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapObjectEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeGrpNo();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl	m_Slider;
	CStaticMapObjectGrp m_ctlObject;
	CString m_strName;
	int m_nCx;
	int m_nCy;
	afx_msg void OnEnChangeCx();
	afx_msg void OnEnChangeCy();
	afx_msg void OnBnClickedAdd();
	int m_nCount;
	CSpinButtonCtrl m_ctlSpinX;
	CSpinButtonCtrl m_ctlSpinY;
	int m_nNow;
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
