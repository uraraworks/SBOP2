/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:StateProcDISCONNECT.h										 */
/* ���e			:��ԏ����N���X(�ؒf) ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/08													 */
/* ========================================================================= */

#pragma once

#include "StateProcBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CStateProcDISCONNECT : public CStateProcBase
{
public:
			CStateProcDISCONNECT();						/* �R���X�g���N�^ */
	virtual ~CStateProcDISCONNECT();					/* �f�X�g���N�^ */

	void Init	(void);											/* ������ */


protected:
} CStateProcDISCONNECT, *PCStateProcDISCONNECT;

/* Copyright(C)URARA-works 2006 */
