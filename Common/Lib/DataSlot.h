/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DataSlot.h													 */
/* ���e			:�f�[�^�X���b�g�N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/07													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �\���̂̒�`																 */
/* ========================================================================= */

/* �f�[�^�X���b�g��� */
typedef struct _DATASLOTINFO {
	DWORD		dwID;					/* �f�[�^ID */
	DWORD		dwSize;					/* �f�[�^�T�C�Y */
	PBYTE		pData;					/* �f�[�^�o�b�t�@ */
} DATASLOTINFO, *PDATASLOTINFO;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CDataSlot
{
public:
			CDataSlot();					/* �R���X�g���N�^ */
	virtual ~CDataSlot();					/* �f�X�g���N�^ */

	virtual void Destroy(void);						/* �j�� */

	/* ���[�U�f�[�^�̃A�N�Z�X */
	DWORD	Add			(DWORD dwSize, PBYTE pSrc);					/* �f�[�^�ǉ� */
	DWORD	AddDelete	(DWORD dwSize, PBYTE *pSrc);				/* �f�[�^�ǉ����ăo�b�t�@���폜 */
	DWORD	GetSize		(DWORD dwID);								/* �f�[�^�T�C�Y�擾 */
	BOOL	Copy		(DWORD dwID, LPBYTE pDst, BOOL bDelete);	/* �f�[�^�擾 */
	PBYTE	GetPtr		(DWORD dwID);								/* �f�[�^�A�h���X�擾 */
	void	Delete		(DWORD dwID);								/* �f�[�^�폜 */
	void	Lock		(void);										/* �f�[�^�̃��b�N */
	void	UnLock		(void);										/* �f�[�^�̃��b�N���� */


protected:
	int GetID(DWORD dwID);							/* �e�[�u��ID�擾 */


protected:
	CmySection		m_CritData,						/* �f�[�^�A�N�Z�X���̃N���e�B�J���Z�N�V���� */
					m_CritDataLock;					/* �f�[�^���b�N�p�̃N���e�B�J���Z�N�V���� */
	DWORD			m_dwNewID;						/* �f�[�^ID�e���|���� */

	CmyArray<DATASLOTINFO, DATASLOTINFO> m_DataSlot;	/* �f�[�^�X���b�g */
} CDataSlot, *PCDataSlot;

/* Copyright(C)URARA-works 2008 */
