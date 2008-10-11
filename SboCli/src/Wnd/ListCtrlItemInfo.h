/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:ListCtrlItemInfo.h											 */
/* ���e			:�A�C�e���ꗗ���X�g�R���g���[���N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/24													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CLibInfoItem;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CListCtrlItemInfo : public CListCtrl
{
public:
			CListCtrlItemInfo();						/* �R���X�g���N�^ */
	virtual ~CListCtrlItemInfo();						/* �f�X�g���N�^ */


	BOOL	Create		(CWnd *pParent, CMgrData *pMgrData);	/* �쐬 */
	void	Renew		(void);									/* �ꗗ���X�V */


protected:


protected:
	CWnd			*m_pWndParent;			/* �e�E�B���h�E */
	CMgrData		*m_pMgrData;			/* �f�[�^�Ǘ� */
	CLibInfoItem	*m_pLibInfoItem;		/* �A�C�e����� */



public:
	//{{AFX_VIRTUAL(CListCtrlItemInfo)
	public:
	protected:
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CListCtrlItemInfo)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CListCtrlItemInfo, *PCListCtrlItemInfo;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
