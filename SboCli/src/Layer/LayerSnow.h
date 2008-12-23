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
class CLayerMap;

/* ========================================================================= */
/* �\���̐錾																 */
/* ========================================================================= */
/* ���� */
typedef struct _STLAYERSNOW_SNOWINFO {
	int		nState,			/* ��� */
			nLevel,			/* �����x */
			nSize,			/* �T�C�Y */
			nStartY,		/* �J�n�n�_ */
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


public:
	CLayerMap		*m_pLayerMap;						/* �}�b�v���C���[ */


protected:
	DWORD			m_dwLastProc;		/* �O��̏������� */
	ARRAYSNOWINFO	m_aSnowInfo;		/* ���� */
	CImg32			*m_pImgTmp;			/* �摜�e���|���� */
} CLayerSnow, *PCLayerSnow;

/* Copyright(C)URARA-works 2008 */
