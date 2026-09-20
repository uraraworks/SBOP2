/// @file LoginCode.h
/// @brief ログインコード・端末トークンの生成/正規化/ハッシュ 定義ファイル
/// @details docs/login-code-auth-plan.md の「コードとトークンの仕様」節を実装する。
///          SQLite やアカウントライブラリには一切依存しない、純粋なロジックだけを
///          切り出したモジュール(テストしやすさのため)。
///          乱数生成は SboPlatform::GenerateRandomBytes、ハッシュは
///          PasswordHash::Sha256Hex を使う(どちらも SboSvr 内限定)。

#pragma once

#include <string>

namespace LoginCode
{
	/// コードの文字数(ハイフン抜き)
	const size_t kCodeLength = 16;

	/// 端末トークンのバイト長(16進表記で64文字になる)
	const size_t kDeviceTokenBytes = 32;

	/// @brief 新しいログインコードを生成する(ハイフン無し、16文字、大文字)。
	/// @return 失敗時(乱数生成エラー)は空文字列。
	std::string	GenerateCode(void);

	/// @brief コードを4文字ごとにハイフンで区切った表示用文字列にする。
	/// @param strCode NormalizeCode() 済みの16文字のコード
	std::string	FormatCodeForDisplay(const std::string &strCode);

	/// @brief 利用者が入力したコードを正規化する。
	///
	/// - ハイフン・空白を除去する。
	/// - 大文字にそろえる。
	/// - O→0、I/L→1 に同一視する。
	/// - 上記処理後、Crockford Base32 の32文字(0-9, A-Z から I,L,O,U を除いたもの)
	///   のみ・長さ16文字であることを検証する。
	///
	/// @param strInput 利用者が入力した文字列
	/// @param outValid 正規化後の文字列が正しい形式なら true
	/// @return 正規化後の文字列(不正な場合は空文字列)
	std::string	NormalizeCode(const std::string &strInput, bool &outValid);

	/// @brief 正規化済みコードから、保存用ハッシュ(SHA-256の16進64文字)を作る。
	std::string	HashCode(const std::string &strNormalizedCode);

	/// @brief 新しい端末トークンを生成する(16進64文字、乱数32バイト)。
	/// @return 失敗時(乱数生成エラー)は空文字列。
	std::string	GenerateDeviceToken(void);

	/// @brief 端末トークンから、保存用ハッシュ(SHA-256の16進64文字)を作る。
	///
	/// トークンは前後の空白だけ取り除き、大文字小文字はそのまま扱う
	/// (生成時は小文字16進で統一されるため、正規化は不要)。
	std::string	HashDeviceToken(const std::string &strToken);

	/// @brief 知らない名前でのログイン要求(REQ_LOGIN)を受けたとき、アカウントを
	/// 自動作成してよいかどうか。
	///
	/// ログインコード方式(docs/login-code-auth-plan.md S4)への移行に伴い false 固定。
	/// アカウントは /api/account/register からのみ作る。MainFrameRecvProcCONNECT.cpp の
	/// RecvProcCONNECT_REQ_LOGIN はこの値を見て、未登録名なら LOGINRES_NG_PASSWORD を返す
	/// (アカウントの有無を区別させないため、パスワード不一致と同じ結果にする)。
	/// この関数を true に変えるとテスト(TestLoginCode.cpp)が落ちる。
	bool	ShouldAutoCreateAccountOnUnknownLogin(void);

	/// @brief アカウント名として使える文字かどうかを判定する。
	///
	/// アカウント登録API(/api/account/register)のパスワード検証と
	/// 同じ条件(ASCII の表示可能文字 0x21〜0x7E のみ)にそろえる。
	/// 全角文字・空白・制御文字は不可。
	/// @param strName アカウント名(UTF-8想定)
	/// @return 1文字以上、かつ全文字が条件を満たせば true
	bool	IsAcceptableAccountName(const std::string &strName);

	/// @brief アカウント名を ASCII 小文字に正規化する(A-Z のみ a-z にする)。
	///
	/// クライアント(SboCli/src/Window/WindowLOGINBrowser.cpp の NormalizeLoginText)は
	/// アカウント名を小文字化してから送るが、CLibInfoAccount::GetPtr(LPCSTR) は
	/// 大小文字を区別して比較する。register / issue-code で検証・検索・保存する前に
	/// 必ずこれを通し、大文字混じりで登録した既存アカウントとの表記ゆれを防ぐ。
	/// ASCII 以外のバイトは変更しない(IsAcceptableAccountName で別途弾く)。
	/// @param strName アカウント名(UTF-8想定)
	std::string	NormalizeAccountName(const std::string &strName);

	/// @brief おまかせ登録用に、アカウント名を自動生成する。
	///
	/// "player-" + 小文字英数字6文字(Crockford Base32由来。I,L,O,U を含まず
	/// 紛らわしくない)。一意性は呼び出し側(CAccountRegisterHandler)が
	/// CLibInfoAccount::GetPtr で確認し、衝突したら作り直すこと
	/// (このモジュールはDBを見ないため、ここでは一意性を保証しない)。
	/// @return 失敗時(乱数生成エラー)は空文字列。
	std::string	GenerateAutoAccountName(void);

	/// @brief おまかせ登録用に、パスワードを自動生成する。
	///
	/// 英数字12文字(Crockford Base32由来、大文字のみ・紛らわしい文字を含まない)。
	/// PasswordHash::IsAcceptable を必ず満たす文字種にしてある。
	/// @return 失敗時(乱数生成エラー)は空文字列。
	std::string	GenerateAutoPassword(void);
}
