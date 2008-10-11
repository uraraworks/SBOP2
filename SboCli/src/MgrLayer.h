/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrLayer.h													 */
/* ���e			:���C���[�}�l�[�W���N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#pragma once

#include "LayerBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CMgrLayer
{
public:
			CMgrLayer();						/* �R���X�g���N�^ */
	virtual ~CMgrLayer();						/* �f�X�g���N�^ */

	void Create		(CMgrData *pMgrData);				/* �쐬 */
	void Destroy	(void);								/* �j�� */

	void Draw			(CImg32 *pDst);					/* �`�� */
	BOOL TimerProc		(void);							/* ���ԏ��� */
	void Delete			(int nID);						/* �w�背�C���[��j�� */
	void DeleteAll		(void);							/* �S�Ẵ��C���[��j�� */
	CLayerBase	*Get	(int nID);						/* �w��ID�̃��C���[���擾 */

	/* ���C���[�쐬 */
	void MakeLOGO		(void);							/* ���S */
	void MakeTITLE		(void);							/* �^�C�g�� */
	void MakeINFO		(void);							/* ���m�点 */
	void MakeLOGINMENU	(void);							/* ���O�C�����j���[ */
	void MakeCHARSELECT	(DWORD dwAccountID);			/* �L�����I�� */
	void MakeMAP		(void);							/* �}�b�v */
	void MakeCLOUD		(void);							/* �_ */
	void MakeSYSTEMMSG	(void);							/* �V�X�e�����b�Z�[�W */


private:
	CMgrData			*m_pMgrData;			/* �f�[�^�Ǘ� */
	CMgrGrpData			*m_pMgrGrpData;			/* �O���t�B�b�N�f�[�^�Ǘ� */

	PARRAYLAYERBASE		m_paLayer;				/* ���C���[�`�� */
} CMgrLayer, *PCMgrLayer;

/* Copyright(C)URARA-works 2006 */
