/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:WindowPLACEINFORMATION.h									 */
/* ���e			:�ꏊ���E�B���h�E�N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/02/11													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowPLACEINFORMATION : public CWindowBase
{
public:
			CWindowPLACEINFORMATION();						/* �R���X�g���N�^ */
	virtual ~CWindowPLACEINFORMATION();						/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */


protected:
} CWindowPLACEINFORMATION, *PCWindowPLACEINFORMATION;

/* Copyright(C)URARA-works 2009 */
