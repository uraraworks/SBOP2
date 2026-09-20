#pragma once

// マップパーツ配置の Undo/Redo 履歴を管理するシングルトン。
//
// - 記録: RecvProcADMIN_MAP_SETPARTS（ゲームスレッド）が上書き前後の値を Push する。
// - 取り出し: 管理画面の HTTP API（HTTP スレッド）が Undo/Redo で取り出して適用する。
// 異なるスレッドから触られるため std::mutex で保護する。

#include <mutex>
#include <string>
#include <vector>

// DWORD 等の基本型は StdAfx.h 経由で入る（Windows なら windows.h、
// 非Windows なら Platform/SvrCompat.h）。このヘッダから windows.h を
// 直接 include すると脱Windows ビルドが壊れるので入れないこと。

class CMapPartsHistory
{
public:
	// 1回のパーツ配置変更を表す履歴エントリ
	struct Entry
	{
		DWORD	mapId;
		int	x;
		int	y;
		bool	pile;		// true: 重ね合わせパーツ(SetPartsPile), false: 通常パーツ(SetParts)
		DWORD	oldPartsId;	// 変更前の値（Undo で復元する値）
		DWORD	newPartsId;	// 変更後の値（Redo で再適用する値）
	};

	static CMapPartsHistory &Instance();

	// 新しい変更を Undo スタックへ積む。Redo スタックはクリアする。
	// ※ Undo/Redo 自体の適用結果はここに積まないこと（無限ループ防止）。
	void Push(DWORD mapId, int x, int y, bool pile, DWORD oldPartsId, DWORD newPartsId);

	// Undo スタックから1件取り出し、Redo スタックへ積む。
	// outEntry.oldPartsId が「復元すべき値」になる。
	bool Undo(Entry &outEntry);

	// Redo スタックから1件取り出し、Undo スタックへ積む。
	// outEntry.newPartsId が「再適用すべき値」になる。
	bool Redo(Entry &outEntry);

	// 現在の Undo/Redo 件数を取得する
	void GetCounts(int &undoCount, int &redoCount);

	// {"undoCount":N,"redoCount":M} 形式の JSON を組み立てる
	// （WS ブロードキャストと HTTP レスポンスの両方から共通で使う）
	static std::string BuildCountsJson(int undoCount, int redoCount);

private:
	CMapPartsHistory();
	CMapPartsHistory(const CMapPartsHistory &);
	CMapPartsHistory &operator=(const CMapPartsHistory &);

	static const size_t kMaxHistory = 200;	// 上限件数（超えたら古いものから捨てる）

	std::mutex		m_mutex;
	std::vector<Entry>	m_undoStack;
	std::vector<Entry>	m_redoStack;
};
