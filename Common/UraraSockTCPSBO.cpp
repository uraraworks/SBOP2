/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* UraraSockTCPSBO.cpp														 */
/* �ėpTCP�ʐM�N���X														 */
/* 2006/11/05 �쐬�J�n														 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Packet/PacketBase.h"
#include "UraraSockTCPSBO.h"

/* ========================================================================= */
/* �֐���	:CUraraSockTCPSBO::CUraraSockTCPSBO								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

CUraraSockTCPSBO::CUraraSockTCPSBO(void)
{
	char szFileName[MAX_PATH];
	PFGETURARASOCKTCP pfGetUraraSockTCP;

	ZeroMemory (szFileName, sizeof (szFileName));

	GetModuleFilePath (szFileName, sizeof (szFileName));
	strcat (szFileName, "UraraSockTCP.dll");

	m_pSock = NULL;

	m_hDll = LoadLibrary(szFileName);
	pfGetUraraSockTCP = (PFGETURARASOCKTCP)GetProcAddress (m_hDll, "GetUraraSockTCP");
	if (pfGetUraraSockTCP) {
		m_pSock = pfGetUraraSockTCP ();
	}
}


/* ========================================================================= */
/* �֐���	:CUraraSockTCPSBO::~CUraraSockTCPSBO							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

CUraraSockTCPSBO::~CUraraSockTCPSBO(void)
{
	SAFE_DELETE (m_pSock);
	if (m_hDll) {
		FreeLibrary (m_hDll);
	}
}


void CUraraSockTCPSBO::DeleteRecvData(PBYTE pData)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->DeleteRecvData (pData);
}


/* ========================================================================= */
/* �֐���	:CUraraSockTCPSBO::Destroy										 */
/* ���e		:��n��															 */
/* ���t		:2003/10/28														 */
/* ========================================================================= */

void CUraraSockTCPSBO::Destroy(void)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->Destroy ();
}


/* ========================================================================= */
/* �֐���	:CUraraSockTCPSBO::Host											 */
/* ���e		:�ڑ��҂��J�n													 */
/* ���t		:2003/10/28														 */
/* ========================================================================= */

BOOL CUraraSockTCPSBO::Host(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount)
{
	if (m_pSock == NULL) {
		return FALSE;
	}

	return m_pSock->Host (hWndParent, dwMsgBase, dwKey, wPort, dwCount);
}


/* ========================================================================= */
/* �֐���	:CUraraSockTCPSBO::Connect										 */
/* ���e		:�T�[�o�[�֐ڑ�													 */
/* ���t		:2003/10/29														 */
/* ========================================================================= */

BOOL CUraraSockTCPSBO::Connect(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr)
{
	if (m_pSock == NULL) {
		return FALSE;
	}

	return m_pSock->Connect (hWndParent, dwMsgBase, dwKey, wPort, pszAddr);
}


/* ========================================================================= */
/* �֐���	:CUraraSockTCPSBO::DeleteClient									 */
/* ���e		:�N���C�A���g��ؒf												 */
/* ���t		:2003/11/03														 */
/* ========================================================================= */

void CUraraSockTCPSBO::DeleteClient(DWORD dwID)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->DeleteClient (dwID);
}


/* ========================================================================= */
/* �֐���	:CUraraSockTCPSBO::SendCancel									 */
/* ���e		:���M�L�����Z��													 */
/* ���t		:2003/11/03														 */
/* ========================================================================= */
void CUraraSockTCPSBO::SendCancel(DWORD dwID)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->SendCancel (dwID);
}


/* ========================================================================= */
/* �֐���	:CUraraSockTCPSBO::Send											 */
/* ���e		:�T�[�o�[�փf�[�^���M											 */
/* ���t		:2003/11/01														 */
/* ========================================================================= */

void CUraraSockTCPSBO::Send(CPacketBase *pPacket)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->Send (pPacket->m_pPacket, pPacket->m_dwPacketSize);
}


/* ========================================================================= */
/* �֐���	:CUraraSockTCPSBO::SendTo										 */
/* ���e		:�N���C�A���g�փf�[�^���M										 */
/* ���t		:2003/11/02														 */
/* ========================================================================= */

void CUraraSockTCPSBO::SendTo(DWORD dwID, CPacketBase *pPacket)
{
	if (m_pSock == NULL) {
		return;
	}

	m_pSock->SendTo (dwID, pPacket->m_pPacket, pPacket->m_dwPacketSize);
}


/* ========================================================================= */
/* �֐���	:CUraraSockTCPSBO::GetThrowghPutSend							 */
/* ���e		:�����N�E�X���[�v�b�g�ʂ��擾									 */
/* ���t		:2003/11/08														 */
/* ========================================================================= */

DWORD CUraraSockTCPSBO::GetThrowghPutSend(DWORD dwID)
{
	if (m_pSock == NULL) {
		return 0;
	}

	return m_pSock->GetThrowghPutSend (dwID);
}


/* ========================================================================= */
/* �֐���	:CUraraSockTCPSBO::GetThrowghPutRecv							 */
/* ���e		:�����N�E�X���[�v�b�g�ʂ��擾									 */
/* ���t		:2003/11/08														 */
/* ========================================================================= */

DWORD CUraraSockTCPSBO::GetThrowghPutRecv(DWORD dwID)
{
	if (m_pSock == NULL) {
		return 0;
	}

	return m_pSock->GetThrowghPutRecv (dwID);
}


/* ========================================================================= */
/* �֐���	:CUraraSockTCPSBO::GetQueCount									 */
/* ���e		:�L���[�����擾													 */
/* ���t		:2003/11/08														 */
/* ========================================================================= */

DWORD CUraraSockTCPSBO::GetQueCount(DWORD dwID)
{
	if (m_pSock == NULL) {
		return 0;
	}

	return m_pSock->GetQueCount (dwID);
}


/* ========================================================================= */
/* �֐���	:CUraraSockTCPSBO::GetIPAddress									 */
/* ���e		:IP�A�h���X���擾												 */
/* ���t		:2005/03/28														 */
/* ========================================================================= */

DWORD CUraraSockTCPSBO::GetIPAddress(DWORD dwID)
{
	if (m_pSock == NULL) {
		return 0;
	}

	return m_pSock->GetIPAddress (dwID);
}

/* Copyright(C)URARA-works 2006 */
