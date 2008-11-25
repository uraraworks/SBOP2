/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WndMap.h													 */
/* ���e			:�}�b�v�E�B���h�E�N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/09/12													 */
/* ========================================================================= */

#pragma once

#include "WndToolbar.h"

class CMgrData;
class CMgrGrpData;
class CImg32;
class CMgrDraw;
class CInfoMapBase;
class CUraraSockTCPSBO;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWndMap : public CWnd
{
public:
			CWndMap();									/* �R���X�g���N�^ */
	virtual ~CWndMap();									/* �f�X�g���N�^ */

	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData, int nNotify);	/* �쐬 */
	void	Destroy	(void);												/* �j�� */


protected:
	void	RenewScrollRange	(int cx, int cy);			/* �X�N���[���o�[�͈̔͂��X�V */
	void	RenewToolbar		(void);						/* �c�[���o�[���X�V */
	void	RenewStatusText		(void);						/* �X�e�[�^�X�o�[�̃e�L�X�g���X�V */
	BOOL	GetCheck			(int nNo);					/* �c�[���o�[�̃{�^����Ԃ��擾 */
	void	SetCheck			(int nNo, BOOL bCheck);		/* �c�[���o�[�̃{�^����Ԃ�ݒ� */


protected:
	BOOL		m_bRClickFirst,					/* �E�N���b�N���ꂽ */
				m_bViewGrid;					/* �O���b�h�\�� */
	int			m_nSelect,						/* �I�����ꂽ�p�[�c�ԍ� */
				m_nNotify;						/* �ʒm�f�[�^ */
	DWORD		*m_pdwParts;					/* �R�s�[���̃}�b�v */
	CWnd		*m_pWndParent;					/* �e�E�B���h�E */
	CImg32		*m_pImgBack,					/* �o�b�N�o�b�t�@ */
				*m_pImgBase;					/* �\���摜 */
	CWndToolbar	m_wndToolBar;					/* �c�[���o�[ */
	CStatusBar	m_StatusBar;					/* �X�e�[�^�X�o�[ */
	CImageList	m_ImgListToolBar;				/* �c�[���o�[�p�C���[�W���X�g */
	CRect		m_rcRange,						/* �I��͈� */
				m_rcCopy;						/* �R�s�[����͈� */
	POINT		m_ptBack;						/* �O��̃}�E�X�ʒu */

	CMgrData			*m_pMgrData;			/* �f�[�^�Ǘ� */
	CMgrGrpData			*m_pMgrGrpData;			/* �O���t�B�b�N�f�[�^�Ǘ� */
	CMgrDraw			*m_pMgrDraw;			/* �`��}�l�[�W�� */
	CInfoMapBase		*m_pInfoMap;			/* �}�b�v��� */
	CUraraSockTCPSBO	*m_pSock;				/* �ʐM�}�l�[�W�� */



public:
	//{{AFX_VIRTUAL(CWndMap)
	public:
	protected:
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndMap)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateCommandUI(CCmdUI* pCmdUI);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnCopy();
	afx_msg void OnPaste();
	afx_msg void OnToolPaint();
	afx_msg void OnRange();
	afx_msg void OnGrid();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CWndMap, *PCWndMap;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
