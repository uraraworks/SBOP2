#pragma once

#include <string>

/// @brief 管理Webのログインセッションをサーバーメモリ上で管理する。
/// SESSID Cookie にはランダムトークンのみを格納し、アカウント名やIDそのものは
/// 一切露出させない。トークン→アカウントの対応はこのモジュール内でのみ保持する。
namespace SessionStore
{
/// @brief セッション1件分の情報
struct SessionInfo
{
        unsigned int    dwAccountID;    ///< 発行対象のアカウントID（DWORD相当）
        std::string     loginId;        ///< 発行時のログインID（ログ用途。認可には使わない）
        time_t          timeExpire;     ///< 絶対有効期限（Touch成功で延長される）
};

/// @brief セッションを新規発行する。
/// @return 64文字の16進トークン。乱数生成に失敗した場合は空文字列。
std::string Create(unsigned int dwAccountID, const char *pszLoginId);

/// @brief トークンを検証し、有効なら情報を返しつつ有効期限をスライド延長する。
/// @return トークンが有効なら true。期限切れ・未登録なら false。
bool Touch(const std::string &token, SessionInfo &outInfo);

/// @brief セッションを破棄する（ログアウト等）。
void Destroy(const std::string &token);
}
