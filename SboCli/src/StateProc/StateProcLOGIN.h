/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:StateProcLOGIN.h											 */
/* ���e			:��ԏ����N���X(���O�C�����) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/03													 */
/* ========================================================================= */

#pragma once

#include "StateProcBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CStateProcLOGIN : public CStateProcBase
{
public:
			CStateProcLOGIN();							/* �R���X�g���N�^ */
	virtual ~CStateProcLOGIN();							/* �f�X�g���N�^ */

	void Init			(void);									/* ������ */
	BOOL TimerProc		(void);									/* ���ԏ��� */
	void OnLButtonDown	(int x, int y);							/* ���b�Z�[�W�n���h��(WM_LBUTTONDOWN) */


protected:
	BOOL OnX					(BOOL bDown);					/* �L�[�n���h��(X) */
	void OnMgrDrawSTART_FADEIN	(DWORD dwPara);					/* �t�F�[�h�C���J�n */


protected:
	BOOL	m_bFadeIn;					/* �t�F�[�h�C�������H */
} CStateProcLOGIN, *PCStateProcLOGIN;

/* Copyright(C)URARA-works 2006 */
