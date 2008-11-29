/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemList.h											 */
/* ���e			:�A�C�e���ꗗ�_�C�A���O�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/14													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoItemType;
class CLibInfoItem;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminItemList : public CDlgAdminBase
{
public:
			CDlgAdminItemList(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminItemList();							/* �f�X�g���N�^ */

	void	Init		(CMgrData *pMgrData);						/* ������ */
	void	Renew		(void);										/* �ꗗ���X�V */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);			/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */


protected:
	int		GetIndex	(DWORD dwItemID);							/* �A�C�e��ID����C���f�b�N�X���擾 */


protected:
	CWnd				*m_pWndNotify;				/* �ʒm��E�B���h�E */
	CLibInfoItemType	*m_pLibInfoItemType;		/* �A�C�e����ʏ�� */
	CLibInfoItem		*m_pLibInfoItem;			/* �ҏW���̃A�C�e����� */



public:
	//{{AFX_DATA(CDlgAdminItemList)
	enum { IDD = IDD_ITEM_LIST };
	CListCtrl	m_List;
	BOOL m_bDrop;
	BOOL m_bMap;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemList)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnCopy();
	afx_msg void OnDelete();
	afx_msg void OnRenew();
	afx_msg void OnBnClickedDrop();
	afx_msg void OnBnClickedMap();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemList, *PCDlgAdminItemList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
