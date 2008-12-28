/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrData.h													 */
/* ���e			:�f�[�^�}�l�[�W���N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include <comdef.h>
#include "InfoAccount.h"
#include "InfoMapEventBase.h"
#include "InfoTalkEvent.h"
#include "LibInfoMapBase.h"
#include "LibInfoMapObject.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapShadow.h"
#include "LibInfoItem.h"
#include "LibInfoItemType.h"
#include "LibInfoItemWeapon.h"
#include "LibInfoEffect.h"
#include "LibInfoMotion.h"
#include "LibInfoMotionType.h"
#include "LibInfoEfcBalloon.h"
#include "LibInfoSystem.h"
#include "LibInfoSkill.h"
#include "CryptUtil.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MainFrame.h"
#include "MgrDraw.h"
#include "MgrSound.h"
#include "MgrGrpData.h"
#include "MgrLayer.h"
#include "MgrWindow.h"
#include "MgrKeyInput.h"
#include "MgrData.h"


/* ========================================================================= */
/* �֐���	:CMgrData::CMgrData												 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

CMgrData::CMgrData()
{
	m_hWndMain				= NULL;
	m_hWndAdmin				= NULL;
	m_pSock					= NULL;
	m_hInstance				= NULL;
	m_wServerPort			= 0;
	m_dwSelectMapPartsID	= 0;
	m_dwSelectMapShadowID	= 0;
	m_dwSetItemID			= 1;
	m_nOnlineCount			= 0;
	m_nAdminLevel			= ADMINLEVEL_NONE;
	m_nDrawMode				= 1;
	m_nChatModeBack			= 0;
	m_nAdminNotifyTypeL		= ADMINNOTIFYTYPE_NONE;
	m_nAdminNotifyTypeR		= ADMINNOTIFYTYPE_NONE;
	m_nAdminNotifyTypeRR	= ADMINNOTIFYTYPE_NONE;
	m_nSEVolume				= 2;
	m_nBGMVolume			= 2;
	m_nSleepTimer			= 0;
	m_bEditMapPile			= FALSE;
	m_bMoveNoBlock			= FALSE;
	m_bSavePassword			= FALSE;
	m_bOptionTaskbar		= FALSE;
	m_bOptionViewChat		= TRUE;
	m_bOptionViewItem		= TRUE;
	m_bOptionViewItemName	= TRUE;
	m_bOptionViewHelpIcon	= TRUE;
	m_bOptionBattleMsgLog	= FALSE;
	m_bOption60Frame		= FALSE;
	m_byViewGrid			= 0;

	m_pMainFrame			= NULL;
	m_pMgrGrpData			= NULL;
	m_pPlayerChar			= NULL;
	m_pMap					= NULL;

	m_pInfoAccount			= NULL;
	m_pInfoTalkEvent		= NULL;
	m_pMgrSound				= NULL;
	m_pMgrDraw				= NULL;
	m_pMgrLayer				= NULL;
	m_pMgrWindow			= NULL;
	m_pMgrKeyInput			= NULL;
	m_pLibInfoChar			= NULL;
	m_pLibInfoMap			= NULL;
	m_pLibInfoMapObject		= NULL;
	m_pLibInfoMapParts		= NULL;
	m_pLibInfoMapShadow		= NULL;
	m_pLibInfoItem			= NULL;
	m_pLibInfoItemType		= NULL;
	m_pLibInfoItemWeapon	= NULL;
	m_pLibInfoEffect		= NULL;
	m_pLibInfoMotion		= NULL;
	m_pLibInfoMotionType	= NULL;
	m_pLibInfoEfcBalloon	= NULL;
	m_pLibInfoSystem		= NULL;
	m_pLibInfoSkill			= NULL;

	m_pLibInfoCharOnline	= NULL;

	ZeroMemory (&m_stInputGuid, sizeof (m_stInputGuid));
	m_sizeDlgMapPartsEdit.cx = 0;
	m_sizeDlgMapPartsEdit.cy = 0;
	m_sizeWndMap.cx			 = 400;
	m_sizeWndMap.cy			 = 400;
	m_nLastSelectGrpNo		 = 0;
}


/* ========================================================================= */
/* �֐���	:CMgrData::~CMgrData											 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

CMgrData::~CMgrData()
{
	Destroy ();

	SAFE_DELETE (m_pInfoAccount);
	SAFE_DELETE (m_pInfoTalkEvent);
	SAFE_DELETE (m_pMgrSound);
	SAFE_DELETE (m_pMgrDraw);
	SAFE_DELETE (m_pMgrLayer);
	SAFE_DELETE (m_pMgrWindow);
	SAFE_DELETE (m_pMgrKeyInput);
	SAFE_DELETE (m_pLibInfoChar);
	SAFE_DELETE (m_pLibInfoMap);
	SAFE_DELETE (m_pLibInfoMapObject);
	SAFE_DELETE (m_pLibInfoMapParts);
	SAFE_DELETE (m_pLibInfoMapShadow);
	SAFE_DELETE (m_pLibInfoItem);
	SAFE_DELETE (m_pLibInfoItemType);
	SAFE_DELETE (m_pLibInfoItemWeapon);
	SAFE_DELETE (m_pLibInfoEffect);
	SAFE_DELETE (m_pLibInfoMotion);
	SAFE_DELETE (m_pLibInfoMotionType);
	SAFE_DELETE (m_pLibInfoEfcBalloon);
	SAFE_DELETE (m_pLibInfoSystem);
	SAFE_DELETE (m_pLibInfoSkill);

	SAFE_DELETE (m_pLibInfoCharOnline);
}


/* ========================================================================= */
/* �֐���	:CMgrData::Create												 */
/* ���e		:�쐬															 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrData::Create(
	CMainFrame		*pMainFrame,		/* [in] ���C���t���[�� */
	CMgrGrpData		*pMgrGrpData)		/* [in] �O���t�B�b�N�f�[�^�Ǘ� */
{
	m_pMainFrame	= pMainFrame;
	m_pMgrGrpData	= pMgrGrpData;

	m_pInfoAccount			= new CInfoAccount;
	m_pInfoTalkEvent		= new CInfoTalkEvent;
	m_pMgrSound				= new CMgrSound;
	m_pMgrDraw				= new CMgrDraw;
	m_pMgrLayer				= new CMgrLayer;
	m_pMgrWindow			= new CMgrWindow;
	m_pMgrKeyInput			= new CMgrKeyInput;
	m_pLibInfoChar			= new CLibInfoCharCli;
	m_pLibInfoMap			= new CLibInfoMapBase;
	m_pLibInfoMapObject		= new CLibInfoMapObject;
	m_pLibInfoMapParts		= new CLibInfoMapParts;
	m_pLibInfoMapShadow		= new CLibInfoMapShadow;
	m_pLibInfoItem			= new CLibInfoItem;
	m_pLibInfoItemType		= new CLibInfoItemType;
	m_pLibInfoItemWeapon	= new CLibInfoItemWeapon;
	m_pLibInfoEffect		= new CLibInfoEffect;
	m_pLibInfoMotion		= new CLibInfoMotion;
	m_pLibInfoMotionType	= new CLibInfoMotionType;
	m_pLibInfoEfcBalloon	= new CLibInfoEfcBalloon;
	m_pLibInfoSystem		= new CLibInfoSystem;
	m_pLibInfoSkill			= new CLibInfoSkill;
	m_pLibInfoCharOnline	= new CLibInfoCharCli;

	ReadIniData ();

	m_pMgrSound->			Create ();
	m_pMgrDraw->			Create (this);
	m_pMgrLayer->			Create (this);
	m_pMgrWindow->			Create (this);
	m_pMgrKeyInput->		Create ();
	m_pLibInfoChar->		Create (this);
	m_pLibInfoMapObject->	Create ();
	m_pLibInfoMapParts->	Create ();
	m_pLibInfoMapShadow->	Create ();
	m_pLibInfoMap->			Create (m_pLibInfoMapParts);
	m_pLibInfoItem->		Create ();
	m_pLibInfoItemType->	Create ();
	m_pLibInfoItemWeapon->	Create ();
	m_pLibInfoEffect->		Create ();
	m_pLibInfoMotion->		Create ();
	m_pLibInfoMotionType->	Create ();
	m_pLibInfoEfcBalloon->	Create ();
	m_pLibInfoSystem->		Create ();
	m_pLibInfoSkill->		Create ();

	m_pLibInfoCharOnline->	Create (this);

	m_pLibInfoItem->SetTypeInfo (m_pLibInfoItemType);
	m_pLibInfoItem->SetWeaponInfo (m_pLibInfoItemWeapon);

	m_pMgrSound->SetBGMVolume (m_nBGMVolume);
	m_pMgrSound->SetSEVolume (m_nSEVolume);
}


/* ========================================================================= */
/* �֐���	:CMgrData::Destroy												 */
/* ���e		:�j��															 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrData::Destroy(void)
{
	if (m_pMgrSound)			{	m_pMgrSound->			Destroy ();	}
	if (m_pMgrDraw)				{	m_pMgrDraw->			Destroy ();	}
	if (m_pMgrLayer)			{	m_pMgrLayer->			Destroy ();	}
	if (m_pMgrWindow)			{	m_pMgrWindow->			Destroy ();	}
	if (m_pMgrKeyInput)			{	m_pMgrKeyInput->		Destroy ();	}
	if (m_pLibInfoChar)			{	m_pLibInfoChar->		Destroy ();	}
	if (m_pLibInfoMap)			{	m_pLibInfoMap->			Destroy ();	}
	if (m_pLibInfoMapObject)	{	m_pLibInfoMapObject->	Destroy ();	}
	if (m_pLibInfoMapParts)		{	m_pLibInfoMapParts->	Destroy ();	}
	if (m_pLibInfoMapShadow)	{	m_pLibInfoMapShadow->	Destroy ();	}
	if (m_pLibInfoItem)			{	m_pLibInfoItem->		Destroy ();	}
	if (m_pLibInfoItemType)		{	m_pLibInfoItemType->	Destroy ();	}
	if (m_pLibInfoItemWeapon)	{	m_pLibInfoItemWeapon->	Destroy ();	}
	if (m_pLibInfoEffect)		{	m_pLibInfoEffect->		Destroy ();	}
	if (m_pLibInfoMotion)		{	m_pLibInfoMotion->		Destroy ();	}
	if (m_pLibInfoMotionType)	{	m_pLibInfoMotionType->	Destroy ();	}
	if (m_pLibInfoEfcBalloon)	{	m_pLibInfoEfcBalloon->	Destroy ();	}
	if (m_pLibInfoSystem)		{	m_pLibInfoSystem->		Destroy ();	}
	if (m_pLibInfoSkill)		{	m_pLibInfoSkill->		Destroy ();	}

	DeleteAllSystemMsg ();
}


/* ========================================================================= */
/* �֐���	:CMgrData::SaveIniData											 */
/* ���e		:�ݒ����ۑ�													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

void CMgrData::SaveIniData(void)
{
	char szFileName[MAX_PATH], szTmp[128];
	WCHAR szwGuid[40];
	_bstr_t bstrTmp;
	CmyString strTmp;
	CCryptUtil CryptUtil;

	ZeroMemory (szFileName, sizeof (szFileName));
	GetModuleFileName (NULL, szFileName, MAX_PATH);
	strcpy (szFileName + strlen (szFileName) - 3, "ini");

	/* �Í��� */
	CryptUtil.CryptStr (m_strLastPassword, szTmp, 10);
	/* �ŏI�A�J�E���g�� */
	WritePrivateProfileString ("Account", "Account", m_strLastAccount, szFileName);
	/* �ŏI�p�X���[�h */
	WritePrivateProfileString ("Account", "Password", szTmp, szFileName);
	/* �p�X���[�h��ۑ�����H */
	strTmp.Format ("%d", m_bSavePassword);
	WritePrivateProfileString ("Setting", "SavePassword", strTmp, szFileName);
	/* �������Ƀ^�X�N�o�[�`�J�`�J */
	strTmp.Format ("%d", m_bOptionTaskbar);
	WritePrivateProfileString ("Setting", "OptionTaskbar", strTmp, szFileName);
	/* ������\������ */
	strTmp.Format ("%d", m_bOptionViewChat);
	WritePrivateProfileString ("Setting", "OptionViewChat", strTmp, szFileName);
	/* �A�C�e����\������ */
	strTmp.Format ("%d", m_bOptionViewItem);
	WritePrivateProfileString ("Setting", "OptionViewItem", strTmp, szFileName);
	/* �A�C�e������\������ */
	strTmp.Format ("%d", m_bOptionViewItemName);
	WritePrivateProfileString ("Setting", "OptionViewItemName", strTmp, szFileName);
	/* �w���v�A�C�R����\������ */
	strTmp.Format ("%d", m_bOptionViewHelpIcon);
	WritePrivateProfileString ("Setting", "OptionViewHelpIcon", strTmp, szFileName);
	/* �퓬���b�Z�[�W�����O�Ɏc�� */
	strTmp.Format ("%d", m_bOptionBattleMsgLog);
	WritePrivateProfileString ("Setting", "OptionBattleMsgLog", strTmp, szFileName);
	/* 60�t���[���ŕ\������ */
	strTmp.Format ("%d", m_bOption60Frame);
	WritePrivateProfileString ("Setting", "Option60Frame", strTmp, szFileName);
	/* ���ʉ��� */
	strTmp.Format ("%d", m_nSEVolume);
	WritePrivateProfileString ("Setting", "SEVolume", strTmp, szFileName);
	/* BGM���� */
	strTmp.Format ("%d", m_nBGMVolume);
	WritePrivateProfileString ("Setting", "BGMVolume", strTmp, szFileName);
	/* �`�惂�[�h */
	strTmp.Format ("%d", m_nDrawMode);
	WritePrivateProfileString ("Setting", "DrawMode", strTmp, szFileName);
	/* ���̓W���C�p�b�h */
	StringFromGUID2 (m_stInputGuid, szwGuid, sizeof (szwGuid));
	bstrTmp = szwGuid;
	strcpy (szTmp, bstrTmp);
	WritePrivateProfileString ("Setting", "InputDevice", szTmp, szFileName);
	/* ���Ђ�˃^�C�}�[ */
	strTmp.Format ("%d", m_nSleepTimer);
	WritePrivateProfileString ("Setting", "SleepTimer", strTmp, szFileName);
}


/* ========================================================================= */
/* �֐���	:CMgrData::SetWindowInfo										 */
/* ���e		:�E�B���h�E����ݒ�											 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrData::SetWindowInfo(HINSTANCE hInstance, HWND hWndMain)
{
	m_hInstance	= hInstance;
	m_hWndMain	= hWndMain;
}


/* ========================================================================= */
/* �֐���	:CMgrData::SetAccount											 */
/* ���e		:�g�p���̃A�J�E���g����ݒ�									 */
/* ���t		:2006/11/08														 */
/* ========================================================================= */

void CMgrData::SetAccount(CInfoAccount *pInfoAccount)
{
	m_pInfoAccount->Copy (pInfoAccount);
}


/* ========================================================================= */
/* �֐���	:CMgrData::GetAccountID											 */
/* ���e		:�g�p���̃A�J�E���gID���擾										 */
/* ���t		:2006/12/02														 */
/* ========================================================================= */

DWORD CMgrData::GetAccountID(void)
{
	return m_pInfoAccount->m_dwAccountID;
}


/* ========================================================================= */
/* �֐���	:CMgrData::SetCharID											 */
/* ���e		:�g�p���̃L����ID��ݒ�											 */
/* ���t		:2007/03/21														 */
/* ========================================================================= */

void CMgrData::SetCharID(DWORD dwCharID)
{
	m_pInfoAccount->m_dwCharID = dwCharID;
}


/* ========================================================================= */
/* �֐���	:CMgrData::GetCharID											 */
/* ���e		:�g�p���̃L����ID���擾											 */
/* ���t		:2007/01/02														 */
/* ========================================================================= */

DWORD CMgrData::GetCharID(void)
{
	return m_pInfoAccount->m_dwCharID;
}


/* ========================================================================= */
/* �֐���	:CMgrData::GetMapID												 */
/* ���e		:���݂̃}�b�vID���擾											 */
/* ���t		:2008/11/29														 */
/* ========================================================================= */

DWORD CMgrData::GetMapID(void)
{
	DWORD dwRet;

	dwRet = 0;
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}

	dwRet = m_pPlayerChar->m_dwMapID;
Exit:
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CMgrData::GetFamilyTypeName									 */
/* ���e		:�푰�����擾													 */
/* ���t		:2007/04/07														 */
/* ========================================================================= */

LPCSTR CMgrData::GetFamilyTypeName(WORD wFamilyTypeID)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (wFamilyTypeID) {
	case FAMILYTYPE_HUMAN:	pszRet = "�j���Q��";	break;		/* �j���Q�� */
	case FAMILYTYPE_ELF:	pszRet = "�G���t";		break;		/* �G���t */
	case FAMILYTYPE_BST:	pszRet = "�W���E�W��";	break;		/* �W���E�W�� */
	case FAMILYTYPE_DAEMON:	pszRet = "�}�]�N";		break;		/* �}�]�N */
	}

	return pszRet;
}


/* ========================================================================= */
/* �֐���	:CMgrData::GetHairTypeName										 */
/* ���e		:���^�����擾													 */
/* ���t		:2007/04/07														 */
/* ========================================================================= */

LPCSTR CMgrData::GetHairTypeName(WORD wHairTypeID)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (wHairTypeID) {
	case 1:		pszRet = "�g���j�e�B";		break;
	case 2:		pszRet = "�c�C��";			break;
	case 3:		pszRet = "�J�E�{�[�C";		break;
	case 4:		pszRet = "�G�b�O";			break;
	case 5:		pszRet = "�i�C�g";			break;
	case 6:		pszRet = "�E�F�[�u";		break;
	case 7:		pszRet = "�V���C��";		break;
	case 8:		pszRet = "�|�P�b�g";		break;
	case 9:		pszRet = "�u���C�u";		break;
	case 10:	pszRet = "�N�[��";			break;
	case 11:	pszRet = "���t�g";			break;
	case 12:	pszRet = "�e�C���Y";		break;
	case 13:	pszRet = "�|�j�[";			break;
	case 14:	pszRet = "�G�b�W";			break;
	case 15:	pszRet = "���C�g";			break;
	case 16:	pszRet = "�g���C�t��";		break;
	case 17:	pszRet = "�L���b�X��";		break;
	case 18:	pszRet = "�s�[�X";			break;
	case 19:	pszRet = "���g���f�r��";	break;
	case 20:	pszRet = "�E�C���h";		break;
	case 21:	pszRet = "�m�[�u��";		break;
	case 22:	pszRet = "�t�F�A���[";		break;
	}

	return pszRet;
}


/* ========================================================================= */
/* �֐���	:CMgrData::GetHairColorName										 */
/* ���e		:���F�����擾													 */
/* ���t		:2007/05/11														 */
/* ========================================================================= */

LPCSTR CMgrData::GetHairColorName(WORD wHairColorID)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (wHairColorID) {
	case 1:		pszRet = "�O���C";		break;
	case 2:		pszRet = "�u���E��";	break;
	case 3:		pszRet = "�A�b�V��";	break;
	case 4:		pszRet = "�V���o�[";	break;
	case 5:		pszRet = "�N�����]��";	break;
	case 6:		pszRet = "�X�J�C";		break;
	case 7:		pszRet = "�u�����h";	break;
	case 8:		pszRet = "�O���[��";	break;
	case 9:		pszRet = "�`�F���[";	break;
	case 10:	pszRet = "�O���[�v";	break;
	case 11:	pszRet = "�I�����W";	break;
	case 12:	pszRet = "���[�t";		break;
	case 13:	pszRet = "�A�N�A";		break;
	}

	return pszRet;
}


/* ========================================================================= */
/* �֐���	:CMgrData::GetEyeColorName										 */
/* ���e		:�ڐF�����擾													 */
/* ���t		:2007/05/11														 */
/* ========================================================================= */

LPCSTR CMgrData::GetEyeColorName(WORD wEyeColorID)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (wEyeColorID) {
	case 1:		pszRet = "�u���b�N";	break;
	case 2:		pszRet = "�g�p�[�Y";	break;
	case 3:		pszRet = "���r�[";		break;
	case 4:		pszRet = "�A���W�X�g";	break;
	case 5:		pszRet = "�G�������h";	break;
	case 6:		pszRet = "�T�t�@�C�A";	break;
	}

	return pszRet;
}


/* ========================================================================= */
/* �֐���	:CMgrData::GetMotionName										 */
/* ���e		:���[�V���������擾												 */
/* ���t		:2007/05/11														 */
/* ========================================================================= */

LPCSTR CMgrData::GetMotionName(DWORD dwMotionID)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (dwMotionID) {
	case CHARMOTIONLISTID_STAND_UP:					pszRet = "����(��)";				break;	/* ����(��) */
	case CHARMOTIONLISTID_STAND_DOWN:				pszRet = "����(��)";				break;	/* ����(��) */
	case CHARMOTIONLISTID_STAND_LEFT:				pszRet = "����(��)";				break;	/* ����(��) */
	case CHARMOTIONLISTID_STAND_RIGHT:				pszRet = "����(�E)";				break;	/* ����(�E) */
	case CHARMOTIONLISTID_WALK_UP:					pszRet = "����(��)";				break;	/* ����(��) */
	case CHARMOTIONLISTID_WALK_DOWN:				pszRet = "����(��)";				break;	/* ����(��) */
	case CHARMOTIONLISTID_WALK_LEFT:				pszRet = "����(��)";				break;	/* ����(��) */
	case CHARMOTIONLISTID_WALK_RIGHT:				pszRet = "����(�E)";				break;	/* ����(�E) */
	case CHARMOTIONLISTID_SIT_UP:					pszRet = "����(��)";				break;	/* ����(��) */
	case CHARMOTIONLISTID_SIT_DOWN:					pszRet = "����(��)";				break;	/* ����(��) */
	case CHARMOTIONLISTID_SIT_LEFT:					pszRet = "����(��)";				break;	/* ����(��) */
	case CHARMOTIONLISTID_SIT_RIGHT:				pszRet = "����(�E)";				break;	/* ����(�E) */
	case CHARMOTIONLISTID_BATTLESTAND_UP:			pszRet = "�퓬����(��)";			break;	/* �퓬����(��) */
	case CHARMOTIONLISTID_BATTLESTAND_DOWN:			pszRet = "�퓬����(��)";			break;	/* �퓬����(��) */
	case CHARMOTIONLISTID_BATTLESTAND_LEFT:			pszRet = "�퓬����(��)";			break;	/* �퓬����(��) */
	case CHARMOTIONLISTID_BATTLESTAND_RIGHT:		pszRet = "�퓬����(�E)";			break;	/* �퓬����(�E) */
	case CHARMOTIONLISTID_BATTLEWALK_UP:			pszRet = "���葫(��)";				break;	/* ���葫(��) */
	case CHARMOTIONLISTID_BATTLEWALK_DOWN:			pszRet = "���葫(��)";				break;	/* ���葫(��) */
	case CHARMOTIONLISTID_BATTLEWALK_LEFT:			pszRet = "���葫(��)";				break;	/* ���葫(��) */
	case CHARMOTIONLISTID_BATTLEWALK_RIGHT:			pszRet = "���葫(�E)";				break;	/* ���葫(�E) */
	case CHARMOTIONLISTID_SWING_UP:					pszRet = "�U��(��)";				break;	/* �U��(��) */
	case CHARMOTIONLISTID_SWING_DOWN:				pszRet = "�U��(��)";				break;	/* �U��(��) */
	case CHARMOTIONLISTID_SWING_LEFT:				pszRet = "�U��(��)";				break;	/* �U��(��) */
	case CHARMOTIONLISTID_SWING_RIGHT:				pszRet = "�U��(�E)";				break;	/* �U��(�E) */
	case CHARMOTIONLISTID_POKE_UP:					pszRet = "�˂�(��)";				break;	/* �˂�(��) */
	case CHARMOTIONLISTID_POKE_DOWN:				pszRet = "�˂�(��)";				break;	/* �˂�(��) */
	case CHARMOTIONLISTID_POKE_LEFT:				pszRet = "�˂�(��)";				break;	/* �˂�(��) */
	case CHARMOTIONLISTID_POKE_RIGHT:				pszRet = "�˂�(�E)";				break;	/* �˂�(�E) */
	case CHARMOTIONLISTID_BOWWALK_UP:				pszRet = "�|�p���葫(��)";			break;	/* �|�p���葫(��) */
	case CHARMOTIONLISTID_BOWWALK_DOWN:				pszRet = "�|�p���葫(��)";			break;	/* �|�p���葫(��) */
	case CHARMOTIONLISTID_BOWWALK_LEFT:				pszRet = "�|�p���葫(��)";			break;	/* �|�p���葫(��) */
	case CHARMOTIONLISTID_BOWWALK_RIGHT:			pszRet = "�|�p���葫(�E)";			break;	/* �|�p���葫(�E) */
	case CHARMOTIONLISTID_BOW_UP:					pszRet = "�|(��)";					break;	/* �|(��) */
	case CHARMOTIONLISTID_BOW_DOWN:					pszRet = "�|(��)";					break;	/* �|(��) */
	case CHARMOTIONLISTID_BOW_LEFT:					pszRet = "�|(��)";					break;	/* �|(��) */
	case CHARMOTIONLISTID_BOW_RIGHT:				pszRet = "�|(�E)";					break;	/* �|(�E) */
	case CHARMOTIONLISTID_GLOVEWALK_UP:				pszRet = "�Ō��p���葫(��)";		break;	/* �Ō��p���葫(��) */
	case CHARMOTIONLISTID_GLOVEWALK_DOWN:			pszRet = "�Ō��p���葫(��)";		break;	/* �Ō��p���葫(��) */
	case CHARMOTIONLISTID_GLOVEWALK_LEFT:			pszRet = "�Ō��p���葫(��)";		break;	/* �Ō��p���葫(��) */
	case CHARMOTIONLISTID_GLOVEWALK_RIGHT:			pszRet = "�Ō��p���葫(�E)";		break;	/* �Ō��p���葫(�E) */
	case CHARMOTIONLISTID_GLOVE_UP:					pszRet = "�Ō�(��)";				break;	/* �Ō�(��) */
	case CHARMOTIONLISTID_GLOVE_DOWN:				pszRet = "�Ō�(��)";				break;	/* �Ō�(��) */
	case CHARMOTIONLISTID_GLOVE_LEFT:				pszRet = "�Ō�(��)";				break;	/* �Ō�(��) */
	case CHARMOTIONLISTID_GLOVE_RIGHT:				pszRet = "�Ō�(�E)";				break;	/* �Ō�(�E) */
	case CHARMOTIONLISTID_FISHINGWALK_UP:			pszRet = "�ނ�p���葫(��)";		break;	/* �ނ�p���葫(��) */
	case CHARMOTIONLISTID_FISHINGWALK_DOWN:			pszRet = "�ނ�p���葫(��)";		break;	/* �ނ�p���葫(��) */
	case CHARMOTIONLISTID_FISHINGWALK_LEFT:			pszRet = "�ނ�p���葫(��)";		break;	/* �ނ�p���葫(��) */
	case CHARMOTIONLISTID_FISHINGWALK_RIGHT:		pszRet = "�ނ�p���葫(�E)";		break;	/* �ނ�p���葫(�E) */
	case CHARMOTIONLISTID_FISHING_UP:				pszRet = "�ނ�(��)";				break;	/* �ނ�(��) */
	case CHARMOTIONLISTID_FISHING_DOWN:				pszRet = "�ނ�(��)";				break;	/* �ނ�(��) */
	case CHARMOTIONLISTID_FISHING_LEFT:				pszRet = "�ނ�(��)";				break;	/* �ނ�(��) */
	case CHARMOTIONLISTID_FISHING_RIGHT:			pszRet = "�ނ�(�E)";				break;	/* �ނ�(�E) */
	case CHARMOTIONLISTID_FISHING_HIT_UP:			pszRet = "�ނ�q�b�g(��)";			break;	/* �ނ�q�b�g(��) */
	case CHARMOTIONLISTID_FISHING_HIT_DOWN:			pszRet = "�ނ�q�b�g(��)";			break;	/* �ނ�q�b�g(��) */
	case CHARMOTIONLISTID_FISHING_HIT_LEFT:			pszRet = "�ނ�q�b�g(��)";			break;	/* �ނ�q�b�g(��) */
	case CHARMOTIONLISTID_FISHING_HIT_RIGHT:		pszRet = "�ނ�q�b�g(�E)";			break;	/* �ނ�q�b�g(�E) */
	case CHARMOTIONLISTID_BOWBATTLESTAND_UP:		pszRet = "�|�p�퓬����(��)";		break;	/* �|�p�퓬����(��) */
	case CHARMOTIONLISTID_BOWBATTLESTAND_DOWN:		pszRet = "�|�p�퓬����(��)";		break;	/* �|�p�퓬����(��) */
	case CHARMOTIONLISTID_BOWBATTLESTAND_LEFT:		pszRet = "�|�p�퓬����(��)";		break;	/* �|�p�퓬����(��) */
	case CHARMOTIONLISTID_BOWBATTLESTAND_RIGHT:		pszRet = "�|�p�퓬����(�E)";		break;	/* �|�p�퓬����(�E) */
	case CHARMOTIONLISTID_GLOVEBATTLESTAND_UP:		pszRet = "�Ō��p�퓬����(��)";		break;	/* �Ō��p�퓬����(��) */
	case CHARMOTIONLISTID_GLOVEBATTLESTAND_DOWN:	pszRet = "�Ō��p�퓬����(��)";		break;	/* �Ō��p�퓬����(��) */
	case CHARMOTIONLISTID_GLOVEBATTLESTAND_LEFT:	pszRet = "�Ō��p�퓬����(��)";		break;	/* �Ō��p�퓬����(��) */
	case CHARMOTIONLISTID_GLOVEBATTLESTAND_RIGHT:	pszRet = "�Ō��p�퓬����(�E)";		break;	/* �Ō��p�퓬����(�E) */
	case CHARMOTIONLISTID_FISHINGBATTLESTAND_UP:	pszRet = "�ނ�p�퓬����(��)";		break;	/* �ނ�p�퓬����(��) */
	case CHARMOTIONLISTID_FISHINGBATTLESTAND_DOWN:	pszRet = "�ނ�p�퓬����(��)";		break;	/* �ނ�p�퓬����(��) */
	case CHARMOTIONLISTID_FISHINGBATTLESTAND_LEFT:	pszRet = "�ނ�p�퓬����(��)";		break;	/* �ނ�p�퓬����(��) */
	case CHARMOTIONLISTID_FISHINGBATTLESTAND_RIGHT:	pszRet = "�ނ�p�퓬����(�E)";		break;	/* �ނ�p�퓬����(�E) */
	case CHARMOTIONLISTID_STAND:					pszRet = "����";					break;	/* ���� */
	case CHARMOTIONLISTID_STAND_SIMPLE:				pszRet = "����(�㉺���E1�R�}����)";	break;	/* ����(�㉺���E1�R�}����) */
	case CHARMOTIONLISTID_WALK:						pszRet = "����(��������)";			break;	/* ����(��������) */
	case CHARMOTIONLISTID_DESTROY:					pszRet = "�j��";					break;	/* �j�� */
	case CHARMOTIONLISTID_SIMPLE:					pszRet = "16�R�}�A��";				break;	/* 16�R�}�A�� */
	case CHARMOTIONLISTID_DEFENSE_UP:				pszRet = "�h��(��)";				break;	/* �h��(��) */
	case CHARMOTIONLISTID_DEFENSE_DOWN:				pszRet = "�h��(��)";				break;	/* �h��(��) */
	case CHARMOTIONLISTID_DEFENSE_LEFT:				pszRet = "�h��(��)";				break;	/* �h��(��) */
	case CHARMOTIONLISTID_DEFENSE_RIGHT:			pszRet = "�h��(�E)";				break;	/* �h��(�E) */
	case CHARMOTIONLISTID_DEFENSE_SUCCESS_UP:		pszRet = "�h�䐬��(��)";			break;	/* �h�䐬��(��) */
	case CHARMOTIONLISTID_DEFENSE_SUCCESS_DOWN:		pszRet = "�h�䐬��(��)";			break;	/* �h�䐬��(��) */
	case CHARMOTIONLISTID_DEFENSE_SUCCESS_LEFT:		pszRet = "�h�䐬��(��)";			break;	/* �h�䐬��(��) */
	case CHARMOTIONLISTID_DEFENSE_SUCCESS_RIGHT:	pszRet = "�h�䐬��(�E)";			break;	/* �h�䐬��(�E) */
	case CHARMOTIONLISTID_BOWDEFENSE_UP:			pszRet = "�|�p�h��(��)";			break;	/* �|�p�h��(��) */
	case CHARMOTIONLISTID_BOWDEFENSE_DOWN:			pszRet = "�|�p�h��(��)";			break;	/* �|�p�h��(��) */
	case CHARMOTIONLISTID_BOWDEFENSE_LEFT:			pszRet = "�|�p�h��(��)";			break;	/* �|�p�h��(��) */
	case CHARMOTIONLISTID_BOWDEFENSE_RIGHT:			pszRet = "�|�p�h��(�E)";			break;	/* �|�p�h��(�E) */
	case CHARMOTIONLISTID_DAMAGE_UP:				pszRet = "�_���[�W(��)";			break;	/* �_���[�W(��) */
	case CHARMOTIONLISTID_DAMAGE_DOWN:				pszRet = "�_���[�W(��)";			break;	/* �_���[�W(��) */
	case CHARMOTIONLISTID_DAMAGE_LEFT:				pszRet = "�_���[�W(��)";			break;	/* �_���[�W(��) */
	case CHARMOTIONLISTID_DAMAGE_RIGHT:				pszRet = "�_���[�W(�E)";			break;	/* �_���[�W(�E) */
	case CHARMOTIONLISTID_DEAD_UP:					pszRet = "���S(��)";				break;	/* ���S(��) */
	case CHARMOTIONLISTID_DEAD_DOWN:				pszRet = "���S(��)";				break;	/* ���S(��) */
	case CHARMOTIONLISTID_DEAD_LEFT:				pszRet = "���S(��)";				break;	/* ���S(��) */
	case CHARMOTIONLISTID_DEAD_RIGHT:				pszRet = "���S(�E)";				break;	/* ���S(�E) */
	}

	return pszRet;
}


/* ========================================================================= */
/* �֐���	:CMgrData::GetMotionProcName									 */
/* ���e		:�s�������擾													 */
/* ���t		:2008/01/01														 */
/* ========================================================================= */

LPCSTR CMgrData::GetMotionProcName(DWORD dwProcID)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (dwProcID) {
	case CHARMOTIONPROCID_ATACK:		pszRet = "�U��";		break;		/* �U�� */
	case CHARMOTIONPROCID_FISHING:		pszRet = "�ނ�";		break;		/* �ނ� */
	default:							pszRet = "�������Ȃ�";	break;
	}

	return pszRet;
}


/* ========================================================================= */
/* �֐���	:CMgrData::GetMapEventName										 */
/* ���e		:�}�b�v�C�x���g��ʖ����擾										 */
/* ���t		:2008/07/17														 */
/* ========================================================================= */

LPCSTR CMgrData::GetMapEventName(int nMapEventType)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (nMapEventType) {
	case MAPEVENTTYPE_MOVE:			pszRet = "�}�b�v���ړ�";	break;	/* �}�b�v���ړ� */
	case MAPEVENTTYPE_MAPMOVE:		pszRet = "�}�b�v�Ԉړ�";	break;	/* �}�b�v�Ԉړ� */
	case MAPEVENTTYPE_TRASHBOX:		pszRet = "�S�~��";			break;	/* �S�~�� */
	case MAPEVENTTYPE_INITSTATUS:	pszRet = "�X�e�[�^�X������";break;	/* �X�e�[�^�X������ */
	case MAPEVENTTYPE_GRPIDTMP:		pszRet = "�ꎞ�摜�ݒ�";	break;	/* �ꎞ�摜�ݒ� */
	case MAPEVENTTYPE_LIGHT:		pszRet = "����";			break;	/* ���� */
	default:						pszRet = "���ݒ�";			break;
	}

	return pszRet;
}


/* ========================================================================= */
/* �֐���	:CMgrData::GetSystemMsgCount									 */
/* ���e		:�V�X�e�����b�Z�[�W�����擾										 */
/* ���t		:2007/09/06														 */
/* ========================================================================= */

int CMgrData::GetSystemMsgCount(void)
{
	return m_aSystemMsg.GetSize ();
}


/* ========================================================================= */
/* �֐���	:CMgrData::GetSystemMsg											 */
/* ���e		:�V�X�e�����b�Z�[�W��ǉ�										 */
/* ���t		:2007/09/06														 */
/* ========================================================================= */

PSTSYSTEMMSG CMgrData::GetSystemMsg(int nNo)
{
	PSTSYSTEMMSG pRet;

	pRet = NULL;

	if (nNo >= m_aSystemMsg.GetSize ()) {
		goto Exit;
	}

	pRet = m_aSystemMsg[nNo];
Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CMgrData::AddSystemMsg											 */
/* ���e		:�V�X�e�����b�Z�[�W��ǉ�										 */
/* ���t		:2007/09/06														 */
/* ========================================================================= */

void CMgrData::AddSystemMsg(BOOL bAddLog, LPCSTR pszMsg, COLORREF clMsg)
{
	PSTSYSTEMMSG pSystemMsg;

	pSystemMsg = new STSYSTEMMSG;
	pSystemMsg->bAddLog	= bAddLog;
	pSystemMsg->strMsg	= pszMsg;
	pSystemMsg->clMsg	= clMsg;

	m_aSystemMsg.Add (pSystemMsg);
}


/* ========================================================================= */
/* �֐���	:CMgrData::DeleteAllSystemMsg									 */
/* ���e		:�V�X�e�����b�Z�[�W��S�č폜									 */
/* ���t		:2007/09/06														 */
/* ========================================================================= */

void CMgrData::DeleteAllSystemMsg(void)
{
	int i, nCount;
	PSTSYSTEMMSG pSystemMsg;

	nCount = m_aSystemMsg.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pSystemMsg = m_aSystemMsg[i];
		SAFE_DELETE (pSystemMsg);
	}
	m_aSystemMsg.RemoveAll ();
}


/* ========================================================================= */
/* �֐���	:CMgrData::SetSEVolume											 */
/* ���e		:���ʉ��ʂ�ݒ�													 */
/* ���t		:2007/09/16														 */
/* ========================================================================= */

void CMgrData::SetSEVolume(int nSEVolume)
{
	m_nSEVolume = nSEVolume;
	SaveIniData ();
}


/* ========================================================================= */
/* �֐���	:CMgrData::SetSEVolume											 */
/* ���e		:BGM���ʂ�ݒ�													 */
/* ���t		:2007/09/16														 */
/* ========================================================================= */

void CMgrData::SetBGMVolume(int nBGMVolume)
{
	m_nBGMVolume = nBGMVolume;
	SaveIniData ();
}


/* ========================================================================= */
/* �֐���	:CMgrData::SetInfoTalkEvent										 */
/* ���e		:��b�C�x���g���̐ݒ�											 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

void CMgrData::SetInfoTalkEvent(CInfoTalkEvent *pSrc)
{
	m_pInfoTalkEvent->Copy (pSrc);
}


/* ========================================================================= */
/* �֐���	:CMgrData::GetInfoTalkEvent										 */
/* ���e		:��b�C�x���g���̎擾											 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

CInfoTalkEvent *CMgrData::GetInfoTalkEvent(void)
{
	return m_pInfoTalkEvent;
}


/* ========================================================================= */
/* �֐���	:CMgrData::ReadIniData											 */
/* ���e		:ini�t�@�C������ݒ��ǂݍ���									 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrData::ReadIniData(void)
{
	int nTmp;
	char szFileName[MAX_PATH], szTmp[128], szTmp2[128];
	CCryptUtil CryptUtil;
	_bstr_t bstrTmp;

	ZeroMemory (szFileName, sizeof (szFileName));
	GetModuleFileName (NULL, szFileName, MAX_PATH);
	strcpy (szFileName + strlen (szFileName) - 3, "ini");

	/* �T�[�o�[�A�h���X */
	GetPrivateProfileString ("Setting", "ServerAddr", "127.0.0.1", szTmp, sizeof (szTmp) - 1, szFileName);
	m_strServerAddr = szTmp;
	/* �҂������|�[�g */
	m_wServerPort = GetPrivateProfileInt ("Setting", "ServerPort", 2006, szFileName);
	/* �p�X���[�h��ۑ�����H */
	m_bSavePassword = (BOOL)GetPrivateProfileInt ("Setting", "SavePassword", 0, szFileName);
	/* �������Ƀ^�X�N�o�[�`�J�`�J */
	m_bOptionTaskbar = (BOOL)GetPrivateProfileInt ("Setting", "OptionTaskbar", 0, szFileName);
	/* ������\������ */
	m_bOptionViewChat = (BOOL)GetPrivateProfileInt ("Setting", "OptionViewChat", 1, szFileName);
	/* �A�C�e����\������ */
	m_bOptionViewItem = (BOOL)GetPrivateProfileInt ("Setting", "OptionViewItem", 1, szFileName);
	/* �A�C�e������\������ */
	m_bOptionViewItemName = (BOOL)GetPrivateProfileInt ("Setting", "OptionViewItemName", 1, szFileName);
	/* �w���v�A�C�R����\������ */
	m_bOptionViewHelpIcon = (BOOL)GetPrivateProfileInt ("Setting", "OptionViewHelpIcon", 1, szFileName);
	/* �퓬���b�Z�[�W�����O�Ɏc�� */
	m_bOptionBattleMsgLog = (BOOL)GetPrivateProfileInt ("Setting", "OptionBattleMsgLog", 0, szFileName);
	/* 60�t���[���ŕ\������ */
	m_bOption60Frame = (BOOL)GetPrivateProfileInt ("Setting", "Option60Frame", 0, szFileName);
	/* �ŏI�A�J�E���g�� */
	GetPrivateProfileString ("Account", "Account", "", szTmp, sizeof (szTmp) - 1, szFileName);
	m_strLastAccount = szTmp;
	/* �ŏI�p�X���[�h */
GetPrivateProfileString ("Account", "Password", "", szTmp, sizeof (szTmp) - 1, szFileName);
	CryptUtil.UnCryptStr (szTmp, szTmp2, 10);
	m_strLastPassword = szTmp2;
	/* ���ʉ��� */
	nTmp = GetPrivateProfileInt ("Setting", "SEVolume", 2, szFileName);
	m_nSEVolume = min (nTmp, 4);
	/* BGM���� */
	nTmp = GetPrivateProfileInt ("Setting", "BGMVolume", 2, szFileName);
	m_nBGMVolume = min (nTmp, 4);
	/* �`�惂�[�h */
	nTmp = GetPrivateProfileInt ("Setting", "DrawMode", 1, szFileName);
	m_nDrawMode = min (nTmp, 4);
	/* ���̓W���C�p�b�h */
	GetPrivateProfileString ("Setting", "InputDevice", "", szTmp, sizeof (szTmp) - 1, szFileName);
	bstrTmp = szTmp;
	CLSIDFromString (bstrTmp, &m_stInputGuid); 
	/* ���Ђ�˃^�C�}�[ */
	nTmp = GetPrivateProfileInt ("Setting", "SleepTimer", 0, szFileName);
	m_nSleepTimer = min (nTmp, 5);
}

/* Copyright(C)URARA-works 2006 */
