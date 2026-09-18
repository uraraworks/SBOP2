/// @file AccountAuthStore.h
/// @brief ログインコード・端末トークンの SQLite 永続化 定義ファイル
/// @details sys_account_code / sys_account_device テーブルへの CRUD。
///          CSaveLoadInfoBase::s_pDb は CMgrData::Save()/Load() の間だけ
///          有効な共有ハンドルであり、発行/失効のたびに即時書き込みたい
///          この用途には使えない。CSaveLoadInfoMapGenPattern と同じく、
///          呼び出しのたびに自前で SQLite 接続を開閉する独立実装にする。

#pragma once

#include <string>
#include <vector>

struct sqlite3;

/// sys_account_code / sys_account_device テーブルへの CRUD ヘルパークラス。
class CAccountAuthStore
{
public:
	CAccountAuthStore();
	~CAccountAuthStore();

	/// @brief コードを発行(新規 or 再発行)する。
	///
	/// 既存のコード・そのアカウントの全端末トークンを失効させたうえで、
	/// 新しいコードを保存する(1トランザクション)。
	/// @retval true 成功
	bool	IssueCode(unsigned int dwAccountID, const std::string &strCodeHash, long lTimeIssued);

	/// @brief コードのハッシュからアカウントIDを引く。
	/// @retval true 見つかった
	bool	FindAccountIDByCodeHash(const std::string &strCodeHash, unsigned int &outAccountID);

	/// @brief 端末トークンを新規発行する。
	/// @retval true 成功
	bool	CreateDevice(unsigned int dwAccountID, const std::string &strTokenHash,
		const std::string &strLabel, long lTimeCreated);

	/// @brief 端末トークンのハッシュからアカウントIDを引き、最終使用日時を更新する。
	/// @retval true 見つかった(トークンは有効)
	bool	TouchDevice(const std::string &strTokenHash, long lTimeNow, unsigned int &outAccountID);

	/// @brief 端末トークンを1件失効させる(ログアウト)。
	///        存在しなくてもエラーにしない(冪等)。
	/// @retval true DB操作が成功した(該当行が無くてもtrue)
	bool	DeleteDeviceByTokenHash(const std::string &strTokenHash);

	/// @brief 指定アカウントの全端末トークンを失効させる。
	/// @retval true DB操作が成功した
	bool	DeleteAllDevicesForAccount(unsigned int dwAccountID);

	/// @brief sys_account に存在しない AccountID の sys_account_code /
	///        sys_account_device 行を削除する(起動時のアカウント読込直後に呼ぶ想定)。
	///
	/// register はメモリへの Add のみで、次の定期保存まで sys_account には書かれない。
	/// その間にサーバーが落ちると sys_account_code 側にだけ行が残り、再起動後に
	/// 同じ AccountID が別の新規アカウントへ割り当てられて、古いコードで他人の
	/// アカウントに入れてしまう恐れがある。起動のたびにこれで掃除しておく。
	/// @param validAccountIDs 現在 sys_account に存在する(=メモリへロード済みの)AccountID一覧
	/// @param outDeletedCodeRows 削除した sys_account_code の行数(不要ならNULL)
	/// @param outDeletedDeviceRows 削除した sys_account_device の行数(不要ならNULL)
	/// @retval true DB操作が成功した
	bool	PruneOrphanedAuth(const std::vector<unsigned int> &validAccountIDs,
		int *outDeletedCodeRows = NULL, int *outDeletedDeviceRows = NULL);

private:
	bool	OpenDb(sqlite3 **ppDb);
	void	EnsureTables(sqlite3 *pDb);
};
