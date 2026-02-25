#include "StdAfx.h"

#include "InfoItemTypeBase.h"
#include "LibInfoItem.h"
#include "LibInfoItemType.h"
#include "LibInfoCharCli.h"

// Compatibility shims for Step2 staged migration units.
// Keep these minimal and local to DLL project to avoid pulling
// large legacy compilation units at once.

LPCSTR CLibInfoItem::GetTypeName(DWORD dwTypeID)
{
	UNREFERENCED_PARAMETER(dwTypeID);
	return "";
}

CInfoBase* CLibInfoCharCli::GetPtr(DWORD dwCharID)
{
	UNREFERENCED_PARAMETER(dwCharID);
	return NULL;
}
