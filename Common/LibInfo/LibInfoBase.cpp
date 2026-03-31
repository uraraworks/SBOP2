/// @file LibInfoBase.cpp
/// @brief 情報ライブラリ 基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "LibInfoBase.h"

CLibInfoBase::CLibInfoBase()
{
	m_nProcNo = 0;
}

CLibInfoBase::~CLibInfoBase()
{
	Destroy();
}

void CLibInfoBase::Create(void)
{
}

void CLibInfoBase::Destroy(void)
{
}

BOOL CLibInfoBase::Proc(void)
{
	return FALSE;
}

int  CLibInfoBase::GetCount(void)
{
	return 0;
}

PCInfoBase CLibInfoBase::GetNew(void)
{
	return new CInfoBase;
}

PCInfoBase CLibInfoBase::RenewInfo(int nNo)
{
	return GetPtr(nNo);
}

void CLibInfoBase::GetSaveNo(ARRAYINT &anDst)
{
	int i, nCount;

	anDst.clear();

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		anDst.push_back(i);
	}
}

void CLibInfoBase::Add(PCInfoBase pInfo)
{
}

void CLibInfoBase::RenewIDPtr(void)
{
}

PCInfoBase CLibInfoBase::GetPtr(int nNo)
{
	return NULL;
}

