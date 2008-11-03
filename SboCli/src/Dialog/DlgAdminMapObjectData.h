/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapObjectData.h									 */
/* ���e			:�}�b�v�I�u�W�F�N�g�z�u�f�[�^�ꗗ�_�C�A���O�N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/03													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoMapBase;
class CLibInfoMapObject;
class CLibInfoMapObjectData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminMapObjectData : public CDlgAdminBase
{
public:
			CDlgAdminMapObjectData(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminMapObjectData();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);							/* ������ */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */
	void	Renew		(void);											/* �ꗗ���X�V */


protected:
	CWnd					*m_pWndNotify;				/* �ʒm��E�B���h�E */
	CInfoMapBase			*m_pInfoMap;				/* �ҏW���̃}�b�v��� */
	CLibInfoMapObject		*m_pLibInfoMapObject;		/* �}�b�v�I�u�W�F�N�g�f�[�^���C�u���� */
	CLibInfoMapObjectData	*m_pLibInfoMapObjectData;	/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^���C�u���� */



public:
	//{{AFX_DATA(CDlgAdminMapObjectData)
	enum { IDD = IDD_MAP_OBJECTDATA };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapObjectData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapObjectData)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnCopy();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminMapObjectData, *PCDlgAdminMapObjectData;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
