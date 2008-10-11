/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:LayerCloud.h												 */
/* ���e			:���C���[�`��N���X(�_) ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/09/08													 */
/* ========================================================================= */

#pragma once

#include "LayerBase.h"

class CImg32;
class CMgrData;
class CLayerMap;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLayerCloud : public CLayerBase
{
public:
			CLayerCloud();								/* �R���X�g���N�^ */
	virtual ~CLayerCloud();								/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);			/* �쐬 */
	void	Draw		(CImg32 *pDst);					/* �`�� */
	BOOL	TimerProc	(void);							/* ���ԏ��� */


protected:
	CLayerMap		*m_pLayerMap;						/* �}�b�v���C���[ */


private:
	int				m_nCount,							/* �ړ��J�E���^ */
					m_nPosXBack,						/* ��_(��) */
					m_nPosYBack,						/* ��_(�c) */
					m_nPosX,							/* ��_(��) */
					m_nPosY;							/* ��_(�c) */
	DWORD			m_dwLastProc;						/* �O��̏������� */
	CImg32			*m_pDibCloud;						/* �_�摜 */
} CLayerCloud, *PCLayerCloud;

/* Copyright(C)URARA-works 2005 */
