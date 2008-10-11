/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_MAPNAME.cpp										 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v���ʒm) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/24													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_MAPNAME.h"

/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPNAME::CPacketMAP_MAPNAME							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/05/24														 */
/* ========================================================================= */

CPacketMAP_MAPNAME::CPacketMAP_MAPNAME()
{
	m_dwMapID		= 0;			/* �}�b�vID */
	m_dwBGMID		= 0;			/* BGMID */
	m_dwWeatherType	= 0;			/* �V�C��� */
	m_byLevel		= 0;			/* ���邳���x�� */
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPNAME::~CPacketMAP_MAPNAME						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/05/24														 */
/* ========================================================================= */

CPacketMAP_MAPNAME::~CPacketMAP_MAPNAME()
{
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPNAME::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/05/24														 */
/* ========================================================================= */

void CPacketMAP_MAPNAME::Make(
	DWORD dwMapID,			/* [in] �}�b�vID */
	DWORD dwBGMID,			/* [in] BGMID */
	DWORD dwWeatherType,	/* [in] �V�C��� */
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
			 sizeof (byLevel)		+
			 (strlen (pszMapName) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_MAPNAME;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,		sizeof (dwMapID),		pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (pDataTmp, &dwBGMID,		sizeof (dwBGMID),		pDataTmp);	/* BGMID */
	CopyMemoryRenew (pDataTmp, &dwWeatherType,	sizeof (dwWeatherType),	pDataTmp);	/* �V�C��� */
	CopyMemoryRenew (pDataTmp, &byLevel,		sizeof (byLevel),		pDataTmp);	/* ���邳���x�� */
	strcpyRenew ((LPSTR)pDataTmp, pszMapName, pDataTmp);				/* �}�b�v�� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_MAPNAME::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/05/24														 */
/* ========================================================================= */

PBYTE CPacketMAP_MAPNAME::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID,		pDataTmp, sizeof (m_dwMapID),		pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (&m_dwBGMID,		pDataTmp, sizeof (m_dwBGMID),		pDataTmp);	/* BGMID */
	CopyMemoryRenew (&m_dwWeatherType,	pDataTmp, sizeof (m_dwWeatherType),	pDataTmp);	/* �V�C��� */
	CopyMemoryRenew (&m_byLevel,		pDataTmp, sizeof (m_byLevel),		pDataTmp);	/* ���邳���x�� */
	StoreRenew (m_strMapName, (LPCSTR)pDataTmp, pDataTmp);					/* �}�b�v�� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
