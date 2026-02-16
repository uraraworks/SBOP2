# SBOPサーバー コードベース早見表

コードを読まずに「どこに何があるか」を把握するためのリファレンス。
新しいAPIを追加する際の入り口として参照すること。

---

## プロジェクト構成（SBO.sln の9プロジェクト）

| プロジェクト | 役割 | 備考 |
|---|---|---|
| **SboSvr** | ゲームサーバー本体（メインプロセス） | HTTP API + ゲーム通信を担う |
| **SboCli** | MFC管理ツール | Web移行対象。管理画面の元実装 |
| **Common** | 共有ライブラリ | Info/LibInfo/Packet定義など |
| **myLib** | 汎用ユーティリティ | CmyString等の基本クラス |
| **SboSockLib** | TCP通信ライブラリ | UraraSockTCP実装 |
| **SboLaunch** | ゲームランチャーUI | 起動画面 |
| **SboGrpData** | グラフィックスデータ処理 | マップパーツ画像をDLLに埋め込む |
| **SboSoundData** | サウンドデータ処理 | |
| **SboCopy** | ファイルコピーツール | |

---

## 起動フロー（処理追跡の起点）

```
WinMain
└─ SboSvr/src/SboSvr.cpp

CMainFrame::MainLoop()
└─ SboSvr/src/MainFrame/MainFrame.h

    ├─ CMgrData::Create()
    │   └─ SboSvr/src/MgrData.h
    │       全LibInfoクラスをここで初期化・ロード
    │       詳細は docs/codebase/data_class_map.md 参照
    │
    ├─ CUraraSockTCPSBO   ← ゲームTCP通信（既存の仕組み）
    │
    └─ CHttpServer::Start(wHttpPort)
        └─ SboSvr/src/Web/HttpServer.h/cpp
            ポートはSboSvr.ini の [Setting] HttpPort= で指定
            デフォルト: 18080
```

---

## HTTPリクエスト処理フロー

```
ブラウザ → HTTP/1.1リクエスト

CHttpServer (SboSvr/src/Web/HttpServer.cpp)
  HandleClient() → ParseHttpRequest() でリクエスト解析
  上限: ヘッダ 8KB / ボディ 64KB

    CApiRouter::Dispatch() (SboSvr/src/Web/ApiRouter.cpp)
      完全パスマッチ または プレフィックスマッチ

        IApiHandler::Handle() (各Handlers/*.cpp)
          ├─ AuthProvider::Authenticate()  ← 認証チェック
          │   SboSvr/src/Web/AuthProvider.h
          ├─ CMgrData経由でLibInfoにアクセス
          │   例: m_pMgrData->GetLibInfoAccount()
          └─ response.SetJsonBody("{...}")  ← レスポンス送信
```

---

## 実装済みAPIエンドポイント一覧

| メソッド | パス | ハンドラクラス | 認証ロール |
|---------|------|--------------|----------|
| GET | `/health` | `CHealthHandler` | 不要 |
| GET | `/api/server` | `CServerInfoHandler` | 不要（現状） |
| POST | `/api/accounts` | `CAccountCreateHandler` | `ACCOUNT_CREATE` |
| GET | `/api/admin/roles` | `CAdminRolesListHandler` | `ROLE_UPDATE` |
| PUT | `/api/admin/roles` | `CAdminRolesUpdateHandler` | `ROLE_UPDATE` |
| GET | `/api/maps/objects` | `CMapObjectListHandler` | 不要（現状） |
| GET | `/api/maps/parts` | `CMapPartsListHandler` | 不要（現状） |
| GET | `/api/assets/map-parts/sheets/{n}` | `CMapPartsSheetHandler` | 不要（現状） |
| GET | `/`（プレフィックス） | `CStaticFileHandler` | 不要 |

ルート登録箇所: `SboSvr/src/Web/HttpServer.cpp` の `RegisterDefaultHandlers()`

---

## Web関連ディレクトリ構成

```
SboSvr/src/Web/
├── HttpServer.h/cpp        サーバー本体（Winsock2スレッド、接続受付・送受信）
├── ApiRouter.h/cpp         ルーティングテーブル（Register/RegisterPrefix）
├── ApiHandler.h            IApiHandlerインターフェース（純粋仮想Handle()のみ）
├── HttpTypes.h/cpp         HttpRequest/HttpResponse構造体
├── AuthProvider.h/cpp      認証・認可（CookieのSESSID検証 + ロールチェック）
├── JsonUtils.h/cpp         JSON補助（Escape/TryGetString/TryGetStringArray等）
└── Handlers/
    ├── HealthHandler.h/cpp          GET /health（最シンプルな実装例）
    ├── ServerInfoHandler.h/cpp      GET /api/server
    ├── AccountCreateHandler.h/cpp   POST /api/accounts（認証・バリデーションの手本）
    ├── AdminRolesHandler.h/cpp      GET,PUT /api/admin/roles（2クラスが1ファイルに）
    ├── MapObjectHandler.h/cpp       GET /api/maps/objects
    ├── MapPartsHandler.h/cpp        GET /api/maps/parts + シート画像（共有リソースの例）
    └── StaticFileHandler.h/cpp      GET /（webroot配信）
```

---

## フロントエンドファイル構成

```
SboSvr/webroot/
├── index.html   SPAのHTML本体（539行）。data-view属性でビュー管理
├── app.js       全UI処理（1906行）。フレームワーク不使用のバニラJS
└── styles.css   スタイル（779行）
```

フロントエンドの構造詳細は `docs/codebase/frontend_structure.md` 参照。

---

## 設定ファイル

| ファイル | 場所 | 主要設定 |
|---|---|---|
| `SboSvr.ini` | `Release/` または実行ファイルと同じ場所 | `[Setting]` に Port, HttpPort |
| `SboCliInit.ini` | `Release/` | クライアント初期設定 |

---

## 未実装のAPIビュー（今後の開発対象）

Wave 2: キャラ一覧/編集、NPC追加、スキル一覧/編集、マップ情報/イベント編集、アイテム/武器/エフェクト一覧
Wave 3: 監査レポート、操作履歴、フェイルバック手順

詳細は `docs/admin_phase_d_transition_plan.md` 参照。
