/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoEfcBalloon.h										 */
/* ���e			:���o����񃉃C�u�������N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/24													 */
/* ========================================================================= */

#pragma once

#include "LibInfoBase.h"
#include "InfoEfcBalloon.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibInfoEfcBalloon : public CLibInfoBase
{
public:
			CLibInfoEfcBalloon();							/* �R���X�g���N�^ */
	virtual ~CLibInfoEfcBalloon();							/* �f�X�g���N�^ */

	void Create			(void);										/* �쐬 */
	void Destroy		(void);										/* �j�� */

	virtual PCInfoBase GetNew	(void);								/* �V�K�f�[�^���擾 */
	virtual PCInfoBase GetPtr	(int nNo);							/* �����擾 */

	int		GetCount	(void);										/* �f�[�^�����擾 */
	void	Add			(PCInfoBase pInfo);							/* �ǉ� */
	void	Delete		(int nNo);									/* �폜 */
	void	Delete		(DWORD dwEfcBalloonID);						/* �폜 */
	void	DeleteAll	(void);										/* �S�č폜 */
	void	SetList		(DWORD dwListID, CLibInfoEfcBalloon *pSrc);	/* �w����ID�̏���ݒ� */
	void	GetListID	(ARRAYDWORD &aDst);							/* ���ID�ꗗ���擾 */
	void	GetName		(DWORD dwListID, CmyString &strDst);		/* ���o�������擾 */
	void	Sort		(void);										/* ���ID�Ń\�[�g */

	DWORD			GetNextAnimeID	(DWORD dwListID, DWORD dwAnimeID);		/* ���̃R�}�ԍ����擾 */
	CInfoEfcBalloon	*GetPtr			(DWORD dwListID, DWORD dwAnimeID);		/* ���o�������擾 */

	void	GetEfcBalloonInfo	(DWORD dwListID, ARRAYEFCBALLOON &aDst);	/* �w��ID�̕��o�����ꗗ���擾 */
	DWORD	GetSendDataSize		(DWORD dwListID);							/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(DWORD dwListID);							/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);								/* ���M�f�[�^�����荞�� */


protected:
	DWORD	GetNewID	(void);										/* �V����ID���擾 */


protected:
	DWORD	m_dwNewIDTmp;						/* �V�KID�쐬�p */
	PARRAYEFCBALLOON	m_paInfo;				/* ���o����� */
} CLibInfoEfcBalloon, *PCLibInfoEfcBalloon;

/* Copyright(C)URARA-works 2007 */
