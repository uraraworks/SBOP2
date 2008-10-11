/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LayerSystemMsg.h											 */
/* ���e			:���C���[�`��N���X(�V�X�e�����b�Z�[�W) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/30													 */
/* ========================================================================= */

#pragma once

#include "LayerCloud.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �\���̐錾																 */
/* ========================================================================= */

typedef struct _SYSTEMMSGINFO {
	int			nPosY;						/* Y���W */
	CImg32		*pImg;						/* �C���[�W */
} SYSTEMMSGINFO, *PSYSTEMMSGINFO;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLayerSystemMsg : public CLayerCloud
{
public:
			CLayerSystemMsg();						/* �R���X�g���N�^ */
	virtual ~CLayerSystemMsg();						/* �f�X�g���N�^ */

	void	Draw		(CImg32 *pDst);						/* �`�� */
	BOOL	TimerProc	(void);								/* ���ԏ��� */
	void	AddMsg		(LPCSTR pszMsg, COLORREF cl);		/* ���b�Z�[�W��ǉ� */


private:
	void	DeleteMsg		(int nNo);						/* ���b�Z�[�W���폜 */
	void	DeleteAllMsg	(void);							/* �S�Ẵ��b�Z�[�W���폜 */


private:
	DWORD		m_dwLastTimeProc;				/* �ŏI�������� */
	CImg32		*m_pDibBack;					/* �w�i�摜 */

	CmyArray<PSYSTEMMSGINFO, PSYSTEMMSGINFO>	m_aSystemMsgInfo;	/* �V�X�e�����b�Z�[�W��� */
} CLayerSystemMsg, *PCLayerSystemMsg;

/* Copyright(C)URARA-works 2007 */
