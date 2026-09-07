/// @file TestWebSocketProtocol.cpp
/// @brief WebSocket 用 SHA-1 / ハンドシェイクのテスト
/// @copyright Copyright(C)URARA-works
///
/// 旧実装は Windows CryptoAPI (wincrypt.h) の SHA-1 を使っていたが、
/// 非Windows でも通る自前実装に置き換えた。
/// このテストの役割は2つ。
///
/// 1. RFC 3174 / FIPS 180-1 の公開テストベクタと照合し、規格どおりの
///    SHA-1 であることを証明する（ブロック境界をまたぐ入力を含む）。
/// 2. RFC 6455 の例で Sec-WebSocket-Accept 計算（SHA-1 + Base64）が
///    ハンドシェイク全体として一致することを確認する。

#include "StdAfx.h"
#include "TestFramework.h"
#include "WebSocketProtocol.h"
#include <string>

//////////////////////////////////////////////////////////////////////
// 1. SHA-1 公開テストベクタとの照合
//////////////////////////////////////////////////////////////////////

TEST(SHA1_空文字列)
{
    CHECK(WebSocketProtocol::Sha1Hex("") == "da39a3ee5e6b4b0d3255bfef95601890afd80709");
}

TEST(SHA1_abc)
{
    // FIPS 180-1 のサンプル
    CHECK(WebSocketProtocol::Sha1Hex("abc") == "a9993e364706816aba3e25717850c26c9cd0d89d");
}

TEST(SHA1_448bit境界)
{
    // 448bit(56バイト) ちょうど。パディングが2ブロック目に押し出される境界。
    const char *pszMsg = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
    CHECK(WebSocketProtocol::Sha1Hex(pszMsg) == "84983e441c3bd26ebaae4aa1f95129e5e54670f1");
}

TEST(SHA1_ブロック境界55バイト)
{
    // 55バイト: パディングの0x80がぎりぎり同一ブロックに収まる境界
    std::string strMsg(55, 'a');
    CHECK(WebSocketProtocol::Sha1Hex(strMsg) == "c1c8bbdc22796e28c0e15163d20899b65621d65a");
}

TEST(SHA1_ブロック境界56バイト)
{
    // 56バイト: 0x80を足すと64バイトを超え、パディングが2ブロック目に押し出される境界
    std::string strMsg(56, 'a');
    CHECK(WebSocketProtocol::Sha1Hex(strMsg) == "c2db330f6083854c99d4b5bfb6e8f29f201be699");
}

TEST(SHA1_ちょうど64バイト)
{
    // 64バイト(1ブロックちょうど): パディングが丸々2ブロック目
    std::string strMsg(64, 'a');
    CHECK(WebSocketProtocol::Sha1Hex(strMsg) == "0098ba824b5c16427bd7a1122a5a442a25ec644d");
}

TEST(SHA1_100万個のa)
{
    // FIPS 180-1 のサンプル3。長いメッセージでも複数ブロックを正しく積み上げられるかの確認。
    std::string strMsg(1000000, 'a');
    CHECK(WebSocketProtocol::Sha1Hex(strMsg) == "34aa973cd4c4daa4f61eeb2bdbad27316534016f");
}

//////////////////////////////////////////////////////////////////////
// 2. RFC 6455 の例でハンドシェイク全体を確認
//////////////////////////////////////////////////////////////////////

TEST(WebSocket_RFC6455の例でAcceptキーが一致する)
{
    const std::string strClientKey = "dGhlIHNhbXBsZSBub25jZQ==";
    const std::string strExpected  = "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=";

    CHECK(WebSocketProtocol::ComputeAcceptKey(strClientKey) == strExpected);
}
