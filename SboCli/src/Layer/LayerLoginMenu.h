/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LayerLoginMenu.h											 */
/* ���e			:���C���[�`��N���X(���O�C�����j���[) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/15													 */
/* ========================================================================= */

#pragma once

#include "LayerCloud.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLayerLoginMenu : public CLayerCloud
{
public:
			CLayerLoginMenu();						/* �R���X�g���N�^ */
	virtual ~CLayerLoginMenu();						/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);				/* �쐬 */
	void	Draw		(CImg32 *pDst);						/* �`�� */


private:
	DWORD		m_dwLastTimeProc;				/* �ŏI�������� */
	CImg32		*m_pDibBack;					/* �w�i�摜 */
} CLayerLoginMenu, *PCLayerLoginMenu;

/* Copyright(C)URARA-works 2007 */
