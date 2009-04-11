/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoDisable.h											 */
/* ���e			:���ۏ�񃉃C�u�������N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/04/08													 */
/* ========================================================================= */

#pragma once

#include "InfoDisable.h"
#include "LibInfoBase.h"

/* IP�A�h���X���ۏ�� */
typedef struct _STDISABLEIPADDRESS {
	ULONG	ulIPAddress;		/* IP�A�h���X */
	DWORD	dwLastTime;			/* �ŏI���O�C������ */
} STDISABLEIPADDRESS, *PSTDISABLEIPADDRESS;
typedef CmyArray<PSTDISABLEIPADDRESS, PSTDISABLEIPADDRESS>   ARRAYDISABLEIPADDRESS;
typedef CmyArray<PSTDISABLEIPADDRESS, PSTDISABLEIPADDRESS> *PARRAYDISABLEIPADDRESS;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoDisable : public CLibInfoBase
{
public:
			CLibInfoDisable();								/* �R���X�g���N�^ */
	virtual ~CLibInfoDisable();								/* �f�X�g���N�^ */

	void Create			(void);										/* �쐬 */
	void Destroy		(void);										/* �j�� */
	BOOL Proc			(void);										/* ���� */

	PCInfoBase GetNew	(void);										/* �V�K�f�[�^���擾 */

	int		GetCount	(void);										/* �f�[�^�����擾 */
	void	Add			(PCInfoBase pInfo);							/* �ǉ� */
	void	Delete		(int nNo);									/* �폜 */
	void	Delete		(DWORD dwDisableID);						/* �폜 */
	void	DeleteAll	(void);										/* �S�č폜 */
	void	CopyAll		(CLibInfoDisable *pSrc);					/* �S�ăR�s�[ */
	void	Merge		(CLibInfoDisable *pSrc);					/* ��荞�� */
	BOOL	IsDisable	(LPCSTR pszMacAddress);						/* ���ۏ�񂪓o�^����Ă��邩���� */

	PCInfoBase	GetPtr (int nNo);									/* ���ۏ����擾 */
	PCInfoBase	GetPtr (DWORD dwDisableID);							/* ���ۏ����擾 */
	PCInfoBase	GetPtr (LPCSTR pszMacAddress);						/* ���ۏ����擾 */

	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	/* IP�A�h���X�֘A */
	void	AddIP		(ULONG ulIPAddress);						/* �ǉ� */
	void	DeleteIP	(int nNo);									/* �폜 */
	void	DeleteAllIP	(void);										/* �S�č폜 */
	BOOL	IsDisableIP	(ULONG ulIPAddress);						/* �֎~����Ă���IP�A�h���X������ */


protected:
	DWORD	GetNewID	(void);										/* �V��������ID���擾 */


protected:
	DWORD	m_dwNewIDTmp,						/* �V�KID�쐬�p */
			m_dwLastTimeProc;					/* �Ō�̏������� */

	PARRAYDISABLEINFO		m_paInfo;			/* ���ۏ�� */
	PARRAYDISABLEIPADDRESS	m_paInfoIPADdress;	/* ����IP�A�h���X��� */
} CLibInfoDisable, *PCLibInfoDisable;

/* Copyright(C)URARA-works 2009 */
