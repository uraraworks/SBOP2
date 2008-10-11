/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	WndSelectMapPartsGrp.h										 */
/* ���e�F		�}�b�v�p�[�c�摜�I���E�B���h�E�N���X ��`�t�@�C��			 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/10/07													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWndSelectMapPartsGrp : public CWnd
{
public:
			CWndSelectMapPartsGrp();							/* �R���X�g���N�^ */
	virtual ~CWndSelectMapPartsGrp();							/* �f�X�g���N�^ */


	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData);	/* �쐬 */
	void	Destroy	(void);									/* �j�� */


protected:
	int			m_nSelect;						/* �I�����ꂽ�p�[�c�ԍ� */
	CWnd		*m_pWndParent;					/* �e�E�B���h�E */
	CImg32		*m_pImgBack;					/* �o�b�N�o�b�t�@ */

	CMgrData		*m_pMgrData;				/* �f�[�^�Ǘ� */
	CMgrGrpData		*m_pMgrGrpData;				/* �O���t�B�b�N�f�[�^�Ǘ� */



public:
	//{{AFX_VIRTUAL(CWndSelectMapPartsGrp)
	public:
	protected:
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndSelectMapPartsGrp)
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
} CWndSelectMapPartsGrp, *PCWndSelectMapPartsGrp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2005 */
