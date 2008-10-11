/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:ListCtrlEffectInfo.h										 */
/* ���e			:�G�t�F�N�g�ꗗ���X�g�R���g���[���N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CLibInfoEffect;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CListCtrlEffectInfo : public CListCtrl
{
public:
			CListCtrlEffectInfo();						/* �R���X�g���N�^ */
	virtual ~CListCtrlEffectInfo();						/* �f�X�g���N�^ */


	BOOL	Create		(CWnd *pParent, CMgrData *pMgrData);	/* �쐬 */
	void	Renew		(void);									/* �ꗗ���X�V */


protected:


protected:
	CWnd			*m_pWndParent;			/* �e�E�B���h�E */
	CMgrData		*m_pMgrData;			/* �f�[�^�Ǘ� */
	CLibInfoEffect	*m_pLibInfoEffect;		/* �G�t�F�N�g��� */



public:
	//{{AFX_VIRTUAL(CListCtrlEffectInfo)
	public:
	protected:
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CListCtrlEffectInfo)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CListCtrlEffectInfo, *PCListCtrlEffectInfo;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
