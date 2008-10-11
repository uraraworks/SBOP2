/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WndSelectGrp.h												 */
/* ���e			:�摜�I���E�B���h�E�N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/30													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWndSelectGrp : public CWnd
{
public:
			CWndSelectGrp();						/* �R���X�g���N�^ */
	virtual ~CWndSelectGrp();						/* �f�X�g���N�^ */

	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData, int nNotify);	/* �쐬 */
	void	Destroy	(void);									/* �j�� */

	void	SetType	(int nType, int nGrpIDSub = 0);			/* �摜�^�C�v�̐ݒ� */


protected:
	void	RenewScrollRange	(int cx, int cy);			/* �X�N���[���o�[�͈̔͂��X�V */
	void	RenewGrp			(int nType);				/* �\���摜�̍X�V */


protected:
	int			m_nSelect,						/* �I�����ꂽ�p�[�c�ԍ� */
				m_nNotify,						/* �ʒm�f�[�^ */
				m_nType,						/* �摜�^�C�v */
				m_nGrpSizeX,					/* 1�摜�̉��T�C�Y */
				m_nGrpCountX,					/* �摜�P�t�@�C���̉��� */
				m_nGrpIDSub;					/* �O���t�B�b�N�T�uID */
	CWnd		*m_pWndParent;					/* �e�E�B���h�E */
	CImg32		*m_pImgBack,					/* �o�b�N�o�b�t�@ */
				*m_pImgBase;					/* �\���摜 */

	CMgrData		*m_pMgrData;				/* �f�[�^�Ǘ� */
	CMgrGrpData		*m_pMgrGrpData;				/* �O���t�B�b�N�f�[�^�Ǘ� */



public:
	//{{AFX_VIRTUAL(CWndSelectGrp)
	public:
	protected:
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndSelectGrp)
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
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CWndSelectGrp, *PCWndSelectGrp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
