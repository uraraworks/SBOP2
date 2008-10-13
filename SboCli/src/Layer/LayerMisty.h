/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LayerMisty.h												 */
/* ���e			:���C���[�`��N���X(��) ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/13													 */
/* ========================================================================= */

#pragma once

#include "LayerBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLayerMisty : public CLayerBase
{
public:
			CLayerMisty();						/* �R���X�g���N�^ */
	virtual ~CLayerMisty();						/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);			/* �쐬 */
	void	Draw		(CImg32 *pDst);					/* �`�� */
} CLayerMisty, *PCLayerMisty;

/* Copyright(C)URARA-works 2008 */
