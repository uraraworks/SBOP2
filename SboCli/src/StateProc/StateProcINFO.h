/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:StateProcINFO.h											 */
/* ���e			:��ԏ����N���X(���m�点) ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/08/16													 */
/* ========================================================================= */

#pragma once

#include "StateProcBase.h"


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CStateProcINFO : public CStateProcBase
{
public:
			CStateProcINFO();					/* �R���X�g���N�^ */
	virtual ~CStateProcINFO();					/* �f�X�g���N�^ */

	void Init			(void);							/* ������ */
	void OnLButtonDown	(int x, int y);					/* ���b�Z�[�W�n���h��(WM_LBUTTONDOWN) */


protected:
	BOOL	OnX			(BOOL bDown);					/* �L�[�n���h��(X) */
} CStateProcINFO, *PCStateProcINFO;

/* Copyright(C)URARA-works 2005 */
