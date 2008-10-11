/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapPartsEdit.h										 */
/* ���e			:�}�b�v�p�[�c�ҏW�_�C�A���O�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/14													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CMgrData;
class CWndMapPartsAnimeList;
class CWndMapPartsGrp;
class CInfoMapParts;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CDlgAdminMapPartsEdit : public CDlgAdminBase
{
public:
			CDlgAdminMapPartsEdit(CWnd* pParent = NULL);	/* �R���X�g���N�^ */
	virtual	~CDlgAdminMapPartsEdit();						/* �f�X�g���N�^ */

	int DoModal (CMgrData *pMgrData);								/* ���[�_���_�C�A���O�\�� */


protected:
	void	SetData				(void);					/* �f�[�^����ʂɔ��f */
	void	GetData				(void);					/* ��ʂ���f�[�^���擾 */
	void	SetPartsType		(DWORD dwType);			/* �p�[�c��ʂɉ����ă`�F�b�N�{�b�N�X�̏�Ԃ�ݒ� */
	DWORD	GetPartsType		(void);					/* �`�F�b�N�{�b�N�X�̏�Ԃɉ����ăp�[�c��ʂ��擾 */
	void	OnSelectAnime		(void);					/* �R�}�I������ */
	void	RenewButtonState	(void);					/* �{�^����Ԃ̐ݒ� */


public:
	int						m_nSelectType,				/* �ǂ���̉摜��I�����Ă��邩 */
							m_nAnimeNo;					/* �A�j���[�V�������̔ԍ� */
	WORD					m_wPartsBase,				/* ���n */
							m_wPartsPile;				/* �d�ˍ��킹 */
	DWORD					m_dwTimeLastAnime;			/* �Ō�ɃA�j���[�V������������ */
	SIZE					m_sizeWindow;				/* �\���J�n���̃E�B���h�E�T�C�Y */
	CMgrData				*m_pMgrData;				/* �f�[�^�Ǘ� */
	CMgrGrpData				*m_pMgrGrpData;				/* �O���t�B�b�N�f�[�^�Ǘ� */
	CWndMapPartsAnimeList	*m_pWndMapPartsAnimeList;	/* �}�b�v�p�[�c�A�j���[�V�����ꗗ�E�B���h�E */
	CInfoMapParts			*m_pInfoMapParts;			/* �}�b�v�p�[�c��� */
	CWndMapPartsGrp			*m_pWndMapPartsGrp;			/* �}�b�v�p�[�c�摜�ꗗ */



public:
	//{{AFX_DATA(CDlgAdminMapPartsEdit)
	enum { IDD = IDD_PARTS_EDIT };
	CSliderCtrl	m_Slider;
	CString	m_strCount;
	CString	m_strNow;
	CComboBox	m_cbGrpNo;
	int		m_nGrpNo;
	int		m_nLevel;
	int		m_nViewTime;
	BOOL	m_bTypeBlock;
	BOOL	m_bTypePileBack;
	BOOL	m_bTypePile;
	BOOL	m_bFishing;
	BOOL	m_bBlockDown;
	BOOL	m_bBlockLeft;
	BOOL	m_bBlockRight;
	BOOL	m_bBlockUp;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapPartsEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapPartsEdit)
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
	afx_msg void OnSelchangeGrpNo();
	//}}AFX_MSG
	afx_msg LRESULT OnWndClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
