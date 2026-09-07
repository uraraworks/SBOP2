/// @file TestPasswordHash.cpp
/// @brief パスワードハッシュのテスト
/// @copyright Copyright(C)URARA-works
///
/// 実装は RFC 6234 (SHA-256) / RFC 2104 (HMAC) / RFC 8018 (PBKDF2) 準拠の
/// 自前実装(外部ライブラリ・OS API 不使用)。以前は Windows CNG
/// (bcrypt.h) の BCryptDeriveKeyPBKDF2 を使っていたが、非Windows 移植の
/// ため差し替えた。**既存アカウントのハッシュと互換でなければログインが
/// 全滅する**ため、差し替え前の CNG 実装が実際に生成した値をそのまま
/// 固定テストベクタとして残してある(「後方互換」節)。
///
/// このテストの役割は4つ。
///
/// 1. SHA-256 / HMAC-SHA256 の内部実装が、外部の公開テストベクタどおり
///    であることを確認する(RFC 6234 / RFC 4231)。
///
/// 2. 現在の実装が標準の PBKDF2-HMAC-SHA256 どおりであることを、
///    外部の公開テストベクタで証明する。ここが通れば、規格に準拠した
///    任意の実装(OpenSSL / mbedTLS 等)が自動的に互換になる。
///    総当たりで一致を確かめる必要は無い。
///
/// 3. 差し替え前の CNG 実装が生成した値を新実装が Verify できることを
///    確認する(後方互換の直接の証拠)。
///
/// 4. 平文パスワードのバイト列が UTF-8 であることを固定する。
///    PasswordHash は変換を一切せず生バイトを渡すため、呼び出し側が
///    CP932 を渡すと同じパスワードでも別のハッシュになる。
///    実装の移植よりもこちらの方が現実的なリスク。

#include "StdAfx.h"
#include "TestFramework.h"
#include "PasswordHash.h"
#include <string>

//////////////////////////////////////////////////////////////////////
// 0. SHA-256 / HMAC-SHA256 単体の公開テストベクタ
//
// PasswordHash::Sha256Hex() / HmacSha256Hex() はテスト専用に公開している
// 内部関数。ブロック境界(55/56/63/64/65バイト)をまたぐケースは
// 実装バグが出やすいので必ず含める。
//////////////////////////////////////////////////////////////////////

TEST(SHA256_空文字列)
{
    CHECK(PasswordHash::Sha256Hex((const unsigned char *)"", 0)
        == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

TEST(SHA256_abc)
{
    CHECK(PasswordHash::Sha256Hex((const unsigned char *)"abc", 3)
        == "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
}

TEST(SHA256_境界_55バイト)
{
    // 1ブロック(64バイト)に 0x80 + 8バイト長 が収まる最大長
    std::string strMsg(55, 'a');
    CHECK(PasswordHash::Sha256Hex((const unsigned char *)strMsg.data(), strMsg.size())
        == "9f4390f8d30c2dd92ec9f095b65e2b9ae9b0a925a5258e241c9f1e910f734318");
}

TEST(SHA256_境界_56バイト)
{
    // 55バイトの次。パディングが2ブロック目に溢れる最小長
    std::string strMsg(56, 'a');
    CHECK(PasswordHash::Sha256Hex((const unsigned char *)strMsg.data(), strMsg.size())
        == "b35439a4ac6f0948b6d6f9e3c6af0f5f590ce20f1bde7090ef7970686ec6738a");
}

TEST(SHA256_境界_63バイト)
{
    std::string strMsg(63, 'a');
    CHECK(PasswordHash::Sha256Hex((const unsigned char *)strMsg.data(), strMsg.size())
        == "7d3e74a05d7db15bce4ad9ec0658ea98e3f06eeecf16b4c6fff2da457ddc2f34");
}

TEST(SHA256_境界_64バイト)
{
    // ちょうど1ブロック分。パディングは丸ごと2ブロック目になる
    std::string strMsg(64, 'a');
    CHECK(PasswordHash::Sha256Hex((const unsigned char *)strMsg.data(), strMsg.size())
        == "ffe054fe7ae0cb6dc65c3af9b61d5209f439851db43d0ba5997337df154668eb");
}

TEST(SHA256_境界_65バイト)
{
    std::string strMsg(65, 'a');
    CHECK(PasswordHash::Sha256Hex((const unsigned char *)strMsg.data(), strMsg.size())
        == "635361c48bb9eab14198e76ea8ab7f1a41685d6ad62aa9146d301d4f17eb0ae0");
}

TEST(SHA256_複数ブロックにまたがる長いメッセージ)
{
    const char *pszMsg =
        "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno"
        "ijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
    CHECK_EQ(112, (int)strlen(pszMsg));
    CHECK(PasswordHash::Sha256Hex((const unsigned char *)pszMsg, strlen(pszMsg))
        == "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1");
}

//////////////////////////////////////////////////////////////////////
// 0-2. HMAC-SHA256 (RFC 4231 公開テストベクタ)
//////////////////////////////////////////////////////////////////////

TEST(HMACSHA256_RFC4231_TC1)
{
    unsigned char pKey[20];
    memset(pKey, 0x0b, sizeof(pKey));
    const char *pszData = "Hi There";
    CHECK(PasswordHash::HmacSha256Hex(pKey, sizeof(pKey), (const unsigned char *)pszData, strlen(pszData))
        == "b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7");
}

TEST(HMACSHA256_RFC4231_TC2_鍵が短い文字列)
{
    const char *pszKey = "Jefe";
    const char *pszData = "what do ya want for nothing?";
    CHECK(PasswordHash::HmacSha256Hex((const unsigned char *)pszKey, strlen(pszKey), (const unsigned char *)pszData, strlen(pszData))
        == "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843");
}

TEST(HMACSHA256_RFC4231_TC3)
{
    unsigned char pKey[20];
    memset(pKey, 0xaa, sizeof(pKey));
    unsigned char pData[50];
    memset(pData, 0xdd, sizeof(pData));
    CHECK(PasswordHash::HmacSha256Hex(pKey, sizeof(pKey), pData, sizeof(pData))
        == "773ea91e36800e46854db8ebd09181a72959098b3ef8c122d9635514ced565fe");
}

TEST(HMACSHA256_RFC4231_TC6_鍵がブロック長より長い)
{
    // 鍵が64バイト(SHA-256のブロック長)より長い(131バイト)ケース。
    // HMAC の仕様上、こういう鍵は先にハッシュして短縮してから使う必要が
    // あり、この前処理を間違える実装が多い。
    unsigned char pKey[131];
    memset(pKey, 0xaa, sizeof(pKey));
    const char *pszData = "Test Using Larger Than Block-Size Key - Hash Key First";
    CHECK(PasswordHash::HmacSha256Hex(pKey, sizeof(pKey), (const unsigned char *)pszData, strlen(pszData))
        == "60e431591ee0b67f0d8a26aacbf5b77f8e0bc6213728c5140546040f0ee37f54");
}

TEST(HMACSHA256_RFC4231_TC7_鍵とデータが共にブロック長より長い)
{
    unsigned char pKey[131];
    memset(pKey, 0xaa, sizeof(pKey));
    const char *pszData =
        "This is a test using a larger than block-size key and a larger "
        "than block-size data. The key needs to be hashed before being "
        "used by the HMAC algorithm.";
    CHECK(PasswordHash::HmacSha256Hex(pKey, sizeof(pKey), (const unsigned char *)pszData, strlen(pszData))
        == "9b09ffa71b942fcb27635fbcd5b0e944bfdc63644f0713938a7f51535c3a35e2");
}

//////////////////////////////////////////////////////////////////////
// 0-3. 後方互換: 差し替え前の CNG 実装が生成した値
//
// 実装差し替え前(Windows CNG / BCryptDeriveKeyPBKDF2)にビルドした
// バイナリで実際に PasswordHash::Hash() を呼んで採取した値。
// 本番DBには同じCNG実装で作られたハッシュが685件あるため、
// 新実装の Verify() がこれらを受け入れることが後方互換の直接の証拠になる。
//////////////////////////////////////////////////////////////////////

TEST(後方互換_CNGが生成した値をVerifyできる_password123)
{
    const char *pszStored =
        "$s1$100000$ceOG2lZ0sansFGiGr/0SbQ==$dLH6ecOwlHb/ayMJug4lba+/xpvH5B/ZJD9zh48z4ac=";
    CHECK(PasswordHash::Verify(pszStored, "password123") != false);
}

TEST(後方互換_CNGが生成した値をVerifyできる_非ASCII)
{
    const char *pszStored =
        "$s1$100000$JKBA9uxdyOo36S4fUWw1yQ==$bLJLA6xnYBqzzmV8JOgR/OiVYp8AIqHLep7/uYI4bZk=";
    CHECK(PasswordHash::Verify(pszStored, "ぱすわーど") != false);
}

TEST(後方互換_CNGが生成した値をVerifyできる_空文字列)
{
    const char *pszStored =
        "$s1$100000$bi/cImuHuUEamQ8LJlWm5w==$KIY1NJ3k+sRp8s4LC5v21DRrnD6nXiiJ9n1/qOTYSfs=";
    CHECK(PasswordHash::Verify(pszStored, "") != false);
}

TEST(後方互換_CNGが生成した値をVerifyできる_記号入り)
{
    const char *pszStored =
        "$s1$100000$QyJ88TNRwRnXLa9wujiO5A==$TiHwDgiKYY6dM6Ity3ZlLelzMO9ZJr6oeSZ3dhEpK5o=";
    CHECK(PasswordHash::Verify(pszStored, "Passw0rd!123") != false);
}

TEST(後方互換_CNGが生成した値はパスワードが違えば一致しない)
{
    const char *pszStored =
        "$s1$100000$ceOG2lZ0sansFGiGr/0SbQ==$dLH6ecOwlHb/ayMJug4lba+/xpvH5B/ZJD9zh48z4ac=";
    CHECK(PasswordHash::Verify(pszStored, "password124") == false);
}

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
