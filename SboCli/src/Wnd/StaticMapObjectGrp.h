/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:StaticMapObjectGrp.h										 */
/* ���e			:�}�b�v�I�u�W�F�N�g�\���X�^�e�B�b�N�R���g���[���N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/02													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;
class CInfoMapObject;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CStaticMapObjectGrp : public CStatic
{
public:
			CStaticMapObjectGrp();					/* �R���X�g���N�^ */
	virtual ~CStaticMapObjectGrp();					/* �f�X�g���N�^ */


	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData);	/* �쐬 */
	void	Destroy	(void);									/* �j�� */
	void	Init	(CInfoMapObject *pInfoMapObject);		/* ������ */
	void	Set		(int nAnimeNo);							/* �X�V */
	void	IncPosX	(void);									/* �I���ʒu���E�Ɉړ� */


protected:
	void	RenewGrp(void);									/* �摜�X�V */


public:
	int				m_nAnimeNo;					/* �\������A�j���ԍ� */
	POINT			m_ptSelect;					/* �I���ʒu */
	CWnd			*m_pWndParent;				/* �e�E�B���h�E */
	CImg32			*m_pImgBack;				/* �o�b�N�o�b�t�@ */
	CMgrData		*m_pMgrData;				/* �f�[�^�Ǘ� */
	CMgrGrpData		*m_pMgrGrpData;				/* �O���t�B�b�N�f�[�^�Ǘ� */
	CInfoMapObject	*m_pInfoMapObject;			/* �\������}�b�v�I�u�W�F�N�g��� */



public:
	//{{AFX_VIRTUAL(CStaticMapObjectGrp)
	public:
	protected:
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStaticMapObjectGrp)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CStaticMapObjectGrp, *PCStaticMapObjectGrp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
