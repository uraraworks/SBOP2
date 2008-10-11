/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoEfcBalloon.h											 */
/* ���e			:���o�����N���X ��`�t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/24													 */
/* ========================================================================= */

#pragma once

#include "InfoAnime.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoEfcBalloon : public CInfoBase
{
public:
			CInfoEfcBalloon();								/* �R���X�g���N�^ */
	virtual ~CInfoEfcBalloon();								/* �f�X�g���N�^ */

	int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	void	Copy				(CInfoEfcBalloon *pSrc);			/* �R�s�[ */
	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */


public:
	/* �ۑ����Ȃ��f�[�^ */

	/* �ۑ�����f�[�^ */
	BYTE		m_byWait;							/* �҂�����(�~�P�O�~���b) */
	BOOL		m_bLoop;							/* ���[�v�Đ� */
	DWORD		m_dwEfcBalloonID,					/* ���o��ID */
				m_dwListID,							/* ���o�����ID */
				m_dwAnimeID,						/* �R�}�ԍ� */
				m_dwSoundID,						/* ���ʉ�ID */
				m_dwGrpID;							/* �摜ID */
	CmyString	m_strName;							/* ���o���� */
} CInfoEfcBalloon, *PCInfoEfcBalloon;
typedef CmyArray<PCInfoEfcBalloon, PCInfoEfcBalloon>	  ARRAYEFCBALLOON;
typedef CmyArray<PCInfoEfcBalloon, PCInfoEfcBalloon>	*PARRAYEFCBALLOON;

/* Copyright(C)URARA-works 2007 */
