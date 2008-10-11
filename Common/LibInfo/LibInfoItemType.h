/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoItemType.h											 */
/* ���e			:�A�C�e����ʏ�񃉃C�u�������N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/27													 */
/* ========================================================================= */

#pragma once

#include "InfoItemTypeBase.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoItemType : public CLibInfoBase
{
public:
			CLibInfoItemType();							/* �R���X�g���N�^ */
	virtual ~CLibInfoItemType();						/* �f�X�g���N�^ */

	void Create			(void);									/* �쐬 */
	void Destroy		(void);									/* �j�� */

			PCInfoBase GetNew	(void);							/* �V�K�f�[�^���擾 */
	virtual	PCInfoBase GetNew	(DWORD dwItemTypeID);			/* �V�K�f�[�^���擾 */

	void	Revice		(void);									/* �f�[�^�̕␳ */
	int		GetCount	(void);									/* �f�[�^�����擾 */
	void	Add			(PCInfoBase pInfo);						/* �ǉ� */
	void	Delete		(int nNo);								/* �폜 */
	void	Delete		(DWORD dwTypeID);						/* �폜 */
	void	DeleteAll	(void);									/* �S�č폜 */

	int		GetGrpIDMainCount	(DWORD dwTypeID);						/* �A�C�e����ʂ̉摜ID���C�������擾 */
	LPCSTR	GetTypeName			(DWORD dwItemTypeID);					/* �A�C�e����ʖ����擾 */
	LPCSTR	GetTypeNameTypeID	(DWORD dwTypeID);						/* �A�C�e����ʖ����擾 */
	LPCSTR	GetGrpIDMainName	(DWORD dwTypeID, WORD wGrpIDMain);		/* �摜ID���C����ʖ����擾 */
	DWORD	GetGrpIDMain		(DWORD dwTypeID, WORD wGrpIDMain);		/* �摜ID���C�����擾 */

	PCInfoBase	GetPtr (int nNo);										/* �A�C�e�������擾 */
	PCInfoBase	GetPtr (DWORD dwTypeID);								/* �A�C�e�������擾 */

	DWORD	GetSendDataSize		(void);									/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);									/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);							/* ���M�f�[�^�����荞�� */
	PBYTE	GetNewSendData		(PBYTE pSrc, PCInfoItemTypeBase &pDst);	/* ���M�f�[�^����V�K�f�[�^���擾 */


protected:
	DWORD	GetNewID	(void);									/* �V�����A�C�e��ID���擾 */


protected:
	DWORD	m_dwNewIDTmp;						/* �V�KID�쐬�p */
	PARRAYITEMTYPEINFO	m_paInfo;				/* �A�C�e����ʏ�� */
} CLibInfoItemType, *PCLibInfoItemType;

/* Copyright(C)URARA-works 2007 */
