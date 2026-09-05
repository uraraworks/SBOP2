/// @file UraraSockTCPSelect.h
/// @brief select ベースの TCP 通信クラス 定義ファイル
/// @copyright Copyright(C)URARA-works
///
/// 既存の UraraSockTCP は WSAAsyncSelect + 隠しウィンドウに依存しており、
/// ウィンドウが無いと動かせないため単体テストが書けない。
/// こちらはウィンドウに依存しない select ベースの実装。
///
/// 現時点ではサーバーモード(Host)のみ対応。Connect() は FALSE を返す。

#pragma once

#include "UraraSockTCP.h"

#ifdef _WIN32

// select ベースの実装を取得する
CUraraSockTCP *GetUraraSockTCPSelect(void);

#endif // _WIN32
