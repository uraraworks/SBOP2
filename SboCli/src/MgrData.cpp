/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MgrData.h													 */
/* 内容			:データマネージャクラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
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
/* 関数名	:CMgrData::CMgrData												 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/09/24														 */
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
/* 関数名	:CMgrData::~CMgrData											 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/09/24														 */
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
/* 関数名	:CMgrData::Create												 */
/* 内容		:作成															 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CMgrData::Create(
	CMainFrame		*pMainFrame,		/* [in] メインフレーム */
	CMgrGrpData		*pMgrGrpData)		/* [in] グラフィックデータ管理 */
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
/* 関数名	:CMgrData::Destroy												 */
/* 内容		:破棄															 */
/* 日付		:2006/09/24														 */
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
/* 関数名	:CMgrData::SaveIniData											 */
/* 内容		:設定情報を保存													 */
/* 日付		:2007/07/05														 */
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

	/* 暗号化 */
	CryptUtil.CryptStr (m_strLastPassword, szTmp, 10);
	/* 最終アカウント名 */
	WritePrivateProfileString ("Account", "Account", m_strLastAccount, szFileName);
	/* 最終パスワード */
	WritePrivateProfileString ("Account", "Password", szTmp, szFileName);
	/* パスワードを保存する？ */
	strTmp.Format ("%d", m_bSavePassword);
	WritePrivateProfileString ("Setting", "SavePassword", strTmp, szFileName);
	/* 発言時にタスクバーチカチカ */
	strTmp.Format ("%d", m_bOptionTaskbar);
	WritePrivateProfileString ("Setting", "OptionTaskbar", strTmp, szFileName);
	/* 発言を表示する */
	strTmp.Format ("%d", m_bOptionViewChat);
	WritePrivateProfileString ("Setting", "OptionViewChat", strTmp, szFileName);
	/* アイテムを表示する */
	strTmp.Format ("%d", m_bOptionViewItem);
	WritePrivateProfileString ("Setting", "OptionViewItem", strTmp, szFileName);
	/* アイテム名を表示する */
	strTmp.Format ("%d", m_bOptionViewItemName);
	WritePrivateProfileString ("Setting", "OptionViewItemName", strTmp, szFileName);
	/* ヘルプアイコンを表示する */
	strTmp.Format ("%d", m_bOptionViewHelpIcon);
	WritePrivateProfileString ("Setting", "OptionViewHelpIcon", strTmp, szFileName);
	/* 戦闘メッセージをログに残す */
	strTmp.Format ("%d", m_bOptionBattleMsgLog);
	WritePrivateProfileString ("Setting", "OptionBattleMsgLog", strTmp, szFileName);
	/* 60フレームで表示する */
	strTmp.Format ("%d", m_bOption60Frame);
	WritePrivateProfileString ("Setting", "Option60Frame", strTmp, szFileName);
	/* 効果音量 */
	strTmp.Format ("%d", m_nSEVolume);
	WritePrivateProfileString ("Setting", "SEVolume", strTmp, szFileName);
	/* BGM音量 */
	strTmp.Format ("%d", m_nBGMVolume);
	WritePrivateProfileString ("Setting", "BGMVolume", strTmp, szFileName);
	/* 描画モード */
	strTmp.Format ("%d", m_nDrawMode);
	WritePrivateProfileString ("Setting", "DrawMode", strTmp, szFileName);
	/* 入力ジョイパッド */
	StringFromGUID2 (m_stInputGuid, szwGuid, sizeof (szwGuid));
	bstrTmp = szwGuid;
	strcpy (szTmp, bstrTmp);
	WritePrivateProfileString ("Setting", "InputDevice", szTmp, szFileName);
	/* おひるねタイマー */
	strTmp.Format ("%d", m_nSleepTimer);
	WritePrivateProfileString ("Setting", "SleepTimer", strTmp, szFileName);
}


/* ========================================================================= */
/* 関数名	:CMgrData::SetWindowInfo										 */
/* 内容		:ウィンドウ情報を設定											 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CMgrData::SetWindowInfo(HINSTANCE hInstance, HWND hWndMain)
{
	m_hInstance	= hInstance;
	m_hWndMain	= hWndMain;
}


/* ========================================================================= */
/* 関数名	:CMgrData::SetAccount											 */
/* 内容		:使用中のアカウント情報を設定									 */
/* 日付		:2006/11/08														 */
/* ========================================================================= */

void CMgrData::SetAccount(CInfoAccount *pInfoAccount)
{
	m_pInfoAccount->Copy (pInfoAccount);
}


/* ========================================================================= */
/* 関数名	:CMgrData::GetAccountID											 */
/* 内容		:使用中のアカウントIDを取得										 */
/* 日付		:2006/12/02														 */
/* ========================================================================= */

DWORD CMgrData::GetAccountID(void)
{
	return m_pInfoAccount->m_dwAccountID;
}


/* ========================================================================= */
/* 関数名	:CMgrData::SetCharID											 */
/* 内容		:使用中のキャラIDを設定											 */
/* 日付		:2007/03/21														 */
/* ========================================================================= */

void CMgrData::SetCharID(DWORD dwCharID)
{
	m_pInfoAccount->m_dwCharID = dwCharID;
}


/* ========================================================================= */
/* 関数名	:CMgrData::GetCharID											 */
/* 内容		:使用中のキャラIDを取得											 */
/* 日付		:2007/01/02														 */
/* ========================================================================= */

DWORD CMgrData::GetCharID(void)
{
	return m_pInfoAccount->m_dwCharID;
}


/* ========================================================================= */
/* 関数名	:CMgrData::GetMapID												 */
/* 内容		:現在のマップIDを取得											 */
/* 日付		:2008/11/29														 */
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
/* 関数名	:CMgrData::GetFamilyTypeName									 */
/* 内容		:種族名を取得													 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

LPCSTR CMgrData::GetFamilyTypeName(WORD wFamilyTypeID)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (wFamilyTypeID) {
	case FAMILYTYPE_HUMAN:	pszRet = "ニンゲン";	break;		/* ニンゲン */
	case FAMILYTYPE_ELF:	pszRet = "エルフ";		break;		/* エルフ */
	case FAMILYTYPE_BST:	pszRet = "ジュウジン";	break;		/* ジュウジン */
	case FAMILYTYPE_DAEMON:	pszRet = "マゾク";		break;		/* マゾク */
	}

	return pszRet;
}


/* ========================================================================= */
/* 関数名	:CMgrData::GetHairTypeName										 */
/* 内容		:髪型名を取得													 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

LPCSTR CMgrData::GetHairTypeName(WORD wHairTypeID)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (wHairTypeID) {
	case 1:		pszRet = "トリニティ";		break;
	case 2:		pszRet = "ツイン";			break;
	case 3:		pszRet = "カウボーイ";		break;
	case 4:		pszRet = "エッグ";			break;
	case 5:		pszRet = "ナイト";			break;
	case 6:		pszRet = "ウェーブ";		break;
	case 7:		pszRet = "シャイン";		break;
	case 8:		pszRet = "ポケット";		break;
	case 9:		pszRet = "ブレイブ";		break;
	case 10:	pszRet = "クール";			break;
	case 11:	pszRet = "レフト";			break;
	case 12:	pszRet = "テイルズ";		break;
	case 13:	pszRet = "ポニー";			break;
	case 14:	pszRet = "エッジ";			break;
	case 15:	pszRet = "ライト";			break;
	case 16:	pszRet = "トライフル";		break;
	case 17:	pszRet = "キャッスル";		break;
	case 18:	pszRet = "ピース";			break;
	case 19:	pszRet = "リトルデビル";	break;
	case 20:	pszRet = "ウインド";		break;
	case 21:	pszRet = "ノーブル";		break;
	case 22:	pszRet = "フェアリー";		break;
	}

	return pszRet;
}


/* ========================================================================= */
/* 関数名	:CMgrData::GetHairColorName										 */
/* 内容		:髪色名を取得													 */
/* 日付		:2007/05/11														 */
/* ========================================================================= */

LPCSTR CMgrData::GetHairColorName(WORD wHairColorID)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (wHairColorID) {
	case 1:		pszRet = "グレイ";		break;
	case 2:		pszRet = "ブラウン";	break;
	case 3:		pszRet = "アッシュ";	break;
	case 4:		pszRet = "シルバー";	break;
	case 5:		pszRet = "クリムゾン";	break;
	case 6:		pszRet = "スカイ";		break;
	case 7:		pszRet = "ブロンド";	break;
	case 8:		pszRet = "グリーン";	break;
	case 9:		pszRet = "チェリー";	break;
	case 10:	pszRet = "グレープ";	break;
	case 11:	pszRet = "オレンジ";	break;
	case 12:	pszRet = "リーフ";		break;
	case 13:	pszRet = "アクア";		break;
	}

	return pszRet;
}


/* ========================================================================= */
/* 関数名	:CMgrData::GetEyeColorName										 */
/* 内容		:目色名を取得													 */
/* 日付		:2007/05/11														 */
/* ========================================================================= */

LPCSTR CMgrData::GetEyeColorName(WORD wEyeColorID)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (wEyeColorID) {
	case 1:		pszRet = "ブラック";	break;
	case 2:		pszRet = "トパーズ";	break;
	case 3:		pszRet = "ルビー";		break;
	case 4:		pszRet = "アメジスト";	break;
	case 5:		pszRet = "エメラルド";	break;
	case 6:		pszRet = "サファイア";	break;
	}

	return pszRet;
}


/* ========================================================================= */
/* 関数名	:CMgrData::GetMotionName										 */
/* 内容		:モーション名を取得												 */
/* 日付		:2007/05/11														 */
/* ========================================================================= */

LPCSTR CMgrData::GetMotionName(DWORD dwMotionID)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (dwMotionID) {
	case CHARMOTIONLISTID_STAND_UP:					pszRet = "立ち(上)";				break;	/* 立ち(上) */
	case CHARMOTIONLISTID_STAND_DOWN:				pszRet = "立ち(下)";				break;	/* 立ち(下) */
	case CHARMOTIONLISTID_STAND_LEFT:				pszRet = "立ち(左)";				break;	/* 立ち(左) */
	case CHARMOTIONLISTID_STAND_RIGHT:				pszRet = "立ち(右)";				break;	/* 立ち(右) */
	case CHARMOTIONLISTID_WALK_UP:					pszRet = "歩き(上)";				break;	/* 歩き(上) */
	case CHARMOTIONLISTID_WALK_DOWN:				pszRet = "歩き(下)";				break;	/* 歩き(下) */
	case CHARMOTIONLISTID_WALK_LEFT:				pszRet = "歩き(左)";				break;	/* 歩き(左) */
	case CHARMOTIONLISTID_WALK_RIGHT:				pszRet = "歩き(右)";				break;	/* 歩き(右) */
	case CHARMOTIONLISTID_SIT_UP:					pszRet = "座り(上)";				break;	/* 座り(上) */
	case CHARMOTIONLISTID_SIT_DOWN:					pszRet = "座り(下)";				break;	/* 座り(下) */
	case CHARMOTIONLISTID_SIT_LEFT:					pszRet = "座り(左)";				break;	/* 座り(左) */
	case CHARMOTIONLISTID_SIT_RIGHT:				pszRet = "座り(右)";				break;	/* 座り(右) */
	case CHARMOTIONLISTID_BATTLESTAND_UP:			pszRet = "戦闘立ち(上)";			break;	/* 戦闘立ち(上) */
	case CHARMOTIONLISTID_BATTLESTAND_DOWN:			pszRet = "戦闘立ち(下)";			break;	/* 戦闘立ち(下) */
	case CHARMOTIONLISTID_BATTLESTAND_LEFT:			pszRet = "戦闘立ち(左)";			break;	/* 戦闘立ち(左) */
	case CHARMOTIONLISTID_BATTLESTAND_RIGHT:		pszRet = "戦闘立ち(右)";			break;	/* 戦闘立ち(右) */
	case CHARMOTIONLISTID_BATTLEWALK_UP:			pszRet = "すり足(上)";				break;	/* すり足(上) */
	case CHARMOTIONLISTID_BATTLEWALK_DOWN:			pszRet = "すり足(下)";				break;	/* すり足(下) */
	case CHARMOTIONLISTID_BATTLEWALK_LEFT:			pszRet = "すり足(左)";				break;	/* すり足(左) */
	case CHARMOTIONLISTID_BATTLEWALK_RIGHT:			pszRet = "すり足(右)";				break;	/* すり足(右) */
	case CHARMOTIONLISTID_SWING_UP:					pszRet = "振り(上)";				break;	/* 振り(上) */
	case CHARMOTIONLISTID_SWING_DOWN:				pszRet = "振り(下)";				break;	/* 振り(下) */
	case CHARMOTIONLISTID_SWING_LEFT:				pszRet = "振り(左)";				break;	/* 振り(左) */
	case CHARMOTIONLISTID_SWING_RIGHT:				pszRet = "振り(右)";				break;	/* 振り(右) */
	case CHARMOTIONLISTID_POKE_UP:					pszRet = "突き(上)";				break;	/* 突き(上) */
	case CHARMOTIONLISTID_POKE_DOWN:				pszRet = "突き(下)";				break;	/* 突き(下) */
	case CHARMOTIONLISTID_POKE_LEFT:				pszRet = "突き(左)";				break;	/* 突き(左) */
	case CHARMOTIONLISTID_POKE_RIGHT:				pszRet = "突き(右)";				break;	/* 突き(右) */
	case CHARMOTIONLISTID_BOWWALK_UP:				pszRet = "弓用すり足(上)";			break;	/* 弓用すり足(上) */
	case CHARMOTIONLISTID_BOWWALK_DOWN:				pszRet = "弓用すり足(下)";			break;	/* 弓用すり足(下) */
	case CHARMOTIONLISTID_BOWWALK_LEFT:				pszRet = "弓用すり足(左)";			break;	/* 弓用すり足(左) */
	case CHARMOTIONLISTID_BOWWALK_RIGHT:			pszRet = "弓用すり足(右)";			break;	/* 弓用すり足(右) */
	case CHARMOTIONLISTID_BOW_UP:					pszRet = "弓(上)";					break;	/* 弓(上) */
	case CHARMOTIONLISTID_BOW_DOWN:					pszRet = "弓(下)";					break;	/* 弓(下) */
	case CHARMOTIONLISTID_BOW_LEFT:					pszRet = "弓(左)";					break;	/* 弓(左) */
	case CHARMOTIONLISTID_BOW_RIGHT:				pszRet = "弓(右)";					break;	/* 弓(右) */
	case CHARMOTIONLISTID_GLOVEWALK_UP:				pszRet = "打撃用すり足(上)";		break;	/* 打撃用すり足(上) */
	case CHARMOTIONLISTID_GLOVEWALK_DOWN:			pszRet = "打撃用すり足(下)";		break;	/* 打撃用すり足(下) */
	case CHARMOTIONLISTID_GLOVEWALK_LEFT:			pszRet = "打撃用すり足(左)";		break;	/* 打撃用すり足(左) */
	case CHARMOTIONLISTID_GLOVEWALK_RIGHT:			pszRet = "打撃用すり足(右)";		break;	/* 打撃用すり足(右) */
	case CHARMOTIONLISTID_GLOVE_UP:					pszRet = "打撃(上)";				break;	/* 打撃(上) */
	case CHARMOTIONLISTID_GLOVE_DOWN:				pszRet = "打撃(下)";				break;	/* 打撃(下) */
	case CHARMOTIONLISTID_GLOVE_LEFT:				pszRet = "打撃(左)";				break;	/* 打撃(左) */
	case CHARMOTIONLISTID_GLOVE_RIGHT:				pszRet = "打撃(右)";				break;	/* 打撃(右) */
	case CHARMOTIONLISTID_FISHINGWALK_UP:			pszRet = "釣り用すり足(上)";		break;	/* 釣り用すり足(上) */
	case CHARMOTIONLISTID_FISHINGWALK_DOWN:			pszRet = "釣り用すり足(下)";		break;	/* 釣り用すり足(下) */
	case CHARMOTIONLISTID_FISHINGWALK_LEFT:			pszRet = "釣り用すり足(左)";		break;	/* 釣り用すり足(左) */
	case CHARMOTIONLISTID_FISHINGWALK_RIGHT:		pszRet = "釣り用すり足(右)";		break;	/* 釣り用すり足(右) */
	case CHARMOTIONLISTID_FISHING_UP:				pszRet = "釣り(上)";				break;	/* 釣り(上) */
	case CHARMOTIONLISTID_FISHING_DOWN:				pszRet = "釣り(下)";				break;	/* 釣り(下) */
	case CHARMOTIONLISTID_FISHING_LEFT:				pszRet = "釣り(左)";				break;	/* 釣り(左) */
	case CHARMOTIONLISTID_FISHING_RIGHT:			pszRet = "釣り(右)";				break;	/* 釣り(右) */
	case CHARMOTIONLISTID_FISHING_HIT_UP:			pszRet = "釣りヒット(上)";			break;	/* 釣りヒット(上) */
	case CHARMOTIONLISTID_FISHING_HIT_DOWN:			pszRet = "釣りヒット(下)";			break;	/* 釣りヒット(下) */
	case CHARMOTIONLISTID_FISHING_HIT_LEFT:			pszRet = "釣りヒット(左)";			break;	/* 釣りヒット(左) */
	case CHARMOTIONLISTID_FISHING_HIT_RIGHT:		pszRet = "釣りヒット(右)";			break;	/* 釣りヒット(右) */
	case CHARMOTIONLISTID_BOWBATTLESTAND_UP:		pszRet = "弓用戦闘立ち(上)";		break;	/* 弓用戦闘立ち(上) */
	case CHARMOTIONLISTID_BOWBATTLESTAND_DOWN:		pszRet = "弓用戦闘立ち(下)";		break;	/* 弓用戦闘立ち(下) */
	case CHARMOTIONLISTID_BOWBATTLESTAND_LEFT:		pszRet = "弓用戦闘立ち(左)";		break;	/* 弓用戦闘立ち(左) */
	case CHARMOTIONLISTID_BOWBATTLESTAND_RIGHT:		pszRet = "弓用戦闘立ち(右)";		break;	/* 弓用戦闘立ち(右) */
	case CHARMOTIONLISTID_GLOVEBATTLESTAND_UP:		pszRet = "打撃用戦闘立ち(上)";		break;	/* 打撃用戦闘立ち(上) */
	case CHARMOTIONLISTID_GLOVEBATTLESTAND_DOWN:	pszRet = "打撃用戦闘立ち(下)";		break;	/* 打撃用戦闘立ち(下) */
	case CHARMOTIONLISTID_GLOVEBATTLESTAND_LEFT:	pszRet = "打撃用戦闘立ち(左)";		break;	/* 打撃用戦闘立ち(左) */
	case CHARMOTIONLISTID_GLOVEBATTLESTAND_RIGHT:	pszRet = "打撃用戦闘立ち(右)";		break;	/* 打撃用戦闘立ち(右) */
	case CHARMOTIONLISTID_FISHINGBATTLESTAND_UP:	pszRet = "釣り用戦闘立ち(上)";		break;	/* 釣り用戦闘立ち(上) */
	case CHARMOTIONLISTID_FISHINGBATTLESTAND_DOWN:	pszRet = "釣り用戦闘立ち(下)";		break;	/* 釣り用戦闘立ち(下) */
	case CHARMOTIONLISTID_FISHINGBATTLESTAND_LEFT:	pszRet = "釣り用戦闘立ち(左)";		break;	/* 釣り用戦闘立ち(左) */
	case CHARMOTIONLISTID_FISHINGBATTLESTAND_RIGHT:	pszRet = "釣り用戦闘立ち(右)";		break;	/* 釣り用戦闘立ち(右) */
	case CHARMOTIONLISTID_STAND:					pszRet = "立ち";					break;	/* 立ち */
	case CHARMOTIONLISTID_STAND_SIMPLE:				pszRet = "立ち(上下左右1コマずつ)";	break;	/* 立ち(上下左右1コマずつ) */
	case CHARMOTIONLISTID_WALK:						pszRet = "歩き(方向無し)";			break;	/* 歩き(方向無し) */
	case CHARMOTIONLISTID_DESTROY:					pszRet = "破壊";					break;	/* 破壊 */
	case CHARMOTIONLISTID_SIMPLE:					pszRet = "16コマ連続";				break;	/* 16コマ連続 */
	case CHARMOTIONLISTID_DEFENSE_UP:				pszRet = "防御(上)";				break;	/* 防御(上) */
	case CHARMOTIONLISTID_DEFENSE_DOWN:				pszRet = "防御(下)";				break;	/* 防御(下) */
	case CHARMOTIONLISTID_DEFENSE_LEFT:				pszRet = "防御(左)";				break;	/* 防御(左) */
	case CHARMOTIONLISTID_DEFENSE_RIGHT:			pszRet = "防御(右)";				break;	/* 防御(右) */
	case CHARMOTIONLISTID_DEFENSE_SUCCESS_UP:		pszRet = "防御成功(上)";			break;	/* 防御成功(上) */
	case CHARMOTIONLISTID_DEFENSE_SUCCESS_DOWN:		pszRet = "防御成功(下)";			break;	/* 防御成功(下) */
	case CHARMOTIONLISTID_DEFENSE_SUCCESS_LEFT:		pszRet = "防御成功(左)";			break;	/* 防御成功(左) */
	case CHARMOTIONLISTID_DEFENSE_SUCCESS_RIGHT:	pszRet = "防御成功(右)";			break;	/* 防御成功(右) */
	case CHARMOTIONLISTID_BOWDEFENSE_UP:			pszRet = "弓用防御(上)";			break;	/* 弓用防御(上) */
	case CHARMOTIONLISTID_BOWDEFENSE_DOWN:			pszRet = "弓用防御(下)";			break;	/* 弓用防御(下) */
	case CHARMOTIONLISTID_BOWDEFENSE_LEFT:			pszRet = "弓用防御(左)";			break;	/* 弓用防御(左) */
	case CHARMOTIONLISTID_BOWDEFENSE_RIGHT:			pszRet = "弓用防御(右)";			break;	/* 弓用防御(右) */
	case CHARMOTIONLISTID_DAMAGE_UP:				pszRet = "ダメージ(上)";			break;	/* ダメージ(上) */
	case CHARMOTIONLISTID_DAMAGE_DOWN:				pszRet = "ダメージ(下)";			break;	/* ダメージ(下) */
	case CHARMOTIONLISTID_DAMAGE_LEFT:				pszRet = "ダメージ(左)";			break;	/* ダメージ(左) */
	case CHARMOTIONLISTID_DAMAGE_RIGHT:				pszRet = "ダメージ(右)";			break;	/* ダメージ(右) */
	case CHARMOTIONLISTID_DEAD_UP:					pszRet = "死亡(上)";				break;	/* 死亡(上) */
	case CHARMOTIONLISTID_DEAD_DOWN:				pszRet = "死亡(下)";				break;	/* 死亡(下) */
	case CHARMOTIONLISTID_DEAD_LEFT:				pszRet = "死亡(左)";				break;	/* 死亡(左) */
	case CHARMOTIONLISTID_DEAD_RIGHT:				pszRet = "死亡(右)";				break;	/* 死亡(右) */
	}

	return pszRet;
}


/* ========================================================================= */
/* 関数名	:CMgrData::GetMotionProcName									 */
/* 内容		:行動名を取得													 */
/* 日付		:2008/01/01														 */
/* ========================================================================= */

LPCSTR CMgrData::GetMotionProcName(DWORD dwProcID)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (dwProcID) {
	case CHARMOTIONPROCID_ATACK:		pszRet = "攻撃";		break;		/* 攻撃 */
	case CHARMOTIONPROCID_FISHING:		pszRet = "釣り";		break;		/* 釣り */
	default:							pszRet = "何もしない";	break;
	}

	return pszRet;
}


/* ========================================================================= */
/* 関数名	:CMgrData::GetMapEventName										 */
/* 内容		:マップイベント種別名を取得										 */
/* 日付		:2008/07/17														 */
/* ========================================================================= */

LPCSTR CMgrData::GetMapEventName(int nMapEventType)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (nMapEventType) {
	case MAPEVENTTYPE_MOVE:			pszRet = "マップ内移動";	break;	/* マップ内移動 */
	case MAPEVENTTYPE_MAPMOVE:		pszRet = "マップ間移動";	break;	/* マップ間移動 */
	case MAPEVENTTYPE_TRASHBOX:		pszRet = "ゴミ箱";			break;	/* ゴミ箱 */
	case MAPEVENTTYPE_INITSTATUS:	pszRet = "ステータス初期化";break;	/* ステータス初期化 */
	case MAPEVENTTYPE_GRPIDTMP:		pszRet = "一時画像設定";	break;	/* 一時画像設定 */
	case MAPEVENTTYPE_LIGHT:		pszRet = "灯り";			break;	/* 灯り */
	default:						pszRet = "未設定";			break;
	}

	return pszRet;
}


/* ========================================================================= */
/* 関数名	:CMgrData::GetSystemMsgCount									 */
/* 内容		:システムメッセージ数を取得										 */
/* 日付		:2007/09/06														 */
/* ========================================================================= */

int CMgrData::GetSystemMsgCount(void)
{
	return m_aSystemMsg.GetSize ();
}


/* ========================================================================= */
/* 関数名	:CMgrData::GetSystemMsg											 */
/* 内容		:システムメッセージを追加										 */
/* 日付		:2007/09/06														 */
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
/* 関数名	:CMgrData::AddSystemMsg											 */
/* 内容		:システムメッセージを追加										 */
/* 日付		:2007/09/06														 */
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
/* 関数名	:CMgrData::DeleteAllSystemMsg									 */
/* 内容		:システムメッセージを全て削除									 */
/* 日付		:2007/09/06														 */
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
/* 関数名	:CMgrData::SetSEVolume											 */
/* 内容		:効果音量を設定													 */
/* 日付		:2007/09/16														 */
/* ========================================================================= */

void CMgrData::SetSEVolume(int nSEVolume)
{
	m_nSEVolume = nSEVolume;
	SaveIniData ();
}


/* ========================================================================= */
/* 関数名	:CMgrData::SetSEVolume											 */
/* 内容		:BGM音量を設定													 */
/* 日付		:2007/09/16														 */
/* ========================================================================= */

void CMgrData::SetBGMVolume(int nBGMVolume)
{
	m_nBGMVolume = nBGMVolume;
	SaveIniData ();
}


/* ========================================================================= */
/* 関数名	:CMgrData::SetInfoTalkEvent										 */
/* 内容		:会話イベント情報の設定											 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

void CMgrData::SetInfoTalkEvent(CInfoTalkEvent *pSrc)
{
	m_pInfoTalkEvent->Copy (pSrc);
}


/* ========================================================================= */
/* 関数名	:CMgrData::GetInfoTalkEvent										 */
/* 内容		:会話イベント情報の取得											 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

CInfoTalkEvent *CMgrData::GetInfoTalkEvent(void)
{
	return m_pInfoTalkEvent;
}


/* ========================================================================= */
/* 関数名	:CMgrData::ReadIniData											 */
/* 内容		:iniファイルから設定を読み込み									 */
/* 日付		:2006/09/24														 */
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

	/* サーバーアドレス */
	GetPrivateProfileString ("Setting", "ServerAddr", "127.0.0.1", szTmp, sizeof (szTmp) - 1, szFileName);
	m_strServerAddr = szTmp;
	/* 待ちうけポート */
	m_wServerPort = GetPrivateProfileInt ("Setting", "ServerPort", 2006, szFileName);
	/* パスワードを保存する？ */
	m_bSavePassword = (BOOL)GetPrivateProfileInt ("Setting", "SavePassword", 0, szFileName);
	/* 発言時にタスクバーチカチカ */
	m_bOptionTaskbar = (BOOL)GetPrivateProfileInt ("Setting", "OptionTaskbar", 0, szFileName);
	/* 発言を表示する */
	m_bOptionViewChat = (BOOL)GetPrivateProfileInt ("Setting", "OptionViewChat", 1, szFileName);
	/* アイテムを表示する */
	m_bOptionViewItem = (BOOL)GetPrivateProfileInt ("Setting", "OptionViewItem", 1, szFileName);
	/* アイテム名を表示する */
	m_bOptionViewItemName = (BOOL)GetPrivateProfileInt ("Setting", "OptionViewItemName", 1, szFileName);
	/* ヘルプアイコンを表示する */
	m_bOptionViewHelpIcon = (BOOL)GetPrivateProfileInt ("Setting", "OptionViewHelpIcon", 1, szFileName);
	/* 戦闘メッセージをログに残す */
	m_bOptionBattleMsgLog = (BOOL)GetPrivateProfileInt ("Setting", "OptionBattleMsgLog", 0, szFileName);
	/* 60フレームで表示する */
	m_bOption60Frame = (BOOL)GetPrivateProfileInt ("Setting", "Option60Frame", 0, szFileName);
	/* 最終アカウント名 */
	GetPrivateProfileString ("Account", "Account", "", szTmp, sizeof (szTmp) - 1, szFileName);
	m_strLastAccount = szTmp;
	/* 最終パスワード */
GetPrivateProfileString ("Account", "Password", "", szTmp, sizeof (szTmp) - 1, szFileName);
	CryptUtil.UnCryptStr (szTmp, szTmp2, 10);
	m_strLastPassword = szTmp2;
	/* 効果音量 */
	nTmp = GetPrivateProfileInt ("Setting", "SEVolume", 2, szFileName);
	m_nSEVolume = min (nTmp, 4);
	/* BGM音量 */
	nTmp = GetPrivateProfileInt ("Setting", "BGMVolume", 2, szFileName);
	m_nBGMVolume = min (nTmp, 4);
	/* 描画モード */
	nTmp = GetPrivateProfileInt ("Setting", "DrawMode", 1, szFileName);
	m_nDrawMode = min (nTmp, 4);
	/* 入力ジョイパッド */
	GetPrivateProfileString ("Setting", "InputDevice", "", szTmp, sizeof (szTmp) - 1, szFileName);
	bstrTmp = szTmp;
	CLSIDFromString (bstrTmp, &m_stInputGuid); 
	/* おひるねタイマー */
	nTmp = GetPrivateProfileInt ("Setting", "SleepTimer", 0, szFileName);
	m_nSleepTimer = min (nTmp, 5);
}

/* Copyright(C)URARA-works 2006 */
