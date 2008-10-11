/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:StaticMapParts.h											 */
/* ���e			:�}�b�v�p�[�c�ꗗ�X�^�e�B�b�N�R���g���[���N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/10													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;
class CLibInfoMapParts;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CStaticMapParts : public CStatic
{
public:
			CStaticMapParts();						/* �R���X�g���N�^ */
	virtual ~CStaticMapParts();						/* �f�X�g���N�^ */


	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData);	/* �쐬 */
	void	Destroy	(void);									/* �j�� */

	void	Renew			(void);							/* �p�[�c�ꗗ�摜���쐬���čĕ`�� */
	void	SetScrollPos	(int nPos);						/* �c�X�N���[���ʒu��ݒ� */

	void	OnLButtonDown	(CPoint point);					/* ���b�Z�[�W�n���h��(WM_LBUTTONDOWN) */
	void	OnRButtonDown	(CPoint point);					/* ���b�Z�[�W�n���h��(WM_RBUTTONDOWN) */
	void	OnMouseWheel	(short zDelta, CPoint pt);		/* ���b�Z�[�W�n���h��(WM_MOUSEWHEEL) */
	void	OnMouseMove		(CPoint point);					/* ���b�Z�[�W�n���h��(WM_MOUSEMOVE) */


protected:
	void	MakePartsImage		(void);				/* �p�[�c�ꗗ�摜���쐬 */


protected:
	int			m_nPos;							/* �c�X�N���[���ʒu */
	CWnd		*m_pWndParent;					/* �e�E�B���h�E */
	CImg32		*m_pImgParts;					/* �o�b�N�o�b�t�@ */
	CPoint		m_ptCursor,						/* �J�[�\���̂���p�[�c�ʒu */
				m_ptMoveSrc,					/* �ړ����̈ʒu */
				m_ptMoveDst;					/* �ړ���̈ʒu */

	CMgrData			*m_pMgrData;			/* �f�[�^�Ǘ� */
	CMgrGrpData			*m_pMgrGrpData;			/* �O���t�B�b�N�f�[�^�Ǘ� */
	CLibInfoMapParts	*m_pLibInfoMapParts;	/* �}�b�v�p�[�c��� */



public:
	//{{AFX_VIRTUAL(CStaticMapParts)
	public:
	protected:
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStaticMapParts)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CStaticMapParts, *PCStaticMapParts;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
