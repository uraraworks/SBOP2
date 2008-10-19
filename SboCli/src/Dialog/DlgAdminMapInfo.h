/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapInfo.h											 */
/* ���e			:�}�b�v���ҏW�_�C�A���O�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/01													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoMapBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminMapInfo : public CDlgAdminBase
{
public:
			CDlgAdminMapInfo(CWnd* pParent = NULL);			/* �R���X�g���N�^ */
	virtual ~CDlgAdminMapInfo();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);						/* ������ */
	void	Renew		(void);										/* �X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:
	CInfoMapBase	*m_pInfoMap;				/* �ҏW���̃}�b�v��� */



public:
	//{{AFX_DATA(CDlgAdminMapInfo)
	enum { IDD = IDD_MAP_INFO };
	CString	m_strMapSize;
	int		m_nValue;
	int		m_nDarkLevel;
	CString	m_strMapID;
	CString	m_strMapName;
	CComboBox	m_cmbBGMID;
	CComboBox	m_cmbWeatherType;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnChangemapname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminMapInfo, *PCDlgAdminMapInfo;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
