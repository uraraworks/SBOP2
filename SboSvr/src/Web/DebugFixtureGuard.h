/// @file DebugFixtureGuard.h
/// @brief /api/debug/fixture (テスト準備API) の接続元判定 定義ファイル
/// @details このAPIは _DEBUG ビルドにのみ存在する(登録側で #ifdef _DEBUG する)。
///          ここではその「実行時にも loopback からの直接リクエストだけを許可する」
///          判定だけを、依存の少ない純粋関数として切り出す(テストしやすさのため)。
///          HttpTypes.h 以外へ依存しない。

#pragma once

struct HttpRequest;

namespace DebugFixtureGuard
{
	/// @brief このリクエストを /api/debug/fixture 用として受け付けてよいか判定する。
	///
	/// 条件:
	/// - HttpRequest::clientIp が 127.0.0.1 または ::1 (loopback)であること。
	///   (HttpServer::ResolveClientIp は直接の接続元がloopbackの場合に限り
	///   X-Forwarded-For の右端で上書きするため、これが偽装されたIIS越しの
	///   loopback偽装を弾く決定打にはならない。そのため下のヘッダー存在チェックを
	///   別途行う。)
	/// - X-Forwarded-For / X-Real-IP のいずれのヘッダーも存在しないこと。
	///   IIS(ARR)等のリバースプロキシ経由のリクエストにはこれらのヘッダーが
	///   付与される想定のため、存在するだけで「プロキシ経由の疑いあり」として拒否する。
	///
	/// @retval true 許可してよい(loopbackからの直接リクエスト)
	/// @retval false 拒否する(呼び出し側は404を返すこと)
	bool IsAllowedRequest(const HttpRequest &request);
}
