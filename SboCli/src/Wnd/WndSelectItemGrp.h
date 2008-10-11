/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WndSelectItemGrp.h											 */
/* ���e			:�A�C�e���摜�I���E�B���h�E�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/21													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWndSelectItemGrp : public CWnd
{
public:
			CWndSelectItemGrp();					/* �R���X�g���N�^ */
	virtual ~CWndSelectItemGrp();					/* �f�X�g���N�^ */

	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData);	/* �쐬 */
	void	Destroy	(void);									/* �j�� */


protected:
	int			m_nSelect;						/* �I�����ꂽ�p�[�c�ԍ� */
	CWnd		*m_pWndParent;					/* �e�E�B���h�E */
	CImg32		*m_pImgBack;					/* �o�b�N�o�b�t�@ */

	CMgrData		*m_pMgrData;				/* �f�[�^�Ǘ� */
	CMgrGrpData		*m_pMgrGrpData;				/* �O���t�B�b�N�f�[�^�Ǘ� */



public:
	//{{AFX_VIRTUAL(CWndSelectItemGrp)
	public:
	protected:
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndSelectItemGrp)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CWndSelectItemGrp, *PCWndSelectItemGrp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
