#include "StdAfx.h"
#include "MapPartsHistory.h"

#include <sstream>

CMapPartsHistory::CMapPartsHistory()
{
}

CMapPartsHistory &CMapPartsHistory::Instance()
{
	static CMapPartsHistory instance;
	return instance;
}

void CMapPartsHistory::Push(DWORD mapId, int x, int y, bool pile, DWORD oldPartsId, DWORD newPartsId, DWORD sessionId, DWORD strokeId)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	Cell cell;
	cell.x = x;
	cell.y = y;
	cell.oldPartsId = oldPartsId;
	cell.newPartsId = newPartsId;

	// 新しい操作が積まれたら Redo 履歴は無効化する
	m_redoStack.clear();

	// 同じストロークの続きなら末尾エントリへ追記する
	if (strokeId != 0 && !m_undoStack.empty()) {
		Entry &last = m_undoStack.back();
		if (last.strokeId == strokeId && last.sessionId == sessionId && last.mapId == mapId && last.pile == pile) {
			last.cells.push_back(cell);
			return;
		}
	}

	Entry entry;
	entry.mapId = mapId;
	entry.pile = pile;
	entry.sessionId = sessionId;
	entry.strokeId = strokeId;
	entry.cells.push_back(cell);
	m_undoStack.push_back(entry);

	// 上限を超えたら古いものから捨てる
	if (m_undoStack.size() > kMaxHistory) {
		m_undoStack.erase(m_undoStack.begin());
	}
}

bool CMapPartsHistory::Undo(Entry &outEntry)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_undoStack.empty()) {
		return false;
	}

	Entry entry = m_undoStack.back();
	m_undoStack.pop_back();

	m_redoStack.push_back(entry);
	if (m_redoStack.size() > kMaxHistory) {
		m_redoStack.erase(m_redoStack.begin());
	}

	outEntry = entry;	// cells の逆順に oldPartsId を適用
	return true;
}

bool CMapPartsHistory::Redo(Entry &outEntry)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_redoStack.empty()) {
		return false;
	}

	Entry entry = m_redoStack.back();
	m_redoStack.pop_back();

	m_undoStack.push_back(entry);
	if (m_undoStack.size() > kMaxHistory) {
		m_undoStack.erase(m_undoStack.begin());
	}

	outEntry = entry;	// cells の正順に newPartsId を適用
	return true;
}

void CMapPartsHistory::GetCounts(int &undoCount, int &redoCount)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	undoCount = static_cast<int>(m_undoStack.size());
	redoCount = static_cast<int>(m_redoStack.size());
}

std::string CMapPartsHistory::BuildCountsJson(int undoCount, int redoCount)
{
	std::ostringstream oss;
	oss << "{\"undoCount\":" << undoCount << ",\"redoCount\":" << redoCount << "}";
	return oss.str();
}
