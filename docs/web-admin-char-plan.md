# Web 管理画面 Wave 2C「キャラクター管理」実装計画書

**作成日**: 2026-04-23  
**調査対象**: SBOP2 プロジェクト Web 管理画面キャラクター関連機能  
**目的**: 旧 MFC 管理画面のキャラクター関連ダイアログ群を Web 版へ置き換えるための API・UI 設計

## 1. 旧 MFC ダイアログ一覧と概要

### 検出されたダイアログクラス（15+ファイル）
- **DlgAdminCharList**: オンラインキャラ一覧表示
- **DlgAdminCharModify**: 基本情報編集（名前、移動タイプ、性別、座標、ブロック/プッシュ）
- **DlgAdminCharModifyStatus**: ステータス編集（HP/SP、能力値6種、属性6種、移動設定）
- **DlgAdminCharModifyItem**: 所持アイテム管理
- **DlgAdminCharModifySkill**: 所持スキル管理
- **DlgAdminCharModifyGrp**: グラフィック ID 編集（15種類）
- **DlgAdminCharAccountInfo**: アカウント情報表示・変更
- **DlgAdminCharAdmin**: 管理者権限設定
- **DlgAdminCharAddNPC**: NPC 追加
- **DlgAdminCharDisable/DisableNew**: 無効化（BAN）管理

## 2. サーバー側 Char データモデル

### SQLite 正規化テーブル構成

#### sys_char（メインテーブル）
- **主キー**: CharID（INTEGER）
- **カラム数**: 約50（SaveLoadInfoChar より）

**基本情報**: CharID, MapID, MapX, MapY, Direction, MoveState, MoveType, CharName, Sex, FamilyID, GrpSize  
**HP/SP/Exp/Level**: HP, MaxHP, SP, MaxSP, Exp, Level  
**装備**: EquipItemIDCloth, Accesory1/2, ArmsRight/Left, Head  
**グラフィック（15種）**: GrpIDNPC, GrpIDCloth, GrpIDEye, GrpIDEyeColor, GrpIDHairType, GrpIDHairColor, GrpIDSP, GrpIDTmpMain/Sub, GrpIDAcce, GrpIDArmsMain/Sub, GrpIDArmsLeftMain/Sub, GrpIDInitNPC  
**基礎ステータス（5種）**: Stamina, Power, Strength, Magic, Skillful  
**計算ステータス（13種）**: AbilityAT/DF, PAtack, PDefense, PMagic, PMagicDefense, PHitAverage, PAvoidAverage, PCriticalAverage  
**属性値（6種）**: AttrFire/Wind/Water/Earth/Light/Dark  
**初期グラフィック（6種）**: GrpIDInitCloth, GrpIDInitEye, GrpIDInitEyeColor, GrpIDInitHairType/Color, GrpIDInitSP  
**移動・検索（9項）**: MaxItemCount, DropItemAverage, MoveAverage, MoveAverageBattle, MoveWait, MoveWaitBattle, SearchDistanceCX/Y, MotionTypeID  
**NPC発生（6項）**: PutCycle, PutMoveType, MaxPutCount, PutAverage, PutAreaX/Y  
**フラグ**: Block, Push, IsNPC

#### sys_char_item / sys_char_skill（サブテーブル）
- **主キー**: (CharID, Slot)
- アイテム / スキル ID リスト

## 3. 既存 Web API の現状

### 実装済み（Wave 1–2）
- `GET /api/server`, `POST /api/accounts`, `GET/PUT /api/admin/roles`
- `GET/POST/PUT /api/maps`, `GET/POST/PUT/DELETE /api/maps/events`
- `GET/POST/PUT/DELETE /api/maps/parts`, `GET /api/maps/objects` 他

### キャラクター関連 API: **完全欠落** ❌

| 機能 | 優先度 | エンドポイント |
|---|---|---|
| 一覧検索 | ⭐⭐⭐ | `GET /api/characters?name=...&accountId=...` |
| 詳細取得 | ⭐⭐⭐ | `GET /api/characters/{charId}` |
| 基本情報更新 | ⭐⭐⭐ | `PUT /api/characters/{charId}` |
| ステータス更新 | ⭐⭐⭐ | `PUT /api/characters/{charId}/status` |
| 装備更新 | ⭐⭐⭐ | `PUT /api/characters/{charId}/equipment` |
| グラフィック更新 | ⭐⭐ | `PUT /api/characters/{charId}/graphics` |
| アイテム管理 | ⭐⭐⭐ | `GET/POST/DELETE /api/characters/{charId}/items` |
| スキル管理 | ⭐⭐⭐ | `GET/POST/DELETE /api/characters/{charId}/skills` |
| アカウント管理 | ⭐⭐⭐ | `GET/PUT /api/characters/{charId}/account` |
| NPC 追加 | ⭐⭐ | `POST /api/characters/npc` |

## 4. 既存 Web UI プレースホルダー

Wave 2 セクションに既に定義:
- character-overview（基本情報編集）
- character-list（一覧表示）
- character-account（アカウント・紐付け）
- npc-management（NPC 追加）
- motion-management, skill-management（保留可）

## 5. 実装ウェーブ案

### フェーズ 1（最優先）: 検索・一覧基盤
- `GET /api/characters` — 一覧取得（フィルター対応）
- UI: character-list

### フェーズ 2: 詳細表示
- `GET /api/characters/{charId}` — 詳細取得
- UI: 詳細ビュー

### フェーズ 3: 基本編集
- `PUT /api/characters/{charId}` — 基本情報
- `PUT /api/characters/{charId}/status` — ステータス
- `PUT /api/characters/{charId}/equipment` — 装備
- `PUT /api/characters/{charId}/graphics` — グラフィック
- UI: character-overview（タブ分割）

### フェーズ 4: アイテム・スキル（並行可）
- `GET/POST/DELETE /api/characters/{charId}/items`
- `GET/POST/DELETE /api/characters/{charId}/skills`

### フェーズ 5: アカウント・NPC（並行可）
- `GET/PUT /api/characters/{charId}/account`
- `POST /api/characters/npc`

## 6. 旧ダイアログと新 UI のマッピング

| 旧 MFC | 新 Web UI | API |
|---|---|---|
| DlgAdminCharList | character-list | `GET /api/characters` |
| DlgAdminCharModify | character-overview(基本) | `PUT /api/characters/{charId}` |
| DlgAdminCharModifyStatus | character-overview(ステータス) | `PUT /api/characters/{charId}/status` |
| DlgAdminCharModifyItem | item-modal | `GET/POST/DELETE /items` |
| DlgAdminCharModifySkill | skill-modal | `GET/POST/DELETE /skills` |
| DlgAdminCharModifyGrp | character-overview(グラフィック) | `PUT /api/characters/{charId}/graphics` |
| DlgAdminCharAccountInfo | character-account | `GET/PUT /account` |
| DlgAdminCharAdmin | character-account(権限) | `PUT /admin` |
| DlgAdminCharAddNPC | npc-management | `POST /api/characters/npc` |

## 7. API 設計サマリー

### 一覧検索
```
GET /api/characters?name=...&accountId=...&status=...&limit=20&offset=0
Response: { total, limit, offset, characters: [{charId, charName, level, mapId, x, y, accountId, status}] }
```

### 詳細取得
```
GET /api/characters/{charId}
Response: { charId, charName, level, hp, maxHp, sp, maxSp, ..., equipment: {...}, graphics: {...}, status: {...}, accountId }
```

### 基本情報・ステータス・装備・グラフィック更新
```
PUT /api/characters/{charId}              → { charName, block, push, moveType, motionTypeId }
PUT /api/characters/{charId}/status       → { hp, maxHp, sp, maxSp, level, exp, stamina, ... }
PUT /api/characters/{charId}/equipment    → { cloth, accesory1/2, armsRight/Left, head }
PUT /api/characters/{charId}/graphics     → { npc, cloth, eye, eyeColor, ... (15 fields) }
```

### アイテム・スキル操作
```
GET /api/characters/{charId}/items        → [{ slot, itemId, itemName }]
POST /api/characters/{charId}/items       → { itemId } → 201
DELETE /api/characters/{charId}/items/{slot} → 204

GET /api/characters/{charId}/skills       → [{ slot, skillId, skillName }]
POST /api/characters/{charId}/skills      → { skillId } → 201
DELETE /api/characters/{charId}/skills/{slot} → 204
```

### アカウント・NPC
```
GET /api/characters/{charId}/account      → { accountId, loginId, ip, mac }
PUT /api/characters/{charId}/account      → { accountId } → 200
POST /api/characters/npc                  → { familyId, charName, mapId, x, y, moveType } → 201
```

## 8. 実装時の注意点

### サーバーサイド
1. Slot 管理方式決定（シーケンシャル vs 固定スロット）
2. グラフィック変更時の全クライアント通知機構（**未実装・保留**）  
   `PUT /api/characters/{charId}/graphics` でメモリ上の Lib オブジェクトを書き換えても、  
   オンライン中の他クライアントへのパケット通知は行われない。  
   旧 MFC 管理画面と同等の通知を実現するには、キャラクターのグラフィック変化を  
   接続中の全クライアントへブロードキャストするパケットを追加する必要があるが、  
   現行フェーズでは未解決事項として保留する。
3. キャラ削除時の副作用処理（ソフト vs ハードデリート）
4. アカウント紐付け変更の検証・同時実行制御
5. NPC 生成の制限・検証ロジック

### フロントエンド
1. フォーム差分検知（保存確認用）
2. 大量データ対応（仮想スクロール検討）
3. リアルタイム同期（他管理者による更新警告）
4. フォームバリデーション（キャラ名255字、ステータス範囲等）
5. ナビゲーション設計（詳細→編集→アイテム→スキル）

### データモデル
1. ステータス初期化ロジック（ファミリー別テンプレート）
2. グラフィック ID の妥当性チェック（DLL シート確認）
3. アカウント一意性制約（1対1 vs 1対N）

## 9. 参考ファイル

### MFC ダイアログ
`SboCliAdminMfc/src/Migrated/Dialog/DlgAdminChar*.cpp` (15+ files)

### サーバーモデル
- `SboSvr/src/Info/InfoCharSvr.h`
- `SboSvr/src/LibInfo/LibInfoCharSvr.h`
- `SboSvr/src/SaveLoad/SaveLoadInfoChar.cpp`（テーブル詳細定義）

### Web API インフラ
- `SboSvr/src/Web/ApiRouter.cpp`
- `SboSvr/src/Web/HttpServer.cpp`
- `SboSvr/src/Web/Handlers/*`

### Web UI
- `SboSvr/webroot/index.html`
- `SboSvr/webroot/app.js`

---

**実装設計の基礎資料。開発段階で調整可能。**
