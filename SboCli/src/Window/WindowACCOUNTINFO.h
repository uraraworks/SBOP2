/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:WindowACCOUNTINFO.h										 */
/* ���e			:�A�J�E���g���E�B���h�E�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/12/29													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowACCOUNTINFO : public CWindowBase
{
public:
			CWindowACCOUNTINFO();							/* �R���X�g���N�^ */
	virtual ~CWindowACCOUNTINFO();							/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */


private:
} CWindowACCOUNTINFO, *PCWindowACCOUNTINFO;

/* Copyright(C)URARA-works 2006 */
