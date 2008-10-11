/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:StaticGrp.h												 */
/* ���e			:�摜�\���X�^�e�B�b�N�R���g���[���N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/11/10													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CStaticGrp : public CStatic
{
public:
			CStaticGrp();							/* �R���X�g���N�^ */
	virtual ~CStaticGrp();							/* �f�X�g���N�^ */


	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData);	/* �쐬 */
	void	Destroy	(void);									/* �j�� */

	void	Init		(CImg32 *pSrc);						/* ������ */
	void	Init		(DWORD dwGrpIDMain);				/* ������ */
	void	Set			(DWORD dwGrpIDSub);					/* �摜�ݒ� */
	void	SetParam	(DWORD dwGrpIDParam);				/* �摜�p�����[�^�ݒ� */
	DWORD	GetIDMain	(void)	{ return m_dwGrpIDMain;	}	/* �摜ID���C�����擾 */
	DWORD	GetIDSub	(void)	{ return m_dwGrpIDSub;	}	/* �摜ID�T�u���擾 */


protected:
	void	RenewGrp	(DWORD dwGrpIDMain, DWORD dwGrpIDSub);	/* �摜�X�V */


protected:
	DWORD			m_dwGrpIDMain,				/* �摜ID���C�� */
					m_dwGrpIDSub,				/* �摜ID�T�u */
					m_dwGrpIDParam;				/* �摜�p�����[�^ */
	CWnd			*m_pWndParent;				/* �e�E�B���h�E */
	CImg32			*m_pImgBack;				/* �o�b�N�o�b�t�@ */
	CMgrData		*m_pMgrData;				/* �f�[�^�Ǘ� */
	CMgrGrpData		*m_pMgrGrpData;				/* �O���t�B�b�N�f�[�^�Ǘ� */



public:
	//{{AFX_VIRTUAL(CStaticGrp)
	public:
	protected:
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStaticGrp)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CStaticGrp, *PCStaticGrp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
