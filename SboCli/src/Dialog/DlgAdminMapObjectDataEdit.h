/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapObjectDataEdit.h								 */
/* ���e			:�}�b�v�I�u�W�F�N�g�z�u�f�[�^�ҏW�_�C�A���O�N���X ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/03													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoMapObject;
class CInfoMapObjectData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminMapObjectDataEdit : public CDlgAdminBase
{
public:
			CDlgAdminMapObjectDataEdit(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual ~CDlgAdminMapObjectDataEdit();						/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData, CWnd **pWndNotify);		/* ������ */
	void	Get			(CInfoMapObjectData *&pDst);					/* �ҏW���e���擾 */
	void	SetModify	(CInfoMapObjectData *pSrc);						/* �ҏW���[�h�Ƃ��Đݒ� */


protected:
	BOOL					m_bModeModify;			/* �ҏW���[�h���� */
	CWnd					**m_ppWndNotify;		/* �ʒm��E�B���h�E */
	CLibInfoMapObject		*m_pLibInfoMapObject;	/* �}�b�v�I�u�W�F�N�g��� */
	CInfoMapObjectData		*m_pInfo;				/* �ҏW���̔z�u�f�[�^ */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminMapObjectDataEdit)
	enum { IDD = IDD_MAP_OBJECTDATA_EDIT };
	CComboBox	m_ctlType;
	int		m_nPosX;
	int		m_nPosY;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapObjectDataEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapObjectDataEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeType();
	afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminMapObjectDataEdit, *PCDlgAdminMapObjectDataEdit;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
