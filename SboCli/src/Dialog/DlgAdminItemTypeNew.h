/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemTypeNew.h										 */
/* ���e			:�A�C�e����ʐV�K�ǉ��_�C�A���O�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/30													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoItemTypeBase;
class CMgrGrpData;
class CWndSelectItemGrp;
class CLibInfoItemType;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminItemTypeNew : public CDlgAdminBase
{
public:
			CDlgAdminItemTypeNew(CWnd* pParent = NULL);			/* �R���X�g���N�^ */
	virtual ~CDlgAdminItemTypeNew();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData, CWnd **pWndNotify, BOOL bPosSet = TRUE);	/* ������ */
	void	Get			(CInfoItemTypeBase *&pDst);						/* �ҏW���e���擾 */
	void	SetModify	(CInfoItemTypeBase *pInfoItemType);				/* �ҏW���[�h�Ƃ��Đݒ� */


protected:
	void	SetGrpIDMain	(DWORD dwItemTypeID);	/* �������C���摜ID�R���{�{�b�N�X��ݒ� */


protected:
	DWORD				m_dwSelectItemTypeID,	/* ��� */
						m_dwSelectSoundID,		/* ���Ƃ����Ƃ��̌��ʉ�ID */
						m_dwUseEffectID,		/* �g�������ɍĐ�����G�t�F�N�gID */
						m_dwUseSoundID,			/* �g�������ɍĐ�������ʉ�ID */
						m_dwSelectGrpIDMain;	/* �������C���摜ID */
	BOOL				m_bModeModify;			/* �ҏW���[�h���� */
	CWnd				**m_ppWndNotify;		/* �ʒm��E�B���h�E */
	CMgrGrpData			*m_pMgrGrpData;			/* �O���t�B�b�N�f�[�^�Ǘ� */
	CWndSelectItemGrp	*m_pWndSelectItemGrp;	/* �A�C�e���摜�I���E�B���h�E */
	CLibInfoItemType	*m_pLibInfoItemType;	/* �A�C�e����ʏ�񃉃C�u���� */
	CInfoItemTypeBase	*m_pInfoItemType;		/* �ҏW���̃A�C�e����ʏ�� */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminItemTypeNew)
	enum { IDD = IDD_ITEMTYPE_NEW };
	CComboBox	m_ctlTypeGrpIDMain;
	CComboBox	m_ctlDropSoundID;
	CComboBox	m_ctlUseSoundID;
	CComboBox	m_ctlUseEffectID;
	CComboBox	m_ctlType;
	CString	m_strName;
	int		m_nDelAverage;
	int		m_nGrpID;
	BOOL	m_bPutOn;
	int		m_nGrpIDSub;
	int		m_nIconGrpID;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemTypeNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemTypeNew)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectItemgrp();
	afx_msg void OnPlay();
	afx_msg void OnSelchangeType();
	afx_msg void OnTypeinfo();
	//}}AFX_MSG
	afx_msg LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemTypeNew, *PCDlgAdminItemTypeNew;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
