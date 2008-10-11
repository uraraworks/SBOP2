/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LayerTitle.h												 */
/* ���e			:���C���[�`��N���X(�^�C�g��) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/03/26													 */
/* ========================================================================= */

#pragma once

#include "LayerCloud.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLayerTitle : public CLayerBase
{
public:
			CLayerTitle();							/* �R���X�g���N�^ */
	virtual ~CLayerTitle();						/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);				/* �쐬 */
	void	Draw		(CImg32 *pDst);						/* �`�� */
	BOOL	TimerProc	(void);								/* ���ԏ��� */
	void	StartFadeIn	(void);								/* �t�F�[�h�C���J�n */
	void	EndFadeIn	(void);								/* �t�F�[�h�C���I�� */
	BOOL	IsFadeInEnd	(void);								/* �t�F�[�h�C���I���H */


private:
	int			m_nAnime,						/* �A�j���[�V�����ԍ� */
				m_nFadeLevel,					/* �t�F�[�h���x�� */
				m_nCloudPos;					/* �_�̈ʒu�p */
	DWORD		m_dwLastTimeProc,				/* �ŏI�������� */
				m_dwLastTimeFadeIn;				/* �ŏI��������(�t�F�[�h�C��) */
	CImg32		*m_pDibTitle,					/* �^�C�g���摜 */
				*m_pDibTitleBack,				/* �^�C�g���w�i�摜 */
				*m_pDibTitleCloud;				/* �^�C�g���_�摜 */
} CLayerTitle, *PCLayerTitle;

/* Copyright(C)URARA-works 2007 */
