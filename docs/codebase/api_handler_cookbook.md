# APIハンドラ追加クックブック

新しいAPIエンドポイントを追加する際の実装手順書。
既存ハンドラのパターンをそのまま流用すること。

---

## 最小実装の5ステップ

### Step 1: `Handlers/` にヘッダと実装ファイルを作成

**ファイル**: `SboSvr/src/Web/Handlers/XxxHandler.h` / `.cpp`

最小テンプレート（認証なし・`CMgrData`使用の例）:

```cpp
// XxxHandler.h
#pragma once
#include "Web/ApiHandler.h"

class CMgrData;

class CXxxHandler : public IApiHandler
{
public:
    explicit CXxxHandler(CMgrData *pMgrData);
    void Handle(const HttpRequest &request, HttpResponse &response) override;

private:
    CMgrData *m_pMgrData;
};
```

```cpp
// XxxHandler.cpp
#include "stdafx.h"
#include "XxxHandler.h"
#include <sstream>
#include "Web/JsonUtils.h"
#include "MgrData.h"

CXxxHandler::CXxxHandler(CMgrData *pMgrData)
    : m_pMgrData(pMgrData)
{
}

void CXxxHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    // ビジネスロジックを記述
    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody("{\"ok\":true}");
}
```

### Step 2: 認証チェックを実装する（必要な場合）

`AccountCreateHandler.cpp` のパターンをそのまま流用する。

```cpp
#include "Web/AuthProvider.h"

void CXxxHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    // 1. 認証チェック
    AuthProvider::AuthContext authContext;
    AuthProvider::AuthStatus authStatus =
        AuthProvider::Authenticate(request, m_pMgrData, authContext);

    if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
        response.statusLine = "HTTP/1.1 503 Service Unavailable";
        response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
        return;
    }
    if (authStatus != AuthProvider::AuthStatusOk) {
        response.statusLine = "HTTP/1.1 401 Unauthorized";
        response.SetJsonBody("{\"error\":\"unauthorized\"}");
        return;
    }
    if (!AuthProvider::HasRole(authContext, "YOUR_ROLE")) {
        response.statusLine = "HTTP/1.1 403 Forbidden";
        response.SetJsonBody(AuthProvider::BuildForbiddenBody("YOUR_ROLE"));
        return;
    }

    // 2. ビジネスロジック（認証成功後）
}
```

### Step 3: JSONレスポンスを組み立てる

外部ライブラリなし。`std::ostringstream` + `JsonUtils::Escape()` で手組みする。

```cpp
#include "Web/JsonUtils.h"
#include <sstream>

std::ostringstream oss;
oss << "{";
oss << "\"id\":" << nId << ",";
oss << "\"name\":\"" << JsonUtils::Escape(strName) << "\",";
oss << "\"enabled\":" << (bEnabled ? "true" : "false");
oss << "}";

response.statusLine = "HTTP/1.1 200 OK";
response.SetJsonBody(oss.str());
```

**注意**: 文字列フィールドは必ず `JsonUtils::Escape()` を通すこと。

### Step 4: `HttpServer.cpp` にルートを登録する

**ファイル**: `SboSvr/src/Web/HttpServer.cpp` の `RegisterDefaultHandlers()`

```cpp
// パターンA: 完全一致ルート
std::unique_ptr<IApiHandler> xxxHandler(new CXxxHandler(m_pMgrData));
m_router.Register("GET", "/api/xxx", std::move(xxxHandler));

// パターンB: プレフィックスルート（パス以降の文字列も渡す場合）
std::unique_ptr<IApiHandler> xxxHandler(new CXxxHandler(provider, "/api/xxx/prefix/"));
m_router.RegisterPrefix("GET", "/api/xxx/prefix/", std::move(xxxHandler));
```

### Step 5: インクルードを追加する

`HttpServer.cpp` の先頭インクルード群に追加:

```cpp
#include "Handlers/XxxHandler.h"
```

---

## 既存ハンドラの実装パターン比較表

| ハンドラ | 認証 | CMgrData | 共有リソース | 参考にすべき場面 |
|---------|------|---------|------------|--------------|
| `CHealthHandler` | なし | なし | なし | 最もシンプル。ヘルスチェック系 |
| `CServerInfoHandler` | なし | あり | なし | 読み取り専用・メトリクス系 |
| `CAccountCreateHandler` | あり | あり | なし | **新規APIの標準参照元**（認証+バリデーション+書き込み） |
| `CAdminRolesListHandler` | あり | あり | なし | ロール一覧返却のパターン |
| `CAdminRolesUpdateHandler` | あり | あり | なし | PUTメソッド・複数ロール検証 |
| `CMapObjectListHandler` | なし | あり | なし | 複数データのJSON配列返却 |
| `CMapPartsListHandler` | なし | あり | `shared_ptr` | 複数ハンドラでリソース共有する場合 |
| `CMapPartsSheetHandler` | なし | なし | `shared_ptr` | プレフィックスルート・バイナリレスポンス |

---

## AdminRoleCatalog の使い方

認証ロールの定義と照合は `AdminRolesHandler.h` で管理されている。

**ロールID一覧**（`g_roles[]` で定義）:

| ロールID | 説明 |
|--------|------|
| `SERVER_VIEW` | サーバーダッシュボード表示 |
| `ACCOUNT_CREATE` | 新規アカウント作成 |
| `ROLE_UPDATE` | 管理者ロール割り当ての更新 |
| `SERVER_ADMIN` | 全権限（排他制約あり、1アカウントのみ） |
| `MAP_OBJECT_VIEW` | マップオブジェクト表示 |

新しいロールを追加する場合は `AdminRolesHandler.h` の `g_roles[]` に追加する。

---

## よくあるエラー処理パターン

```cpp
// 400 Bad Request: バリデーションエラー
response.statusLine = "HTTP/1.1 400 Bad Request";
response.SetJsonBody("{\"error\":\"invalid_input\",\"message\":\"xxx is required.\"}");

// 401 Unauthorized: 未認証
response.statusLine = "HTTP/1.1 401 Unauthorized";
response.SetJsonBody("{\"error\":\"unauthorized\"}");

// 403 Forbidden: 権限不足
response.statusLine = "HTTP/1.1 403 Forbidden";
response.SetJsonBody(AuthProvider::BuildForbiddenBody("ROLE_NAME"));

// 404 Not Found: 対象リソースが存在しない
response.statusLine = "HTTP/1.1 404 Not Found";
response.SetJsonBody("{\"error\":\"not_found\"}");

// 409 Conflict: 重複など
response.statusLine = "HTTP/1.1 409 Conflict";
response.SetJsonBody("{\"error\":\"account_exists\"}");

// 503 Service Unavailable: バックエンド（LibInfo）が利用不可
response.statusLine = "HTTP/1.1 503 Service Unavailable";
response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
```

---

## JSONリクエストの読み取り方

```cpp
#include "Web/JsonUtils.h"

std::string loginId;
std::string password;
std::vector<std::string> roles;

bool bHasLogin    = JsonUtils::TryGetString(request.body, "loginId", loginId);
bool bHasPassword = JsonUtils::TryGetString(request.body, "password", password);
bool bHasRoles    = JsonUtils::TryGetStringArray(request.body, "roles", roles);

// キーの存在チェック（値の取得なし）
bool bHasEmail = (JsonUtils::FindKey(request.body, "email") != std::string::npos);

// null判定
bool bIsNull = JsonUtils::IsNull(request.body, "email");
```

---

## 関連ドキュメント

- 全体アーキテクチャ: `docs/codebase/architecture_overview.md`
- データクラス: `docs/codebase/data_class_map.md`
- OpenAPI仕様: `docs/apis/admin/README.md`
