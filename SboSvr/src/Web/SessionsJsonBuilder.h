/// @file SessionsJsonBuilder.h
/// @brief 接続中プレイヤー一覧 JSON組み立て(純粋関数) 定義ファイル
/// @copyright Copyright(C)URARA-works
///
/// ServerSessionsHandler から「ロックを取ってアカウント/キャラ情報を集める」部分と
/// 「JSON文字列を組み立てる」部分を分離するためのファイル。
/// ここに置いた関数は OS API・MgrData・ロック等に一切依存しないため、
/// SboSvrTest から直接リンクしてテストできる。

#pragma once

#include <ctime>
#include <string>
#include <vector>

/// 1セッション分の表示用データ(ロック済みの生データから抽出した後の値)
///
/// 値が無い項目は has* フラグで null 出力にする。
struct SboSessionRow
{
	unsigned int	nAccountId;
	std::string	strAccount;

	unsigned int	nSessionId;	// アカウント側のセッションID。ログイン中キャラの照合キー(JSON出力はしない)

	unsigned int	nCharId;	// 0 なら未使用中(charId は 0 のまま出力する)

	bool	bHasCharName;
	std::string	strCharName;

	bool	bHasMapId;
	unsigned int	nMapId;

	bool	bHasPos;
	int	nX;
	int	nY;

	bool	bHasIp;
	std::string	strIp;	// "a.b.c.d" 形式

	bool	bHasLastKeepalive;
	std::time_t	tLastKeepaliveEpoch;	// UNIX秒(UTC)

	bool	bHasLoginAt;
	std::time_t	tLoginAtEpoch;	// UNIX秒(UTC)。今回のログイン時刻(CInfoAccount::m_dwTimeLastLogin)
};

/// ログイン中キャラ側の照合用データ(CLibInfoCharSvr::GetPtrLogIn() で得たキャラ情報から抽出)
struct SboLoginCharRow
{
	unsigned int	nSessionId;	// CInfoCharBase::m_dwSessionID
	unsigned int	nAccountId;	// CInfoCharBase::m_dwAccountID

	unsigned int	nCharId;
	std::string	strCharName;
	unsigned int	nMapId;
	int	nX;
	int	nY;
};

namespace SessionsJsonBuilder
{
	/// レスポンスJSONを組み立てる(ロック等の副作用が無い純粋関数)
	///
	/// @param rows 表示するセッション。呼び出し前に accountId 昇順へソートしておくこと
	/// @param strUpdatedAt "updatedAt" にそのまま入れる ISO8601 文字列
	/// @param tNowEpoch lastKeepaliveSec の算出に使う「現在時刻」(UNIX秒、UTC)
	std::string	Build(const std::vector<SboSessionRow> &rows,
			const std::string &strUpdatedAt, std::time_t tNowEpoch);

	/// UTC の time_t を ISO8601("YYYY-MM-DDTHH:MM:SSZ")へ変換する
	///
	/// @return 変換できなければ空文字列
	std::string	FormatIso8601(std::time_t tEpoch);

	/// アカウント行にログイン中キャラの情報を合成する(ロック等の副作用が無い純粋関数)
	///
	/// CInfoAccount::m_dwCharID は「使用中のキャラID」として設定されておらず常に0のため、
	/// ログイン中キャラ一覧(CLibInfoCharSvr::GetPtrLogIn())と突き合わせて補完する。
	/// まず nSessionId が一致するキャラを探し、無ければ nAccountId が一致するキャラで代用する。
	/// 一致するキャラが無ければ(キャラ未選択等)そのアカウント行は変更しない。
	///
	/// @param rows [in,out] CollectSessions() 相当で集めたアカウント側の行
	/// @param loginChars ログイン中キャラの一覧
	void	ApplyLoginChars(std::vector<SboSessionRow> &rows,
			const std::vector<SboLoginCharRow> &loginChars);
}
