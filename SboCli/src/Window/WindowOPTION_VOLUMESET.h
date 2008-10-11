/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowOPTION_VOLUMESET.h									 */
/* ���e			:�I�v�V����-���ʐݒ�E�B���h�E�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/28													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowOPTION_VOLUMESET : public CWindowBase
{
public:
			CWindowOPTION_VOLUMESET();						/* �R���X�g���N�^ */
	virtual ~CWindowOPTION_VOLUMESET();						/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */


private:
	BOOL	OnUp		(void);								/* �L�[�n���h��(��) */
	BOOL	OnDown		(void);								/* �L�[�n���h��(��) */
	BOOL	OnLeft		(void);								/* �L�[�n���h��(��) */
	BOOL	OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL	OnZ			(BOOL bDown);						/* �L�[�n���h��(Z) */


protected:
	int		m_nVolumeBgm,			/* BGM���� */
			m_nVolumeSound,			/* ���ʉ����� */
			m_nBackVolumeBgm,		/* �ύX�O��BGM���� */
			m_nBackVolumeSound;		/* �ύX�O�̌��ʉ����� */
} CWindowOPTION_VOLUMESET, *PCWindowOPTION_VOLUMESET;

/* Copyright(C)URARA-works 2008 */
