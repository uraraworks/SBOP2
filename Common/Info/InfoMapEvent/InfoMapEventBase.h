/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapEventBase.h											 */
/* ���e			:�C�x���g�����N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/05													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �C�x���g��� */
enum {
	MAPEVENTTYPE_NONE = 0,
	MAPEVENTTYPE_MOVE,									/* �}�b�v���ړ� */
	MAPEVENTTYPE_MAPMOVE,								/* �}�b�v�Ԉړ� */
	MAPEVENTTYPE_TRASHBOX,								/* �S�~�� */
	MAPEVENTTYPE_INITSTATUS,							/* �X�e�[�^�X������ */
	MAPEVENTTYPE_GRPIDTMP,								/* �ꎞ�摜�ݒ� */
	MAPEVENTTYPE_MAX
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoMapEventBase : public CInfoBase
{
public:
			CInfoMapEventBase();									/* �R���X�g���N�^ */
	virtual ~CInfoMapEventBase();									/* �f�X�g���N�^ */

	virtual void	RenewSize			(int nDirection, int nSize);		/* �T�C�Y�X�V */
	virtual int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	virtual DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	virtual LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	virtual DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	virtual PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	virtual void	Copy				(CInfoMapEventBase *pSrc);			/* �R�s�[ */


public:
	int			m_nElementCountBase;			/* ���N���X�̗v�f�� */

	DWORD		m_dwMapEventID;					/* �}�b�v�C�x���gID */
	int			m_nType;						/* �C�x���g��� */
	POINT		m_ptPos;						/* ���W */
} CInfoMapEventBase, *PCInfoMapEventBase;
typedef CmyArray<PCInfoMapEventBase, PCInfoMapEventBase>	  ARRAYMAPEVENTBASEINFO;
typedef CmyArray<PCInfoMapEventBase, PCInfoMapEventBase>	*PARRAYMAPEVENTBASEINFO;

/* Copyright(C)URARA-works 2007 */
