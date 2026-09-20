/// @file AccountAdminStore.h
/// @brief アカウントのゴミ箱/BAN状態を保持する sys_account_admin テーブルの永続化 定義ファイル
/// @details sys_account はサーバー停止時にメモリから DELETE + 全INSERT で書き戻されるため、
///          そこに列を足すと CInfoAccount の旧 .dat 形式要素テーブル
///          (GetDataSize/GetDataSizeNo/GetWriteData/ReadElementData/Copy/GetTmpData) を
///          全部直す必要が出る。別テーブルにすることでその置換に巻き込まれないようにする。
///          CAccountAuthStore と同じく、呼び出しのたびに自前で SQLite 接続を開閉する
///          独立実装にする(CSaveLoadInfoBase::s_pDb は Save()/Load() の間だけ有効なため)。
///
///          通常状態(active)のアカウントは行を持たない。行があれば trashed/banned のいずれか。

#pragma once

#include <string>
#include <vector>

struct sqlite3;

/// sys_account_admin テーブルの1行分
struct AccountAdminRow
{
	unsigned int	dwAccountID;
	std::string	strStatus;		// "trashed" / "banned"
	std::string	strReason;
	long		lTimeChanged;
	unsigned int	dwActorAccountID;
	int		nPrevDisable;		// 状態変更前の m_bDisable (0/1)
	std::string	strPrevStatus;		// ゴミ箱に入れる直前の Status ("banned" or 空)
	std::string	strPrevReason;		// その時の Reason
};

/// sys_account_admin テーブルへの CRUD ヘルパークラス。
class CAccountAdminStore
{
public:
	CAccountAdminStore();
	~CAccountAdminStore();

	/// @brief 全件読み込む(起動時のアカウント読込直後に呼ぶ想定)。
	/// @retval true 成功(0件でもtrue)
	bool	LoadAll(std::vector<AccountAdminRow> &outRows);

	/// @brief 1件取得する。
	/// @retval true 見つかった(=trashed/banned)
	bool	Get(unsigned int dwAccountID, AccountAdminRow &outRow);

	/// @brief 状態を設定する(UPSERT)。
	/// @param strPrevStatus ゴミ箱に入れる直前の Status("banned"想定)。無ければ空文字。
	/// @param strPrevReason その時の Reason。無ければ空文字。
	/// @retval true 成功
	bool	SetStatus(unsigned int dwAccountID, const std::string &strStatus,
		const std::string &strReason, long lTimeChanged,
		unsigned int dwActorAccountID, int nPrevDisable,
		const std::string &strPrevStatus = std::string(),
		const std::string &strPrevReason = std::string());

	/// @brief 行を削除する(復帰 or 完全削除の後始末)。
	/// @retval true DB操作が成功した(該当行が無くてもtrue)
	bool	DeleteRow(unsigned int dwAccountID);

private:
	bool	OpenDb(sqlite3 **ppDb);
	void	EnsureTables(sqlite3 *pDb);
};
