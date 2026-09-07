/// @file PasswordHash.h
/// @brief アカウントパスワードのハッシュ化ユーティリティ 定義ファイル
/// @details PBKDF2-HMAC-SHA256 (RFC 8018 準拠の自前実装。外部ライブラリ・
///          OS API 不使用)を用いる。
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

	/// パスワードに使える文字かどうかを判定する。
	///
	/// 許可するのは ASCII の表示可能文字(0x21〜0x7E)のみ。
	/// 全角文字・空白・制御文字は不可。
	///
	/// PasswordHash は変換せず生バイトを扱うため、全角を許すと
	/// 呼び出し側のエンコーディング次第で同じパスワードが別のハッシュに
	/// なりうる。入口で弾いておくのが確実。
	///
	/// **新規作成・変更時にのみ使うこと。** 既存アカウントの照合(Verify)に
	/// 掛けてはいけない。過去に全角で登録されたアカウントが在るかは
	/// ハッシュからは判別できず、掛けると締め出しになる。
	///
	/// @param pszPassword 平文パスワード(UTF-8想定)
	/// @return 使用可能なら true。空文字列や NULL は false。
	bool	IsAcceptable(const char *pszPassword);

	/// 保存値が既にハッシュ形式(先頭が "$s1$")かどうかを判定する。
	bool	IsHashed(const char *pszStored);

	/// SHA-256 ダイジェストを16進文字列で返す(テスト用に公開)
	std::string	Sha256Hex(const unsigned char *pData, size_t nLen);

	/// HMAC-SHA256 ダイジェストを16進文字列で返す(テスト用に公開)
	std::string	HmacSha256Hex(
		const unsigned char *pKey, size_t nKeyLen,
		const unsigned char *pData, size_t nDataLen);
}
