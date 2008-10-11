/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:UraraSockTCP.h												 */
/* ���e			:�ėpTCP�ʐM�N���X ��`�t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2003/10/28													 */
/* ========================================================================= */

#pragma once

#define DLLURARASOCKTCP_API __declspec(dllexport)

/* ========================================================================= */
/* �萔�̒�`																 */
/* ========================================================================= */

#ifndef SAFE_DELETE
	#define SAFE_DELETE(p)			{ if(p) { delete (p);	(p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);	(p)=NULL; } }
#endif

#define URARASOCK_SENDALL		0x00000000						/* �S���ɑ��M */

/* �E�B���h�E���b�Z�[�W(�ݒ肵����l�{�ȉ��̒l) */
enum {
	WM_URARASOCK_HOST = 0,										/* �҂��󂯊J�n */
	WM_URARASOCK_ADDCLIENT,										/* �N���C�A���g���ڑ����� */
	WM_URARASOCK_DECCLIENT,										/* �N���C�A���g���ؒf���� */
	WM_URARASOCK_CONNECT,										/* �T�[�o�[�ɐڑ����� */
	WM_URARASOCK_DISCONNECT,									/* �T�[�o�[�Ɛؒf���� */
	WM_URARASOCK_RECV,											/* �f�[�^��M */
	WM_URARASOCK_SEND,											/* �f�[�^���M */
	WM_URARASOCK_MAX
};

/* ���M�D�揇�� */
enum URARASOCK_SENDPRIORITY {
	URARASOCK_SENDPRIORITY_HIGH = 0,							/* ���� */
	URARASOCK_SENDPRIORITY_MIDDLE,								/* ���� */
	URARASOCK_SENDPRIORITY_LOW,									/* �Ⴂ */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CUraraSockTCP
{
public:
	virtual void	DeleteRecvData		(PBYTE pData)									= 0;	/* ��M�f�[�^���폜 */
	virtual void	Destroy				(void)											= 0;	/* ��n�� */
	virtual BOOL	Host				(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount)	= 0;	/* �ڑ��҂��J�n */
	virtual BOOL	Connect				(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr)	= 0;	/* �T�[�o�[�֐ڑ� */
	virtual void	DeleteClient		(DWORD dwID)									= 0;	/* �N���C�A���g��ؒf */
	virtual void	SendCancel			(DWORD dwID = 0)								= 0;	/* ���M�L�����Z�� */
	virtual void	Send				(PBYTE pData, DWORD dwSize, BYTE byPriority = URARASOCK_SENDPRIORITY_MIDDLE)				= 0;	/* �T�[�o�[�փf�[�^���M */
	virtual void	SendTo				(DWORD dwID, PBYTE pData, DWORD dwSize, BYTE byPriority = URARASOCK_SENDPRIORITY_MIDDLE)	= 0;	/* �N���C�A���g�փf�[�^���M */
	virtual DWORD	GetThrowghPutSend	(DWORD dwID)									= 0;	/* �����N�E�X���[�v�b�g�ʂ��擾 */
	virtual DWORD	GetThrowghPutRecv	(DWORD dwID)									= 0;	/* �����N�E�X���[�v�b�g�ʂ��擾 */
	virtual DWORD	GetQueCount			(DWORD dwID)									= 0;	/* �L���[�����擾 */
	virtual DWORD	GetIPAddress		(DWORD dwID)									= 0;	/* IP�A�h���X���擾 */
};

extern "C"{
DLLURARASOCKTCP_API CUraraSockTCP *GetUraraSockTCP(void);			/* �N���X�̃|�C���^���擾 */
DLLURARASOCKTCP_API void ReleaseUraraSockTCP(CUraraSockTCP *&pSrc);	/* �N���X�̃|�C���^����� */
}
typedef CUraraSockTCP *(*PFGETURARASOCKTCP)(void);
typedef void (*PFRELEASEURARASOCKTCP)(CUraraSockTCP *&pSrc);

/* Copyright(C)URARA-works 2008 */
