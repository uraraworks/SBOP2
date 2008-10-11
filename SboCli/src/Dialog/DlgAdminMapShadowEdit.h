/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapShadowEdit.h									 */
/* ���e			:�}�b�v�e�ҏW�_�C�A���O�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/06													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CMgrData;
class CWndMapShadowAnimeList;
class CWndMapShadowGrp;
class CInfoMapShadow;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CDlgAdminMapShadowEdit : public CDlgAdminBase
{
public:
			CDlgAdminMapShadowEdit(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual	~CDlgAdminMapShadowEdit();						/* �f�X�g���N�^ */

	int DoModal (CMgrData *pMgrData);								/* ���[�_���_�C�A���O�\�� */


protected:
	void	SetData				(void);					/* �f�[�^����ʂɔ��f */
	void	GetData				(void);					/* ��ʂ���f�[�^���擾 */
	void	OnSelectAnime		(void);					/* �R�}�I������ */
	void	RenewButtonState	(void);					/* �{�^����Ԃ̐ݒ� */


public:
	int						m_nSelectType,				/* �ǂ���̉摜��I�����Ă��邩 */
							m_nAnimeNo;					/* �A�j���[�V�������̔ԍ� */
	WORD					m_wShadowGrpID;				/* �e�摜ID */
	DWORD					m_dwTimeLastAnime;			/* �Ō�ɃA�j���[�V������������ */
	SIZE					m_sizeWindow;				/* �\���J�n���̃E�B���h�E�T�C�Y */
	CMgrData				*m_pMgrData;				/* �f�[�^�Ǘ� */
	CMgrGrpData				*m_pMgrGrpData;				/* �O���t�B�b�N�f�[�^�Ǘ� */
	CWndMapShadowAnimeList	*m_pWndMapShadowAnimeList;	/* �}�b�v�e�A�j���[�V�����ꗗ�E�B���h�E */
	CInfoMapShadow			*m_pInfoMapShadow;			/* �}�b�v�e��� */
	CWndMapShadowGrp		*m_pWndMapShadowGrp;		/* �}�b�v�e�摜�ꗗ */



public:
	//{{AFX_DATA(CDlgAdminMapShadowEdit)
	enum { IDD = IDD_SHADOW_EDIT };
	CSliderCtrl	m_Slider;
	CString	m_strCount;
	CString	m_strNow;
	int		m_nLevel;
	int		m_nViewTime;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapShadowEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapShadowEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnChangeLevel();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	afx_msg void OnChangeViewtime();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPlay();
	afx_msg void OnStop();
	//}}AFX_MSG
	afx_msg LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
