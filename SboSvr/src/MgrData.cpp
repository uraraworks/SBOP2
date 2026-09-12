/// @file MgrData.cpp
/// @brief データマネージャクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/04
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "../../third_party/sqlite/sqlite3.h"
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
#include "SaveLoadInfoBase.h"
#include "LibInfoCharSVr.h"
#include "MgrData.h"
#include "PasswordHash.h"
#include <string>
#include "Platform/SvrPlatform.h"

CMgrData::CMgrData()
{
	m_wPort	= 2005;
	m_wHttpPort	= 18080;
	m_bCookieSecure	= FALSE;
	m_byLastSendClock	= -1;
	m_pMainFrame	= NULL;
	m_pSock	= NULL;

	m_pLog	= new CTextOutput;
	m_pLibInfoAccount	= new CLibInfoAccount;
	m_pLibInfoChar	= new CLibInfoCharSvr;
	m_pLibInfoDisable	= new CLibInfoDisable;
	m_pLibInfoMap	= new CLibInfoMapBase;
	m_pLibInfoMapObject	= new CLibInfoMapObject;
	m_pLibInfoMapParts	= new CLibInfoMapParts;
	m_pLibInfoMapShadow	= new CLibInfoMapShadow;
	m_pLibInfoItemType	= new CLibInfoItemType;
	m_pLibInfoItem	= new CLibInfoItem;
	m_pLibInfoItemWeapon	= new CLibInfoItemWeapon;
	m_pLibInfoEffect	= new CLibInfoEffect;
	m_pLibInfoMotion	= new CLibInfoMotion;
	m_pLibInfoMotionType	= new CLibInfoMotionType;
	m_pLibInfoEfcBalloon	= new CLibInfoEfcBalloon;
	m_pLibInfoSystem	= new CLibInfoSystem;
	m_pLibInfoSkill	= new CLibInfoSkill;
	m_pLibInfoTalkEvent	= new CLibInfoTalkEvent;
	m_pInfoFileList	= new CInfoFileList;
}

CMgrData::~CMgrData()
{
	SAFE_DELETE(m_pLog);
	SAFE_DELETE(m_pLibInfoAccount);
	SAFE_DELETE(m_pLibInfoChar);
	SAFE_DELETE(m_pLibInfoDisable);
	SAFE_DELETE(m_pLibInfoMap);
	SAFE_DELETE(m_pLibInfoMapObject);
	SAFE_DELETE(m_pLibInfoMapParts);
	SAFE_DELETE(m_pLibInfoMapShadow);
	SAFE_DELETE(m_pLibInfoItemType);
	SAFE_DELETE(m_pLibInfoItem);
	SAFE_DELETE(m_pLibInfoItemWeapon);
	SAFE_DELETE(m_pLibInfoEffect);
	SAFE_DELETE(m_pLibInfoMotion);
	SAFE_DELETE(m_pLibInfoMotionType);
	SAFE_DELETE(m_pLibInfoEfcBalloon);
	SAFE_DELETE(m_pLibInfoSystem);
	SAFE_DELETE(m_pLibInfoSkill);
	SAFE_DELETE(m_pLibInfoTalkEvent);
	SAFE_DELETE(m_pInfoFileList);
}

void CMgrData::Create(
	CMainFrame	*pMainFrame,	// [in] メインフレーム
	CUraraSockTCPSBO	*pSock)	// [in] 通信マネージャ
{
	CmyString strTmp;

	ReadIniData();
	ReadHashList();

	m_pMainFrame	= pMainFrame;
	m_pSock	= pSock;

	m_pLibInfoAccount->	Create();
	m_pLibInfoChar->	Create(this);
	m_pLibInfoDisable->	Create();
	m_pLibInfoMapObject->	Create();
	m_pLibInfoMapParts->	Create();
	m_pLibInfoMapShadow->	Create();
	m_pLibInfoMap->	Create(m_pLibInfoMapParts);
	m_pLibInfoItemType->	Create();
	m_pLibInfoItem->	Create();
	m_pLibInfoItemWeapon->	Create();
	m_pLibInfoEffect->	Create();
	m_pLibInfoMotion->	Create();
	m_pLibInfoMotionType->	Create();
	m_pLibInfoEfcBalloon->	Create();
	m_pLibInfoSystem->	Create();
	m_pLibInfoSkill->	Create();
	m_pLibInfoTalkEvent->	Create();

	m_pLibInfoItem->SetTypeInfo(m_pLibInfoItemType);
	m_pLibInfoItem->SetWeaponInfo(m_pLibInfoItemWeapon);

	// 実行ファイルのディレクトリ取得は SboPlatform::GetExeDirectory() に集約済み
	// (末尾は区切り文字で終わる)。GrpImageStore.cpp と同じ置き換え方針。
	std::string strExeDir = SboPlatform::GetExeDirectory();

	// ログファイルの作成
	strTmp = strExeDir.c_str();
	strTmp += "SboSvrLog.txt";
	m_pLog->Create(strTmp, TRUE, TRUE);
}

void CMgrData::Destroy(void)
{
	m_pLibInfoAccount->	Destroy();
	m_pLibInfoChar->	Destroy();
	m_pLibInfoDisable->	Destroy();
	m_pLibInfoMap->	Destroy();
	m_pLibInfoMapObject->	Destroy();
	m_pLibInfoMapParts->	Destroy();
	m_pLibInfoMapShadow->	Destroy();
	m_pLibInfoItemType->	Destroy();
	m_pLibInfoItem->	Destroy();
	m_pLibInfoItemWeapon->	Destroy();
	m_pLibInfoEffect->	Destroy();
	m_pLibInfoMotion->	Destroy();
	m_pLibInfoMotionType->	Destroy();
	m_pLibInfoEfcBalloon->	Destroy();
	m_pLibInfoSystem->	Destroy();
	m_pLibInfoSkill->	Destroy();
	m_pLibInfoTalkEvent->	Destroy();
}

// -------------------------------------------------------
// SQLite DB を開いて s_pDb にセットするヘルパー関数
// 戻り値: 成功時 true
// -------------------------------------------------------
static bool OpenSboDb(sqlite3 **ppDb)
{
	// SBODATA ディレクトリを作り、その中の DB パスを組み立てる
	std::string strDbPath = SboPlatform::MakeDataFilePath("SboData.db");
	const char *szDbPath = strDbPath.c_str();

	// SQLite オープン
	int nRet = sqlite3_open(szDbPath, ppDb);
	if (nRet != SQLITE_OK) {
		SboPlatform::WriteDebugLine("CMgrData: sqlite3_open failed\n");
		*ppDb = NULL;
		return false;
	}

	// テーブルが無ければ作成、PRAGMA 設定
	const char *pszInit =
		"PRAGMA journal_mode=WAL;"
		"PRAGMA synchronous=NORMAL;"
		"CREATE TABLE IF NOT EXISTS sbo_data("
		"  name TEXT PRIMARY KEY NOT NULL,"
		"  data BLOB NOT NULL"
		");";
	sqlite3_exec(*ppDb, pszInit, NULL, NULL, NULL);

	return true;
}

void CMgrData::Save(void)
{
	CSaveLoadInfoAccount SaveLoadInfoAccount;
	CSaveLoadInfoChar SaveLoadInfoChar;
	CSaveLoadInfoDisable SaveLoadInfoDisable;
	CSaveLoadInfoMap SaveLoadInfoMap;
	CSaveLoadInfoMapObject SaveLoadInfoMapObject;
	CSaveLoadInfoMapParts SaveLoadInfoMapParts;
	CSaveLoadInfoMapShadow SaveLoadInfoMapShadow;
	CSaveLoadInfoMapEvent SaveLoadInfoMapEvent;
	CSaveLoadInfoMapObjectData SaveLoadInfoMapObjectData;
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

	// SQLite 接続を開いてクラス共有静的変数にセット
	sqlite3 *pDb = NULL;
	if (OpenSboDb(&pDb)) {
		CSaveLoadInfoBase::s_pDb = pDb;
		sqlite3_exec(pDb, "BEGIN TRANSACTION;", NULL, NULL, NULL);
	} else {
		CSaveLoadInfoBase::s_pDb = NULL;
	}

	SaveLoadInfoAccount.	Save((PCLibInfoBase)m_pLibInfoAccount);
	SaveLoadInfoChar.	Save((PCLibInfoBase)m_pLibInfoChar);
	SaveLoadInfoDisable.	Save((PCLibInfoBase)m_pLibInfoDisable);
	SaveLoadInfoMap.	Save((PCLibInfoBase)m_pLibInfoMap);
	SaveLoadInfoMapEvent.	Save(m_pLibInfoMap);
	SaveLoadInfoMapObjectData.	Save(m_pLibInfoMap);
	SaveLoadInfoMapObject.	Save((PCLibInfoBase)m_pLibInfoMapObject);
	SaveLoadInfoMapParts.	Save((PCLibInfoBase)m_pLibInfoMapParts);
	SaveLoadInfoMapShadow.	Save((PCLibInfoBase)m_pLibInfoMapShadow);
	SaveLoadInfoEffect.	Save((PCLibInfoBase)m_pLibInfoEffect);
	SaveLoadInfoItemType.	Save((PCLibInfoBase)m_pLibInfoItemType);
	SaveLoadInfoItem.	Save((PCLibInfoBase)m_pLibInfoItem);
	SaveLoadInfoItemWeapon.	Save((PCLibInfoBase)m_pLibInfoItemWeapon);
	SaveLoadInfoMotion.	Save((PCLibInfoBase)m_pLibInfoMotion);
	SaveLoadInfoMotionType.	Save((PCLibInfoBase)m_pLibInfoMotionType);
	SaveLoadInfoEfcBalloon.	Save((PCLibInfoBase)m_pLibInfoEfcBalloon);
	SaveLoadInfoSystem.	Save((PCLibInfoBase)m_pLibInfoSystem);
	SaveLoadInfoSkill.	Save((PCLibInfoBase)m_pLibInfoSkill);
	SaveLoadInfoTalkEvent.	Save((PCLibInfoBase)m_pLibInfoTalkEvent);

	// トランザクションをコミットして接続を閉じる
	if (pDb != NULL) {
		sqlite3_exec(pDb, "COMMIT;", NULL, NULL, NULL);
		sqlite3_close(pDb);
		CSaveLoadInfoBase::s_pDb = NULL;
	}
}

void CMgrData::Load(void)
{
	CSaveLoadInfoAccount SaveLoadInfoAccount;
	CSaveLoadInfoChar SaveLoadInfoChar;
	CSaveLoadInfoDisable SaveLoadInfoDisable;
	CSaveLoadInfoMap SaveLoadInfoMap;
	CSaveLoadInfoMapObject SaveLoadInfoMapObject;
	CSaveLoadInfoMapParts SaveLoadInfoMapParts;
	CSaveLoadInfoMapShadow SaveLoadInfoMapShadow;
	CSaveLoadInfoMapEvent SaveLoadInfoMapEvent;
	CSaveLoadInfoMapObjectData SaveLoadInfoMapObjectData;
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

	// SQLite 接続を開いてクラス共有静的変数にセット（DB が無ければ open だけ、行なしで .dat フォールバックが働く）
	sqlite3 *pDb = NULL;
	if (OpenSboDb(&pDb)) {
		CSaveLoadInfoBase::s_pDb = pDb;
	} else {
		CSaveLoadInfoBase::s_pDb = NULL;
	}

	m_pLibInfoAccount->	DeleteAll();
	m_pLibInfoChar->	DeleteAll();
	m_pLibInfoDisable->	DeleteAll();
	m_pLibInfoMap->	DeleteAll();
	m_pLibInfoMapObject->	DeleteAll();
	m_pLibInfoMapParts->	DeleteAll();
	m_pLibInfoMapShadow->	DeleteAll();
	m_pLibInfoItemType->	DeleteAll();
	m_pLibInfoItem->	DeleteAll();
	m_pLibInfoItemWeapon->	DeleteAll();
	m_pLibInfoMotion->	DeleteAll();
	m_pLibInfoMotionType->	DeleteAll();
	m_pLibInfoEfcBalloon->	DeleteAll();
	m_pLibInfoSkill->	DeleteAll();
	m_pLibInfoTalkEvent->	DeleteAll();

	SaveLoadInfoAccount.	Load((PCLibInfoBase)m_pLibInfoAccount);

	// アカウントパスワードの自動ハッシュ化移行(平文のままのものだけ対象。冪等)
	{
		int nAccountCount = m_pLibInfoAccount->GetCount();
		int nPlainCount = 0;
		int nMigrated = 0;

		// PBKDF2 は1件あたり数十msかかる。件数が多いと数十秒間まったく反応が無いように
		// 見えてしまうため、先に対象件数を数えて開始時点でログを出しておく
		for (int i = 0; i < nAccountCount; i ++) {
			PCInfoAccount pInfoAccount = (PCInfoAccount)m_pLibInfoAccount->GetPtr(i);

			if (!PasswordHash::IsHashed(pInfoAccount->m_strPassword.GetUtf8Pointer())) {
				nPlainCount ++;
			}
		}
		if (nPlainCount > 0) {
			if (m_pLog != NULL) {
				m_pLog->Write("アカウントのパスワードをハッシュ化します(%d件)。完了までしばらくかかります", nPlainCount);
			} else {
				SboPlatform::WriteDebugLine("CMgrData: password hash migration start\n");
			}
		}

		for (int i = 0; i < nAccountCount; i ++) {
			PCInfoAccount pInfoAccount = (PCInfoAccount)m_pLibInfoAccount->GetPtr(i);

			if (PasswordHash::IsHashed(pInfoAccount->m_strPassword.GetUtf8Pointer())) {
				continue;
			}

			std::string strHashed = PasswordHash::Hash(pInfoAccount->m_strPassword.GetUtf8Pointer());
			if (strHashed.empty()) {
				// ハッシュ化に失敗した場合は平文のまま残す(空文字列で上書きしない)
				if (m_pLog != NULL) {
					m_pLog->Write("警告: アカウントのパスワードハッシュ化に失敗したため平文のまま残します [AccountID:%u]", pInfoAccount->m_dwAccountID);
				} else {
					SboPlatform::WriteDebugLine("CMgrData: password hash failed, keep plain text\n");
				}
				continue;
			}

			pInfoAccount->m_strPassword = (LPCTSTR)Utf8ToTString(strHashed.c_str());
			nMigrated ++;
		}

		if (nMigrated > 0) {
			// 全件 DELETE + INSERT のため、トランザクションで囲まないと
			// INSERT ごとにコミットが走って極端に遅くなる(CMgrData::Save と同じ扱い)
			if (pDb != NULL) {
				sqlite3_exec(pDb, "BEGIN TRANSACTION;", NULL, NULL, NULL);
			}
			SaveLoadInfoAccount.	Save((PCLibInfoBase)m_pLibInfoAccount);
			if (pDb != NULL) {
				sqlite3_exec(pDb, "COMMIT;", NULL, NULL, NULL);
			}

			if (m_pLog != NULL) {
				m_pLog->Write("アカウントのパスワードを%d件ハッシュ化しました", nMigrated);
			} else {
				SboPlatform::WriteDebugLine("CMgrData: password hash migration done\n");
			}
		}
	}

	SaveLoadInfoChar.	Load((PCLibInfoBase)m_pLibInfoChar);
	SaveLoadInfoDisable.	Load((PCLibInfoBase)m_pLibInfoDisable);
	SaveLoadInfoMap.	Load((PCLibInfoBase)m_pLibInfoMap);
	SaveLoadInfoMapObject.	Load((PCLibInfoBase)m_pLibInfoMapObject);
	SaveLoadInfoMapParts.	Load((PCLibInfoBase)m_pLibInfoMapParts);
	SaveLoadInfoMapShadow.	Load((PCLibInfoBase)m_pLibInfoMapShadow);
	SaveLoadInfoMapEvent.	Load(m_pLibInfoMap);	// マップロード後にイベントを復元
	SaveLoadInfoMapObjectData.	Load(m_pLibInfoMap);	// マップ・イベント復元後に配置を復元
	SaveLoadInfoEffect.	Load((PCLibInfoBase)m_pLibInfoEffect);
	SaveLoadInfoItemType.	Load((PCLibInfoBase)m_pLibInfoItemType);
	SaveLoadInfoItem.	Load((PCLibInfoBase)m_pLibInfoItem);
	SaveLoadInfoItemWeapon.	Load((PCLibInfoBase)m_pLibInfoItemWeapon);
	SaveLoadInfoMotion.	Load((PCLibInfoBase)m_pLibInfoMotion);
	SaveLoadInfoMotionType.	Load((PCLibInfoBase)m_pLibInfoMotionType);
	SaveLoadInfoEfcBalloon.	Load((PCLibInfoBase)m_pLibInfoEfcBalloon);
	SaveLoadInfoSystem.	Load((PCLibInfoBase)m_pLibInfoSystem);
	SaveLoadInfoSkill.	Load((PCLibInfoBase)m_pLibInfoSkill);
	SaveLoadInfoTalkEvent.	Load((PCLibInfoBase)m_pLibInfoTalkEvent);
	if (m_pLibInfoMap->GetCount() <= 0) {
		PCInfoMapBase pMapTmp;

		pMapTmp = (PCInfoMapBase)m_pLibInfoMap->GetNew();
		pMapTmp->Init(20, 20, 0);
		m_pLibInfoMap->Add(pMapTmp);
	}

	// SQLite 接続を閉じる
	if (pDb != NULL) {
		sqlite3_close(pDb);
		CSaveLoadInfoBase::s_pDb = NULL;
	}

	// 読み込み後のデータ補正
	m_pLibInfoChar->	Revice();
	m_pLibInfoItemType->	Revice();
	m_pLibInfoMap->	RenewMapEvent();
	m_pLibInfoMap->	SetMapObject(m_pLibInfoMapObject);
	m_pLibInfoMap->	RenewHitTmp();
	m_pLibInfoMotion->	Revice();
}

void CMgrData::ReadHashList(void)
{
	int i, nCount;
        char szFileName[MAX_PATH];
	DWORD dwTmp;
	CTextInput TextInput;
	CParamUtil ParamUtil;
	CmyString strTmp;

	m_pInfoFileList->DeleteAll();

        // 実行ファイルのディレクトリ取得は SboPlatform::GetExeDirectory() に集約済み
        // (末尾に区切り文字を含む点は GetModuleFilePath と同じ)
        std::string strBasePath = SboPlatform::GetExeDirectory();
        strcpy_s(szFileName, strBasePath.c_str());
        strcat_s(szFileName, "Update\\SBOHashList.txt");

	TextInput.Create(szFileName);

	nCount = TextInput.GetLineCount();
	for (i = 0; i < nCount; i ++) {
		strTmp = TextInput.ReadLine();
		ParamUtil.Set(strTmp, ',');
		dwTmp = atoi(ParamUtil.GetParam(1));
		m_pInfoFileList->Add(ParamUtil.GetParam(0), ParamUtil.GetParam(2), dwTmp);
	}
}

unsigned int CMgrData::CountOnlineAccounts(void)
{
        // 接続中 = セッションIDが割り当たっているアカウント数。
        // ServerInfoHandler(/api/server) と HealthHandler(/health) の両方から使う。
        // 呼び出し元は別スレッド(HTTPハンドラ)であるため、必ずロックを取ってから走査する。
        unsigned int nCount = 0;

        if (m_pLibInfoAccount == NULL) {
                return 0;
        }

        m_pLibInfoAccount->Enter();
        int nTotal = m_pLibInfoAccount->GetCount();
        for (int i = 0; i < nTotal; i ++) {
                PCInfoAccount pAccount = (PCInfoAccount)m_pLibInfoAccount->GetPtr(i);
                if ((pAccount != NULL) && (pAccount->m_dwSessionID != 0)) {
                        nCount ++;
                }
        }
        m_pLibInfoAccount->Leave();

        return nCount;
}

void CMgrData::SetClientVersion(LPCSTR pszVersion)
{
        m_strClientVersion = pszVersion;
        SboPlatform::SetIniString(SboPlatform::GetIniFilePath().c_str(),
                "Info", "ClientVersion", m_strClientVersion.GetUtf8Pointer());
}

void CMgrData::ReadIniData(void)
{
        std::string strIni = SboPlatform::GetIniFilePath();
        const char *pszIni = strIni.c_str();

        // 待ちうけポート
        m_wPort = static_cast<WORD>(SboPlatform::GetIniInt(pszIni, "Setting", "Port", 2006));
        // HTTP待ちうけポート
        m_wHttpPort = static_cast<WORD>(SboPlatform::GetIniInt(pszIni, "Setting", "HttpPort", 18080));
        // Cookieに Secure 属性を付けるか（本番はIISが443で受けるため1にする）
        m_bCookieSecure = static_cast<BOOL>(SboPlatform::GetIniInt(pszIni, "Setting", "CookieSecure", 0));
        // 管理者権限アカウント
        m_strAdminAccount = SboPlatform::GetIniString(pszIni, "Setting", "AdminAccount", "Admin").c_str();

        // クライアントバージョン
        m_strClientVersion = SboPlatform::GetIniString(pszIni, "Info", "ClientVersion", "").c_str();

}
