/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_SETMAPNAME.cpp								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�L�������ʒm) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/24													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_MAP_SETMAPNAME.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_SETMAPNAME::CPacketADMIN_MAP_SETMAPNAME		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/05/24														 */
/* ========================================================================= */

CPacketADMIN_MAP_SETMAPNAME::CPacketADMIN_MAP_SETMAPNAME()
{
	m_dwMapID		= 0;			/* �}�b�vID */
	m_dwBGMID		= 0;			/* BGMID */
	m_dwWeatherType	= 0;			/* �V�C��� */
	m_bEnableBattle	= TRUE;			/* �퓬���� */
	m_bRecovery		= TRUE;			/* �C���񕜂��� */
	m_byLevel		= 0;			/* ���邳���x�� */
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_SETMAPNAME::~CPacketADMIN_MAP_SETMAPNAME		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/05/24														 */
/* ========================================================================= */

CPacketADMIN_MAP_SETMAPNAME::~CPacketADMIN_MAP_SETMAPNAME()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_SETMAPNAME::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/05/24														 */
/* ========================================================================= */

void CPacketADMIN_MAP_SETMAPNAME::Make(
	DWORD dwMapID,			/* [in] �}�b�vID */
	DWORD dwBGMID,			/* [in] BGMID */
	DWORD dwWeatherType,	/* [in] �V�C��� */
	BOOL bEnableBattle,		/* [in] �퓬���� */
	BOOL bRecovery,			/* [in] �C���񕜂��� */
	BYTE byLevel,			/* [in] ���邳���x�� */
	LPCSTR pszMapName)		/* [in] �}�b�v�� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMapID)		+
			 sizeof (dwBGMID)		+
			 sizeof (dwWeatherType)	+
			 sizeof (bEnableBattle)	+
			 sizeof (bRecovery)		+
			 sizeof (byLevel)		+
			 (strlen (pszMapName) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_SETMAPNAME;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,		sizeof (dwMapID),		pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (pDataTmp, &dwBGMID,		sizeof (dwBGMID),		pDataTmp);	/* BGMID */
	CopyMemoryRenew (pDataTmp, &dwWeatherType,	sizeof (dwWeatherType),	pDataTmp);	/* �V�C��� */
	CopyMemoryRenew (pDataTmp, &bEnableBattle,	sizeof (bEnableBattle),	pDataTmp);	/* �퓬���� */
	CopyMemoryRenew (pDataTmp, &bRecovery,		sizeof (bRecovery),		pDataTmp);	/* �C���񕜂��� */
	CopyMemoryRenew (pDataTmp, &byLevel,		sizeof (byLevel),		pDataTmp);	/* ���邳���x�� */
	strcpyRenew ((LPSTR)pDataTmp, pszMapName, pDataTmp);				/* �}�b�v�� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_SETMAPNAME::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/05/24														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_SETMAPNAME::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID,		pDataTmp, sizeof (m_dwMapID),		pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (&m_dwBGMID,		pDataTmp, sizeof (m_dwBGMID),		pDataTmp);	/* BGMID */
	CopyMemoryRenew (&m_dwWeatherType,	pDataTmp, sizeof (m_dwWeatherType),	pDataTmp);	/* �V�C��� */
	CopyMemoryRenew (&m_bEnableBattle,	pDataTmp, sizeof (m_bEnableBattle),	pDataTmp);	/* �퓬���� */
	CopyMemoryRenew (&m_bRecovery,		pDataTmp, sizeof (m_bRecovery),		pDataTmp);	/* �C���񕜂��� */
	CopyMemoryRenew (&m_byLevel,		pDataTmp, sizeof (m_byLevel),		pDataTmp);	/* ���邳���x�� */
	StoreRenew (m_strMapName, (LPCSTR)pDataTmp, pDataTmp);								/* �}�b�v�� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
