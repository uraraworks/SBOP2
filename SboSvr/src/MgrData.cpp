/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MgrData.cpp												 */
/* 内容			:データマネージャクラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/04													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoAccount.h"
#include "LibInfoMapBase.h"
#include "LibInfoMapObject.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapShadow.h"
#include "LibInfoDisable.h"
#include "LibInfoItemType.h"
#include "LibInfoItem.h"
#include "LibInfoItemWeapon.h"
#include "LibInfoEffect.h"
#include "LibInfoMotion.h"
#include "LibInfoMotionType.h"
#include "LibInfoEfcBalloon.h"
#include "LibInfoSystem.h"
#include "LibInfoSkill.h"
#include "LibInfoTalkEvent.h"
#include "InfoFileList.h"
#include "TextOutput.h"
#include "TextInput.h"
#include "ParamUtil.h"
#include "UraraSockTCPSBO.h"
#include "SaveLoad.h"
#include "LibInfoCharSVr.h"
#include "MgrData.h"

/* ========================================================================= */
/* 関数名	:CMgrData::CMgrData												 */
/* 内容		:コンストラクタ													 */
/* 日付		:2005/04/25														 */
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
	m_pLibInfoDisable		= new CLibInfoDisable;
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
	m_pLibInfoSkill			= new CLibInfoSkill;
	m_pLibInfoTalkEvent		= new CLibInfoTalkEvent;
	m_pInfoFileList			= new CInfoFileList;
}


/* ========================================================================= */
/* 関数名	:CMgrData::~CMgrData											 */
/* 内容		:デストラクタ													 */
/* 日付		:2005/04/25														 */
/* ========================================================================= */

CMgrData::~CMgrData()
{
	SAFE_DELETE (m_pLog);
	SAFE_DELETE (m_pLibInfoAccount);
	SAFE_DELETE (m_pLibInfoChar);
	SAFE_DELETE (m_pLibInfoDisable);
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
	SAFE_DELETE (m_pLibInfoSkill);
	SAFE_DELETE (m_pLibInfoTalkEvent);
	SAFE_DELETE (m_pInfoFileList);
}


/* ========================================================================= */
/* 関数名	:CMgrData::Create												 */
/* 内容		:作成															 */
/* 日付		:2005/06/09														 */
/* ========================================================================= */

void CMgrData::Create(
	CMainFrame			*pMainFrame,	/* [in] メインフレーム */
	CUraraSockTCPSBO	*pSock)			/* [in] 通信マネージャ */
{
	char szName[MAX_PATH], *pszTmp;
	CmyString strTmp;

	ReadIniData ();
	ReadHashList ();

	m_pMainFrame	= pMainFrame;
	m_pSock			= pSock;

	m_pLibInfoAccount->		Create ();
	m_pLibInfoChar->		Create (this);
	m_pLibInfoDisable->		Create ();
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
	m_pLibInfoSkill->		Create ();
	m_pLibInfoTalkEvent->	Create ();

	m_pLibInfoItem->SetTypeInfo (m_pLibInfoItemType);
	m_pLibInfoItem->SetWeaponInfo (m_pLibInfoItemWeapon);

	GetModuleFileName (NULL, szName, MAX_PATH);
	pszTmp		= strrchr (szName, '\\');
	pszTmp[1]	= 0;

	/* ログファイルの作成 */
    CString strBasePath = Utf8ToTString (szName);
    strTmp.Format(_T("%sSboSvrLog.txt"), (LPCTSTR)strBasePath);
	m_pLog->Create (strTmp, TRUE, TRUE);
}


/* ========================================================================= */
/* 関数名	:CMgrData::Destroy												 */
/* 内容		:破棄															 */
/* 日付		:2005/06/09														 */
/* ========================================================================= */

void CMgrData::Destroy(void)
{
	m_pLibInfoAccount->		Destroy ();
	m_pLibInfoChar->		Destroy ();
	m_pLibInfoDisable->		Destroy ();
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
	m_pLibInfoSkill->		Destroy ();
	m_pLibInfoTalkEvent->	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CMgrData::Save													 */
/* 内容		:データの保存													 */
/* 日付		:2005/05/01														 */
/* ========================================================================= */

void CMgrData::Save(void)
{
	CSaveLoadInfoAccount SaveLoadInfoAccount;
	CSaveLoadInfoChar SaveLoadInfoChar;
	CSaveLoadInfoDisable SaveLoadInfoDisable;
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
	CSaveLoadInfoSkill SaveLoadInfoSkill;
	CSaveLoadInfoTalkEvent SaveLoadInfoTalkEvent;

	SaveLoadInfoAccount.	Save ((PCLibInfoBase)m_pLibInfoAccount);
	SaveLoadInfoChar.		Save ((PCLibInfoBase)m_pLibInfoChar);
	SaveLoadInfoDisable.	Save ((PCLibInfoBase)m_pLibInfoDisable);
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
	SaveLoadInfoSkill.		Save ((PCLibInfoBase)m_pLibInfoSkill);
	SaveLoadInfoTalkEvent.	Save ((PCLibInfoBase)m_pLibInfoTalkEvent);
}


/* ========================================================================= */
/* 関数名	:CMgrData::Load													 */
/* 内容		:データの読み込み												 */
/* 日付		:2005/05/01														 */
/* ========================================================================= */

void CMgrData::Load(void)
{
	CSaveLoadInfoAccount SaveLoadInfoAccount;
	CSaveLoadInfoChar SaveLoadInfoChar;
	CSaveLoadInfoDisable SaveLoadInfoDisable;
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
	CSaveLoadInfoSkill SaveLoadInfoSkill;
	CSaveLoadInfoTalkEvent SaveLoadInfoTalkEvent;

	m_pLibInfoAccount->		DeleteAll ();
	m_pLibInfoChar->		DeleteAll ();
	m_pLibInfoDisable->		DeleteAll ();
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
	m_pLibInfoSkill->		DeleteAll ();
	m_pLibInfoTalkEvent->	DeleteAll ();

	SaveLoadInfoAccount.	Load ((PCLibInfoBase)m_pLibInfoAccount);
	SaveLoadInfoChar.		Load ((PCLibInfoBase)m_pLibInfoChar);
	SaveLoadInfoDisable.	Load ((PCLibInfoBase)m_pLibInfoDisable);
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
	SaveLoadInfoSkill.		Load ((PCLibInfoBase)m_pLibInfoSkill);
	SaveLoadInfoTalkEvent.	Load ((PCLibInfoBase)m_pLibInfoTalkEvent);
	if (m_pLibInfoMap->GetCount () <= 0) {
		PCInfoMapBase pMapTmp;

		pMapTmp = (PCInfoMapBase)m_pLibInfoMap->GetNew ();
		pMapTmp->Init (20, 20, 0);
		m_pLibInfoMap->Add (pMapTmp);
	}

	/* 読み込み後のデータ補正 */
	m_pLibInfoChar->		Revice ();
	m_pLibInfoItemType->	Revice ();
	m_pLibInfoMap->			RenewMapEvent ();
	m_pLibInfoMap->			SetMapObject (m_pLibInfoMapObject);
	m_pLibInfoMap->			RenewHitTmp ();
	m_pLibInfoMotion->		Revice ();
}


/* ========================================================================= */
/* 関数名	:CMgrData::ReadHashList											 */
/* 内容		:ファイルリストを読み込み										 */
/* 日付		:2008/03/08														 */
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
/* 関数名	:CMgrData::SetClientVersion										 */
/* 内容		:クライアントバージョンを設定									 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CMgrData::SetClientVersion(LPCSTR pszVersion)
{
        TCHAR szFileName[MAX_PATH];

        ZeroMemory (szFileName, sizeof (szFileName));

        GetModuleFileName (NULL, szFileName, _countof (szFileName));
        size_t nLen = _tcslen (szFileName);
        if (nLen >= 3) {
                _tcscpy_s (szFileName + nLen - 3, _countof (szFileName) - (nLen - 3), _T("ini"));
        } else {
                _tcscat_s (szFileName, _T(".ini"));
        }

        m_strClientVersion = pszVersion;
        WritePrivateProfileString (_T("Info"), _T("ClientVersion"), m_strClientVersion, szFileName);
}


/* ========================================================================= */
/* 関数名	:CMgrData::ReadIniData											 */
/* 内容		:iniファイルから設定を読み込み									 */
/* 日付		:2005/06/09														 */
/* ========================================================================= */

void CMgrData::ReadIniData(void)
{
        TCHAR szFileName[MAX_PATH];
        TCHAR szTmp[128];

        ZeroMemory (szFileName, sizeof (szFileName));
        ZeroMemory (szTmp, sizeof (szTmp));

        GetModuleFileName (NULL, szFileName, _countof (szFileName));
        size_t nLen = _tcslen (szFileName);
        if (nLen >= 3) {
                _tcscpy_s (szFileName + nLen - 3, _countof (szFileName) - (nLen - 3), _T("ini"));
        } else {
                _tcscat_s (szFileName, _T(".ini"));
        }

        /* 待ちうけポート */
        m_wPort = static_cast<WORD>(GetPrivateProfileInt (_T("Setting"), _T("Port"), 2006, szFileName));
        /* 管理者権限アカウント */
        GetPrivateProfileString (_T("Setting"), _T("AdminAccount"), _T("Admin"), szTmp, _countof (szTmp), szFileName);
        m_strAdminAccount = szTmp;

        /* クライアントバージョン */
        GetPrivateProfileString (_T("Info"), _T("ClientVersion"), _T(""), szTmp, _countof (szTmp), szFileName);
        m_strClientVersion = szTmp;

        /* FTPアカウント */
        GetPrivateProfileString (_T("FTP"), _T("Account"), _T(""), szTmp, _countof (szTmp), szFileName);
        m_strFtpAccount = szTmp;
        /* FTPパスワード */
        GetPrivateProfileString (_T("FTP"), _T("Password"), _T(""), szTmp, _countof (szTmp), szFileName);
        m_strFtpPassword = szTmp;
        /* サーバーアドレス */
        GetPrivateProfileString (_T("FTP"), _T("ServerAddr"), _T(""), szTmp, _countof (szTmp), szFileName);
        m_strFtpServerAddr = szTmp;
        /* アップロード先 */
        GetPrivateProfileString (_T("FTP"), _T("UploadPath"), _T(""), szTmp, _countof (szTmp), szFileName);
        m_strFtpUploadPath = szTmp;
}

/* Copyright(C)URARA-works 2006 */
