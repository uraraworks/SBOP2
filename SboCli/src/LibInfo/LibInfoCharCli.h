/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoCharCli.h											 */
/* ���e			:�L������񃉃C�u�����N���C�A���g�N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/01													 */
/* ========================================================================= */

#pragma once

#include "LibInfoCharBase.h"

class CMainFrame;
class CMgrData;
class CInfoCharCli;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoCharCli : public CLibInfoCharBase
{
public:
			CLibInfoCharCli();													/* �R���X�g���N�^ */
	virtual ~CLibInfoCharCli();													/* �f�X�g���N�^ */

	void Create				(CMgrData *pMgrData);										/* �쐬 */
	BOOL Proc				(void);														/* ���� */
	BOOL DeleteOutScreen	(CInfoCharCli *pChar, BOOL bDelete = FALSE);				/* ��ʊO�̃L�������폜 */
	void RenewMotionInfo	(DWORD dwCharID);											/* ���[�V���������X�V */
	void RenewMotionInfo	(CInfoCharCli *pChar);										/* ���[�V���������X�V */
	void SetMotionInfo		(CInfoCharCli *pChar, DWORD dwMotionID, DWORD dwListID);	/* ���[�V��������ݒ� */
	void RenewGrpID			(DWORD dwCharID);											/* �����摜ID���X�V */
	BOOL IsMove				(PCInfoCharBase pInfoChar, int &nDirection);				/* �w������ɐi�߂邩�`�F�b�N */

	CInfoBase	*GetNew		(int nType);												/* �V�K�f�[�^���擾 */
	CInfoBase	*GetPtr		(int nNo);													/* �L���������擾 */
	CInfoBase	*GetPtr		(DWORD dwCharID);											/* �L���������擾 */


public:
	int		m_nProcNo;						/* �������̃L�����C���f�b�N�X */

	CMainFrame	*m_pMainFrame;				/* ���C���t���[�� */
	CMgrData	*m_pMgrData;				/* �f�[�^�}�l�[�W�� */
} CLibInfoCharCli, *PCLibInfoCharCli;

/* Copyright(C)URARA-works 2006 */
