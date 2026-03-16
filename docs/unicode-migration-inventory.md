# Unicode/UTF-8 移行 棚卸しメモ

## 結論
- ソースファイルを UTF-8 に統一するだけでは文字化け問題は解決しない
- このプロジェクトでは「ファイル保存形式」「ビルド時の解釈」「実行時の変換」の 3 層を揃える必要がある
- 現状はプロジェクト設定自体は `Unicode` 化されているが、CP932 前提の変換と `.rc` のコードページ混在が残っている

## 現状整理

### 1. プロジェクト設定
- 多くの `.vcxproj` はすでに `CharacterSet=Unicode`
- 例:
  - [SboCli.vcxproj](/c:/Users/masakazu/MyProject/SBOP2/SboCli/SboCli.vcxproj)
  - [SboSvr.vcxproj](/c:/Users/masakazu/MyProject/SBOP2/SboSvr/SboSvr.vcxproj)
  - [SboLaunch.vcxproj](/c:/Users/masakazu/MyProject/SBOP2/SboLaunch/SboLaunch.vcxproj)

### 2. リソースファイル
- `.rc` は UTF-8 と CP932 が混在
- [SboCli.rc](/c:/Users/masakazu/MyProject/SBOP2/SboCli/SboCli.rc) は先頭側で `#pragma code_page(65001)`、末尾側で `#pragma code_page(932)` が共存している
- まだ CP932 固定の `.rc` もある
  - [SboLaunch.rc](/c:/Users/masakazu/MyProject/SBOP2/SboLaunch/SboLaunch.rc)
  - [SboGrpData.rc](/c:/Users/masakazu/MyProject/SBOP2/SboGrpData/SboGrpData.rc)
  - [SboSoundData.rc](/c:/Users/masakazu/MyProject/SBOP2/SboSoundData/SboSoundData.rc)
  - [MakeFileList.rc](/c:/Users/masakazu/MyProject/SBOP2/Tool/MakeFileList/MakeFileList.rc)
- 一方で [SboSvr.rc](/c:/Users/masakazu/MyProject/SBOP2/SboSvr/SboSvr.rc) は `#pragma code_page(65001)`

### 3. 共通文字列変換
- 文字コード移行の中心は [myString.h](/c:/Users/masakazu/MyProject/SBOP2/Common/myLib/myString.h) と [myString.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/myLib/myString.cpp)
- 良い点:
  - `Utf8ToTString`
  - `TStringToUtf8`
  - `RenewUtf8`
  - UTF-8 キャッシュ
- 問題点:
  - `GetAnsiPointer(UINT codePage = 932)`
  - `GetStoreLength(UINT codePage = 932)`
  - `operator LPCSTR()` が実質 CP932 に寄っている
  - `Utf8ToTString()` が UTF-8 失敗時に `932` と `CP_ACP` へフォールバックしている
- この設計は「移行途中の互換維持」には有効だが、「不正な UTF-8 を黙って CP932 扱いする」ため原因追跡を難しくする

### 4. 共通保存/復元処理
- [SBOGlobal.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/SBOGlobal.cpp) に CP932 前提の保存/復元が残っている
- 例:
  - `TStringToAnsi(..., 932)`
  - `AnsiToTString(..., 932)`
- この層はパケット/保存形式に広く影響するため、最初に全面変更すると影響範囲が大きい

### 5. データモデルの読込
- `Common/Info/*` 配下で、バイナリや保存データから文字列を読む箇所が CP932 前提
- 主な例:
  - [InfoAccount.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/Info/InfoAccount.cpp)
  - [InfoCharBase.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/Info/InfoCharBase.cpp)
  - [InfoMapBase.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/Info/InfoMapBase.cpp)
  - [InfoMapObject.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/Info/InfoMapObject.cpp)
  - [InfoItem.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/Info/InfoItem.cpp)
  - [InfoItemWeapon.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/Info/InfoItemWeapon.cpp)
  - [InfoItemTypeBase.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/Info/InfoItemType/InfoItemTypeBase.cpp)
  - [InfoSkillBase.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/Info/InfoSkill/InfoSkillBase.cpp)
  - [InfoEffect.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/Info/InfoEffect.cpp)
  - [InfoEfcBalloon.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/Info/InfoEfcBalloon.cpp)
  - [InfoTalkEventBase.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/Info/InfoTalkEvent/InfoTalkEventBase.cpp)
  - [InfoDisable.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/Info/InfoDisable.cpp)
- これらは「既存データ形式が CP932 のまま」なのか「単に旧実装が残っているだけ」なのかを切り分ける必要がある

### 6. Web/API 側
- 新しめの Web ハンドラは UTF-8 を意識して実装されている
- 代表例: [MapObjectHandler.cpp](/c:/Users/masakazu/MyProject/SBOP2/SboSvr/src/Web/Handlers/MapObjectHandler.cpp)
- 良い点:
  - JSON 出力を UTF-8 へ統一
  - `CP_UTF8` を優先
- 残課題:
  - 非 Unicode ビルド時に `932` へフォールバックする分岐が残る
  - 互換運用としては妥当だが、最終形では不要化したい

## 文字化けの原因パターン

### A. 表示だけが化ける
- ファイルは UTF-8 だが、端末やエディタが CP932 として表示
- 例:
  - `Get-Content` では化けるが、別の読み方では正常に見える

### B. ビルド時に化ける
- ソースは UTF-8 だが、コンパイラやリソースコンパイラが CP932 前提で読む
- `.rc` の `#pragma code_page(...)` 混在が代表例

### C. 実行時に化ける
- 内部は Unicode でも、保存・通信・ロード時に `932` 変換して壊れる
- `AnsiToTString(..., 932)` や `TStringToAnsi(..., 932)` が代表例

## 安全な移行順

### Phase 1: ソースとリソースの保存形式を揃える
- 対象:
  - `.cpp`
  - `.h`
  - `.rc`
  - `.rc2`
  - `.vcxproj`
  - `.filters`
  - `.md`
- 方針:
  - UTF-8 に統一
  - `.rc` は `code_page(65001)` へ整理
- 注意:
  - `.rc2` や古いバイナリ由来のテキストは先にバックアップ

### Phase 2: `.rc` のコードページ混在を解消
- 優先対象:
  - [SboCli.rc](/c:/Users/masakazu/MyProject/SBOP2/SboCli/SboCli.rc)
  - [SboLaunch.rc](/c:/Users/masakazu/MyProject/SBOP2/SboLaunch/SboLaunch.rc)
  - [SboGrpData.rc](/c:/Users/masakazu/MyProject/SBOP2/SboGrpData/SboGrpData.rc)
  - [SboSoundData.rc](/c:/Users/masakazu/MyProject/SBOP2/SboSoundData/SboSoundData.rc)
  - [MakeFileList.rc](/c:/Users/masakazu/MyProject/SBOP2/Tool/MakeFileList/MakeFileList.rc)
- 目的:
  - リソースコンパイラの解釈を UTF-8 に統一

### Phase 3: 共通変換のルールを固定
- 対象:
  - [myString.h](/c:/Users/masakazu/MyProject/SBOP2/Common/myLib/myString.h)
  - [myString.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/myLib/myString.cpp)
  - [SBOGlobal.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/SBOGlobal.cpp)
- 目標:
  - 内部表現は `TCHAR/UTF-16`
  - 外部テキストは原則 UTF-8
  - CP932 は「旧データ読込専用」に限定
- 具体策:
  - `operator LPCSTR()` の既定挙動を見直す
  - `GetAnsiPointer()` の既定コードページを再検討
  - `Utf8ToTString()` の CP932 フォールバックを段階的に縮小

### Phase 4: 旧データ/旧プロトコルを明示的に隔離
- 対象:
  - `Common/Info/*`
  - 保存/ロード
  - パケット化/復元
- 方針:
  - 「旧フォーマットだから CP932」を明示コメント付きで残す
  - それ以外の新規経路は UTF-8 固定にする

### Phase 5: Web/API とゲーム本体の境界を UTF-8 固定にする
- 対象:
  - `SboSvr/src/Web/Handlers/*`
  - 将来の SDL 側設定ファイルやログ
- 目標:
  - HTTP/JSON は UTF-8 固定
  - GUI/内部は Unicode
  - CP932 変換を境界から排除

## 先に手を付けるべき箇所
- 1. [SboCli.rc](/c:/Users/masakazu/MyProject/SBOP2/SboCli/SboCli.rc) の `65001/932` 混在解消
- 2. [myString.h](/c:/Users/masakazu/MyProject/SBOP2/Common/myLib/myString.h) の既定コードページ依存の明確化
- 3. [SBOGlobal.cpp](/c:/Users/masakazu/MyProject/SBOP2/Common/SBOGlobal.cpp) の CP932 固定保存/復元の棚卸し
- 4. `Common/Info/*` の `AnsiToTString(..., 932)` を「旧形式のみ」か「現行経路も依存」かで分類

## 実装方針の提案
- 内部文字列:
  - `CString` / `CmyString` / `TCHAR` を維持
- 新規入出力:
  - UTF-8
- 旧資産読込:
  - 必要箇所のみ CP932
- 判断基準:
  - 「今後新しく生成されるもの」は UTF-8
  - 「既存保存資産との互換が必要なもの」は暫定的に CP932 読込を残す

## 次の具体作業
- `.rc` のコードページ統一から着手する
- その後 `myString` / `SBOGlobal` の既定経路を UTF-8 優先へ寄せる
- 最後に `Common/Info/*` の旧データ互換層を明示化する
