/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:WindowSTATUS.h												 */
/* ���e			:�����E�B���h�E�N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/06/26													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowSTATUS : public CWindowBase
{
public:
			CWindowSTATUS();								/* �R���X�g���N�^ */
	virtual ~CWindowSTATUS();								/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */
	void	SetID		(DWORD dwID);								/* �L����ID��ݒ� */


private:
	DWORD	m_dwCharID;						/* �L����ID */
} CWindowSTATUS, *PCWindowSTATUS;

/* Copyright(C)URARA-works 2005 */
