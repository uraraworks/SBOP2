/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoItem.h													 */
/* ���e			:�A�C�e�����N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/05													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoItem : public CInfoBase
{
public:
			CInfoItem();									/* �R���X�g���N�^ */
	virtual ~CInfoItem();									/* �f�X�g���N�^ */

	int		GetElementNo		(LPCSTR pszName);					/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);								/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);							/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);							/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* �w��v�f�f�[�^��ǂݍ��� */

	DWORD	GetSendDataSize		(void);								/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData			(void);								/* ���M�f�[�^���擾 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* ���M�f�[�^�����荞�� */

	void	Copy				(CInfoItem *pSrc);					/* �R�s�[ */


public:
	/* �ۑ����Ȃ��f�[�^ */

	/* �ۑ�����f�[�^ */
	BOOL		m_bPutOn;									/* ��ɒu���邩���� */
	DWORD		m_dwItemID,									/* �A�C�e��ID */
				m_dwItemTypeID,								/* �A�C�e�����ID */
				m_dwGrpID,									/* �n�ʂɂ��鎞�̉摜ID */
				m_dwIconGrpID,								/* �o�b�N�p�b�N���̉摜ID */
				m_dwMapID,									/* �����Ă���}�b�vID */
				m_dwCharID,									/* ���L�҃L����ID */
				m_dwDropSoundID;							/* �������Ƃ��̌��ʉ�ID */
	int			m_nPosZ;									/* �����Ă��鍂��(0���n��) */
	POINT		m_ptPos,									/* �����Ă�����W */
				m_ptBackPack;								/* �o�b�N�p�b�N���̈ʒu */
	CmyString	m_strName;									/* �A�C�e���� */
} CInfoItem, *PCInfoItem;
typedef CmyArray<PCInfoItem, PCInfoItem>	   ARRAYITEMINFO;
typedef CmyArray<PCInfoItem, PCInfoItem>	 *PARRAYITEMINFO;

/* Copyright(C)URARA-works 2007 */
