/// @file SvrPlatform.h
/// @brief サーバーのプラットフォーム依存処理 定義ファイル
/// @copyright Copyright(C)URARA-works
///
/// SboSvr に散らばっている Win32 API 呼び出しを、ここへ集めて置き換える。
///
/// **このヘッダは windows.h を include しない。** 非Windows でもそのまま
/// 通ることが前提で、実装側だけが `#ifdef _WIN32` で分岐する。
/// 移植できているかは em++ でのコンパイル(tools/test-sbosvr-portability.ps1)
/// で確認する。

#pragma once

#include <string>

namespace SboPlatform
{
	/// 起動からの経過ミリ秒を返す
	///
	/// GetTickCount() / timeGetTime() の置き換え。単調増加する。
	/// 32bit で折り返すのも同じなので、差分を取る使い方はそのまま通る。
	unsigned int	GetTickMs(void);

	/// 現地時刻の分解値
	struct LOCALTIME {
		int	nYear;		// 西暦
		int	nMonth;		// 1〜12
		int	nDay;		// 1〜31
		int	nHour;		// 0〜23
		int	nMinute;	// 0〜59
		int	nSecond;	// 0〜59
		int	nMilli;		// 0〜999
	};

	/// 現地時刻を取得する
	///
	/// GetLocalTime() の置き換え。
	void	GetLocalTime(LOCALTIME *pOut);

	/// 実行ファイルが置かれているディレクトリを返す
	///
	/// GetModuleFileName() + パス切り出しの置き換え。
	/// 末尾は区切り文字で終わる。取得できない場合は "./" を返す。
	std::string	GetExeDirectory(void);

	/// パスの区切り文字
	///
	/// Windows は '\\'、その他は '/'。
	char	GetPathSeparator(void);

	/// ディレクトリを作る(既にあれば何もしない)
	///
	/// CreateDirectory() の置き換え。
	/// @return 作成できた、または既に存在すれば true
	bool	MakeDirectory(const char *pszPath);

	/// 実行ファイルの隣にあるファイルのパスを組み立てる
	///
	/// @param pszRelative 実行ファイルからの相対パス。区切りは '/' で書くと
	///                    プラットフォームに応じて変換される。
	/// @return 絶対パス
	std::string	MakeExeRelativePath(const char *pszRelative);

	/// データ用ディレクトリ(SBODATA)を作り、その中のファイルのパスを返す
	///
	/// 実行ファイルの隣に SBODATA を作る。同じ手順が各所で重複していたため
	/// ここへ集約した。
	///
	/// @param pszFileName SBODATA 内のファイル名
	/// @return 絶対パス。ディレクトリ作成に失敗しても組み立てたパスは返す。
	std::string	MakeDataFilePath(const char *pszFileName);

	/// デバッグ出力へ1行書く
	///
	/// OutputDebugStringA() の置き換え。
	/// Windows ではデバッガへ、その他では stderr へ出す。
	void	WriteDebugLine(const char *pszText);
}
