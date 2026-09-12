/// @file TestSessionsJsonBuilder.cpp
/// @brief 接続中プレイヤー一覧(/api/server/sessions) JSON組み立てのテスト
/// @copyright Copyright(C)URARA-works
///
/// Web管理画面のサーバー情報ダッシュボードが使う「接続中プレイヤー一覧」の
/// JSON形を固定する。ロック・MgrData等の副作用を持たない純粋関数
/// (SessionsJsonBuilder::Build)だけを対象にする。

#include "StdAfx.h"
#include "TestFramework.h"
#include "Web/SessionsJsonBuilder.h"
#include <cstring>

namespace
{
	SboSessionRow	MakeEmptyRow(unsigned int nAccountId)
	{
		SboSessionRow row;
		row.nAccountId = nAccountId;
		row.strAccount = "";
		row.nCharId = 0;
		row.bHasCharName = false;
		row.bHasMapId = false;
		row.nMapId = 0;
		row.bHasPos = false;
		row.nX = 0;
		row.nY = 0;
		row.bHasIp = false;
		row.bHasLastKeepalive = false;
		row.tLastKeepaliveEpoch = 0;
		row.bHasLoginAt = false;
		row.tLoginAtEpoch = 0;
		row.nSessionId = 0;
		return row;
	}

	bool	Contains(const std::string &strHaystack, const char *pszNeedle)
	{
		return strHaystack.find(pszNeedle) != std::string::npos;
	}
}

TEST(SessionsJsonBuilder_0件ならcount0で配列も空)
{
	std::vector<SboSessionRow> rows;
	std::string strJson = SessionsJsonBuilder::Build(rows, "2026-09-12T00:00:00Z", 0);

	CHECK(Contains(strJson, "\"count\":0"));
	CHECK(Contains(strJson, "\"sessions\":[]"));
	CHECK(Contains(strJson, "\"updatedAt\":\"2026-09-12T00:00:00Z\""));
}

TEST(SessionsJsonBuilder_値が無い項目はnullで出す)
{
	std::vector<SboSessionRow> rows;
	rows.push_back(MakeEmptyRow(7));

	std::string strJson = SessionsJsonBuilder::Build(rows, "2026-09-12T00:00:00Z", 1000);

	CHECK(Contains(strJson, "\"accountId\":7"));
	CHECK(Contains(strJson, "\"charId\":0"));
	CHECK(Contains(strJson, "\"charName\":null"));
	CHECK(Contains(strJson, "\"mapId\":null"));
	CHECK(Contains(strJson, "\"x\":null"));
	CHECK(Contains(strJson, "\"y\":null"));
	CHECK(Contains(strJson, "\"ip\":null"));
	CHECK(Contains(strJson, "\"lastKeepaliveSec\":null"));
	CHECK(Contains(strJson, "\"loginAt\":null"));
}

TEST(SessionsJsonBuilder_値がある項目を正しく出す)
{
	SboSessionRow row = MakeEmptyRow(3);
	row.strAccount = "tester";
	row.nCharId = 42;
	row.bHasCharName = true;
	row.strCharName = "勇者";
	row.bHasMapId = true;
	row.nMapId = 5;
	row.bHasPos = true;
	row.nX = 100;
	row.nY = -20;
	row.bHasIp = true;
	row.strIp = "192.168.1.1";
	row.bHasLastKeepalive = true;
	row.tLastKeepaliveEpoch = 900;	// now(1000) との差分は100秒
	row.bHasLoginAt = true;
	row.tLoginAtEpoch = 0;	// UNIXエポック → 1970-01-01T00:00:00Z

	std::vector<SboSessionRow> rows;
	rows.push_back(row);

	std::string strJson = SessionsJsonBuilder::Build(rows, "2026-09-12T00:00:00Z", 1000);

	CHECK(Contains(strJson, "\"count\":1"));
	CHECK(Contains(strJson, "\"accountId\":3"));
	CHECK(Contains(strJson, "\"account\":\"tester\""));
	CHECK(Contains(strJson, "\"charId\":42"));
	CHECK(Contains(strJson, "\"mapId\":5"));
	CHECK(Contains(strJson, "\"x\":100"));
	CHECK(Contains(strJson, "\"y\":-20"));
	CHECK(Contains(strJson, "\"ip\":\"192.168.1.1\""));
	CHECK(Contains(strJson, "\"lastKeepaliveSec\":100"));
	CHECK(Contains(strJson, "\"loginAt\":\"1970-01-01T00:00:00Z\""));
}

TEST(SessionsJsonBuilder_複数行はaccountId順の入力どおりに出る)
{
	std::vector<SboSessionRow> rows;
	rows.push_back(MakeEmptyRow(1));
	rows.push_back(MakeEmptyRow(2));

	std::string strJson = SessionsJsonBuilder::Build(rows, "2026-09-12T00:00:00Z", 0);

	size_t nPos1 = strJson.find("\"accountId\":1");
	size_t nPos2 = strJson.find("\"accountId\":2");
	CHECK(nPos1 != std::string::npos);
	CHECK(nPos2 != std::string::npos);
	CHECK(nPos1 < nPos2);
}

TEST(SessionsJsonBuilder_FormatIso8601がUNIXエポックを変換できる)
{
	std::string strIso = SessionsJsonBuilder::FormatIso8601((std::time_t)0);
	CHECK(strIso == "1970-01-01T00:00:00Z");
}

namespace
{
	SboLoginCharRow	MakeLoginChar(unsigned int nSessionId, unsigned int nAccountId,
			unsigned int nCharId, const char *pszName, unsigned int nMapId, int nX, int nY)
	{
		SboLoginCharRow loginChar;
		loginChar.nSessionId = nSessionId;
		loginChar.nAccountId = nAccountId;
		loginChar.nCharId = nCharId;
		loginChar.strCharName = pszName;
		loginChar.nMapId = nMapId;
		loginChar.nX = nX;
		loginChar.nY = nY;
		return loginChar;
	}
}

TEST(ApplyLoginChars_セッションIDが一致するキャラで埋まる)
{
	SboSessionRow row = MakeEmptyRow(1);
	row.nSessionId = 100;
	std::vector<SboSessionRow> rows;
	rows.push_back(row);

	std::vector<SboLoginCharRow> loginChars;
	loginChars.push_back(MakeLoginChar(100, 1, 42, "勇者", 5, 10, 20));

	SessionsJsonBuilder::ApplyLoginChars(rows, loginChars);

	CHECK(rows[0].nCharId == 42);
	CHECK(rows[0].bHasCharName);
	CHECK(rows[0].strCharName == "勇者");
	CHECK(rows[0].bHasMapId);
	CHECK(rows[0].nMapId == 5);
	CHECK(rows[0].bHasPos);
	CHECK(rows[0].nX == 10);
	CHECK(rows[0].nY == 20);
}

TEST(ApplyLoginChars_セッションID不一致でもアカウントIDが一致すれば埋まる)
{
	SboSessionRow row = MakeEmptyRow(1);
	row.nSessionId = 999;	// ログイン中キャラのセッションIDとは食い違っている状況
	std::vector<SboSessionRow> rows;
	rows.push_back(row);

	std::vector<SboLoginCharRow> loginChars;
	loginChars.push_back(MakeLoginChar(100, 1, 42, "勇者", 5, 10, 20));

	SessionsJsonBuilder::ApplyLoginChars(rows, loginChars);

	CHECK(rows[0].nCharId == 42);
	CHECK(rows[0].bHasCharName);
	CHECK(rows[0].strCharName == "勇者");
}

TEST(ApplyLoginChars_一致するログイン中キャラが無ければ行は変更しない)
{
	SboSessionRow row = MakeEmptyRow(1);
	row.nSessionId = 100;
	std::vector<SboSessionRow> rows;
	rows.push_back(row);

	// キャラ未選択(まだログイン中キャラ一覧に載っていない)状態を想定し、空のまま渡す
	std::vector<SboLoginCharRow> loginChars;

	SessionsJsonBuilder::ApplyLoginChars(rows, loginChars);

	CHECK(rows[0].nCharId == 0);
	CHECK(!rows[0].bHasCharName);
	CHECK(!rows[0].bHasMapId);
	CHECK(!rows[0].bHasPos);
}

TEST(ApplyLoginChars_複数接続はそれぞれ対応するキャラで埋まる)
{
	SboSessionRow row1 = MakeEmptyRow(1);
	row1.nSessionId = 100;
	SboSessionRow row2 = MakeEmptyRow(2);
	row2.nSessionId = 200;
	std::vector<SboSessionRow> rows;
	rows.push_back(row1);
	rows.push_back(row2);

	std::vector<SboLoginCharRow> loginChars;
	loginChars.push_back(MakeLoginChar(100, 1, 42, "勇者", 5, 10, 20));
	loginChars.push_back(MakeLoginChar(200, 2, 43, "魔法使い", 6, 30, 40));

	SessionsJsonBuilder::ApplyLoginChars(rows, loginChars);

	CHECK(rows[0].nCharId == 42);
	CHECK(rows[0].strCharName == "勇者");
	CHECK(rows[1].nCharId == 43);
	CHECK(rows[1].strCharName == "魔法使い");
}
