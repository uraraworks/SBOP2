/// @file TestMyString.cpp
/// @brief myString.h (CString/CStringA ヘルパー) のテスト
/// @copyright Copyright(C)URARA-works
///
/// ATL の CString/CStringA を Common/Platform/CStringCompat.h の
/// CStringTCompat に置き換えるにあたり、置き換え「前」の挙動を
/// このテストで固定する。特に Format() は ATL では無制限長だが、
/// 素朴な移植では内部バッファ上限で黙って壊れやすいので、
/// 長い文字列での Format を重点的に確認する。

#include "StdAfx.h"
#include "TestFramework.h"
#include "myString.h"
#include <string>
#include <cstring>

//////////////////////////////////////////////////////////////////////
// Utf8ToTString
//////////////////////////////////////////////////////////////////////

TEST(Utf8ToTString_ASCII)
{
	CString str = Utf8ToTString("Hello");
	CHECK(str == _T("Hello"));
}

TEST(Utf8ToTString_日本語)
{
	// "最新クライアントバージョン" の UTF-8 バイト列
	const char *pszUtf8 =
		"\xE6\x9C\x80\xE6\x96\xB0\xE3\x82\xAF\xE3\x83\xA9\xE3\x82\xA4"
		"\xE3\x82\xA2\xE3\x83\xB3\xE3\x83\x88\xE3\x83\x90\xE3\x83\xBC"
		"\xE3\x82\xB8\xE3\x83\xA7\xE3\x83\xB3";
	CString str = Utf8ToTString(pszUtf8);
	CHECK(str == _T("最新クライアントバージョン"));
}

TEST(Utf8ToTString_空文字列)
{
	CString str = Utf8ToTString("");
	CHECK(str.IsEmpty() != FALSE);
}

TEST(Utf8ToTString_NULL)
{
	CString str = Utf8ToTString(NULL);
	CHECK(str.IsEmpty() != FALSE);
}

//////////////////////////////////////////////////////////////////////
// AnsiToTString (CP932)
//////////////////////////////////////////////////////////////////////

TEST(AnsiToTString_CP932日本語)
{
	// "日本" の CP932 バイト列
	const char szSjis[] = { (char)0x93, (char)0xFA, (char)0x96, (char)0x7B, '\0' };
	CString str = AnsiToTString(szSjis, 932);
	CHECK(str == _T("日本"));
}

//////////////////////////////////////////////////////////////////////
// TStringToUtf8 / TStringToAnsi (往復一致)
//////////////////////////////////////////////////////////////////////

TEST(TStringToUtf8_日本語往復一致)
{
	CString strSrc = _T("日本語テスト");
	CStringA strUtf8 = TStringToUtf8(strSrc);
	CString strBack = Utf8ToTString(strUtf8);
	CHECK(strBack == strSrc);
}

TEST(TStringToAnsi_日本語往復一致)
{
	CString strSrc = _T("日本語テスト");
	CStringA strAnsi = TStringToAnsi(strSrc, 932);
	CString strBack = AnsiToTString(strAnsi, 932);
	CHECK(strBack == strSrc);
}

//////////////////////////////////////////////////////////////////////
// CString::Format
//////////////////////////////////////////////////////////////////////

TEST(Format_数値と文字列)
{
	CString strName = _T("テスト");
	CString strTmp;
	strTmp.Format(_T("SYSTEM:現在のオンライン数: %d"), 5);
	CHECK(strTmp == _T("SYSTEM:現在のオンライン数: 5"));

	// MainFrameRecvProcCONNECT.cpp と同じ書き方: %s に (LPCTSTR) キャストの CString
	strTmp.Format(_T("SYSTEM:最新クライアントバージョン: %s"), (LPCTSTR)strName);
	CHECK(strTmp == _T("SYSTEM:最新クライアントバージョン: テスト"));
}

TEST(Format_5000文字以上のワイド文字列)
{
	// ATL の CString::Format はバッファ上限が無い。
	// 素朴な移植 (固定長一時バッファで長さを測る実装) だと
	// 4096文字を超えたところで切り詰め/失敗して空文字列になりがち。
	std::wstring strLong(5000, L'あ');

	CString strFmt;
	strFmt.Format(_T("prefix-%s-suffix"), strLong.c_str());

	CHECK_EQ(5000 + 7 + 7, strFmt.GetLength()); // "prefix-" + 5000 + "-suffix"
	CHECK(strFmt.Left(7) == _T("prefix-"));
	CHECK(strFmt.Right(7) == _T("-suffix"));

	CString strMid = strFmt.Mid(7, 5000);
	CHECK_EQ(5000, strMid.GetLength());
	CHECK(strMid == CString(strLong.c_str()));
}

TEST(FormatA_5000文字以上の文字列)
{
	std::string strLong(5000, 'a');

	CStringA strFmt;
	strFmt.Format("prefix-%s-suffix", strLong.c_str());

	CHECK_EQ(5000 + 7 + 7, strFmt.GetLength());
	CHECK(strFmt.Left(7) == "prefix-");
	CHECK(strFmt.Right(7) == "-suffix");

	CStringA strMid = strFmt.Mid(7, 5000);
	CHECK_EQ(5000, strMid.GetLength());
	CHECK(strMid == CStringA(strLong.c_str()));
}

//////////////////////////////////////////////////////////////////////
// GetBuffer / ReleaseBuffer, Mid/Left/Right, Find, CompareNoCase,
// IsEmpty/GetLength
//////////////////////////////////////////////////////////////////////

TEST(GetBuffer_ReleaseBuffer_基本)
{
	CString str;
	LPTSTR pszBuf = str.GetBuffer(16);
	_tcscpy_s(pszBuf, 16, _T("abcXYZ"));
	str.ReleaseBuffer();

	CHECK_EQ(6, str.GetLength());
	CHECK(str == _T("abcXYZ"));
}

TEST(Mid_Left_Right_基本)
{
	CString str = _T("0123456789");

	CHECK(str.Mid(3, 4) == _T("3456"));
	CHECK(str.Left(3) == _T("012"));
	CHECK(str.Right(3) == _T("789"));
}

TEST(Find_基本)
{
	CString str = _T("abcdefabc");

	CHECK_EQ(2, str.Find(_T('c')));
	CHECK_EQ(3, str.Find(_T("def")));
	CHECK_EQ(-1, str.Find(_T("xyz")));
}

TEST(CompareNoCase_基本)
{
	CString strA = _T("ABCdef");
	CString strB = _T("abcDEF");
	CString strC = _T("abcDEG");

	CHECK_EQ(0, strA.CompareNoCase(strB));
	CHECK(strA.CompareNoCase(strC) != 0);
}

TEST(IsEmpty_GetLength_基本)
{
	CString strEmpty;
	CString strNonEmpty = _T("hello");

	CHECK(strEmpty.IsEmpty() != FALSE);
	CHECK_EQ(0, strEmpty.GetLength());

	CHECK(strNonEmpty.IsEmpty() == FALSE);
	CHECK_EQ(5, strNonEmpty.GetLength());
}
