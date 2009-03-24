/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:InfoAccount.h												 */
/* ���e			:�A�J�E���g���N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/04													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoAccount : public CInfoBase
{
public:
			CInfoAccount();							/* �R���X�g���N�^ */
	virtual ~CInfoAccount();						/* �f�X�g���N�^ */

	int		GetElementNo		(LPCSTR pszName);			/* �v�f�ԍ����擾 */
	DWORD	GetDataSize			(void);						/* �f�[�^�T�C�Y���擾 */
	DWORD	GetDataSizeNo		(int nNo);					/* �w��v�f�̃f�[�^�T�C�Y���擾 */
	LPCSTR	GetName				(int nNo);					/* �v�f�����擾 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);	/* �w��v�f�̕ۑ��p�f�[�^���擾 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);		/* �w��v�f�f�[�^��ǂݍ��� */

	DWORD	GetSendDataSize	(void);							/* ���M�f�[�^�T�C�Y���擾 */
	PBYTE	GetSendData		(void);							/* ���M�f�[�^���擾 */
	PBYTE	SetSendData		(PBYTE pSrc);					/* ���M�f�[�^��ݒ� */

	int		GetCharCount	(void);							/* �L���������擾 */
	void	Copy			(CInfoAccount *pSrc);			/* �R�s�[ */

	PBYTE	GetTmpData		(DWORD &dwDataSize);			/* �f�[�^�L���b�V���p�Ɏ擾 */
	void	SetTmpData		(PBYTE pSrc);					/* �f�[�^�L���b�V������ݒ� */


public:
	/* �ۑ����Ȃ���� */
	DWORD		m_dwLastKeepalive,			/* �Ō�Ɏ�M���������m�F�ʒm�̎��� */
				m_dwIP;						/* IP�A�h���X */
	CmyString	m_strLastMacAddr;			/* ���O�C������MAC�A�h���X */

	/* ����M������ */
	DWORD		m_dwAccountID,				/* �A�J�E���gID */
				m_dwCharID,					/* �g�p���̃L����ID */
				m_dwTimeLastLogin,			/* �O��̃��O�C������ */
				m_dwTimeMakeAccount,		/* �A�J�E���g�쐬���� */
				m_dwLoginCount;				/* ���O�C���� */
	ARRAYDWORD	m_adwCharID;				/* �L����ID�e�[�u�� */

	/* ����M�����ɃT�[�o�[�������ň������ */
	BOOL		m_bDisable;					/* ���O�C������ */
	int			m_nAdminLevel;				/* �Ǘ��҃��x�� */
	DWORD		m_dwSessionID;				/* �g�p�҂̃Z�b�V����ID */
	CmyString	m_strAccount,				/* �A�J�E���g */
				m_strPassword,				/* �p�X���[�h */
				m_strMacAddr;				/* �A�J�E���g�o�^MAC�A�h���X */
} CInfoAccount, *PCInfoAccount;
typedef CmyArray<PCInfoAccount, PCInfoAccount>	  ARRAYINFOACCOUNT;
typedef CmyArray<PCInfoAccount, PCInfoAccount>	*PARRAYINFOACCOUNT;

/* Copyright(C)URARA-works 2006 */
