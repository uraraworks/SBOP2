/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	LayerLogo.h													 */
/* ���e�F		���C���[�`��N���X(���S) ��`�t�@�C��						 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/05/17													 */
/* ========================================================================= */

#pragma once

#include "LayerBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLayerLogo : public CLayerBase
{
public:
			CLayerLogo();								/* �R���X�g���N�^ */
	virtual ~CLayerLogo();								/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);				/* �쐬 */
	void	Draw		(CImg32 *pDst);						/* �`�� */


private:
	CImg32			*m_pDibLogo;						/* ���S�摜 */
} CLayerLogo, *PCLayerLogo;

/* Copyright(C)URARA-works 2005 */
