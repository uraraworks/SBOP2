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

	/// 設定ファイル(ini)のパスを返す
	///
	/// 実行ファイルの拡張子を ini に置き換えたもの。
	///
	/// なお ini や DB のパスは従来から ANSI(CP932)前提で扱われている。
	/// 実行ファイルのパスに CP932 で表せない文字が含まれる環境は
	/// 元から想定外なので、その前提は変えていない。
	std::string	GetIniFilePath(void);

	/// 設定ファイル(ini)から整数を読む
	///
	/// GetPrivateProfileInt() の置き換え。ファイル形式は従来と同じ。
	/// 見つからない・数字として読めない場合は nDefault を返す。
	///
	/// セクション名とキー名は大文字小文字を区別しない(Windows API と同じ)。
	int	GetIniInt(const char *pszFile, const char *pszSection, const char *pszKey, int nDefault);

	/// 設定ファイル(ini)から文字列を読む
	///
	/// GetPrivateProfileString() の置き換え。
	std::string	GetIniString(const char *pszFile, const char *pszSection,
			const char *pszKey, const char *pszDefault);

	/// 設定ファイル(ini)へ文字列を書く
	///
	/// WritePrivateProfileString() の置き換え。
	/// 既存のセクション・キーがあれば書き換え、無ければ追加する。
	/// コメント行や順序は保つ。
	///
	/// @return 書き込めれば true
	bool	SetIniString(const char *pszFile, const char *pszSection,
			const char *pszKey, const char *pszValue);

	/// デバッグ出力へ1行書く
	///
	/// OutputDebugStringA() の置き換え。
	/// Windows ではデバッガへ、その他では stderr へ出す。
	void	WriteDebugLine(const char *pszText);
}
