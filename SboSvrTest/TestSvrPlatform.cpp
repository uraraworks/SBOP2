/// @file TestSvrPlatform.cpp
/// @brief プラットフォーム層のテスト
/// @copyright Copyright(C)URARA-works
///
/// ini の読み書きはサーバーの起動設定(ポート等)に直結する。
/// 自前パーサへ差し替えるにあたり、Windows API と同じ結果になることを
/// 押さえておく。ここが狂うとサーバーが起動しない、あるいは意図しない
/// ポートで待ち受ける。

#include "StdAfx.h"
#include "TestFramework.h"
#include "Platform/SvrPlatform.h"
#include <string>
#include <cstdio>

namespace
{
	// テスト用の一時 ini を作る
	class CTempIni
	{
	public:
		explicit CTempIni(const char *pszContent)
		{
			char szDir[MAX_PATH];
			char szPath[MAX_PATH];

			GetTempPathA(MAX_PATH, szDir);
			GetTempFileNameA(szDir, "sbo", 0, szPath);
			m_strPath = szPath;

			if (pszContent != NULL) {
				FILE *pFile = fopen(m_strPath.c_str(), "wb");
				if (pFile) {
					fwrite(pszContent, 1, strlen(pszContent), pFile);
					fclose(pFile);
				}
			}
		}
		~CTempIni(void)
		{
			DeleteFileA(m_strPath.c_str());
		}
		const char *Path(void) const { return m_strPath.c_str(); }

	private:
		std::string m_strPath;
	};

	const char *kSample =
		"[Pos]\r\n"
		"MainLeft=320\r\n"
		"MainTop=58\r\n"
		"[Setting]\r\n"
		"Port=2017\r\n"
		"HttpPort=18090\r\n"
		"AdminAccount=Admin\r\n";
}

//////////////////////////////////////////////////////////////////////
// ini の読み取り
//////////////////////////////////////////////////////////////////////

TEST(ini_整数を読める)
{
	CTempIni ini(kSample);

	CHECK_EQ(2017,  SboPlatform::GetIniInt(ini.Path(), "Setting", "Port", 2006));
	CHECK_EQ(18090, SboPlatform::GetIniInt(ini.Path(), "Setting", "HttpPort", 18080));
	CHECK_EQ(320,   SboPlatform::GetIniInt(ini.Path(), "Pos", "MainLeft", -1));
	CHECK_EQ(58,    SboPlatform::GetIniInt(ini.Path(), "Pos", "MainTop", -1));
}

TEST(ini_無いキーは既定値を返す)
{
	CTempIni ini(kSample);

	CHECK_EQ(2006, SboPlatform::GetIniInt(ini.Path(), "Setting", "NoSuchKey", 2006));
	CHECK_EQ(-1,   SboPlatform::GetIniInt(ini.Path(), "NoSuchSection", "Port", -1));
	CHECK_EQ(0,    SboPlatform::GetIniInt(ini.Path(), "Setting", "CookieSecure", 0));
}

TEST(ini_ファイルが無ければ既定値を返す)
{
	// サーバー初回起動時は ini が無い。ここで既定値が返らないと起動できない。
	CHECK_EQ(2006, SboPlatform::GetIniInt("Z:\\no_such_dir\\none.ini", "Setting", "Port", 2006));
	CHECK(SboPlatform::GetIniString("Z:\\no_such_dir\\none.ini", "Setting", "AdminAccount", "Admin") == "Admin");
}

TEST(ini_文字列を読める)
{
	CTempIni ini(kSample);

	CHECK(SboPlatform::GetIniString(ini.Path(), "Setting", "AdminAccount", "") == "Admin");
	CHECK(SboPlatform::GetIniString(ini.Path(), "Setting", "NoSuchKey", "def") == "def");
}

TEST(ini_セクションとキーは大文字小文字を区別しない)
{
	// Windows API と同じ挙動。既存の ini がそのまま読めることを保証する。
	CTempIni ini(kSample);

	CHECK_EQ(2017, SboPlatform::GetIniInt(ini.Path(), "SETTING", "PORT", -1));
	CHECK_EQ(2017, SboPlatform::GetIniInt(ini.Path(), "setting", "port", -1));
}

TEST(ini_別セクションの同名キーを取り違えない)
{
	const char *pszContent =
		"[A]\r\n"
		"Port=1111\r\n"
		"[B]\r\n"
		"Port=2222\r\n";
	CTempIni ini(pszContent);

	CHECK_EQ(1111, SboPlatform::GetIniInt(ini.Path(), "A", "Port", -1));
	CHECK_EQ(2222, SboPlatform::GetIniInt(ini.Path(), "B", "Port", -1));
}

TEST(ini_空白とコメントを無視する)
{
	const char *pszContent =
		"[Setting]\r\n"
		"; これはコメント\r\n"
		"\r\n"
		"  Port  =  2017  \r\n"
		"# こちらもコメント\r\n";
	CTempIni ini(pszContent);

	CHECK_EQ(2017, SboPlatform::GetIniInt(ini.Path(), "Setting", "Port", -1));
}

TEST(ini_LF改行でも読める)
{
	const char *pszContent = "[Setting]\nPort=2017\n";
	CTempIni ini(pszContent);

	CHECK_EQ(2017, SboPlatform::GetIniInt(ini.Path(), "Setting", "Port", -1));
}

//////////////////////////////////////////////////////////////////////
// ini の書き込み
//////////////////////////////////////////////////////////////////////

TEST(ini_既存キーを書き換えられる)
{
	CTempIni ini(kSample);

	CHECK(SboPlatform::SetIniString(ini.Path(), "Pos", "MainLeft", "999") != false);
	CHECK_EQ(999, SboPlatform::GetIniInt(ini.Path(), "Pos", "MainLeft", -1));

	// 他のキーが壊れていないこと
	CHECK_EQ(58,   SboPlatform::GetIniInt(ini.Path(), "Pos", "MainTop", -1));
	CHECK_EQ(2017, SboPlatform::GetIniInt(ini.Path(), "Setting", "Port", -1));
}

TEST(ini_既存セクションへキーを追加できる)
{
	CTempIni ini(kSample);

	CHECK(SboPlatform::SetIniString(ini.Path(), "Setting", "CookieSecure", "1") != false);
	CHECK_EQ(1, SboPlatform::GetIniInt(ini.Path(), "Setting", "CookieSecure", 0));
	CHECK_EQ(2017, SboPlatform::GetIniInt(ini.Path(), "Setting", "Port", -1));
}

TEST(ini_新しいセクションを追加できる)
{
	CTempIni ini(kSample);

	CHECK(SboPlatform::SetIniString(ini.Path(), "Info", "ClientVersion", "0.043") != false);
	CHECK(SboPlatform::GetIniString(ini.Path(), "Info", "ClientVersion", "") == "0.043");
	CHECK_EQ(2017, SboPlatform::GetIniInt(ini.Path(), "Setting", "Port", -1));
}

TEST(ini_書き換えても他セクションを壊さない)
{
	// ウィンドウ位置の保存で [Pos] を4回書き換える。
	// [Setting] のポートが巻き添えで壊れると起動しなくなる。
	CTempIni ini(kSample);

	CHECK(SboPlatform::SetIniString(ini.Path(), "Pos", "MainLeft",   "10") != false);
	CHECK(SboPlatform::SetIniString(ini.Path(), "Pos", "MainTop",    "20") != false);
	CHECK(SboPlatform::SetIniString(ini.Path(), "Pos", "MainRight",  "30") != false);
	CHECK(SboPlatform::SetIniString(ini.Path(), "Pos", "MainBottom", "40") != false);

	CHECK_EQ(10, SboPlatform::GetIniInt(ini.Path(), "Pos", "MainLeft", -1));
	CHECK_EQ(20, SboPlatform::GetIniInt(ini.Path(), "Pos", "MainTop", -1));
	CHECK_EQ(30, SboPlatform::GetIniInt(ini.Path(), "Pos", "MainRight", -1));
	CHECK_EQ(40, SboPlatform::GetIniInt(ini.Path(), "Pos", "MainBottom", -1));
	CHECK_EQ(2017,  SboPlatform::GetIniInt(ini.Path(), "Setting", "Port", -1));
	CHECK_EQ(18090, SboPlatform::GetIniInt(ini.Path(), "Setting", "HttpPort", -1));
}

//////////////////////////////////////////////////////////////////////
// その他
//////////////////////////////////////////////////////////////////////

TEST(時刻_経過時間が進む)
{
	unsigned int a = SboPlatform::GetTickMs();
	Sleep(50);
	unsigned int b = SboPlatform::GetTickMs();

	// 折り返しても差分は正しく出る
	unsigned int nDiff = b - a;
	CHECK(nDiff >= 40);
	CHECK(nDiff < 5000);
}

TEST(時刻_現地時刻が妥当な範囲)
{
	SboPlatform::LOCALTIME t;

	SboPlatform::GetLocalTime(&t);

	CHECK(t.nYear >= 2020);
	CHECK((t.nMonth >= 1) && (t.nMonth <= 12));
	CHECK((t.nDay >= 1) && (t.nDay <= 31));
	CHECK((t.nHour >= 0) && (t.nHour <= 23));
	CHECK((t.nMinute >= 0) && (t.nMinute <= 59));
	CHECK((t.nSecond >= 0) && (t.nSecond <= 60));
}

TEST(パス_実行ファイルの隣を指す)
{
	std::string strDir = SboPlatform::GetExeDirectory();

	CHECK(strDir.empty() == false);
	// 末尾は区切り文字
	CHECK(strDir[strDir.size() - 1] == SboPlatform::GetPathSeparator());

	std::string strPath = SboPlatform::MakeExeRelativePath("SBODATA/x.db");
	// 区切り文字が統一されている
	CHECK(strPath.find('/') == std::string::npos);
	CHECK(strPath.find("SBODATA") != std::string::npos);
}
