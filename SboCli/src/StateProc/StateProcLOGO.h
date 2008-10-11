/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	StateProcLOGO.h												 */
/* ���e�F		��ԏ����N���X(���S) ��`�t�@�C��							 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/05/28													 */
/* ========================================================================= */

#pragma once

#include "StateProcBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CStateProcLOGO : public CStateProcBase
{
public:
			CStateProcLOGO();					/* �R���X�g���N�^ */
	virtual ~CStateProcLOGO();					/* �f�X�g���N�^ */

	void Init			(void);							/* ������ */
	BOOL TimerProc		(void);							/* ���ԏ��� */
	void OnLButtonDown	(int x, int y);					/* ���b�Z�[�W�n���h��(WM_LBUTTONDOWN) */


protected:
	BOOL OnX					(BOOL bDown);			/* �L�[�n���h��(X) */
	BOOL OnZ					(BOOL bDown);			/* �L�[�n���h��(Z) */
	void OnMgrDrawEND_FADEIN	(DWORD dwPara);			/* �t�F�[�h�C������ */
	void OnMgrDrawEND_FADEOUT	(DWORD dwPara);			/* �t�F�[�h�A�E�g���� */


protected:
	DWORD	m_dwTimeWait;					/* �҂����ԗp */
} CStateProcLOGO, *PCStateProcLOGO;

/* Copyright(C)URARA-works 2005 */
