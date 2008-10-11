/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMotion.h											 */
/* ���e			:���[�V������񃉃C�u�������N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/29													 */
/* ========================================================================= */

#pragma once

#include "LibInfoBase.h"
#include "InfoMotion.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoMotion : public CLibInfoBase
{
public:
			CLibInfoMotion();								/* �R���X�g���N�^ */
	virtual ~CLibInfoMotion();								/* �f�X�g���N�^ */

	void Create			(void);										/* �쐬 */
	void Destroy		(void);										/* �j�� */

	virtual PCInfoBase GetNew	(void);								/* �V�K�f�[�^���擾 */
	virtual PCInfoBase GetPtr	(int nNo);							/* �����擾 */

	void	Revice		(void);											/* �f�[�^�̕␳ */
	int		GetCount	(void);											/* �f�[�^�����擾 */
	void	Add			(PCInfoBase pInfo);								/* �ǉ� */
	void	Add			(PCInfoBase pInfo, int nNo);					/* �ǉ� */
	void	AddType		(DWORD dwMotionTypeID);							/* �w���ʂňꗗ�ǉ� */
	void	Delete		(int nNo);										/* �폜 */
	void	Delete		(DWORD dwMotionID);								/* �폜 */
	void	DeleteAll	(void);											/* �S�č폜 */
	void	SetList		(DWORD dwMotionTypeID, DWORD dwMotionListID, CLibInfoMotion *pSrc);	/* �w��ID�̏���ݒ� */
	BOOL	IsDirection	(DWORD dwMotionListID);							/* �������ɐݒ肪���郂�[�V���������� */
	DWORD	GetWaitTime	(DWORD dwMotionTypeID, DWORD dwMotionListID);	/* �S�R�}�̕\�����Ԃ��擾 */

	void	GetMotionInfo		(DWORD dwMotionTypeID, DWORD dwMotionListID, ARRAYMOTIONINFO &aDst);	/* �w��ID�̃��[�V�������ꗗ���擾 */
	DWORD	GetSendDataSize		(DWORD dwMotionTypeID, DWORD dwMotionListID);							/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(DWORD dwMotionTypeID, DWORD dwMotionListID);							/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);									/* ���M�f�[�^�����荞�� */

	DWORD	GetSendDataSizeType	(DWORD dwMotionTypeID);							/* �w����ID�̑��M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendDataType		(DWORD dwMotionTypeID);							/* �w����ID�̑��M�f�[�^���擾 */
	PBYTE	SetSendDataType		(PBYTE pSrc);									/* ���M�f�[�^�����荞�� */


protected:
	DWORD	GetNewID	(void);										/* �V�����A�J�E���gID���擾 */


protected:
	DWORD	m_dwNewIDTmp;						/* �V�KID�쐬�p */
	PARRAYMOTIONINFO	m_paInfo;				/* ���[�V������� */
} CLibInfoMotion, *PCLibInfoMotion;

/* Copyright(C)URARA-works 2007 */
