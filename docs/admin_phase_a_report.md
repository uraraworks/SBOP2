# フェーズA成果サマリー（現状調査と要件定義）

## 1. 管理機能棚卸し一覧
以下の表は既存 MFC 管理画面の主な機能と Web 化時の想定用途を整理したものです。主なデータ取得元は `CMgrData` もしくは関連ライブラリ経由で参照されます。

| カテゴリ | 機能/画面 | 主な操作内容 | 主なデータソース | 備考 |
| --- | --- | --- | --- | --- |
| マップ | マップ情報編集 (`DlgAdminMapInfo`) | マップ名・サイズ・BGM・天候の更新 | `CMgrData::GetMap()` → `InfoMapBase` | マップ更新通知 `ADMINMSG_RENEWMAPINFO` を受信し UI 更新【F:SboCli/src/Dialog/DlgAdminMapInfo.cpp†L12-L120】 |
| マップ | マップイベント一覧 (`DlgAdminMapEvent`) | イベントの追加・編集・削除、種別確認 | `InfoMapBase::m_pLibInfoMapEvent`、`CMgrData::GetMapEventName()` | イベント一覧をリスト表示し、クリック通知を他ダイアログへ転送【F:SboCli/src/Dialog/DlgAdminMapEvent.cpp†L12-L118】 |
| マップ | マップパーツ編集 (`DlgAdminMapParts` 系) | パーツ種類・レイヤー設定・属性閲覧 | `CMgrData::GetMgrGrpData()` 経由でパーツリソースを取得 | マップレイヤーとテクスチャ管理が中心【F:SboCli/src/Dialog/DlgAdminMapPartsEdit.cpp†L180-L214】 |
| マップ | マップオブジェクト配置 (`DlgAdminMapObject`, `DlgAdminMapObjectData`) | オブジェクト位置・情報の確認と配置操作 | `CMgrData::GetMap()` から現在マップ状態を取得 | 位置座標とオブジェクト種別を双方向編集【F:SboCli/src/Dialog/DlgAdminMapObjectData.cpp†L12-L140】 |
| キャラクター | 選択キャラ編集（基本・ステータス・画像・所持品・スキル） | キャラ属性、移動種別、装備、会話データを更新 | `CMgrData::GetInfoChar()` 系、`LibInfoMotionType`、`PacketADMIN_CHARINFO` | 複数ダイアログでキャラ構造体 (`InfoCharCli`) を編集【F:SboCli/src/Dialog/DlgAdminCharModify.cpp†L12-L129】 |
| キャラクター | キャラ一覧 (`DlgAdminCharList`) | キャラ検索、ID 選択、状況確認 | `CMgrData::GetLibInfoCharCli()` | 選択結果を他ダイアログへ連携【F:SboCli/src/Dialog/DlgAdminCharList.cpp†L12-L140】 |
| キャラクター | 管理者権限設定 (`DlgAdminCharAdmin`) | 権限フラグ切替 | `CMgrData::GetInfoChar()`、`PacketADMIN_PARA2` | サーバーへ権限変更要求を送信【F:SboCli/src/Dialog/DlgAdminCharAdmin.cpp†L12-L92】 |
| キャラクター | NPC 追加 (`DlgAdminCharAddNPC`) | NPC スポーン、テンプレ選択 | `CMgrData::GetInfoChar()`、`PacketADMIN_PARA2` | マップ座標とテンプレ ID 指定【F:SboCli/src/Dialog/DlgAdminCharAddNPC.cpp†L12-L110】 |
| キャラクター | モーション編集 (`DlgAdminCharMotionTypeList`) | モーション種別設定 | `LibInfoMotionType`、`CMgrData::GetLibInfoMotionType()` | モーション一覧の再構成【F:SboCli/src/Dialog/DlgAdminCharMotionTypeList.cpp†L12-L100】 |
| キャラクター | アカウント情報編集 (`DlgAdminCharAccountInfo`) | アカウント紐付け・状態変更 | `PacketADMIN_PARA2`、`CMgrData` 内のアカウントキャッシュ | 認証関連情報を扱うため Web 化時は権限管理が重要【F:SboCli/src/Dialog/DlgAdminCharAccountInfo.cpp†L12-L104】 |
| キャラクター | スキル一覧/編集 (`DlgAdminCharSkillList`, `DlgAdminCharSkill`) | 所持スキル確認と変更 | `LibInfoSkill`、`PacketADMIN_CHARINFO` | スキルマスタ参照が必要【F:SboCli/src/Dialog/DlgAdminCharSkillList.cpp†L12-L122】 |
| アイテム | 種別一覧 (`DlgAdminItemTypeList`) | アイテム分類管理 | `CMgrData::GetLibInfoItemType()` | 種別マスタへの CRUD【F:SboCli/src/Dialog/DlgAdminItemTypeList.cpp†L12-L118】 |
| アイテム | アイテム一覧 (`DlgAdminItemList`) | マップ上アイテムの確認・生成 | `CMgrData::GetMapID()`、`PacketADMIN_ITEM_LIST` | マップ依存情報をサーバーから取得【F:SboCli/src/Dialog/DlgAdminItemList.cpp†L12-L148】 |
| アイテム | 武器一覧 (`DlgAdminItemWeaponList`) | 武器パラメータ確認 | `LibInfoWeapon` | ステータス比較機能あり【F:SboCli/src/Dialog/DlgAdminItemWeaponList.cpp†L12-L116】 |
| エフェクト | 噴出し一覧 (`DlgAdminEfcBalloonList`) | メッセージ吹き出し定義編集 | `LibInfoEfcBalloon` | 表示プレビュー機能を持つ【F:SboCli/src/Dialog/DlgAdminEfcBalloonList.cpp†L12-L108】 |
| エフェクト | エフェクト一覧 (`DlgAdminEfcEffectList`) | エフェクトパラメータ調整 | `LibInfoEfcEffect` | アニメーション再生設定【F:SboCli/src/Dialog/DlgAdminEfcEffectList.cpp†L12-L132】 |
| システム | サーバー情報 (`DlgAdminServerInfo`) | 接続中サーバーの状態表示 | `PacketADMIN_PARA2` 応答 | Web 版ではステータス API が必要【F:SboCli/src/Dialog/DlgAdminServerInfo.cpp†L12-L96】 |
| システム | 初期ステータス設定 (`DlgAdminSystemSetInitCharStatus`) | 初期値テンプレ編集 | `LibInfoInitCharStatus` | ゲーム開始時パラメータを編集【F:SboCli/src/Dialog/DlgAdminSystemSetInitCharStatus.cpp†L12-L112】 |
| アカウント | アカウント追加 (`DlgAdminAccountAdd`) | 新規アカウント登録 | `PacketADMIN_ACCOUNT_ADD` | 認証情報の入力を伴う【F:SboCli/src/Dialog/DlgAdminAccountAdd.cpp†L12-L104】 |

## 2. データソースと依存関係
- `CMgrData` が中心的なデータハブとなり、マップ・キャラクタ・アイテム・エフェクトなど各種ライブラリ (`LibInfo*`) へのアクセスを提供する。HTTP API 化では `SboSvr` 側に同等のデータ取得・更新用エンドポイントを定義する必要がある。
- マップ関連機能は `InfoMapBase` とその内部の `LibInfoMapEvent`、`MgrGrpData` に強く依存しており、ファイルベースのリソース（マップパーツ、影データなど）とサーバー上の状態情報が混在している。Web 化では静的リソースの配信と動的 API の役割を分離する。
- キャラクター機能は `InfoCharCli` 構造体や `LibInfoMotionType`、`LibInfoSkill` など複数のマスタに依存するほか、`PacketADMIN_*` でサーバー連携している。セッション管理と権限モデルの再設計が必須となる。
- アイテム・エフェクト関連はマスタ参照が中心であり、Web UI ではリスト取得 API と編集 API を整備することで置き換えが可能。
- サーバー情報・初期ステータス・アカウント管理はセキュリティ観点での優先度が高い。HTTPS・多要素認証・操作ログの検討が必要。

## 3. 優先度評価
| 優先度 | 対象機能 | 評価理由 |
| --- | --- | --- |
| 高 | サーバー情報、アカウント追加、管理者権限設定 | 運用上必須で即時性が求められるため。Web 管理画面移行時の最小機能セットとして実装が必要。 |
| 中 | キャラ編集（基本情報・所持情報）、マップ情報、マップイベント | 運用頻度が高く、既存運用フローでの利用が多いが、当面は既存 MFC 併用も可能。 |
| 中 | アイテム一覧、マップオブジェクト配置 | GM 作業で重要だが緊急性は中程度。 |
| 低 | エフェクト一覧、マップパーツ編集、武器一覧 | 更新頻度が低く、後回しでも運用に影響が限定的。 |

## 4. Web 管理画面の非機能要件（暫定）
- **同時接続数**: 運用担当 5～10 名を想定し、10 セッション同時接続での安定稼働を目標。
- **応答時間**: CRUD 系 API は 500ms 以内、リスト取得は 1,000 件で 1 秒以内を目標。
- **可用性**: SboSvr 稼働中は常時利用可能とし、HTTP サーバー障害時は即時フェイルオーバー（再起動手順）を整備。
- **認証方式**: 社内 SSO 連携またはアクセストークン＋IP 制限を想定。多要素認証をオプションとして検討。
- **利用ブラウザ**: 最新版 Chrome / Edge をサポート対象とし、IE は対象外。
- **監査ログ**: 全ての管理操作についてユーザーID・操作内容・結果・タイムスタンプを記録。
- **通信保護**: HTTPS (TLS1.2 以上) を必須とし、証明書管理手順を別途整備。

## 5. 引き継ぎ用ドキュメントテンプレート案
1. **概要**: 機能の目的、対象範囲、画面キャプチャ。
2. **API 一覧**: エンドポイント、メソッド、リクエスト/レスポンススキーマ、認可要件。
3. **データモデル**: 参照するマスタやテーブル、依存するリソースファイル。
4. **フロントエンド仕様**: UI コンポーネント構成、バリデーション仕様、エラーハンドリング。
5. **テスト観点**: 正常系シナリオ、異常系（認証エラー、バリデーション失敗等）、性能要件。
6. **運用手順**: デプロイ方法、ロールバック手順、フェイルオーバー手順、問い合わせ窓口。
7. **未解決課題**: 技術的課題、依存タスク、将来的な改善案。

## 6. 次アクション
- フェーズB（アーキテクチャ設計）では、上記優先度「高」の機能群を対象に HTTP API の境界を定義し、既存 `PacketADMIN_*` 通信の置き換え方針を決定する。
- 認証基盤検討のため、社内 SSO との連携可否調査と代替案（トークンベース認証）を比較検討する。
- Web UI プロトタイプ作成に向け、最優先画面（サーバー情報、アカウント管理）のワイヤーフレームを作成し、関係者レビューを実施する。
