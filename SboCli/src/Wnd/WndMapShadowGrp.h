/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WndMapShadowGrp.h											 */
/* ���e			:�}�b�v�e�摜�ꗗ�X�^�e�B�b�N�R���g���[���N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/06													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWndMapShadowGrp : public CWnd
{
public:
			CWndMapShadowGrp();						/* �R���X�g���N�^ */
	virtual ~CWndMapShadowGrp();					/* �f�X�g���N�^ */

	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData, int nResourceID);	/* �쐬 */
	void	Destroy	(void);									/* �j�� */


protected:
	void	MakeImage	(void);			/* �摜���쐬 */


protected:
	int			m_nPos;							/* �c�X�N���[���ʒu */
	CWnd		*m_pWndParent;					/* �e�E�B���h�E */
	CImg32		*m_pImgShadow;					/* �o�b�N�o�b�t�@ */
	CPoint		m_ptCursor;						/* �J�[�\���̂���p�[�c�ʒu */

	CMgrData		*m_pMgrData;				/* �f�[�^�Ǘ� */
	CMgrGrpData		*m_pMgrGrpData;				/* �O���t�B�b�N�f�[�^�Ǘ� */



public:
	//{{AFX_VIRTUAL(CWndMapShadowGrp)
	public:
	protected:
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndMapShadowGrp)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CWndMapShadowGrp, *PCWndMapShadowGrp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
