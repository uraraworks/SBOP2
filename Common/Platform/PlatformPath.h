/// @file PlatformPath.h
/// @brief 実行ファイルの場所に関するパス解決 定義ファイル
/// @copyright Copyright(C)URARA-works
///
/// 「実行ファイルの置き場所」「実行ファイル基準の ini パス」は
/// `SboSvr/src/Platform/SvrPlatform.cpp` と `Common/SBOGlobal.cpp` の
/// 両方に同じロジックが重複していたため、ここへ集約する。
///
/// **このヘッダは windows.h を include しない。** 非Windows でもそのまま
/// 通ることが前提で、実装側(.cpp)だけが `#ifdef _WIN32` 等で分岐する。
///
/// 名前空間は既存のサーバー側呼び出し元(`SboPlatform::GetExeDirectory()` 等)を
/// 無改造のまま乗せ替えられるよう、あえて `SboPlatform` を使う。

#pragma once

#include <string>

namespace SboPlatform
{
	/// パスの区切り文字
	///
	/// Windows は '\\'、その他は '/'。
	char	GetPathSeparator(void);

	/// 実行ファイルが置かれているディレクトリを返す(char版)
	///
	/// 実行ファイルの絶対パスからファイル名部分を切り落としたもの。
	/// 末尾は区切り文字で終わる。取得できない場合は "./" を返す。
	std::string	GetExeDirectory(void);

	/// 実行ファイルが置かれているディレクトリを返す(wchar_t版)
	std::wstring	GetExeDirectoryW(void);

	/// 設定ファイル(ini)のパスを返す(char版)
	///
	/// 実行ファイルの拡張子を ini に置き換えたもの。
	///
	/// なお ini や DB のパスは従来から ANSI(CP932)前提で扱われている。
	/// 実行ファイルのパスに CP932 で表せない文字が含まれる環境は
	/// 元から想定外なので、その前提は変えていない。
	std::string	GetIniFilePath(void);

	/// 設定ファイル(ini)のパスを返す(wchar_t版)
	std::wstring	GetIniFilePathW(void);
}
