/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:UpdateServerInfo.h											 */
/* ���e			:�T�[�o�[���X�V�N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/22													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CUpdateServerInfo
{
public:
			CUpdateServerInfo();					/* �R���X�g���N�^ */
	virtual ~CUpdateServerInfo();					/* �f�X�g���N�^ */

	BOOL	Create		(LPCSTR pszAccount, LPCSTR pszPassword, LPCSTR pszServerAddr, LPCSTR pszUploadPath);	/* �쐬 */
	void	Destroy		(void);								/* �j�� */
	void	Update		(int nCount, LPCSTR pszPlace, BOOL bUpload = FALSE);		/* �X�V */


protected:
	static UINT __stdcall ThreadEntry(LPVOID lpParam);	/* �X���b�h���C������ */
	void	ThreadMain		(void);							/* �X���b�h���C������ */
	void	MakeServerInfo	(void);							/* �T�[�o�[���쐬 */
	void	Upload			(void);							/* �A�b�v���[�h */
	void	GetFileName		(CmyString &strDst);			/* �T�[�o�[���t�@�C�������擾 */


protected:
	HANDLE		m_hThread,			/* �X���b�h�n���h�� */
				m_hInitEvent,		/* �J�n�҂��C�x���g */
				m_hExitEvent,		/* �I���҂��C�x���g */
				m_hUpdate;			/* �X�V�C�x���g */
	CmyString	m_strAccount,		/* FTP�A�J�E���g */
				m_strPassword,		/* FTP�p�X���[�h */
				m_strServerAddr,	/* �T�[�o�[�A�h���X */
				m_strUploadPath;	/* �A�b�v���[�h�� */
	CmySection	m_csUpdate;			/* ���X�V�p�N���e�B�J���Z�N�V���� */

	int			m_nCount;			/* ���O�C���l�� */
	CmyString	m_strPlace;			/* �ł��W�܂��Ă���ꏊ */
};

/* Copyright(C)URARA-works 2008 */
