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

void CMapPartsHistory::Push(DWORD mapId, int x, int y, bool pile, DWORD oldPartsId, DWORD newPartsId)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	Entry entry;
	entry.mapId = mapId;
	entry.x = x;
	entry.y = y;
	entry.pile = pile;
	entry.oldPartsId = oldPartsId;
	entry.newPartsId = newPartsId;

	m_undoStack.push_back(entry);
	// 新しい操作が積まれたら Redo 履歴は無効化する
	m_redoStack.clear();

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

	outEntry = entry;	// oldPartsId が復元すべき値
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

	outEntry = entry;	// newPartsId が再適用すべき値
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
