#pragma once

// ------------------------------------------------------------
// 監査ログ（Web 管理画面の変更系操作の記録）
//
// 管理画面から発行された POST/PUT/DELETE 系 API の成功リクエストを
// メモリ上のリングバッファに記録する簡易実装。
//
// 本格運用では SQLite や外部ストレージへの永続化を予定しているが、
// 最小実用セットとして in-memory 実装から始める。
// 詳細は docs/web-admin-migration-status.md の「監査ログ」節参照。
// ------------------------------------------------------------

#include <string>
#include <vector>
#include <deque>

class CCriticalSection;

namespace AuditLog
{
struct Entry
{
        Entry();

        unsigned long long      id;             // 連番 ID（サーバー起動後の通し番号）
        std::string             timestamp;      // ISO8601 UTC 文字列 ("YYYY-MM-DDTHH:MM:SSZ")
        std::string             actor;          // 実行者 loginId（未認証リクエストは "-"）
        std::string             accountId;      // 実行者 accountId
        int                     adminLevel;     // 実行者の管理レベル
        std::string             method;         // HTTP メソッド（POST/PUT/DELETE）
        std::string             path;           // リクエストパス（クエリ含む）
        int                     statusCode;     // レスポンスステータスコード
        std::string             bodyDigest;     // リクエストボディの要約（最大 256 文字）
        size_t                  bodySize;       // 元のリクエストボディバイト数
};

// 監査対象かどうかを判定（POST/PUT/DELETE かつ /api/ 配下）
// /api/selection/* と /api/audit-logs* は記録対象外（ノイズ除去）
bool ShouldAudit(const std::string &method, const std::string &path);

// 記録
void Record(
        const std::string &method,
        const std::string &path,
        int statusCode,
        const std::string &actor,
        const std::string &accountId,
        int adminLevel,
        const std::string &requestBody);

// 一覧取得（新しい順）。フィルタは空文字で無効扱い。
// pathPrefix は前方一致、actor/method は完全一致（大文字小文字区別あり）。
// maxCount が 0 なら全件返す。
std::vector<Entry> Query(
        const std::string &actorFilter,
        const std::string &methodFilter,
        const std::string &pathPrefixFilter,
        size_t maxCount);

// 現在保持している件数
size_t Size();

// 最大保持件数（デフォルト 500）
size_t Capacity();
}
