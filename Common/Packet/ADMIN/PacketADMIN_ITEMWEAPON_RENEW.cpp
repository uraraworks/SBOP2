/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEMWEAPON_RENEW.cpp							 */
/* ���e			:�R�}���h(�A�C�e���n:������X�V) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoItemWeapon.h"
#include "Command.h"
#include "PacketADMIN_ITEMWEAPON_RENEW.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMWEAPON_RENEW::CPacketADMIN_ITEMWEAPON_RENEW	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CPacketADMIN_ITEMWEAPON_RENEW::CPacketADMIN_ITEMWEAPON_RENEW()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMWEAPON_RENEW::~CPacketADMIN_ITEMWEAPON_RENEW	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CPacketADMIN_ITEMWEAPON_RENEW::~CPacketADMIN_ITEMWEAPON_RENEW()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMWEAPON_RENEW::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CPacketADMIN_ITEMWEAPON_RENEW::Make(
	CInfoItemWeapon *pInfo)		/* [in] ������ */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE) +
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEMWEAPON_RENEW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* ������ */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMWEAPON_RENEW::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

PBYTE CPacketADMIN_ITEMWEAPON_RENEW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoItemWeapon;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
