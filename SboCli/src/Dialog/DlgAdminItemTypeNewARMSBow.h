/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemTypeNewARMSBow.h								 */
/* ���e			:�A�C�e�����(������[�|�p�̐ݒ�])�ݒ�_�C�A���O�N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/08/29													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoItemTypeBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CDlgAdminItemTypeNewARMSBow : public CDlgAdminBase
{
	DECLARE_DYNAMIC(CDlgAdminItemTypeNewARMSBow)

public:
			CDlgAdminItemTypeNewARMSBow(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CDlgAdminItemTypeNewARMSBow();							/* �f�X�g���N�^ */

	void	Set		(CInfoItemTypeBase *pSrc);								/* �ҏW���e��ݒ� */
	void	Get		(CInfoItemTypeBase *&pDst);								/* �ҏW���e���擾 */


public:
	enum { IDD = IDD_ITEMTYPE_NEW_ARMS_BOW };
	DWORD m_dwMoveWait;
	DWORD m_dwMoveCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};

/* Copyright(C)URARA-works 2009 */
