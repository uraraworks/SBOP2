/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:myThread.h													 */
/* ���e			:�X���b�h�N���X ��`�t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/28													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CmyThread
{
public:
			CmyThread();					/* �R���X�g���N�^ */
	virtual ~CmyThread();					/* �f�X�g���N�^ */

	virtual BOOL Create		(void);					/* �쐬 */
	virtual void Destroy	(void);					/* �j�� */


protected:
	static	UINT __stdcall ThreadEntry(void* pParam);	/* �G���g���|�C���g */
	virtual	DWORD	ThreadInit(void);					/* ������ */
	virtual	void	ThreadMain(void);					/* ���C������ */
	virtual	void	ThreadExit(void);					/* �I������ */


protected:
	BOOL		m_dwInitResult;			/* ���������� */
	HANDLE		m_hThread,				/* �X���b�h�n���h�� */
				m_hInitEvent,			/* �����������C�x���g */
				m_hExitEvent;			/* �I���v���C�x���g */
};

/* Copyright(C)URARA-works 2007 */
