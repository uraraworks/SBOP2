/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WndMapPartsAnimeList.h										 */
/* ���e			:�}�b�v�p�[�c�A�j���[�V�����ꗗ�X�^�e�B�b�N�R���g���[���N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/22													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CMgrGrpData;
class CImg32;
class CInfoMapParts;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWndMapPartsAnimeList : public CWnd
{
public:
			CWndMapPartsAnimeList();						/* �R���X�g���N�^ */
	virtual ~CWndMapPartsAnimeList();						/* �f�X�g���N�^ */

	BOOL	Create	(CWnd *pParent, CMgrData *pMgrData, int nResourceID, CInfoMapParts *pInfoMapParts);	/* �쐬 */
	void	Destroy	(void);									/* �j�� */
	void	Renew	(void);									/* �X�V */


protected:
	void	MakeImage		(void);				/* �摜���쐬 */


protected:
	CWnd		*m_pWndParent;					/* �e�E�B���h�E */
	CImg32		*m_pImgParts;					/* �o�b�N�o�b�t�@ */

	CMgrData		*m_pMgrData;				/* �f�[�^�Ǘ� */
	CMgrGrpData		*m_pMgrGrpData;				/* �O���t�B�b�N�f�[�^�Ǘ� */
	CInfoMapParts	*m_pInfoMapParts;			/* �}�b�v�p�[�c��� */



public:
	//{{AFX_VIRTUAL(CWndMapPartsAnimeList)
	public:
	protected:
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndMapPartsAnimeList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CWndMapPartsAnimeList, *PCWndMapPartsAnimeList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
