/// @file PasswordHash.h
/// @brief アカウントパスワードのハッシュ化ユーティリティ 定義ファイル
/// @details PBKDF2-HMAC-SHA256 (Windows CNG / bcrypt.h) を用いる。
///          Common/ 配下からは利用しないこと(ブラウザ版ビルドが壊れるため)。

#pragma once
#include <string>

namespace PasswordHash
{
	/// 既定の反復回数
	const unsigned int kDefaultIterations = 100000;

	/// 平文パスワードから保存用ハッシュ文字列を作る。
	/// @param pszPassword 平文パスワード(UTF-8想定)
	/// @return "$s1$<反復回数>$<salt_base64>$<hash_base64>" 形式の文字列。失敗時は空文字列。
	std::string	Hash(const char *pszPassword);

	/// 保存値と平文パスワードを照合する。
	/// @param pszStored DBに保存されている値(ハッシュ形式であること)
	/// @param pszPassword 照合する平文パスワード
	/// @return 一致すれば true。pszStoredがハッシュ形式でない場合は false。
	bool	Verify(const char *pszStored, const char *pszPassword);

	/// 保存値が既にハッシュ形式(先頭が "$s1$")かどうかを判定する。
	bool	IsHashed(const char *pszStored);
}
