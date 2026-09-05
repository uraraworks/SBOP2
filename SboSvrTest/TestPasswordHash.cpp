/// @file TestPasswordHash.cpp
/// @brief パスワードハッシュのテスト
/// @copyright Copyright(C)URARA-works
///
/// 現在の実装は Windows CNG (bcrypt.h) の BCryptDeriveKeyPBKDF2 を使う。
/// 非Windows へ移す際はここを差し替えることになるが、**既存アカウントの
/// ハッシュと互換でなければログインが全滅する**。
///
/// このテストの役割は2つ。
///
/// 1. 現在の実装が標準の PBKDF2-HMAC-SHA256 どおりであることを、
///    外部の公開テストベクタで証明する。ここが通れば、規格に準拠した
///    任意の実装(OpenSSL / mbedTLS 等)が自動的に互換になる。
///    総当たりで一致を確かめる必要は無い。
///
/// 2. 平文パスワードのバイト列が UTF-8 であることを固定する。
///    PasswordHash は変換を一切せず生バイトを渡すため、呼び出し側が
///    CP932 を渡すと同じパスワードでも別のハッシュになる。
///    実装の移植よりもこちらの方が現実的なリスク。

#include "StdAfx.h"
#include "TestFramework.h"
#include "PasswordHash.h"
#include <string>

//////////////////////////////////////////////////////////////////////
// 1. 公開テストベクタとの照合(規格準拠の証明)
//
// PBKDF2-HMAC-SHA256 の広く公開されているベクタ。
// P="password" / S="salt" / dkLen=32
//   c=1     120fb6cffcf8b32c43e7225256c4f837a86548c92ccc35480805987cb70be17b
//   c=2     ae4d0c95af6b46d32d0adff928f06dd02a303f8ef3c251dfd6e2d85a95474c43
//   c=4096  c5e478d59288c841aa530db6845c4c8d962893a001ce4e11a4963873aa98134a
//
// 保存形式 "$s1$<反復回数>$<salt_base64>$<hash_base64>" に載せて
// Verify() が真を返すかで確認する("salt" の base64 は "c2FsdA==")。
//////////////////////////////////////////////////////////////////////

TEST(規格_公開ベクタと一致する_反復1)
{
    const char *pszStored =
        "$s1$1$c2FsdA==$Eg+2z/z4syxD5yJSVsT4N6hlSMkszDVICAWYfLcL4Xs=";

    CHECK(PasswordHash::Verify(pszStored, "password") != false);
}

TEST(規格_公開ベクタと一致する_反復2)
{
    const char *pszStored =
        "$s1$2$c2FsdA==$rk0Mla9rRtMtCt/5KPBt0CowP47zwlHf1uLYWpVHTEM=";

    CHECK(PasswordHash::Verify(pszStored, "password") != false);
}

TEST(規格_公開ベクタと一致する_反復4096)
{
    const char *pszStored =
        "$s1$4096$c2FsdA==$xeR41ZKIyEGqUw22hFxMjZYok6ABzk4RpJY4c6qYE0o=";

    CHECK(PasswordHash::Verify(pszStored, "password") != false);
}

TEST(規格_パスワードが違えば一致しない)
{
    const char *pszStored =
        "$s1$4096$c2FsdA==$xeR41ZKIyEGqUw22hFxMjZYok6ABzk4RpJY4c6qYE0o=";

    CHECK(PasswordHash::Verify(pszStored, "Password") == false);
    CHECK(PasswordHash::Verify(pszStored, "password ") == false);
    CHECK(PasswordHash::Verify(pszStored, "") == false);
}

//////////////////////////////////////////////////////////////////////
// 2. パスワードのバイト列(UTF-8)を固定する
//
// "ぱすわーど" は
//   UTF-8 : E3 81 B1 E3 81 99 E3 82 8F E3 83 BC E3 81 A9 (15バイト)
//   CP932 : 82 CF 82 B7 82 ED 81 5B 82 C7                (10バイト)
// で別物になる。salt="saltsaltsaltsalt" / c=1000 のとき期待値も当然変わる。
//////////////////////////////////////////////////////////////////////

TEST(バイト列_非ASCIIはUTF8として扱われる)
{
    // ソースが UTF-8 で保存されているため、この文字列リテラルは UTF-8 バイト列
    const char *pszPassword = "ぱすわーど";
    const char *pszStoredUtf8 =
        "$s1$1000$c2FsdHNhbHRzYWx0c2FsdA==$Y88vion+oDahPbuB/ZqBjSxW5glOQQ5Ebif0KSyahCU=";

    CHECK_EQ(15, (int)strlen(pszPassword));
    CHECK(PasswordHash::Verify(pszStoredUtf8, pszPassword) != false);
}

TEST(バイト列_CP932のハッシュとは一致しない)
{
    // 同じ「ぱすわーど」でも CP932 バイト列から作ったハッシュとは一致しない。
    // 呼び出し側が GetUtf8Pointer() を使わず (LPCSTR) 素キャストすると
    // この状態になり、同じパスワードなのにログインできなくなる。
    const char *pszPassword = "ぱすわーど";
    const char *pszStoredCp932 =
        "$s1$1000$c2FsdHNhbHRzYWx0c2FsdA==$vh6vYi62r1tiG7JKp0Y5VuG/f9MLEspwZx6dconghD8=";

    CHECK(PasswordHash::Verify(pszStoredCp932, pszPassword) == false);
}

//////////////////////////////////////////////////////////////////////
// 3. 生成と照合の往復
//////////////////////////////////////////////////////////////////////

TEST(往復_Hashした値をVerifyできる)
{
    std::string strHashed = PasswordHash::Hash("secret123");

    CHECK(strHashed.empty() == false);
    CHECK(PasswordHash::Verify(strHashed.c_str(), "secret123") != false);
    CHECK(PasswordHash::Verify(strHashed.c_str(), "secret124") == false);
}

TEST(往復_非ASCIIでも往復できる)
{
    std::string strHashed = PasswordHash::Hash("ぱすわーど");

    CHECK(strHashed.empty() == false);
    CHECK(PasswordHash::Verify(strHashed.c_str(), "ぱすわーど") != false);
    CHECK(PasswordHash::Verify(strHashed.c_str(), "ぱすわーと") == false);
}

TEST(往復_空文字列も扱える)
{
    std::string strHashed = PasswordHash::Hash("");

    CHECK(strHashed.empty() == false);
    CHECK(PasswordHash::Verify(strHashed.c_str(), "") != false);
    CHECK(PasswordHash::Verify(strHashed.c_str(), "x") == false);
}

TEST(生成_saltが毎回変わる)
{
    // 同じパスワードでも保存値が一致してはいけない(salt が乱数のため)
    std::string strA = PasswordHash::Hash("samepassword");
    std::string strB = PasswordHash::Hash("samepassword");

    CHECK(strA.empty() == false);
    CHECK(strB.empty() == false);
    CHECK(strA != strB);
    CHECK(PasswordHash::Verify(strA.c_str(), "samepassword") != false);
    CHECK(PasswordHash::Verify(strB.c_str(), "samepassword") != false);
}

TEST(生成_保存形式が期待どおり)
{
    std::string strHashed = PasswordHash::Hash("format");

    CHECK(strHashed.compare(0, 4, "$s1$") == 0);
    CHECK(PasswordHash::IsHashed(strHashed.c_str()) != false);

    // "$s1$<反復回数>$<salt>$<hash>" で区切りが3つ
    int nCount = 0;
    for (size_t i = 0; i < strHashed.size(); i ++) {
        if (strHashed[i] == '$') {
            nCount ++;
        }
    }
    CHECK_EQ(4, nCount);    // 先頭の "$s1$" が2つ + 区切り2つ
}

//////////////////////////////////////////////////////////////////////
// 4. 不正な入力を弾く
//////////////////////////////////////////////////////////////////////

TEST(不正_ハッシュ形式でない値は拒否する)
{
    CHECK(PasswordHash::IsHashed("plaintext") == false);
    CHECK(PasswordHash::Verify("plaintext", "plaintext") == false);
    CHECK(PasswordHash::Verify("", "") == false);
    CHECK(PasswordHash::Verify("$s2$1$c2FsdA==$AAAA", "password") == false);
}

TEST(不正_壊れた保存値は拒否する)
{
    // 反復回数が数字でない
    CHECK(PasswordHash::Verify("$s1$abc$c2FsdA==$Eg+2z/z4syxD5yJSVsT4N6hlSMkszDVICAWYfLcL4Xs=", "password") == false);
    // 反復回数が0
    CHECK(PasswordHash::Verify("$s1$0$c2FsdA==$Eg+2z/z4syxD5yJSVsT4N6hlSMkszDVICAWYfLcL4Xs=", "password") == false);
    // 区切りが足りない
    CHECK(PasswordHash::Verify("$s1$1$c2FsdA==", "password") == false);
    // hash 長が 32 バイトでない
    CHECK(PasswordHash::Verify("$s1$1$c2FsdA==$AAAA", "password") == false);
}

//////////////////////////////////////////////////////////////////////
// 5. 使用できる文字の制限
//
// 全角文字は PasswordHash が生バイトを扱う都合上、呼び出し側の
// エンコーディング次第で同じパスワードが別のハッシュになりうる。
// そのため新規作成・変更時は ASCII 表示可能文字(0x21〜0x7E)のみ許可する。
//
// **既存アカウントの照合には掛けないこと。** 過去に全角で登録された
// アカウントが在るかはハッシュからは判別できず、掛けると締め出しになる。
//////////////////////////////////////////////////////////////////////

TEST(文字種_半角英数字と記号は使える)
{
    CHECK(PasswordHash::IsAcceptable("abc123") != false);
    CHECK(PasswordHash::IsAcceptable("Passw0rd!") != false);
    CHECK(PasswordHash::IsAcceptable("!\"#$%&'()*+,-./") != false);
    CHECK(PasswordHash::IsAcceptable(":;<=>?@[\\]^_`{|}~") != false);
    CHECK(PasswordHash::IsAcceptable("a") != false);
}

TEST(文字種_全角は使えない)
{
    CHECK(PasswordHash::IsAcceptable("ぱすわーど") == false);
    CHECK(PasswordHash::IsAcceptable("ＰＡＳＳ") == false);      // 全角英字
    CHECK(PasswordHash::IsAcceptable("１２３４") == false);      // 全角数字
    CHECK(PasswordHash::IsAcceptable("pass日本語") == false);    // 混在
    CHECK(PasswordHash::IsAcceptable("パスワード") == false);    // 全角カナ
}

TEST(文字種_空白と制御文字は使えない)
{
    CHECK(PasswordHash::IsAcceptable("") == false);
    CHECK(PasswordHash::IsAcceptable(" ") == false);
    CHECK(PasswordHash::IsAcceptable("pass word") == false);    // 途中の空白
    CHECK(PasswordHash::IsAcceptable("pass\tword") == false);
    CHECK(PasswordHash::IsAcceptable("pass\nword") == false);
    CHECK(PasswordHash::IsAcceptable("\x7F") == false);          // DEL
    CHECK(PasswordHash::IsAcceptable(NULL) == false);
}

TEST(文字種_既存の照合には影響しない)
{
    // 全角パスワードで作られたハッシュでも Verify は通る。
    // IsAcceptable() を Verify に掛けてしまうと既存ユーザーを締め出す。
    const char *pszPassword = "ぱすわーど";
    const char *pszStoredUtf8 =
        "$s1$1000$c2FsdHNhbHRzYWx0c2FsdA==$Y88vion+oDahPbuB/ZqBjSxW5glOQQ5Ebif0KSyahCU=";

    CHECK(PasswordHash::IsAcceptable(pszPassword) == false);
    CHECK(PasswordHash::Verify(pszStoredUtf8, pszPassword) != false);
}

TEST(不正_NULLを渡しても落ちない)
{
    CHECK(PasswordHash::Hash(NULL).empty() != false);
    CHECK(PasswordHash::Verify(NULL, "password") == false);
    CHECK(PasswordHash::Verify("$s1$1$c2FsdA==$AAAA", NULL) == false);
    CHECK(PasswordHash::IsHashed(NULL) == false);
}
