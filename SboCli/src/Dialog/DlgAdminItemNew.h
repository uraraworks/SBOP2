/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemNew.h											 */
/* ���e			:�A�C�e���V�K�ǉ��_�C�A���O�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/15													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoItemType;
class CInfoItem;
class CMgrGrpData;
class CWndSelectItemGrp;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminItemNew : public CDlgAdminBase
{
public:
			CDlgAdminItemNew(CWnd* pParent = NULL);			/* �R���X�g���N�^ */
	virtual ~CDlgAdminItemNew();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData, CWnd **pWndNotify, BOOL bPosSet = TRUE);	/* ������ */
	void	Get			(CInfoItem *pDst);							/* �ҏW���e���擾 */
	void	SetModify	(CInfoItem *pInfoItem);						/* �ҏW���[�h�Ƃ��Đݒ� */


protected:
	void	SelectSound	(DWORD dwSoundID);					/* ���Ƃ����Ƃ��̌��ʉ�ID��I�� */


protected:
	DWORD				m_dwSelectItemTypeID,	/* ��� */
						m_dwSelectSoundID;		/* ���Ƃ����Ƃ��̌��ʉ�ID */
	BOOL				m_bModeModify,			/* �ҏW���[�h���� */
						m_bPosSet;				/* �ݒu�ʒu��ݒ�ł��邩���� */
	CWnd				**m_ppWndNotify;		/* �ʒm��E�B���h�E */
	CMgrGrpData			*m_pMgrGrpData;			/* �O���t�B�b�N�f�[�^�Ǘ� */
	CWndSelectItemGrp	*m_pWndSelectItemGrp;	/* �A�C�e���摜�I���E�B���h�E */
	CLibInfoItemType	*m_pLibInfoItemType;	/* �A�C�e����ʏ�񃉃C�u���� */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminItemNew)
	enum { IDD = IDD_ITEM_NEW };
	CComboBox	m_ctlDropSoundID;
	CComboBox	m_ctlType;
	CString	m_strName;
	int		m_nMapID;
	int		m_nPosX;
	int		m_nPosY;
	int		m_nGrpID;
	BOOL	m_bPutOn;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemNew)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectItemgrp();
	afx_msg void OnPlay();
	afx_msg void OnSelchangeType();
	//}}AFX_MSG
	afx_msg LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemNew, *PCDlgAdminItemNew;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
