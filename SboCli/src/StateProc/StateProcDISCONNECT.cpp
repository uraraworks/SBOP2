/// @file StateProcDISCONNECT.cpp
/// @brief 状態処理クラス(切断) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/08
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "MgrWindow.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "StateProcDISCONNECT.h"

CStateProcDISCONNECT::CStateProcDISCONNECT()
{
}

CStateProcDISCONNECT::~CStateProcDISCONNECT()
{
}

void CStateProcDISCONNECT::Init(void)
{
	m_pMgrWindow->MakeWindowMSG("サーバーとの接続が切れました");
}
