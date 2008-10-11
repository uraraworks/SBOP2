/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminSelectItem.h										 */
/* ���e			:�A�C�e���I���_�C�A���O�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/24													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "ListCtrlItemInfo.h"

class CLibInfoItem;
class CInfoItem;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminSelectItem : public CDlgAdminBase
{
public:
			CDlgAdminSelectItem(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminSelectItem();							/* �f�X�g���N�^ */

	DWORD	GetSelectItemID	(void);			/* �I�����ꂽ�A�C�e��ID���擾 */


protected:
	DWORD	m_dwSelectItemID;				/* �I�����ꂽ�A�C�e��ID */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminSelectItem)
	enum { IDD = IDD_SELECT_ITEM };
	CListCtrlItemInfo	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminSelectItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminSelectItem)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminSelectItem, *PCDlgAdminSelectItem;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
