/* Copyright(C)URARA-works 2003-2005 */
/* ========================================================================= */
/* UraraSockTCPSBO.h														 */
/* �ėpTCP�ʐM�N���X														 */
/* 2003/10/28 �쐬�J�n														 */
/* ========================================================================= */

#pragma once

#include "UraraSockTCP.h"

class CPacketBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CUraraSockTCPSBO
{
public:
			CUraraSockTCPSBO(void);									/* �R���X�g���N�^ */
	virtual ~CUraraSockTCPSBO(void);								/* �f�X�g���N�^ */

	void	DeleteRecvData		(PBYTE pData);										/*��M�f�[�^���폜 */
	void	Destroy				(void);												/* ��n�� */
	BOOL	Host				(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount);		/* �ڑ��҂��J�n */
	BOOL	Connect				(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr);	/* �T�[�o�[�֐ڑ� */
	void	DeleteClient		(DWORD dwID);										/* �N���C�A���g��ؒf */
	void	SendCancel			(DWORD dwID = 0);									/* ���M�L�����Z�� */
	void	Send				(CPacketBase *pPacket);								/* �T�[�o�[�փf�[�^���M */
	void	SendTo				(DWORD dwID, CPacketBase *pPacket);					/* �N���C�A���g�փf�[�^���M */
	DWORD	GetThrowghPutSend	(DWORD dwID);										/* �����N�E�X���[�v�b�g�ʂ��擾 */
	DWORD	GetThrowghPutRecv	(DWORD dwID);										/* �����N�E�X���[�v�b�g�ʂ��擾 */
	DWORD	GetQueCount			(DWORD dwID);										/* �L���[�����擾 */
	DWORD	GetIPAddress		(DWORD dwID);										/* IP�A�h���X���擾 */


protected:
	HMODULE			m_hDll;					/* �ʐM���C�u����DLL */
	CUraraSockTCP	*m_pSock;				/* �ʐM���C�u���� */
} CUraraSockTCPSBO, *PCUraraSockTCPSBO;

/* Copyright(C)URARA-works 2003-2005 */
