/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LayerSnow.h												 */
/* ���e			:���C���[�`��N���X(��) ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/14													 */
/* ========================================================================= */

#pragma once

#include "LayerBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �\���̐錾																 */
/* ========================================================================= */
/* ���� */
typedef struct _STLAYERSNOW_SNOWINFO {
	int		nSize,			/* �T�C�Y */
			nEndY,			/* ���n�_ */
			x,				/* �����W */
			y;				/* �c���W */
	DWORD	dwStartWait,	/* �J�n�܂ł̑҂����� */
			dwWait,			/* ���x */
			dwLastProc;		/* �O��̏������� */
} STLAYERSNOW_SNOWINFO, *PSTLAYERSNOW_SNOWINFO;
typedef CmyArray<PSTLAYERSNOW_SNOWINFO, PSTLAYERSNOW_SNOWINFO> ARRAYSNOWINFO;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLayerSnow : public CLayerBase
{
public:
			CLayerSnow();						/* �R���X�g���N�^ */
	virtual ~CLayerSnow();						/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);			/* �쐬 */
	void	Draw		(CImg32 *pDst);					/* �`�� */
	BOOL	TimerProc	(void);							/* ���ԏ��� */


protected:
	void	RenewSnowInfo		(int nCount);			/* ������X�V */
	void	DeleteSnowInfoAll	(void);					/* �����S�č폜 */


protected:
	DWORD			m_dwLastProc;		/* �O��̏������� */
	ARRAYSNOWINFO	m_aSnowInfo;		/* ���� */
} CLayerSnow, *PCLayerSnow;

/* Copyright(C)URARA-works 2008 */
