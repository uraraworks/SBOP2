/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoItemWeapon.h											 */
/* ���e			:�A�C�e��������N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/10													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoItemWeapon : public CInfoBase
{
public:
			CInfoItemWeapon();									/* �R���X�g���N�^ */
	virtual ~CInfoItemWeapon();									/* �f�X�g���N�^ */

	virtual int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	virtual DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	virtual LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	virtual DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	virtual void	Copy				(CInfoItemWeapon *pSrc);			/* �R�s�[ */


public:
	/* �ۑ����Ȃ��f�[�^ */

	/* �ۑ�����f�[�^ */
	DWORD		m_dwWeaponInfoID,				/* ������ID */
				m_dwMotionType,					/* �g�p�\�ȍU�����[�V���� */
				m_dwMotionTypeStand,			/* �퓬���[�h���̗������[�V���� */
				m_dwMotionTypeWalk;				/* �퓬���[�h���̂��葫���[�V���� */
	CmyString	m_strName;						/* �����ʖ� */
	ARRAYDWORD	m_adwEffectIDAtack,				/* �ʏ�U�����̃G�t�F�N�gID */
				m_adwEffectIDCritical;			/* �N���e�B�J�����̃G�t�F�N�gID */

	/* ������ */
} CInfoItemWeapon, *PCInfoItemWeapon;
typedef CmyArray<PCInfoItemWeapon, PCInfoItemWeapon>	   ARRAYWEAPONINFO;
typedef CmyArray<PCInfoItemWeapon, PCInfoItemWeapon>	 *PARRAYWEAPONINFO;

/* Copyright(C)URARA-works 2008 */
