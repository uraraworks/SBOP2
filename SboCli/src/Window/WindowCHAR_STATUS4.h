/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:WindowCHAR_STATUS4.h										 */
/* ���e			:�L�������E�B���h�E�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/02/15													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowCHAR_STATUS4 : public CWindowBase
{
public:
			CWindowCHAR_STATUS4();							/* �R���X�g���N�^ */
	virtual ~CWindowCHAR_STATUS4();							/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */


protected:
} CWindowCHAR_STATUS4, *PCWindowCHAR_STATUS4;

/* Copyright(C)URARA-works 2009 */
