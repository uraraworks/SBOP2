/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoFishing.h												 */
/* ���e			:�ނ���N���X ��`�t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/02													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �ނ���p�����[�^														 */
/* ========================================================================= */
typedef struct _INFOFISHINGPARAM {
	DWORD		dwItemTypeID;			/* �A�C�e�����ID */
	int			nAverage;				/* �ނ肫��m�� */
} INFOFISHINGPARAM, *PINFOFISHINGPARAM;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoFishing : public CInfoBase
{
public:
			CInfoFishing();									/* �R���X�g���N�^ */
	virtual ~CInfoFishing();								/* �f�X�g���N�^ */

	int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	void	Copy				(CInfoFishing *pSrc);				/* �R�s�[ */

	int		GetCount			(void);								/* �ނ���p�����[�^�����擾 */
	void	AddParam			(DWORD dwItemTypeID, int nAverage);	/* �ނ���p�����[�^��ǉ� */
	void	DeleteParam			(int nNo);							/* �ނ���p�����[�^���폜 */
	void	DeleteAllParam		(void);								/* �ނ���p�����[�^��S�č폜 */
	PINFOFISHINGPARAM	GetPtr	(int nNo);							/* �ނ���p�����[�^���擾 */


public:
	/* �ۑ����Ȃ��f�[�^ */

	/* �ۑ�����f�[�^ */
	DWORD		m_dwFishingID;					/* �ނ�ID */
	int			m_nAverage;						/* �ނ��m�� */
	CmyString	m_strName;						/* �ނ�ꖼ */
	CmyArray <PINFOFISHINGPARAM, PINFOFISHINGPARAM>	m_apParam;	/* �ނ���p�����[�^ */
} CInfoFishing, *PCInfoFishing;
typedef CmyArray<PCInfoFishing, PCInfoFishing>	   ARRAYITEMINFO;
typedef CmyArray<PCInfoFishing, PCInfoFishing>	 *PARRAYITEMINFO;

/* Copyright(C)URARA-works 2008 */
