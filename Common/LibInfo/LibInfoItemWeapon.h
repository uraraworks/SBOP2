/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoItemWeapon.h										 */
/* ���e			:�A�C�e�������񃉃C�u�����N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/10													 */
/* ========================================================================= */

#pragma once

#include "InfoItemWeapon.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoItemWeapon : public CLibInfoBase
{
public:
			CLibInfoItemWeapon();						/* �R���X�g���N�^ */
	virtual ~CLibInfoItemWeapon();						/* �f�X�g���N�^ */

	void Create			(void);									/* �쐬 */
	void Destroy		(void);									/* �j�� */

	PCInfoBase GetNew	(void);									/* �V�K�f�[�^���擾 */
	int		GetCount	(void);									/* �f�[�^�����擾 */
	void	Add			(PCInfoBase pInfo);						/* �ǉ� */
	void	Delete		(int nNo);								/* �폜 */
	void	Delete		(DWORD dwWeaponInfoID);					/* �폜 */
	void	DeleteAll	(void);									/* �S�č폜 */

	PCInfoBase	GetPtr (int nNo);								/* ��������擾 */
	PCInfoBase	GetPtr (DWORD dwWeaponInfoID);					/* ��������擾 */

	DWORD	GetSendDataSize		(void);							/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);							/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);					/* ���M�f�[�^�����荞�� */
	PBYTE	GetNewSendData		(PBYTE pSrc, PCInfoItemWeapon &pDst);	/* ���M�f�[�^����V�K�f�[�^���擾 */


protected:
	DWORD	GetNewID	(void);									/* �V�����A�C�e��ID���擾 */


protected:
	DWORD	m_dwNewIDTmp;						/* �V�KID�쐬�p */
	PARRAYWEAPONINFO	m_paInfo;				/* ������ */
} CLibInfoItemWeapon, *PCLibInfoItemWeapon;

/* Copyright(C)URARA-works 2008 */
