/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrData.cpp												 */
/* ���e			:�f�[�^�}�l�[�W���N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/04													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoAccount.h"
#include "LibInfoMapBase.h"
#include "LibInfoMapObject.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapShadow.h"
#include "LibInfoItemType.h"
#include "LibInfoItem.h"
#include "LibInfoItemWeapon.h"
#include "LibInfoEffect.h"
#include "LibInfoMotion.h"
#include "LibInfoMotionType.h"
#include "LibInfoEfcBalloon.h"
#include "LibInfoSystem.h"
#include "InfoFileList.h"
#include "TextOutput.h"
#include "TextInput.h"
#include "ParamUtil.h"
#include "UraraSockTCPSBO.h"
#include "SaveLoad.h"
#include "LibInfoCharSVr.h"
#include "MgrData.h"

/* ========================================================================= */
/* �֐���	:CMgrData::CMgrData												 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/04/25														 */
/* ========================================================================= */

CMgrData::CMgrData()
{
	m_wPort				= 2005;
	m_byOnline			= 0;
	m_byLastSendClock	= -1;
	m_pMainFrame		= NULL;
	m_pSock				= NULL;

	m_pLog					= new CTextOutput;
	m_pLibInfoAccount		= new CLibInfoAccount;
	m_pLibInfoChar			= new CLibInfoCharSvr;
	m_pLibInfoMap			= new CLibInfoMapBase;
	m_pLibInfoMapObject		= new CLibInfoMapObject;
	m_pLibInfoMapParts		= new CLibInfoMapParts;
	m_pLibInfoMapShadow		= new CLibInfoMapShadow;
	m_pLibInfoItemType		= new CLibInfoItemType;
	m_pLibInfoItem			= new CLibInfoItem;
	m_pLibInfoItemWeapon	= new CLibInfoItemWeapon;
	m_pLibInfoEffect		= new CLibInfoEffect;
	m_pLibInfoMotion		= new CLibInfoMotion;
	m_pLibInfoMotionType	= new CLibInfoMotionType;
	m_pLibInfoEfcBalloon	= new CLibInfoEfcBalloon;
	m_pLibInfoSystem		= new CLibInfoSystem;
	m_pInfoFileList			= new CInfoFileList;
}


/* ========================================================================= */
/* �֐���	:CMgrData::~CMgrData											 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2005/04/25														 */
/* ========================================================================= */

CMgrData::~CMgrData()
{
	SAFE_DELETE (m_pLog);
	SAFE_DELETE (m_pLibInfoAccount);
	SAFE_DELETE (m_pLibInfoChar);
	SAFE_DELETE (m_pLibInfoMap);
	SAFE_DELETE (m_pLibInfoMapObject);
	SAFE_DELETE (m_pLibInfoMapParts);
	SAFE_DELETE (m_pLibInfoMapShadow);
	SAFE_DELETE (m_pLibInfoItemType);
	SAFE_DELETE (m_pLibInfoItem);
	SAFE_DELETE (m_pLibInfoItemWeapon);
	SAFE_DELETE (m_pLibInfoEffect);
	SAFE_DELETE (m_pLibInfoMotion);
	SAFE_DELETE (m_pLibInfoMotionType);
	SAFE_DELETE (m_pLibInfoEfcBalloon);
	SAFE_DELETE (m_pLibInfoSystem);
	SAFE_DELETE (m_pInfoFileList);
}


/* ========================================================================= */
/* �֐���	:CMgrData::Create												 */
/* ���e		:�쐬															 */
/* ���t		:2005/06/09														 */
/* ========================================================================= */

void CMgrData::Create(
	CMainFrame			*pMainFrame,	/* [in] ���C���t���[�� */
	CUraraSockTCPSBO	*pSock)			/* [in] �ʐM�}�l�[�W�� */
{
	char szName[MAX_PATH], *pszTmp;
	CmyString strTmp;

	ReadIniData ();
	ReadHashList ();

	m_pMainFrame	= pMainFrame;
	m_pSock			= pSock;

	m_pLibInfoAccount->		Create ();
	m_pLibInfoChar->		Create (this);
	m_pLibInfoMapObject->	Create ();
	m_pLibInfoMapParts->	Create ();
	m_pLibInfoMapShadow->	Create ();
	m_pLibInfoMap->			Create (m_pLibInfoMapParts);
	m_pLibInfoItemType->	Create ();
	m_pLibInfoItem->		Create ();
	m_pLibInfoItemWeapon->	Create ();
	m_pLibInfoEffect->		Create ();
	m_pLibInfoMotion->		Create ();
	m_pLibInfoMotionType->	Create ();
	m_pLibInfoEfcBalloon->	Create ();
	m_pLibInfoSystem->		Create ();

	m_pLibInfoItem->SetTypeInfo (m_pLibInfoItemType);
	m_pLibInfoItem->SetWeaponInfo (m_pLibInfoItemWeapon);

	GetModuleFileName (NULL, szName, MAX_PATH);
	pszTmp		= strrchr (szName, '\\');
	pszTmp[1]	= 0;

	/* ���O�t�@�C���̍쐬 */
	strTmp.Format ("%sSboSvrLog.txt", szName);
	m_pLog->Create (strTmp, TRUE, TRUE);
}


/* ========================================================================= */
/* �֐���	:CMgrData::Destroy												 */
/* ���e		:�j��															 */
/* ���t		:2005/06/09														 */
/* ========================================================================= */

void CMgrData::Destroy(void)
{
	m_pLibInfoAccount->		Destroy ();
	m_pLibInfoChar->		Destroy ();
	m_pLibInfoMap->			Destroy ();
	m_pLibInfoMapObject->	Destroy ();
	m_pLibInfoMapParts->	Destroy ();
	m_pLibInfoMapShadow->	Destroy ();
	m_pLibInfoItemType->	Destroy ();
	m_pLibInfoItem->		Destroy ();
	m_pLibInfoItemWeapon->	Destroy ();
	m_pLibInfoEffect->		Destroy ();
	m_pLibInfoMotion->		Destroy ();
	m_pLibInfoMotionType->	Destroy ();
	m_pLibInfoEfcBalloon->	Destroy ();
	m_pLibInfoSystem->		Destroy ();
}


/* ========================================================================= */
/* �֐���	:CMgrData::Save													 */
/* ���e		:�f�[�^�̕ۑ�													 */
/* ���t		:2005/05/01														 */
/* ========================================================================= */

void CMgrData::Save(void)
{
	CSaveLoadInfoAccount SaveLoadInfoAccount;
	CSaveLoadInfoChar SaveLoadInfoChar;
	CSaveLoadInfoMap SaveLoadInfoMap;
	CSaveLoadInfoMapObject SaveLoadInfoMapObject;
	CSaveLoadInfoMapParts SaveLoadInfoMapParts;
	CSaveLoadInfoMapShadow SaveLoadInfoMapShadow;
	CSaveLoadInfoEffect SaveLoadInfoEffect;
	CSaveLoadInfoItemType SaveLoadInfoItemType;
	CSaveLoadInfoItem SaveLoadInfoItem;
	CSaveLoadInfoItemWeapon SaveLoadInfoItemWeapon;
	CSaveLoadInfoMotion SaveLoadInfoMotion;
	CSaveLoadInfoMotionType SaveLoadInfoMotionType;
	CSaveLoadInfoEfcBalloon SaveLoadInfoEfcBalloon;
	CSaveLoadInfoSystem SaveLoadInfoSystem;

	SaveLoadInfoAccount.	Save ((PCLibInfoBase)m_pLibInfoAccount);
	SaveLoadInfoChar.		Save ((PCLibInfoBase)m_pLibInfoChar);
	SaveLoadInfoMap.		Save ((PCLibInfoBase)m_pLibInfoMap);
	SaveLoadInfoMapObject.	Save ((PCLibInfoBase)m_pLibInfoMapObject);
	SaveLoadInfoMapParts.	Save ((PCLibInfoBase)m_pLibInfoMapParts);
	SaveLoadInfoMapShadow.	Save ((PCLibInfoBase)m_pLibInfoMapShadow);
	SaveLoadInfoEffect.		Save ((PCLibInfoBase)m_pLibInfoEffect);
	SaveLoadInfoItemType.	Save ((PCLibInfoBase)m_pLibInfoItemType);
	SaveLoadInfoItem.		Save ((PCLibInfoBase)m_pLibInfoItem);
	SaveLoadInfoItemWeapon.	Save ((PCLibInfoBase)m_pLibInfoItemWeapon);
	SaveLoadInfoMotion.		Save ((PCLibInfoBase)m_pLibInfoMotion);
	SaveLoadInfoMotionType.	Save ((PCLibInfoBase)m_pLibInfoMotionType);
	SaveLoadInfoEfcBalloon.	Save ((PCLibInfoBase)m_pLibInfoEfcBalloon);
	SaveLoadInfoSystem.		Save ((PCLibInfoBase)m_pLibInfoSystem);
}


/* ========================================================================= */
/* �֐���	:CMgrData::Load													 */
/* ���e		:�f�[�^�̓ǂݍ���												 */
/* ���t		:2005/05/01														 */
/* ========================================================================= */

void CMgrData::Load(void)
{
	CSaveLoadInfoAccount SaveLoadInfoAccount;
	CSaveLoadInfoChar SaveLoadInfoChar;
	CSaveLoadInfoMap SaveLoadInfoMap;
	CSaveLoadInfoMapObject SaveLoadInfoMapObject;
	CSaveLoadInfoMapParts SaveLoadInfoMapParts;
	CSaveLoadInfoMapShadow SaveLoadInfoMapShadow;
	CSaveLoadInfoEffect SaveLoadInfoEffect;
	CSaveLoadInfoItemType SaveLoadInfoItemType;
	CSaveLoadInfoItem SaveLoadInfoItem;
	CSaveLoadInfoItemWeapon SaveLoadInfoItemWeapon;
	CSaveLoadInfoMotion SaveLoadInfoMotion;
	CSaveLoadInfoMotionType SaveLoadInfoMotionType;
	CSaveLoadInfoEfcBalloon SaveLoadInfoEfcBalloon;
	CSaveLoadInfoSystem SaveLoadInfoSystem;

	m_pLibInfoAccount->		DeleteAll ();
	m_pLibInfoChar->		DeleteAll ();
	m_pLibInfoMap->			DeleteAll ();
	m_pLibInfoMapObject->	DeleteAll ();
	m_pLibInfoMapParts->	DeleteAll ();
	m_pLibInfoMapShadow->	DeleteAll ();
	m_pLibInfoItemType->	DeleteAll ();
	m_pLibInfoItem->		DeleteAll ();
	m_pLibInfoItemWeapon->	DeleteAll ();
	m_pLibInfoMotion->		DeleteAll ();
	m_pLibInfoMotionType->	DeleteAll ();
	m_pLibInfoEfcBalloon->	DeleteAll ();

	SaveLoadInfoAccount.	Load ((PCLibInfoBase)m_pLibInfoAccount);
	SaveLoadInfoChar.		Load ((PCLibInfoBase)m_pLibInfoChar);
	SaveLoadInfoMap.		Load ((PCLibInfoBase)m_pLibInfoMap);
	SaveLoadInfoMapObject.	Load ((PCLibInfoBase)m_pLibInfoMapObject);
	SaveLoadInfoMapParts.	Load ((PCLibInfoBase)m_pLibInfoMapParts);
	SaveLoadInfoMapShadow.	Load ((PCLibInfoBase)m_pLibInfoMapShadow);
	SaveLoadInfoEffect.		Load ((PCLibInfoBase)m_pLibInfoEffect);
	SaveLoadInfoItemType.	Load ((PCLibInfoBase)m_pLibInfoItemType);
	SaveLoadInfoItem.		Load ((PCLibInfoBase)m_pLibInfoItem);
	SaveLoadInfoItemWeapon.	Load ((PCLibInfoBase)m_pLibInfoItemWeapon);
	SaveLoadInfoMotion.		Load ((PCLibInfoBase)m_pLibInfoMotion);
	SaveLoadInfoMotionType.	Load ((PCLibInfoBase)m_pLibInfoMotionType);
	SaveLoadInfoEfcBalloon.	Load ((PCLibInfoBase)m_pLibInfoEfcBalloon);
	SaveLoadInfoSystem.		Load ((PCLibInfoBase)m_pLibInfoSystem);

	if (m_pLibInfoMap->GetCount () <= 0) {
		PCInfoMapBase pMapTmp;

		pMapTmp = (PCInfoMapBase)m_pLibInfoMap->GetNew ();
		pMapTmp->Init (20, 20, 0);
		m_pLibInfoMap->Add (pMapTmp);
	}

	/* �ǂݍ��݌�̃f�[�^�␳ */
	m_pLibInfoChar->		Revice ();
	m_pLibInfoItemType->	Revice ();
	m_pLibInfoMap->			RenewMapEvent ();
	m_pLibInfoMotion->		Revice ();
}


/* ========================================================================= */
/* �֐���	:CMgrData::ReadHashList											 */
/* ���e		:�t�@�C�����X�g��ǂݍ���										 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

void CMgrData::ReadHashList(void)
{
	int i, nCount;
	char szFileName[MAX_PATH];
	DWORD dwTmp;
	CTextInput TextInput;
	CParamUtil ParamUtil;
	CmyString strTmp;

	m_pInfoFileList->DeleteAll ();

	GetModuleFilePath (szFileName, MAX_PATH);
	strcat (szFileName, "Update\\SBOHashList.txt");

	TextInput.Create (szFileName);

	nCount = TextInput.GetLineCount ();
	for (i = 0; i < nCount; i ++) {
		strTmp = TextInput.ReadLine ();
		ParamUtil.Set (strTmp, ',');
		dwTmp = atoi (ParamUtil.GetParam (1));
		m_pInfoFileList->Add (ParamUtil.GetParam (0), ParamUtil.GetParam (2), dwTmp);
	}
}


/* ========================================================================= */
/* �֐���	:CMgrData::SetClientVersion										 */
/* ���e		:�N���C�A���g�o�[�W������ݒ�									 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

void CMgrData::SetClientVersion(LPCSTR pszVersion)
{
	char szFileName[MAX_PATH], szTmp[128];

	ZeroMemory (szFileName, sizeof (szFileName));
	ZeroMemory (szTmp, sizeof (szTmp));

	GetModuleFileName (NULL, szFileName, MAX_PATH);
	strcpy (szFileName + strlen (szFileName) - 3, "ini");

	m_strClientVersion = pszVersion;
	WritePrivateProfileString ("Info", "ClientVersion", m_strClientVersion, szFileName);
}


/* ========================================================================= */
/* �֐���	:CMgrData::ReadIniData											 */
/* ���e		:ini�t�@�C������ݒ��ǂݍ���									 */
/* ���t		:2005/06/09														 */
/* ========================================================================= */

void CMgrData::ReadIniData(void)
{
	char szFileName[MAX_PATH], szTmp[128];

	ZeroMemory (szFileName, sizeof (szFileName));
	ZeroMemory (szTmp, sizeof (szTmp));

	GetModuleFileName (NULL, szFileName, MAX_PATH);
	strcpy (szFileName + strlen (szFileName) - 3, "ini");

	/* �҂������|�[�g */
	m_wPort = GetPrivateProfileInt ("Setting", "Port", 2006, szFileName);
	/* �Ǘ��Ҍ����A�J�E���g */
	GetPrivateProfileString ("Setting", "AdminAccount", "Admin", szTmp, sizeof (szTmp) - 1, szFileName);
	m_strAdminAccount = szTmp;

	/* �N���C�A���g�o�[�W���� */
	GetPrivateProfileString ("Info", "ClientVersion", "", szTmp, sizeof (szTmp) - 1, szFileName);
	m_strClientVersion = szTmp;

	/* FTP�A�J�E���g */
	GetPrivateProfileString ("FTP", "Account", "", szTmp, sizeof (szTmp) - 1, szFileName);
	m_strFtpAccount = szTmp;
	/* FTP�p�X���[�h */
	GetPrivateProfileString ("FTP", "Password", "", szTmp, sizeof (szTmp) - 1, szFileName);
	m_strFtpPassword = szTmp;
	/* �T�[�o�[�A�h���X */
	GetPrivateProfileString ("FTP", "ServerAddr", "", szTmp, sizeof (szTmp) - 1, szFileName);
	m_strFtpServerAddr = szTmp;
	/* �A�b�v���[�h�� */
	GetPrivateProfileString ("FTP", "UploadPath", "", szTmp, sizeof (szTmp) - 1, szFileName);
	m_strFtpUploadPath = szTmp;
}

/* Copyright(C)URARA-works 2006 */
