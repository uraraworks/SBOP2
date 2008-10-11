/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:LayerInfo.h												 */
/* ���e			:���C���[�`��N���X(���m�点) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/08/16													 */
/* ========================================================================= */

#pragma once

#include "LayerBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLayerInfo : public CLayerBase
{
public:
			CLayerInfo();							/* �R���X�g���N�^ */
	virtual ~CLayerInfo();							/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);				/* �쐬 */
	void	Draw		(CImg32 *pDst);						/* �`�� */


private:
	void	DrawFrame	(CImg32 *pDst);						/* �t���[���̕`�� */


private:
	int		m_nX;					/* �\���ʒu(��) */
	int		m_nY;					/* �\���ʒu(�c) */
	int		m_nCx;					/* �� */
	int		m_nCy;					/* ���� */
} CLayerInfo, *PCLayerInfo;

/* Copyright(C)URARA-works 2005 */
