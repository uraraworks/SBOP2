/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:LayerCharSelect.h											 */
/* ���e			:���C���[�`��N���X(�L�����I��) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/07/04													 */
/* ========================================================================= */

#pragma once

#include "LayerBase.h"

class CImg32;
class CInfoAccount;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLayerCharSelect : public CLayerBase
{
public:
			CLayerCharSelect();									/* �R���X�g���N�^ */
	virtual ~CLayerCharSelect();								/* �f�X�g���N�^ */

	void	Create			(CMgrData *pMgrData, DWORD dwAccountID);	/* �쐬 */
	void	Draw			(CImg32 *pDst);								/* �`�� */
	BOOL	TimerProc		(void);										/* ���ԏ��� */

	void	SetSelect		(int nNo);									/* �I���ʒu��ݒ� */
	DWORD	GetSelectCharID	(void);										/* �I�𒆂̃L����ID���擾 */


private:
	void DrawFrame			(CImg32 *pDst, int x, int y, int cx, int cy, int nType);	/* �t���[����`�� */


private:
	int				m_nSelect;						/* �I���ʒu */
	BYTE			m_byAnime;						/* �A�j���[�V�����ԍ� */
	DWORD			m_dwTimeLastAnime;				/* �O��̃A�j���[�V������������ */

	CInfoAccount	*m_pInfoAccount;				/* �A�J�E���g��� */
} CLayerCharSelect, *PCLayerCharSelect;

/* Copyright(C)URARA-works 2005 */
