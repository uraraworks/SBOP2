/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharBATTLE2Svr.h										 */
/* ���e			:�L�������(�퓬2)�T�[�o�[�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/07/17													 */
/* ========================================================================= */

#pragma once

#include "InfoCharBATTLE1Svr.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoCharBATTLE2Svr : public CInfoCharBATTLE1Svr
{
public:
			CInfoCharBATTLE2Svr();						/* �R���X�g���N�^ */
	virtual ~CInfoCharBATTLE2Svr();						/* �f�X�g���N�^ */


protected:
	virtual BOOL TimerProcSTAND	(DWORD dwTime);					/* ���ԏ���(����) */


public:
} CInfoCharBATTLE2Svr, *PCInfoCharBATTLE2Svr;

/* Copyright(C)URARA-works 2009 */
