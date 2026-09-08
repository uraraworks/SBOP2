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

#include <ctime>
#include <string>
#include <vector>

// GetPathSeparator / GetExeDirectory / GetIniFilePath は
// Common/SBOGlobal.cpp (GetModuleFilePath/GetModuleIniPath) と実装が
// 重複していたため、Common/Platform/PlatformPath.h へ集約した。
// SboPlatform 名前空間はそのまま使えるので、既存の呼び出し元は無改造。
#include "../../../Common/Platform/PlatformPath.h"

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

	/// UTC時刻を取得する
	///
	/// GetSystemTime() の置き換え。LOCALTIME 構造体を流用するが、
	/// 中身はタイムゾーン変換前の UTC 値になる。
	void	GetSystemTime(LOCALTIME *pOut);

	// GetExeDirectory() / GetPathSeparator() は
	// Common/Platform/PlatformPath.h (上で include 済み) で宣言されている。

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

	// GetIniFilePath() も Common/Platform/PlatformPath.h で宣言されている。

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

	/// UTC時刻の分解値
	struct GMTIME {
		int	nYear;		// 西暦
		int	nMonth;		// 1〜12
		int	nDay;		// 1〜31
		int	nHour;		// 0〜23
		int	nMinute;	// 0〜59
		int	nSecond;	// 0〜59
		int	nWeekDay;	// 0(日)〜6(土)
	};

	/// time_t(UTC) を分解値に変換する
	///
	/// gmtime() の置き換え(スレッド安全)。Windows は gmtime_s、
	/// その他は gmtime_r を使う(分岐はこの関数の実装内だけに閉じる)。
	///
	/// @return 変換できれば true
	bool	GmTimeUtc(std::time_t t, GMTIME *pOut);

	/// UTC分解値を time_t へ変換する
	///
	/// timegm() の置き換え。Windows は timegm が無いため _mkgmtime を使う。
	///
	/// @return 変換できれば true
	bool	TimeGmUtc(const GMTIME &in, std::time_t *pOut);

	/// ソケットライブラリを初期化する(WSAStartup() の置き換え)
	///
	/// Windows では WSAStartup() を呼ぶ。WSAStartup は呼び出し回数を内部で
	/// 参照カウントする方式なので、Web層の複数箇所(WebSocketBridge /
	/// HttpServer 等)から呼んでも重ねて安全(対応する SocketCleanup() も
	/// 同数呼べばよい)。非Windowsでは初期化不要なので何もせず true を返す。
	///
	/// @return 初期化できれば true
	bool	SocketStartup(void);

	/// ソケットライブラリを終了する(WSACleanup() の置き換え)
	///
	/// SocketStartup() の呼び出しと対にして呼ぶこと。非Windowsでは何もしない。
	void	SocketCleanup(void);

	/// 暗号論的に安全な乱数を生成する
	///
	/// SessionStore(セッショントークン) / PasswordHash(salt) 双方で使う
	/// 「安全な乱数」をここへ集約する。Windows は BCryptGenRandom、
	/// その他は /dev/urandom (または getrandom(2)) を使う(分岐はこの関数の
	/// 実装内だけに閉じる)。
	///
	/// rand() / std::mt19937 等の非暗号論的な乱数源、および
	/// std::random_device への安易な依存(実装によっては決定論的になりうる)
	/// は使わない。
	///
	/// @return 成功すれば true。失敗した場合は false を返す。
	///         **呼び出し側は失敗時に弱い乱数へフォールバックしないこと。**
	bool	GenerateRandomBytes(void *pBuffer, size_t nLength);

	/// 実行ファイルに同梱されたリソースから PNG を取り出す
	///
	/// SpriteSheetHandler / MapPartsHandler が SboGrpData.dll のリソースを
	/// 読むために使っていた LoadLibraryW / FindResourceW / LockResource /
	/// FreeLibrary をここへ集約する。PE のリソースという概念自体が
	/// Windows 専有のものなので、非Windows は常に false を返す
	/// (呼び出し側は画像ストア(DB) / ファイル(res/) 等、前段のフォールバックで
	/// 賄う設計になっている)。
	///
	/// DLL ハンドルはプロセス内で使い回す(内部でスレッド安全にキャッシュする)。
	/// HttpServer は複数のクライアントスレッドから同時に呼ぶため、
	/// この関数自体もどのスレッドから呼んでも安全。
	///
	/// @param pszResourceName リソース名(ASCII 前提。ppszFixedNames 等の
	///                        char テーブル由来、または "IDP_MAP_%02d" のような
	///                        printf パターンから組み立てたもの)
	/// @param outPng          取得できた PNG の生バイト列
	/// @return 見つかって読み出せれば true
	bool	LoadEmbeddedPng(const char *pszResourceName, std::vector<unsigned char> &outPng);
}
