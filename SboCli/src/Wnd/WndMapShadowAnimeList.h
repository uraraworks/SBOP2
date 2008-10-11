/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WndMapShadowAnimeList.h									 */
/* ���e			:�}�b�v�e�A�j���[�V�����ꗗ�X�^�e�B�b�N�R���g���[���N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/06													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;
class CInfoMapShadow;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWndMapShadowAnimeList : public CWnd
{
public:
			CWndMapShadowAnimeList();						/* �R���X�g���N�^ */
	virtual ~CWndMapShadowAnimeList();						/* �f�X�g���N�^ */

	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData, int nResourceID, CInfoMapShadow *pInfoMapShadow);	/* �쐬 */
	void	Destroy	(void);									/* �j�� */
	void	Renew	(void);									/* �X�V */


protected:
	void	MakeImage		(void);				/* �摜���쐬 */


protected:
	CWnd		*m_pWndParent;					/* �e�E�B���h�E */
	CImg32		*m_pImgShadow;					/* �o�b�N�o�b�t�@ */

	CMgrData		*m_pMgrData;				/* �f�[�^�Ǘ� */
	CMgrGrpData		*m_pMgrGrpData;				/* �O���t�B�b�N�f�[�^�Ǘ� */
	CInfoMapShadow	*m_pInfoMapShadow;			/* �}�b�v�e��� */



public:
	//{{AFX_VIRTUAL(CWndMapShadowAnimeList)
	public:
	protected:
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndMapShadowAnimeList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CWndMapShadowAnimeList, *PCWndMapShadowAnimeList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
