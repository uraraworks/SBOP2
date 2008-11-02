/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapObject.h											 */
/* ���e			:�}�b�v�I�u�W�F�N�g�N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/01													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* ========================================================================= */
/* �\���̒�`																 */
/* ========================================================================= */

/* �}�b�v�I�u�W�F�N�g�A�j����� */
typedef struct _STMAPOBJECTANIMEINFO {
	BYTE	byWait,				/* �҂�����(�~�P�O�~���b) */
			byLevel;			/* �����x */
	PWORD	pwGrpID;			/* �摜ID */
} STMAPOBJECTANIMEINFO, *PSTMAPOBJECTANIMEINFO;
typedef CmyArray<PSTMAPOBJECTANIMEINFO, PSTMAPOBJECTANIMEINFO>	   ARRAYMAPOBJECTANIMEINFO;
typedef CmyArray<PSTMAPOBJECTANIMEINFO, PSTMAPOBJECTANIMEINFO>	 *PARRAYMAPOBJECTANIMEINFO;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoMapObject : public CInfoBase
{
public:
			CInfoMapObject();								/* �R���X�g���N�^ */
	virtual ~CInfoMapObject();								/* �f�X�g���N�^ */

	int		GetElementCount		(void);								/* �v�f�����擾 */
	int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	void	Copy				(CInfoMapObject *pSrc);				/* �R�s�[ */
	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */
	void	GetAnimeSize		(int nNo = -1);						/* �I�u�W�F�N�g�A�j�����T�C�Y���擾 */

	int		GetAnimeCount		(void);								/* �A�j���[�V�����R�}�����擾 */
	void	AddAnime			(void);								/* �A�j���[�V�����R�}��ǉ� */
	void	DeleteAnime			(int nNo);							/* �A�j���[�V�����R�}���폜 */
	void	DeleteAllAnime		(void);								/* �A�j���[�V�����R�}��S�č폜 */
	void	RenewGrpSize		(int cx = -1, int cy = -1);			/* �摜�T�C�Y���X�V */
	void	SetGrpID			(int nNo, int x, int y, WORD wGrpID);	/* �摜ID��ݒ� */

	PSTMAPOBJECTANIMEINFO	GetAnimePtr	(int nNo);					/* �A�j���[�V�����R�}���擾 */


public:
	/* �ۑ�����f�[�^ */
	DWORD				m_dwObjectID,						/* �I�u�W�F�N�gID */
						m_dwAttr;							/* �I�u�W�F�N�g�̑��� */
	SIZE				m_sizeGrp;							/* �摜�T�C�Y */
	CmyString			m_strName;							/* �I�u�W�F�N�g�� */

	ARRAYMAPOBJECTANIMEINFO	m_aInfoAnime;					/* �I�u�W�F�N�g�A�j����� */
} CInfoMapObject, *PCInfoMapObject;
typedef CmyArray<PCInfoMapObject, PCInfoMapObject>	  ARRAYMAPOBJECT;
typedef CmyArray<PCInfoMapObject, PCInfoMapObject>	*PARRAYMAPOBJECT;

/* Copyright(C)URARA-works 2008 */
