/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharModifyTypePUTNPC.h								 */
/* ���e			:�L������� �ړ���ʂ̐ݒ�(NPC����)�_�C�A���O�N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/12													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminCharModifyTypeBase.h"

class CInfoCharCli;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDlgAdminCharModifyTypePUTNPC : public CDlgAdminCharModifyTypeBase
{
public:
			CDlgAdminCharModifyTypePUTNPC(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminCharModifyTypePUTNPC();							/* �f�X�g���N�^ */

	virtual void	Set	(CInfoCharCli *pSrc);									/* �ݒ� */
	virtual void	Get	(CInfoCharCli *pDst);									/* �擾 */


protected:
	int	m_nPutMoveType;			/* �ړ���� */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminCharModifyTypePUTNPC)
	enum { IDD = IDD_CHAR_MODIFY_TYPE_PUTNPC };
	CComboBox	m_ctlMoveType;
	DWORD m_dwPutCycle;
	int	m_nMaxPutCount;
	int	m_nPutAverage;
	int	m_nPutAreaX;
	int	m_nPutAreaY;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharModifyTypePUTNPC)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharModifyTypePUTNPC)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharModifyTypePUTNPC, *PCDlgAdminCharModifyTypePUTNPC;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
