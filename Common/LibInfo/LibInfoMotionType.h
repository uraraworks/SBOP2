/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMotionType.h										 */
/* ���e			:���[�V������ʏ�񃉃C�u�������N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/26													 */
/* ========================================================================= */

#pragma once

#include "LibInfoBase.h"
#include "InfoMotionType.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoMotionType : public CLibInfoBase
{
public:
			CLibInfoMotionType();									/* �R���X�g���N�^ */
	virtual ~CLibInfoMotionType();									/* �f�X�g���N�^ */

	void Create			(void);												/* �쐬 */
	void Destroy		(void);												/* �j�� */

	virtual PCInfoBase GetNew	(void);										/* �V�K�f�[�^���擾 */
	virtual PCInfoBase GetPtr	(int nNo);									/* �����擾 */
	virtual PCInfoBase GetPtr	(DWORD dwMotionTypeID);						/* �����擾 */

	int		GetCount	(void);												/* �f�[�^�����擾 */
	void	Add			(PCInfoBase pInfo);									/* �ǉ� */
	void	Delete		(int nNo);											/* �폜 */
	void	Delete		(DWORD dwMotionTypeID);								/* �폜 */
	void	DeleteAll	(void);												/* �S�č폜 */

	DWORD	GetSendDataSize		(DWORD dwMotionTypeID);						/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(DWORD dwMotionTypeID);						/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);								/* ���M�f�[�^�����荞�� */


protected:
	DWORD	GetNewID	(void);										/* �V�����A�J�E���gID���擾 */


protected:
	DWORD	m_dwNewIDTmp;						/* �V�KID�쐬�p */
	PARRAYMOTIONTYPEINFO	m_paInfo;			/* ���[�V������ʏ�� */
} CLibInfoMotionType, *PCLibInfoMotionType;

/* Copyright(C)URARA-works 2008 */
