/// @file LibInfoSystem.cpp
/// @brief システム情報クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/09/28
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "LibInfoSystem.h"

CLibInfoSystem::CLibInfoSystem()
{
	m_pInfo = NULL;
}

CLibInfoSystem::~CLibInfoSystem()
{
	Destroy();
}

void CLibInfoSystem::Create(void)
{
	m_pInfo = new CInfoSystem;
}

void CLibInfoSystem::Destroy(void)
{
	SAFE_DELETE(m_pInfo);
}

int  CLibInfoSystem::GetCount(void)
{
	return 1;
}

PCInfoBase CLibInfoSystem::GetNew(void)
{
	return (PCInfoSystem)new CInfoSystem;
}

void CLibInfoSystem::Add(PCInfoBase pInfo)
{
	SAFE_DELETE(m_pInfo);
	m_pInfo = (PCInfoSystem)pInfo;
}

void CLibInfoSystem::Delete(int nNo)
{
	SAFE_DELETE(m_pInfo);
}

void CLibInfoSystem::DeleteAll(void)
{
	SAFE_DELETE(m_pInfo);
}

PCInfoBase CLibInfoSystem::GetPtr(int nNo/*0*/)
{
	return m_pInfo;
}

