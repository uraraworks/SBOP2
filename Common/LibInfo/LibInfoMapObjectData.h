/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMapObjectData.h										 */
/* ���e			:�}�b�v�I�u�W�F�N�g�z�u�f�[�^���C�u�����N���X ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/03													 */
/* ========================================================================= */

#pragma once

#include "InfoMapObjectData.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoMapObjectData : public CLibInfoBase
{
public:
			CLibInfoMapObjectData();					/* �R���X�g���N�^ */
	virtual ~CLibInfoMapObjectData();					/* �f�X�g���N�^ */

	void Create			(void);									/* �쐬 */
	void Destroy		(void);									/* �j�� */

	PCInfoBase GetNew	(void);									/* �V�K�f�[�^���擾 */

	int		GetCount		(void);								/* �f�[�^�����擾 */
	void	Add				(PCInfoBase pInfo);					/* �ǉ� */
	void	Delete			(int nNo);							/* �폜 */
	void	Delete			(DWORD dwDataID);					/* �폜 */
	void	DeleteAll		(void);								/* �S�č폜 */
	void	Merge			(CLibInfoMapObjectData *pSrc);		/* ��荞�� */

	CInfoMapObjectData	*Renew	(DWORD dwDataID, CInfoMapObjectData *pSrc);	/* �X�V */
	PCInfoBase			GetPtr	(int nNo);									/* �z�u�f�[�^���擾 */
	PCInfoBase			GetPtr	(DWORD dwDataID);							/* �z�u�f�[�^���擾 */

	virtual DWORD	GetDataSize		(void);						/* �f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetWriteData	(PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	virtual DWORD	ReadElementData	(PBYTE pSrc);				/* �w��v�f�f�[�^��ǂݍ��� */

	virtual DWORD	GetSendDataSize	(void);						/* ���M�f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetSendData		(void);						/* ���M�f�[�^���擾 */
	virtual PBYTE	SetSendData		(PBYTE pSrc);				/* ���M�f�[�^�����荞�� */


protected:
	DWORD	GetNewID	(void);									/* �V����ID���擾 */


protected:
	DWORD	m_dwNewIDTmp;						/* �V�KID�쐬�p */
	PARRAYMAPOBJECTDATAINFO	m_paInfo;			/* �z�u�f�[�^ */
} CLibInfoMapObjectData, *PCLibInfoMapObjectData;

/* Copyright(C)URARA-works 2008 */
