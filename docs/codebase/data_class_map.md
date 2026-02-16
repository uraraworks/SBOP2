# データクラス完全マップ

バックエンドのLibInfo/Infoクラス体系を把握するためのリファレンス。
新しいAPIで既存データにアクセスする際に参照すること。

---

## CMgrData からのアクセス方法一覧

`CMgrData` (SboSvr/src/MgrData.h) が全LibInfoへの入り口。
ハンドラクラスは `m_pMgrData->GetLibInfoXxx()` でアクセスする。

| ゲッター | 返すクラス | 管理するデータ | ヘッダファイル |
|---------|-----------|------------|-------------|
| `GetLibInfoAccount()` | `CLibInfoAccount` | アカウント・ユーザー | `SboSvr/src/LibInfo/LibInfoAccount.h` |
| `GetLibInfoChar()` | `CLibInfoCharSvr` | サーバー側キャラクター | `SboSvr/src/LibInfo/LibInfoCharSvr.h` |
| `GetLibInfoDisable()` | `CLibInfoDisable` | ログイン拒否リスト | `Common/LibInfo/LibInfoDisable.h` |
| `GetLibInfoMap()` | `CLibInfoMapBase` | マップ情報 | `Common/LibInfo/LibInfoMapBase.h` |
| `GetLibInfoMapObject()` | `CLibInfoMapObject` | マップオブジェクト定義 | `Common/LibInfo/LibInfoMapObject.h` |
| `GetLibInfoMapParts()` | `CLibInfoMapParts` | マップパーツ（タイル） | `Common/LibInfo/LibInfoMapParts.h` |
| `GetLibInfoMapShadow()` | `CLibInfoMapShadow` | マップ影情報 | `Common/LibInfo/LibInfoMapShadow.h` |
| `GetLibInfoItemType()` | `CLibInfoItemType` | アイテム種別 | `Common/LibInfo/LibInfoItemType.h` |
| `GetLibInfoItem()` | `CLibInfoItem` | アイテム | `Common/LibInfo/LibInfoItem.h` |
| `GetLibInfoItemWeapon()` | `CLibInfoItemWeapon` | 武器 | `Common/LibInfo/LibInfoItemWeapon.h` |
| `GetLibInfoEffect()` | `CLibInfoEffect` | エフェクト | `Common/LibInfo/LibInfoEffect.h` |
| `GetLibInfoMotion()` | `CLibInfoMotion` | モーション | `Common/LibInfo/LibInfoMotion.h` |
| `GetLibInfoMotionType()` | `CLibInfoMotionType` | モーション種別 | `Common/LibInfo/LibInfoMotionType.h` |
| `GetLibInfoEfcBalloon()` | `CLibInfoEfcBalloon` | 噴出し（吹き出し） | `Common/LibInfo/LibInfoEfcBalloon.h` |
| `GetLibInfoSystem()` | `CLibInfoSystem` | システム設定 | `Common/LibInfo/LibInfoSystem.h` |
| `GetLibInfoSkill()` | `CLibInfoSkill` | スキル | `Common/LibInfo/LibInfoSkill.h` |
| `GetLibInfoTalkEvent()` | `CLibInfoTalkEvent` | 会話イベント | `Common/LibInfo/LibInfoTalkEvent.h` |
| `GetInfoFileList()` | `CInfoFileList` | ファイルリスト | `SboSvr/src/` 以下 |

---

## LibInfoBase の共通インターフェース

全LibInfoクラスが持つ基本メソッド（`Common/LibInfo/LibInfoBase.h`）。

```cpp
// 作成・破棄
void  Create();
void  Destroy();

// データ件数の取得
int   GetCount();

// インデックスからデータ取得（0始まり）
CInfoXxx*  GetPtr(int nNo);

// IDからデータ取得（IDの型はLibInfoによって異なる）
CInfoXxx*  GetPtr(DWORD dwId);
CInfoXxx*  GetPtr(LPCSTR pszKey);   // 文字列キーの場合

// 追加・削除
BOOL  Add(CInfoXxx *pInfo);
BOOL  Delete(int nNo);
void  DeleteAll();

// IDマップ再構築（Add/Delete後に呼ぶ場合がある）
void  RenewIDPtr();
```

---

## LibInfoクラスと対応するInfoクラスのペア

| LibInfoクラス | Infoクラス（1件分） | Infoヘッダパス |
|---|---|---|
| `CLibInfoAccount` | `CInfoAccount` | `Common/Info/InfoAccount.h` |
| `CLibInfoCharSvr` | `CInfoCharSvr` | `SboSvr/src/Info/InfoCharSvr.h` |
| `CLibInfoMapBase` | `CInfoMapBase` | `Common/Info/InfoMapBase.h` |
| `CLibInfoMapObject` | `CInfoMapObject` | `Common/Info/InfoMapObject.h` |
| `CLibInfoMapParts` | `CInfoMapParts` | `Common/Info/InfoMapParts.h` |
| `CLibInfoItemType` | `CInfoItemType` | `Common/Info/InfoItemType.h` |
| `CLibInfoItem` | `CInfoItem` | `Common/Info/InfoItem.h` |
| `CLibInfoItemWeapon` | `CInfoItemWeapon` | `Common/Info/InfoItemWeapon.h` |
| `CLibInfoEffect` | `CInfoEffect` | `Common/Info/InfoEffect.h` |
| `CLibInfoMotion` | `CInfoMotion` | `Common/Info/InfoMotion.h` |
| `CLibInfoSkill` | `CInfoSkill` | `Common/Info/InfoSkill.h` |
| `CLibInfoTalkEvent` | `CInfoTalkEvent` | `Common/Info/InfoTalkEvent.h` |

---

## CInfoAccount の主要フィールド

アカウント管理APIで頻繁に参照する (`Common/Info/InfoAccount.h`)。

```cpp
DWORD    m_dwAccountID;       // アカウントID
DWORD    m_dwCharID;          // 使用中のキャラID
DWORD    m_dwTimeLastLogin;   // 前回ログイン日時（UNIX時間）
DWORD    m_dwTimeMakeAccount; // アカウント作成日時（UNIX時間）
DWORD    m_dwLoginCount;      // ログイン回数
int      m_nAdminLevel;       // 管理者レベル（0=一般、1以上=管理者）
CmyString m_strAccount;       // アカウント名（ログインID）
CmyString m_strPassword;      // パスワード
CmyString m_strMacAddr;       // MAC アドレス
ARRAYDWORD m_adwCharID;       // キャラIDテーブル（複数キャラ対応）
```

---

## データ永続化（保存・読み込み）

各Infoクラスの保存・読み込みは `SaveLoad/` ディレクトリで管理。

```
SboSvr/src/SaveLoad/
SboCli/src/SaveLoad/
Common/SaveLoad/（共通部分）
```

新しいフィールドを追加した場合は対応する `SaveLoadInfoXxx.cpp` を修正する。
APIで書き込みが必要な場合は必ず `CMgrData::Save()` を呼ぶこと。

---

## SboSvr固有 vs Common の使い分け

| 場所 | 含まれるクラス | 理由 |
|---|---|---|
| `Common/LibInfo/` | マップ・アイテム・スキル等 | クライアント（SboCli）とサーバーで共有 |
| `SboSvr/src/LibInfo/` | `CLibInfoCharSvr`, `CLibInfoDisable` | サーバー専用の追加情報を持つ |
| `Common/Info/` | 全Infoクラスの基底 | クライアントとサーバーで共有 |
| `SboSvr/src/Info/` | `CInfoCharSvr` | サーバー専用の状態管理フィールドを持つ |
